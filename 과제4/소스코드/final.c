#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>

int *buf;   //page stream
int *write; //write bit for esc
int *read;  //read bit for esc
int algo[8]={0,};   //select page replace algo
int pageframesNum; //페이지 프레임 개수
int data;   //데이터입력방식
int REFERENCE_STRING_NUM;    //참조 페이지 스트링 개수

void fifo1();
void lifo1();
void optimal1();
void lru1();
void lfu1();
void sc1();
void esc1();
void execAlgo();

void execAlgo(){
    if(algo[0]==1)
        optimal1();
    if(algo[1]==1)
        fifo1();
    if(algo[2]==1)
        lifo1();
    if(algo[3]==1)
        lru1();
    if(algo[4]==1)
        lfu1();
    if(algo[5]==1)
        sc1();
    if(algo[6]==1)
        esc1();
    if(algo[7]==1){
        optimal1();
        fifo1();
        lifo1();   
        lru1();
        lfu1();
        sc1();
        esc1();
    }      
    return;  
}

int main(){
    printf("A. Page Replacement 알고리즘 시뮬레이터를 선택하시오 (최대 3개)\n");
    printf("(1)Optimal (2)FIFO (3)LIFO (4)LRU (5)LFU (6)SC (7)ESC (8)All\n");
    while(1){
        for(int j=0;j<8;j++){
            algo[j]=0;  //배열 초기화
        }
        char selectAlgo[100];
        scanf("%[^\n]s",selectAlgo);
        getchar();
        char *ptr = strtok(selectAlgo, " ");      // " " 공백 문자를 기준으로 문자열을 자름, 포인터 반환
        int cnt=0;
        while (ptr != NULL)               // 자른 문자열이 나오지 않을 때까지 반복
        {
            cnt++;
            // printf("%d : %s %d\n",cnt, ptr, atoi(ptr));          // 자른 문자열 출력
            if(atoi(ptr)<=8 && atoi(ptr)>=1){
                algo[atoi(ptr)-1]=1;
            }else{
                cnt=-9999;
                break;
            }
            ptr = strtok(NULL, " ");      // 다음 문자열을 잘라서 포인터를 반환
        } //cnt=3개입력하면 3개로 종료.

        //all을 제외하고 최대 3개의 알고리즘 선택
        if(cnt<=3&&cnt>=1){
            if(algo[7]==1){
                if(cnt==1){
                    // printf("%d %d %d %d %d %d %d %d\n",algo[0],algo[1],algo[2],algo[3],algo[4],algo[5],algo[6],algo[7]);
                    break;
                }else{
                    // printf("%d %d %d %d %d %d %d %d\n",algo[0],algo[1],algo[2],algo[3],algo[4],algo[5],algo[6],algo[7]);
                    printf("입력 에러, ALL을 제외하고 최대 3개의 알고리즘(1~8)을 선택할 수 있습니다.\n");
                    continue;
                }
            }else{
                // printf("%d %d %d %d %d %d %d %d\n",algo[0],algo[1],algo[2],algo[3],algo[4],algo[5],algo[6],algo[7]);
                break;
            }
        }else{
            // printf("%d %d %d %d %d %d %d %d\n",algo[0],algo[1],algo[2],algo[3],algo[4],algo[5],algo[6],algo[7]);
            printf("입력 에러, ALL을 제외하고 최대 3개의 알고리즘(1~8)을 선택할 수 있습니다.\n");
            continue;
        }
    }

    printf("B. 페이지 프레임의 개수를 입력하시오 (3~10)\n");
    while(1){
        scanf("%d",&pageframesNum);
        // printf("pageframesNum : %d\n",pageframesNum);
        if(pageframesNum>=3 && pageframesNum<=10){    //입력 에러 처리
            break;
        }
        printf("입력 에러, 페이지 프레임 개수는 (3~10)입니다.\n");
    }

    printf("C.데이터 입력 방식을 선택하시오 (1,2)\n");
    printf("(1)랜덤하게 생성 (2)사용자 생성 파일 오픈\n");
    while(1){
        scanf("%d",&data);
        // printf("data : %d\n",data);
        if(data>=1 && data<=2){     //입력 에러 처리
            break;
        }
        printf("입력 에러, 데이터 입력 방식 선택은 (1,2)입니다.\n");
    }
    FILE *fp;
    if((fp=fopen("result.txt","w+"))==NULL){   //읽기 쓰기 모두 가능하도록 오픈. 이미 파일이 존재하면 파일 길이 0이 됨. 파일이 존재 하지 않으면 creat.
        fprintf(stderr,"fopen error for %s\n","result.txt");
        exit(1);
    }
    fclose(fp);
    switch(data){
        case 1:{
            REFERENCE_STRING_NUM=500;
            int rand_reference_string[REFERENCE_STRING_NUM];
            memset(rand_reference_string, 0, sizeof(rand_reference_string[0])*REFERENCE_STRING_NUM);
            buf=rand_reference_string;
            int rand_r[REFERENCE_STRING_NUM];   //Read bit
            memset(rand_r, 0, sizeof(rand_r[0])*REFERENCE_STRING_NUM);
            read=rand_r;
            int rand_w[REFERENCE_STRING_NUM];   //write bit
            memset(rand_w, 0, sizeof(rand_w[0])*REFERENCE_STRING_NUM);
            write=rand_w;
            srand(time(NULL));  //랜덤하게 참조 스트링 생성.
            for (int i = 0; i < REFERENCE_STRING_NUM; i++)
                rand_reference_string[i]=(rand() % 30) + 1;
            // for (int i = 0; i < REFERENCE_STRING_NUM; i++)
            //     printf("%d ",buf[i]);
            // printf("\n");
            for (int i = 0; i < REFERENCE_STRING_NUM; i++){
                if(rand()%2==0){
                    rand_r[i]=0;
                    rand_w[i]=1;
                }
                else{
                    rand_r[i]=1;
                    rand_w[i]=0;
                }
            }
            // for (int i = 0; i < REFERENCE_STRING_NUM; i++)
            //     printf("%d ",rand_r[i]);
            // printf("\n");
            // for (int i = 0; i < REFERENCE_STRING_NUM; i++)
            //     printf("%d ",rand_w[i]);
            // printf("\n");
            execAlgo();
            // printf("case 1 : success\n");
            break;
        }
        case 2:{
            printf("(1)파일명을 인자로 입력 (2)프로그램 내부에서 스트림 파일 생성\n");
            int data2=0;
            while(1){
                scanf("%d",&data2);
                // printf("data2 : %d\n",data2);
                if(data2>=1 && data2<=2){     //입력 에러 처리
                    break;
                }
                printf("입력 에러, 파일 입력 방식 선택은 (1,2)입니다.\n");
            }
            char *fname;
            if(data2==1){   //파일명 입력
                char filename[101];
                printf("파일명 : ");
                scanf("%s",filename);
                fname=malloc(strlen(filename) + 1);
                strcpy(fname, filename);
                fname[strlen(filename)] = '\0';
                // printf("파일명 : %s\n",fname);
            }else if(data2==2){ //프로그램 내부에서 파일 생성
                fname="stream.txt";
                FILE *fp1;
                if((fp1=fopen(fname,"w+"))==NULL){   //읽기 쓰기 모두 가능하도록 오픈. 이미 파일이 존재하면 파일 길이 0이 됨. 파일이 존재 하지 않으면 creat.
                    fprintf(stderr,"fopen error for %s\n",fname);
                    exit(1);
                }
                srand(time(NULL));  //랜덤하게 참조 스트링 생성.
                for (int i = 0; i < 500; i++){  //reference_string_num
                    fprintf(fp1, "%d\t", (rand() % 30) + 1);    //파일에 랜덤하게 생성한 참조 스트링을 저장.
                    fprintf(fp1, "%c\n", (rand() % 2)==0? 'R' : 'W');
                    // fprintf(fp1, "%c\n", (rand() % 2)==0? 'W' : '-');
                }
                fclose(fp1); //파일 닫기.
            }            
            FILE *fp2;
            if((fp2=fopen(fname,"r"))==NULL){ //읽기 전용으로 파일 오픈.
                fprintf(stderr,"fopen error for %s\n",fname);
                exit(1);
            }
            char *line = NULL;
            size_t size = 0;
            ssize_t len;
            int cnt_line=0; //파일 line 카운트.
            while ((len = getline(&line, &size, fp2)) != -1) {
                    // printf("Retrieved line of length %zu :\n", len);
                    // printf("%s", line);
                    cnt_line++; //파일 line 수 카운트.
            }
            // printf("line : %d\n",cnt_line);
            REFERENCE_STRING_NUM=cnt_line;

            int file_reference_string[REFERENCE_STRING_NUM];
            memset(file_reference_string, 0, sizeof(file_reference_string[0])*REFERENCE_STRING_NUM);
            buf=file_reference_string;
            int file_r[REFERENCE_STRING_NUM];   //Read bit
            memset(file_r, 0, sizeof(file_r[0])*REFERENCE_STRING_NUM);
            read=file_r;
            int file_w[REFERENCE_STRING_NUM];   //write bit
            memset(file_w, 0, sizeof(file_w[0])*REFERENCE_STRING_NUM);
            write=file_w;

            fseek(fp2, 0, SEEK_SET);    //file pointer 위치를 0으로 설정
            cnt_line=0;
            while ((len = getline(&line, &size, fp2)) != -1) {
                    // printf("Retrieved line of length %zu :\n", len);
                    // printf("%s", line);
                    if(strchr(line,'R')!=NULL)
                        read[cnt_line]=1;
                    if(strchr(line,'W')!=NULL)
                        write[cnt_line]=1;
                    file_reference_string[cnt_line++]=atoi(line);   //파일에 랜덤하게 생성한 참조 스트링을 읽어 배열에 저장.
            }
            free(line);  // getline에서 할당된 힙 메모리 해제

            // printf("page stream : ");
            // for (int i = 0; i < REFERENCE_STRING_NUM; i++)
            //     printf("%d ",file_reference_string[i]);
            // printf("\n");
            // for (int i = 0; i < REFERENCE_STRING_NUM; i++)
            //     printf("%d ",buf[i]);
            // printf("\n");
            // for (int i = 0; i < REFERENCE_STRING_NUM; i++)
            //     printf("%d ",read[i]);
            // printf("\n");
            // for (int i = 0; i < REFERENCE_STRING_NUM; i++)
            //     printf("%d ",write[i]);
            // printf("\n");

            fclose(fp2);
            execAlgo();
            // printf("case 2 : success\n");
            break;
        }
    }

    printf("D. 종료\n");
    
    return 0;
}

