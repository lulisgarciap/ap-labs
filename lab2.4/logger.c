#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <syslog.h>

#define RESET       0
#define BRIGHT      1
#define DIM         2
#define UNDERLINE   3
#define BLINK       4
#define REVERSE     7
#define HIDDEN      8

#define BLACK       0
#define RED         1
#define GREEN       2
#define YELLOW      3
#define BLUE        4
#define MAGENTA     5
#define CYAN        6
#define WHITE       7

int so, sl;

int initLogger(char *logType){
    if(strcmp(logType, "stdout") == 0){
        so = 1;
	sl=0;
    } else if (strcmp(logType, "syslog") == 0){
	so=0;
        sl = 1;
    }else{
	so=1; // I set it as default
	sl=0;
	printf("error\n");
	return 0;
    }
    printf("Initializing logger on %s\n", logType);
    return 0;
}

int infof(const char *format, ...) {
    if (so){
        int result;
	va_list args;
	va_start(args, format);
	textColor(UNDERLINE, CYAN);
	result = vfprintf(stdout, format, args);
	textColor(RESET, WHITE);
	va_end(args);
    return result;
    } else if (sl){
        openlog("logger", LOG_PID|LOG_CONS, LOG_USER);
        syslog(LOG_INFO, format);
        closelog();
    }
    
}

int warnf(const char *format, ...) {
	if(so){	    
	    int result;
	    va_list args;
	    va_start(args, format);
	    textColor(DIM, BLUE);
	    fputs("Warning: ", stdout);
	    textColor(RESET, WHITE);
	    result = vfprintf(stdout, format, args);
	    va_end(args);
	    return result;
	}else if (sl){
		openlog("logger", LOG_PID|LOG_CONS, LOG_USER);
        	syslog(LOG_WARNING, format);
	        closelog();
    	}
}

int errorf(const char *format, ...) {
	if(so){    
		int result;
		va_list args;
		va_start(args, format);
		textColor(DIM, RED);  
		fputs("Error: ", stdout);
		textColor(RESET, WHITE);
		result = vfprintf(stdout, format, args);
		va_end(args);
		return result;
	}else if (sl){
		openlog("logger", LOG_PID|LOG_CONS, LOG_USER);
		syslog(LOG_ERR, format);
		closelog();
	}
}

int panicf(const char *format, ...) {
	if(so){   
		int result;
		va_list args;
		va_start(args, format);
		textColor(DIM, YELLOW);
		fputs("Panic: ", stdout);
		textColor(RESET, WHITE);
		result = vfprintf(stdout, format, args);
		va_end(args);
		return result;
	}else if (sl){
		openlog("logger", LOG_PID|LOG_CONS, LOG_USER);
		syslog(LOG_INFO, format);
		closelog();
	}
}

void textColor(int attr, int fg){   
    char command[13];
    sprintf(command, "%c[%d;%dm", 0x1B, attr, fg + 30);
    printf("%s", command);
    
}
