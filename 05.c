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

FuncLog stack[MAX_LOGS];      // Stack to keep track of active function calls
int top = -1;                 // Top of the stack
FuncTime funcTimes[MAX_FUNC]; // Array to store function names and their total execution times
int funcCount = 0;            // Number of unique functions

void push(char *funcName, int startTime)
{
    top++;
    strncpy(stack[top].funcName, funcName, MAX_NAME_LEN);
    stack[top].startTime = startTime;
    stack[top].totalTime = 0;
}

int pop(char *funcName, int endTime, int *execTime)
{
    if (top == -1)
    {
        return -1; // Stack is empty
    }

    if (strcmp(stack[top].funcName, funcName) != 0)
    {
        return -1; // Function name mismatch
    }

    int startTime = stack[top].startTime;
    if (startTime > endTime)
    {
        return -1; // End time is before start time
    }

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
    scanf("%d\n", &n); // Read the number of log entries

    char line[100], funcName[MAX_NAME_LEN], op[6];
    int time, execTime;

    for (int i = 0; i < n; i++)
    {
        fgets(line, sizeof(line), stdin); // Read each log entry
        sscanf(line, "%s %s %d", funcName, op, &time);

        if (strcmp(op, "start") == 0)
        {
            push(funcName, time);
        }
        else if (strcmp(op, "end") == 0)
        {
            if (pop(funcName, time, &execTime) != 0)
            {
                printf("ERROR\n");
                return 1; // Early exit on error
            }
            else
            {
                updateFuncTime(funcName, execTime);
            }
        }
    }

    if (top != -1)
    {
        printf("ERROR\n"); // Unfinished functions indicate a log error
        return 1;
    }

    // Assuming the input is well-formed and there's no error
    // Proceed to find and print the function with the longest execution time
    if (funcCount == 0)
    {
        printf("No functions were executed.\n");
        return 0;
    }

    int maxIndex = 0;
    for (int i = 1; i < funcCount; i++)
    {
        if (funcTimes[i].totalTime > funcTimes[maxIndex].totalTime)
        {
            maxIndex = i;
        }
    }

    printf("Function %s has the longest execution time of %d\n", funcTimes[maxIndex].funcName, funcTimes[maxIndex].totalTime);

    return 0;
}
