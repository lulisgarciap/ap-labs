#include <stdio.h>
#include <stdarg.h>

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



int infof(const char *format, ...) {
    int result;
    va_list args;
    va_start(args, format);
    textColor(UNDERLINE, CYAN);
    result = vfprintf(stdout, format, args);
    textColor(RESET, WHITE);
    va_end(args);
    return result;
}

int warnf(const char *format, ...) {
    int result;
    va_list args;
    va_start(args, format);
    textColor(DIM, BLUE);
    fputs("Warning: ", stdout);
    textColor(RESET, WHITE);
    result = vfprintf(stdout, format, args);
    va_end(args);
    return result;
}

int errorf(const char *format, ...) {
    int result;
    va_list args;
    va_start(args, format);
    textColor(DIM, RED);  
    fputs("Error: ", stdout);
    textColor(RESET, WHITE);
    result = vfprintf(stdout, format, args);
    va_end(args);
    return result;
     
}

int panicf(const char *format, ...) {
    int result;
    va_list args;
    va_start(args, format);
    textColor(DIM, YELLOW);
    fputs("Panic: ", stdout);
    textColor(RESET, WHITE);
    result = vfprintf(stdout, format, args);
    va_end(args);
    return result;
}

void textColor(int attr, int fg){   
    char command[13];
    sprintf(command, "%c[%d;%dm", 0x1B, attr, fg + 30);
    printf("%s", command);
    
}
