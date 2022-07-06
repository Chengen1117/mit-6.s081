#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int p1[2];   //从父进程读的pipe
  int p2[2];   //向子进程写的pipe
  int tem,base;
  pipe(p2);
  if (fork()!=0)
  {
    close(p2[0]);
    for (int i = 2; i <= 35; i++)
    {
        write(p2[1],&i,1);
    }
    close(p2[1]);
  }else{
    p1[0]=p2[0];
    p1[1]=p2[1];
    close(p1[1]);
    while (read(p1[0],&base,1)!=0)
    {   
        
        pipe(p2);
        if (fork()!=0)
        {
            
            printf("prime %d\n",base);
            while (read(p1[0],&tem,1))
            {
                if(tem%base!=0){
                    write(p2[1],&tem,1);
                }
            }
            close(p2[0]);
            close(p2[1]);
            wait((int *)0);
            exit(0);
        }else{          
            p1[0]=p2[0];
            p1[1]=p2[1];
            close(p1[1]);
        }
    }
  }
  wait((int *)0);
  exit(0);

}
