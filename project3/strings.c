#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {

	FILE * infile;
	char * buffer = malloc(sizeof(char)*256);
	char * printBuffer = malloc(sizeof(char)*257);
	char current;
	int charCount = 0;

	// usage check
	if(argc<2) {
                printf("usage: %s  <filename>\n", argv[0]);
                return EXIT_FAILURE;
        }

	// file check
	infile = fopen(argv[1],"r");
	if(infile == NULL) {
		printf("Can't open %s.\n",argv[1]);
		return EXIT_FAILURE;
	}

	// perform strings operation
	while(fread(&current, 1, 1, infile) == 1) {

		if(current >= 32 && current <= 126) {
			//printf("current: %c\n",current);
			if(charCount < 256) {
				buffer[charCount] = current;
			}
			charCount++;
		}

		else {
			if(charCount >= 4) {
				strncpy(printBuffer,buffer,charCount);
				printBuffer[charCount] = '\0';
				printf("%s\n",printBuffer);
			}
			charCount = 0;
		}

	}

	// cleanup and exit
	free(buffer);
	free(printBuffer);
	return 0;
}
