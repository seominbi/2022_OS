#include "types.h"
#include "stat.h"
#include "user.h"

#include "fcntl.h"
#include "syscall.h"
#include "param.h"

#define PNUM 5  //proess 개수
#define PRINT_CYCLE 10000000   //process 정보 출력 주기
#define TOTAL_COUNTER 500000000 //프로세스 종료되기까지 시간



void sdebug_func(void)
{
    //implement
    //int sdebug_weight=1;
    printf(1,"start sdebug command\n");

    for(int i=0;i<PNUM;i++){
        int pid;
        pid = fork(); //스레드생성. 성공 시 자식 프로세스 생성 후 부모 프로세서와 함께 돌아감.
        if(pid < 0){
            printf(1, "sdebug: fork failed\n");
            exit();
        }else if(pid==0){
            //자식프로세스인 경우
            //weightset(), scheduler()호출
            int starttik,endtick;
            starttik=uptime();
            weightset(i+1);
            long counter=0;
            // int first=1;
            //     for(int print_counter=PRINT_CYCLE;print_counter>=0;print_counter--){
            //         if(print_counter==0){
            //             if(first){
            //                 endtick=uptime();
            //                 printf(1,"Child %d created with %dweight %dtimes\n",getpid(),i+1,((endtick-starttik)*10));
            //                 first=0;
            //             }
            //             print_counter=PRINT_CYCLE;
            //             counter+=PRINT_CYCLE;
            //         }
            //         if(counter>=TOTAL_COUNTER){
            //             printf(1,"%dcounter done..\n",counter);
            //             break;
            //         }
            //     }
            
            // printf(1, "pid%d is terminated\n",getpid());
            // exit();
            while(1){
                counter++;
                if(counter==PRINT_CYCLE){
                    endtick=uptime();
                    printf(1,"PID : %d WEIGHT : %d TIMES : %d ms\n",getpid(),i+1,((endtick-starttik)*10));
                }
                if(counter>TOTAL_COUNTER){
                    break;
                }
            }
            printf(1, "PID : %d is terminated\n",getpid());
            exit();   
        }
    }

    for(int n=0; n <PNUM; n++){
        wait();
    }

    printf(1,"end of sdebug command\n");
    return;
}
int main(int argc, char **argv)
{
    
    sdebug_func();
    exit();
}