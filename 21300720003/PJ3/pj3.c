#include <stdio.h>
#include <stdlib.h>

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
    int i;

    scanf("%d", &num_computers);
    getchar();

    bandwidths = (int *)malloc((num_computers - 1) * sizeof(int));
    for (i = 0; i < num_computers - 1; i++)
    {
        scanf("%d", &bandwidths[i]);
    }
    getchar();

    tasks = (Task *)malloc(10 * sizeof(Task));
    while (1)
    {
        char line[100];
        if (!fgets(line, sizeof(line), stdin) || line[0] == '\n')
        {
            break;
        }
        if (sscanf(line, "%d %d %d", &tasks[num_tasks].sender, &tasks[num_tasks].receiver, &tasks[num_tasks].workload) == 3)
        {
            num_tasks++;
        }
    }

    printf("Number of computers: %d\n", num_computers);
    printf("Bandwidths: ");
    for (i = 0; i < num_computers - 1; i++)
    {
        printf("%d ", bandwidths[i]);
    }
    printf("\n");

    printf("Tasks:\n");
    for (i = 0; i < num_tasks; i++)
    {
        printf("Sender: %d, Receiver: %d, Workload: %d\n",
               tasks[i].sender, tasks[i].receiver, tasks[i].workload);
    }

    free(bandwidths);
    free(tasks);
    return 0;
}