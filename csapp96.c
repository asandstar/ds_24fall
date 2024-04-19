#include <stdio.h>
#include <assert.h>
typedef unsigned float_bits;
float u2f(unsigned u)
{
    return *(float *)&u;
}
int float_f2i(float_bits f)
{
    unsigned sign = f >> 31;       // 提取浮点数的符号位（第31位）
    unsigned exp = f >> 23 & 0xFF; // 提取浮点数的指数部分（第23到30位）
    unsigned frac = f & 0x7FFFFF;  // 提取尾数部分（最低23位）
    unsigned bias = 0x7F;          // 浮点数的偏移量
    if (exp == 0xFF)               // 处理 NaN 或无穷大
    {
        return 0x80000000;
    }
    else if (exp < bias)
    {
        return 0; // 处理小于1的数
    }
    else if (exp >= 31 + bias)
    {
        return 0x80000000; // 溢出处理
    }
    else
    {
        unsigned E = exp - bias;
        unsigned M = frac | 0x800000;
        unsigned shifted_M = (E > 23) ? (M << (E - 23)) : (M >> (23 - E));
        // 检查是否由于左移而溢出
        if (E > 23 && (shifted_M >> (E - 23)) != M)
        {
            return 0x80000000;
        }
        // 应用符号位
        if (sign)
        {
            return -shifted_M; // 负数
        }
        else
        {
            return shifted_M; // 正数
        }
    }
}
int main()
{
    assert(float_f2i(0x98765432) == (int)u2f(0x98765432));
    assert(float_f2i(0x19802385) == (int)u2f(0x19802385));
    assert(float_f2i(0x78000087) == 0x80000000);
    assert(float_f2i(0x8F088888) == (int)u2f(0x8F088888));
    return 0;
}
