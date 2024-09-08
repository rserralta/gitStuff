
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
int keylen;
//char* key[];
//int srn; 

void encrypt(char* message, int key[keylen], int size){
// adds the message into the matrix
int nRow = (int)ceil((double)size/keylen);
char matrix[nRow][keylen];
int index = 0;
for(int i=0; i<nRow;i++){
  for(int j=0; j<keylen;j++){
    if(message[index]) {
      matrix[i][j] = message[index++];
    }
    else {
      matrix[i][j] = 'z' - (keylen - j - 1);
    }
    //  if(!*p) {
    //   matrix[i][j] = 'z' - (keylen - j - 1);
    //   continue;
    //  }
    //  if(isalpha(message[i])!=0){

    //    if(isupper(*p)){

    //     matrix[i][j] = tolower(*p);

    //     //printf("matrix[%d][%d] = %c\n", j, i, matrix[j][i]);
    
    //   }else{ 
    //     matrix[i][*p];
    //    // printf("matrix[%d][%d] = %c\n", j, i, matrix[j][i]);
    //   }
      
    // }else{
    //   j--;
    // }
    // p++;
  }//for
  //printf("matrix[%d][%d] = %c\n", j, i, matrix[j][i]);
}// for

for(int i=0;i<nRow;i++){
    for(int j=0;j<keylen;j++){
     // printf("%c", tolower(matrix[i][j]));
    }//for
    //printf("\n");
  }//for
// scrambles the message and places it into a new matrixs

// error is happinging around here ------------------------------bdbbmbcn
int key_index = 0;
char scramble[nRow][keylen];
for(int i=0;i<keylen;i++){
  for(int j=0; j<nRow;j++){
    scramble[j][key[key_index] - 1] = matrix[j][i];
  }
  key_index++;

}//for
// prints the scrambled message
  for(int i=0;i<keylen;i++){
    for(int j=0;j<nRow;j++){
      printf("%c", toupper(scramble[j][i]));
    }//for
  }//for
  printf("\n");

}//encrypt
////--------------------------------------------------------------

void decrypt(char* message, int key[keylen], int size){
  int nRow = (int)ceil((double)size/keylen);
  char split[nRow][keylen];
 
  for(int i = 0; i <keylen; i++){
    int currKey = key[i] - 1;

    for (int j = 0; j <nRow; j++){
      int currMessPlace = currKey * nRow + j;
      split[j][i]= message[currMessPlace];
    }
  }
 for(int i=0;i<nRow;i++){
    for(int j=0;j<keylen;j++){
      printf("%c", split[i][j]);
    }//for
    printf("\n");
  }//for
  printf("\n");


}//decrypt

char* clean_message(char* message, int size) {
  char* cleaned = (char*)malloc(sizeof(char) * size);

  int index = 0;
  for(int i = 0; i < size; i++) {
    if(isalpha(message[i])) {
      cleaned[index++] = tolower(message[i]);
    }
  }

  return cleaned;
}

int main(int argc,char*argv[]){// argc tells the lenght, 7 4312567

       
  keylen=atoi(argv[1]);
  
  if(argc < 3){
      printf("wrong format use ./railcipher [key length] [0-9]");
      }
      if(keylen>26||keylen <=0){
      printf("key length must be less then 26");
      }
      if(keylen != argc-2){
      printf("key length and length of key is not the same");
      return -1;

      }//if

    for(int i=1; i<argc;i++){
      
        if(!atoi(argv[i])){
      printf("wrong format use ./railcipher [key length] [0-9]");

        }
    }//for
  
      char* command;
      char* message;
      do{ 
        char scanned[100]; //stringsplit
       
       printf("give a command ");

       
       fgets(scanned,sizeof(scanned),stdin);
      
       //printf("%s", scanned)

       command = strtok(scanned," ");
       message=strtok(NULL,"\n");

       message = clean_message(message, strlen(message));

       int keys[keylen];

       for(int i = 2; i < argc; i++)
        keys[i - 2] = atoi(argv[i]);

       if(strcmp(command, "encrypt")==0){
        encrypt(message,keys, strlen(message));

       }else if(strcmp(command, "decrypt")==0 ){// the command is decrypt 
        
          decrypt(message,keys, strlen(message));


       }else {
        printf("%s","incorrect format");

       }



      } while(strcmp(command, "quit")!=0);


        


}//main


