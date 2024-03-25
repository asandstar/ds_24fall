// https://c.biancheng.net/view/3346.html
/*已知 n 个人（分别用编号 1，2，3，…，n 表示）围坐在一张圆桌周围，
从编号为 k 的人开始顺时针报数，数到 m 的那个人出列；
他的下一个人又从 1 开始，还是顺时针开始报数，
数到 m 的那个人又出列；依次重复下去，直到圆桌上剩余一个人。*/

#include <stdio.h>
#include <stdlib.h>

typedef struct List
{
    int data;
    struct List *next;
} list;

list *initLink(int n)
{
    list *head = (list *)malloc(sizeof(list));
    head->data = 1;
    head->next = NULL;
    list *cycle = head;
    for (int i = 2; i <= n; i++)
    {
        list *body = (list *)malloc(sizeof(list));
        body->data = i;
        body->next = NULL;

        cycle->next = body;
        cycle = cycle->next;
    }
    cycle->next = head;
    return head;
}

void outList(list *head, int m)
{
    list *tail = head;
    while (tail->next != head)
    {
        tail = tail->next;
    }
    list *p = head;
    while (p->next != p)
    {
        for (int i = 1; i < m; i++)
        {
            tail = p;
            p = p->next;
        }
        tail->next = p->next;
        printf("%d ", p->data);
        free(p);
        p = tail->next;
    }
    printf("%d ", p->data);
    free(p);
}

int main()
{
    int n;
    scanf("%d", &n);
    int m;
    scanf("%d", &m);
    list *head = initLink(n);
    outList(head, m);
    return 0;
}