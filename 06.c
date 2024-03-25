#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
typedef struct qnode
{
    int data;
    struct qnode *next;
} DataNode;

typedef struct
{
    DataNode *front;
    DataNode *rear;
} LinkQuNode;

void InitQueue(LinkQuNode *q)
{
    q = (LinkQuNode *)malloc(sizeof(LinkQuNode));
    q->front = q->rear = NULL;
}

void DestroyQueue(LinkQuNode *q)
{
    DataNode *p = q->front, *r;
    while (p != NULL)
    {
        r = p->next;
        free(p);
        p = r;
    }
}
bool QueueEmpty(LinkQuNode *q)
{
    return (q->rear == NULL);
}

void enQueue(LinkQuNode *q, int e)
{
    DataNode *p = (DataNode *)malloc(sizeof(DataNode));
    p->data = e;
    p->next = NULL;
    if (q->rear == NULL)
        q->front = q->rear = p;
    else
    {
        q->rear->next = p;
        q->rear = p;
    }
}

bool deQueue(LinkQuNode *q, int e)
{
    if (q->rear == NULL)
        return false;
    DataNode *t = q->front;
    e = t->data;
    if (q->front == q->rear)
        q->front = q->rear = NULL;
    else
        q->front = q->front->next;

    free(t);
    return true;
}

int main()
{
    int n, s;
    float t;
    LinkQuNode num;
    InitQueue(&num);

    scanf("%d", &n);
    scanf("%d", &s);

    for (int i = 0; i < 4; ++i)
    {
        int number;
        scanf("%d", &number);
        enQueue(&num, number);
    }

    return 0;
}

/**
 *
 *         // scanf("%f", &t);
    // // test
    // printf("%d", n);
    // printf("\n");
    // printf("%d", s);
    // printf("\n");
    // // for (int i = 0; i < n; i++)
    // // {
    // //     printf("%d", d[i]);
    // // }
    // printf("\n");
    // printf("%d", t);
*/