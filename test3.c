#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include<string.h>

#define err_sys printf

pid_t
pty_fork(int *ptrfdm,char *slave_name,int slave_namez
		/*const struct termios *slave_termios,*/
		/*const struct winsize *slave_winsize*/
	)
{
	int fdm,fds;
	pid_t pid;
	char pts_name[20];
	
	if((fdm = ptym_open(pts_name,sizeof(pts_name))) < 0){
		err_sys("can't open master pty:%s,error %d",pts_name,fdm);
	}

	if(slave_name != NULL){
		strncpy(slave_name,pts_name,slave_namez);
		slave_name[slave_namez - 1] = '\0';
	}

	if((pid = fork()) < 0){

		return -1;
	}else if (pid == 0){

		if(setsid() < 0){
			err_sys("setsid error");

		}

		if((fds = ptys_open(pts_name)) < 0){
			err_sys("can't open slave pty");

		}
		close(fdm);

		dup2(fds,STDIN_FILENO);
		dup2(fds,STDOUT_FILENO);
		dup2(fds,STDERR_FILENO);

		if(fds != STDIN_FILENO||fds != STDOUT_FILENO||fds != STDERR_FILENO)
			close(fds);

		return 0;
			

	}else{
		*ptrfdm = fdm;
		return pid;
	}

}

int test3(){
	char ptym[64];
	char ptys[64];
	
	pty_fork(ptym,ptys,64);
}
