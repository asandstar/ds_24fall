// 暴力
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

typedef struct
{
    int sender;
    int receiver;
    int workload;
} Task;

int main()
{
    int num_computers;
    int *bandwidths;
    Task *tasks;
    int num_tasks = 0;
    int i, j;

    scanf("%d", &num_computers);

    bandwidths = (int *)malloc((num_computers - 1) * sizeof(int));
    for (i = 0; i < num_computers - 1; i++)
    {
        scanf("%d", &bandwidths[i]);
    }

    tasks = (Task *)malloc(100 * sizeof(Task));
    getchar();
    char line[100];
    while (fgets(line, sizeof(line), stdin) && line[0] != '\n')
    {
        if (sscanf(line, "%d %d %d", &tasks[num_tasks].sender, &tasks[num_tasks].receiver, &tasks[num_tasks].workload) == 3)
        {
            num_tasks++;
        }
    }

    LARGE_INTEGER nFreq;
    LARGE_INTEGER nBeginTime;
    LARGE_INTEGER nEndTime;
    double time;

    QueryPerformanceFrequency(&nFreq);
    QueryPerformanceCounter(&nBeginTime);

    for (i = 0; i < num_tasks; i++)
    {
        int sender = tasks[i].sender - 1;
        int receiver = tasks[i].receiver - 1;
        int workload = tasks[i].workload;

        if (sender > receiver)
        {
            int temp = sender;
            sender = receiver;
            receiver = temp;
        }

        // Find the minimum bandwidth on the path
        int min_bandwidth = bandwidths[sender];
        for (j = sender + 1; j < receiver; j++)
        {
            if (bandwidths[j] < min_bandwidth)
            {
                min_bandwidth = bandwidths[j];
            }
        }

        if (min_bandwidth >= workload)
        {
            printf("YES\n");
            for (j = sender; j < receiver; j++)
            {
                bandwidths[j] -= workload;
            }
        }
        else
        {
            printf("NO\n");
        }
    }
    QueryPerformanceCounter(&nEndTime);
    time = (double)(nEndTime.QuadPart - nBeginTime.QuadPart) / (double)nFreq.QuadPart;

    printf("%f\n", time);

    free(bandwidths);
    free(tasks);
    return 0;
}