void esc1(){
    FILE *fp;
    if((fp=fopen("result.txt","a+"))==NULL){   //읽기 쓰기 모두 가능하도록 오픈. 이미 파일이 존재하면 파일 길이 0이 됨. 파일이 존재 하지 않으면 creat.
        fprintf(stderr,"fopen error for %s\n","result.txt");
        exit(1);
    }
    printf(">>>>>>>>>>>>>>>>>>>>ESC<<<<<<<<<<<<<<<<<\n");
    fprintf(fp, ">>>>>>>>>>>>>>>>>>>>ESC<<<<<<<<<<<<<<<<<\n");

    int pointer=0;
    int pageFaults = 0;
    int check1,check2,check3,check4;
    int pageStringNum=REFERENCE_STRING_NUM; 
    char c='?';
    int frame[pageframesNum];   //frame
    memset(frame, -1, sizeof(frame[0])*pageframesNum);
    int r[pageframesNum];   //R bit
    memset(r, 1, sizeof(r[0])*pageframesNum);
    int w[pageframesNum];   //W bit
    memset(w, 0, sizeof(w[0])*pageframesNum);

    pageStringNum=REFERENCE_STRING_NUM;    
    for(int i = 0; i < pageStringNum; i++)  
    {  
        check1=check2=check3=check4=0;
        for(int j = 0; j < pageframesNum; j++)
        {
            if(frame[j] == buf[i])  //page hit
            {
                r[j] = read[i]==1?1:r[j];   //set read bit
                w[j] = write[i]==1?1:w[j];     //set write bit
                check1=check2=check3=check4=1;
                c='H';
            }
        }

        if(!check1)  //page miss & no swapping
        {
            for(int j = 0; j < pageframesNum; ++j){
                if(frame[j] == -1){    //empty page
                    frame[j] = buf[i];   //just load page
                    r[j]=read[i];   //set r bit
                    w[j]=write[i];  //set w bit
                    r[j]=1;
                    check2 =check3=check4= 1;
                    pageFaults++;   //count pagefault
                    c='F';
                    pointer=(pointer + 1) % pageframesNum;  //set queue pointer to next page
                    break;
                }
            }
        }

        //goto
        LOOP:
        if(check2==0){  //page fault & swapping between (0,0) and incomming
            for(int j = 0; j < pageframesNum; ++j){
                if(((r[pointer]==0)&&(w[pointer]==0))) //if (0,0)
                {
                    frame[pointer] = buf[i];        //page replacement    
                    r[pointer]=read[i];
                    w[pointer]=write[i];
                    r[pointer]=1;
                    pointer=(pointer + 1) % pageframesNum;  //set queue pointer to next page
                    check3=check4= 1;
                    pageFaults++;   //count pagefault
                    c='F';
                    break;
                }
                // r[pointer] = 0; //if r bit == 1 then r bit=0
                // w[pointer] = 0; //if w bit == 1 then w bit=0
                pointer = (pointer + 1) % pageframesNum;    //set queue pointer to next page
            }
        }
                
        if(check3==0){  //page fault & swapping between (0,1) and incomming while (1,0),(1,1)->(0,0),(0,1)
            for(int j = 0; j < pageframesNum; ++j){
                if((((r[pointer]==0)&&(w[pointer]==1)))) //check (0,1) is exist
                {
                    frame[pointer] = buf[i];        //page replacement    
                    r[pointer]=read[i];
                    w[pointer]=write[i];
                    r[pointer]=1;
                    pointer=(pointer + 1) % pageframesNum;  //set queue pointer to next page
                    check4=1;
                    pageFaults++;   //count pagefault
                    c='F';
                    break;
                }
                else{
                    r[pointer] = 0; //if r bit == 1 then r bit=0
                    pointer = (pointer + 1) % pageframesNum;    //set queue pointer to next page
                }
            }
        }
        if(check4==0){
            //check2,3반복
            goto LOOP;
        }
                
        //print state
        printf("%2d(%d,%d)\t\t",buf[i],read[i],write[i]);  //print incomming page string
        fprintf(fp, "%2d(%d,%d)\t\t", buf[i],read[i],write[i]);
        for(int m = 0; m< pageframesNum; m++){
            // printf("%d(%d)\t",frame[m],second_chance[m]);
            if(frame[m] != -1){
                printf(" %2d(%d,%d)\t\t", frame[m],r[m],w[m]);
                fprintf(fp, "%2d(%d,%d)\t\t", frame[m],r[m],w[m]);
            }else{
                printf(" -(-,-)\t\t");
                fprintf(fp, " -(-,-)\t\t");
            }
        }
        printf("%c\n",c);
        fprintf(fp, "%c\n",c);
    }

    printf("ESC Total Page Faults : %d\n",pageFaults);
    fprintf(fp, "ESC Total Page Faults : %d\n", pageFaults);
    fclose(fp);
    return;
}

