#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

// !a:
// if a is true, then !a is false
// if a is false, then !a is true

//~00001111=11110000
// 遵循位级整数编码规则，另外还有一个限制，你不能使用相等(==)和不相等（！=）
bool A(int x)
{
    return !~x;
}
bool B(int x)
{
    return !x;
}
bool C(int x)
{
    return !~(x | ~0xff);
}
// 右移 总字节数-1，左边全为0，保留符号位
// 左移 右边全补0
bool D(int x)
{
    return !(x >> ((sizeof(int) - 1) << 3));
}

int main()
{
    // test A
    int a0 = 0xffffffff;
    int a1 = 0xff;
    int a2 = 0xff002;
    // %#x:带0x前缀的十六进制
    printf("A.×的任何位都等于1\n");
    printf("A(%#x):%d\n", a0, A(a0));
    printf("A(%#x):%d\n", a1, A(a1));
    printf("A(%#x):%d\n", a2, A(a2));

    // test B
    int b0 = 0x0;
    int b1 = 0x1;
    int b2 = 0xff;
    printf("B.x的任何位都等于0\n");
    printf("B(%#x):%d\n", b0, B(b0));
    printf("B(%#x):%d\n", b1, B(b1));
    printf("B(%#x):%d\n", b2, B(b2));

    // test C
    int c0 = 0x00ff;
    int c1 = 0x01234;
    int c2 = 0x012345ff;
    printf("C.×的最低有效字节中的位都等于1\n");
    printf("C(%#x):%d\n", c0, C(c0));
    printf("C(%#x):%d\n", c1, C(c1));
    printf("C(%#x):%d\n", c2, C(c2));

    // test D
    int d0 = 0x00123456;
    int d1 = 0x12345678;
    int d2 = 0xff000000;
    printf("D.×的最高有效字节中的位都等于0\n");
    printf("D(%#x):%d\n", d0, D(d0));
    printf("D(%#x):%d\n", d1, D(d1));
    printf("D(%#x):%d\n", d2, D(d2));

    return 0;
}

// if (x | 0x00)
// {
//     return 1;
// }
// else
// {
//     return 0;
// }

// // if (x & 0xff == 0x00)
// if (!(x & 0xff))
// {
//     return 1;
// }
// else
// {
//     return 0;
// }

// // if (x | 0x0f == 0xff)
// if (x | 0x0f)
// {
//     return 1;
// }
// else
// {
//     return 0;
// }

// // if (x & 0xf0 == 0x00)
// if (!(x | 0xf0))
// {
//     return 1;
// }
// else
// {
//     return 0;
// }