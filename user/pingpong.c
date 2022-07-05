#include "kernel/types.h"
#include "user/user.h"
int
main(int argc, char *argv[])
{
  int p[2];
  pipe(p);
  if (fork()==0) //子进程
  {
    /* code */
    char ch[5];
    int pid=getpid();
    read(p[0],ch,5);
    printf("%d: received %s\n",pid,ch);
    write(p[1],"pong",5);
    

  }else{
    write(p[1],"ping",5);
    wait((int *)0);
    char ch[5];
    int pid=getpid();
    read(p[0],ch,5);
    printf("%d: received %s\n",pid,ch);

  }
  close(p[0]);
  close(p[1]);
  exit(0);
 
}
