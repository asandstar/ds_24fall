#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int id;   // 进程ID
    int data; // 剩余数据量
} Process;

// 初始化进程队列
void initProcesses(Process **processes, int n)
{
    *processes = (Process *)malloc(n * sizeof(Process));
    for (int i = 0; i < n; i++)
    {
        (*processes)[i].id = i + 1;           // 设置进程ID
        scanf("%d", &((*processes)[i].data)); // 读取每个进程的数据量
    }
}

// 处理进程传输
void processTransfers(Process *processes, int n, int s, float T)
{
    float currentTime = 0.0;
    while (currentTime < T)
    {
        for (int i = 0; i < n && currentTime < T; i++)
        {
            if (processes[i].data > 0)
            {                                                                                      // 如果进程还有数据需要传输
                float transferTime = (processes[i].data < s) ? (float)processes[i].data / s : 1.0; // 计算传输所需时间
                if (currentTime + transferTime > T)
                { // 如果当前时间加上传输时间超出总时间，调整传输时间
                    transferTime = T - currentTime;
                }
                int dataTransferred = s * transferTime; // 计算这次传输的数据量
                processes[i].data -= dataTransferred;   // 更新剩余数据量
                currentTime += transferTime;            // 更新当前时间
            }
        }
    }
}

// 打印最终进程状态
void printProcesses(Process *processes, int n)
{
    int allTransferred = 1;
    for (int i = 0; i < n; i++)
    {
        if (processes[i].data > 0)
        {
            printf("%d %d\n", processes[i].id, processes[i].data);
            allTransferred = 0;
        }
    }
    if (allTransferred)
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
