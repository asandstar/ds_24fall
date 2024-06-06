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

    free(bandwidths);
    free(tasks);
    return 0;
}

// #include <stdio.h>
// #include <stdlib.h>
// #include <math.h>

// #define INF 0x3f3f3f3f

// typedef struct
// {
//     int min_bandwidth;
//     int lazy;
// } SegmentTreeNode;

// typedef struct
// {
//     int sender;
//     int receiver;
//     int workload;
// } Task;

// void build_segment_tree(SegmentTreeNode *tree, int *bandwidths, int node, int start, int end)
// {
//     if (start == end)
//     {
//         tree[node].min_bandwidth = bandwidths[start];
//         tree[node].lazy = 0;
//     }
//     else
//     {
//         int mid = (start + end) >> 1;
//         int left_child = 2 * node + 1;
//         int right_child = 2 * node + 2;
//         build_segment_tree(tree, bandwidths, left_child, start, mid);
//         build_segment_tree(tree, bandwidths, right_child, mid + 1, end);
//         tree[node].min_bandwidth = (tree[left_child].min_bandwidth < tree[right_child].min_bandwidth) ? tree[left_child].min_bandwidth : tree[right_child].min_bandwidth;
//         tree[node].lazy = 0;
//     }
// }

// void propagate(SegmentTreeNode *tree, int node, int start, int end)
// {
//     if (tree[node].lazy != 0)
//     {
//         tree[node].min_bandwidth -= tree[node].lazy;
//         if (start != end)
//         {
//             tree[2 * node + 1].lazy += tree[node].lazy;
//             tree[2 * node + 2].lazy += tree[node].lazy;
//         }
//         tree[node].lazy = 0;
//     }
// }

// int query_min_bandwidth(SegmentTreeNode *tree, int node, int start, int end, int l, int r)
// {
//     propagate(tree, node, start, end);
//     if (r < start || l > end)
//     {
//         return INF;
//     }
//     if (l <= start && r >= end)
//     {
//         return tree[node].min_bandwidth;
//     }
//     int mid = (start + end) >> 1;
//     int left_child = 2 * node + 1;
//     int right_child = 2 * node + 2;
//     int left_query = query_min_bandwidth(tree, left_child, start, mid, l, r);
//     int right_query = query_min_bandwidth(tree, right_child, mid + 1, end, l, r);
//     return (left_query < right_query) ? left_query : right_query;
// }

// void update_segment_tree(SegmentTreeNode *tree, int node, int start, int end, int l, int r, int value)
// {
//     propagate(tree, node, start, end);
//     if (start > end || start > r || end < l)
//     {
//         return;
//     }
//     if (start >= l && end <= r)
//     {
//         tree[node].lazy += value;
//         propagate(tree, node, start, end);
//         return;
//     }
//     int mid = (start + end) >> 1;
//     int left_child = 2 * node + 1;
//     int right_child = 2 * node + 2;
//     update_segment_tree(tree, left_child, start, mid, l, r, value);
//     update_segment_tree(tree, right_child, mid + 1, end, l, r, value);
//     tree[node].min_bandwidth = (tree[left_child].min_bandwidth < tree[right_child].min_bandwidth) ? tree[left_child].min_bandwidth : tree[right_child].min_bandwidth;
// }

// int main()
// {
//     int num_computers;
//     int *bandwidths;
//     Task *tasks;
//     int num_tasks = 0;
//     int i;

//     scanf("%d", &num_computers);

//     bandwidths = (int *)malloc((num_computers - 1) * sizeof(int));
//     for (i = 0; i < num_computers - 1; i++)
//     {
//         scanf("%d", &bandwidths[i]);
//     }

//     tasks = (Task *)malloc(100 * sizeof(Task));
//     getchar();
//     char line[100];
//     while (fgets(line, sizeof(line), stdin) && line[0] != '\n')
//     {
//         if (sscanf(line, "%d %d %d", &tasks[num_tasks].sender, &tasks[num_tasks].receiver, &tasks[num_tasks].workload) == 3)
//         {
//             num_tasks++;
//         }
//     }

