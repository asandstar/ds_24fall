#include <stdio.h>
#include <stdlib.h>

#define MAXCHILDREN 100

// 1.定义节点
typedef struct Node
{
    int value;
    struct Node *children[MAXCHILDREN];
    int childCount;
} Node;
// 2.创建节点
Node *createNode(int value)
{
    Node *node = (Node *)malloc(sizeof(Node));
    node->value = value;
    node->childCount = 0;
    return node;
}
// 3.添加孩子节点
void addChild(Node *parent, Node *child)
{
    parent->children[parent->childCount++] = child;
}
// 4.根据邻接表构建输入的树
Node *createTree(int N)
{
    Node *nodes[N];
    for (int i = 0; i < N; i++)
    {
        nodes[i] = createNode(i + 1);
    }

    for (int i = 0; i < N; i++)
    {
        int childCount;
        scanf("%d", &childCount);

        for (int j = 0; j < childCount; j++)
        {
            int childIndex;
            scanf("%d", &childIndex);
            addChild(nodes[i], nodes[childIndex - 1]);
        }
    }

    return nodes[0];
}
// 5.根据路径获取节点
// 根据给定的路径从根节点开始遍历树，沿着路径依次访问子节点，直到找到目标节点或者路径不合法为止。
// 如果路径合法并且能够找到对应的节点，则返回该节点的指针
// 如果路径不合法或者无法找到对应节点，则返回 NULL
Node *getNodeByPath(Node *root, int path[], int pathLength)
{
    Node *node = root;
    for (int i = 0; i < pathLength && node;)
    {
        if (path[i] < node->childCount)
        {
            node = node->children[path[i]];
        }
        else
        {
            break;
        }
        i++;
    }
    return node;
}

// 6.从路径获取父节点（与5同，但返回目标节点的父节点）
// 路径长度小于等于1，则直接返回根节点作为父节点。
// 路径长度大于1，则沿着路径遍历树，找到目标节点的父节点并返回。如果路径不合法或者无法找到目标节点的父节点，则返回 NULL。
Node *getParentNodeByPath(Node *root, int path[], int pathLength)
{
    if (pathLength <= 1)
        return root;
    Node *node = root;
    Node *parentNode = NULL;
    for (int i = 0; i < pathLength && node;)
    {
        if (path[i] < node->childCount)
        {
            parentNode = node;
            node = node->children[path[i]];
        }
        else
        {
            break;
        }
        i++;
    }
    return parentNode;
}
//

// 7.移动子树
void moveSubtree(Node *root, int srcPath[], int srcLen, int destPath[], int destLen, int destRank)
{
    Node *srcParent = getParentNodeByPath(root, srcPath, srcLen);
    Node *srcNode = getNodeByPath(root, srcPath, srcLen);
    for (int i = 0; i < srcParent->childCount; i++)
    {
        if (srcParent->children[i] == srcNode)
        {
            for (int j = i; j < srcParent->childCount - 1; j++)
            {
                srcParent->children[j] = srcParent->children[j + 1];
            }
            srcParent->childCount--;
            break;
        }
    }
    Node *destParent = getNodeByPath(root, destPath, destLen); // 另目的位置的节点表示为移除源子树后的节点表示。
    if (destRank >= destParent->childCount)
    {
        destParent->children[destParent->childCount++] = srcNode;
    }
    else
    {
        for (int i = destParent->childCount; i > destRank; i--)
        {
            destParent->children[i] = destParent->children[i - 1];
        }
        destParent->children[destRank] = srcNode;
        destParent->childCount++;
    }
}
// 8.层次遍历输出（BFS广度优先遍历算法）
void levelOrderTraversal(Node *root)
{
    if (!root)
        return;

    Node *queue[MAXCHILDREN];
    int front = 0, rear = 0;
    queue[rear++] = root;

    while (front < rear)
    {
        Node *node = queue[front++];
        printf("%d\n", node->value);

        for (int i = 0; i < node->childCount; i++)
        {
            queue[rear++] = node->children[i];
        }
    }
}

int main()
{
    int N, M;
    scanf("%d %d", &N, &M);
    getchar();

    Node *root = createTree(N);

    for (int i = 0; i < M; i++)
    {
        int srcPath[MAXCHILDREN], srcLen;
        int destPath[MAXCHILDREN], destLen, destRank;

        scanf("%d", &srcLen);
        for (int j = 0; j < srcLen; j++)
        {
            scanf("%d", &srcPath[j]);
        }
        scanf("%d", &destLen);
        for (int j = 0; j < destLen; j++)
        {
            scanf("%d", &destPath[j]);
        }
        scanf("%d", &destRank);

        moveSubtree(root, srcPath, srcLen, destPath, destLen, destRank);
    }

    levelOrderTraversal(root);

    return 0;
}
