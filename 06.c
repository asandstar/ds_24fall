#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int id;        // 进程ID
    int data;      // 剩余数据量
    int lastStart; // 最后一次开始处理的时间，用于排序输出
} Process;

void initProcesses(Process **processes, int n)
{
    *processes = (Process *)malloc(n * sizeof(Process));
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &((*processes)[i].data));
        (*processes)[i].id = i + 1;
        (*processes)[i].lastStart = -1; // 初始化时未处理
    }
}

int compare(const void *a, const void *b)
{
    Process *processA = (Process *)a;
    Process *processB = (Process *)b;
    return processA->lastStart - processB->lastStart; // 按最后处理时间排序
}

void processTransfers(Process *processes, int n, int s, float T)
{
    float currentTime = 0.0;
    int processOrder = 0; // 用于记录处理顺序
    while (currentTime < T)
    {
        for (int i = 0; i < n && currentTime < T; i++)
        {
            if (processes[i].data > 0)
            {
                float transferTime = processes[i].data < s ? (float)processes[i].data / s : 1.0;
                if (currentTime + transferTime > T)
                {
                    transferTime = T - currentTime;
                }
                processes[i].data -= s * transferTime;
                processes[i].lastStart = processOrder++; // 更新处理顺序
                currentTime += transferTime;
            }
        }
    }
    // 根据最后一次处理时间对进程进行排序
    qsort(processes, n, sizeof(Process), compare);
}

void printProcesses(Process *processes, int n)
{
    int printed = 0;
    for (int i = 0; i < n; i++)
    {
        if (processes[i].data > 0)
        {
            printf("%d %d\n", processes[i].id, processes[i].data);
            printed = 1;
        }
    }
    if (!printed)
    {
        printf("DONE\n");
    }
}

int main()
{
    int n, s; // 进程数和网卡速率
    scanf("%d %d", &n, &s);

    Process *processes;
    initProcesses(&processes, n);

    float T; // 总时间
    scanf("%f", &T);

    processTransfers(processes, n, s, T);
    printProcesses(processes, n);

    free(processes);
    return 0;
}