/* strtok example */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD 500
#define MAX_LENGTH 150

int main()
{
    char input[MAX_LENGTH];
    char *word;
    char longest[MAX_LENGTH] = "";
    char shortest[MAX_LENGTH] = "";
    int longest_len = 0;
    int shortest_len = MAX_LENGTH;
    // read a line
    fgets(input, sizeof(input), stdin);
    // remove "\n"
    input[strcspn(input, "\n")] = 0;
    // address punctuation
    for (int i = 0; input[i]; i++)
    {
        if (ispunct(input[i]) && input[i] != '\'')
        {
            input[i] = ' ';
        }
    }
    // split the words with strtok and space
    // address word one by one if word!=NULL
    word = strtok(input, " ");
    while (word != NULL)
    {
        // remove apostrophe
        char clean_word[MAX_LENGTH];
        int idx = 0;
        for (int wi = 0; word[wi]; wi++)
        {
            if (word[wi] != '\'')
            {
                clean_word[idx++] = word[wi];
            }
        }
        clean_word[idx] = '\0';

        int len = strlen(clean_word);
        if (len > longest_len)
        {
            longest_len = len;
            strcpy(longest, clean_word);
        }
        if (len < shortest_len)
        {
            shortest_len = len;
            strcpy(shortest, clean_word);
        }
        word = strtok(NULL, " ");
    }

    printf("%s\n", longest);
    printf("%s\n", shortest);
}