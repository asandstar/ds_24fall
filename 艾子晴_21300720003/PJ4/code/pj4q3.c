#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_NODES 100
#define INF INT_MAX

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

int calculateServerImpact(int graph[MAX_NODES][MAX_NODES], int n, int S[], int sSize)
{
    bool visited[MAX_NODES] = {false};
    int totalImpact = 0;

    for (int i = 0; i < sSize; i++)
    {
        int node = S[i];
        if (!visited[node])
        {
            int stack[MAX_NODES], top = 0;
            stack[top++] = node;
            visited[node] = true;
            int count = 0;

            while (top > 0)
            {
                int curr = stack[--top];
                count++;
                for (int j = 0; j < n; j++)
                {
                    if (graph[curr][j] != INF && !visited[j])
                    {
                        stack[top++] = j;
                        visited[j] = true;
                    }
                }
            }

            totalImpact += count;
        }
    }

    return totalImpact;
}

int calculateNetworkImpact(int graph[MAX_NODES][MAX_NODES], int n, int N[][3], int nSize)
{
    int totalImpact = 0;

    for (int i = 0; i < nSize; i++)
    {
        int u = N[i][0];
        int v = N[i][1];
        int originalBandwidth = 50;
        int faultyBandwidth = N[i][2];

        graph[u][v] = faultyBandwidth;
        graph[v][u] = faultyBandwidth;

        bool visited[MAX_NODES] = {false};
        int minBandwidth = INF;
        int nodeCount = 0;
        dfs(u, graph, visited, n, &minBandwidth, &nodeCount);

        graph[u][v] = originalBandwidth;
        graph[v][u] = originalBandwidth;

        int bandwidthDecrease = originalBandwidth - minBandwidth;
        int impact = bandwidthDecrease * nodeCount;
        totalImpact += impact;
    }

    return totalImpact;
}

void optimizeImpact(int graph[MAX_NODES][MAX_NODES], int n, int S[], int sSize, int N[][3], int nSize, int P, int Rs, int Re, double alpha, double beta, int result[])
{
    int minImpact = INT_MAX;
    int bestS = -1, bestN = -1;

    for (int i = 0; i < sSize; i++)
    {
        int tempS[MAX_NODES];
        int tempSSize = 0;
        for (int t = 0; t < sSize; t++)
        {
            if (t != i)
            {
                tempS[tempSSize++] = S[t];
            }
        }

        int serverImpact = calculateServerImpact(graph, n, tempS, tempSSize);

        for (int j = 0; j < nSize; j++)
        {
            int tempN[MAX_NODES][3];
            int tempNSize = 0;
            for (int t = 0; t < nSize; t++)
            {
                if (t != j)
                {
                    tempN[tempNSize][0] = N[t][0];
                    tempN[tempNSize][1] = N[t][1];
                    tempN[tempNSize][2] = N[t][2];
                    tempNSize++;
                }
            }

            int networkImpact = calculateNetworkImpact(graph, n, tempN, tempNSize);

            int totalImpact = alpha * serverImpact + beta * networkImpact;

            if (totalImpact < minImpact && Rs + Re <= P)
            {
                minImpact = totalImpact;
                bestS = i;
                bestN = j;
            }
        }
    }

    result[0] = bestS;
    result[1] = bestN;
}

int main()
{
    int V, E;
    scanf("%d %d", &V, &E);

    int graph[MAX_NODES][MAX_NODES];
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

    for (int i = 0; i < E; i++)
    {
        int u, v, e;
        scanf("%d %d %d", &u, &v, &e);
        graph[u - 1][v - 1] = 50;
        graph[v - 1][u - 1] = 50;
    }

    int sSize;
    scanf("%d", &sSize);
    int S[sSize];
    for (int i = 0; i < sSize; i++)
    {
        scanf("%d", &S[i]);
        S[i]--;
    }

    int nSize;
    scanf("%d", &nSize);
    int N[nSize][3];
    for (int i = 0; i < nSize; i++)
    {
        int index;
        scanf("%d", &index);
        N[i][0] = index / 2;
        N[i][1] = (index % 2 == 0) ? index / 2 + 1 : index / 2;
        N[i][2] = 70;
    }

    int P, Rs, Re;
    double alpha, beta;
    scanf("%d %d %d", &P, &Rs, &Re);
    scanf("%lf %lf", &alpha, &beta);

    int result[2];
    optimizeImpact(graph, V, S, sSize, N, nSize, P, Rs, Re, alpha, beta, result);

    if (result[0] != -1 && result[1] != -1)
    {
        printf("Best server to replace: %d\n", S[result[0]] + 1);
        printf("Best network to replace: %d\n", result[1]);
    }
    else
    {
        printf("Not possible within the budget.\n");
    }

    return 0;
}
