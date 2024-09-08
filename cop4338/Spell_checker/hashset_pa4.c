#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<ctype.h>
//#include"hash.h"

#define BINS 101
#define MAX_LEN 100
#define BUF_SIZE 60
struct node {
  struct node *next;
  char *value;
};
typedef struct node node;
typedef struct {
  node **table;
  int size;
  int bins;
} hashset;
//-------------------------------------
hashset set_init() {
  hashset h = {(node **)malloc(BINS * sizeof(node *)), 0, BINS};
  for (int i = 0; i < BINS; i++)
    h.table[i] = NULL;
  return h;
}
static int hash(char *key, int bins) {//key: "abcd" = {97, 98, 99, 100, 0}
  unsigned hashval = 0;
  for (int i = 0; i < strlen(key); i++)
    hashval = 31 * hashval + tolower(key[i]);//97* 31^3 + 98 * 31^2 + 99 * 31 + 100
  return hashval % bins;//return an index from 0..bins - 1
}
int search(hashset h, char *val) {
  int index = hash(val, h.bins);
  for (node *iterator = h.table[index]; iterator; iterator = iterator->next)
    if (!strcasecmp(iterator->value, val))
      return 1;
  return 0;
}//-------------------------------------

static void rehash(hashset*);

int insert(hashset *h, char *val) {
  if(search(*h, val)){ // I found the value
	  return 0;  
  }                      // duplicate
  int index = hash(val, h->bins);
  if (h->size >= h->bins){//load factor >= 100%
    rehash(h);
  node *new_element = (node *)malloc(sizeof(node));
  new_element->next = h->table[index];
  new_element->value = strdup(val);
  h->table[index] = new_element;
  h->size++;
  return 1;
  }
}
void print_all(hashset h){
	for(int i = 0; i < h.bins;i++)
		for(node* it = h.table[i]; it;it = it->next)
			printf("%s ", it->value);
      // fflush(stdout);
}
int eliminate(hashset *h, char *val){
	if(!search(*h, val)) // I didn't find the value
		return 0;                        //failed to remove
	int index = hash(val, h->bins);
	node *prev = NULL, *it;
	for(it = h->table[index];it->next;it = it->next)
		if (!strcasecmp(it->value, val))
			break;
		else
			prev = it;
	if(prev)//removed node isn't the head of the linked list!
		prev->next = it->next;
	else
		h->table[index] = it->next;
	free(it);//de-allocate the removed node!
	h->size--;
	return 1;//successful removal!
}
static int next_prime(int min) {
  while (1) {
    int prime = 1;
    for (int div = 2; div <= sqrt(min); div++)
      if (min % div == 0) {//divisible by div
        prime = 0;//not prime
        break;
      }
    if (prime)
      return min;
    else
      min++;
  }
  return min;
}
static void rehash(hashset *h) {
  node **oldtable = h->table;
  int old_bins = h->bins;
  h->table = (node **)malloc(next_prime(2 * h->bins) * sizeof(node *));
  h->size = 0;
  h->bins = next_prime(2 * h->bins);
  for (int i = 0; i < old_bins; i++)
    for (node *it = oldtable[i]; it; it = it->next)
      insert(h, it->value);
}//-------------------------------------------
//char correction_list[1000][60];
//int indx = 0;
//hashset corrections=set_init();

// void add_list(char* correction) {
//     if (indx < 1000) {
//         strncpy(correction_list[indx], correction, 59);
//         correction_list[indx][59] = '\0';
//         indx++;
//     }
// }

// void print_list() {
//     for (int i = 0; i < indx; i++) {
//         printf("%s,\n", correction_list[i]);
//     }
// }

void cmd_r(char*word, hashset hash_set){
  printf("%s",word);
  for(int i=0;i<strlen(word);i++){
    for(char k = 'a'; k<='z'; k++){
      char og_char=word[i];
      word[i]=k;
      if(search(hash_set,word)){//if true
       // add_list(word);
        //insert(&corrections,word);
        printf("%s",word);
      }
      word[i]=og_char;
        
    }
  }
  //add_list("/");

}// end cmd_r-----------------------------------


