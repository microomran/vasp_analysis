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

Atom atoms[MAX_ATOM_COUNT];

int Xyzmove(const char* filename,double new_x,double new_y,double new_z){
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
  if(atomCount<0 || atomCount>MAX_ATOM_COUNT){
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
  for(i=0;i<atomCount;i++){
    atoms[i].x -= new_x;
    atoms[i].y -= new_y;
    atoms[i].z -= new_z;
  }

  /* Output */
  printf("%d\n",atomCount);
  printf("%s",commentLine);
  for(i=0;i<atomCount;i++){
    printf("%s  %19.15f  %19.15f  %19.15f\n",
	   atoms[i].element_type,
	   atoms[i].x,
	   atoms[i].y,
	   atoms[i].z);
  }
  
  fclose(pFile);
  return 0;
}

int main(int argc,char* argv[]){
  if(argc!=5){
    printf("Usage: xyzmove xyzfile new_x new_y new_z\n");
    return -1;
  }
  else{
    return Xyzmove(argv[1],atof(argv[2]),atof(argv[3]),atof(argv[4]));
  }
}
