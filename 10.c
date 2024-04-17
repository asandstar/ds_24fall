// // #include <stdio.h>
// // #include <string.h>
// // #include <stdlib.h>

// // int countSolutions = 0; // global variable

// // int isSafe(int *position, int currentRow, int col, int m)
// // {
// //     for (int i = 0; i < currentRow; i++)
// //     {
// //         int otherCol = position[i];
// //         // check col and diagonal collapse
// //         if (col == otherCol || abs(col - otherCol) == abs(currentRow - i))
// //         {
// //             return 0;
// //         }
// //     }
// //     return 1;
// // }

// // void solveNQueen(int *position, int currentRow, int m, int n)
// // {
// //     if (currentRow == m) // place m queen
// //     {
// //         countSolutions++;
// //         return;
// //     }
// //     for (int col = 0; col < n; col++)
// //     {
// //         if (isSafe(position, currentRow, col, n))
// //         {
// //             position[currentRow] = col;
// //             solveNQueen(position, currentRow + 1, m, n);
// //             position[currentRow] = -1;
// //         }
// //     }
// // }

// // int totalNQueens(int n, int m)
// // {
// //     int *position = malloc(n * sizeof(int));

// //     memset(position, 0, n * sizeof(int));

// //     countSolutions = 0;
// //     solveNQueen(position, 0, m, n);

// //     free(position);
// //     return countSolutions;
// // }

// // int main()
// // {
// //     int n, m;
// //     scanf("%d %d", &n, &m);
// //     printf("%d", totalNQueens(n, m));
// //     return 0;
// // }

// // // #include <stdio.h>
// // // #include <stdlib.h>

// // // int ans = 0; // 全局变量，用于计数解决方案的数量

// // // // 这些数组将被用于标记是否安全放置皇后
// // // int *on_path;
// // // int *diag1;
// // // int *diag2;

// // // void dfs(int n, int m, int r)
// // // {
// // //     if (r == m)
// // //     { // 注意这里改为m，意味着只放置m个皇后
// // //         ans++;
// // //         return;
// // //     }
// // //     for (int c = 0; c < n; ++c)
// // //     {
// // //         int rc = r - c + n - 1;
// // //         if (!on_path[c] && !diag1[r + c] && !diag2[rc])
// // //         {
// // //             on_path[c] = diag1[r + c] = diag2[rc] = 1; // 放置皇后
// // //             dfs(n, m, r + 1);
// // //             on_path[c] = diag1[r + c] = diag2[rc] = 0; // 恢复现场
// // //         }
// // //     }
// // // }

// // // int totalNQueens(int n, int m)
// // // {
// // //     // 动态分配内存
// // //     on_path = (int *)calloc(n, sizeof(int));
// // //     diag1 = (int *)calloc(n * 2 - 1, sizeof(int));
// // //     diag2 = (int *)calloc(n * 2 - 1, sizeof(int));

// // //     ans = 0;      // 初始化解决方案的数量
// // //     dfs(n, m, 0); // 从第0行开始深度优先搜索

// // //     // 释放内存
// // //     free(on_path);
// // //     free(diag1);
// // //     free(diag2);

// // //     return ans; // 返回解决方案的数量
// // // }

// // // int main()
// // // {
// // //     int n, m;
// // //     printf("Enter the size of the board N and the number of queens M:\n");
// // //     scanf("%d %d", &n, &m);

// // //     if (m > n)
// // //     {
// // //         printf("Number of queens cannot be greater than the size of the board.\n");
// // //         return 0;
// // //     }

// // //     printf("There are %d solutions to place %d queens on a %d x %d board.\n", totalNQueens(n, m), m, n, n);
// // //     return 0;
// // // }
// #include <stdio.h>
// #include <stdlib.h>

// int ans = 0; // 全局变量，用于计数解决方案的数量

// // 这些数组将被用于标记是否安全放置皇后
// int *on_path;
// int *diag1;
// int *diag2;

// void dfs(int n, int m, int r)
// {
//     if (r == m)
//     { // 注意这里改为m，意味着只放置m个皇后
//         ans++;
//         return;
//     }
//     for (int c = 0; c < n; ++c)
//     {
//         int rc = r - c + n - 1;
//         if (!on_path[c] && !diag1[r + c] && !diag2[rc])
//         {
//             on_path[c] = diag1[r + c] = diag2[rc] = 1; // 放置皇后
//             dfs(n, m, r + 1);
//             on_path[c] = diag1[r + c] = diag2[rc] = 0; // 恢复现场
//         }
//     }
// }

// int totalNQueens(int n, int m)
// {
//     // 动态分配内存
//     on_path = (int *)calloc(n, sizeof(int));
//     diag1 = (int *)calloc(n * 2 - 1, sizeof(int));
//     diag2 = (int *)calloc(n * 2 - 1, sizeof(int));

//     ans = 0;      // 初始化解决方案的数量
//     dfs(n, m, 0); // 从第0行开始深度优先搜索

//     // 释放内存
//     free(on_path);
//     free(diag1);
//     free(diag2);

//     return ans; // 返回解决方案的数量
// }

// int main()
// {
//     int n, m;
//     printf("Enter the size of the board N and the number of queens M:\n");
//     scanf("%d %d", &n, &m);

//     if (m > n)
//     {
//         printf("Number of queens cannot be greater than the size of the board.\n");
//         return 0;
//     }

//     printf("There are %d solutions to place %d queens on a %d x %d board.\n", totalNQueens(n, m), m, n, n);
//     return 0;
// }

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int ans = 0; // 解的数量

// 数组大小根据问题规模设定
int *on_path; // 表示列的冲突
int *diag1;   // 正对角线
int *diag2;   // 反对角线

void dfs(int n, int m, int k, int row)
{
    if (k == 0)
    {
        ans++;
        return;
    }
    if (row == m)
        return; // 所有行都尝试完，结束递归

    for (int col = 0; col < n; col++)
    {
        int d1 = row + col;         // 计算正对角线索引
        int d2 = row - col + n - 1; // 计算反对角线索引
        if (!on_path[col] && !diag1[d1] && !diag2[d2])
        {
            on_path[col] = diag1[d1] = diag2[d2] = 1;
            dfs(n, m, k - 1, row + 1);
            on_path[col] = diag1[d1] = diag2[d2] = 0;
        }
    }
    // 尝试在当前行不放置皇后，直接跳到下一行
    dfs(n, m, k, row + 1);
}

int totalNQueens(int n, int m, int k)
{
    // 根据棋盘和皇后的数量调整数组大小
    on_path = (int *)calloc(n, sizeof(int));
    diag1 = (int *)calloc(m + n - 1, sizeof(int));
    diag2 = (int *)calloc(m + n - 1, sizeof(int));

    ans = 0;
    dfs(n, m, k, 0);

    free(on_path);
    free(diag1);
    free(diag2);
    return ans;
}

int main()
{
    int m, n, k;
    printf("Enter the dimensions of the board (MxN) and the number of queens K:\n");
    scanf("%d %d %d", &m, &n, &k);

    printf("There are %d solutions to place %d queens on a %d x %d board.\n",
           totalNQueens(n, m, k), k, m, n);
    return 0;
}
