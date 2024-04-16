#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// int main()
// {
//     char *inputString;
//     char **words;
//     int bufferSize = 256; // 可以根据需要调整这个大小
//     int maxWords = 50;    // 假设句子中最多有50个单词
//     int wordCount = 0;

//     // 为整个字符串分配内存
//     inputString = (char *)malloc(bufferSize * sizeof(char));
//     // Error allocating memory
//     if (inputString == NULL)
//     {
//         return 1;
//     }

//     // 为指针数组分配内存
//     words = (char **)malloc(maxWords * sizeof(char *));
//     // Error allocating memory for words
//     if (words == NULL)
//     {
//         free(inputString);
//         return 1;
//     }

//     // 输入字符串
//     fgets(inputString, bufferSize, stdin);
//     // 去除可能的换行符
//     inputString[strcspn(inputString, "\n")] = 0;

//     // 使用 strtok 分割字符串
//     char *token = strtok(inputString, " ");
//     while (token != NULL)
//     {
//         words[wordCount++] = token;
//         if (wordCount >= maxWords)
//             break;
//         token = strtok(NULL, " ");
//     }

//     // 逆序输出单词
//     for (int i = wordCount - 1; i >= 0; i--)
//     {
//         printf("%s ", words[i]);
//     }
//     printf("\n");

//     // 释放分配的内存
//     free(words);
//     free(inputString);

//     return 0;
// }

void reverseString(char *str, int start, int end)
{
    char temp;
    while (start < end)
    {
        temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

int main()
{
    char *inputString;
    int bufferSize = 256;
    int i, start;

    inputString = (char *)malloc(bufferSize * sizeof(char));
    if (inputString == NULL)
    {
        return 1;
    }

    fgets(inputString, bufferSize, stdin);
    inputString[strcspn(inputString, "\n")] = 0;

    reverseString(inputString, 0, strlen(inputString) - 1);

    start = 0;
    for (i = 0; inputString[i]; i++)
    {
        if (inputString[i] == ' ')
        {
            reverseString(inputString, start, i - 1);
            start = i + 1;
        }
    }
    reverseString(inputString, start, i - 1);

    printf("%s\n", inputString);
    free(inputString);

    return 0;
}