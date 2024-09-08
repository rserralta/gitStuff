#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<ctype.h>
#define BINS 101
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
int search(hashset, char*);
hashset set_init();
int search(hashset h, char *val);
int insert(hashset *h, char *val);
int eliminate(hashset *h, char *val);