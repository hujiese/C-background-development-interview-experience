## 一个基于libevent的http静态网页案例 ##

编译该文件：

	gcc http-server.c  -o app -levent

然后运行:

	./app ./http

打开浏览器，输入: http://127.0.0.1:8880/index.html 

然后浏览器端：

![](../img/http_server_demo.png)

终端：

![](../img/http_server_demo_t.png)

