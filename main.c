#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include<string.h>
#include <stdlib.h>
#include <fcntl.h>

//#include "test.h"
#include "test3.h"

int main(int argc,char *argv[]){
	printf("Start:\n");

	//test();
	//test3();

	test4(argc,argv);
/*
	int fd;
    int new_fd;

    fd = open("./test.file", O_RDWR | O_CREAT | O_TRUNC, 0666);
    printf("fd = %d\n", fd);

    dup2(fd, 1);
	if(fd != STDOUT_FILENO){
		printf("error\n");
		close(fd);
	}
    printf("hello world!!\n");
*/
	printf("End\n");
	return 0;
}
