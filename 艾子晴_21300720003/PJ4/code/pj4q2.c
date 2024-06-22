#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_NODES 100
#define INF INT_MAX

// 使用DFS找到连通分量并计算最小带宽
void dfs(int node, int graph[MAX_NODES][MAX_NODES], bool visited[], int n, int *minBandwidth, int *nodeCount)
{
    visited[node] = true;
    (*nodeCount)++;
    for (int i = 0; i < n; i++)
    {
        if (graph[node][i] != INF && !visited[i])
        {
            if (graph[node][i] < *minBandwidth)
            {
                *minBandwidth = graph[node][i];
            }
            dfs(i, graph, visited, n, minBandwidth, nodeCount);
        }
    }
}

// 计算单个异常边对网络的总影响
int calculateImpact(int graph[MAX_NODES][MAX_NODES], int n, int u, int v, int faultyBandwidth)
{
    int originalBandwidth = graph[u][v];
    int totalImpact = 0;

    // 恢复原始带宽
    graph[u][v] = originalBandwidth;
    graph[v][u] = originalBandwidth;

    // 计算连通分量的最小带宽和节点数
    bool visited[MAX_NODES] = {false};
    int minBandwidth = INF;
    int nodeCount = 0;
    dfs(u, graph, visited, n, &minBandwidth, &nodeCount);

    // 如果最小带宽是原始带宽，那么异常后的最小带宽将是faultyBandwidth
    if (minBandwidth == originalBandwidth)
    {
        minBandwidth = faultyBandwidth;
    }

    int bandwidthDecrease = originalBandwidth - minBandwidth;
    int impact = bandwidthDecrease * nodeCount;

    totalImpact += impact;

    // 恢复异常带宽
    graph[u][v] = faultyBandwidth;
    graph[v][u] = faultyBandwidth;

    return totalImpact;
}

// 找出能够最小化网络影响的异常边替换策略
void minimizeNetworkImpact(int graph[MAX_NODES][MAX_NODES], int n, int N[][3], int nSize, int k, int result[])
{
    int impacts[nSize];
    int indices[nSize];

    for (int i = 0; i < nSize; i++)
    {
        int u = N[i][0];
        int v = N[i][1];
        int faultyBandwidth = N[i][2];
        impacts[i] = calculateImpact(graph, n, u, v, faultyBandwidth);
        indices[i] = i;
    }

    // 按影响排序并选择最小的k个
    for (int i = 0; i < nSize - 1; i++)
    {
        for (int j = 0; j < nSize - i - 1; j++)
        {
            if (impacts[j] > impacts[j + 1] || (impacts[j] == impacts[j + 1] && indices[j] > indices[j + 1]))
            {
                int tempImpact = impacts[j];
                impacts[j] = impacts[j + 1];
                impacts[j + 1] = tempImpact;

                int tempIdx = indices[j];
                indices[j] = indices[j + 1];
                indices[j + 1] = tempIdx;
            }
        }
    }

    // 将选择的k个最小影响的边索引存入result数组
    for (int i = 0; i < k; i++)
    {
        result[i] = indices[i];
    }
}

int main()
{
    int V, E;
    scanf("%d %d", &V, &E);

    int graph[MAX_NODES][MAX_NODES];
    // 初始化图的邻接矩阵
    for (int i = 0; i < V; i++)
    {
        for (int j = 0; j < V; j++)
        {
            if (i == j)
            {
                graph[i][j] = 0;
            }
            else
            {
                graph[i][j] = INF;
            }
        }
    }

    // 读取边的连接信息，初始化带宽为50
    for (int i = 0; i < E; i++)
    {
        int u, v, e;
        scanf("%d %d %d", &u, &v, &e);
        graph[u - 1][v - 1] = 50; // 带宽为50
        graph[v - 1][u - 1] = 50; // 无向图
    }

    // 读取异常边的信息
    int nSize;
    scanf("%d", &nSize);
    int N[nSize][3];
    for (int i = 0; i < nSize; i++)
    {
        int index;
        scanf("%d", &index);
        N[i][0] = index / 2;                                    // 起始节点索引
        N[i][1] = (index % 2 == 0) ? index / 2 + 1 : index / 2; // 结束节点索引
        N[i][2] = 70;                                           // 异常后的带宽
    }

    // 读取可替换的异常边数量
    int k;
    scanf("%d", &k);

    int result[k];
    // 调用函数计算最小化影响的替换边
    minimizeNetworkImpact(graph, V, N, nSize, k, result);

    // 输出结果
    printf("Edges to replace: ");
    for (int i = 0; i < k; i++)
    {
        printf("%d ", result[i]);
    }
    printf("\n");

    return 0;
}
