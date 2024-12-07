#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "json.h"
#include "debug.h"
#include "str_op.h"
#include "key_op.h"
#include "file.h"
#include "crud.h"
#include "parse.h"
#include "bst.h"
#include "db_instruction.h"
#include "common.h"

unsigned char convert_pairs_in_db_instruction(BST pairs_tree, Instructions inst)
{
	/* HERE YOU CAN PROGRAM THE INSTRUCTIONS CASES FOR YOUR DATABASE */
	switch (inst)
	{
	default:
		printf("unknow instruction.\n");
		return 0;
	}

	/*debug statement */
	printf("instruction succeed.\n");
	return 1;
}
