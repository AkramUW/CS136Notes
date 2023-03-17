/* 
Dynamic Memory and ADTs:
+++++++++++++++++++++++++

Malloc(n *sizeof(type)) allocates memory, used for structs, pointers and arrays.
Free(array/struct) -> Deallocates memory, preventing memory leak
Dangling pointers -> Pointer pointing to my_array, my_array was freed, Pointer is just there
Garbage Collector: Some languages have something called garbage collectors that automatically frees up any memory that was used.
