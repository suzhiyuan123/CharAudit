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
			system("echo \"111\" >> err.log");

		}

		if((fds = ptys_open(pts_name)) < 0){
			err_sys("can't open slave pty");
			system("echo \"222\" >> err.log");
		}
		close(fdm);

#if defined (TIOCSCTTY)
		if(ioctl(fds,TIOCSCTTY,(char *)0) < 0)
		{
			perror("TIOCSCTTY error");
			exit(-1);
		}
#endif




        if(dup2(fds,STDIN_FILENO) != STDIN_FILENO)
        {
             perror("dups error to stdin");
            exit(-1);
         }
         if(dup2(fds,STDOUT_FILENO) != STDOUT_FILENO)
         {
             perror("dups error to stdout");
             exit(-1);
        }
        if(dup2(fds,STDERR_FILENO) != STDERR_FILENO)
         {
            perror("dups error to stderr");
             exit(-1);
         }
         if(fds != STDIN_FILENO && fds != STDOUT_FILENO && fds != STDERR_FILENO)
             close(fds);

	char tmp[128];

	read(fds,tmp,128);
system("echo \"fds error111 \" >> err.log");
	sprintf(tmp,"%s","1234567890");
	write(fds,tmp,strlen(tmp));


/*
		system("echo \"444\" >> err.log");
		char tmp[128];
		gets(tmp);
system("echo \"555\" >> err.log");
		sleep(1);
		strcpy(tmp,"Hi,i am sub!");
		char cmd[128];
		sprintf(cmd,"echo \"%s\" >> err.log",tmp);
system("echo \"666\" >> err.log");
		system(cmd);
		printf("sdfsdfsdfsdf\n");
		//puts(tmp);
*/
		return 0;
			

	}else{
		*ptrfdm = fdm;
		return pid;
	}

}

int test3(){
	int ptym;
	char ptys[64];
	
	pty_fork(&ptym,ptys,64);
	printf("ptys:%s\n",ptys);
	sleep(1);
	char tmp[128];
	gets(tmp);
	int ret = write(ptym,tmp,strlen(tmp));
	printf("write ret:%d\n",ret);
//write(ptym,"\r\n",3);
	ret = read(ptym,tmp,128);
	printf("read ret:%d\n",ret);
	puts(tmp);

}