void cmd_s(char *word, hashset hash_set) {
  printf("%s",word);
  size_t length = strlen(word);
  if (length < 2) return;  // If there's less than two characters, no swap can occur

  for (int i = 0; i < length - 1; i++) {  // Ensure i+1 does not go out of bounds
    char og_char = word[i];
    char og_char2 = word[i + 1];
    word[i] = og_char2;
    word[i + 1] = og_char;

    if (search(hash_set, word)) {  // Check if the swapped word exists in the hash set
      //add_list(word);
      //insert(&corrections,word);
      printf("%s",word);
    }

    // Swap back to restore original word
    word[i] = og_char;
    word[i + 1] = og_char2;
  }
  //add_list("/");
}

void cmd_e(char *word, hashset hash_set) {


  //printf("%s",word);
  size_t length = strlen(word);  
  char *extracted = (char*)malloc(sizeof(char) * (length + 1));  // Allocate enough space for the null terminator
  for (int i = 0; i < length; i++) {
    int e_index = 0;  // Index for 'extracted'
    for (int j = 0; j < length; j++) {
      if (i != j) {
        extracted[e_index++] = word[j];  // Populate 'extracted' skipping 'word[i]'
      }
    }
      extracted[e_index] = '\0';  // Null-terminate the string
      // printf("before search: %s\n", extracted);
      if (search(hash_set, extracted)) {  // If true
        //add_list(extracted);
       // insert(&corrections,extracted);
        printf("%s",extracted);
      }
  }
  // add_list("/");

  free(extracted);  // Free the memory allocated for 'extracted'
}// end cmd_e----------------------------------


int main(int argc, char**argv){// ./programName -d fileName
  // populating the hashset---->

  char* file_path;

  if (strstr(argv[1],"-d")){
    if(argc > 2) {
      file_path=argv[2];//error
    } else {
      printf("Insufficient arguments for -d\n");
      return -1;
    }
  }else{
    file_path= "words.txt";
  }
  hashset set = set_init();
  //printf("Here!");fflush(stdout);
  FILE* words_file = fopen(file_path, "r" /* "w", or "a" */);
  char buffer[BUF_SIZE];
  while(fgets(buffer, BUF_SIZE, words_file)){
      int len = strlen(buffer);
      if(buffer[len-1] == '\n')
          buffer[len-1] = '\0';
      printf("next line: %s\n", buffer);
      insert(&set, buffer);
  }
  fclose(words_file);

  int line = 0;
  char row[MAX_LEN];

  //print_all(set);
  printf("Set size is %d\n", set.size);

  while(fgets(row,MAX_LEN,stdin)){
    line++;
    int len2 = strlen(row);
    // if ( len2 >= 2 && row[len2 - 2] == '\n') {
    //   row[len2 - 2] = '\0'; // Remove the newline character
    // }else{ 
     
      int column = 1;
      char* r_stream = strtok(row," \t\n");
      printf("misspeled word\tLine#\tCol#\tpossible corrections");

      while (r_stream != NULL) {
        printf("Evaluating %s\n", r_stream);fflush(stdout);
        if (!search(set, r_stream)){
          char line_str[4];
          char column_str[4];
          printf("\n%s\t%d\t%d\t",r_stream, line,column);

          // add_list(r_stream);
          // insert(&corrections, r_stream);
         // sprintf(line_str, "%d", line);//doesnt print in councel 
          // add_list(line_str);
          //printf("\nline: %d",line);
          //printf("\ncolumn: %d",column);
          //insert(&correstions,line_str);
          //sprintf(column_str, "%d", column);
          //add_list(column_str);
          //insert(&corrections, column_str);
          if(strstr(argv[3],"r")){
            cmd_r(r_stream,set);
          }
          if(strstr(argv[3],"s")){
            cmd_s(r_stream,set);
          }

          if(strstr(argv[3],"e")){
            cmd_e(r_stream,set);
          }
        }//if
        //corrections[indx]="\0";
        r_stream = strtok(NULL," \t\n");
        column += strlen(r_stream) + 1;
      }//while
    //}//else
        
  //  for (int i = 0; i <= indx; i++) {
  //       printf("%s", correction_list[i]);
   }
  //   print_all(corrections);

}//end main
