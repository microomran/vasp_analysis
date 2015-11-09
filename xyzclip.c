#include <stdio.h>
#include <stdlib.h>

#define MAX_ATOM_COUNT 2500
#define MAX_LINE_LENG 256

typedef struct{
  char element_type[5];
  double x;
  double y;
  double z;
} Atom;

Atom atoms[MAX_ATOM_COUNT];

int Xyzmove(const char* filename,double z_max, double z_min){
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

  /* Translation */
  int newCount = 0;
  for(i=0;i<atomCount;i++){
    if(atoms[i].z > z_min && atoms[i].z < z_max){
      newCount++;
    }
  }

  /* Output */
  printf("%d\n",newCount);
  printf("%s",commentLine);
  for(i=0;i<atomCount;i++){
    if(atoms[i].z > z_min && atoms[i].z < z_max){
      printf("%s  %19.15f  %19.15f  %19.15f\n",
	     atoms[i].element_type,
	     atoms[i].x,
	     atoms[i].y,
	     atoms[i].z);
    }
  }
  
  fclose(pFile);
  return 0;
}

int main(int argc,char* argv[]){
  if(argc!=4){
    printf("Usage: xyzmove xyzfile max_z min_z\n");
    return -1;
  }
  else{
    return Xyzmove(argv[1],atof(argv[2]),atof(argv[3]));
  }
}
