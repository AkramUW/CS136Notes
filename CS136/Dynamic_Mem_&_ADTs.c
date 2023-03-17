/* 
Dynamic Memory and ADTs:
+++++++++++++++++++++++++

Malloc(sizeof(type)) allocates memory, used for structs, pointers and arrays.
Malloc returns null instead of crashing
Calloc inits memory and fills it with zeroes, but it's O(n) sooooo
Free(array/struct) -> Deallocates memory, preventing memory leak
Dangling pointers -> Pointer pointing to my_array, my_array was freed, Pointer is just there
Garbage Collector: Some languages have something called garbage collectors that automatically frees up any memory that was used.

*/

// Application to Merge Sort

// helper:
void merge(int dest[], const int src1[], int len1,
                       const int src2[], int len2) {
  assert(dest);
  assert(src1);
  assert(src2);
  int pos1 = 0;
  int pos2 = 0;
  for (int i = 0; i < len1 + len2; ++i) {
    if (pos1 == len1 || (pos2 < len2 && src2[pos2] < src1[pos1])) {
      dest[i] = src2[pos2];
      ++pos2;
    } else {
      dest[i] = src1[pos1];
      ++pos1;
    }
  }
                       }

void merge_sort(int a[], int len) {
  array_print(a, len);
  assert(a);
  if (len <= 1) {
    return;
  }
  int llen = len / 2;
  int rlen = len - llen;
  
  int *left = malloc(llen * sizeof(int));
  int *right = malloc(rlen * sizeof(int));
  
  for (int i = 0; i < llen; ++i) {
    left[i] = a[i];
  }
  for (int i = 0; i < rlen; ++i) {
    right[i] = a[i + llen];
  }
  
  merge_sort(left, llen);
  merge_sort(right, rlen);
  
  merge(a, left, llen, right, rlen);
  
  free(left);
  free(right);
}

/* What's mergesort doing?
Array = [1,2,4,6,7,8,0,5]
Larr = [1,2,4,6]
rarr = [7,8,0,5]
each of these array will be further dissected until they're of length one
LLarr = [1,2]
RLarr = [4,6]
LRarr = [7,8]
RRarr = [0,5]
and then this whole thing dissects furhter into
[1][2] [4][6] [7][8] [0][5]
at this point we have a return so from the bottom-up the stack will 
pop them one by one into merge
[1][2] = [1,2]
[4][6] = [4,6]
[1,2][4,6] = [1,2,4,6]
[7][8] = [7,8]
[0][5] = [0,5]
[7, 8] [0,5] = [0,5,7,8]
[1,2,4,6][0,5,7,8] = [0,1,2,4,5,6,7,8]
=====================================================================

Duration
Dynamically allocated memory persists after the function returns.
This is a side effect of the function that should be mentioned in the documentation
Example: effects: frees a 
         effects: allocates memory (caller must free)


Resizing
realloc(pointer, new_length) resizes memory to new_length and frees old memory
It returns a pointer to new memory which is what you have to assign to a variable.
Considered side effect.
O(n) where n = min(old_length, new_length)

Application: String I/O
Instead of using an oversized array and risking running into buffer overflow
we could instead reallocate memory for the input string as we
read in each character at a time
Example:*/
char *readstr(void) {
  char c = 0;
  if (scanf(" %c", &c) != 1) {   // ignore initial WS
    return NULL;
  }
  int len = 1;
  char *str = malloc(len * sizeof(char));
  str[0] = c;
  while (1) {
    if (scanf("%c", &c) != 1) break;
    if (c == ' ' || c == '\n') break;
    ++len;
    str = realloc(str, len * sizeof(char));
    str[len - 1] = c;
  }
  str = realloc(str, (len + 1) * sizeof(char));
  str[len] = '\0';
  return str;
}
/* This is O(n^2)
You can improve the efficiency of this by initing more memory
than necessary, and doubling its size only when full

Array of Strings
Basically an array of pointers to null-terminated arrays
*/ 

char **read_aos(int *len) { // len is a pointer so that we can
                            // modify it
  assert(len);
  char **aos = NULL;
  int maxlen = 0;
  *len = 0;
  while (1) {
    char *s = readstr();
    if (s == NULL) {
      break;
    }
    if (*len == maxlen) {
      if (maxlen == 0) {
        maxlen = 1;
      }
      maxlen *= 2; // doubling when max len is reached
      aos = realloc(aos, maxlen * sizeof(char *));
    }
    aos[*len] = s;
    *len += 1;
  }
  if (*len < maxlen) { // resizing for space efficiency
    aos = realloc(aos, *len * sizeof(char *));
  }
  return aos;
}

int main(void) {
  int len = 0;
  char **aos = read_aos(&len);
  for (int i = 0; i < len; ++i) {
    printf("-->%s<--\n", aos[i]);
  }  
  // free all the strings and the aos itself
  for (int i = 0; i < len; ++i) {
    free(aos[i]);
  }
  free(aos);
}

// ADTs
/*  We allocate memory, so just like we have an init function
we have to have a destroy to free the memory. 
We use the doubling strategy to add as many ints as we want to the stack
We keep track of actual len and max_len
Example (with stack)
*/
struct stack *create_stack(void) {
  struct stack *s = malloc(sizeof(struct stack));
  s->len = 0;
  s->maxlen = 1;
  s->data = malloc(s->maxlen * sizeof(int));
  return s;
}

// Time: O(1) [amortized]
void stack_push(int item, struct stack *s) {
  assert(s);
  if (s->len == s->maxlen) {
    s->maxlen *= 2;
    s->data = realloc(s->data, s->maxlen * sizeof(int));
  }
  s->data[s->len] = item;
  s->len += 1;
}

void stack_destroy(struct stack *s) {
  assert(s);
  free(s->data);
  free(s);
}
bool stack_is_empty(const struct stack *s) {
  assert(s);
  return s->len == 0;
}

int stack_top(const struct stack *s) {
  assert(s);
  assert(s->len);
  return s->data[s->len - 1];
}

int stack_pop(struct stack *s) {
  assert(s);
  assert(s->len);
  s->len -= 1;
  return s->data[s->len];
}
 
/* Amortizied (average-case) Analysis
Say your friend wants to do push-ups, he does
k pushups on each day that's the k-th power of 2
His pushup schedule will look like this
1,2,0,4,0,0,0,8,0,0,0,0,0,0,0,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
and over 31 days he'd have done 31 push-ups, averaging 1 pushup
a day
For stack_push:
If we allocate an array of size n, the calls to stack_push upto n times
will be O(1), but we we call it an (n+1) time, we'll need to realloc memory
which is O(n).
Notice how it took us O(n) (i.e. n+whatever) calls to get O(n) run-time
hence, amortizied analysis allows us to spread the cost over the number of
operations needed to get there. So stack_push amortizied run-time will be O(1)

*/
