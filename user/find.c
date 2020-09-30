#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

// #include <stdio.h>
//         #include <unistd.h>
//        #include <sys/types.h>
//#include <stdlib.h>
#define stderr 2
#define stdout 1
#define stdin 0

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  buf[strlen(p)] = 0;
  return buf;
}

void
find(char *path, char*pattern) {
	// fprintf(2, "%s %s\n", path, pattern);
	char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;
	char *fname;
  if((fd = open(path, 0)) < 0){
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }
	// printf("%s %s %d" , path, pattern, st.type);
  switch(st.type){
  case T_FILE:
		fname	= fmtname(path);
		if(strcmp(fname, pattern) == 0){
			printf("%s\n", path);
		}else{
			//printf("$s", path);
		}
    
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("ls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf + strlen(buf);
    if (*p != '/'){
			*p = '/';
		}
		p ++;
		*p = 0;
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf("ls: cannot stat %s\n", buf);
        continue;
      }
			fname	= fmtname(buf);
      if (fname[0] == '.'|| fname[1] == '.'){
				continue;
			}else {
				// printf("%s|\n", fname);
			}
			find(buf, pattern);
    }
    break;
  }
  close(fd);
}

int main(int argc, char *argv[]) {

  if(argc != 3){
    fprintf(stderr, "usage: find path pattern\n");
    exit(0);
  }
  find(argv[1], argv[2]);
  exit(0);
}