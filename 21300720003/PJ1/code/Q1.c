#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_EMAIL_LEN 256

// 判断一个字符是否是合法的邮箱字符
int is_valid_char(char c)
{
    return isalnum(c) || c == '@' || c == '.' || c == '-' || c == '_';
}

// 读取并清理黑名单
void clean_blacklist(const char *filename, char emails[][MAX_EMAIL_LEN], int *count)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Failed to open blacklist file");
        exit(EXIT_FAILURE);
    }

    char ch;
    int index = 0, email_index = 0;
    while ((ch = fgetc(file)) != EOF)
    {
        if (is_valid_char(ch))
        {
            emails[*count][email_index++] = ch;
        }
        else
        {
            if (email_index > 0)
            {
                emails[*count][email_index] = '\0';
                (*count)++;
                email_index = 0;
            }
        }
    }
    if (email_index > 0)
    {
        emails[*count][email_index] = '\0';
        (*count)++;
    }

    fclose(file);
}
void compute_lps_array(char *pattern, int M, int *lps)
{
    int length = 0;
    lps[0] = 0;

    int i = 1;
    while (i < M)
    {
        if (pattern[i] == pattern[length])
        {
            length++;
            lps[i] = length;
            i++;
        }
        else
        {
            if (length != 0)
            {
                length = lps[length - 1];
            }
            else
            {
                lps[i] = 0;
                i++;
            }
        }
    }
}

int KMP_search(char *pattern, char *text)
{
    int M = strlen(pattern);
    int N = strlen(text);

    int lps[M];
    compute_lps_array(pattern, M, lps);

    int i = 0, j = 0;
    while (i < N)
    {
        if (pattern[j] == text[i])
        {
            j++;
            i++;
        }

        if (j == M)
        {
            return 1;
            j = lps[j - 1];
        }
        else if (i < N && pattern[j] != text[i])
        {
            if (j != 0)
            {
                j = lps[j - 1];
            }
            else
            {
                i++;
            }
        }
    }
    return 0;
}

void check_emails(const char *filename, char emails[][MAX_EMAIL_LEN], int count, const char *output_filename)
{
    FILE *input_file = fopen(filename, "r");
    FILE *output_file = fopen(output_filename, "w");

    if (!input_file)
    {
        perror("Failed to open underfilt file");
        exit(EXIT_FAILURE);
    }

    if (!output_file)
    {
        perror("Failed to open output file");
        exit(EXIT_FAILURE);
    }

    char email[MAX_EMAIL_LEN];
    while (fgets(email, sizeof(email), input_file))
    {
        email[strcspn(email, "\n")] = 0; // Remove newline character

        for (int i = 0; i < count; i++)
        {
            if (KMP_search(emails[i], email))
            {
                fprintf(output_file, "%s\n", email);
                break;
            }
        }
    }

    fclose(input_file);
    fclose(output_file);
}

int main()
{
    char blacklist_emails[1000][MAX_EMAIL_LEN];
    int blacklist_count = 0;

    clean_blacklist("blacklist_1.txt", blacklist_emails, &blacklist_count);
    check_emails("underfilt_1.txt", blacklist_emails, blacklist_count, "output_1.txt");

    printf("Emails matching the blacklist have been written to output_1.txt\n");
    return 0;
}
