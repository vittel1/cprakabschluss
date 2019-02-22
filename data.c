#define _GNU_SOURCE
#include "data.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct data
{
  //char *const is a constant pointer to a char
  // the value being pointed at can change but the pointer can't (similar to a reference).
  char *string;
  int refcount;
  int laenge;
  //0 String, 1 BLOP
  int typ;
};

/* "content" is a null-terminated string. */
data* data_new_string (char const* content)
{
  struct data *data;
  data = malloc(sizeof(struct data));
  if(data == NULL) {
    return NULL;
  }
  data->refcount = 1;
  data->typ = 0;
  data->laenge = strlen(content);
  data->string = malloc(sizeof(char) * (data->laenge + 1));
  //TODO
  strcpy(data->string, content);
  return data;
}

/* "content" is a blob of length "length". */
data* data_new_blob (char const* content, unsigned int length)
{
  struct data *data;
  data = malloc(sizeof(struct data));
  if(data == NULL) {
    return NULL;
  }
  data->refcount = 1;
  data->typ = 1;
  data->laenge = length;
  data->string = malloc(sizeof(char) * (length + 1));
  for(unsigned int i = 0; i < length; i++) {
    data->string[i] = content[i];
  }
  return data;
}

data* data_ref (data* data)
{
  data->refcount++;
  return data;
}

/* Frees memory allocated by "data" if reference count reaches 0. */
void data_unref (data* data)
{
  data->refcount = data->refcount - 1;
  if( data->refcount == 0) {
    free(data->string);
    free(data);
  }
}

/* Returns a newly-allocated string that must be freed by the caller. */
char* data_as_string (data const* data)
{
  char* result;
  if(data->typ == 0){
    asprintf(&result, "String: %s", data->string);
  }
  else {
    asprintf(&result, "Blop: %p", data->string);
  }
  return result;
}

unsigned int data_hash (data const* data)
{
  char *p = data->string;
  unsigned hash = 5381;
  for(int i=0; i < data->laenge; i++) {
    int c = p[i];
    hash = ((hash << 5) + hash) + c;
    hash = hash & 0x7fffffff;
  }
  return hash;

}

int data_cmp (data const* a, data const* b)
{
//  char const *str2 = b->string;
//  if(a->laenge > b->laenge) {
//    return 1;
//  }
//  else if(a->laenge < b->laenge) {
//    return -1;
//  }
//  else {
//    return strcmp(str1, str2);
//  }
 return strcmp(a->string, b->string);

}
