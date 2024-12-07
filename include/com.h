/*
    HEADER FOR SOCKET AND EXTERNAL WORD COMUNICATION
        here you have the prototypes of the fucntions used in the main program to make the underling
        database connected to teh world

author: Lorenzo Piombini
*/
#ifndef _COM_H_
#define _COM_H_

int open_socket();
unsigned char listen_set_up(int *fd_sock);
unsigned char accept_instructions(int *fd_sock, int *client_sock, char *instruction_buff, int buff_size);

#endif
