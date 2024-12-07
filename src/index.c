#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "date.h"
#include "debug.h"
#include "record.h"
#include "file.h"
#include "common.h"
#include "hash_tbl.h"
#include "str_op.h"
#include "key_op.h"

unsigned char put_data_index_nr(int index_pos, HashTable *ht, char *key)
{

	off_t offset = 0;
	if ((offset = get(key, &ht[0])) == -1)
	{
		printf("record not found");
		return 0;
	}

	if (!set(key, offset, &ht[index_pos]))
	{
		printf("indexing failed. %s:%d.\n", F, L - 2);
		return IND_FLD;
	}

	return 1;
}

unsigned char clean_index_nr(int index_n, HashTable *ht)
{
	size_t l = len(ht[index_n]);
	char **keys_ar = keys(&ht[index_n]);
	int i = 0;
	for (i = 0; i < l; i++)
	{
		Node *node = delete (keys_ar[i], &ht[index_n]);
		if (!node)
		{
			printf("index clean failed. %s:%d.\n", F, L - 3);
			return 0;
		}
	}

	free_strs(l, 1, keys_ar);
	return 1;
}

/*your custom code below this line*/