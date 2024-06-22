#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void generate_test_data(int num_computers, int num_tasks)
{
    printf("%d\n", num_computers);
    for (int i = 0; i < num_computers - 1; i++)
    {
        printf("%d ", rand() % 1000 + 100);
    }
    printf("\n");
    for (int i = 0; i < num_tasks; i++)
    {
        int sender = rand() % num_computers + 1;
        int receiver;
        do
        {
            receiver = rand() % num_computers + 1;
        } while (receiver == sender);
        int workload = rand() % 100 + 50;
        printf("%d %d %d\n", sender, receiver, workload);
    }
}

int main()
{
    srand(time(NULL));
    int num_computers = 1000;
    int num_tasks = 10000;
    generate_test_data(num_computers, num_tasks);
    return 0;
}
