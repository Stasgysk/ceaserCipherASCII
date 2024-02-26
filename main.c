#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    result = result % 94;
    return result;
}

char* getInputString(char inputFileName[]){
    FILE *fptr;

    fptr = fopen(inputFileName, "r");

    if(fptr == NULL) {
        return NULL;
    }

    fseek (fptr, 0, SEEK_END);
    long length = ftell (fptr);
    char* fileContent = malloc(length + 1);
    fseek (fptr, 0, SEEK_SET);

    char ch;
    int pos = 0;
    do {
        ch = fgetc(fptr);
        if(ch != EOF){
            fileContent[pos] = ch;
            pos++;
        }
    } while (ch != EOF);

    fgets(fileContent, length, fptr);

    fclose(fptr);
    
    return fileContent;
}

int writeToOutputFile(char* outputFileContent, char outputFileName[]){
    FILE *fptr;

    fptr = fopen(outputFileName, "w");
    if(fptr == NULL) {
        printError();
        return -1;
    }

    fprintf(fptr, "%s", outputFileContent);

    fclose(fptr);
    return 0;
}

void encrypt(char key[], char inputFileName[], char outputFileName[]) {
    int keyValue = getKeyIntValue(key);

    printf("%d\n", keyValue);

    char* inputFileContent = getInputString(inputFileName);
    if(inputFileContent == NULL) {
        printError();
        return;
    }

    char* outputFileContent = malloc(strlen(inputFileContent) + 1);

    for (int i = 0; i < strlen(inputFileContent); i++) {
        int integerValue = inputFileContent[i];
        if(integerValue == 10) {
            outputFileContent[i] = '\n';
            continue;
        }
        if(integerValue == 13) {
            outputFileContent[i] = 13;
            continue;
        }
        integerValue += keyValue;
        if(integerValue > 126) {
            integerValue = (integerValue - 126) + 32;
        }
        outputFileContent[i] = integerValue;
    }

    writeToOutputFile(outputFileContent, outputFileName);
}

void decrypt(char key[], char inputFileName[], char outputFileName[]) {
    int keyValue = getKeyIntValue(key);

    char* inputFileContent = getInputString(inputFileName);
    if(inputFileContent == NULL) {
        printError();
        return;
    }

    char* outputFileContent = malloc(strlen(inputFileContent) + 1);

    for (int i = 0; i < strlen(inputFileContent); i++) {
        int integerValue = inputFileContent[i];
        if(integerValue == 10) {
            outputFileContent[i] = '\n';
            continue;
        }
        if(integerValue == 13) {
            outputFileContent[i] = 13;
            continue;
        }
        integerValue -= keyValue;
        if(integerValue < 32) {
            integerValue += keyValue;
            integerValue = 126 - (keyValue - (integerValue - 32));
        }
        outputFileContent[i] = integerValue;
    }

    writeToOutputFile(outputFileContent, outputFileName);
}