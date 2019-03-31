//http://fm4dd.com/programming/base64/base64_stringencode_c.htm

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include "logger.h"
/* ---- Base64 Encoding/Decoding Table --- */
float progress=1.0;
char b64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static void
sigHandler(int sig)
{
    printf("Progress: %.0f%c\n", progress, 37);
}

/* decodeblock - decode 4 '6-bit' characters into 3 8-bit binary bytes */
void decodeblock(unsigned char in[], char *clrstr) {
  unsigned char out[4];
  out[0] = in[0] << 2 | in[1] >> 4;
  out[1] = in[1] << 4 | in[2] >> 2;
  out[2] = in[2] << 6 | in[3] >> 0;
  out[3] = '\0';

  strncat(clrstr, out, sizeof(out));
}

void b64_decode(char *b64src, char *clrdst) {
  int c, phase, i;
  unsigned char in[4];
  char *p;

  clrdst[0] = '\0';
  phase = 0; i=0;
  while(b64src[i]) {
   progress = ((float)i/(float)strlen(b64src))*100;
   if (signal(SIGINT, sigHandler) == SIG_ERR) errorf("signal");
   if (signal(SIGPWR, sigHandler) == SIG_ERR) errorf("signal");
    c = (int) b64src[i];
    if(c == '=') {
      decodeblock(in, clrdst); 
      break;
    }
    p = strchr(b64, c);
    if(p) {
      in[phase] = p - b64;
      phase = (phase + 1) % 4;
      if(phase == 0) {
        decodeblock(in, clrdst);
        in[0]=in[1]=in[2]=in[3]=0;
      }
    }
    i++;
  }
}

/* encodeblock - encode 3 8-bit binary bytes as 4 '6-bit' characters */
void encodeblock( unsigned char in[], char b64str[], int len ) {
    unsigned char out[5];
    out[0] = b64[ in[0] >> 2 ];
    out[1] = b64[ ((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4) ];
    out[2] = (unsigned char) (len > 1 ? b64[ ((in[1] & 0x0f) << 2) |
             ((in[2] & 0xc0) >> 6) ] : '=');
    out[3] = (unsigned char) (len > 2 ? b64[ in[2] & 0x3f ] : '=');
    out[4] = '\0';
    strncat(b64str, out, sizeof(out));
}

/* encode - base64 encode a stream, adding padding if needed */
void b64_encode(char *clrstr, char *b64dst) {
  unsigned char in[3];
  int i, len = 0;
  int j = 0;

  b64dst[0] = '\0';
  while(clrstr[j]) {
   progress = ((float)j/(float)strlen(clrstr))*100;
   if (signal(SIGINT, sigHandler) == SIG_ERR) errorf("signal");
   if (signal(SIGPWR, sigHandler) == SIG_ERR) errorf("signal");
    len = 0;
    for(i=0; i<3; i++) {
     in[i] = (unsigned char) clrstr[j];
     if(clrstr[j]) {
        len++; j++;
      }
      else in[i] = 0;
    }
    if( len ) {
      encodeblock( in, b64dst, len );
    }
  }
}

int main(int argc, char **argv) {

    if(argc < 3){
        printf("Error, not enough arguments. Usage: ./base64 --encode|--decode <input-file>\n");
        return -1;
    }
	
    char *buffer = NULL;
    size_t size = 0;

    FILE *fp = fopen(argv[2], "r");

    fseek(fp, 0, SEEK_END);
    size = ftell(fp); 

    rewind(fp);

    buffer = malloc((size + 1) * sizeof(*buffer));

    fread(buffer, size, 1, fp);

    buffer[size] = '\0';
    if (signal(SIGINT, sigHandler) == SIG_ERR) errorf("signal");
    if (signal(SIGPWR, sigHandler) == SIG_ERR) errorf("signal");
    char *result = (char *)malloc(size*10);
    sleep(5);
    if(strcmp(argv[1], "--encode") == 0){
        b64_encode(buffer, result);
        FILE *f = fopen("encoded.txt", "wb");
        fprintf(f, "%s\n", result);
        fclose(f);
    } else if (strcmp(argv[1], "--decode") == 0) {
        b64_decode(buffer, result);
        FILE *f = fopen("decoded.txt", "wb");
        fprintf(f, "%s\n", result);
        fclose(f);
    } else {
        printf("Invalid flag, try --encode or --decode.\n");
        return -1;
    }
	int cou;
    
    for (cou = 0; ; cou++) {
		printf("%d\n", cou);
		sleep(3);                       /* Loop slowly... */
	    }
    return 0;
}