void sc1(){
    FILE *fp;
    if((fp=fopen("result.txt","a+"))==NULL){   //읽기 쓰기 모두 가능하도록 오픈. 이미 파일이 존재하면 파일 길이 0이 됨. 파일이 존재 하지 않으면 creat.
        fprintf(stderr,"fopen error for %s\n","result.txt");
        exit(1);
    }

    printf(">>>>>>>>>>>>>>>>>>>>SC<<<<<<<<<<<<<<<<<\n");
    fprintf(fp, ">>>>>>>>>>>>>>>>>>>>SC<<<<<<<<<<<<<<<<<\n");

    int pointer=0;
    int pageFaults = 0;
    int pageStringNum=0;
    char c='?'; //c means page hit(h)/fault(f)
    
    int frame[pageframesNum];   //frame
    memset(frame, -1, sizeof(frame[0])*pageframesNum);

    int second_chance[pageframesNum];   //R bit
    memset(second_chance, 0, sizeof(second_chance[0])*pageframesNum);

    pageStringNum=REFERENCE_STRING_NUM;    
    for(int i = 0; i < pageStringNum; i++)  
    {  
        int check=0;
        for(int j = 0; j < pageframesNum; j++)
        {
            
            if(frame[j] == buf[i])  //page hit
            {
                second_chance[j] = 1;   //set second chance bit = 1
                check=1;
                c='H';
            }
        }

        if(!check)  //page miss
        {
            while(1)
            {
                if(!second_chance[pointer]) //if secod chance==0
                {
                    frame[pointer] = buf[i];        //page replacement    
                    pointer=(pointer + 1) % pageframesNum;  //set queue pointer to next page
                    break;
                }
                second_chance[pointer] = 0; //if second chance == 1 then second chance=0
                pointer = (pointer + 1) % pageframesNum;    //set queue pointer to next page
            }
            pageFaults++;   //count pagefault
            c='F';
        }
        //print state
        printf("%2d\t",buf[i]);  //print incomming page string
        fprintf(fp, "%2d\t", buf[i]);
        for(int m = 0; m< pageframesNum; m++){
            // printf("%d(%d)\t",frame[m],second_chance[m]);
            if(frame[m] != -1){
                printf(" %2d(%d)\t", frame[m],second_chance[m]);
                fprintf(fp, "%2d(%d)\t", frame[m],second_chance[m]);
            }else{
                printf(" -(-) \t");
                fprintf(fp, " -(-) \t");
            }
        }
        printf("%c\n",c);
        fprintf(fp, "%c\n",c);
    }
    printf("SC Total Page Faults : %d\n",pageFaults);
    fprintf(fp, "SC Total Page Faults : %d\n", pageFaults);
    fclose(fp);
    return;
}

