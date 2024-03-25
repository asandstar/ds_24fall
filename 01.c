#include <stdio.h>
#include <stdlib.h> // 包含 malloc 函数的头文件

// 定义链表节点结构体
struct ListNode
{
  int val;
  struct ListNode *next;
};

// 反转链表函数
struct ListNode *reverseList(struct ListNode *head)
{
  struct ListNode *pre = NULL;
  struct ListNode *cur = head;

  while (cur != NULL)
  {
    struct ListNode *nex = cur->next;
    cur->next = pre;
    pre = cur;
    cur = nex;
  }
  return pre;
}

// 主函数
int main()
{
  struct ListNode *head = NULL; // 链表头指针
  int num;                      // 用于存储输入的整数
  int inputCount = 0;           // 用于统计输入的数字个数
  int *inputArray = NULL;       // 动态分配数组用于存储输入的数字

  // 动态分配数组
  inputArray = (int *)malloc(sizeof(int));

  // 读取整数序列，构建链表
  while (scanf("%d", &num) != EOF)
  {
    // 创建新节点
    struct ListNode *newNode = (struct ListNode *)malloc(sizeof(struct ListNode));
    newNode->val = num;
    newNode->next = NULL;

    // 将新节点插入链表尾部
    if (head == NULL)
    {
      head = newNode; // 如果链表为空，将新节点设为头节点
    }
    else
    {
      struct ListNode *temp = head;
      while (temp->next != NULL)
      {
        temp = temp->next;
      }
      temp->next = newNode; // 将新节点插入链表尾部
    }
    // 将输入的数字存储在数组中
    inputArray[inputCount++] = num;
    // 动态调整数组大小
    inputArray = (int *)realloc(inputArray, (inputCount + 1) * sizeof(int));
  }

  // 输出输入的数字（含空格）
  for (int i = 0; i < inputCount; i++)
  {
    printf("%d ", inputArray[i]);
  }
  printf("\n");

  // 反转链表
  struct ListNode *reversedHead = reverseList(head);

  // 输出反转后的链表
  while (reversedHead != NULL)
  {
    printf("%d ", reversedHead->val);
    reversedHead = reversedHead->next;
  }
  printf("\n");

  return 0;
}
