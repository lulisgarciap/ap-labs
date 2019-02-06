
#include <stdio.h>
#include <stdlib.h>
//lib.c

int strlen(char *str){
   int i=0;
   for(i=0;*(str+i)!=NULL;i++);
   return i;
}


char* stradd(char *origin, char *addition)
{
    int size_or = strlen(origin);
    int size_add = strlen(addition);
    char *result= (char *)malloc(size_or+size_add+1);
    for(int i=0;i<=size_or;i++){
        *(result+i) = *(origin+i);
    }
    for(int i=0;i<=size_add;i++){
        *(result+size_add+i) = *(addition+i);
    }
    return (result);
}

int strfind(char *origin, char *substr){
    int no_s=0;
    int s =0;
    int size_or= strlen(origin);
    int size_sub= strlen(substr);
    printf("%d",size_or);
    for(int i=0; i< size_or; i++){
            if( *(substr+s) ==*(origin+i)){
                printf("%c", *(substr+s));
                printf("%c", *(origin+i));
                no_s++;
                s++;
                printf("%d", no_s);
                printf("%d", s);
                if(no_s==size_sub){
                    printf("string found1");
                    return no_s;
                }
            }else{
                no_s=0;
                s=0;
            }
    }
    return no_s;
}
/*
int main()
{
    char argv[]= ["pr","hi","by","h"];
    char str1[] = argv[1];
    char str2[] = argv[2];
    char substring[]= argv[3];

    printf("initial string: %d\n", strlen(*str));

    ////////////////////////////////
    char *add_res= (char *)malloc(strlen(&str1)+strlen(&str2));
    add_res= stradd(&str1,&str2);
    int size_add = strlen(add_res);
    ////////////////////////////////
    int i=0;
    printf("new string: ");
    while(i<size_add){
        printf("%c", *(add_res+(i))); //Aqui no imprime los ultimos 2
        i++;
    }
    printf("\n");
    ////////////////////////////////
    printf(" string found: %d", strfind(add_res, &substring));
    return 0;
}

*/

