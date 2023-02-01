// init: The initial user-level program

#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

char *argv[] = { "sh", 0 };

int
main(void)
{
  int pid, wpid;

  if(open("console", O_RDWR) < 0){  
    mknod("console", 1, 1); 
    open("console", O_RDWR);
  }
  dup(0);  // stdout
  dup(0);  // stderr

  /*for(;;){  //loop
    //printf(1, "init: starting login\n");
    printf(1, "init: starting sh\n");
    pid = fork(); //스레드생성. 성공 시 자식 프로세스 생성 후 부모 프로세서와 함께 돌아감.
    if(pid < 0){
      printf(1, "init: fork failed\n");
      exit();
    }
    if(pid == 0){ //자식 프로세스에서 돌아가는 코드
      //exec("ssu_login",argv);
      exec("sh", argv); //sh파일을 argv에 따라 실행? 덮어씀?
      //printf(1, "init: exec login failed\n");
      printf(1, "init: exec sh failed\n");
      exit();
    }
    while((wpid=wait()) >= 0 && wpid != pid)  //자식프로세스가 종료될 떄까지 부모가 while문 돌면서 기다림
      printf(1, "zombie!\n");
  }*/

    printf(1, "init: starting login\n");
    pid = fork(); //스레드생성. 성공 시 자식 프로세스 생성 후 부모 프로세서와 함께 돌아감.
    if(pid < 0){
      printf(1, "init: fork failed\n");
      exit();
    }
    if(pid == 0){ //자식 프로세스에서 돌아가는 코드
      exec("ssu_login",argv);
      printf(1, "init: exec login failed\n");
      exit();
    }
    while((wpid=wait()) >= 0 && wpid != pid)  //자식프로세스가 종료될 떄까지 부모가 while문 돌면서 기다림
      printf(1, "zombie!\n");
}
