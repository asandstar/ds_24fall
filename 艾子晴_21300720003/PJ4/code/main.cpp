#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>
#include <windows.h>

#define MAX_NODES 100

using namespace std;

void dfs(int node, vector<vector<int>> &graph, vector<bool> &visited, int n, vector<int> &component)
{
    visited[node] = true;
    component.push_back(node);
    for (int i = 0; i < n; i++)
    {
        if (graph[node][i] == 1 && !visited[i])
        {
            dfs(i, graph, visited, n, component);
        }
    }
}

void findComponents(vector<vector<int>> &graph, int n, vector<vector<int>> &components)
{
    vector<bool> visited(n, false);

    for (int i = 0; i < n; i++)
    {
        if (!visited[i])
        {
            vector<int> component;
            dfs(i, graph, visited, n, component);
            components.push_back(component);
        }
    }
}

bool isAffected(const vector<int> &component, const vector<int> &S)
{
    for (int node : component)
    {
        if (find(S.begin(), S.end(), node) != S.end())
        {
            return true;
        }
    }
    return false;
}

void minimizeImpact(vector<vector<int>> &graph, int n, vector<int> &S, int k, vector<int> &result)
{
    vector<vector<int>> components;
    findComponents(graph, n, components);

    vector<int> replaceCandidates;
    vector<int> affectedComponents;
    int replaceCount = 0;

    for (int i = 0; i < components.size(); i++)
    {
        if (isAffected(components[i], S))
        {
            for (int j : components[i])
            {
                replaceCandidates.push_back(j);
                affectedComponents.push_back(i); // 记录该候选节点属于哪个连通分量
                replaceCount++;
            }
        }
    }

    // 选择每个受影响的连通分量中的一个节点进行替换
    vector<int> selectedComponents(components.size(), 0); // 记录每个连通分量是否已经有节点被选择替换
    int selectedCount = 0;
    for (int i = 0; i < replaceCount && selectedCount < k; i++)
    {
        int maxIdx = -1;
        for (int j = 0; j < replaceCount; j++)
        {
            if (selectedComponents[affectedComponents[j]] == 0)
            {
                maxIdx = j;
                break;
            }
        }
        if (maxIdx != -1)
        {
            result.push_back(replaceCandidates[maxIdx]);
            selectedComponents[affectedComponents[maxIdx]] = 1;
            selectedCount++;
        }
    }

    // 如果还没选满k个节点，再进行贪心选择
    for (int i = selectedCount; i < k; i++)
    {
        int maxIdx = -1;
        for (int j = 0; j < replaceCount; j++)
        {
            if (selectedComponents[affectedComponents[j]] == 0)
            {
                maxIdx = j;
                break;
            }
        }
        if (maxIdx != -1)
        {
            result.push_back(replaceCandidates[maxIdx]);
            selectedComponents[affectedComponents[maxIdx]] = 1;
        }
    }
}

int main()
{
    int V, E;
    cin >> V >> E;

    vector<vector<int>> graph(MAX_NODES, vector<int>(MAX_NODES, 0));

    for (int i = 0; i < E; i++)
    {
        int u, v, e;
        cin >> u >> v >> e;
        graph[u - 1][v - 1] = e; // 注意这里是基于1的索引，所以减去1
        graph[v - 1][u - 1] = e; // 无向图
    }

    vector<int> S;
    string line;
    cin.ignore();
    getline(cin, line); // 读取异常服务器行
    char *token = strtok(&line[0], " ");
    while (token != NULL)
    {
        S.push_back(atoi(token) - 1); // 注意这里是基于1的索引，所以减去1
        token = strtok(NULL, " ");    // 继续解析下一部分
    }

    int k;
    cin >> k;

    vector<int> result;
    minimizeImpact(graph, V, S, k, result);

    cout << "Nodes to replace: ";
    for (int i = 0; i < k; i++)
    {
        cout << result[i] + 1 << " "; // 输出时加回1
    }
    cout << endl;
    Sleep(10000); // 等待10秒
    return 0;
}

/*C语言版本*/
// #include <stdio.h>
// #include <stdlib.h>
// #include <stdbool.h>
// #include <string.h>

// #define MAX_NODES 100

// /*
// int node: 当前正在访问的节点。
// bool visited[]: 记录每个节点是否已被访问的布尔数组。
// int graph[MAX_NODES][MAX_NODES]: 图的邻接矩阵表示，其中 graph[i][j] 表示节点 i 和 j 之间是否有连接。
// int n: 节点的总数。
// int component[]: 用于存储在当前深度优先搜索过程中遇到的所有节点，也就是一个联通分量中的所有节点。
// 包含了从起始节点开始所有能够到达的节点集合。
// int *size: 指向 component 数组当前大小的指针，便于在递归中修改。
// */
// void dfs(int node, bool visited[], int graph[MAX_NODES][MAX_NODES], int n, int component[], int *size)
// {
//     visited[node] = true;
//     component[(*size)++] = node;
//     for (int i = 0; i < n; i++)
//     {
//         if (graph[node][i] == 1 && !visited[i])
//         {
//             dfs(i, visited, graph, n, component, size);
//         }
//     }
// }

