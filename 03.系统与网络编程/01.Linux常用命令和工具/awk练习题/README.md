<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->

- [AWK常见问题汇总](#awk%E5%B8%B8%E8%A7%81%E9%97%AE%E9%A2%98%E6%B1%87%E6%80%BB)
  - [问题一 -- 打印学生成绩单](#%E9%97%AE%E9%A2%98%E4%B8%80----%E6%89%93%E5%8D%B0%E5%AD%A6%E7%94%9F%E6%88%90%E7%BB%A9%E5%8D%95)
  - [问题二 -- 最多用户名和IP](#%E9%97%AE%E9%A2%98%E4%BA%8C----%E6%9C%80%E5%A4%9A%E7%94%A8%E6%88%B7%E5%90%8D%E5%92%8Cip)
  - [问题三 -- 加减输出](#%E9%97%AE%E9%A2%98%E4%B8%89----%E5%8A%A0%E5%87%8F%E8%BE%93%E5%87%BA)
  - [问题四 -- 分类输出文件内容](#%E9%97%AE%E9%A2%98%E5%9B%9B----%E5%88%86%E7%B1%BB%E8%BE%93%E5%87%BA%E6%96%87%E4%BB%B6%E5%86%85%E5%AE%B9)
  - [问题五 -- 输出重复最多的IP](#%E9%97%AE%E9%A2%98%E4%BA%94----%E8%BE%93%E5%87%BA%E9%87%8D%E5%A4%8D%E6%9C%80%E5%A4%9A%E7%9A%84ip)
  - [问题六 -- 排序输出重复最多的域名](#%E9%97%AE%E9%A2%98%E5%85%AD----%E6%8E%92%E5%BA%8F%E8%BE%93%E5%87%BA%E9%87%8D%E5%A4%8D%E6%9C%80%E5%A4%9A%E7%9A%84%E5%9F%9F%E5%90%8D)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## AWK常见问题汇总

### 问题一 -- 打印学生成绩单

假设有这么一个文件（学生成绩表，score.txt）：

```shell
Marry   2143 78 84 77
Jack    2321 66 78 45
Tom     2122 48 77 71
Mike    2537 87 97 95
Bob     2415 40 57 62
```
请按照格式输出如下内容：
```shell
NAME    NO.   MATH  ENGLISH  COMPUTER   TOTAL
---------------------------------------------
Marry  2143     78       84       77      239
Jack   2321     66       78       45      189
Tom    2122     48       77       71      196
Mike   2537     87       97       95      279
Bob    2415     40       57       62      159
---------------------------------------------
  TOTAL:       319      393      350
AVERAGE:     63.80    78.60    70.00
```
awk脚本（question1.awk）：
```shell
BEGIN {
    math = 0
    english = 0
    computer = 0
    printf "NAME    NO.   MATH  ENGLISH  COMPUTER   TOTAL\n"
    printf "---------------------------------------------\n"
}

{
   math += $3
   english += $4
   computer += $5

   printf "%-6s %-4s %5d %5d %5d %5d\n", $1, $2, $3, $4, $5, $1+$2+$3
}

END{
    printf "TOTAL: %10d %8d %6d\n", math, english, computer
    printf "AVG: %10.2f %8.2f %6.2f\n", math/NR, english/NR, computer/NR
}
```
命令如下：
```
awk -f question1.awk score.txt
```
### 问题二 -- 最多用户名和IP

有文件内容如下（ipAndUser）：
```shell
1.1.1.1 st_000012 ajgoahgoahgoahgghg
2.2.2.2 st_000014 ajgoaoghaughughgu
3.3.3.3 st_000015 alfjoigrhuogrub
4.4.4.4 st_000016 ajgoaghkaahgoahgghg
1.1.1.1 st_000019 ajgoalllllghughgu
5.5.5.5 st_000015 alfjoigrhuogrub
```
找到第一列中出现最多的ip，第二列中出现最多的用户名。
awk脚本如下（question2.awk）：

```shell
BEGIN {
    max_ip = 0
    max_ip_cnt = 0
    max_usr = 0
    max_usr_cnt = 0
}

{
    ++ip[$1]
    ++host[$2]
}

END {
    for(i in ip)
    {
        if(ip[i] > max_ip_cnt)
        {
            max_ip = i
            max_ip_cnt = ip[i]
        }
    }
    printf "%-10s %2d\n", max_ip, max_ip_cnt

    for (j in host)
    {
       if(host[j] > max_usr_cnt)
       {
            max_usr = j
            max_usr_cnt = host[j]
       }
    }
    printf "%-10s %2d\n", max_usr, max_usr_cnt
}
```
命令：
```shell
awk -f question2.awk ipAndUser
```
结果如下：
```
1.1.1.1     2
st_000015   2
```
### 问题三 -- 加减输出
有文件内容如下（numop）：
```
 100
 a 100
 b -50
 c -20
 d -30
```
 请输出：
 ```
100
a 100
200
b -50
150
c -20
130
d -30
 ```
脚本如下：
```shell
BEGIN {
    sum = 0   
}

{
    if( NR == 1 )
    {
        sum = $1
        printf "%3d\n", sum
    }
    else if ( NR == 5 )
    {
        printf "%-2s %3d\n", $1, $2
    }
    else
    {
        sum += $2
        printf "%-2s %3d\n%3d\n", $1, $2, sum
    }
}

END {

}
```
命令如下：
```shell
awk -f question3.awk numop
```

### 问题四 -- 分类输出文件内容

假设qq.tel文件内容:
```
12334:13510014336
12345:12334555666
12334:12343453453
12099:13598989899
12334:12345454545
12099:12343454544
```
需要如下分类：
```
[12334]
13510014336
12343453453
...........
[12099]
13598989899
12343454544
```
调用脚本实现功能（question4.awk）：
```shell
BEGIN {
    id = 0
}
{
    if($1 != id)
    {
        id = $1
        print "["id"]"
    }
    print $2
}

END {

}
```
命令如下：
```
cat qq.tel|sort -r|awk -F: -f question4.awk
[12345]
12334555666
[12334]
13510014336
12345454545
12343453453
[12099]
13598989899
12343454544
```
这里先将文件内容排序，然后将排序结果通过管道输入到question4.awk中。
### 问题五 -- 输出重复最多的IP
一个文本类型的文件（ips），里面每行存放登陆者的IP (某些行是重复的),写一个shell脚本输出登陆次数最多的用户。IP内容如下：
```
219.217.49.14
175.43.4.87
87.48.98.1
59.73.38.25
219.217.50.14
59.92.48.32
219.217.49.14
59.72.38.142
59.73.38.25
219.217.49.14
```
脚本如下：
```shell
BEGIN { 
    max = 0
    maxIp
}

{
    ++IP[$1]
}

END {
    for(i in IP)
    {
        if(IP[i] > max)
            max = IP[i]
            maxIp = i
    }
    print maxIp
}
```
结果如下：
```
$ awk -f question5.awk ips 
59.92.48.32
```
### 问题六 -- 排序输出重复最多的域名

处理一 下文件内容，将域名取出并进行计数排数：如处理:
```
http://www.baidu.com/index.html
http://www.baidu.com/1.html
http://www.baidu.com/2.html
http://post.baidu.com/index.html
http://mp3.baidu.com/index.html
http://www.baidu.com/3.html
http://post.baidu.com/2.html
```
输出格式如下：
```
4 www.baidu.com
2 post.baidu.com
1 mp3.baidu.com
```
处理脚本如下：
```shell
{
    ++url[$3]    
}

END{
    for(i in url)
    {
        print url[i] " " i
    }
}
```
处理命令和结果如下：
```shell
$ awk -F/ -f question6.awk urls | sort -r
4 www.baidu.com
2 post.baidu.com
1 mp3.baidu.com
```

