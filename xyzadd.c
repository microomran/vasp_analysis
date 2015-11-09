#include <stdio.h>
#include <stdlib.h>

#define MAX_ATOM_COUNT 500
#define MAX_LINE_LENG 256

typedef struct{
  char element_type[5];
  double x;
  double y;
  double z;
} Atom;



int XyzRead(const char* filename,Atom* atoms,int* pAtomCount){
  int i;
  int atomCount;
  char line[MAX_LINE_LENG];
  char commentLine[MAX_LINE_LENG];
  FILE* pFile = fopen(filename,"r");
  if(pFile==NULL){
    printf("Cannot open %s\n",filename);
    return -1;
  }

  /* Reading original file */
  
  /* First line is the count of atoms */
  fgets(line,MAX_LINE_LENG,pFile);
  sscanf(line,"%d",&atomCount);
  if(atomCount<=0 || atomCount>MAX_ATOM_COUNT){
    printf("Atom count %d is wrong\n",atomCount);
    fclose(pFile);
    return -1;
  }

  /* Second line is comment */
  fgets(commentLine,MAX_LINE_LENG,pFile); 
  
  /* Following lines are atom coordinates */
  for(i=0;i<atomCount;i++){  
    if( ! fgets(line,MAX_LINE_LENG,pFile) ){
      printf("File format not correct\n");
      fclose(pFile);
      return -1;
    }
    sscanf(line,"%5s %lf %lf %lf",
	   &atoms[i].element_type,
	   &atoms[i].x,
	   &atoms[i].y,
	   &atoms[i].z);
  }


  *pAtomCount = atomCount;
   
  fclose(pFile);
  return 0;
}

int AtomsAdd(Atom* atoms1,Atom* atoms2,int count){
  int i;
  for(i=0;i<count;i++){
    atoms1[i].x = atoms1[i].x + atoms2[i].x;
    atoms1[i].y = atoms1[i].y + atoms2[i].y;
    atoms1[i].z = atoms1[i].z + atoms2[i].z;
  }
  return 0;
}

int AtomsShow(Atom* atoms,int atomCount){
  int i;
  printf("%d\n",atomCount);
  printf("%s","commentLine\n");
  for(i=0;i<atomCount;i++){
    printf("%s  %19.15f  %19.15f  %19.15f\n",
	   atoms[i].element_type,
	   atoms[i].x,
	   atoms[i].y,
	   atoms[i].z);
  }
  return 0;
}

int main(int argc,char* argv[]){
  Atom atoms1[MAX_ATOM_COUNT];
  Atom atoms2[MAX_ATOM_COUNT];
  
  if(argc!=3){
    printf("Usage: xyzadd xyzfile1 xyzfile2\n");
    return -1;
  }
  else{
    int count1,count2;
    if ( XyzRead(argv[1],atoms1,&count1)!=0 ||  XyzRead(argv[2],atoms2,&count2)!=0 ){
      return -1;
    }
    if(count1 != count2){
      printf("Atom count in two files does not match!\n");
      return -1;
    }
    
    AtomsAdd(atoms1,atoms2,count1);

    AtomsShow(atoms1,count1);
  }
}