void lfu1(){
    FILE *fp;
    if((fp=fopen("result.txt","a+"))==NULL){   //읽기 쓰기 모두 가능하도록 오픈. 이미 파일이 존재하면 파일 길이 0이 됨. 파일이 존재 하지 않으면 creat.
        fprintf(stderr,"fopen error for %s\n","result.txt");
        exit(1);
    }

    printf(">>>>>>>>>>>>>>>>>>>>LFU<<<<<<<<<<<<<<<<<\n");
    fprintf(fp, ">>>>>>>>>>>>>>>>>>>>LFU<<<<<<<<<<<<<<<<<\n");

		int frequency[pageframesNum];   //counter
        memset(frequency, 0, pageframesNum*sizeof(frequency[0]));
		int frames[pageframesNum];
        memset(frames, -1, pageframesNum*sizeof(frames[0]));
		int len=REFERENCE_STRING_NUM;
        int pageFaults=0;
        char c='?';
		
		for(int i=0; i< len; i++){
			int check = 0;
			int min,minpos;
			for(int j=0; j<pageframesNum; j++){
				if (frames[j] == buf[i]){	//page hit
					frequency[j]++;	//increase frequency(counter)
					check = 1;
                    c='H';
					break;
				}
			}
			if(check == 0){	//page miss
				for(int j=0; j<pageframesNum; j++){
					if(frames[j] == -1){	//no swapping. just insert page.
						frames[j] = buf[i];
						frequency[j];	//not increase frequency
						check = 1;
                        pageFaults++;   //count page fault
                        c='F';
						break;
					}
				}
			}
			if(check == 0){ //page miss & swapping
				min=frequency[0];
				minpos=0;
				for(int k = 1; k< pageframesNum; k++){
					if(frequency[k] < min){	//select least frequency page
						min = frequency[k];
						minpos = k;
					}
				}
				frames[minpos] = buf[i];    //page swapping
				frequency[minpos] = 0;  //set frequency(counter)
                pageFaults++;   //count pagefault
                c='F';
			}

			//print state
            printf("%2d\t",buf[i]);  //print incomming page string
            fprintf(fp, "%2d\t", buf[i]);
			for(int m = 0; m< pageframesNum; m++){
                if(frames[m] != -1){
                    printf(" %2d(%d)\t", frames[m],frequency[m]);
                    fprintf(fp, "%2d(%d)\t", frames[m],frequency[m]);
                }else{
                    printf(" -(-) \t");
                    fprintf(fp, " -(-) \t");
                }

			}
			printf("%c\n",c);
            fprintf(fp, "%c\n",c);
		}
    printf("LFU Total Page Faults : %d\n", pageFaults);
    fprintf(fp, "LFU Total Page Faults : %d\n", pageFaults);
    fclose(fp);
}

