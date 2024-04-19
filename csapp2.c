#include <stdio.h>
unsigned rotate_left(unsigned x, int n);
int main()
{
    unsigned x = 0x12345678;
    unsigned output;
    int n;
    int w = 32;

    scanf("%d", &n);
    if (0 <= n && n < w)
    {
        output = rotate_left(x, n);
        printf("%#x", output);
    }
}
unsigned rotate_left(unsigned x, int n)
{
    return (x << n) | (x >> (32 - n));
}