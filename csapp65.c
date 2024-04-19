#include <stdio.h>
#include <assert.h>

int odd_ones(unsigned x)
{
    x ^= x >> 16;
    x ^= x >> 8;
    x ^= x >> 4;
    x ^= x >> 2;
    x ^= x >> 1;
    return x & 0x1;
}

int main()
{
    unsigned uval = 0x10101010;
    assert(!odd_ones(uval));
    uval = 0x10101011;
    assert(odd_ones(uval));
    return 0;
}