//     int tree_size = 2 * (1 << (int)(ceil(log2(num_computers - 1)))) - 1;
//     SegmentTreeNode *segment_tree = (SegmentTreeNode *)malloc(tree_size * sizeof(SegmentTreeNode));
//     build_segment_tree(segment_tree, bandwidths, 0, 0, num_computers - 2);

//     for (i = 0; i < num_tasks; i++)
//     {
//         int sender = tasks[i].sender - 1;
//         int receiver = tasks[i].receiver - 1;
//         int workload = tasks[i].workload;

//         if (sender > receiver)
//         {
//             int temp = sender;
//             sender = receiver;
//             receiver = temp;
//         }

//         int min_bandwidth = query_min_bandwidth(segment_tree, 0, 0, num_computers - 2, sender, receiver - 1);

//         if (min_bandwidth >= workload)
//         {
//             printf("YES\n");
//             update_segment_tree(segment_tree, 0, 0, num_computers - 2, sender, receiver - 1, workload);
//         }
//         else
//         {
//             printf("NO\n");
//         }
//     }

//     free(bandwidths);
//     free(tasks);
//     free(segment_tree);
//     return 0;
// }

// // // #include <stdio.h>
// // // #include <stdlib.h>

// // // typedef struct
// // // {
// // //     int sender;
// // //     int receiver;
// // //     int workload;
// // // } Task;

// // // int main()
// // // {
// // //     int num_computers;
// // //     int *bandwidths;
// // //     Task *tasks;
// // //     int num_tasks = 0;
// // //     int i;

// // //     scanf("%d", &num_computers);

// // //     bandwidths = (int *)malloc((num_computers - 1) * sizeof(int));
// // //     for (i = 0; i < num_computers - 1; i++)
// // //     {
// // //         scanf("%d", &bandwidths[i]);
// // //     }

// // //     tasks = (Task *)malloc(100 * sizeof(Task));
// // //     getchar();
// // //     char line[100];
// // //     while (fgets(line, sizeof(line), stdin) && line[0] != '\n')
// // //     {
// // //         if (sscanf(line, "%d %d %d", &tasks[num_tasks].sender, &tasks[num_tasks].receiver, &tasks[num_tasks].workload) == 3)
// // //         {
// // //             num_tasks++;
// // //         }
// // //     }

// // //     for (i = 0; i < num_tasks; i++)
// // //     {
// // //         int sender = tasks[i].sender - 1;
// // //         int receiver = tasks[i].receiver - 1;
// // //         int workload = tasks[i].workload;

// // //         if (sender > receiver)
// // //         {
// // //             int temp = sender;
// // //             sender = receiver;
// // //             receiver = temp;
// // //         }

// // //         int can_handle = 1;
// // //         for (int j = sender; j < receiver; j++)
// // //         {
// // //             if (bandwidths[j] < workload)
// // //             {
// // //                 can_handle = 0;
// // //                 break;
// // //             }
// // //         }

// // //         if (can_handle)
// // //         {
// // //             printf("YES\n");
// // //             for (int j = sender; j < receiver; j++)
// // //             {
// // //                 bandwidths[j] -= workload;
// // //             }
// // //         }
// // //         else
// // //         {
// // //             printf("NO\n");
// // //         }
// // //     }

// // //     free(bandwidths);
// // //     free(tasks);
// // //     return 0;
// // // }

// // #include <stdio.h>
// // #include <stdlib.h>
// // #include <math.h>

// // typedef struct
// // {
// //     int sender;
// //     int receiver;
// //     int workload;
// // } Task;

// // typedef struct
// // {
// //     int max_bandwidth;
// //     int min_bandwidth;
// // } SegmentTreeNode;

// // void build_segment_tree(SegmentTreeNode *tree, int *bandwidths, int node, int start, int end)
// // {
// //     if (start == end)
// //     {
// //         tree[node].max_bandwidth = bandwidths[start];
// //         tree[node].min_bandwidth = bandwidths[start];
// //     }
// //     else
// //     {
// //         int mid = (start + end) / 2;
// //         int left_child = 2 * node + 1;
// //         int right_child = 2 * node + 2;
// //         build_segment_tree(tree, bandwidths, left_child, start, mid);
// //         build_segment_tree(tree, bandwidths, right_child, mid + 1, end);
// //         tree[node].max_bandwidth = (tree[left_child].max_bandwidth > tree[right_child].max_bandwidth) ? tree[left_child].max_bandwidth : tree[right_child].max_bandwidth;
// //         tree[node].min_bandwidth = (tree[left_child].min_bandwidth < tree[right_child].min_bandwidth) ? tree[left_child].min_bandwidth : tree[right_child].min_bandwidth;
// //     }
// // }

