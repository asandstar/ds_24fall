#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_EMAIL_LEN 256
#define MAX_UNDERFIT_SIZE 1000
// https://www.geeksforgeeks.org/kmp-algorithm-for-pattern-searching/
//  函数用于计算部分匹配表（前缀函数）
// Fills lps[] for given pattern pat[0..M-1]
void computeLPSArray(const char *pat, int M, int *lps)
{
    // length of the previous longest prefix suffix
    int length = 0;
    lps[0] = 0; // lps[0] is always 0
    // the loop calculates lps[i] for i = 1 to M-1
    int i = 1;
    while (i < M)
    {
        if (pat[i] == pat[length])
        {
            length++;
            lps[i] = length;
            i++;
        }
        else // (pat[i] != pat[len])
        {
            // This is tricky. Consider the example.
            // AAACAAAA and i = 7. The idea is similar
            // to search step.
            if (length != 0)
            {
                length = lps[length - 1];
                // Also, note that we do not increment
                // i here
            }
            else // if (len == 0)
            {
                lps[i] = 0;
                i++;
            }
        }
    }
}

// KMP 算法实现
int KMPSearch(const char *pat, const char *txt)
{
    int M = strlen(pat);
    int N = strlen(txt);

    int *lps = (int *)malloc(sizeof(int) * M);
    computeLPSArray(pat, M, lps);

    int i = 0; // txt 的索引
    int j = 0; // pat 的索引
    while (i < N)
    {
        if (pat[j] == txt[i])
        {
            j++;
            i++;
        }

        if (j == M)
        {
            free(lps);
            return 1; // 找到匹配
        }
        // mismatch after j matches
        else if (i < N && pat[j] != txt[i])
        {
            // Do not match lps[0..lps[j-1]] characters,
            // they will match anyway
            if (j != 0)
                j = lps[j - 1];
            else
                i++;
        }
    }

    free(lps);
    return 0; // 没有匹配
}
// Driver code
int main()
{
    // 读取黑名单文件内容
    FILE *blacklist_file = fopen("blacklist_1.txt", "r");
    if (!blacklist_file)
    {
        perror("Failed to open blacklist_1.txt");
        return 1;
    }
    // fseek移动文件指针到文件中指定位置
    // fseek(FILE *stream,long offset,int whence);
    fseek(blacklist_file, 0, SEEK_END);
    long blacklist_file_size = ftell(blacklist_file);
    fseek(blacklist_file, 0, SEEK_SET);

    char *blacklist_content = (char *)malloc(blacklist_file_size + 1);
    fread(blacklist_content, 1, blacklist_file_size, blacklist_file);
    blacklist_content[blacklist_file_size] = '\0';
    fclose(blacklist_file);

    // 读取待检测的邮件地址文件内容
    FILE *underfit_file = fopen("underfilt_1.txt", "r");
    if (!underfit_file)
    {
        perror("Failed to open underfilt_1.txt");
        free(blacklist_content);
        return 1;
    }

    char underfit_emails[MAX_UNDERFIT_SIZE][MAX_EMAIL_LEN];
    int underfit_count = 0;
    while (fgets(underfit_emails[underfit_count], sizeof(underfit_emails[underfit_count]), underfit_file))
    {
        underfit_emails[underfit_count][strcspn(underfit_emails[underfit_count], "\n")] = 0; // 移除换行符
        underfit_count++;
    }
    fclose(underfit_file);

    // 打开输出文件
    FILE *output_file = fopen("output_1.txt", "w");
    if (!output_file)
    {
        perror("Failed to open output_1.txt");
        free(blacklist_content);
        return 1;
    }

    // 检查每个待检测的邮件地址是否在黑名单内容中
    for (int i = 0; i < underfit_count; i++)
    {
        if (KMPSearch(underfit_emails[i], blacklist_content))
        {
            fprintf(output_file, "%s\n", underfit_emails[i]);
        }
    }

    fclose(output_file);
    free(blacklist_content);

    printf("Matching emails have been written to output_1.txt\n");
    return 0;
}
