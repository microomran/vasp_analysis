#include <stdio.h>
#include <stdlib.h>
int main(int argc,char* argv[]){
  int i;
  int totalAtomCount = 0;
  char command[256];
  if(argc==1){
    return 0;
  }
  system("echo \"created by xyzcat\" > temp_by_xyzcat");
  for(i=1;i<argc;i++){
    int count;
    FILE* pFile = fopen(argv[i],"r");
    fscanf(pFile,"%d",&count);
    fclose(pFile);
    sprintf(command,"tail -n+3 %s >> temp_by_xyzcat",argv[i]);
    system(command);
    totalAtomCount += count;
  }
  sprintf(command, "echo %d",totalAtomCount);
  system(command);
  system("cat temp_by_xyzcat");
  system("rm temp_by_xyzcat");
  return 0;
}
  
