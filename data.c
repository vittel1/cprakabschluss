#include "data.h"

struct data
{
};

/* "content" is a null-terminated string. */
data* data_new_string (char const* content)
{
}

/* "content" is a blob of length "length". */
data* data_new_blob (char const* content, unsigned int length)
{
}

data* data_ref (data* data)
{
}

/* Frees memory allocated by "data" if reference count reaches 0. */
void data_unref (data* data)
{
}

/* Returns a newly-allocated string that must be freed by the caller. */
char* data_as_string (data const* data)
{
}

unsigned int data_hash (data const* data)
{
}

int data_cmp (data const* a, data const* b)
{
}
