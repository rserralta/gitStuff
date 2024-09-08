//change lines 6-19 to update the following:
// 1. program name (mentioned in the Makefile), 
// 2. # of test-cases
// 3. stdin of each test-case
// 4. and command-line arguments of each test-case
static char DEFAULT_PROGRAM_NAME[] = "bucketsort";
#define NUM_TEST_CASES 4
static char* TESTCASES[] = {"This is a simple example with 8 words."//TC#0
								  ,"This is a simple example with 8 words."//TC#1
								  ,"This is a simple example with 8 words."//TC#2
								  ,"I'd rather go shopping than go to the gym!"//TC#3
								  };
static const char**  TEST_ARGS[] = { ""//TC#0
							  "strawberry", "blackberry",""//TC#1
							  ,"apple", "carrot", "kiwi", "pineapple", "watermelon", ""//TC#2
							  ,"mango", ""//TC#3
							 };	
// ------------------------------NO Change After this line ------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <sys/wait.h>
#include <dirent.h> 
#define MAX_STUDENTS 150
#define MAX_INPUT_SIZE 2000
#define MAX_PROGRAM_NAME_LEN 100
#define MAX_BUF 200
#define die(e) do { fprintf(stderr, "%s\n", e); exit(EXIT_FAILURE); } while (0);
typedef enum{
	LIST_FILES,
	UNZIP,
	CLEAN,
	MOSS,
	MAKE,
	RUN
}phase;
static char* CLA_ERROR_MESSAGE = "Fatal error. proper usage: ag [-xu] [-xm] [-xs] [-xc] [-s nn mm... or -s nn-mm] where nn, mm, etc are the submissions that you do not want to test";
char** submissions;
char** submissions_zipped;
int sub_count;
int test_case = 0;
static char *PROGRAM_NAME;
void update_name(char* dir);
void compile_h_c_files(char* dir_path);
int DONT_GRADE[MAX_STUDENTS];
char** get_args(char* file_name, int test_case);
char* str_args(char* file_name, int test_case);