#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define MAX_LEN 10000
#define TRUE 1
#define FALSE 0

typedef struct node {
    char* string;
    struct node* next;
} node;

typedef struct bucket {
    char* min_word;
    node* head;
} bucket;

void print_buckets(bucket* buckets) {
    for (int i = 0; buckets[i].min_word != NULL; i++) {
        printf("Bucket %d: ", i);
        node* node_ptr = buckets[i].head;
        while (node_ptr != NULL) {
            printf("%s ", node_ptr->string);
            node_ptr = node_ptr->next;
        }
        printf("\n");
    }
}

void insert_sort(char* wd, bucket* buckets) {
    bucket* current_bucket = buckets;

    int count = 0;
    while ((current_bucket + 1)->min_word != NULL && strcasecmp((current_bucket + 1)->min_word, wd) < 0) {
        current_bucket++;
    }

    node* new_node = malloc(sizeof(node));
    
    new_node->string = strdup(wd);
    new_node->next = current_bucket->head;
    current_bucket->head = new_node;
}

int main(int argc, char* argv[]) {
    
    if (argc <= 2) {
        printf("ERROR MESSAGE: at least 2 buckets needed\n");
        return TRUE;
    }

    printf("Type a string of characters into terminal:\n");

    char line[MAX_LEN];
    if (!fgets(line, MAX_LEN, stdin)) {
        printf("stdin is empty\n");
        return TRUE;
    }

    if (line[strlen(line) - 1] == '\n') {
        line[strlen(line) - 1] = '\0';
    }

    bucket* buckets = malloc(argc * sizeof(bucket));
    if (buckets == NULL) {
        printf("Memory allocation failed\n");
        return TRUE;
    }

    buckets[0].min_word = "";
    buckets[0].head = NULL;

    for (int i = 1; i < argc; i++) {
        buckets[i].min_word = strdup(argv[i]);
        buckets[i].head = NULL;
    }

    char* input_string = strtok(line, " ");
    
    while (input_string != NULL) {
        //printf("Next word is %s\n", input_string);
        insert_sort(input_string, buckets);
        input_string = strtok(NULL, " ");
    }

    print_buckets(buckets);

    // Free allocated memory
    /*for (int i = 0; i < argc; i++) {
        free(buckets[i].min_word);
        node* current = buckets[i].head;
        while (current != NULL) {
            node* temp = current;
            current = current->next;
            free(temp->string);
            //free(temp);
        }
    }
    free(buckets);*/

    return 0;
}

// #include<stdio.h>
// #include<string.h>
// #include<stdlib.h>
// #define MAX_LEN 10000
// #define TRUE 1
// #define FALSE 0

// typedef struct node{
//     char* string;
//     struct node* next;
// } node;

// typedef struct bucket {
//     char* min_word;
//     node* head;
// } bucket;


// void print_buckets(struct bucket* buckets ){
//     struct bucket* prev_bucket=buckets;
//     struct bucket* cur_bucket=prev_bucket;

//     while(cur_bucket->min_word!=NULL){
        
//         struct node*node_ptr=prev_bucket->head;
//             while(node_ptr!= NULL){
//                 printf("%s\n",node_ptr->string);
                
            
//                 node_ptr++;     
//             }
//         cur_bucket++;
//         prev_bucket++;
        

//     }

// }


// void insert_sort(char* wd, struct bucket* buckets){
//     struct bucket* prev_bucket=buckets;
//     struct bucket* cur_bucket=prev_bucket;

//     struct node* node = malloc(sizeof(struct node));
//     node->string =(char*)malloc(sizeof(char)*strlen(wd)+1);// explain this to me 

//     while(cur_bucket->min_word!=NULL&& strcasecmp(cur_bucket->min_word,wd)){
//      if (prev_bucket->head==NULL){
//          prev_bucket->head=node;
        
//         }else{
//             struct node*node_ptr=prev_bucket->head;// im haveing a hard time following where this is poining to 
//             while(node_ptr->next!=NULL){
//                 if(strcasecmp(node_ptr->next->string,wd)>0){//if  next node  was bigger then wd
//                     struct node*insert_node=node_ptr->next; // adds a new node after current node
//                     insert_node->string=wd;//makes the inserted node string = wd
//                     break;
//                 }else{
//                     node_ptr++;

//                 }
                
//             }
            
//         }
    
//         cur_bucket++;
//         prev_bucket++;
//     }
// }


// int main(int argc, char* argv[]){
//     if (argc<3){
//         printf("ERROR MESSAGE: at least 2 buckets needed?");
//         return TRUE;
//     }

//     struct bucket* buckets =(bucket*)malloc(argc * sizeof(bucket));// is this makeing a bucket array? 
//    //buckets[0].min_word = "";
//     //buckets[0].head = NULL;
//     for(int i=1;i<=argc;i++) {
//          strcpy(buckets[i-1].min_word, argv[i]);//adds argv into array
//          buckets[i].head = NULL;
//      }

//      printf("type a string of characters into terminal %s \n");

//     char* line = (char*)malloc(MAX_LEN);
//     if(!fgets(line,MAX_LEN,stdin)){
//         printf("stdin is empty!\n");
//         return 1;
//     }
//     if(line[strlen(line)-1] == '\n')
//         line[strlen(line)-1] = '\0';

//     char* input_string = strtok(line, " ");
    
//     while(input_string = strtok(NULL, " ")){

//         printf("Next word is %s \n", input_string);
//         insert_sort(input_string, buckets);
//         print_buckets(buckets);
//     }


//     return 0;

// }
