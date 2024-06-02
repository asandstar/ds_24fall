第一问的黑名单为blacklist_1.txt，待检测名单为underfilt_1.txt，最终输出为output_1.txt.

第二问的黑名单为blacklist_2.txt，待检测名单为underfilt_2.txt，最终输出为output_2.txt.

提交的文件无需包含这些文件，但代码中文件的读写请使用同样的文件名（测试时文件会与代码处于同一目录下），例如

```c
FILE* pf1 = fopen("blacklist_1.txt", "r");
FILE* pf2 = fopen("underfilt_1.txt", "r");
FILE* pf3 = fopen("output_1.txt", "w");
```

最终会使用另外的测试样例，但数据规模一致。

