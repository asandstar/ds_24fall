#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int cntString(char *s)
{
    int num = 0; // 初始化num为0
    char *start = s;

    while (*s != '\0')
    {
        num++;
        char *p = s + 1;
        char *q = s;
        while (q >= start && *p != '\0' && *q == *p)
        {
            q--;
            p++;
            num++;
        }
        p = s + 1;
        q = s - 1;

        while (q >= start && *p != '\0' && *q == *p)
        {
            num++;
            q--;
            p++;
        }

        s++;
    }

    return num;
}

int main()
{
    char s[101]; // 分配101个字符的数组给s（包括'\0'）
    int num;
    fgets(s, sizeof(s), stdin); // 使用fgets安全读取字符串
    s[strcspn(s, "\n")] = 0;    // 移除可能的换行符

    num = cntString(s);
    printf("%d\n", num);
}
