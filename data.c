#define _GNU_SOURCE
#include "data.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct data
{
  //char *const is a constant pointer to a char
  // the value being pointed at can change but the pointer can't (similar to a reference).
  char const* string;
  int refcount; //Array?
  int laenge;
  int typ; //0 String, 1 BLOP
};

/* "content" is a null-terminated string. */
data* data_new_string (char const* content)
{
  struct data *data = malloc(sizeof *data);
  data->string = content;
  data->refcount = 0;
  data->laenge = strlen(content);
  data->typ = 0;
  return data;
}

/* "content" is a blob of length "length". */
data* data_new_blob (char const* content, unsigned int length)
{
  struct data *data = malloc(sizeof *data);
  //TODO
  data->string = content;
  data->refcount = 0;
  data->laenge = length;
  data->typ = 1;
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
  data->refcount = data->refcount -1;
  if( data->refcount == 0) {
    //free(data->string);
    free(data);
    data = NULL;
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
  while(*p != 0) {
    hash = ((hash << 5) + hash) + *(p++);
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
