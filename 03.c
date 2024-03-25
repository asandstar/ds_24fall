#include <stdio.h>
#include <ctype.h>
#include <string.h>

int calculate(char *s)
{
    int n = strlen(s);
    int stack[n];
    int top = 0;
    char preSign = '+';
    int num = 0;
    for (int i = 0; i < n; ++i)
    {
        if (isdigit(s[i]))
        {
            num = num * 10 + (int)(s[i] - '0');
            // 从字符串中解析出纯数字，从高位向低位处理
        }
        if (!isdigit(s[i]) && s[i] != ' ' || i == n - 1)
        {
            switch (preSign)
            {
            case '+':
                stack[top++] = num;
                break;
            case '-':
                stack[top++] = -num;
                break;
            case '*':
                stack[top - 1] *= num;
                break;
            case '/':
                stack[top - 1] /= num;
                break;
            }
            preSign = s[i];
            num = 0;
        }
    }
    int ret = 0;
    for (int i = 0; i < top; i++)
    {
        ret += stack[i];
    }
    return ret;
}

int main()
{
    char expression[100];
    scanf("%s", expression);
    int result = calculate(expression);
    printf("%d", result);
    return 0;
}
