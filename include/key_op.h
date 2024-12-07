#ifndef _KEY_OP_H_
#define _KEY_OP_H_

#include "record.h"
#include "hash_tbl.h"

/*
    assemble_key() is customizable according to your key procedure and technique.
*/
unsigned char assemble_key(char ***key, int n, char c, char *str);
unsigned char load_files_system(char ***files, int *len);
unsigned char key_generator(Record_f *rec, char **key, int fd_data, int fd_index, int lock);

#endif /* key_op.h */
