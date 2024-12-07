#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include "key_op.h"
#include "common.h"
#include "str_op.h"
#include "debug.h"
#include "record.h"
#include "hash_tbl.h"
#include "date.h"
#include "crud.h"
#include "lock.h"
#include "file.h"

unsigned char assemble_key(char ***key, int n, char c, char *str)
{
	size_t len = number_of_digit(n) + strlen(str) + 2; /* 1 is the char c, and 1 is for '\0' so + 2*/

	*(*key) = calloc(len, sizeof(char));
	if (!*(*key))
	{
		printf("calloc failed. %s:%d", F, L - 2);
		return 0;
	}

	if (snprintf(*(*key), len, "%c%d%s", c, n, str) < 0)
	{
		printf("key generation failed. %s:%d.\n", F, L - 2);
		free(*(*key));
		return 0;
	}

	return 1;
}

/* in the project folder you wil find a file file_sys.txt,
	write the file name in this txt document.
		if you have 3 files named:
				-first
				- second
				- third
		in your db then you can write the three names inside the document

	and load_files_system() will load the names in a char** that will be used from
	the key_generator() to understand which file are we working on


*/
unsigned char load_files_system(char ***files, int *len)
{
	FILE *fp = fopen("file_sys.txt", "r");
	if (!fp)
	{
		printf("failed to open file or it does not exist. %s:%d", F, L - 3);
		return 0;
	}

	char buffer[15];
	int i = 0, j = 0;
	while (fgets(buffer, sizeof(buffer), fp))
	{
		i++;
	}
	*len = i;
	rewind(fp);

	*files = calloc(i, sizeof(char *));
	while (fgets(buffer, sizeof(buffer), fp))
	{
		buffer[strcspn(buffer, "\n")] = '\0';
		(*files)[j] = strdup(buffer);
		j++;
	}

	fclose(fp);
	return 1;
}
/*
	this is a suggested method to crate keys for your index
		you can come up with your own method!

	if you decide to use the suggested method key_generator() does:
		loads the file system names (all the name from the file system)
		rec (Record_f, see record.h) contains the file name that the record itself belongs to,
		this file name is matched against the name in the all file system,
		once the match is found the fucntion switch the index (i)  value from the loop to understand which
		key create base on the file that you will save the record to.
		this way is a coded way to customize key cration for the each file
*/
unsigned char key_generator(Record_f *rec, char **key, int fd_data, int fd_index, int lock)
{
	char **files = NULL;
	int l = 0, *p_l = &l;
	if (!load_files_system(&files, p_l))
	{
		printf("load files name failed %s:%d.\n", F, L - 2);
		return 0;
	}

	if (!files)
	{
		printf("load files name failed %s:%d.\n", F, L - 2);
		return 0;
	}

	/*generate a key based on the file name in the rec*/
	int i = 0;
	for (i = 0; i < *p_l; i++)
	{
		if (strcmp(files[i], rec->file_name) == 0)
		{
			break;
		}
	}

	/* this variables are needed for the lock system */
	int lock_pos_i = 0, *plp_i = &lock_pos_i;
	int lock_pos_arr_i = 0, *plpa_i = &lock_pos_arr_i;

	/*acquire WR_IND lock*/
	if (shared_locks && lock == LK_REQ)
	{
		char **file_lck = two_file_path(rec->file_name);
		if (!file_lck)
		{
			printf("%s() failed, %s:%d.\n", __func__, F, L - 3);
			free_strs(*p_l, 1, files);
			return 0;
		}

		int result_i = 0;
		do
		{
			off_t fd_i_s = get_file_size(fd_index, NULL);

			if ((result_i = acquire_lock_smo(&shared_locks, plp_i, plpa_i, files[0], 0,
											 fd_i_s, WR_IND, fd_index)) == 0)
			{
				__er_acquire_lock_smo(F, L - 5);
				free_strs(*p_l, 1, files);
				free_strs(2, 1, file_lck);
				if (munmap(shared_locks,
						   sizeof(lock_info) * MAX_NR_FILE_LOCKABLE) == -1)
				{
					__er_munmap(F, L - 3);
					return 0;
				}
				return 0;
			}
		} while (result_i == MAX_WTLK || result_i == WTLK);

		free_strs(2, 1, file_lck);
		file_lck = NULL;
	}

	HashTable *ht = NULL;
	int ht_i = 0, *pht_i = &ht_i;
	if (!read_all_index_file(fd_index, &ht, pht_i))
	{
		printf("read index failed. %s:%d.\n", F, L - 2);
		free_strs(*p_l, 1, files);
		if (shared_locks && lock == LK_REQ)
		{ /*release the locks before exit*/
			int result_i = 0;
			do
			{
				if ((result_i = release_lock_smo(&shared_locks, plp_i, plpa_i)) == 0)
				{
					__er_release_lock_smo(F, L - 3);
					return 0;
				}
			} while (result_i == WTLK);

			return 0;
		}
		return 0;
	}

	off_t pos = 0;
	switch (i)
	{
	case 0: /**/
	{
		break;
	}
	case 1: /**/
	{
		break;
	}
	case 2: /* */
	{
		break;
	}
	case 3: /*  */
	{
		break;
	}
	case 4: /**/
	{
		break;
	}
	case 5: /*user*/
	{
		break;
	}
	case 6: /*timecard*/
	{
		break;
	}
	default:
		printf("no cases for key gen. %s:%d.\n", F, L);
		free_strs(*p_l, 1, files);
		free_ht_array(ht, *pht_i);
		return 0;
	}

	if (shared_locks && lock == LK_REQ)
	{ /*release the lock normally*/
		int result_i = 0;
		do
		{
			if ((result_i = release_lock_smo(&shared_locks, plp_i, plpa_i)) == 0)
			{
				__er_release_lock_smo(F, L - 3);
				return 0;
			}
		} while (result_i == WTLK);
	}

	return 1;
}
