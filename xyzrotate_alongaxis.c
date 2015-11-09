#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ATOM_COUNT 5000
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

int ConstructRotationMatrix(double rx, double ry, double rz, double phi,double RMatrix[3][3]){
  double c = cos(phi);
  double s = sin(phi);
  RMatrix[0][0] = c + (1-c)*rx*rx;
  RMatrix[0][1] = (1-c)*rx*ry - rz*s;
  RMatrix[0][2] = (1-c)*rx*rz + ry*s;
  RMatrix[1][0] = (1-c)*rx*ry + rz*s;
  RMatrix[1][1] = c + (1-c)*ry*ry;
  RMatrix[1][2] = (1-c)*ry*rz - rx*s;
  RMatrix[2][0] = (1-c)*rx*rz - ry*s;
  RMatrix[2][1] = (1-c)*ry*rz + rx*s;
  RMatrix[2][2] = c+(1-c)*rz*rz;
  return 0;
}

double DegToRad(double degree){
	return degree/180.0*3.14159265358979324;
}

int Xyzmove(const char* filename,double axisX,double axisY, double axisZ, double degree){
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
  double phi = DegToRad(degree);
  double norm = sqrt(axisX*axisX + axisY*axisY + axisZ*axisZ);
  double rx = axisX / norm;
  double ry = axisY / norm;
  double rz = axisZ / norm;
  ConstructRotationMatrix(rx,ry,rz,phi,rotationMatrix);
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
  if(argc!=6){
    printf("Usage: xyzrotate_alongaxis XYZfile, axis_x axis_y axis_z degreeAlongAxis\n");
    return -1;
  }
  else{
    return Xyzmove(argv[1],atof(argv[2]),atof(argv[3]),atof(argv[4]),atof(argv[5]));
  }
}
