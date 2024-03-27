#include <stdio.h>
#include <stdlib.h>

// typedef struct
// {
//     int id;
//     int data;
// } Process;

typedef struct
{
    int id;
    int data;
    int startOrder; // 记录进程开始处理的顺序
} Process;

typedef struct
{
    Process *processes; // 进程数组
    int front;          // 队列头部索引
    int rear;           // 队列尾部索引，同时作为下一个进程的插入位置
    int count;          // 队列中元素的数量
    int capacity;       // 队列的容量
} Queue;

void initQueue(Queue *q, int capacity)
{
    q->processes = (Process *)malloc(capacity * sizeof(Process));
    q->front = 0;
    q->rear = 0;
    q->count = 0;
    q->capacity = capacity;
}

void enqueue(Queue *q, Process process)
{
    if (q->count < q->capacity)
    {
        q->processes[q->rear] = process;
        q->rear = (q->rear + 1) % q->capacity;
        q->count++;
    }
}

Process dequeue(Queue *q)
{
    Process process = {0, 0};
    if (q->count > 0)
    {
        process = q->processes[q->front];
        q->front = (q->front + 1) % q->capacity;
        q->count--;
    }
    return process;
}

void processQueue(Queue *q, int s, float T)
{
    float currentTime = 0.0;
    Process currentProcess;
    int isProcessing = 0; // 是否有进程正在处理

    while (currentTime < T)
    {
        if (!isProcessing)
        { // 如果没有进程正在处理
            if (q->count > 0)
            {
                currentProcess = dequeue(q);
                isProcessing = 1;
            }
            else
            {
                // 队列为空，结束处理
                break;
            }
        }

        // 计算处理时间
        float transferTime = (currentProcess.data <= s) ? (float)currentProcess.data / s : 1.0;
        if (currentTime + transferTime > T)
        {
            transferTime = T - currentTime; // 调整最后一次处理时间
        }
        currentProcess.data -= s * transferTime;
        currentTime += transferTime;

        // 判断进程是否完成
        if (currentProcess.data <= 0)
        {
            isProcessing = 0; // 处理完成
        }
        else if (currentTime >= T)
        {
            // 时间耗尽，但进程未完成，需要特殊处理
            // 将未完成的进程放回队首
            q->front = (q->front - 1 + q->capacity) % q->capacity;
            q->processes[q->front] = currentProcess;
            q->count++;
            break; // 结束循环
        }
        else
        {
            // 如果进程未完成，且时间未耗尽，正常放回队尾
            enqueue(q, currentProcess);
            isProcessing = 0; // 重置处理状态
        }
    }
}

int compareProcess(const void *a, const void *b)
{
    const Process *pa = (const Process *)a;
    const Process *pb = (const Process *)b;
    return pa->startOrder - pb->startOrder; // 根据处理顺序进行比较
}

void printQueue(Queue *q)
{
    if (q->count == 0)
    {
        printf("DONE\n");
    }
    else
    {
        // 创建一个临时数组用于存放并排序未完成的进程
        Process *tempArray = (Process *)malloc(q->count * sizeof(Process));
        if (!tempArray)
        {
            // 内存分配失败
            return;
        }

        // 复制未完成的进程到临时数组
        for (int i = 0; i < q->count; i++)
        {
            int index = (q->front + i) % q->capacity;
            tempArray[i] = q->processes[index];
        }

        // 对临时数组进行排序
        qsort(tempArray, q->count, sizeof(Process), compareProcess);

        // 打印排序后的未完成进程
        for (int i = 0; i < q->count; i++)
        {
            printf("%d %d\n", tempArray[i].id, tempArray[i].data);
        }

        free(tempArray); // 释放临时数组的内存
    }
}

// void printQueue(Queue *q)
// {
//     // 检查队列是否为空
//     if (q->count == 0)
//     {
//         printf("DONE\n");
//     }
//     else
//     {
//         for (int i = q->front, j = 0; j < q->count; i = (i + 1) % q->capacity, j++)
//         {
//             printf("%d %d\n", q->processes[i].id, q->processes[i].data);
//         }
//     }
// }

int main()
{
    int n, s;
    scanf("%d %d", &n, &s);

    Queue queue;
    initQueue(&queue, n);

    for (int i = 0; i < n; i++)
    {
        Process process;
        scanf("%d", &process.data);
        process.id = i + 1;
        enqueue(&queue, process);
    }

    float T;
    scanf("%f", &T);

    processQueue(&queue, s, T);
    printQueue(&queue);

    free(queue.processes);
    return 0;
}