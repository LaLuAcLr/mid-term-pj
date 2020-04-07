1. 编写一个函数，计算逻辑表达式的真值。
逻辑表达式最多有 8个输入项，分别为 A,B,C,D,E,F,G,H；
支持的逻辑运算符按优先级从高到低分别为 ~（非）、 （非）、 &（与）、 （与）、 （与）、 ^（异或）、 （异或）、 （异或）、 |（或）；可使用括号改变计算的顺序。
输出结果是一个由 '0' 或'1' 组成的字符串。
2. 编写一个函数，根据真值表计算逻辑表达式，以字符串格式输出。
使用Quine-McCluskey算法对逻辑表达式进行化简。

思路：
ABC从低到高分别是二进制的从低到高。可以用int8循环，分解为vector，再翻译表达式变成c++，逐个代入int中。