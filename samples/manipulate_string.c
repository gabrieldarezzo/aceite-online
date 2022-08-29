
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// CONTANTS
#define MAX_LENGTH_STRING 12

const char* cutString(char *stringToCut, int lengthToCut) {
    static char subtext[13];
    memcpy(subtext, &stringToCut[0], lengthToCut);
    subtext[lengthToCut] = '\0';
    return subtext;
}

int main(void)
{
    char *text = "KLAUDINEI DOS SANTOS";
    char subtext[13];
    memcpy(subtext,&text[0],12);
    subtext[12] = '\0';
    printf("The original string is: \n%s\n\n",text);
    printf("Substring is: \n%s\n\n", subtext);
    printf("Substring is: \n%s\n\n", cutString(subtext, MAX_LENGTH_STRING));
    return 0;
}

