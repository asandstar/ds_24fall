#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

// !a:
// if a is true, then !a is false
// if a is false, then !a is true

//~00001111=11110000
// ��ѭλ����������������⻹��һ�����ƣ��㲻��ʹ�����(==)�Ͳ���ȣ���=��
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
// ���� ���ֽ���-1�����ȫΪ0����������λ
// ���� �ұ�ȫ��0
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
    // %#x:��0xǰ׺��ʮ������
    printf("A.�����κ�λ������1\n");
    printf("A(%#x):%d\n", a0, A(a0));
    printf("A(%#x):%d\n", a1, A(a1));
    printf("A(%#x):%d\n", a2, A(a2));

    // test B
    int b0 = 0x0;
    int b1 = 0x1;
    int b2 = 0xff;
    printf("B.x���κ�λ������0\n");
    printf("B(%#x):%d\n", b0, B(b0));
    printf("B(%#x):%d\n", b1, B(b1));
    printf("B(%#x):%d\n", b2, B(b2));

    // test C
    int c0 = 0x00ff;
    int c1 = 0x01234;
    int c2 = 0x012345ff;
    printf("C.���������Ч�ֽ��е�λ������1\n");
    printf("C(%#x):%d\n", c0, C(c0));
    printf("C(%#x):%d\n", c1, C(c1));
    printf("C(%#x):%d\n", c2, C(c2));

    // test D
    int d0 = 0x00123456;
    int d1 = 0x12345678;
    int d2 = 0xff000000;
    printf("D.���������Ч�ֽ��е�λ������0\n");
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