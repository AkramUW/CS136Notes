/* 
MEMORY CHECKERS
---------------
These concepts apply to C AND C++

1- Valgrind (Or memcheck in valgrind)
When compiling: -g for debug info
                -O0 disabling compiler optimizations (for more accurate
                                                    valgrind output)
Example: gcc -std=c99 -g -O0 -Wall prog.c -o prog
(note: gcc is better for valgrind)
cmd-line-arg: valgrind ./prog
run valgrind after development and testing cuz very slow

2- Asan (Address Sanitizer)
part of clang, is fast(er).
enable using "-fsanitize=address -O0-fno-omit-frame-pointer"
Only produces output if something went wrong
memory sanitizer: clang -O0 -fsanitize=memory test2.c 

Common errors:
- Conditional jump over uninitialized value (-Wall would catch it)
asan produced no errors, however memory sanitizer was able to

- Invalid write
e.g. trying to print an array with prinf("%d")
- Invalid read
e.g. 
int *p = NULL
int x = *p
BOTH: SEGV in Address San

-buffer overruns
Accessing out of range
e.g. int a[10];
printf("%d", a[10]); -> This will print an unpredictable output 
Valgrind -> wasn't able to catch it, since it doesn't know where the stack ends
ASAN -> Able to detect when optimizations are off (common theme)
*/
// How do C programs receive command line arguments:
int main(int argc, char **argv) { // argc is number of args
                                  // argv array of string rep of each arg
  int arr[2]; 
  if (arr[argc != 1])
    return 1;
  else
    return 0;
}

// -improper stack use
// Example
char *get_hello(){
  char str[20] = "Hello World!"; 
  char *toRet = str;
  return toRet;
}
int main (){
  printf("%s\n",get_hello()); 
  return 0;
}
/*
Program will compile no crash but the output will be unpredictable, 
that's because we created a stack-allocated array 
(which doesn't exist in memory after the func returns)
and returned a pointer to it.

Memory checkers weren't able to detect this by default. With ASAN, you
can detect it if you use 
'fsanitize-address-use-after-return=always'
and compile with 'ASAN_OPTIONS=detect_stack_use_after_return=1'

To avoid such error, you'd return &str instead or use dynamic memory (but be sure to free itd)

- Incorrect calls to malloc
say you want to an array that stores 2 ints, but instead of using
malloc(2*sizeof(int)), you use malloc(2)
that may produce the correct outut at times but will definitely cause 
a memory error 100% of the time

- Invalid free
Freeing stack-allocated memory -> Crash during exec
Freeing prematurely before reading/using the memory allocated

- Common errors wit linked data structures
Using memory after it's been freed
Example
*/
void free_list(struct Node *node){
  struct Node *curr = node; 
  while(curr){
    free(curr);
    curr = curr->next; 
  }
}
// Instead:
void free_list(struct Node *node){
  struct Node *curr = node; 
  while(curr){
    // store in temp to delay freeing
    struct Node *temp = curr; 
    curr = curr ->next;
    // now free
    free(temp); 
  }
}
/*
Inserting a node without init its next or init as null and causing memleak
*/
// Aside, implementing a deep copy in C
struct Node *deep_copy(struct Node *other){
  if(other == NULL) return NULL;
  struct Node *to_ret = malloc(sizeof(struct Node));
  to_ret->data = other->data;
  to_ret->next = deep_copy(other->next);
  return to_ret;
}
/* SUMMARY
==========
ASAN Limitations: Detection of Stack use after return (not by default), Uninit reads (had to
use MemSan) 
*/