// // int query_min_bandwidth(SegmentTreeNode *tree, int node, int start, int end, int l, int r)
// // {
// //     if (r < start || l > end)
// //     {
// //         return 100;
// //     }
// //     if (l <= start && r >= end)
// //     {
// //         return tree[node].min_bandwidth;
// //     }
// //     int mid = (start + end) / 2;
// //     int left_child = 2 * node + 1;
// //     int right_child = 2 * node + 2;
// //     int left_query = query_min_bandwidth(tree, left_child, start, mid, l, r);
// //     int right_query = query_min_bandwidth(tree, right_child, mid + 1, end, l, r);
// //     return (left_query < right_query) ? left_query : right_query;
// // }

// // void update_segment_tree(SegmentTreeNode *tree, int node, int start, int end, int idx, int value)
// // {
// //     if (start == end)
// //     {
// //         tree[node].max_bandwidth = value;
// //         tree[node].min_bandwidth = value;
// //     }
// //     else
// //     {
// //         int mid = (start + end) / 2;
// //         int left_child = 2 * node + 1;
// //         int right_child = 2 * node + 2;
// //         if (idx <= mid)
// //         {
// //             update_segment_tree(tree, left_child, start, mid, idx, value);
// //         }
// //         else
// //         {
// //             update_segment_tree(tree, right_child, mid + 1, end, idx, value);
// //         }
// //         tree[node].max_bandwidth = (tree[left_child].max_bandwidth > tree[right_child].max_bandwidth) ? tree[left_child].max_bandwidth : tree[right_child].max_bandwidth;
// //         tree[node].min_bandwidth = (tree[left_child].min_bandwidth < tree[right_child].min_bandwidth) ? tree[left_child].min_bandwidth : tree[right_child].min_bandwidth;
// //     }
// // }

// // int main()
// // {
// //     int num_computers;
// //     int *bandwidths;
// //     Task *tasks;
// //     int num_tasks = 0;
// //     int i;

// //     scanf("%d", &num_computers);

// //     bandwidths = (int *)malloc((num_computers - 1) * sizeof(int));
// //     for (i = 0; i < num_computers - 1; i++)
// //     {
// //         scanf("%d", &bandwidths[i]);
// //     }

// //     tasks = (Task *)malloc(100 * sizeof(Task));
// //     getchar();
// //     char line[100];
// //     while (fgets(line, sizeof(line), stdin) && line[0] != '\n')
// //     {
// //         if (sscanf(line, "%d %d %d", &tasks[num_tasks].sender, &tasks[num_tasks].receiver, &tasks[num_tasks].workload) == 3)
// //         {
// //             num_tasks++;
// //         }
// //     }
// //     int tree_size = 2 * (1 << (int)(ceil(log2(num_computers - 1)))) - 1;
// //     SegmentTreeNode *segment_tree = (SegmentTreeNode *)malloc(tree_size * sizeof(SegmentTreeNode));
// //     build_segment_tree(segment_tree, bandwidths, 0, 0, num_computers - 2);

// //     for (i = 0; i < num_tasks; i++)
// //     {
// //         int sender = tasks[i].sender - 1;
// //         int receiver = tasks[i].receiver - 1;
// //         int workload = tasks[i].workload;

// //         if (sender > receiver)
// //         {
// //             int temp = sender;
// //             sender = receiver;
// //             receiver = temp;
// //         }

// //         int min_bandwidth = query_min_bandwidth(segment_tree, 0, 0, num_computers - 2, sender, receiver - 1);

