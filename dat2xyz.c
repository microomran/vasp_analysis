#include <stdio.h>
#include <stdlib.h>
int main(int argc,char* argv[]){
  FILE* pFile;
  int counter;
  char line[256];
  int i;
  if(argc<2){
    return -1;
  }
  pFile = fopen(argv[1],"r");
  if(pFile==NULL){
    return -1;
  }
  counter = 0;
  while(fgets(line,256,pFile)){
    counter++;
  }
    
  rewind(pFile);
  
  printf("%d\n",counter);
  printf("Generated by dat2xyz\n");
  for(i=0;i<counter;i++){
    double x,y,z;
    fscanf(pFile,"%lf %lf %lf",&x,&y,&z);
    printf("%s\t%7.4f\t%7.4f\t%7.4f\n",argc==3?argv[2]:"Pt",x,y,z);
  }
  return 0;

  
}
