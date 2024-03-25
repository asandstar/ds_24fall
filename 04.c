#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
/*两个序列
一个入栈一个出栈
*/
bool valStkSeq(int *pushed, int pushedSize, int *poped, int poppedSize)
{
    int *stack = (int *)malloc(pushedSize * sizeof(int));
    int top = 0;
    for (int i = 0, j = 0; i < pushedSize; i++)
    {
        stack[top++] = pushed[i];
        while (top > 0 && stack[top - 1] == poped[j])
        {
            top--;
            j++;
        }
    }

    free(stack);
    return top == 0;
}

int main()
{
    int n;
    scanf("%d", &n);
    int push[n];
    int pop[n];
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &push[i]);
    }

    for (int j = 0; j < n; j++)
    {
        scanf("%d", &pop[j]);
    }

    bool isValid = valStkSeq(push, n, pop, n);
    if (isValid)
    {
        printf("true");
    }
    else
    {
        printf("false");
    }
    return 0;
}