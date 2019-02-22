
#include <stdio.h>
#include <stdlib.h>
//strlib.c

int strlen(char *str){
   int i=0;
   for(i=0;*(str+i)!='\0';i++);
   return i;
}


char* stradd(char *origin, char *addition)
{
    int size_or = strlen(origin);
    int size_add = strlen(addition);
    char *result= (char *)malloc(size_or+size_add+1);
    for(int i=0;i<size_or;i++){
        result[i] = origin[i];
    }
    for(int i=0;i<size_add;i++){
        result[size_or+i]= addition[i];
    }
    return result;
}

int strfind(char *origin, char *substr){
    int no_s=0;
    int i;
    int size_or= strlen(origin);
    int size_sub= strlen(substr);
    for(i=0; i< size_or; i++){
            if( (substr[no_s]-origin[i])==0){
                no_s++;
		//printf("%d\n",no_s);
                if(no_s == size_sub){
                    //printf("string found1");
                    return 1;
                }

            }else{
                no_s=0;
            }
    }
	//printf("%d ",size_or);
	//printf("%d ",i);
	//printf("%d",size_sub);
    return no_s;
}