// //         if (min_bandwidth >= workload)
// //         {
// //             printf("YES\n");
// //             for (int j = sender; j <= receiver - 1; j++)
// //             {
// //                 bandwidths[j] -= workload;
// //                 update_segment_tree(segment_tree, 0, 0, num_computers - 2, j, bandwidths[j]);
// //             }
// //         }
// //         else
// //         {
// //             printf("NO\n");
// //         }
// //     }
// //     free(bandwidths);
// //     free(tasks);
// //     free(segment_tree);
// //     return 0;
// // }

// // //-------------------------------------------------------------------------------------
// // // #include <stdio.h>
// // // #include <stdlib.h>
// // // #include <time.h>

// // // typedef struct
// // // {
// // //     int sender;
// // //     int receiver;
// // //     int workload;
// // // } Task;

// // // void brute_force_algorithm(int num_computers, int *bandwidths, Task *tasks, int num_tasks);
// // // void segment_tree_algorithm(int num_computers, int *bandwidths, Task *tasks, int num_tasks);

// // // // 主函数，进行性能测试
// // // int main()
// // // {
// // //     // 定义数据集
// // //     int num_computers;
// // //     int *bandwidths;
// // //     Task *tasks;
// // //     int num_tasks;
// // //     int original_bandwidths[20];

// // //     // 读取数据集（以数据集1为例，其他数据集类似）
// // //     num_computers = 3;
// // //     int bandwidths1[] = {100, 100};
// // //     Task tasks1[] = {
// // //         {1, 2, 80},
// // //         {2, 3, 50},
// // //         {1, 3, 50},
// // //         {3, 2, 50},
// // //         {3, 2, 10}};
// // //     num_tasks = sizeof(tasks1) / sizeof(tasks1[0]);

// // //     bandwidths = bandwidths1;
// // //     tasks = tasks1;
// // //     memcpy(original_bandwidths, bandwidths, sizeof(bandwidths1));

// // //     // 暴力算法测试
// // //     clock_t start = clock();
// // //     brute_force_algorithm(num_computers, bandwidths, tasks, num_tasks);
// // //     clock_t end = clock();
// // //     double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
// // //     printf("Brute force algorithm time: %f seconds\n", time_spent);

// // //     // 恢复带宽数据
// // //     memcpy(bandwidths, original_bandwidths, sizeof(bandwidths1));

// // //     // 线段树算法测试
// // //     start = clock();
// // //     segment_tree_algorithm(num_computers, bandwidths, tasks, num_tasks);
// // //     end = clock();
// // //     time_spent = (double)(end - start) / CLOCKS_PER_SEC;
// // //     printf("Segment tree algorithm time: %f seconds\n", time_spent);

// // //     return 0;
// // // }

// // // // 暴力算法实现
// // // void brute_force_algorithm(int num_computers, int *bandwidths, Task *tasks, int num_tasks)
// // // {
// // //     for (int i = 0; i < num_tasks; i++)
// // //     {
// // //         int sender = tasks[i].sender - 1;
// // //         int receiver = tasks[i].receiver - 1;
// // //         int workload = tasks[i].workload;
// // //         int can_accept = 1;

// // //         // 确保sender < receiver
// // //         if (sender > receiver)
// // //         {
// // //             int temp = sender;
// // //             sender = receiver;
// // //             receiver = temp;
// // //         }

// // //         // 检查路径上带宽是否足够
// // //         for (int j = sender; j < receiver; j++)
// // //         {
// // //             if (bandwidths[j] < workload)
// // //             {
// // //                 can_accept = 0;
// // //                 break;
// // //             }
// // //         }

// // //         // 受理任务并更新带宽
// // //         if (can_accept)
// // //         {
// // //             printf("YES\n");
// // //             for (int j = sender; j < receiver; j++)
// // //             {
// // //                 bandwidths[j] -= workload;
// // //             }
// // //         }
// // //         else
// // //         {
// // //             printf("NO\n");
// // //         }
// // //     }
// // // }

// // // // 线段树算法实现
// // // typedef struct
// // // {
// // //     int max_bandwidth;
// // //     int min_bandwidth;
// // // } SegmentTreeNode;

