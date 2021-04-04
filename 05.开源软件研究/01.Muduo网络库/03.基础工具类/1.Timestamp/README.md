## Timestamp

更多内容可见[muduo_base库源码分析 -- Timestamp](https://github.com/hujiese/Large-concurrent-serve/blob/master/05_muduo_Timestamp/muduo_Timestamp.md)

Timestamp是muduo的时间戳类，在base/Timestamp.cc和base/Timestamp.h中可以找到它的完整定义。该类提供了时间戳的一些列操作。

### 一、Timestamp类

* <base/types.h>头文件
* less_than_comparable，要求实现<运算符，可自动实现>,<=,>=
* BOOST_STATIC_ASSERT编译时检查错误
* 使用PRId64，实现跨平台
* Timestamp实现及测试

Timestamp类图：

![](https://camo.githubusercontent.com/df49a64c810472f80212d8bbe29abfe3c12e9cea/68747470733a2f2f692e696d6775722e636f6d2f774969757067332e706e67)

### 二、补充：使用PRId64

int64_t用来表示64位整数，在32位系统中是long long int，在64位系统中是long int,所以打印int64_t的格式化方法是：

```c
printf(“%ld”, value);  // 64bit OS
printf("%lld", value); // 32bit OS
```
跨平台的做法：
```c
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#undef __STDC_FORMAT_MACROS 
printf("%" PRId64 "\n", value);
```

在muduo中使用案例：
```c
string Timestamp::toString() const
{
  char buf[32] = {0};
  int64_t seconds = microSecondsSinceEpoch_ / kMicroSecondsPerSecond;
  int64_t microseconds = microSecondsSinceEpoch_ % kMicroSecondsPerSecond;
  snprintf(buf, sizeof(buf)-1, "%" PRId64 ".%06" PRId64 "", seconds, microseconds);
  return buf;
}
```