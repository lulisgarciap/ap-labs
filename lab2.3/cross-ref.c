#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define char_size 1
#define size_buf 32250
#define size_noisyW 12
#define num_words 4000
#define word_size 30
#define lines_of_word 70

char *noisyWords[] = {
	    "and",  
	    "because",
	    "but",
	    "for",
	    "has",
	    "into",
	    "the",
	    "too",
	    "with",
	    "are",
	    "that",
	    "what"
 };

void appendString(char * word,int index_words);
void appendLine(int index_words, int line);
char * readFile(char *nameFile);
int isNoisyWord(char * word);
void evaluateString(char * buffer);
int append(char * word, int line);
void printWords(void);
int index_reference=0;

struct Word{
	char str[word_size];
	int line[lines_of_word];
};

struct Word words[num_words];


int main(int argc, char *argv[]){
	char *nameFile= argv[1];
	char *buffer= (char *)malloc(size_buf);
	buffer= readFile(nameFile);
	evaluateString(buffer);

	printWords();
	return 0;
}

void printWords(void){
	int c,d;
	for(d=0;d<num_words;d++){
		if(words[d].line[0]!= 0){
			printf("\n\n  %s : ", words[d].str);
			for(c=0;c<lines_of_word;c++){		
				if(words[d].line[c] !=0){
					printf("%d , ", words[d].line[c]);
				}
			}		
		}
	}
}

char * readFile(char *nameFile){
	FILE *fp = fopen(nameFile, "r");	
	if (fp == NULL){
      		perror("Error while opening the file.\n");
      		exit(EXIT_FAILURE);
   	}
	int c;
	char *buffer= (char *)malloc(size_buf);
	fread(buffer, char_size, size_buf, fp);
	//printf("%s : ",buffer);
	fclose(fp);
	return buffer;
}

int append(char *word, int line){
	
	//printf(",w  %s\n", word);
	int i;
	for(i=0; i<num_words; i++){
		if(strcmp(word,words[i].str)==0){ // busca si ya existe
			appendLine(i,line); //si existe solo agrega el num linea
			//printf("i was append %d \n", line);
			return 0;
			break;
		}
	}
	//printf("i was append %s \n", word);
	appendString(word,index_reference);
	appendLine(index_reference,line);
	index_reference++;
	return 0;
}


void appendString(char * word,int index_words){
	//printf("append new string: %s\n",word);
	//printf("in line %d\n",index_words);
	strcpy(words[index_words].str,word);
}

void appendLine(int index_words, int line){
	//line++;
	//printf("this word already exists: %s\n",words[index_words].str);
	printf(".");
	int i;
	for(i = 0; i < lines_of_word; i++){
		if (words[index_words].line[i] == 0){
			words[index_words].line[i] = line;
			break;
		} else if (words[index_words].line[i] == line){
			break;
		}
	}
}

void evaluateString(char * buffer){
	int i;
	int line;
	line=1;
	int tempCount = 0;
	char *temp= (char *)malloc(30);
	//char *temp;
	for(i=0; i < size_buf; i++){
		if( ( ((buffer[i] >=32) && (buffer[i]<=47)) || ((buffer[i] >=48) && (buffer[i]<=64)))|| ((buffer[i] >=91) && (buffer[i]<=96))  ){ //signs
			if((isNoisyWord(temp) ==0)&&(strlen(temp)>2)){	
				append(temp,line);
			}
			memset(temp, 0, 30);
			tempCount = 0;
		}
		else if(buffer[i] == '\n'){	
			//printf("salto de linea %d\n",line);	
			if((isNoisyWord(temp) ==0)&&(strlen(temp)>2)){
		  		append(temp,line);
			}
			memset(temp, 0, 30);
			tempCount = 0;			
			line++;		
			
		}else{
			temp[tempCount] = tolower(buffer[i]);
			tempCount++;
		}
	}
}

int isNoisyWord(char * word){
	int i;
	
	for(i=0; i < size_noisyW; i++){
		if(strcmp(word, noisyWords[i])==0){
			//printf("NW: %s\n",word);	
			return 1;
		}
	}
	return 0;
}
	
