#include "solver.h"
#define MAX_LEN 100
#define BUF_SIZE 60
 
 hashset set;
/*
hashset set_init() { // KianoushCode
  hashset h = {(node **)malloc(BINS * sizeof(node *)), 0, BINS};
  for (int i = 0; i < BINS; i++)
    h.table[i] = NULL;
  return h;
}
*/
// --------------------------------------------------------------------------------

typedef struct {
  char **subpuzzle; // this is part of the char*** buffer that you want a single
                    // solver thread to solve
  int min_len;      // this is the first integer passed after -len flag in the
                    // command-line argument: minimum length of puzzle solution
  int max_len;      // this is the second integer passed after -len flag in the
                    // command-line argument: minimum length of puzzle solution
  int rows; // the line number which the subpuzzle is taken from. Used for
            // printing line span on the output.
  int col;  // the column number which the subpuzzle is taken from. Used for
            // printing column span on the output.
  int subpuzzle_rows; // determines the height (# of rows/lines) of current
                      // subpuzzle that the solver thread must solve
  int subpuzzle_cols; // determines the width (# of cols) of current subpuzzle
                      // that the solver thread must solve
  int t_id; // the id of thread. Used for printing it on the output along with
            // the puzzle solution
  // char* buf;// i added this to store the puzzle solution in a buffer. I will
  // use it in the main thread to print the solution on the
  
} info;

// void up() {}
// void down() {}
// void forword() {}
// void backword() {}
//------------------------------- chat gpt + my code based off of kianoush
// office hours---->
void *solve(void *arg) {
  info *puzzle = (info *)arg;
  char **subpuzzle = puzzle->subpuzzle;
  int min_len = puzzle->min_len;
  int max_len = puzzle->max_len;
  int rows = puzzle->rows;
  int cols = puzzle->col;
  int subpuzzle_rows = puzzle->subpuzzle_rows;
  int subpuzzle_cols = puzzle->subpuzzle_cols;
  int t_id = puzzle->t_id;

	//printf("Before accessing subpuzzle[1][1] ln:54\n");
	//printf("subpuzzle[1][1]: ln:55'%c'\n", subpuzzle[1][1]);
	//printf("After accessing subpuzzle[1][1] ln:56\n");

  //printf("min_len: '%d'\n max_len: '%d'\n rows: '%d'\n cols:'%d'\n subpuzzle_rows: '%d'\n subpuzzle_cols: '%d'\n t_id: '%d'", min_len,rows,cols,subpuzzle_rows,subpuzzle_cols,t_id);// this is a print statement for debugging
  // char* buf = puzzle->buf;
  // Iterate over each starting position (i, j)
  // Inside the solve function, before the first for loop
  //printf("Inside solve function for thread line 58 %d\n", t_id);
	//printf("Entering nested loops\n");
   char word[max_len]; // Buffer to store the word
  for (int i = 0; i < subpuzzle_rows; i++) {
    for (int j = 0; j < subpuzzle_cols; j++) {
      int k = 0;
      while (k < max_len && j + k < cols) {
        word[k] = subpuzzle[i][j + k];
        word[k + 1] = '\0';
        int len = strlen(word);
        if (len>min_len && search(set, word)) {
          printf("Found word '%s' at (%d, %d) horizontally.\n", word, rows + i,cols + j);
        }//if
        k++;

      }
    }
  }

      //   // Check vertically
      //   for (int len = min_len; len <= max_len && i + len <= subpuzzle_rows;len++) {
      //     char word[max_len]; // Buffer to store the word
      //     for (int k = 0; k < len; k++) {
      //       word[k] = subpuzzle[i + k][j]; // Copy the characters to the buffer
      //     }
      //     word[len] = '\0'; // Null-terminate the string

      //     // Check if the word exists in the dictionary
      //     if (search(set,
      //               word)) { // JORDANRAINEY - Is this supposed to be the
      //                         // dictionary set?
      //       //printf("the search funtion: %d\n",search(dic_set,word));
      //       printf("Found word '%s' at (%d, %d) vertically.\n", word, rows + i,
      //             cols + j);
      //       // Handle the word (e.g., print it, store it in a buffer, etc.)
      //     }
      //   }
      // }
  

  //fprintf(stderr, "solver thread: %d\n", t_id);
	// Inside the solve function, before returning NULL
	//printf("Exiting solve function for thread %d\n", t_id);

  return NULL;
}
//--------------------------------------------------------------------------------------kianoush
// shell code----->
void print_buffer(char **sub_puzzle, int subpuzzle_rows, int subpuzzle_cols) {
  // this function is used for printing the content of each buffer cell.
  // do NOT call this function anywhere in your final submission.
  printf("%d by %d\n", subpuzzle_rows, subpuzzle_cols);
  for (int i = 0; i < subpuzzle_rows; i++)
    for (int j = 0; j < subpuzzle_cols; j++)
      printf("%c%s", sub_puzzle[i][j], j == subpuzzle_cols - 1 ? "\n" : "");
}

