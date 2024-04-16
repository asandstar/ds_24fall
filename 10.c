#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Function to check if a queen can be placed on board[row][col]
bool isSafe(int row, int col, int N, int *columns, int *majorDiag, int *minorDiag)
{
    int d1 = row - col + N;
    int d2 = row + col;
    return !columns[col] && !majorDiag[d1] && !minorDiag[d2];
}

// Recursive function to place queens
int placeQueens(int row, int N, int M, int *columns, int *majorDiag, int *minorDiag, int queensPlaced)
{
    if (queensPlaced == M)
    {
        return 1; // Found a valid placement
    }
    if (row == N)
        return 0; // Reached beyond the last row without placing all queens

    int solutions = 0;
    for (int col = 0; col < N; col++)
    {
        if (isSafe(row, col, N, columns, majorDiag, minorDiag))
        {
            columns[col] = majorDiag[row - col + N] = minorDiag[row + col] = 1; // Place queen
            solutions += placeQueens(row + 1, N, M, columns, majorDiag, minorDiag, queensPlaced + 1);
            columns[col] = majorDiag[row - col + N] = minorDiag[row + col] = 0; // Remove queen (backtrack)
        }
    }
    return solutions;
}

// Main function to drive the program
int main()
{
    int N, M;
    printf("Enter the size of the chess board N: ");
    scanf("%d", &N);
    printf("Enter the number of queens M: ");
    scanf("%d", &M);

    if (M > N)
    {
        printf("Number of queens M cannot be greater than N.\n");
        return 1;
    }

    int *columns = calloc(N, sizeof(int));
    int *majorDiag = calloc(2 * N, sizeof(int));
    int *minorDiag = calloc(2 * N, sizeof(int));

    if (!columns || !majorDiag || !minorDiag)
    {
        printf("Failed to allocate memory.\n");
        free(columns);
        free(majorDiag);
        free(minorDiag);
        return 1;
    }

    int totalSolutions = placeQueens(0, N, M, columns, majorDiag, minorDiag, 0);
    printf("Total solutions for %d queens on %d x %d board: %d\n", M, N, totalSolutions);

    free(columns);
    free(majorDiag);
    free(minorDiag);
    return 0;
}
