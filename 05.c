#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 10

int main()
{
    int n = 0;
    scanf("%d", &n);
    char lines[n][MAX_LENGTH];
    int i = 0;

    // while (i <= n)
    // {
    //     scanf("%s", &lines[i]);
    //     i++;
    // }
    while (i < n)
    {
        if (fgets(lines[i], MAX_LENGTH, stdin))
        {
            if (lines[i][0] == '\n')
                break;
        }
        else
        {
            clearerr(stdin);
            break;
        }
        i++;
    }

    for (int j = 0; j < i; j++)
    {
        printf("%d : %s", j + 1, lines[j]);
    }

    return 0;
}