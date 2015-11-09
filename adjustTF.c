#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_LENG 512
#define MAX_ATOM_COUNT 1000

int AdjFree(FILE* pPOSCAR,FILE* pTF){
  int i;
  char delim;
  char* pos;
  int atomCountInPOSCAR;
  int atomCountInTFFile;
  char line[MAX_LINE_LENG];
  char word[MAX_LINE_LENG];
  char tOrF[MAX_ATOM_COUNT];


  atomCountInTFFile = 0;
  while(fscanf(pTF,"%s",word)!=EOF){
    char status = word[0];
    int count;
    if(strlen(word)!=1){
      sscanf(word+1,"%d",&count);
    }
    else{
      count = 1;
    }
    for(i=atomCountInTFFile; i<atomCountInTFFile+count; i++){
      tOrF[i] = status;
    }
    atomCountInTFFile += count;
  }
  //for(i=0;i<atomCountInTFFile;i++){
  //printf("%c ",tOrF[i]);
  //}
  //printf("\n");

  for(i=0;i<6;i++){
    fgets(line,MAX_LINE_LENG,pPOSCAR);
    printf("%s",line);
  }

  atomCountInPOSCAR = 0;
  pos = strtok(line," \r\n\t");
  do{
    sscanf(pos,"%d",&i);
    atomCountInPOSCAR += i;
    pos = strtok(NULL," \r\n\t");
  }while(pos!=NULL);


  for(i=0;i<2;i++){
    fgets(line,MAX_LINE_LENG,pPOSCAR);
    printf("%s",line);
  }

  if(atomCountInPOSCAR != atomCountInTFFile){
    printf("Atom count in POSCAR and atom count in T/F file do not match: %d vs %d\n",
	   atomCountInPOSCAR, atomCountInTFFile);
    return -1;
  }
  
  for(i=0;i<atomCountInTFFile;i++){
    int j;
    for(j=0;j<3;j++){
      fscanf(pPOSCAR,"%s",word);
      printf("  %s",word);
    }
    for(j=0;j<3;j++){
      printf("  %c",tOrF[i]);
    }
    fgets(line,MAX_LINE_LENG,pPOSCAR);
    printf("\n");
  }

  return 0;
}

int main(int argc,char* argv[]){
  FILE* pPOSCAR;
  FILE* pTF;
  int result;
  if(argc!=3){
    printf("Usage: adjfree POSCARfile T/Ffile\n");
    return -1;
  }
  pPOSCAR = fopen(argv[1],"r");
  pTF = fopen(argv[2],"r");
  if(pPOSCAR==NULL || pTF==NULL){
    printf("Cannot open %s or %s\n",argv[1],argv[2]);
    return -1;
  }
  
  result = AdjFree(pPOSCAR,pTF);
  
  fclose(pPOSCAR);
  fclose(pTF);
  return result;
}



