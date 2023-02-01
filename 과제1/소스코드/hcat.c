#include "types.h"
#include "stat.h"
#include "user.h"

char buf;
int row;

void
cat(int fd)
{
  int n;
  int cnt=1;

  while((n = read(fd, &buf, sizeof(buf))) > 0) {  //read one char in file
    if((cnt==row)&&(write(1, &buf, n) != n)) { //then print
        printf(1, "cat: write error\n");
        exit();
      }
    if(buf!='\n'){  //if char is not enter
      ;
    }else{ //if char is enter
      cnt++;
    }
  }
  if(n < 0){
    printf(1, "cat: read error\n");
    exit();
  }
}

int
main(int argc, char *argv[])
{
  int fd;

  if(argc <= 1){
    cat(0);
    exit();
  }

  if(argc!=3){
    printf(1, "input error\n");
    exit();
  }

    if((fd = open(argv[2], 0)) < 0){
      printf(1, "cat: cannot open %s\n", argv[2]);
      exit();
    }

    char *s=argv[1];
    while('0' <= *s && *s <= '9')
      row = row*10 + *s++ - '0';

    printf(1, "argv[0]: %s,argv[1]: %s,argv[2]: %s, row : %d\n", argv[0], argv[1], argv[2],strlen(argv[1]));

    cat(fd);
    close(fd);
  
  exit();
}