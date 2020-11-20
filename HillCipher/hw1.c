#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*=============================================================================
|   Assignment:  HW 01 –Encrypting a plaintext file using the Hill cipher in the key file
|
|    Author:  ********
|    Language:  C
|
|   To Compile:  gcc -o hw1 hw1.c
|
|   To Execute:  ./hw1 hillcipherkey.txt plaintextfile.txt
|                     where the files in the command line are in the current directory.
|                    The key text contains a single digit on the first line defining the size of the key
|                    while the remaining linesdefine the key, for example:
|                     3
|                     1 2 3
|                     4 5 6
|                     7 8 9 
|The plain text file contains the plain text in mixed case with spaces & punctuation.
|
|   Class:  CIS3360-Security in Computing-Fall2020
|   Instructor:  McAlpin
|   Due Date:  per assignment
|
+=============================================================================*/

int** createKeyMatrix(FILE* keyFile, int* n);
char* createPlainText(FILE* textFile, int n);
char* encodeText(int** matrix, int n, char* plaintext);
void printMatrix(int** matrix, int n);

int main(int arc, char *argv[]) {
    FILE* cipherFile = fopen(argv[1], "r");
    FILE* textFile = fopen(argv[2], "r");

    int n;
    int** keyMatrix = createKeyMatrix(cipherFile, &n);
    printf("\n\nKey matrix:\n\n");
    printMatrix(keyMatrix, n);
    printf("\n\n");

    char* plainText =  createPlainText(textFile, n);
    printf("Plaintext:\n\n");
    int len = strlen(plainText);
    for(int i = 0; i < len; i++) {
        printf("%c", plainText[i]);
        if((i+1) % 80 == 0 && i != 0)
            printf("\n");
    }
    printf("\n\n");

    char* encodedText = encodeText(keyMatrix, n, plainText);
    printf("\nCiphertext:\n\n");
    len = strlen(encodedText);
    for(int i = 0; i < len; i++) {
        printf("%c", encodedText[i]);
        if((i+1) % 80 == 0 && i != 0)
            printf("\n");
    }
    printf("\n");

    return 0;
}

int** createKeyMatrix(FILE* keyFile, int* n) {
    fscanf(keyFile, "%d", n);
    //Allocate the space for the n x n matrix
    int** matrix = (int**) malloc(sizeof(int*) * *n);
    for(int i = 0; i < *n; i++) {
        matrix[i] = (int*) malloc(sizeof(int) * *n);
        for(int j = 0; j < *n; j++) {
            //assign all the positions of the matrix
            fscanf(keyFile, "%d", &matrix[i][j]);
        }
    }
    return matrix;
}

char* createPlainText(FILE* textFile, int n) {
    char* plainText = (char*) malloc(sizeof(char) * 10000);
    int charPos = 0;
    char c;
    while((c = fgetc(textFile)) != EOF) {
        //If the character is upper case
        if(c >= 65 && c <= 90) {
            //make it lower case
            c += 32;
        //If the character is not lower case
        } else if(c < 97 || c > 122) {
            //do nothing
            continue;
        }
        //add the character to the array
        plainText[charPos++] = c;
    }
    //If our string is not evenly divisible by n
    while((charPos) % n != 0) {
        //pad with x
        plainText[charPos++] = 'x';
    }
    //terminate the string
    plainText[charPos] = '\0';
    
    return plainText;
}

char* encodeText(int** matrix, int n, char* plaintext) {
    int len = strlen(plaintext);
    char* encodedText = (char*) malloc(len * sizeof(char) + 1);
    for(int i = 0; i < len; i += n) {
        for(int row = 0; row < n; row++) {
            int product = 0;
            for(int col = 0; col < n; col++) {
                product += (matrix[row][col] * (plaintext[i+col] - 97));
            }
            encodedText[i+row] = product % 26 + 97;
        }
    }
    return encodedText;
}

void printMatrix(int** matrix, int n) {
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++)
            printf("%d  ", matrix[i][j]);
        printf("\n");
    }
}

/*=============================================================================
|     I ************** affirm that this program is
| entirely my own work and that I have neither developed my code together with
| any another person, nor copied any code from any other person, nor permitted
| my code to be copied  or otherwise used by any other person, nor have I 
| copied, modified, or otherwise used programs created by others. I acknowledge
| that any violation of the above terms will be treated as academic dishonesty.
+=============================================================================*/