#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "syscall.h"
#include "param.h"

int main(int argc, char*argv[]){
    if(argc <= 1){
        exit();
    }

    // if(argc!=3){
    //     printf(1, "input error\n");
    //     exit();
    // }
    //printf(1, "argv[0]: %s,argv[1]: %s,argv[2]: %s, maskLength : %d\n", argv[0], argv[1], argv[2],strlen(argv[1]));
    
    int mask=0;
    char *s=argv[1];
    while('0' <= *s && *s <= '9')
      mask = mask*10 + *s++ - '0';
    //printf(1, "argv[0]: %s,argv[1]: %s,argv[2]: %s, maskLength : %d, mask : %d\n", argv[0], argv[1], argv[2],strlen(argv[1]),mask);

  int i;
	char *command[MAXARG];
  if (trace(mask) < 0)
	{
		printf(1, "ssu_trace fail\n");
		exit();
	}

	for (i = 2; i < argc && i < MAXARG; i++)
	{
		command[i - 2] = argv[i];
	}
	exec(command[0],command);
	exit();
}