void lru1(){
    FILE *fp;
    if((fp=fopen("result.txt","a+"))==NULL){   //읽기 쓰기 모두 가능하도록 오픈. 이미 파일이 존재하면 파일 길이 0이 됨. 파일이 존재 하지 않으면 creat.
        fprintf(stderr,"fopen error for %s\n","result.txt");
        exit(1);
    }

    printf(">>>>>>>>>>>>>>>>>>>>LRU<<<<<<<<<<<<<<<<<\n");
    fprintf(fp, ">>>>>>>>>>>>>>>>>>>>LRU<<<<<<<<<<<<<<<<<\n");

    int pageStringNum=REFERENCE_STRING_NUM;
    int frames[pageframesNum];
    int temp[pageframesNum];
    int counter = 0;
    int check1, check2, pageFaults = 0;
    char c='?';
        
    for(int i = 0; i < pageframesNum; ++i){  //initialize
        frames[i] = -1;
    }
        
    for(int i = 0; i < pageStringNum; ++i){
        check1 = 0;
        check2 = 0;
        
        for(int j = 0; j < pageframesNum; ++j){
            if(frames[j] == buf[i]){  //page hit
                counter++;  //increase time(=counter)
                temp[j] = counter;  //record time(=counter)
                check1 = 1;
                check2 = 1;
                c='H';
                break;
            }
        }
        
        if(check1 == 0){     //page fault & no swapping(just insert page)
            for(int j = 0; j < pageframesNum; ++j){
                if(frames[j] == -1){    //empty page
                    counter++;  //increase time(=counter)
                    pageFaults++;   //count page fault
                    frames[j] = buf[i];   //page replacement
                    temp[j] = counter;  //record time(=counter)
                    check2 = 1;
                    c='F';
                    break;
                }
            }
        }
        
        if(check2 == 0){     //page fault & swapping
            int minimum = temp[0];
            int pos=0;
            for(int j = 1; j < pageframesNum; ++j){
                if(temp[j] < minimum){  //select old used page
                    minimum = temp[j];  
                    pos = j;    //old used page's index
                }
            }
            counter++;  //increase time(=counter)
            pageFaults++;   //count page fault
            frames[pos] = buf[i]; //page replacement
            temp[pos] = counter;    //record time(=counter)
            c='F';
        }
        
        //print state
        printf("%2d\t",buf[i]);  //print incomming page string
        fprintf(fp, "%2d\t", buf[i]);
        for(int j = 0; j < pageframesNum; ++j){  //print pages state
            if(frames[j] != -1){
                printf("%2d\t", frames[j]);
                fprintf(fp, "%2d\t", frames[j]);
            }else{
                printf(" - \t");
                fprintf(fp, " - \t");
            }
        }
        printf("%c\n",c);
        fprintf(fp, "%c\n",c);
    }
    printf("LRU Total Page Faults : %d\n", pageFaults);
    fprintf(fp, "LRU Total Page Faults : %d\n", pageFaults);
    fclose(fp);
    return;
}

