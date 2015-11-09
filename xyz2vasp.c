/* This program transform a .xyz file into a POSCAR format file
* It takes two files as input. The first file is the .xyz file which needs to be transformed.
* For example, we have a file named "slab.xyz":
20
CO on Cu
Cu   -0.010589244392532    0.011460623044236    0.082917756392755
Cu    0.015473125344320    2.635332395524031   -0.013037687709541
Cu    2.133090950785867    1.324597432063087   -0.694493964289238
Cu    2.132394666403729    3.944091388300119   -0.693934206557480
Cu    4.256809428272725    0.012320055111634   -1.402298992898712
Cu    4.261050474006291    2.636159142422932   -1.392333695276098
Cu   -0.023149899367056    1.325431699049147   -2.164764096517645
Cu   -0.023149899367056    3.948797857251238   -2.164764096517645
Cu    2.118819577906760    0.013748619948101   -2.922064733505858
Cu    2.118819577906760    2.637114778150192   -2.922064733505858
Cu    4.260789119439663    1.325431699049147   -3.679365188741932
Cu    4.260789119439663    3.948797857251238   -3.679365188741932
Cu   -0.023149899367056    0.013748619948101   -4.436665825730144
Cu   -0.023149899367056    2.637114778150192   -4.436665825730144
Cu    2.118819577906760    1.325431699049147   -5.193966462718357
Cu    2.118819577906760    3.948797857251238   -5.193966462718357
Cu    4.260789119439663    0.013748619948101   -5.951266917954432
Cu    4.260789119439663    2.637114778150192   -5.951266917954432
C   -0.052877044153315    0.007734998181751    1.959930139777441
O    0.004106815642364    0.002485234355787    3.126225831452322

* The second input file which defines the surface vectors should have the POSCAR format
* For example, we have a file named "surface":
Surface vectors defined here
 3.71
   1.73205081  0.0         0.0
   0.0         1.41421356  0.0
   0.0         0.0         5.0
* All remaining line of this file are discarded by the program

* Suppose we have compiled this program:
gcc xyz2vasp.c -o xyz2vasp
* And run the program:
./xyz2vasp slab.xyz surface
* Then we shall get the results on standard output:
Cu C O
  3.710000
  1.732050810  0.000000000  0.000000000
  0.000000000  1.414213560  0.000000000
  0.000000000  0.000000000  5.000000000
  18  1  1
Selective Dynamics
Fractional
  -0.001647898  0.002184335  0.004469960  T  T  T
  0.002407928  0.502280704  -0.000702840  T  T  T
  0.331951653  0.252461409  -0.037439028  T  T  T
  0.331843297  0.751723388  -0.037408852  T  T  T
  0.662444762  0.002348139  -0.075595633  T  T  T
  0.663104753  0.502438277  -0.075058420  T  T  T
  -0.003602588  0.252620416  -0.116698873  T  T  T
  -0.003602588  0.752620417  -0.116698873  T  T  T
  0.329730742  0.002620416  -0.157523705  T  T  T
  0.329730742  0.502620417  -0.157523705  T  T  T
  0.663064081  0.252620416  -0.198348528  T  T  T
  0.663064081  0.752620417  -0.198348528  T  T  T
  -0.003602588  0.002620416  -0.239173360  T  T  T
  -0.003602588  0.502620417  -0.239173360  T  T  T
  0.329730742  0.252620416  -0.279998192  T  T  T
  0.329730742  0.752620417  -0.279998192  T  T  T
  0.663064081  0.002620416  -0.320823014  T  T  T
  0.663064081  0.502620417  -0.320823014  T  T  T
  -0.008228727  0.001474251  0.105656611  T  T  T
  0.000639103  0.000473673  0.168529694  T  T  T

* What the program is doing is calculating every atom's position under the coordinate system defined by 
* {V1,V2,V3} given in "surface", and outputing their fractional coordinates.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ATOM_COUNT 10000
#define MAX_LINE_LENG 256
#define PRECISION 9

typedef struct{
  char element_type[5];
  double x;
  double y;
  double z;
} Atom;

  
int AtomShow(Atom* pAtom){
  printf("%.*f  %.*f  %.*f",
	 PRECISION,pAtom->x,
	 PRECISION,pAtom->y,
	 PRECISION,pAtom->z);
  return 0;
}

double Reminant(double a[3][3],int i, int j){
	double A = a[(i-1+3)%3][(j-1+3)%3];
	double B = a[(i-1+3)%3][(j+1+3)%3];
	double C = a[(i+1+3)%3][(j-1+3)%3];
	double D = a[(i+1+3)%3][(j+1+3)%3];
	return A*D - B*C;
}

int MatrixReverse(double a[3][3], double rev[3][3]){
	/* Reverse of a 3*3 matrix */
	int i,j;
	double determinant = 
		+ a[0][0]*a[1][1]*a[2][2]
		+ a[0][1]*a[1][2]*a[2][0]
		+ a[0][2]*a[1][0]*a[2][1]
		- a[0][0]*a[1][2]*a[2][1]
		- a[0][1]*a[1][0]*a[2][2]
		- a[0][2]*a[1][1]*a[2][0];
	for(i=0;i<3;i++){
		for(j=0;j<3;j++){
			rev[j][i] = Reminant(a,i,j)/determinant;
		}
	}
	return 0;
}


