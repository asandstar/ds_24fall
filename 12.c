#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    int id;
    int childCount;
    struct Node **children;
} Node;

Node *createNode(int id)
{
    Node *newNode = (Node *)calloc(1, sizeof(Node));
    newNode->id = id;
    newNode->childCount = 0;
    newNode->children = NULL;
    return newNode;
}

void addChild(Node *parent, Node *child)
{
    parent->children = realloc(parent->children, sizeof(Node *) * (parent->childCount + 1));
    parent->children[parent->childCount] = child;
    parent->childCount++;
}

void removeChild(Node *parent, Node *child)
{
    int found = -1;
    for (int i = 0; i < parent->childCount; i++)
    {
        if (parent->children[i] == child)
        {
            found = i;
            break;
        }
    }
    if (found != -1)
    {
        for (int i = found; i < parent->childCount - 1; i++)
        {
            parent->children[i] = parent->children[i + 1];
        }
        parent->childCount--;
        parent->children = realloc(parent->children, sizeof(Node *) * parent->childCount);
    }
}

void moveSubtree(Node *source, Node *destination, int rank)
{
    if (rank > destination->childCount)
    {
        rank = destination->childCount;
    }
    Node **newChildren = malloc(sizeof(Node *) * (destination->childCount + 1));
    int index = 0;
    for (int i = 0; i <= destination->childCount; i++)
    {
        if (i == rank)
        {
            newChildren[i] = source;
        }
        else
        {
            newChildren[i] = destination->children[index++];
        }
    }
    free(destination->children);
    destination->children = newChildren;
    destination->childCount++;
}

void levelOrderTraversal(Node *root)
{
    Node **queue = malloc(sizeof(Node *) * 100); // Assuming a max of 100 nodes in the queue
    int front = 0, rear = 0;
    queue[rear++] = root;

    while (front < rear)
    {
        Node *current = queue[front++];
        printf("%d\n", current->id);
        for (int i = 0; i < current->childCount; i++)
        {
            queue[rear++] = current->children[i];
        }
    }
    free(queue);
}

int main()
{
    int N, M;
    scanf("%d %d", &N, &M);
    Node *nodes[N + 1];

    for (int i = 1; i <= N; i++)
    {
        nodes[i] = createNode(i);
    }

    for (int i = 1; i <= N; i++)
    {
        int childCount;
        scanf("%d", &childCount);
        for (int j = 0; j < childCount; j++)
        {
            int childId;
            scanf("%d", &childId);
            addChild(nodes[i], nodes[childId]);
        }
    }

    for (int i = 0; i < M; i++)
    {
        int src, dst, rank;
        scanf("%d %d %d", &src, &dst, &rank);
        Node *parent = NULL;
        // Find parent of source to remove it
        for (int j = 1; j <= N; j++)
        {
            for (int k = 0; k < nodes[j]->childCount; k++)
            {
                if (nodes[j]->children[k] == nodes[src])
                {
                    parent = nodes[j];
                    break;
                }
            }
            if (parent)
            {
                removeChild(parent, nodes[src]);
                break;
            }
        }
        moveSubtree(nodes[src], nodes[dst], rank);
    }

    levelOrderTraversal(nodes[1]);
    return 0;
}