void optimal1(){
    FILE *fp;
    if((fp=fopen("result.txt","a+"))==NULL){   //읽기 쓰기 모두 가능하도록 오픈. 이미 파일이 존재하면 파일 길이 0이 됨. 파일이 존재 하지 않으면 creat.
        fprintf(stderr,"fopen error for %s\n","result.txt");
        exit(1);
    }

     printf(">>>>>>>>>>>>>>>>>>>>Optimal<<<<<<<<<<<<<<<<<\n");
    fprintf(fp, ">>>>>>>>>>>>>>>>>>>>Optimal<<<<<<<<<<<<<<<<<\n");

    int pageStringNum=REFERENCE_STRING_NUM;
    int frames[pageframesNum], temp[pageframesNum];
    int check1, check2, check3;
    int pos, max;
    int pageFaults = 0;
    char c='?';
      
    for(int i = 0; i < pageframesNum; ++i){
        frames[i] = -1;
    }
    
    for(int i = 0; i < pageStringNum; ++i){
        check1 = 0;
        check2 = 0;
        
        for(int j = 0; j < pageframesNum; ++j){
            if(frames[j] == buf[i]){    //page hit
                   check1 = 1;
                   check2 = 1;
                   c='H';
                   break;
               }
        }
        
        if(check1 == 0){ //page miss & no swapping(just insert page)
            for(int j = 0; j < pageframesNum; ++j){
                if(frames[j] == -1){    //no swap
                    pageFaults++;
                    frames[j] = buf[i]; //insert page
                    check2 = 1;
                    c='F';
                    break;
                }
            }    
        }
        
        if(check2 == 0){ //page miss & swapping
            check3 =0;
            for(int j = 0; j < pageframesNum; ++j){
                temp[j] = -1;
                for(int k = i + 1; k < pageStringNum; ++k){
                    if(frames[j] == buf[k]){
                        temp[j] = k;    //현재 프레임에 저장된 string이 이후에 등장하명 해당 index 저장, 아니면 -1.
                        break;
                    }
                }
            }
            
            for(int j = 0; j < pageframesNum; ++j){
                if(temp[j] == -1){  //optimal, 현재 프레임에 저장된 string 중 이후에 나오지 않는 string 존재.
                    pos = j;
                    check3 = 1;
                    break;
                }
            }
            
            if(check3 ==0){  //optimal, 현재 프레임에 저장된 string이 모두 이후에 등장함.
             max = temp[0];
             pos = 0;
            
                for(int j = 1; j < pageframesNum; ++j){
                    if(temp[j] > max){  //optimal, 현재 프레임에 저장된 string 중 가장 나중에 등장하는 string 찾기.
                    max = temp[j]; 
                    pos = j;
                    }
                }            
            }
            frames[pos] = buf[i];   //가장 나중에 사용될 string 을 page replacement
            pageFaults++;   //count page fault
            c='F';
        }
        
        //print state
        printf("%2d\t",buf[i]);
        fprintf(fp, "%2d\t", buf[i]);
        for(int j = 0; j < pageframesNum; ++j){
            if(frames[j] != -1){
                printf(" %2d\t", frames[j]);
                fprintf(fp, "%2d\t", frames[j]);
            }else{
                printf(" - \t");
                fprintf(fp, " - \t");
            }
        }
        printf("%c\n",c);
        fprintf(fp, "%c\n",c);
    }
    
    printf("Optimal Total Page Faults:\t%d\n", pageFaults);
    fprintf(fp, "Optimal Total Page Faults : %d\n", pageFaults);
    fclose(fp);
    return;
}

