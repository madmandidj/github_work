#include <stdio.h>
#include "Files.h"



int main()
{
    FILE* fp;
    /*
    Chars Frequency
    */
    printf("Test #1 repeated char counter: \n");
    fp = fopen("testfile", "w+");
    fprintf(fp,"Hello World ha ha!.");
    PrintCharFreq(fp);
    fclose(fp);
    printf("\n");
    printf("Test #2 repeated char counter: \n");
    fp = fopen("testfile", "w+");
    fprintf(fp,"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxzy\n \n  \n ");
    PrintCharFreq(fp);
    fclose(fp);
    printf("\n");

    /*
    Strings Frequency - 1st string is fine, 2nd and 3rd strings struggle, 3rd string some results produce garbage
    */
    printf("Test #1 repeated string counter: \n");
    fp = fopen("testfile", "w+");
    fprintf(fp,"Dani studies in Medical school and Yael studies in college");
    PrintStringFreq(fp);
    fclose(fp);
    printf("\n");
    printf("Test #2 repeated string counter: \n");
    fp = fopen("amchap2.txt", "r");
    PrintStringFreq(fp);
    fclose(fp);
    printf("\n");

    /*
    Print Last N Lines
    */
    printf("Testing print last N lines: \n");
    fp = fopen("testfile", "w+");
    fprintf(fp,"Test\n1\nTest2\nTest3\nTest4\n");
    PrintLastNLines(fp, 3);
    fclose(fp);
    return 0;
}
