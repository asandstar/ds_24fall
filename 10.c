#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int countSolutions = 0; // global variable

int isSafe(int *position, int currentRow, int col, int m)
{
    for (int i = 0; i < currentRow; i++)
    {
        int otherCol = position[i];
        // check col and diagonal collapse
        if (col == otherCol || abs(col - otherCol) == abs(currentRow - i))
        {
            return 0;
        }
    }
    return 1;
}

void solveNQueen(int *position, int currentRow, int m, int n)
{
    if (currentRow == m) // place m queen
    {
        countSolutions++;
        return;
    }
    for (int col = 0; col < n; col++)
    {
        if (isSafe(position, currentRow, col, n))
        {
            position[currentRow] = col;
            solveNQueen(position, currentRow + 1, m, n);
            position[currentRow] = -1;
        }
    }
}

int totalNQueens(int n, int m)
{
    int *position = malloc(n * sizeof(int));

    memset(position, 0, n * sizeof(int));

    countSolutions = 0;
    solveNQueen(position, 0, m, n);

    free(position);
    return countSolutions;
}

int main()
{
    int n, m;
    scanf("%d %d", &n, &m);
    printf("%d", totalNQueens(n, m));
    return 0;
}