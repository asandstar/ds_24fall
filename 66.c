double polyhorner(double a[], double x, long degree)
{
    long i;
    double result = 0.0;

    // 处理前面的部分，使其对齐到8的倍数
    for (i = degree % 8; i > 0; --i)
    {
        result = a[i - 1] + x * result;
    }

    // 预先计算 x 的幂次
    double x1 = x;
    double x2 = x1 * x;
    double x3 = x2 * x;
    double x4 = x3 * x;
    double x5 = x4 * x;
    double x6 = x5 * x;
    double x7 = x6 * x;
    double x8 = x7 * x;

    // 处理对齐后的部分
    for (i = degree - 8; i >= 0; i -= 8)
    {
        result = (a[i] + a[i + 1] * x1 + a[i + 2] * x2 + a[i + 3] * x3 +
                  a[i + 4] * x4 + a[i + 5] * x5 + a[i + 6] * x6 +
                  a[i + 7] * x7 + result * x8);
    }

    return result;
}
