#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 构建部分匹配表
void buildPartialMatchTable(char *pattern, int *table)
{
    int length = strlen(pattern);
    table[0] = 0;
    int j = 0;

    for (int i = 1; i < length; i++)
    {
        while (j > 0 && pattern[i] != pattern[j])
        {
            j = table[j - 1];
        }
        if (pattern[i] == pattern[j])
        {
            j++;
        }
        table[i] = j;
    }
}

// KMP搜索算法
int KMP(char *src, char *dst)
{
    int srcLen = strlen(src);
    int dstLen = strlen(dst);
    int *table = (int *)malloc(dstLen * sizeof(int));

    buildPartialMatchTable(dst, table);

    int j = 0;
    for (int i = 0; i < srcLen; i++)
    {
        while (j > 0 && src[i] != dst[j])
        {
            j = table[j - 1];
        }
        if (src[i] == dst[j])
        {
            j++;
        }
        if (j == dstLen)
        {
            free(table);
            return i - j + 1; // 返回匹配的起始位置
        }
    }

    free(table);
    return -1; // 未找到匹配
}

int main()
{
    char src[1001]; // 假设输入字符串长度不超过1000
    char dst[1001];

    // 读取输入字符串
    scanf("%1000s", src);
    scanf("%1000s", dst);

    int index = KMP(src, dst);
    printf("%d\n", index); // 输出匹配的起始下标或-1

    return 0;
}
