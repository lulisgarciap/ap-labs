#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
int maskLine,space,flag_name_space;
FILE *pid_list;
FILE *cmd_ppid;
FILE *cmd_fd;
FILE *cmd_snapshot;
int no_proc;
int progress;
static void sigHandler(int sig);
void clear();
void printHeader();
void getPID();

void printData(int l, int ch,int spaces);
void getPID();

int main(){
  if (signal(SIGINT, sigHandler) == SIG_ERR) exit("signal");  
  getPID();
  clear();
  int loop;

  //for (loop = 0; ; loop++);
  return 0;
}


static void sigHandler(int sig){
 // from: https://stackoverflow.com/questions/1442116/how-to-get-the-date-and-time-values-in-a-c-program

 time_t t = time(NULL);
 struct tm tm = *localtime(&t);
 //progress=49;
 char output[100];
 char output2[100];
 
 sprintf(output,"scrot mytop-%d-%d_%d_%d_%d:%d:%d.png\n",  progress,tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
 cmd_snapshot = (FILE *)popen(output, "r");
 //printf(output);
 //printf(output);
}


void printData(int l, int ch, int spaces){
 int j;
 if((maskLine-l)==0){
  putc(ch,stdout);
  space--;
 }
 else{
   if(space>0){
    while(space>0){
     printf(" ");
     space--;
    }
   }
  if(l==3) printState(ch);
  else{
	  printf("| ");
	  putc(ch,stdout);
	  space=spaces;
	  space--;
	  maskLine=l;
  }
 }
}

void printState(int state){
 if(state=='R') printf( "| Running  ");
 if(state=='I') printf( "| Iddle    ");
 if(state=='S') printf( "| Sleeping ");
 if(state=='D') printf( "| Waiting  ");
 if(state=='Z') printf( "| Zombie   ");
 if(state=='T') printf( "| Stopped  ");
 if(state=='t') printf( "| Tracing  ");
 if(state=='W') printf( "| Paging   ");
 if(state=='X') printf( "| Dead     ");
 if(state=='x') printf( "| Dead     ");
 if(state=='K') printf( "| Wakekill ");
 if(state=='P') printf( "| Parked   ");
}

void clear() {
  printf("\e[1;1H\e[2J"); 
}

void printHeader(){
 printf("\n");     
printf(" ------------------------------------------------------------------------");
 printf("\n");//6, 39, 10, 6, 10, 10, 12
 printf("| PID  |                Name                    |  State  | PPID  | #Threads |  Memory  | Open Files |");
 printf("\n");
 printf(" ------------------------------------------------------------------------");
 printf("\n");
}



int isNum(char digit){
 if(digit>47 && digit<58){
  return 1;
 }else{
  return 0;
 }
}

void getPID(){
  printHeader();
  int c,ppid,p,at,i,x,pr;
  int lineno = 1;
  char pid[10];
  char line[200];
  char line2[200];
  for(i=0;i<10;i++) pid[i]='\0';
  for(i=0;i<200;i++) line[i]='\0';
  pid_list = (FILE *)popen("ls /proc", "r");

  while ( (c = fgetc(pid_list)) != EOF ){
    sleep(0.5);
    progress = (sizeof(pid_list)-(pr++))/sizeof(pid_list);
    if(isNum(c)){
     pid[at++]=c;
    } 
    if ( (c == '\n') && (pid[0]>0) ){
     sprintf(line, "cat /proc/%s/stat",pid);
     
     //printf("\n");

     cmd_ppid = (FILE *)popen(line, "r");

     if(cmd_ppid!=NULL){ //command error
      while ( ((x = fgetc(cmd_ppid)) != EOF) ) {
       if(x==40){
	flag_name_space=1;
        //x=0;
       }
       if(x==41){
	flag_name_space=0;
        //x=0;
       }
       if ((x == 32) && (flag_name_space==0) ) lineno++;
       if( (lineno==1) && isNum(x)) printData(lineno,x,5);//linea 1: pid
       else if((lineno==2) && (x!=40 && x!=41)) printData(lineno,x,39);//linea 2: name
       else if( (lineno==3) ) printData(lineno,x,6);//linea 3: state
       else if( (lineno==4) && (isNum(x)) ) printData(lineno,x,6);//linea 4: PPID
       else if( (lineno==20) && (isNum(x)) ) printData(lineno,x,10);//linea 20: Thread
       else if( (lineno==23) && (isNum(x)) ) printData(lineno,x,10);//linea 20:
      }
      at=0;
      lineno=1;
      maskLine=space=0;
      //for(i=0;i<10;i++) pid[i]='\0';
      for(i=0;i<200;i++) line[i]='\0';
      pclose(cmd_ppid);
     } 
     
   sprintf(line, "sudo ls -1 /proc/%s/fd | wc -l",pid); //to get open files
   cmd_fd = (FILE *)popen(line, "r");
   while ( ((x = fgetc(cmd_ppid)) != EOF) ) printData(5,x,12);
   pclose(cmd_fd);
   at=0;
   lineno=1;
   maskLine=space=0;
   for(i=0;i<10;i++) pid[i]='\0';
   for(i=0;i<200;i++) line[i]='\0';
   }
  }
  pclose(pid_list);
}




