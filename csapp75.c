#include <stdio.h>
#include <assert.h>
#include <inttypes.h>

int signed_high_prod(int x, int y)
{
    int64_t prod = (int64_t)x * y; // 64bit
    return prod >> 32;             // return 32bit
}

unsigned unsigned_high_prod(unsigned x, unsigned y)
{
    int sign_x = x >> 31;                     // reserve the sign of x
    int sign_y = y >> 31;                     // reserve the sign of y
    int signed_prod = signed_high_prod(x, y); // high 32 bit of signed numbers
    return signed_prod + sign_x * y + sign_y * x;
}

unsigned another_unsigned_high_prod(unsigned x, unsigned y)
{
    uint64_t mul = (uint64_t)x * y;
    return mul >> 32;
}

int main(int argc, char *argv[])
{
    unsigned x = 0x12345678;
    unsigned y = 0xFFFFFFFF;
    assert(another_unsigned_high_prod(x, y) == unsigned_high_prod(x, y));
    return 0;
}
