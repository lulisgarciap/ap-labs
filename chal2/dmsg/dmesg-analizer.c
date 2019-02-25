#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#define REPORT_FILE "report.txt"
typedef struct{
	int m[500][500];
}matrix;
int fo;
char arr[500][65000];
char general[5][65000];
//printf("hola");
//char *arr;
//arr = (char *) malloc(500);
//arr[0]= '\0';
void analizeLog(char *logFile, char *report);

int main(int argc, char **argv) {
    
    if (argc < 2) {
	printf("Usage:./dmesg-analizer.o others/dmesg.txt\n");
	return 1;
    }
    
	printf("0");
	analizeLog(argv[1], REPORT_FILE);

    return 0;
}

void openFile(char *output){
	
	fo = open(output,O_WRONLY| O_CREAT | O_APPEND, S_IRUSR | S_IWUSR) ;
	if(fo==-1){
		printf("open write error");	
	}	
}

void iterateTagsDict(int index, matrix tag_dictionary){
	void *buf;	
	buf = (char*) malloc(200);
	//printf("size> %ld \n",sizeof(tag_dictionary.m)); 
	int j;
	printf("\n");
	buf=tag_dictionary.m[index];
	//printf("buf: %c \n",buf); 
	for(j = 0; j <sizeof(tag_dictionary.m[index]); j++){
		if(tag_dictionary.m[index][j]==NULL) break;
		printf("%c",tag_dictionary.m[index][j]);
		write(fo, buf,200);
	} 
}

int getIndex(char str1[500], matrix dict, int len_str){
	int it_str,line, end;
	for(line = 0; line <500; line++){		
		it_str=0;
		while(it_str<len_str){
			if(!((str1[it_str])- (dict.m[line][it_str]))){
				it_str++;
			}else{
				it_str=len_str;
				break;		
			}
			if(it_str>=len_str){ //tag was found
					//printf("\ntag found line %d \n",line);
					return line;				
			}
		}
	}
	return -1;
}

matrix getTags(char bl[500][500], int nLines){
	int j, i,ii, index_tag, index_dict =0;
	matrix tags_dict;
	char tags[500];
	for(i = 0; i < nLines; i++){ 
		index_tag=0;
		for(j = 15; j <sizeof(bl[i]); j++){ 
			tags[index_tag++]= bl[i][j];
			//printf("%c",tags[index_tag]);
			if ((bl[i][j]==':') && (bl[i][j+1]==' ')){
				//printf("\n");
				for(ii = 0; ii <index_tag; ii++){
					//printf("%c",tags[ii]);
					tags_dict.m[index_dict][ii]= tags[ii];}
				index_dict++;
				break;
			}
		}
	}
	return tags_dict;
} //en esta funcion se pasa el buf lines y el dict, comparara strings a partir del caracter 15 para asi dividirlos en categorias

void insert(int index, char str[500],int len_str, char array[500][65000]){
	int i,c, j=0;	
	
	while(array[index][c]!=NULL)c++;
	
	if(len_str==0){
		for(j=0; str[j]!=NULL; j++){
			array[index][c+j]=str[j];
		}	
	}else{
	
		for(j=0; j<15; j++){
			array[index][c+j]=str[j];
		}	
		for(i=16+len_str;str[i]!=NULL;i++){	
				array[index][c+(j++)]=str[i];
		}
	}
	//printf("\n j es %d",j);
	//printf("\n c es %d",c);
	array[index][c+(j++)]='\n';
	
	
}

void analizeLog(char *logFile, char *report) {
    	printf("Generating Report from: [] log file");
    	int input_file;
	//*****************TEXT PROCESSED***********************//
	char buffer[65535]; //the file is stored here
	char buff_lines[500][500];  //here is processed by lines	
	input_file = open(logFile, O_RDONLY);
	read(input_file, buffer, 65535);

	int i,j = 0;
	int no_line= 0;
	int character=0;
	while(buffer[i]!=EOF){
		if(buffer[i]!='\n'){
			buff_lines[no_line][character++]= buffer[i];
			//printf("%c", buff_lines[no_line][character-1]);
        	 }else{
			//printf("****\n");
        	 	no_line++;
			character=0;
            	}
		i++;
	}
	//********************************************************//
	
	//*************GUARDAR TAGS DE LOG_FILES*****************//
	int end=0;
	matrix tag_dictionary;
	tag_dictionary= getTags(buff_lines,no_line);

	//RECORRE TAGSDICT: (uncomment)
	/*printf("size> %ld \n",sizeof(tag_dictionary.m)); 
	for(i = 0; i <500; i++){
		for(j = 0; j <sizeof(tag_dictionary.m[i]); j++){
			if(tag_dictionary.m[i][j]==NULL) break;
			printf("%c",tag_dictionary.m[i][j]);
		}
	}*/
	
	//******************************************************//

	//*****************ORGANIZAR POR TAGS*******************//
	int t, index, index_tag, aux, c;
	char tag_in_line[500];
	for(i = 0; i <no_line; i++){ //<no_line
		index_tag=0;
		for(j = 15; buff_lines[i][j]!=NULL; j++){
			tag_in_line[index_tag++]= buff_lines[i][j];
			if((buff_lines[i][j]==':') && (buff_lines[i][j+1]==' ')){
				index = getIndex(tag_in_line, tag_dictionary, index_tag);	
				insert(index, buff_lines[i], j-15, arr); 
				break;
			}		
		}
		//insert(0, buff_lines[i],0,general);		
	}
	
	//******************************************************//

  	printf("Report is generated at: [%s]\n", REPORT_FILE);
	openFile(REPORT_FILE);
	for(j = 0; j <500; j++){
		if(arr[j][i=0]!=NULL){
			printf("\n");
			iterateTagsDict(j, tag_dictionary);
			printf("\n");
		}
		for(i = 0; i <500; i++){
			if(arr[j][i]!=NULL){
				printf("%c",arr[j][i]);
			}
			else break;
		}
	//printf("\n");
	}
	close(fo);
	//writeTags(x, REPORT_FILE);
}	
