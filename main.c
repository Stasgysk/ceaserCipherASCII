#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

void printError();
int ifEmpty(char string[]);
void encrypt(char key[], char inputFileName[], char outputFileName[]);
void decrypt(char key[], char inputFileName[], char outputFileName[]);

int main(int argc, char *argv[]) {

    char type[20] = "";
    char key[1000] = "";
    char inputFile[255] = "";
    char outputFile[255] = "";

    for (int i = 0; i < argc; i++){
        if(strcmp(argv[i], "-s") == 0) {
            if(strcmp(type, "decrypt") == 0) {
                printError();
                return 0;
            }
            if(strlen(type) != 0) {
                printError();
                return 0;
            }
            strcpy(type, "encrypt");
        }
        if(strcmp(argv[i], "-d") == 0) {
            if(strcmp(type, "encrypt") == 0) {
                printError();
                return 0;
            }
            if(strlen(type) != 0) {
                printError();
                return 0;
            }
            strcpy(type, "decrypt");
        }
        if(strcmp(argv[i], "-p") == 0) {
            if(argv[i+1] == NULL) {
                printError();
                return 0;
            }
            if(strlen(key) != 0) {
                printError();
                return 0;
            }
            if(strcmp(argv[i+1], "-p") == 0 || strcmp(argv[i+1], "-i") == 0 || strcmp(argv[i+1], "-o") == 0) {
                printError();
                return 0;
            }
            strcpy(key, argv[i+1]);
            i++;
        }
        if(strcmp(argv[i], "-i") == 0) {
            if(argv[i+1] == NULL) {
                printError();
                return 0;
            }
            if(strlen(inputFile) != 0) {
                printError();
                return 0;
            }
            if(strcmp(argv[i+1], "-p") == 0 || strcmp(argv[i+1], "-i") == 0 || strcmp(argv[i+1], "-o") == 0) {
                printError();
                return 0;
            }
            strcpy(inputFile, argv[i+1]);
            i++;
        }
        if(strcmp(argv[i], "-o") == 0) {
            if(argv[i+1] == NULL) {
                printError();
                return 0;
            }
            if(strlen(outputFile) != 0) {
                printError();
                return 0;
            }
            if(strcmp(argv[i+1], "-p") == 0 || strcmp(argv[i+1], "-i") == 0 || strcmp(argv[i+1], "-o") == 0) {
                printError();
                return 0;
            }
            strcpy(outputFile, argv[i+1]);
            i++;
        }
    }

    if(ifEmpty(type) == -1) {
        return 0;
    }
    if(ifEmpty(key) == -1) {
        return 0;
    }
    if(ifEmpty(inputFile) == -1) {
        return 0;
    }
    if(ifEmpty(outputFile) == -1) {
        return 0;
    }

    if(strcmp(inputFile, outputFile) == 0) {
        printError();
        return 0;
    }

    if(strcmp(type, "encrypt") == 0) {
        encrypt(key, inputFile, outputFile);
    } else if(strcmp(type, "decrypt") == 0) {
        decrypt(key, inputFile, outputFile);
    } else {
        printError();
        return 0;
    }
    
    return 0;
}

void printError() {
    printf("chyba");
}

int ifEmpty(char string[]) {
    if(strlen(string) == 0) {
        printError();
        return -1;
    }
    return 0;
}

int getKeyIntValue(char key[]) {
    int result = 0;
    for (int i = 0; i < strlen(key); i++) {
        result += key[i];
    }
    result = result % 255;
    return result;
}

long getFileSize(char fileName[]) {
    int file = -1;
    file = open(fileName, O_RDONLY);
    struct stat file_status;
    if (stat(fileName, &file_status) < 0) {
        return -1;
    }
    close(file);
    return file_status.st_size;
}

char* getInputString(char inputFileName[]){
    int file = -1;

    long size = getFileSize(inputFileName);
    if(size < 0) {
        return NULL;
    }

    file = open(inputFileName, O_RDONLY);

    if(file == -1){
        return NULL;
    }
    
    char* fileContentToReturn = malloc(size + 1);
    read(file, fileContentToReturn, size);
    close(file);
    return fileContentToReturn;
}

int writeToOutputFile(char* outputFileContent, char outputFileName[], int length){
    FILE *fptr;

    fptr = fopen(outputFileName, "w");
    if(fptr == NULL) {
        printError();
        return -1;
    }

    for (int i = 0; i < length; i++){
        fprintf(fptr, "%c", outputFileContent[i]);
    }

    fclose(fptr);
    return 0;
}

void encrypt(char key[], char inputFileName[], char outputFileName[]) {
    int keyValue = getKeyIntValue(key);

    if(keyValue < 0){
        return;
    }

    char* inputFileContent = getInputString(inputFileName);
    if(inputFileContent == NULL) {
        printError();
        return;
    }
    
    long size = getFileSize(inputFileName);
    for (int i = 0; i < size; i++) {
        inputFileContent[i] += keyValue;
        if(inputFileContent[i] > 255) {
            inputFileContent[i] = (inputFileContent[i] - 256);
        }
    }

    writeToOutputFile(inputFileContent, outputFileName, size);
    free(inputFileContent);
}

void decrypt(char key[], char inputFileName[], char outputFileName[]) {
    int keyValue = getKeyIntValue(key);

    if(keyValue < 0){
        return;
    }

    char* inputFileContent = getInputString(inputFileName);
    if(inputFileContent == NULL) {
        printError();
        return;
    }
    
    long size = getFileSize(inputFileName);
    for (int i = 0; i < size; i++) {
        if(inputFileContent[i] - keyValue < 0) {
            inputFileContent[i] = 256 - (keyValue - inputFileContent[i]);
        } else {
            inputFileContent[i] -= keyValue;
        }
    }

    writeToOutputFile(inputFileContent, outputFileName, size);
    free(inputFileContent);
}