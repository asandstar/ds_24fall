#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LOGS 100
#define MAX_FUNC 50
#define MAX_NAME_LEN 51

typedef struct
{
    char funcName[MAX_NAME_LEN];
    int startTime;
    int totalTime;
} FuncLog;

typedef struct
{
    char funcName[MAX_NAME_LEN];
    int totalTime;
} FuncTime;

FuncLog stack[MAX_LOGS];      // 用于跟踪活动函数调用的栈
int top = -1;                 // 栈顶指针
FuncTime funcTimes[MAX_FUNC]; // 存储函数名称及其总执行时间的数组
int funcCount = 0;            // 函数的数量
int errorFlag = 0;            // 错误标志

void push(char *funcName, int startTime)
{
    top++;
    strncpy(stack[top].funcName, funcName, MAX_NAME_LEN);
    stack[top].startTime = startTime;
    stack[top].totalTime = 0;
}

int pop(char *funcName, int endTime, int *execTime)
{
    // 栈为空or函数名不匹配
    if (top == -1 || strcmp(stack[top].funcName, funcName) != 0)
    {
        errorFlag = 1; // 设置错误标志
        return -1;
    }
    // 开始时间 > 结束时间，错误
    int startTime = stack[top].startTime;
    if (startTime > endTime)
    {
        errorFlag = 1; // 设置错误标志
        return -1;
    }
    // 函数独占时间 = 结束时间 - 开始时间 - 栈顶总计时间
    *execTime = endTime - startTime - stack[top].totalTime;
    top--;

    if (top != -1)
    {
        stack[top].totalTime += endTime - startTime;
    }

    return 0;
}

void updateFuncTime(char *funcName, int execTime)
{
    for (int i = 0; i < funcCount; i++)
    {
        if (strcmp(funcTimes[i].funcName, funcName) == 0)
        {
            funcTimes[i].totalTime += execTime;
            return;
        }
    }
    strncpy(funcTimes[funcCount].funcName, funcName, MAX_NAME_LEN);
    funcTimes[funcCount].totalTime = execTime;
    funcCount++;
}

int main()
{
    int n;
    scanf("%d\n", &n); // 读取日志条目数

    char line[100], funcName[MAX_NAME_LEN], op[6];
    int time, execTime;

    for (int i = 0; i < n; i++)
    {
        fgets(line, sizeof(line), stdin); // 读取每个日志条目
        sscanf(line, "%s %s %d", funcName, op, &time);

        if (strcmp(op, "start") == 0)
        {
            push(funcName, time);
        }
        else if (strcmp(op, "end") == 0)
        {
            if (pop(funcName, time, &execTime) == 0)
            {
                updateFuncTime(funcName, execTime);
            }
        }
    }

    // 处理完所有输入后，根据是否有错误来决定输出
    if (errorFlag || top != -1)
    {
        printf("ERROR\n"); // 如果有未结束的函数或者设置了错误标志，则输出错误
    }
    else
    {
        // 如果输入合法，找出执行时间最长的函数并输出
        int maxIndex = 0;
        for (int i = 1; i < funcCount; i++)
        {
            if (funcTimes[i].totalTime > funcTimes[maxIndex].totalTime)
            {
                maxIndex = i;
            }
        }

        printf("%s %d\n", funcTimes[maxIndex].funcName, funcTimes[maxIndex].totalTime);
    }

    return 0;
}