int main(int argc, char **argv) { //argv= [progname] [-dict] [dict.txt] [-input] [puzzle1mb.txt] [-size]

  if (argc < 11)
    error("Fatal Error. Usage: solve -dict dict.txt -input puzzle1mb.txt -size "
          "1000 -nbuffer 64 -len 4:7 [-s]",1);
  int puzzle_size = 0, buf_cells = 0, i = 0, j, fd, min_len = 0, max_len = 0,// ex. puzzle size is 15, buf_cell= amount of threads,min_len is 3 maxlen is 11
      sorted = 0, buf_dimension;//sorted=? buf_demention=?
  char *filename = NULL;
  FILE *dictionary_file;
  char ***buffer; //buffer[thread][][]
  while (++i < argc) {//[-dict]
    if (argv[i][0] != '-')//checks for flags b/c 
      error("Fatal Error. Invalid CLA", 2);
    if (!strcmp(argv[i], "-size")) { //if argv[i] is -size
      puzzle_size = atoi(argv[++i]);
      if (puzzle_size < 15 || puzzle_size > 46340)
        error("Fatal Error. Illegal value passed after -size", 3);
    } else if (!strcmp(argv[i], "-nbuffer")) {//nbuffer is the amount of threads
      buf_cells = atoi(argv[++i]);//becomes the number of threads
      if (buf_cells != 1 && buf_cells != 4 && buf_cells != 16 &&
          buf_cells != 64)
        error("Fatal Error. Illegal value passed after -nbuffer", 4);
      buf_dimension = (int)sqrt(MEM_LIMIT / buf_cells);// equally breaks the large word search into the number of threads buf_dimention = how many X or y b/c it the same number
      // buf_cells = 1 -> buf_dimension = 8000
      // buf_cells = 4 -> buf_dimension = 4000
      // buf_cells = 16 -> buf_dimension = 2000
      // buf_cells = 64 -> buf_dimension = 1000
    } else if (!strcmp(argv[i], "-input")) {
      filename = strdup(argv[++i]);// assigns the file 
      fd = open(filename, O_RDONLY, 0);// read only
      if (fd < 0)
        error("Fatal Error. Illegal value passed after -input", 5);
    } else if (!strcmp(argv[i], "-dict")) {
      dictionary_file = fopen(argv[++i], "r");// reads
      if (!dictionary_file)
        error("Fatal Error. Illegal value passed after -dict", 6);
    } else if (!strcmp(argv[i], "-len")) {
      char *min_max = strdup(argv[++i]);//grabs the whole string 3:14?
      char *max_str;//?
      if (strrchr(min_max, ':') != (max_str = strchr(min_max, ':')) ||
          !strchr(min_max, ':'))// if there is no : or if there is more then one : and max_str points to the :
        error("Fatal Error. Illegal value passed after -len", 7);
      max_len = atoi(max_str + 1);//what ever is after the colon
      min_max[max_str - min_max] = '\0';// removing the colen and the max
      min_len = atoi(min_max);// assigning the min_len
      if (min_len < 3 || max_len > 14 || min_len > max_len)
        error("Fatal Error. Illegal value passed after -len", 7);
    } else if (!strcmp(argv[i], "-s"))
      sorted = 1;
    else {
      error("Fatal Error. Usage: solve -dict dict.txt -input puzzle1mb.txt "
            "-size 1000 -nbuffer 64 -len 4:7 [-s]",
            1);
      break;
    }
  }
  // Read and move all words from dictionary_file to a new hash table (hashset)
  // Your code here...
  //------------------------------------------------------------------------------------kianoush
  // code copyed an pasted from last assignment.-------------------------->

  // JORDANRAINEY - I think the code here and below MIGHT be uneccesary.

 // filename = "dict.txt"; // JORDANRAINEY - it was 'dic.txt' and not 'dict.txt'.
                         // Changed it

  hashset set = set_init();

  // printf("Here!");fflush(stdout);
 // FILE *words_file = fopen(filename, "r" /* "w", or "a" */);
  char set_buffer[BUF_SIZE];

  while (fgets(set_buffer, BUF_SIZE, dictionary_file)) {

    // JORDANRAINEY - Why are we getting the buffer length using strlen? Just
    // use BUF_SIZE, which is the size of the dictionary set?) int len =
    // strlen(buffer);
    int len = BUF_SIZE;
    if (set_buffer[len - 1] == '\n')
      set_buffer[len - 1] = '\0';
    //printf("next line: %s\n", set_buffer);
    insert(&set, set_buffer);
  }// check if it gets inserted again

  fclose(dictionary_file);
  //---------------------------------------------------------------kianoush
  // shell code-->
  // allocate 64MB of buffer in the heap
  // buffer is a 3D array
  // on the outermost dimension, we have buf_cells elements
  // each element is given to one consumer thread
  // each element is going to store a (square-shape) sub-puzzle
  // each element is a square-shape 2D array of characters
  // dimension of this square-shape array is buf_dimension by buf_dimension
  buffer = (char ***)malloc(buf_cells * sizeof(char **));//buffer[allowcting this space][][] is a 3d array that has the size of the amount of threads times the space needed for a 2d array.
  for (i = 0; i < buf_cells; i++) {// itterates over every thread 0-63
    buffer[i] = (char **)malloc(buf_dimension * sizeof(char *));//each buffer [][allocating this space created hear][]
    for (j = 0; j < buf_dimension; j++)
      buffer[i][j] = (char *)malloc(buf_dimension);//each buffer [][][allowcating this space which is ]
  }
  int buf_index = 0;
  pthread_t t_id[buf_cells];// a empty box that is the same size as
  for (i = 0; i < buf_cells; i++)
    t_id[i] = NULL;
  for (int row = 0; row + max_len - 1 < puzzle_size;//iterates over the rows, it
       row += (buf_dimension - max_len + 1)) {
    int subpuzzle_rows = (row + buf_dimension <= puzzle_size)
                             ? buf_dimension
                             : puzzle_size - row;
    for (int column = 0; column + max_len - 1 < puzzle_size;
         column += (buf_dimension - max_len + 1)) {
      long start = (long)row * (puzzle_size + 1) + column;
      lseek(fd, start, SEEK_SET);
      int subpuzzle_cols = (column + buf_dimension <= puzzle_size)
                               ? buf_dimension
                               : puzzle_size - column;
      if (t_id[buf_index]) // if there is a busy consumer/solver,
        pthread_join(t_id[buf_index],
                     NULL); // wait for it to finish the job before manipulating
                            // the buffer[buffer_index]
      for (i = 0; i < subpuzzle_rows; i++) {
        int n_read = read(fd, buffer[buf_index][i], subpuzzle_cols);
        if (n_read < subpuzzle_cols)
          error("Fatal Error. Bad read from input file", 10);
        if (subpuzzle_cols < buf_dimension)
          buffer[buf_index][i][subpuzzle_cols] = '\0';
        lseek(fd, puzzle_size - subpuzzle_cols + 1, SEEK_CUR);
      }
      if (subpuzzle_rows < buf_dimension)
        buffer[buf_index][subpuzzle_rows] = NULL;
      // populates the arrays in the buffer which


      // modify these lines so that you can create and start a solver
      // thread-------------me an chat gpt basesd off of anouncement-->
	  // Inside the main function, before the "Consuming buffer #0" line
      //printf("Before consuming buffer #0 line 244\n");
	 //fprintf(stderr, "Consuming buffer #%d\n", buf_index);
	  // Inside the main function, after the "Consuming buffer #0" line
	  //printf("After consuming buffer #0 line 247 \n");
	  
		
      info *subpuzzle_info=(info*)malloc(sizeof(info));
      subpuzzle_info->subpuzzle=buffer[buf_index];// Assign the subpuzzle buffer
      subpuzzle_info->min_len = min_len; // Assign the minimum length
      subpuzzle_info->max_len = max_len; // Assign the maximum length
      subpuzzle_info->rows = row; // Assign the line number
      subpuzzle_info->col = column;// Assign the column number
      subpuzzle_info->subpuzzle_rows =subpuzzle_rows; // Assign the subpuzzle rows
      subpuzzle_info->subpuzzle_cols =subpuzzle_cols; // Assign the subpuzzle columns
      subpuzzle_info->t_id = buf_index; // Assign the thread ID // the thread we are on
      
      // pthread_create(t_id + buf_index, NULL, solve, &subpuzzle_info);
      //  Create the solver thread and pass the info struct by reference
      while (pthread_create(t_id + buf_index, NULL, solve, subpuzzle_info));
      //---------------------------------------------------------------------------------------------kianoush
      // shell code----->
      // after passing the right information to it...

      //fprintf(stderr, "Consuming buffer #%d\n", buf_index);
    //   char *message = (char *)malloc(1000);
    //   sprintf(
    //       message,
    //       "solving sub-puzzle of dimensions %d by %d located at index (%d,%d).",
    //       subpuzzle_rows, subpuzzle_cols, row, column);
    //   pthread_create(t_id + buf_index, NULL, solve, message);
      // print_buffer(buffer[buf_index], subpuzzle_rows, subpuzzle_cols);
      // end of modification

      buf_index = (buf_index == buf_cells - 1) ? 0 : buf_index + 1;//increments the the buff index 
    }
  }
  for (i = 0; i < buf_cells; i++)
    if (t_id[i])
      pthread_join(t_id[i], NULL);

  if (sorted) {
    // print the binary search tree using in-order traversal...
    // your code here...
  }
  // END OF MAIN
}