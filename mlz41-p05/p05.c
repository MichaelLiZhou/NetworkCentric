#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h> 
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>

char* fileValues;
int size;
FILE* checker;

// if successful in reading the file return 0
int readFile(char* fileName,char* argv[]){
	FILE* checker = NULL;
	checker = fopen(fileName,"r+");
	// first status message
	// return -1 means there was an error
	if (checker == NULL){
		perror(argv[0]);
		return -1;
	}
//	printf("succeded in opening file\n");
	
	struct stat st;
	stat(fileName,&st);
	size = st.st_size;

	// try to store the words into a dynamic array
	fileValues = NULL;
	fileValues = (char*) malloc(size);

	if(fileValues == NULL){
		free(fileValues);
		perror(argv[0]);
		return -1;
	}
	
	// stores values in charRead	
	size_t charRead = fread(fileValues,1,size-1,checker);
	return 0;
}

// Will check if file has certain string
//int checkInFile(const char* searchValue){
int checkInFile(char* searchValue){
//	printf("%s\n",fileValues);
	unsigned tokenCounter = 0;
	while((fileValues=strcasestr(fileValues,searchValue))!= NULL){
//		printf("%s\n",fileValues);
		//fileValues+=strlen(searchValue);
		fileValues++;
		tokenCounter++;
	}
	printf("%u\n",tokenCounter);
    return 0;

}

// read values bit by bit
int readFileBit(int argc, char* argv[]){
	
	char* fileName = argv[1];
	FILE* checker = NULL;
	checker = fopen(fileName,"r+");
	// first status message
	// return -1 means there was an error
	if (checker == NULL){
		perror(argv[0]);
		return -1;
	}
	//	printf("succeded in opening file\n");
	
	struct stat st;
	stat(fileName,&st);
	size = st.st_size;
	//	printf("file size %d\n",size);
	
	// try to store the words into a dynamic array
	fileValues = NULL;
	fileValues = (char*) malloc(size);
	if(fileValues == NULL){
		free(fileValues);
		perror(argv[0]);
		return -1;
	}
	
	
	int check = open(argv[1],O_RDONLY);
	char* buffer = (char*)malloc(1);
	int readReturn;
	int fileCounter = 0;
	// reading in from file: 1 byte at a time
	while ((readReturn=read(check,buffer,1))>0) {
		fileValues[fileCounter] = buffer[0];
		fileCounter++;
	}
	free(buffer);
    return 0;
}

int systemOne(int argc, char* argv[]){
	for (int i = 2 ; i < argc ; i++){
		// this will read the file argv[1], which is the name of the file	
		int valid = readFile(argv[1],argv);
		if ( valid !=0){
			perror(argv[0]);
			return 0;
		}else if (argc == 2){
			perror(argv[0]);
			return 0;
		}
		// Check if the value is in the file	
		checkInFile(argv[i]);
	}
    return 0;
}

int systemTwo(int argc, char* argv[]){
	for (int i = 2 ; i < argc-1 ; i++){
		// this will read the file argv[1], which is the name of the file	
		int valid = readFileBit(argc, argv);
		if ( valid !=0){
			perror(argv[0]);
			return 0;
		}else if (argc == 2){
			perror(argv[0]);
			return 0;
		}
		//Check if the value is in the file
		checkInFile(argv[i]);
	}
    return 0;
}

int main(int argc, char* argv[]){
	char systemCall[30];
	char argument[30];
	strcpy(systemCall,"--systemcalls");
	strcpy(argument,argv[argc-1]);
	int compareString=strcmp(systemCall,argument);
	//printf("%d\n",compareString);
	if(compareString != 0){
		systemOne(argc, argv);
	}else{
		//printf("%s\n","system2 in if statement");
		systemTwo(argc, argv);
	}
	free(fileValues);
	return 0;
}
