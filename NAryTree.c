// 多叉树的建立、层次遍历、深度遍历
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define M 100 + 1 // 宏定义，定义最大名字字母长度

// 定义多叉树的节点结构体
typedef struct node_t
{
    char *name;               // 节点名
    int n_children;           // 子节点个数
    int level;                // 记录该节点在多叉树中的层数
    struct node_t **children; // 指向其自身的子节点，children一个数组，该数组中的元素时node_t*指针
} NODE;                       // 对结构体重命名

// 实现一个栈，用于后续操作
typedef struct stack_t
{
    NODE **array; // array是个数组，其元素为NODE*型指针
    int index;    // 指示栈顶元素
    int size;     // 栈的大小
} STACK;          // 重命名

// 实现一个队列，用于后续操作
typedef struct queue_t
{
    NODE **array; // array是个数组，其内部元素为NODE*型指针
    int head;     // 队列的头
    int tail;     // 队列的尾
    int num;      // 队列中元素的个数
    int size;     // 队列的大小
} QUEUE;

// 这里的栈和队列，都是用动态数组实现的，另一种实现方式是用链表

// 内存分配函数
void *util_malloc(int size)
{
    void *ptr = malloc(size);

    if (ptr == NULL) // 如果分配失败，则终止程序
    {
        printf("Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }

    // 分配成功，则返回
    return ptr;
}

// 字符串赋值函数
// 对strdup函数的封装，strdup函数直接进行字符串赋值，不用对被赋值指针分配空间
// 比strcpy用起来方便，但其不是标准库里面的函数
// 用strdup函数赋值的指针，在最后也是需要free掉的
char *util_strdup(char *src)
{
    char *dst = strdup(src);

    if (dst == NULL) // 如果赋值失败，则终止程序
    {
        printf("Memroy allocation error!\n");
        exit(EXIT_FAILURE);
    }

    // 赋值成功，返回
    return dst;
}

// 对fopen函数封装
FILE *util_fopen(char *name, char *access)
{
    FILE *fp = fopen(name, access);

    if (fp == NULL) // 如果打开文件失败，终止程序
    {
        printf("Error opening file %s!\n", name);
        exit(EXIT_FAILURE);
    }

    // 打开成功，返回
    return fp;
}

// 实现一些栈操作

// 栈的初始化
STACK *STACKinit(int size) // 初始化栈大小为size
{
    STACK *sp;

    sp = (STACK *)util_malloc(sizeof(STACK));
    sp->size = size;
    sp->index = 0;
    sp->array = (NODE **)util_malloc(size * sizeof(NODE *));

    return sp;
}

// 检测栈是否为空
// 如果为空返回1，否则返回0
int STACKempty(STACK *sp)
{
    if (sp == NULL || sp->index <= 0) // 空
    {
        return 1;
    }
    return 0;
}

// 压栈操作
int STACKpush(STACK *sp, NODE *data)
{
    if (sp == NULL || sp->index >= sp->size) // sp没有被初始化，或者已满
    {
        return 0; // 压栈失败
    }

    sp->array[sp->index++] = data; // 压栈
    return 1;
}

// 弹栈操作
int STACKpop(STACK *sp, NODE **data_ptr)
{
    if (sp == NULL || sp->index <= 0) // sp为初始化，或者为空没有元素
    {
        return 0;
    }

    *data_ptr = sp->array[--sp->index]; // 弹栈
    return 1;
}

// 将栈消毁
void STACKdestroy(STACK *sp)
{
    free(sp->array);
    free(sp);
}
// 以上是栈的操作

// 实现队列的操作
QUEUE *QUEUEinit(int size)
{
    QUEUE *qp;

    qp = (QUEUE *)util_malloc(sizeof(QUEUE));
    qp->size = size;
    qp->head = qp->tail = qp->num = 0;
    qp->array = (NODE **)util_malloc(size * sizeof(NODE *));

    return qp;
}

// 入队列
int QUEUEenqueue(QUEUE *qp, NODE *data)
{
    if (qp == NULL || qp->num >= qp->size) // qp未初始化或已满
    {
        return 0; // 入队失败
    }

    qp->array[qp->tail] = data;             // 入队，tail一直指向最后一个元素的下一个位置
    qp->tail = (qp->tail + 1) % (qp->size); // 循环队列
    ++qp->num;
    return 1;
}

// 出队列
int QUEUEdequeue(QUEUE *qp, NODE **data_ptr)
{
    if (qp == NULL || qp->num <= 0) // qp未初始化或队列内无元素
    {
        return 0;
    }

    *data_ptr = qp->array[qp->head];        // 出队
    qp->head = (qp->head + 1) % (qp->size); // 循环队列
    --qp->num;

    return 1;
}

// 检测队列是否为空
int QUEUEempty(QUEUE *qp)
{
    if (qp == NULL || qp->num <= 0)
    {
        return 1;
    }

    return 0;
}

// 销毁队列
void QUEUEdestroy(QUEUE *qp)
{
    free(qp->array);
    free(qp);
}
// 以上是队列的有关操作实现

// 生成多叉树节点
NODE *create_node()
{
    NODE *q;

    q = (NODE *)util_malloc(sizeof(NODE));
    q->n_children = 0;
    q->level = -1;
    q->children = NULL;

    return q;
}

// 按节点名字查找
NODE *search_node_r(char name[M], NODE *head)
{
    NODE *temp = NULL;
    int i = 0;

    if (head != NULL)
    {
        if (strcmp(name, head->name) == 0) // 如果名字匹配
        {
            temp = head;
        }
        else // 如果不匹配，则查找其子节点
        {
            for (i = 0; i < head->n_children && temp == NULL /*如果temp不为空，则结束查找*/; ++i)
            {
                temp = search_node_r(name, head->children[i]); // 递归查找子节点
            }
        }
    }

    return temp; // 将查找到的节点指针返回，也有可能没有找到，此时temp为NULL
}

// 从文件中读取多叉树数据，并建立多叉树
void read_file(NODE **head, char *filename)
{
    NODE *temp = NULL;
    int i = 0, n = 0;
    char name[M], child[M];
    FILE *fp;

    fp = util_fopen(filename, "r"); // 打开文件

    while (fscanf(fp, "%s %d", name, &n) != EOF) // 先读取节点名字和当前节点的子节点个数
    {
        if (*head == NULL) // 若为空
        {
            temp = *head = create_node();   // 生成一个新节点
            temp->name = util_strdup(name); // 赋名
        }
        else
        {
            temp = search_node_r(name, *head); // 根据name找到节点
            // 这里默认数据文件是正确的，一定可以找到与name匹配的节点
            // 如果不匹配，那么应该忽略本行数据
        }
        // 找到节点后，对子节点进行处理
        temp->n_children = n;
        temp->children = (NODE **)malloc(n * sizeof(NODE *));
        if (temp->children == NULL) // 分配内存失败
        {
            fprintf(stderr, "Dynamic allocation error!\n");
            exit(EXIT_FAILURE);
        }

        // 如果分配成功，则读取后面的子节点，并保存
        for (i = 0; i < n; ++i)
        {
            fscanf(fp, "%s", child);                      // 读取子节点
            temp->children[i] = create_node();            // 生成子节点
            temp->children[i]->name = util_strdup(child); // 读子节点赋名
        }
    }

    // 读取完毕，关闭文件
    fclose(fp);
}

// 实现函数1
// 将多叉树中的节点，按照深度进行输出
// 实质上实现的是层次优先遍历
void f1(NODE *head)
{
    NODE *p = NULL;
    QUEUE *q = NULL; // 定义一个队列
    STACK *s = NULL; // 定义一个栈
    int i = 0;

    q = QUEUEinit(100); // 将队列初始化大小为100
    s = STACKinit(100); // 将栈初始化大小为100

    head->level = 0; // 根节点的深度为0

    // 将根节点入队列
    QUEUEenqueue(q, head);

    // 对多叉树中的节点的深度值level进行赋值
    // 采用层次优先遍历方法，借助于队列
    while (QUEUEempty(q) == 0) // 如果队列q不为空
    {
        QUEUEdequeue(q, &p); // 出队列
        for (i = 0; i < p->n_children; ++i)
        {
            p->children[i]->level = p->level + 1; // 对子节点深度进行赋值：父节点深度加1
            QUEUEenqueue(q, p->children[i]);      // 将子节点入队列
        }
        STACKpush(s, p); // 将p入栈
    }

    while (STACKempty(s) == 0) // 不为空
    {
        STACKpop(s, &p); // 弹栈
        fprintf(stdout, "   %d %s\n", p->level, p->name);
    }

    QUEUEdestroy(q); // 消毁队列
    STACKdestroy(s); // 消毁栈
}

// 实现函数2
// 找到从根节点到叶子节点路径上节点名字字母个数最大的路径
// 实质上实现的是深度优先遍历
void f2(NODE *head, char *str, char **strBest, int level)
{
    int i = 0;
    char *tmp = NULL;

    if (head == NULL)
    {
        return;
    }

    tmp = (char *)util_malloc((strlen(str) + strlen(head->name) + 1 /*原程序中未加1*/) * sizeof(char));
    sprintf(tmp, "%s%s", str, head->name);

    if (head->n_children == 0)
    {
        if (*strBest == NULL || strlen(tmp) > strlen(*strBest))
        {
            free(*strBest);
            *strBest = util_strdup(tmp);
        }
    }

    for (i = 0; i < head->n_children; ++i)
    {
        f2(head->children[i], tmp, strBest, level + 1);
    }

    free(tmp);
}

// 消毁树
void free_tree_r(NODE *head)
{
    int i = 0;
    if (head == NULL)
    {
        return;
    }

    for (i = 0; i < head->n_children; ++i)
    {
        free_tree_r(head->children[i]);
    }

    free(head->name);
    // free(head->children); // 消毁子节点指针数组
    free(head);
}

int main(int argc, char *argv[])
{
    NODE *head = NULL;
    char *strBest = NULL;

    if (argc != 2)
    {
        fprintf(stderr, "Missing parameters!\n");
        exit(EXIT_FAILURE);
    }

    read_file(&head, argv[1]);

    fprintf(stdout, "f1:\n");
    f1(head);
    f2(head, "", &strBest, 0);
    fprintf(stdout, "f2:\n   %s\n", strBest);

    free_tree_r(head);

    return EXIT_SUCCESS;
}