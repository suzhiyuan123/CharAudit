 #include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stropts.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <asm/ioctl.h>
  
#define TIOCGPTN    _IOR('T',0x30,  unsigned int) /* Get Pty Number (of pty-mux device) */
#define TIOCSPTLCK    _IOW('T',0x31, int)  /* Lock/unlock Pty */
  
#ifndef _HAS_OPENPT
int posix_openpt(int oflag)
{
	int     fdm;
	fdm = open("/dev/ptmx",oflag);
	return fdm;
}
#endif
  
#ifndef _HAS_PTSNAME
char *ptsname(int fdm)
{
	static char     pts_name[16];
	int             sminor;
	if(ioctl(fdm,TIOCGPTN,&sminor) < 0)
		return NULL;
	snprintf(pts_name,sizeof(pts_name),"/dev/pts/%d",sminor);
	return pts_name;
}
#endif
  
#ifndef _HAS_GRANTPT
int grantpt(int fdm)
{
	char *pts_name;
	pts_name = ptsname(fdm);
	return chmod(pts_name,S_IRUSR | S_IWUSR | S_IWGRP);
}
#endif
  
#ifndef _HAS_UNLOCKPT
int unlockput(int fdm)
{
	int lock = 0;
	return (ioctl(fdm,TIOCSPTLCK,&lock));
}
 #endif
  int ptym_open(char *pts_name,int pts_namesz)
  {
      char    *ptr;
      int     fdm;
      strncpy(pts_name,"/dev/ptmx",pts_namesz);
      pts_name[pts_namesz-1] = '\0';
      if((fdm = posix_openpt(O_RDWR)) < 0)
          return -1;
      if(grantpt(fdm) < 0)
      {
          close(fdm);
          return -2;
      }
      if(unlockput(fdm) < 0)
      {
          close(fdm);
          return -3;
      }
      if((ptr = ptsname(fdm)) < 0)
      {
          close(fdm);
          return -4;
      }
      strncpy(pts_name,ptr,pts_namesz);
      pts_name[pts_namesz-1] = '\0';
      return fdm;
  }
  int ptys_open(char *pts_name)
  {
      int     fds,setup;
      if((fds = open(pts_name,O_RDWR)) < 0)
          return -5;
      return fds;
  }
  int test2()
  {
      int     fdm,fds;
      char    slave_name[20];
      fdm = ptym_open(slave_name,sizeof(slave_name));
      if(fdm<0)
      {
          perror("ptym_open() error");
          exit(-1);
      }
      printf("open master device successfully.\n");
      printf("slave device name is:%s\n",slave_name);
      fds = ptys_open(slave_name);
      if(fds < 0)
      {
		perror("ptys_open() error");
		exit(-1);
	}
	printf("open slave device successfully.\n");
	exit(0);
}
