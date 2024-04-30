#include <stdio.h>
#include <stdlib.h>

typedef struct TreeNode
{
    int data;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

typedef struct QueueNode
{
    TreeNode *treenode;
    struct QueueNode *next;
} QueueNode;

typedef struct
{
    QueueNode *front, *rear;
} Queue;

void initQueue(Queue *q)
{
    q->front = q->rear = NULL;
}

void enqueue(Queue *q, TreeNode *node)
{
    QueueNode *newNode = (QueueNode *)malloc(sizeof(QueueNode));
    newNode->treenode = node;
    newNode->next = NULL;
    if (q->rear == NULL)
    {
        q->front = q->rear = newNode;
    }
    else
    {
        q->rear->next = newNode;
        q->rear = newNode;
    }
}

TreeNode *dequeue(Queue *q)
{
    if (q->front == NULL)
        return NULL;
    QueueNode *temp = q->front;
    TreeNode *node = temp->treenode;
    q->front = q->front->next;
    if (q->front == NULL)
    {
        q->rear = NULL;
    }
    free(temp);
    return node;
}

int isEmptyQueue(Queue *q)
{
    return (q->front == NULL);
}

void printLevelorder(TreeNode *root)
{
    if (root == NULL)
        return;

    Queue q;
    initQueue(&q);
    enqueue(&q, root);

    while (!isEmptyQueue(&q))
    {
        TreeNode *currentNode = dequeue(&q);
        printf("%d ", currentNode->data);
        if (currentNode->left != NULL)
            enqueue(&q, currentNode->left);
        if (currentNode->right != NULL)
            enqueue(&q, currentNode->right);
    }
    printf("\n");
}

TreeNode *buildTreeFromPreAndPost(int *pre, int *post, int preStart, int preEnd, int postStart, int postEnd)
{
    if (preStart > preEnd)
        return NULL;

    TreeNode *root = (TreeNode *)malloc(sizeof(TreeNode));
    root->data = pre[preStart];
    root->left = root->right = NULL;

    if (preStart == preEnd)
        return root;

    int nextRootVal = pre[preStart + 1];
    int postIndex = postStart;
    while (post[postIndex] != nextRootVal)
        postIndex++;

    int lenLeft = postIndex - postStart + 1;
    root->left = buildTreeFromPreAndPost(pre, post, preStart + 1, preStart + lenLeft, postStart, postIndex);
    root->right = buildTreeFromPreAndPost(pre, post, preStart + lenLeft + 1, preEnd, postIndex + 1, postEnd - 1);

    return root;
}

TreeNode *buildTreeFromPreAndIn(int *pre, int *in, int preStart, int preEnd, int inStart, int inEnd)
{
    if (preStart > preEnd)
        return NULL;

    TreeNode *root = (TreeNode *)malloc(sizeof(TreeNode));
    root->data = pre[preStart];
    root->left = root->right = NULL;

    int inRootIndex = inStart;
    while (in[inRootIndex] != pre[preStart])
        inRootIndex++;

    int leftTreeSize = inRootIndex - inStart;

    root->left = buildTreeFromPreAndIn(pre, in, preStart + 1, preStart + leftTreeSize, inStart, inRootIndex - 1);
    root->right = buildTreeFromPreAndIn(pre, in, preStart + leftTreeSize + 1, preEnd, inRootIndex + 1, inEnd);

    return root;
}

TreeNode *buildTreeFromInAndPost(int *in, int *post, int inStart, int inEnd, int postStart, int postEnd)
{
    if (inStart > inEnd)
        return NULL;

    TreeNode *root = (TreeNode *)malloc(sizeof(TreeNode));
    root->data = post[postEnd];
    root->left = root->right = NULL;

    int inRootIndex = inStart;
    while (in[inRootIndex] != post[postEnd])
        inRootIndex++;

    int leftTreeSize = inRootIndex - inStart;

    root->left = buildTreeFromInAndPost(in, post, inStart, inRootIndex - 1, postStart, postStart + leftTreeSize - 1);
    root->right = buildTreeFromInAndPost(in, post, inRootIndex + 1, inEnd, postStart + leftTreeSize, postEnd - 1);

    return root;
}

void printInorder(TreeNode *node)
{
    if (node == NULL)
        return;
    printInorder(node->left);
    printf("%d ", node->data);
    printInorder(node->right);
}

void printPostorder(TreeNode *node)
{
    if (node == NULL)
        return;
    printPostorder(node->left);
    printPostorder(node->right);
    printf("%d ", node->data);
}

void printPreorder(TreeNode *node)
{
    if (node == NULL)
        return;
    printf("%d ", node->data);
    printPreorder(node->left);
    printPreorder(node->right);
}

int main()
{
    int n, k;
    scanf("%d %d", &n, &k);

    int *preorder = (int *)malloc(n * sizeof(int));
    int *inorder = (int *)malloc(n * sizeof(int));
    int *postorder = (int *)malloc(n * sizeof(int));
    TreeNode *root = NULL;

    // 0 // 第二、三行为该二叉树的先序、后序遍历序列，输出其中序遍历序列，输入序列保证该二叉树为真二叉树

    // 1 // 第二、三行为该二叉树的先序、中序遍历序列，输出其后序遍历序列

    // 2 // 第二、三行为该二叉树的中序、后序遍历序列，输出其先序遍历序列

    // 3 // 第二、三行为该二叉树的先序、后序遍历序列，输出其层次遍历序列

    // if (k == 0 || k == 1 || k == 3)
    // {
    //     for (int i = 0; i < n; i++)
    //     {
    //         scanf("%d", &preorder[i]);
    //     }
    // }
    // if (k == 0 || k == 3)
    // {
    //     for (int i = 0; i < n; i++)
    //     {
    //         scanf("%d", &postorder[i]);
    //     }
    // }
    // if (k == 1 || k == 2)
    // {
    //     for (int i = 0; i < n; i++)
    //     {
    //         scanf("%d", &inorder[i]);
    //     }
    // }

    switch (k)
    {
    case 0:
        for (int i = 0; i < n; i++)
        {
            scanf("%d", &preorder[i]);
        }
        for (int i = 0; i < n; i++)
        {
            scanf("%d", &postorder[i]);
        }
        root = buildTreeFromPreAndPost(preorder, postorder, 0, n - 1, 0, n - 1);
        printInorder(root);
        break;
    case 1:
        for (int i = 0; i < n; i++)
        {
            scanf("%d", &preorder[i]);
        }
        for (int i = 0; i < n; i++)
        {
            scanf("%d", &inorder[i]);
        }
        root = buildTreeFromPreAndIn(preorder, inorder, 0, n - 1, 0, n - 1);
        printPostorder(root);
        break;
    case 2:
        for (int i = 0; i < n; i++)
        {
            scanf("%d", &inorder[i]);
        }
        for (int i = 0; i < n; i++)
        {
            scanf("%d", &postorder[i]);
        }
        root = buildTreeFromInAndPost(inorder, postorder, 0, n - 1, 0, n - 1);
        // printf("--%d\n", root);
        printPreorder(root);
        break;
    case 3:
        for (int i = 0; i < n; i++)
        {
            scanf("%d", &preorder[i]);
        }
        for (int i = 0; i < n; i++)
        {
            scanf("%d", &postorder[i]);
        }
        root = buildTreeFromPreAndPost(preorder, postorder, 0, n - 1, 0, n - 1);
        // printf("%d", root);
        printLevelorder(root);
        break;
    }

    free(preorder);
    free(inorder);
    free(postorder);

    return 0;
}
