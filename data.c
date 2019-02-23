#define _GNU_SOURCE
#include "data.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
TODO: Char Array

Um später einfacher Strings vergleichen zu können, wird die Länge im Attribut 'laenge' gespeichert.

Reference counting: Der Struct bekommt ein Attribut, das zählt wie oft das aktuelle Objekt referenziert wird.
Jedes mal, wenn Speicher allokiert wird oder eine Referenz auf das Objekt erzeugt wird, erhöht sich der Zähler.
Zu beachten ist, dass der Counter bei 1 startet.
Wenn der Zeiger auf das Objekt entfernt wird, dann wird der Counter dekrementiert. Ist der Counter 0 gibt es keine
Referenzen mehr auf das Objekt und es kann deallokiert werden (wie es auch ein Garbage Collector tun würde).

Um zu unterscheiden, ob es sich um einen String oder Blob handelt, wird ein Attribut 'typ' erzeugt.
Dabei steht '0' für einen String und '1' für einen Blob.
*/

struct data
{
  char *string;
  int laenge;
  int refcount;
  int typ;
};

/*

 "content" is a null-terminated string.
 Verwendung von 'strdup', um den Speicher dynamisch zu allokieren. Es wird ein Pointer auf den allokierten Speicher zurückgegeben.

 */
data* data_new_string (char const* content)
{
  struct data *data;
  data = malloc(sizeof(struct data));
  //TODO ist das notwendig? Stand das nichtmal in den Folien?
  if(data == NULL) {
    return NULL;
  }
  data->refcount = 1;
  data->typ = 0;
  data->laenge = strlen(content);
  data->string = strdup(content);
  return data;
}

/* "content" is a blob of length "length". */
data* data_new_blob (char const* content, unsigned int length)
{
  struct data *data;
  data = malloc(sizeof(struct data));
  //TODO ist das notwendig? Stand das nichtmal in den Folien?
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

/*
Erhöht den Refercene Counter um 1.
*/
data* data_ref (data* data)
{
  data->refcount++;
  return data;
}

/*
Frees memory allocated by "data" if reference count reaches 0.
*/
void data_unref (data* data)
{
  data->refcount = data->refcount - 1;
  if( data->refcount == 0) {
    free(data->string);
    free(data);
  }
}

/*
Returns a newly-allocated string that must be freed by the caller.

TODO Beschreibung asprintf

*/
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

/*

TODO Beschreibung Hashfunktion

*/
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

/*

TODO Beschreibung

TODO Implementation

*/
int data_cmp (data const* a, data const* b)
{
  if(a->laenge < b->laenge)
  {
    return -1;
  }
  else if(a->laenge > b->laenge)
  {
    return 1;
  }
  
  for(int i = 0; i < a->laenge; i++)
  {
    if(a->string[i] < b->string[i])
    {
      return -1;
    }
    if(a->string[i] > b->laenge[i])
    {
      return 1;
    }
  }

  return 0;
}
