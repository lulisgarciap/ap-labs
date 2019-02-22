#include <stdio.h>
//strmain

int strlen(char *str);
char* stradd(char *origin, char *addition);
int strfind(char *origin, char *substr);

int main(int argc, char **argv)
{
    printf("initial string: %d\n", strlen(argv[1]));
////////////////////////////////
   

	char *add_res= stradd(argv[1],argv[2]);
    printf("%s\n",add_res);
    int size_add = strlen(add_res);

    printf(" string found: %d", strfind(add_res, argv[3]));
    return 0;
}
