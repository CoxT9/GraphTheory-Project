#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef struct generic_node_t {
  struct generic_node_t* next;
  void* data;
} generic_node_t;

int main(void) {
  generic_node_t* test_item = (generic_node_t*)malloc(sizeof(generic_node_t*));
  

  generic_node_t* next_item = (generic_node_t*)malloc(sizeof(generic_node_t*));

  char* x = "hello";
  test_item->data = (void*)x;

  char* y = "world";
  next_item->data = (void*)y;

  test_item->next = next_item;

  generic_node_t* curr = test_item;

  while(curr) {
    char* z = (char*)(curr->data);
    printf("%s\n", z);
    curr = curr->next; 
  }
}
