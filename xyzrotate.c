#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ATOM_COUNT 500
#define MAX_LINE_LENG 256

typedef struct{
  char element_type[5];
  double x;
  double y;
  double z;
} Atom;

Atom atoms[MAX_ATOM_COUNT];

int MatrixMultiply(double m1[3][3],double m2[3][3],double result[3][3]){
  int i,j,k;
  for(i=0;i<3;i++){
    for(j=0;j<3;j++){
      result[i][j] = 0.0;
      for(k=0;k<3;k++){
	result[i][j] += m1[i][k]*m2[k][j];
      }
    }
  }
  return 0;
}

int ConstructRotationMatrix(double alpha, double beta, double gamma,double RMatrix[3][3]){
  double m1[3][3] = {
    {1,                   0,          0},
    {0,          cos(alpha), sin(alpha)},
    {0,         -sin(alpha), cos(alpha)}};
  double m2[3][3] = {
    { cos(beta),          0, sin(beta)},
    {0,                   1,         0},
    {-sin(beta),          0, cos(beta)}};
  double m3[3][3] = {
    { cos(gamma),sin(gamma),         0},
    {-sin(gamma),cos(gamma),         0},
    {0          ,         0,         1}};
  double temp[3][3];
  MatrixMultiply(m1,m2,temp);
  MatrixMultiply(temp,m3,RMatrix);
  return 0;
}

double DegToRad(double degree){
  return degree/180.0*3.14159265358979324;
}

int Xyzmove(const char* filename,double alpha,double beta,double gamma){
  int i;
  int atomCount;
  char line[MAX_LINE_LENG];
  char commentLine[MAX_LINE_LENG];
  double rotationMatrix[3][3];
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

  /* Rotation */
  alpha = DegToRad(alpha);
  beta = DegToRad(beta);
  gamma = DegToRad(gamma);
  ConstructRotationMatrix(alpha,beta,gamma,rotationMatrix);
  for(i=0;i<atomCount;i++){
    double oldCoord[3];
    double newCoord[3];
    int s,t;
    oldCoord[0] = atoms[i].x;
    oldCoord[1] = atoms[i].y;
    oldCoord[2] = atoms[i].z;
    for(s=0;s<3;s++){
      newCoord[s] = 0.0;
      for(t=0;t<3;t++){
	newCoord[s] += oldCoord[t]*rotationMatrix[t][s];
      }
    }
    atoms[i].x = newCoord[0];
    atoms[i].y = newCoord[1];
    atoms[i].z = newCoord[2];
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
    printf("Usage: xyzmove xyzfile around_x around_y around_z\n");
    return -1;
  }
  else{
    return Xyzmove(argv[1],atof(argv[2]),atof(argv[3]),atof(argv[4]));
  }
}
