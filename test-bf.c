 1 #include <stdio.h>
  2 #include <stdlib.h>
  3 #include <unistd.h>
  4 #include <fcntl.h>
  5 #include <stropts.h>
  6 #include <string.h>
  7 #include <errno.h>
  8 #include <sys/types.h>
  9 #include <asm/ioctl.h>
 10 
 11 #define TIOCGPTN    _IOR('T',0x30,  unsigned int) /* Get Pty Number (of pty-mux device) */
 12 #define TIOCSPTLCK    _IOW('T',0x31, int)  /* Lock/unlock Pty */
 13 
 14 #ifndef _HAS_OPENPT
 15 int posix_openpt(int oflag)
 16 {
 17     int     fdm;
 18     fdm = open("/dev/ptmx",oflag);
 19     return fdm;
 20 }
 21 #endif
 22 
 23 #ifndef _HAS_PTSNAME
 24 char *ptsname(int fdm)
 25 {
 26     static char     pts_name[16];
 27     int             sminor;
 28     if(ioctl(fdm,TIOCGPTN,&sminor) < 0)
 29         return NULL;
 30     snprintf(pts_name,sizeof(pts_name),"/dev/pts/%d",sminor);
 31     return pts_name;
 32 }
 33 #endif
 34 
 35 #ifndef _HAS_GRANTPT
 36 int grantpt(int fdm)
 37 {
 38     char *pts_name;
 39     pts_name = ptsname(fdm);
 40     return chmod(pts_name,S_IRUSR | S_IWUSR | S_IWGRP);
 41 }
 42 #endif
 43 
 44 #ifndef _HAS_UNLOCKPT
 45 int unlockput(int fdm)
 46 {
 47     int lock = 0;
 48     return (ioctl(fdm,TIOCSPTLCK,&lock));
 49 }
 50 #endif
 51 int ptym_open(char *pts_name,int pts_namesz)
 52 {
 53     char    *ptr;
 54     int     fdm;
 55     strncpy(pts_name,"/dev/ptmx",pts_namesz);
 56     pts_name[pts_namesz-1] = '\0';
 57     if((fdm = posix_openpt(O_RDWR)) < 0)
 58         return -1;
 59     if(grantpt(fdm) < 0)
 60     {
 61         close(fdm);
 62         return -2;
 63     }
 64     if(unlockput(fdm) < 0)
 65     {
 66         close(fdm);
 67         return -3;
 68     }
 69     if((ptr = ptsname(fdm)) < 0)
 70     {
 71         close(fdm);
 72         return -4;
 73     }
 74     strncpy(pts_name,ptr,pts_namesz);
 75     pts_name[pts_namesz-1] = '\0';
 76     return fdm;
 77 }
 78 int ptys_open(char *pts_name)
 79 {
 80     int     fds,setup;
 81     if((fds = open(pts_name,O_RDWR)) < 0)
 82         return -5;
 83     return fds;
 84 }
 85 int main()
 86 {
 87     int     fdm,fds;
 88     char    slave_name[20];
 89     fdm = ptym_open(slave_name,sizeof(slave_name));
 90     if(fdm<0)
 91     {
 92         perror("ptym_open() error");
 93         exit(-1);
 94     }
 95     printf("open master device successfully.\n");
 96     printf("slave device name is:%s\n",slave_name);
 97     fds = ptys_open(slave_name);
 98     if(fds < 0)
 99     {
100         perror("ptys_open() error");
101         exit(-1);
102     }
103     printf("open slave device successfully.\n");
104     exit(0);
105 }
