#include "hash.h"
hashset set_init() {
  hashset h = {(node **)malloc(BINS * sizeof(node *)), 0, BINS};
  for (int i = 0; i < BINS; i++)
    h.table[i] = NULL;
  return h;
}
static int hash(char *key, int bins) {//key: "abcd" = {97, 98, 99, 100, 0}
  unsigned hashval = 0;
  for (int i = 0; i < strlen(key); i++)
    hashval = 31 * hashval + key[i];//97* 31^3 + 98 * 31^2 + 99 * 31 + 100
  return hashval % bins;//return an index from 0..bins - 1
}
static void rehash(hashset*);
int insert(hashset *h, char *val) {
  if(search(*h, val)) // I found the value
	return 0;                        // duplicate
  int index = hash(val, h->bins);
  if (h->size >= h->bins)//load factor >= 100%
    rehash(h);
  node *new_element = (node *)malloc(sizeof(node));
  new_element->next = h->table[index];
  new_element->value = strdup(val);
  h->table[index] = new_element;
  h->size++;
  return 1;
}
int eliminate(hashset *h, char *val){
	if(!search(*h, val)) // I didn't find the value
		return 0;                        //failed to remove
	int index = hash(val, h->bins);
	node *prev = NULL, *it;
	for(it = h->table[index];it->next;it = it->next)
		if (!strcmp(it->value, val))
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
}
int search(hashset h, char *val) {
  int index = hash(val, h.bins);
  for (node *iterator = h.table[index]; iterator; iterator = iterator->next)
    if (!strcmp(iterator->value, val))
      return 1;
  return 0;
}


