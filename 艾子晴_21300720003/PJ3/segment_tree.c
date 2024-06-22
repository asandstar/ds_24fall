
// 线段树
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <windows.h>

#define INF 0x3f3f3f3f

typedef struct
{
    int min_bandwidth;
    int lazy;
} SegmentTreeNode;

typedef struct
{
    int sender;
    int receiver;
    int workload;
} Task;

void build_segment_tree(SegmentTreeNode *tree, int *bandwidths, int node, int start, int end)
{
    if (start == end)
    {
        tree[node].min_bandwidth = bandwidths[start];
        tree[node].lazy = 0;
    }
    else
    {
        int mid = (start + end) >> 1;
        int left_child = 2 * node + 1;
        int right_child = 2 * node + 2;
        build_segment_tree(tree, bandwidths, left_child, start, mid);
        build_segment_tree(tree, bandwidths, right_child, mid + 1, end);
        tree[node].min_bandwidth = (tree[left_child].min_bandwidth < tree[right_child].min_bandwidth) ? tree[left_child].min_bandwidth : tree[right_child].min_bandwidth;
        tree[node].lazy = 0;
    }
}

void propagate(SegmentTreeNode *tree, int node, int start, int end)
{
    if (tree[node].lazy != 0)
    {
        tree[node].min_bandwidth -= tree[node].lazy;
        if (start != end)
        {
            tree[2 * node + 1].lazy += tree[node].lazy;
            tree[2 * node + 2].lazy += tree[node].lazy;
        }
        tree[node].lazy = 0;
    }
}

int query_min_bandwidth(SegmentTreeNode *tree, int node, int start, int end, int l, int r)
{
    propagate(tree, node, start, end);
    if (r < start || l > end)
    {
        return INF;
    }
    if (l <= start && r >= end)
    {
        return tree[node].min_bandwidth;
    }
    int mid = (start + end) >> 1;
    int left_child = 2 * node + 1;
    int right_child = 2 * node + 2;
    int left_query = query_min_bandwidth(tree, left_child, start, mid, l, r);
    int right_query = query_min_bandwidth(tree, right_child, mid + 1, end, l, r);
    return (left_query < right_query) ? left_query : right_query;
}

void update_segment_tree(SegmentTreeNode *tree, int node, int start, int end, int l, int r, int value)
{
    propagate(tree, node, start, end);
    if (start > end || start > r || end < l)
    {
        return;
    }
    if (start >= l && end <= r)
    {
        tree[node].lazy += value;
        propagate(tree, node, start, end);
        return;
    }
    int mid = (start + end) >> 1;
    int left_child = 2 * node + 1;
    int right_child = 2 * node + 2;
    update_segment_tree(tree, left_child, start, mid, l, r, value);
    update_segment_tree(tree, right_child, mid + 1, end, l, r, value);
    tree[node].min_bandwidth = (tree[left_child].min_bandwidth < tree[right_child].min_bandwidth) ? tree[left_child].min_bandwidth : tree[right_child].min_bandwidth;
}

int main()
{
    int num_computers;
    int *bandwidths;
    Task *tasks;
    int num_tasks = 0;
    int i;

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
    // clock_t begin = clock();
    // time_t begin = time(NULL);
    LARGE_INTEGER nFreq;
    LARGE_INTEGER nBeginTime;
    LARGE_INTEGER nEndTime;
    double time;

    QueryPerformanceFrequency(&nFreq);
    QueryPerformanceCounter(&nBeginTime);

    int tree_size = 2 * (1 << (int)(ceil(log2(num_computers - 1)))) - 1;
    SegmentTreeNode *segment_tree = (SegmentTreeNode *)malloc(tree_size * sizeof(SegmentTreeNode));
    build_segment_tree(segment_tree, bandwidths, 0, 0, num_computers - 2);

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

        int min_bandwidth = query_min_bandwidth(segment_tree, 0, 0, num_computers - 2, sender, receiver - 1);

        if (min_bandwidth >= workload)
        {
            printf("YES\n");
            update_segment_tree(segment_tree, 0, 0, num_computers - 2, sender, receiver - 1, workload);
        }
        else
        {
            printf("NO\n");
        }
    }
    // time_t end = time(NULL);
    //  clock_t end = clock();
    //  int duration = (end - begin) / CLOCKS_PER_SEC;
    //   printf("%d", duration);

    QueryPerformanceCounter(&nEndTime);
    time = (double)(nEndTime.QuadPart - nBeginTime.QuadPart) / (double)nFreq.QuadPart;

    printf("%f\n", time);
    // Sleep(1000);
    // system("Pause");
    free(bandwidths);
    free(tasks);
    free(segment_tree);
    return 0;
}
