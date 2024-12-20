#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h> /* for sockaddr_in */
#include <unistd.h>		/* for read()*/
#include "debug.h"
#include "com.h"
#include "file.h" /* for close_file() */

int open_socket()
{
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == -1)
	{
		perror("socket: ");
		return fd;
	}
	return fd;
}

unsigned char listen_set_up(int *fd_sock)
{
	struct sockaddr_in server_info = {0};
	server_info.sin_family = AF_INET;
	server_info.sin_addr.s_addr = 0;
	server_info.sin_port = htons(5555);

	*fd_sock = open_socket();
	if (*fd_sock == -1)
	{
		printf("open_socket(), failed, %s:%d.\n", F, L - 3);
		return 0;
	}

	/*bind to the port*/
	if (bind(*fd_sock, (struct sockaddr *)&server_info, sizeof(server_info)) == -1)
	{
		perror("bind: ");
		printf("bind() failed, %s:%d.\n", F, L - 3);
		close_file(1, *fd_sock);
		return 0;
	}

	if (listen(*fd_sock, 0) == -1)
	{
		perror("listen: ");
		printf("listen() failed, %s:%d.\n", F, L - 3);
		close_file(1, fd_sock);
		return 0;
	}

	return 1;
}

unsigned char accept_instructions(int *fd_sock, int *client_sock, char *instruction_buff, int buff_size)
{
	struct sockaddr_in client_info = {0};
	socklen_t client_size = sizeof(client_info);

	*client_sock = accept(*fd_sock, (struct sockaddr *)&client_info, &client_size);
	if (*client_sock == -1)
	{
		perror("accept: ");
		printf("accept() failed, %s:%d.\n", F, L - 2);
		return 0;
	}

	int instruction_size = read(*client_sock, instruction_buff, buff_size);
	if (instruction_size <= 0)
	{
		printf("%s() failed to read instruction, or socket is closed, %s:%d.\n", __func__, F, L - 3);
		return 0;
	}

	if (instruction_size > buff_size)
	{
		printf("data to large!.\n");
		return 0;
	}

	/*this is debug statment*/
	printf("read %d bytes from buffer.\n", instruction_size);

	/* erase the \n at the end of the json string */
	/* if a '\n' is present at the end or even a ' ' after the last '}' in the json string is present
		the json string check will failed and therefore the instruction on the database will not execute*/
	instruction_buff[instruction_size - 1] = '\0';
	return 1;
}
