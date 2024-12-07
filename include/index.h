#ifndef INDEX_H
#define INDEX_H

/* for HashTable */
#include "hash_tbl.h"

unsigned char put_data_index_nr(int index_pos, HashTable *ht, char *key);
unsigned char clean_index_nr(int index_n, HashTable *ht);

/*ADD BELOW THIS LINE YOUR CUSTOM INDEXING FUCNTIONS*/

#endif
