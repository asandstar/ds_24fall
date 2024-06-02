#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BITMAP_SIZE 8192
#define HASH_COUNT 5
#define MAX_EMAIL_LEN 256

typedef struct
{
    unsigned char bitmap[BITMAP_SIZE];
} BloomFilter;

// 简单的哈希函数
unsigned int hash1(const char *str)
{
    unsigned int hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    return hash;
}

unsigned int hash2(const char *str)
{
    unsigned int hash = 0;
    int c;
    while ((c = *str++))
        hash = c + (hash << 6) + (hash << 16) - hash;
    return hash;
}

unsigned int hash3(const char *str)
{
    unsigned int hash = 0;
    int c;
    while ((c = *str++))
        hash = (hash << 4) + c;
    return hash;
}

unsigned int hash4(const char *str)
{
    unsigned int hash = 0;
    int c;
    while ((c = *str++))
        hash = (hash << 7) + c;
    return hash;
}

unsigned int hash5(const char *str)
{
    unsigned int hash = 0;
    int c;
    while ((c = *str++))
        hash = (hash << 3) + c;
    return hash;
}

void add_to_bloom_filter(BloomFilter *filter, const char *str)
{
    unsigned int hashes[HASH_COUNT];
    hashes[0] = hash1(str) % (BITMAP_SIZE * 8);
    hashes[1] = hash2(str) % (BITMAP_SIZE * 8);
    hashes[2] = hash3(str) % (BITMAP_SIZE * 8);
    hashes[3] = hash4(str) % (BITMAP_SIZE * 8);
    hashes[4] = hash5(str) % (BITMAP_SIZE * 8);

    for (int i = 0; i < HASH_COUNT; i++)
    {
        filter->bitmap[hashes[i] / 8] |= (1 << (hashes[i] % 8));
    }
}

int check_bloom_filter(BloomFilter *filter, const char *str)
{
    unsigned int hashes[HASH_COUNT];
    hashes[0] = hash1(str) % (BITMAP_SIZE * 8);
    hashes[1] = hash2(str) % (BITMAP_SIZE * 8);
    hashes[2] = hash3(str) % (BITMAP_SIZE * 8);
    hashes[3] = hash4(str) % (BITMAP_SIZE * 8);
    hashes[4] = hash5(str) % (BITMAP_SIZE * 8);

    for (int i = 0; i < HASH_COUNT; i++)
    {
        if (!(filter->bitmap[hashes[i] / 8] & (1 << (hashes[i] % 8))))
        {
            return 0; // 一定不存在
        }
    }
    return 1; // 可能存在
}

int main()
{
    BloomFilter filter;
    memset(&filter, 0, sizeof(filter));

    // 读取黑名单并添加到布隆过滤器
    FILE *blacklist_file = fopen("blacklist_2.txt", "r");
    if (!blacklist_file)
    {
        perror("Failed to open blacklist file");
        return 1;
    }
    char email[MAX_EMAIL_LEN];
    while (fgets(email, sizeof(email), blacklist_file))
    {
        email[strcspn(email, "\n")] = 0; // 移除换行符
        add_to_bloom_filter(&filter, email);
    }
    fclose(blacklist_file);

    // 读取待检测的邮件地址并进行过滤
    FILE *underfilt_file = fopen("underfilt_2.txt", "r");
    if (!underfilt_file)
    {
        perror("Failed to open underfilt file");
        return 1;
    }
    int count = 0;
    while (fgets(email, sizeof(email), underfilt_file))
    {
        email[strcspn(email, "\n")] = 0; // 移除换行符
        if (check_bloom_filter(&filter, email))
        {
            count++;
        }
    }
    fclose(underfilt_file);

    printf("Number of emails in the blacklist: %d\n", count);
    return 0;
}
