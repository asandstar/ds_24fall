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

// MurmurHash3 hash functions
unsigned int MurmurHash3_x86_32(const char *key, int len, unsigned int seed)
{
    const unsigned int c1 = 0xcc9e2d51;
    const unsigned int c2 = 0x1b873593;
    const int r1 = 15;
    const int r2 = 13;
    const unsigned int m = 5;
    const unsigned int n = 0xe6546b64;

    unsigned int hash = seed;

    const int nblocks = len / 4;
    const unsigned int *blocks = (const unsigned int *)(key);
    int i;
    for (i = 0; i < nblocks; i++)
    {
        unsigned int k = blocks[i];

        k *= c1;
        k = (k << r1) | (k >> (32 - r1));
        k *= c2;

        hash ^= k;
        hash = (hash << r2) | (hash >> (32 - r2));
        hash = hash * m + n;
    }

    const unsigned char *tail = (const unsigned char *)(key + nblocks * 4);
    unsigned int k1 = 0;

    switch (len & 3)
    {
    case 3:
        k1 ^= tail[2] << 16;
    case 2:
        k1 ^= tail[1] << 8;
    case 1:
        k1 ^= tail[0];
        k1 *= c1;
        k1 = (k1 << r1) | (k1 >> (32 - r1));
        k1 *= c2;
        hash ^= k1;
    }

    hash ^= len;
    hash ^= (hash >> 16);
    hash *= 0x85ebca6b;
    hash ^= (hash >> 13);
    hash *= 0xc2b2ae35;
    hash ^= (hash >> 16);

    return hash;
}

// DJB2 hash function
unsigned int djb2(const char *str)
{
    unsigned int hash = 5381;
    int c;
    while ((c = *str++))
    {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash;
}

void add_to_bloom_filter(BloomFilter *filter, const char *str)
{
    unsigned int hashes[HASH_COUNT];
    int len = strlen(str);
    hashes[0] = MurmurHash3_x86_32(str, len, 1) % (BITMAP_SIZE * 8);
    hashes[1] = MurmurHash3_x86_32(str, len, 2) % (BITMAP_SIZE * 8);
    hashes[2] = MurmurHash3_x86_32(str, len, 3) % (BITMAP_SIZE * 8);
    hashes[3] = MurmurHash3_x86_32(str, len, 4) % (BITMAP_SIZE * 8);
    hashes[4] = djb2(str) % (BITMAP_SIZE * 8);

    for (int i = 0; i < HASH_COUNT; i++)
    {
        filter->bitmap[hashes[i] / 8] |= (1 << (hashes[i] % 8));
    }
}

int check_bloom_filter(BloomFilter *filter, const char *str)
{
    unsigned int hashes[HASH_COUNT];
    int len = strlen(str);
    hashes[0] = MurmurHash3_x86_32(str, len, 1) % (BITMAP_SIZE * 8);
    hashes[1] = MurmurHash3_x86_32(str, len, 2) % (BITMAP_SIZE * 8);
    hashes[2] = MurmurHash3_x86_32(str, len, 3) % (BITMAP_SIZE * 8);
    hashes[3] = MurmurHash3_x86_32(str, len, 4) % (BITMAP_SIZE * 8);
    hashes[4] = djb2(str) % (BITMAP_SIZE * 8);

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

    // 将结果输出到output_2.txt文件
    FILE *output_file = fopen("output_2.txt", "w");
    if (!output_file)
    {
        perror("Failed to open output file");
        return 1;
    }
    fprintf(output_file, "%d\n", count);
    fclose(output_file);
    printf("Number of emails in the blacklist: %d\n", count);
    return 0;
}
