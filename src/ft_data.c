#include "ft_data.h"


  

/**
 * Initializes a given array. The argument array must be either
 * already allocated or a NULL pointer.
 **/
void array_init(array_t **array,int size) {
  int i;

  if (*array == NULL)
    *array = (array_t*)malloc(sizeof(array_t));
  pthread_mutex_init(&((*array)->mutex),NULL);
  (*array)->data = (void**)malloc(sizeof(void*)*size);
  for(i = 0; i < size; i++)
    (*array)->data[i] = NULL;
  (*array)->size = size;
  (*array)->after = 0;
}

/**
 * Initializes a given varray. The argument array must be either
 * already allocated or a NULL pointer.
 **/
void varray_init(varray_t **array,int size) {
  int i;

  if (*array == NULL)
    *array = (varray_t*)malloc(sizeof(varray_t));
  pthread_mutex_init(&((*array)->mutex),NULL);
  (*array)->data = (volatile void**)malloc(sizeof(void*)*size);
  for(i = 0; i < size; i++)
    (*array)->data[i] = NULL;
  (*array)->size = size;
  (*array)->after = 0;
}

/**
 * Resize array to size. We assume array to be NOT NULL
 **/
void array_resize(array_t **array,int size) {
  int i;

  (*array)->data = (void**)realloc((*array)->data,sizeof(void*)*size);
  (*array)->size = size;

  for(i = (*array)->after; i < size; i++)
    (*array)->data[i] = NULL;

}

/**
 * Resize varray to size. We assume varray to be NOT NULL
 **/
void varray_resize(varray_t **array,int size) {
  int i;

  (*array)->data = (volatile void**)realloc((*array)->data,sizeof(volatile void*)*size);
  (*array)->size = size;

  for(i = (*array)->after; i < size; i++)
    (*array)->data[i] = NULL;

}

/**
 * Free memory for array
 **/
void array_delete(array_t *array) {
  free(array->data);
  free(array);
}

/**
 * Free memory for varray
 **/
void varray_delete(varray_t *array) {
  free(array->data);
  free(array);
}

/**
 * A simple helper function to check whether an ID
 * is pointing to a valid element in arr. This assumes
 * that changes in arr are alwys done using this library.
 *
 * This function is not thread safe
 **/
int is_valid(array_t *arr, int id) {
  if ((id >= 0) && (id < arr->after) && 
      (arr->data[id] != NULL))
    return 1;
  else
    return 0;
}

/**
 * (varray version)
 * A simple helper function to check whether an ID
 * is pointing to a valid element in arr. This assumes
 * that changes in arr are alwys done using this library.
 *
 * This function is not thread safe
 **/
int vis_valid(varray_t *arr, int id) {
  if ((id >= 0) && (id < arr->after) && 
      (arr->data[id] != NULL))
    return 1;
  else
    return 0;
}
