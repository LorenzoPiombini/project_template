/*
	THIS IS THE MAIN PROGRAM.
		it will start a thread pool before entering the infinet socket loop.
		it will handle incomming connections and add the data from the socket in a task queue
			that will be handle from the thread pool.

	@@@@@@@ THERE SHOULD NOT BE ANY NEED TO CHANGE THIS PROGRAM @@@@@@@@

author: Lorenzo Piombini
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <pthread.h>
#include "file.h"
#include "debug.h"
#include "lock.h"
#include "db_instruction.h"
#include "multi_th.h"
#include "queue.h"
#include "com.h" /*for listen_set_up()*/

int main(void)
{

	/*this should be the real main program*/

	/*shared locks is declared as a global variable in lock.h and define as NULL
							inside lock.c */
	if (!set_memory_obj(&shared_locks))
	{
		printf("set_memory_obj() failed, %s:%d.\n", F, L - 2);
		return EXIT_FAILURE;
	}

	int fd_socket = -1;
	if (!listen_set_up(&fd_socket))
	{
		printf("listen_set_up() failed %s:%d.\n", F, L - 2);
		close_file(1, fd_socket);
		free_memory_object(SH_ILOCK);
		return 1;
	}

	/*init the thread pool */
	Thread_pool pool = {0};

	Queue q = {NULL, NULL, 0};
	if (!q_init(&q))
	{
		printf("q_init() failed, %s:%d,\n", F, L - 2);
		close_file(1, fd_socket);
		free_memory_object(SH_ILOCK);
		return 1;
	}

	pool.tasks = &q;
	if (!pool_init(&pool))
	{
		printf("thread pool init failed, %s:%d.\n", F, L - 2);
		close_file(1, fd_socket);
		free_memory_object(SH_ILOCK);
		return 0;
	}

	int fd_client = -1;
	for (;;)
	{
		int buff_size = 1000;
		char instruction[buff_size];

		if (!accept_instructions(&fd_socket, &fd_client, instruction, buff_size))
		{
			printf("accept_instructions() failed %s:%d.\n", F, L - 2);
			close_file(2, fd_socket, fd_client);
			/*close the shared memory*/
			free_memory_object(SH_ILOCK);
			return 1;
		}

		Th_args *arg_st = calloc(1, sizeof(Th_args));
		if (!arg_st)
		{
			__er_calloc(F, L - 3);
			close_file(1, fd_socket);
			free_memory_object(SH_ILOCK);
			return 1;
		}

		arg_st->socket_client = fd_client;
		arg_st->data_from_socket = strdup(instruction);

		/*put the function in a task que*/
		void *(*interface)(void *) = principal_interface;

		task_db *task = calloc(1, sizeof(task_db));
		task->interface = interface;
		task->arg = (void *)arg_st;

		if (!enqueue(pool.tasks, (void *)task))
		{
			printf("q_init() failed, %s:%d,\n", F, L - 2);
			close_file(2, fd_socket, fd_client);
			free_memory_object(SH_ILOCK);
			break;
		}
		pthread_cond_signal(&pool.notify);
		memset(instruction, 0, buff_size);
	}
	/*handle a gracefull crash*/

	return 0;
}
