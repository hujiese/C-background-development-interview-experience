## 五个简单TCP 协议

* discard - 丢弃所有收到的数据；
* bdaytime - 服务端accept 连接之后，以字符串形式发送当前时间，然后主动断
开连接；
* time - 服务端accept 连接之后，以二进制形式发送当前时间（从Epoch 到现在
的秒数），然后主动断开连接；我们需要一个客户程序来把收到的时间转换为字
符串。
* echo - 回显服务，把收到的数据发回客户端；
* chargen - 服务端accept 连接之后，不停地发送测试数据。

### 具体可见reference目录下的《MuduoManual》的第50页。