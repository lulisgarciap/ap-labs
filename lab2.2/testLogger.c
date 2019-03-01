#include <stdio.h>

int infof(const char *format, ...);
int warnf(const char *format, ...);
int errorf(const char *format, ...);
int panicf(const char *format, ...);
void textColor(int attr, int fg);
int main () {
    
    infof("%s\n", "Information...");
    warnf("%s\n", "type pointer assigned to a type int");
    errorf("%d error %s\n", 1, "Found");
    panicf("%s\n", "kernel crushed");
   
   return 0;
}

