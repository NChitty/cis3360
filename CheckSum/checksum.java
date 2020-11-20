import java.io.File;
import java.io.FileNotFoundException;
import java.util.Arrays;
import java.util.Scanner;

/*=============================================================================
|   Assignment:  HW 02–Calculating the 8, 16, or 32 bit checksum for a 
|                given input file
|
|    Author:  Nicholas Chitty
|    Language:  Java
|
|   To Compile:  javac checksum.java checksum
|
|   To Execute:  java checksum textfile.txt checksum_size
|                where the files in the command line are in the current directory.
|                
|                The text file contains text ismixedcase with spaces,punctuation,
|                and is terminated by the hexadecimal ‘0A’, inclusive.
|                (The 0x’0A’ is included in the checksum calculation.)
|
|                The checksum_sizecontains digit(s) expressing the checksum size 
|                of either 8, 16, or 32 bits 
|
|
|   Class:  CIS3360-Security in Computing-Fall2020
|   Instructor:  McAlpin
|   Due Date:  per assignment
|
+=============================================================================*/

public class checksum {
    //using global variable to save on efficiency for only having one function to get
    //text file into words and to get the character count
    static char[][] words;

    public static void main(String[] args) {
        File textFile = new File(args[0]);
        int checkSumSize = Integer.parseInt(args[1]);

        if (checkSumSize == 8 || checkSumSize == 16 || checkSumSize == 32) {
            try {
                System.out.println();
                int characterCount = fileToWords(textFile, checkSumSize);
                long checksum = checkSum(checkSumSize);
                System.out.printf("%2d bit checksum is %8x for all %4d chars\n", checkSumSize, checksum, characterCount);
            } catch (FileNotFoundException e) {
                System.err.println("Could not find file.");
            }
        } else {
            System.err.println("Valid checksum sizes are 8, 16, or 32");
        }
    }

    private static long checkSum(int checkSumSize) {
        long checksum = 0x0;
        long mask = (checkSumSize == 8 ? 0xFF : (checkSumSize == 16 ? 0xFFFF : 0xFFFFFFFFL));
        for(char[] word : words) {
            long curr = 0x0;
            for(char c : word) {
                curr = (curr << 8);
                curr |= c;
            }
            checksum += curr;
        }
        return checksum & mask;
    }

    private static int fileToWords(File file, int checkSumSize) throws FileNotFoundException {
        words = new char[1][checkSumSize / 8];
        int wordCounter = 0, wordLengthCounter = 0, characterCount = 0, lineLength = 0;
        Scanner read = new Scanner(file);
        String line;
        
        while (read.hasNextLine()) {
            
            line = read.nextLine();
            
            for (char c : line.toCharArray()) {
                words[wordCounter][wordLengthCounter++] = c;
                
                if(lineLength % 80 == 0 && lineLength != 0) {
                    System.out.println();
                    lineLength = 0;
                }
                System.out.print(c);
                lineLength++;
                if (wordLengthCounter >= checkSumSize/8) {
                    char[][] newWords = new char[++wordCounter+1][checkSumSize/8];
                    int i = 0;
                    for(char[] word : words) {
                        newWords[i++] = Arrays.copyOf(word, checkSumSize/8);
                    }
                    words = Arrays.copyOf(newWords, wordCounter+1);
                    wordLengthCounter = 0;
                }
                characterCount++;
            }
        }
        
        //add new line
        words[wordCounter][wordLengthCounter++] = '\n';
        System.out.println();
        characterCount++;
        
        //pad
        while(characterCount % (checkSumSize/8) != 0) {
            words[wordCounter][wordLengthCounter++] = 'X';
            System.out.print('X');
            characterCount++;
        }
        
        System.out.println();
        read.close();
        return characterCount;
    }
}

/*
 * +=============================================================================
 * | I, Wm Nicholas Chitty (wi782067) affirm that this program is 
 * | entirely my own work and that I have neither developed my code together with 
 * | any another person, nor copied any code from any other person, nor permitted 
 * | my code to be copied or otherwise used by any other person, nor have I 
 * | copied, modified, or otherwise used programs created by others. I acknowledge
 * | that any violation of the above terms will be treated as academic dishonesty.
 * +=============================================================================
 */