// /*
// int graph[MAX_NODES][MAX_NODES]: 图的邻接矩阵表示。
// int n: 图的节点数。
// int components[MAX_NODES][MAX_NODES]: 用于存储每个连通分量的数组。
// int sizes[]: 用于存储每个连通分量的节点数。
// int *numComponents: 指向连通分量数量的指针，便于在递归中修改。
// */
// void findComponents(int graph[MAX_NODES][MAX_NODES], int n, int components[MAX_NODES][MAX_NODES], int sizes[], int *numComponents)
// {
//     bool visited[MAX_NODES] = {false};
//     *numComponents = 0;

//     for (int i = 0; i < n; i++)
//     {
//         if (!visited[i])
//         {
//             int component[MAX_NODES];
//             int size = 0;
//             dfs(i, visited, graph, n, component, &size);
//             for (int j = 0; j < size; j++)
//             {
//                 components[*numComponents][j] = component[j];
//             }
//             sizes[*numComponents] = size;
//             (*numComponents)++;
//         }
//     }
// }

// bool isAffected(int component[], int size, int S[], int sSize)
// {
//     for (int i = 0; i < size; i++)
//     {
//         for (int j = 0; j < sSize; j++)
//         {
//             if (component[i] == S[j])
//             {
//                 return true;
//             }
//         }
//     }
//     return false;
// }
// /*
// int graph[MAX_NODES][MAX_NODES]: 图的邻接矩阵表示。
// int n: 图的节点数。
// int S[]: 表示需要保护的节点集合。
// int sSize: 表示需要保护的节点集合的大小。
// int k: 表示需要替换的节点数量。
// int result[]: 用于存储需要替换的节点集合。
// */

// void minimizeImpact(int graph[MAX_NODES][MAX_NODES], int n, int S[], int sSize, int k, int result[])
// {
//     int components[MAX_NODES][MAX_NODES];
//     int sizes[MAX_NODES];
//     int numComponents;
//     findComponents(graph, n, components, sizes, &numComponents);

//     int replaceCandidates[MAX_NODES];
//     int affectedComponents[MAX_NODES];
//     int replaceCount = 0;

//     for (int i = 0; i < numComponents; i++)
//     {
//         if (isAffected(components[i], sizes[i], S, sSize))
//         {
//             for (int j = 0; j < sizes[i]; j++)
//             {
//                 replaceCandidates[replaceCount] = components[i][j];
//                 affectedComponents[replaceCount] = i; // 记录该候选节点属于哪个连通分量
//                 replaceCount++;
//             }
//         }
//     }

//     // 选择每个受影响的连通分量中的一个节点进行替换
//     int selectedComponents[MAX_NODES] = {0}; // 记录每个连通分量是否已经有节点被选择替换
//     int selectedCount = 0;
//     for (int i = 0; i < replaceCount && selectedCount < k; i++)
//     {
//         int maxIdx = -1;
//         for (int j = 0; j < replaceCount; j++)
//         {
//             if (selectedComponents[affectedComponents[j]] == 0)
//             {
//                 maxIdx = j;
//                 break;
//             }
//         }
//         if (maxIdx != -1)
//         {
//             result[selectedCount++] = replaceCandidates[maxIdx];
//             selectedComponents[affectedComponents[maxIdx]] = 1;
//         }
//     }

//     // 如果还没选满k个节点，再进行贪心选择
//     for (int i = selectedCount; i < k; i++)
//     {
//         int maxIdx = -1;
//         for (int j = 0; j < replaceCount; j++)
//         {
//             if (selectedComponents[affectedComponents[j]] == 0)
//             {
//                 maxIdx = j;
//                 break;
//             }
//         }
//         if (maxIdx != -1)
//         {
//             result[selectedCount++] = replaceCandidates[maxIdx];
//             selectedComponents[affectedComponents[maxIdx]] = 1;
//         }
//     }
// }

// int main()
// {
//     int V, E;
//     scanf("%d %d", &V, &E);

//     int graph[MAX_NODES][MAX_NODES] = {0};

//     for (int i = 0; i < E; i++)
//     {
//         int i, j, e;
//         scanf("%d %d %d", &i, &j, &e);
//         graph[i - 1][j - 1] = e; // 注意这里是基于1的索引，所以减去1
//         graph[j - 1][i - 1] = e; // 无向图
//     }

//     int S[MAX_NODES];
//     int sSize = 0;
//     char line[256];
//     scanf(" %[^\n]", line); // 读取异常服务器行
//     char *token = strtok(line, " ");
//     while (token != NULL)
//     {
//         S[sSize++] = atoi(token) - 1; // 注意这里是基于1的索引，所以减去1
//         token = strtok(NULL, " ");    // 这里应该是 NULL，继续解析下一部分
//     }

//     int k;
//     scanf("%d", &k);

//     int result[MAX_NODES];
//     minimizeImpact(graph, V, S, sSize, k, result);

//     printf("Nodes to replace: ");
//     for (int i = 0; i < k; i++)
//     {
//         printf("%d ", result[i] + 1); // 输出时加回1
//     }
//     printf("\n");

//     return 0;
// }