void lifo1(){
    FILE *fp;
    if((fp=fopen("result.txt","a+"))==NULL){   //읽기 쓰기 모두 가능하도록 오픈. 이미 파일이 존재하면 파일 길이 0이 됨. 파일이 존재 하지 않으면 creat.
        fprintf(stderr,"fopen error for %s\n","result.txt");
        exit(1);
    }

    printf(">>>>>>>>>>>>>>>>>>>>LIFO<<<<<<<<<<<<<<<<<\n");
    fprintf(fp, ">>>>>>>>>>>>>>>>>>>>LIFO<<<<<<<<<<<<<<<<<\n");

    int pageFaults = 0;
    int check;
    int pageStringNum=REFERENCE_STRING_NUM;
    char c='?';

    int stack[pageframesNum];   //create stack
    int top=-1;
    for(int m = 0; m < pageframesNum; m++)
    {
        stack[m] = -1;
    }
    for(int m = 0; m < pageStringNum; m++)
    {
        check=0;
        for(int n = 0; n < pageframesNum; n++) //PAGE FAULT 검사
        {
            if(buf[m] == stack[n])    //PAGE HIT
            {
                check=1;
                c='H';
            }
        }
        
        //FAULT인 경우 올바른 FRAME 위치에 저장
        if((pageFaults < pageframesNum) && (check == 0))  //PAGE FAULT&&NO SWAPPING
        {
            stack[++top]= buf[m];   //INSERT PAGE INTO STACK
            pageFaults++;   //COUNT PAGE FAULT
            c='F';
        }
        else if(check == 0)     //PAGE FAULT&SWAPPING
        {
            stack[top]= buf[m];     //INSERT PAGE INTO TOP OF STACK
            pageFaults++;   //COUNT PAGE FAULT
            c='F';
        }
        
        //PRINT STATE
        printf("%2d\t",buf[m]);
        fprintf(fp, "%2d\t", buf[m]);
        for(int n = 0; n < pageframesNum; n++)
        {
            if(stack[n] != -1){
                printf("%2d\t", stack[n]);
                fprintf(fp, "%2d\t", stack[n]);
            }else{
                printf(" - \t");
                fprintf(fp, " - \t");
            }
        }
        printf("%c\n",c);
        fprintf(fp, "%c\n",c);
    }
    printf("LIFO Total Page Faults : %d\n", pageFaults);
    fprintf(fp, "LIFO Total Page Faults : %d\n", pageFaults);
    fclose(fp);
}

