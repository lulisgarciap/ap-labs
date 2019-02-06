#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{

    char *str1 = (char *)malloc(strlen(argv[1]));
    str1=argv[1];

    char *str2 = (char *)malloc(strlen(argv[2]));
    str2=argv[2];

    char *subs = (char *)malloc(strlen(argv[3])+1);
    subs=argv[3];

    printf("initial string: %d\n", strlen(str1)+1);

    ////////////////////////////////
    char *add_res= (char *)malloc(strlen(str1)+strlen(str2)+1);
    add_res= stradd(str1,str2);
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
    printf(" string found: %d", strfind(add_res, subs));
    return 0;
}