// // // void build_segment_tree(SegmentTreeNode *tree, int *bandwidths, int node, int start, int end)
// // // {
// // //     if (start == end)
// // //     {
// // //         tree[node].max_bandwidth = bandwidths[start];
// // //         tree[node].min_bandwidth = bandwidths[start];
// // //     }
// // //     else
// // //     {
// // //         int mid = (start + end) / 2;
// // //         int left_child = 2 * node + 1;
// // //         int right_child = 2 * node + 2;
// // //         build_segment_tree(tree, bandwidths, left_child, start, mid);
// // //         build_segment_tree(tree, bandwidths, right_child, mid + 1, end);
// // //         tree[node].max_bandwidth = (tree[left_child].max_bandwidth > tree[right_child].max_bandwidth) ? tree[left_child].max_bandwidth : tree[right_child].max_bandwidth;
// // //         tree[node].min_bandwidth = (tree[left_child].min_bandwidth < tree[right_child].min_bandwidth) ? tree[left_child].min_bandwidth : tree[right_child].min_bandwidth;
// // //     }
// // // }

// // // int query_min_bandwidth(SegmentTreeNode *tree, int node, int start, int end, int l, int r)
// // // {
// // //     if (r < start || l > end)
// // //     {
// // //         return 100;
// // //     }
// // //     if (l <= start && r >= end)
// // //     {
// // //         return tree[node].min_bandwidth;
// // //     }
// // //     int mid = (start + end) / 2;
// // //     int left_child = 2 * node + 1;
// // //     int right_child = 2 * node + 2;
// // //     int left_query = query_min_bandwidth(tree, left_child, start, mid, l, r);
// // //     int right_query = query_min_bandwidth(tree, right_child, mid + 1, end, l, r);
// // //     return (left_query < right_query) ? left_query : right_query;
// // // }

// // // void update_segment_tree(SegmentTreeNode *tree, int node, int start, int end, int idx, int value)
// // // {
// // //     if (start == end)
// // //     {
// // //         tree[node].max_bandwidth = value;
// // //         tree[node].min_bandwidth = value;
// // //     }
// // //     else
// // //     {
// // //         int mid = (start + end) / 2;
// // //         int left_child = 2 * node + 1;
// // //         int right_child = 2 * node + 2;
// // //         if (idx <= mid)
// // //         {
// // //             update_segment_tree(tree, left_child, start, mid, idx, value);
// // //         }
// // //         else
// // //         {
// // //             update_segment_tree(tree, right_child, mid + 1, end, idx, value);
// // //         }
// // //         tree[node].max_bandwidth = (tree[left_child].max_bandwidth > tree[right_child].max_bandwidth) ? tree[left_child].max_bandwidth : tree[right_child].max_bandwidth;
// // //         tree[node].min_bandwidth = (tree[left_child].min_bandwidth < tree[right_child].min_bandwidth) ? tree[left_child].min_bandwidth : tree[right_child].min_bandwidth;
// // //     }
// // // }

// // // void segment_tree_algorithm(int num_computers, int *bandwidths, Task *tasks, int num_tasks)
// // // {
// // //     int tree_size = 2 * (1 << (int)(ceil(log2(num_computers - 1)))) - 1;
// // //     SegmentTreeNode *segment_tree = (SegmentTreeNode *)malloc(tree_size * sizeof(SegmentTreeNode));
// // //     build_segment_tree(segment_tree, bandwidths, 0, 0, num_computers - 2);

// // //     for (int i = 0; i < num_tasks; i++)
// // //     {
// // //         int sender = tasks[i].sender - 1;
// // //         int receiver = tasks[i].receiver - 1;
// // //         int workload = tasks[i].workload;

// // //         if (sender > receiver)
// // //         {
// // //             int temp = sender;
// // //             sender = receiver;
// // //             receiver = temp;
// // //         }

// // //         int min_bandwidth = query_min_bandwidth(segment_tree, 0, 0, num_computers - 2, sender, receiver - 1);

// // //         if (min_bandwidth >= workload)
// // //         {
// // //             printf("YES\n");
// // //             for (int j = sender; j <= receiver - 1; j++)
// // //             {
// // //                 bandwidths[j] -= workload;
// // //                 update_segment_tree(segment_tree, 0, 0, num_computers - 2, j, bandwidths[j]);
// // //             }
// // //         }
// // //         else
// // //         {
// // //             printf("NO\n");
// // //         }
// // //     }
// // //     free(segment_tree);
// // // }