void fifo1(){
    FILE *fp;
    if((fp=fopen("result.txt","a+"))==NULL){   //읽기 쓰기 모두 가능하도록 오픈. 이미 파일이 존재하면 파일 길이 0이 됨. 파일이 존재 하지 않으면 creat.
        fprintf(stderr,"fopen error for %s\n","result.txt");
        exit(1);
    }
    printf(">>>>>>>>>>>>>>>>>>>>FIFO<<<<<<<<<<<<<<<<<\n");
    fprintf(fp, ">>>>>>>>>>>>>>>>>>>>FIFO<<<<<<<<<<<<<<<<<\n");
    int pageFaults = 0;
    int check; 
    int pageStringNum= REFERENCE_STRING_NUM;
    char c='?';

    int frame[pageframesNum];
    for(int m = 0; m < pageframesNum; m++)
    {
        frame[m] = -1;
    }

    for(int m = 0; m < pageStringNum; m++)
    {
        check = 0;

        for(int n = 0; n < pageframesNum; n++)
        {
            if(buf[m] == frame[n])  //psge hit
            {
                check=1;
                c='H';
            }
        }
        if((pageFaults < pageframesNum) && (check == 0)){   //page fault&no swapping(just insert page)
            pageFaults++;   //count page fault
            frame[(pageFaults - 1) % pageframesNum] = buf[m];  //insert page
            c='F';
        }else if(check == 0) {  //fault&swapping
            pageFaults++;   //count page fault
            frame[(pageFaults - 1) % pageframesNum] = buf[m];   //page replacement
            c='F';
        }
        //print state
        printf("%2d\t",buf[m]);
        fprintf(fp, "%2d\t", buf[m]);
        for(int n = 0; n < pageframesNum; n++)
        {
            if(frame[n] != -1){
                printf(" %2d\t", frame[n]);
                fprintf(fp, "%2d\t", frame[n]);
            }else{
                printf(" - \t");
                fprintf(fp, " - \t");
            }
        }
        printf("%c\n",c);
        fprintf(fp, "%c\n",c);
    }
    printf("FIFO Total Page Faults : %d\n", pageFaults);
    fprintf(fp, "FIFO Total Page Faults : %d\n", pageFaults);
    fclose(fp);
    return;
}