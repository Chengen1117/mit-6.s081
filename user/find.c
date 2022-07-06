#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--);
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p)+1);
  
  //(*(buf+strlen(p)))='\0';
  return buf;
}

void 
find(char *path,char *target){
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path,0))< 0 ){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }
  
 if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }
  //printf("path %s %d\n",path,st.type);
  switch(st.type){
  case T_FILE:
   // printf("file %s\n",path);
    if(strcmp(fmtname(path),target)==0){
      printf("%s\n",path);
    }
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("find: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      //printf("path %s\n",path);
      //printf("%s!\n",de.name);
      if(de.inum == 0)
        continue;
      if(strcmp(de.name,".")==0||strcmp(de.name,"..")==0)
        continue;
  
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      find(buf,target);
    }
    break;

  }
  close(fd);
}
int
main(int argc, char *argv[])
{
  //参数
  
  if(argc < 3){
    fprintf(2, "Usage: find need 3..\n");
    exit(1);
  }
  find(argv[1],argv[2]);
  
  exit(0);
}
