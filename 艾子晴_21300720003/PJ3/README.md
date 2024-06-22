使用的IDE:vscode
运行方法:切换到文件所在目录 输入 if ($?) { gcc pj3.c -o pj3 } ; if ($?) { .\pj3 }
输入
3
100 100
1 2 80
2 3 50
1 3 50
3 2 50
3 2 10
回车两次
如
PS C:\Users\azq\OneDrive\桌面\ds_24spring\21300720003\PJ4\code> cd "c:\Users\azq\OneDrive\桌面\ds_24spring\21300720003\PJ3\" ; if ($?) { gcc pj3.c -o pj3 } ; if ($?) { .\pj3 }
3
100 100
1 2 80
2 3 50
1 3 50
3 2 50
3 2 10

YES
YES
NO
YES
NO
0.000484
最后一行是时间测试

测试两个版本的数据所用命令如下
if ($?) { gcc brute_force.c -o brute_force } ; if ($?) { .\brute_force }
if ($?) { gcc segment_tree.c -o segment_tree } ; if ($?) { .\segment_tree }
语言版本:主要使用了标准C语言特性，但也包含了一些特定于Windows平台的性能计时功能，可以在C99及更高版本的C语言中运行。
以及使用的编译器:gcc