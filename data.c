#define _GNU_SOURCE
#include "data.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
Strings sind eine Sequenz von chars, die in einem Array gespeichert werden. Das Nullbyte '\0' zeigt das Ende des Strings an.
Die Variable 'strings' wird im Struct auf die Adresse vom ersten Element des Arrays zeigen.

Um später einfacher Strings vergleichen zu können, wird die Länge in der Variable 'laenge' gespeichert.

Reference counting: Der Struct bekommt eine Variable, die zählt wie oft das aktuelle Objekt referenziert wird.
Jedes mal, wenn Speicher allokiert wird oder eine Referenz auf das Objekt erzeugt wird, erhöht sich der Zähler.
Zu beachten ist, dass der Counter bei 1 startet.
Wenn der Zeiger auf das Objekt entfernt wird, dann wird der Counter dekrementiert. Ist der Counter 0 gibt es keine
Referenzen mehr auf das Objekt und es kann deallokiert werden (wie es auch ein Garbage Collector tun würde).

Um zu unterscheiden, ob es sich um einen String oder Blob handelt, wird eine Variable 'typ' erzeugt.
Dabei steht '0' für einen String und '1' für einen Blob.
*/

struct data
{
  char *string;
  unsigned int laenge;
  int refcount;
  int typ;
};

/*
Im ersten Teil wird Speicher für den Struct allokiert. Als Größe wird die Größe des Structs angegeben.
Wenn kein Speicher allokiert werden kann, dann wird das durch das Programm abgefangen. Durch die fehlende
Fehlerbehandlung in C muss die extra angegeben werden. Nur wenn erfolgreich allokiert wurde, können Werte zugewiesen werden.

Die Funktion 'strlen' gibt die Länge des Strings zurück. Das Nullbyte am Ende wird nicht gezählt.

Es wird 'strdup' verwendet, um den Speicher dynamisch zu allokieren. Es wird ein Pointer auf den allokierten Speicher zurückgegeben.
 */
data* data_new_string (char const* content)
{
  struct data *data;
  data = malloc(sizeof(struct data));
  if(data == NULL) {
    //TODO gibts besseres als NULL?
    return NULL;
  }
  else {
    data->refcount = 1;
    data->typ = 0;
    data->laenge = strlen(content);
    data->string = strdup(content);
    return data;
  }
}

/* "
content" is a blob of length "length".
*/
data* data_new_blob (char const* content, unsigned int length)
{
  struct data *data;
  data = malloc(sizeof(struct data));
  if(data == NULL) {
    //TODO gibts besseres als NULL?
    return NULL;
  }
  else {
    data->refcount = 1;
    data->typ = 1;
    data->laenge = length;
    data->string = malloc(sizeof(char) * (length + 1));
    for(unsigned int i = 0; i < length; i++) {
      data->string[i] = content[i];
    }
    return data;
  }
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
  if(data->typ == 0) {
    asprintf(&result, "String: %s", data->string);
  }
  else {
    asprintf(&result, "Blop: %p", data->string);
  }
  return result;
}

/*

Der im folgenden implementierte Algorithmus (djb2) ist angelegt an die folgenden:
https://gist.github.com/conmarap/e1a802d0f462531bc712b5505b59d5e8
http://www.cse.yorku.ca/~oz/hash.html

*/
unsigned int data_hash (data const* data)
{
  char *p = data->string;
  unsigned int hash = 5381;
  for(unsigned int i=0; i < data->laenge; i++) {
    unsigned int c = p[i];
    hash = ((hash << 5) + hash) + c;
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

  for(unsigned int i = 0; i < a->laenge; i++)
  {
    if(a->string[i] < b->string[i])
    {
      return -1;
    }
    if(a->string[i] > b->string[i])
    {
      return 1;
    }
  }
  return 0;
}