int ReadXyzFile(const char* filename,Atom* atoms,int* pAtomCount){
  int i;
  int atomCount;
  char line[MAX_LINE_LENG];

  FILE* pFile = fopen(filename,"r");
  if(pFile==NULL){
    printf("Cannot open %s\n",filename);
    return -1;
  }

  /* Reading xyz file */
  
  /* First line is the count of atoms */
  fgets(line,MAX_LINE_LENG,pFile);
  sscanf(line,"%d",&atomCount);
  if(atomCount<=0 || atomCount>MAX_ATOM_COUNT){
    printf("Atom count %d is wrong\n",atomCount);
    fclose(pFile);
    return -1;
  }
  
  /* Second line is comment */
  fgets(line,MAX_LINE_LENG,pFile);
  
  /* Following lines are atom coordinates */
  for(i=0;i<atomCount;i++){  
    if( ! fgets(line,MAX_LINE_LENG,pFile) ){
      printf("File format not correct\n");
      fclose(pFile);
      return -1;
    }
    sscanf(line,"%4s %lf %lf %lf",
	   &atoms[i].element_type,
	   &atoms[i].x,
	   &atoms[i].y,
	   &atoms[i].z);
  }

  *pAtomCount = atomCount;

  fclose(pFile);
  return 0;
}

int ReadSurfaceVectorFile(const char* filename,Atom* surfaceVectors,double* pUnitVector){
  int i;
  FILE* pFile = fopen(filename,"r");
  char line[MAX_LINE_LENG];

  if(pFile==NULL){
    printf("Cannot open %s\n",filename);
    return -1;
  }
  
  /* First line is a comment */
  fgets(line,MAX_LINE_LENG,pFile);

  /* Second line is the unit length */
  fgets(line,MAX_LINE_LENG,pFile);
  sscanf(line,"%lf",pUnitVector);

  /* The following three lines are surface vectors */
  for(i=0;i<3;i++){
    fgets(line,MAX_LINE_LENG,pFile);
    sscanf(line,"%lf %lf %lf",
	   &surfaceVectors[i].x,
	   &surfaceVectors[i].y,
	   &surfaceVectors[i].z);
  }
  fclose(pFile);
  return 0;
}

int Transform(int atomCount,Atom* atoms,double unitLength,Atom* surfaceVectors){
  int i;
  double matrixOfSurfVectors[3][3];
  double rev[3][3];
  for(i=0;i<3;i++){
	matrixOfSurfVectors[i][0] = surfaceVectors[i].x;
	matrixOfSurfVectors[i][1] = surfaceVectors[i].y;
	matrixOfSurfVectors[i][2] = surfaceVectors[i].z;
  }
  MatrixReverse(matrixOfSurfVectors,rev);
  
  /*
  for(i=0;i<3;i++){
	  printf("%lf\t%lf\t%lf\n",rev[i][0],rev[i][1],rev[i][2]);
  }
  */
  

  /* For every atom, calculate its coordinates under [0], V[1] and V[2]*/
  for(i=0;i<atomCount;i++){
    double x = atoms[i].x*rev[0][0] + atoms[i].y*rev[1][0]+atoms[i].z*rev[2][0];
    double y = atoms[i].x*rev[0][1] + atoms[i].y*rev[1][1]+atoms[i].z*rev[2][1];
    double z = atoms[i].x*rev[0][2] + atoms[i].y*rev[1][2]+atoms[i].z*rev[2][2];
    atoms[i].x = x/unitLength;
    atoms[i].y = y/unitLength;
    atoms[i].z = z/unitLength;
  }
  return 0;
}

int Output(int atomCount,Atom* atoms,double unitLength,Atom* surfaceVectors){
  int i,j;
  int elementTypeCount;
  char elementTypes[MAX_ATOM_COUNT][5];
  int elementCount[MAX_ATOM_COUNT];

  /* Count atoms of each type */
  elementTypeCount = 0;
  memset(elementCount,0,sizeof(int)*MAX_ATOM_COUNT);
  for(i=0;i<atomCount;i++){
    /* If type of current atom is different from previous one, consider it as new type */
    int lastIndex = elementTypeCount - 1;
    if(elementCount==0 || strcmp(elementTypes[lastIndex],atoms[i].element_type)!=0 ){ /* New Type Found */
      strcpy(elementTypes[elementTypeCount],atoms[i].element_type);
      elementCount[elementTypeCount] = 1;
      elementTypeCount++;
    }
    else{
      elementCount[lastIndex]++;
    }
  }

  /* Output elements names in this slab */
  for(i=0;i<elementTypeCount;i++){
	  printf("%s ",elementTypes[i]);
  }
  printf("\n");

  /* Unit length and surface vectors */
  printf(" %*f\n",PRECISION,unitLength);
  for(i=0;i<3;i++){
    printf("  ");
    AtomShow(&surfaceVectors[i]);
    printf("\n");
  }

  /* Atom count of each element type */
  for(i=0;i<elementTypeCount;i++){
	  printf("  %d",elementCount[i]);
  }
  printf("\n");

  printf("Selective Dynamics\n");
  printf("Fractional\n");

  /* Output atoms by order */
  for(i=0;i<atomCount;i++){
    printf("  ");
    AtomShow(&atoms[i]);
    printf("  T  T  T\n");
    
  }
  
  return 0;
}

int main(int argc,char* argv[]){
  if(argc!=3){
    printf("Usage: xyz2vasp xyz_file surface_vector_file\n");
    return -1;
  }
  else{
    int atomCount;
    Atom atoms[MAX_ATOM_COUNT];
    Atom surfaceVectors[3];
    double unitLength;

    ReadXyzFile(argv[1],atoms,&atomCount);
    ReadSurfaceVectorFile(argv[2],surfaceVectors,&unitLength);
    Transform(atomCount,atoms,unitLength,surfaceVectors);
    Output(atomCount,atoms,unitLength,surfaceVectors);

    return 0;
  }
}
