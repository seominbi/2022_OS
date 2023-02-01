//https://github.com/belfazt/xv6/blob/master/login.c
//https://stackoverflow.com/questions/47250441/add-a-generic-file-in-xv6-makefile   -txt파일 등록
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

char userID[16][32];
char pwdID[16][32];

char *username;
char *password;

void get_user_list(){
    int fd;
    if((fd=open("list.txt",O_RDONLY))<0){
        printf(1, "login.txt can't open\n");
		exit();
    }


    for(int j=0;j<10;j++){
        int i, cc;
        char c;
        char buf[100];
        for(i=0; i+1 < 32; ){
            cc = read(fd, &c, sizeof(c));
            if(cc < 1)
                break;
            if(c==' ')
                break;
            buf[i++] = c;
        }
        buf[i] = '\0';
        strcpy(userID[j],buf);

        for(i=0; i+1 < 32; ){
            cc = read(fd, &c, sizeof(c));
            if(cc < 1)
                break;
            if(c == '\n' || c == '\r')
                break;
            buf[i++] = c;
        }
        buf[i] = '\0';
        strcpy(pwdID[j],buf);
    }

    /*int i,j, cc;
    char c;
    while(1){
        for(i=0; i+1 < 1024; ){
            cc = read(fd, &c, sizeof(c));    //read(fd, &buf, sizeof(buf))
            if(cc < 1)  //end file
                break;
            
            if(c==' ')
                break;

            userID[i++] = c;
        }
        if(cc < 1)  //end file
            break;
        for(j=0;j+1<1024; ){
            cc = read(fd, &c, sizeof(c));    //read(fd, &buf, sizeof(buf))
            if(cc < 1)  //end file
                break;
            
            if(c == '\n' || c == '\r')  //end line
                break;
            
            pwdID[j++] = c;
        }

        if(cc < 1)  //end file
            break;

        if(c == '\n' || c == '\r'){
            userID[i] = '\0';
            pwdID[j]='\0';
        }
    }*/
    
}

int check_idpw(){
    //입력받은 username, password와 list.txt 비교
    //로그인 성공이면 1리턴, 실패면 0리턴

    for(int j=0;j<10;j++){
        if((strcmp(userID[j],username)==0)&&(strcmp(pwdID[j],password)==0)){
            return 1;
        }
    }
    return 0;
    
}

int main(int argc, char*argv[]){
    get_user_list();
    /*for(int z=0;z<16;z++){
        printf(1,"%s %s\n",userID[z], pwdID[z]);
    }*/

    int login=1;
    while(login){
        printf(1,"Username: ");
        username = gets("username", 32);    
        username[strlen(username)-1]='\0';  //엔터 버리기
        printf(1,"Password: ");
        password = gets("password", 32);    
        password[strlen(password)-1]='\0';  //엔터 버리기
        //printf(1,"%s %s\n",username, password);

        if(check_idpw() ==1){
            //exec("sh", argv);
            //printf(1, "login success\n");
            login=0;
        }else{
            //printf(1, "login fail\n");
        }
    }
    exec("sh", argv);

    return 0;
}