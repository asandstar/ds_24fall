#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int id;
    int data;
} Process;

typedef struct
{
    Process *processes; // 进程数组
    int front;          // 队列头部索引
    int rear;           // 队列尾部索引，同时作为下一个进程的插入位置
    int count;          // 队列中元素的数量
    int size;           // 队列的大小
} Queue;

void initQueue(Queue *q, int size)
{
    q->processes = (Process *)malloc(size * sizeof(Process));
    q->front = 0;
    q->rear = 0;
    q->count = 0;
    q->size = size;
}

void enqueue(Queue *q, Process process)
{
    if (q->count < q->size) // 入队限制条件，队列未满
    {
        q->processes[q->rear] = process;
        // 将新进程process放入队列的尾部
        // q->rear指的是队列尾部下一个空闲位置的索引
        q->rear = (q->rear + 1) % q->size;
        // 更新rear指针的位置，使其指向下一个空闲的位置
        // 取模运算使指针在到达队列最大索引后能够循环回到队列的开始位置
        q->count++;
        // 队列内元素数目+1
    }
}
// 上面的入队是把处理完的元素（本题是进程）继续放入待处理的等待队列里面

Process dequeue(Queue *q)
{
    Process process = {0, 0}; // 出队初始化，id和data均为0
    // 在队列为空时返回一个预定义的空Process对象
    if (q->count > 0) // 出队限制条件，队列不为空
    {
        process = q->processes[q->front];
        // 将队列前端的元素（即队列中最早加入的元素）赋值给process
        // 将要从队列中“移除”的元素（虽然物理上它还在数组中，但逻辑上我们将其视为已经被移除）
        q->front = (q->front + 1) % q->size;
        q->count--;
    }
    return process;
}
// 将一个进程从队列中移除，目的是为了对这个进程进行进一步处理

void processQueue(Queue *q, int s, float T)
{
    float currentTime = 0.0;
    Process currentProcess;
    int isProcessing = 0; // 是否有进程正在处理，标志位，默认没有进程在处理

    while (currentTime <= T)
    {
        if (!isProcessing)
        { // 如果没有进程正在处理
            // 下面就开始处理进程
            if (q->count > 0) // 队列里的元素不为空
            {
                currentProcess = dequeue(q); // 想处理的进程，通过队列出队得到
                isProcessing = 1;            // 正在处理进程
            }
            else
            {
                // 队列为空，结束处理
                break;
            }
        }

        // 计算处理时间
        float processTime = (float)currentProcess.data / s;
        // 当前进程传输所需时间
        float transferTime = (currentProcess.data <= s) ? processTime : 1.0;
        // 是否到达指定总时间
        // 超过则调整最后一次处理时间
        if (currentTime + transferTime > T)
        {
            transferTime = T - currentTime;
        }
        // 对每个时间片，当前进程传输完数据后对剩余的数据进行存储
        currentProcess.data -= s * transferTime;
        // 现在的时间
        currentTime += transferTime;

        // 判断进程是否完成
        if (currentProcess.data <= 0) // 进程处理完成
        {
            isProcessing = 0; // 标志清0，不放回队列
        }
        else if (currentTime < T)
        {
            // 进程未完成，且时间未耗尽，正常放回队尾，下一轮继续处理
            enqueue(q, currentProcess);
            isProcessing = 0; // 重置处理状态
        }
        else
        {
            // 时间耗尽，但进程未完成，需要特殊处理
            // 将未完成的进程放回队首
            q->front = (q->front - 1 + q->size) % q->size;
            q->processes[q->front] = currentProcess;
            q->count++;
            break; // 结束循环
        }
    }
}

void printQueue(Queue *q)
{
    // 检查队列是否为空
    if (q->count == 0)
    {
        printf("DONE\n");
    }
    else
    {
        for (int i = q->front, j = q->count; j > 0; i = (i + 1) % q->size, j--)
        {
            printf("%d %d\n", q->processes[i].id, q->processes[i].data);
        }
    }
}

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