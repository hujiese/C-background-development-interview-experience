<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->

- [HTTP 服务器](#http-%E6%9C%8D%E5%8A%A1%E5%99%A8)
  - [一、HTTP 协议案例：](#%E4%B8%80http-%E5%8D%8F%E8%AE%AE%E6%A1%88%E4%BE%8B)
  - [二、HTTP Request](#%E4%BA%8Chttp-request)
    - [1、请求行三个字段的处理](#1%E8%AF%B7%E6%B1%82%E8%A1%8C%E4%B8%89%E4%B8%AA%E5%AD%97%E6%AE%B5%E7%9A%84%E5%A4%84%E7%90%86)
    - [2、处理请求行](#2%E5%A4%84%E7%90%86%E8%AF%B7%E6%B1%82%E8%A1%8C)
  - [三、HTTP Response](#%E4%B8%89http-response)
  - [四、HTTP Server](#%E5%9B%9Bhttp-server)
    - [1、HttpContext](#1httpcontext)
    - [2、onConnection 函数](#2onconnection-%E5%87%BD%E6%95%B0)
    - [3、onMessage 函数](#3onmessage-%E5%87%BD%E6%95%B0)
  - [五、使用案例代码](#%E4%BA%94%E4%BD%BF%E7%94%A8%E6%A1%88%E4%BE%8B%E4%BB%A3%E7%A0%81)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## HTTP 服务器

muduo源代码中net/http下提供了一个简单的HTTP服务器案例，该文章将具体探讨该服务器案例。

一个基于Muduo的Http服务器改进版可见：
> https://github.com/hujiese/HttpServer

### 一、HTTP 协议案例：

一个典型的GET请求如下：
```c
GET /hello.txt HTTP/1.1
User-Agent: curl/7.16.3 libcurl/7.16.3 OpenSSL/0.9.7l zlib/1.2.3
Host: www.example.com
Accept-Language: en, mi
```
服务端响应：
```c
HTTP/1.1 200 OK
Date: Mon, 27 Jul 2009 12:28:53 GMT
Server: Apache
Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT
ETag: "34aa387-d-1568eb00"
Accept-Ranges: bytes
Content-Length: 51
Vary: Accept-Encoding
Content-Type: text/plain
```
具体的协议内容详见基础部分的计算机网络推荐阅读部分。

### 二、HTTP Request

客户端发送一个HTTP请求到服务器的请求消息包括以下格式：请求行（request line）、请求头部（header）、空行和请求数据四个部分组成，下图给出了请求报文的一般格式：

![](https://www.runoob.com/wp-content/uploads/2013/11/2012072810301161.png)

总结起来：

* 请求行里面有“请求方法（GET/POST...）”、“请求资源（URL）”和“协议版本（HTTP/1.0 or HTTP/1.1）”
* 请求头部通过使用“:”将头部字段和对应的值分开，可理解为key-value关系
* 注意协议中的“空行”
* 最后面是请求数据（GET方法没有请求数据，POST有）
* 每一行内容使用\r\n作为结束标志

所以，如果要解析一个HTTP请求，以上内容是必不可少的，muduo的HttpRequest.h中提供了HttpRequest类来处理一个HTTP请求。

对于请求方法和HTTP协议，该类定义了如下几个enmu，并在构造函数中做了一些默认初始化：
```c
enum Method
{
  kInvalid, kGet, kPost, kHead, kPut, kDelete
};

enum Version
{
  kUnknown, kHttp10, kHttp11
};

HttpRequest()
  : method_(kInvalid),
    version_(kUnknown)
{
}
```

#### 1、请求行三个字段的处理
该类定义了如下三个私有成员变量来保存这三个值：
```c
private:
  Method method_;
  Version version_;
  string path_;
```
处理这这个值的部分比较简单：
```c
void setVersion(Version v)
{
  version_ = v;
}

Version getVersion() const
{ return version_; }

bool setMethod(const char* start, const char* end)
{
  assert(method_ == kInvalid);
  string m(start, end);
  if (m == "GET")
  {
    method_ = kGet;
  }
  else if (m == "POST")
  {
    method_ = kPost;
  }
  else if (m == "HEAD")
  {
    method_ = kHead;
  }
  else if (m == "PUT")
  {
    method_ = kPut;
  }
  else if (m == "DELETE")
  {
    method_ = kDelete;
  }
  else
  {
    method_ = kInvalid;
  }
  return method_ != kInvalid;
}

 Method method() const
{ return method_; }

const char* methodString() const
{
  const char* result = "UNKNOWN";
  switch(method_)
  {
    case kGet:
      result = "GET";
      break;
    case kPost:
      result = "POST";
      break;
    case kHead:
      result = "HEAD";
      break;
    case kPut:
      result = "PUT";
      break;
    case kDelete:
      result = "DELETE";
      break;
    default:
      break;
  }
  return result;
}

void setPath(const char* start, const char* end)
{
  path_.assign(start, end);
}

const string& path() const
{ return path_; }
```
#### 2、处理请求行

前面说到过，请求行每行内容都是通过“:”分割的key-value结构，所以在该类中定义了一个map来保存这些内容：
```c
std::map<string, string> headers_;
```
可以使用该函数向该map结构中添加一个请求行内容，：
```c
void addHeader(const char* start, const char* colon, const char* end)
{
  string field(start, colon);
  ++colon;
  while (colon < end && isspace(*colon))
  {
    ++colon;
  }
  string value(colon, end);
  while (!value.empty() && isspace(value[value.size()-1]))
  {
    value.resize(value.size()-1);
  }
  headers_[field] = value;
}
```
这里先“剧透”下该函数的使用场景：
```c
const char* crlf = buf->findCRLF();
if (crlf)
{
  const char* colon = std::find(buf->peek(), crlf, ':');
  if (colon != crlf)
  {
    context->request().addHeader(buf->peek(), colon, crlf);
  }
  ....
```
这部分代码截取自TcpServer中的parseRequest函数。
```c
bool parseRequest(Buffer* buf, HttpContext* context, Timestamp receiveTime)
```
在该函数中读取到一行内容就会查找到改行“:”的偏移，所以addHeader函数的第一个参数指的是请求一行的起始位置，第二个参数指的是":"的偏移，第三个参数指的是"crlf"的地址，从start到end是一个请求行里所有的有意义的内容，以：
```c
Host: www.example.com
```
为例，addHeader函数将“Host”临时保存在field变量中，将“www.example.com”保存在value变量中，然后全部放到headers_中。通过这种方法来保存一行的内容。

如果要获取某个请求行字段的内容，可以通过该函数：
```c
string getHeader(const string& field) const
{
  string result;
  std::map<string, string>::const_iterator it = headers_.find(field);
  if (it != headers_.end())
  {
    result = it->second;
  }
  return result;
}
```
通过该函数则可以获取到所以请求行的内容：
```c
const std::map<string, string>& headers() const
{ return headers_; }
```

到此，HttpRequest便分析完了。

### 三、HTTP Response

HTTP响应也由四个部分组成，分别是：状态行、消息报头、空行和响应正文：
![](https://www.runoob.com/wp-content/uploads/2013/11/httpmessage.jpg)

需要注意的是，请求如果是HTTP/1.0，那么服务器将使用“短连接”，如果是HTTP/1.1，那么使用“长连接”。

响应报文的相应行应包含“协议版本”、“状态码”和“状态”三个字段，所以在该类中也构造了一个枚举HttpStatusCode来保存这些状态：
```c
enum HttpStatusCode
{
  kUnknown,
  k200Ok = 200,
  k301MovedPermanently = 301,
  k400BadRequest = 400,
  k404NotFound = 404,
};

explicit HttpResponse(bool close)
  : statusCode_(kUnknown),
    closeConnection_(close)
{
}
```
该类使用如下变量来标识响应状态和状态码：
```c
  HttpStatusCode statusCode_;
  // FIXME: add http version
  string statusMessage_;
```
通过如下函数来设置这两个变量：
```c
void setStatusCode(HttpStatusCode code)
{ statusCode_ = code; }

void setStatusMessage(const string& message)
{ statusMessage_ = message; }
```
同样地，该类使用一个map结构来保存响应行：
```c
std::map<string, string> headers_;
```
通过这两个函数设置响应行里面内容：
```c
void setContentType(const string& contentType)
{ addHeader("Content-Type", contentType); }
// FIXME: replace string with StringPiece
void addHeader(const string& key, const string& value)
{ headers_[key] = value; }
```
响应体的设置也很简单：
```c
string body_;

void setBody(const string& body){ body_ = body; }
```
下面这个函数才是精髓，它构造了整个响应报文：
```c
void HttpResponse::appendToBuffer(Buffer* output) const
{
  char buf[32];
  snprintf(buf, sizeof buf, "HTTP/1.1 %d ", statusCode_);
  output->append(buf);
  output->append(statusMessage_);
  output->append("\r\n");

  if (closeConnection_)
  {
    output->append("Connection: close\r\n");
  }
  else
  {
    snprintf(buf, sizeof buf, "Content-Length: %zd\r\n", body_.size());
    output->append(buf);
    output->append("Connection: Keep-Alive\r\n");
  }

  for (std::map<string, string>::const_iterator it = headers_.begin();
       it != headers_.end();
       ++it)
  {
    output->append(it->first);
    output->append(": ");
    output->append(it->second);
    output->append("\r\n");
  }

  output->append("\r\n");
  output->append(body_);
}
```
可对照前面的HTTP响应报文自行分析。

### 四、HTTP Server

muduo中定义了HttpServer来处理HTTP请求，该类内部也是持有一个TcpServer，关于该类的细节和使用这里不再累述。对于该类的理解需要理解这几个函数：
```c
void onConnection(const TcpConnectionPtr& conn);
void onMessage(const TcpConnectionPtr& conn,
               Buffer* buf,
               Timestamp receiveTime);
void onRequest(const TcpConnectionPtr&, const HttpRequest&);
```
前两个函数在构造函数中也被设置为了TcpServer的回调函数：
```c
HttpServer::HttpServer(EventLoop* loop,
                       const InetAddress& listenAddr,
                       const string& name)
  : server_(loop, listenAddr, name),
    httpCallback_(detail::defaultHttpCallback)
{
  server_.setConnectionCallback(
      boost::bind(&HttpServer::onConnection, this, _1));
  server_.setMessageCallback(
      boost::bind(&HttpServer::onMessage, this, _1, _2, _3));
}
```
#### 1、HttpContext

在介绍前面几个函数之前，需要了解下HttpContext类。解析一个Http请求需要用到状态机模型，在该http服务器案例中也是如此，对于一个HTTP请求及其报文，解析器需要判断该报文是否是完整的，是否是合法的。对于报文中的不同内容，解析的方法也会有所不同，请求头中字段是通过空格来分隔的，但请求行中内容是通过冒号来分割的，而且请求行往往有多行。为了方便HTTP协议解析，HttpContex类便诞生了，该类的内容十分简单，可自行分析代码：
```c
class HttpContext : public muduo::copyable
{
 public:
  enum HttpRequestParseState
  {
    kExpectRequestLine,
    kExpectHeaders,
    kExpectBody,
    kGotAll,
  };

  HttpContext()
    : state_(kExpectRequestLine)
  {
  }

  // default copy-ctor, dtor and assignment are fine

  bool expectRequestLine() const
  { return state_ == kExpectRequestLine; }

  bool expectHeaders() const
  { return state_ == kExpectHeaders; }

  bool expectBody() const
  { return state_ == kExpectBody; }

  bool gotAll() const
  { return state_ == kGotAll; }

  void receiveRequestLine()
  { state_ = kExpectHeaders; }

  void receiveHeaders()
  { state_ = kGotAll; }  // FIXME

  void reset()
  {
    state_ = kExpectRequestLine;
    HttpRequest dummy;
    request_.swap(dummy);
  }

  const HttpRequest& request() const
  { return request_; }

  HttpRequest& request()
  { return request_; }

 private:
  HttpRequestParseState state_;
  HttpRequest request_;
};
```

#### 2、onConnection 函数

该函数会在一个Http客户端连接时调用，该函数定义如下：
```
void HttpServer::onConnection(const TcpConnectionPtr& conn)
{
  if (conn->connected())
  {
    conn->setContext(HttpContext());
  }
}
```
其内容十分简单，只是给该连接设置了一个HttpContext，该HttpContext在后面解析协议时会用到。

#### 3、onMessage 函数

建立好连接后，客户端将发送HTTP协议内容，onMessage函数便会调用：
```c
void HttpServer::onMessage(const TcpConnectionPtr& conn,
                           Buffer* buf,
                           Timestamp receiveTime)
{
  HttpContext* context = boost::any_cast<HttpContext>(conn->getMutableContext());

  if (!detail::parseRequest(buf, context, receiveTime))
  {
    conn->send("HTTP/1.1 400 Bad Request\r\n\r\n");
    conn->shutdown();
  }

  if (context->gotAll())
  {
    onRequest(conn, context->request());
    context->reset();
  }
}
```
该函数一开始使便获取到onConnection函数中设置的HttpContex内容，然后调用parseRequest函数开始解析HTTP报文。

parseRequest函数定义如下：
```c
bool parseRequest(Buffer* buf, HttpContext* context, Timestamp receiveTime)
{
  bool ok = true;
  bool hasMore = true;
  while (hasMore)
  {
    if (context->expectRequestLine())
    {
      const char* crlf = buf->findCRLF();
      if (crlf)
      {
        ok = processRequestLine(buf->peek(), crlf, context);
        if (ok)
        {
          context->request().setReceiveTime(receiveTime);
          buf->retrieveUntil(crlf + 2);
          context->receiveRequestLine();
        }
        else
        {
          hasMore = false;
        }
      }
      else
      {
        hasMore = false;
      }
    }
    else if (context->expectHeaders())
    {
      const char* crlf = buf->findCRLF();
      if (crlf)
      {
        const char* colon = std::find(buf->peek(), crlf, ':');
        if (colon != crlf)
        {
          context->request().addHeader(buf->peek(), colon, crlf);
        }
        else
        {
          // empty line, end of header
          context->receiveHeaders();
          hasMore = !context->gotAll();
        }
        buf->retrieveUntil(crlf + 2);
      }
      else
      {
        hasMore = false;
      }
    }
    else if (context->expectBody())
    {
      // FIXME:
    }
  }
  return ok;
}
```
在构造函数中已经做过如下设置：
```c
state_(kExpectRequestLine)
```
所以当前状态是“解析请求头”，解析完请求头后便是处理请求行，这部分代码也比较简单，基本上是字符处理，可针对HTTP报文内容自行分析，解析请求行代码如下：
```c
bool processRequestLine(const char* begin, const char* end, HttpContext* context)
{
  bool succeed = false;
  const char* start = begin;
  const char* space = std::find(start, end, ' ');
  HttpRequest& request = context->request();
  if (space != end && request.setMethod(start, space))
  {
    start = space+1;
    space = std::find(start, end, ' ');
    if (space != end)
    {
      request.setPath(start, space);
      start = space+1;
      succeed = end-start == 8 && std::equal(start, end-1, "HTTP/1.");
      if (succeed)
      {
        if (*(end-1) == '1')
        {
          request.setVersion(HttpRequest::kHttp11);
        }
        else if (*(end-1) == '0')
        {
          request.setVersion(HttpRequest::kHttp10);
        }
        else
        {
          succeed = false;
        }
      }
    }
  }
  return succeed;
}
```
在解析完一个HTTP协议后，回到onMessage函数中，该函数调用了onRequest(conn, context->request())，onRequest函数定义如下：
```c
void HttpServer::onRequest(const TcpConnectionPtr& conn, const HttpRequest& req)
{
  const string& connection = req.getHeader("Connection");
  bool close = connection == "close" ||
    (req.getVersion() == HttpRequest::kHttp10 && connection != "Keep-Alive");
  HttpResponse response(close);
  httpCallback_(req, &response);
  Buffer buf;
  response.appendToBuffer(&buf);
  conn->send(&buf);
  if (response.closeConnection())
  {
    conn->shutdown();
  }
}
```
该函数的任务是针对HTTP请求来构造响应报文，一开始便根据协议版本构建响应体内容，然后调用函数httpCallback_，该函数是需要使用者传入的，这里以配套案例为例：
```c
void onRequest(const HttpRequest& req, HttpResponse* resp)
{
  std::cout << "Headers " << req.methodString() << " " << req.path() << std::endl;
  if (!benchmark)
  {
    const std::map<string, string>& headers = req.headers();
    for (std::map<string, string>::const_iterator it = headers.begin();
         it != headers.end();
         ++it)
    {
      std::cout << it->first << ": " << it->second << std::endl;
    }
  }

  if (req.path() == "/")
  {
    resp->setStatusCode(HttpResponse::k200Ok);
    resp->setStatusMessage("OK");
    resp->setContentType("text/html");
    resp->addHeader("Server", "Muduo");
    string now = Timestamp::now().toFormattedString();
    resp->setBody("<html><head><title>This is title</title></head>"
        "<body><h1>Hello</h1>Now is " + now +
        "</body></html>");
  }
  else if (req.path() == "/favicon.ico")
  {
    resp->setStatusCode(HttpResponse::k200Ok);
    resp->setStatusMessage("OK");
    resp->setContentType("image/png");
    resp->setBody(string(favicon, sizeof favicon));
  }
  else if (req.path() == "/hello")
  {
    resp->setStatusCode(HttpResponse::k200Ok);
    resp->setStatusMessage("OK");
    resp->setContentType("text/plain");
    resp->addHeader("Server", "Muduo");
    resp->setBody("hello, world!\n");
  }
  else
  {
    resp->setStatusCode(HttpResponse::k404NotFound);
    resp->setStatusMessage("Not Found");
    resp->setCloseConnection(true);
  }
}
```
在该函数中需要处理HTTP请求的资源，构造响应报文所需要的字段，换言之就是设置HttpResponse的所有字段。在设置好HttpResponse的所有字段之后，回到onRequest函数，构建一个Buffer，调用appendToBuffer函数，将这些字段的内容生成对应的HTTP响应报文并送入Buffer中，然后发送给客户端。

最后:
```c
if (response.closeConnection())
{
  conn->shutdown();
}
```
如果是HTTP1.0协议，那么完成请求后服务器直接断开连接，如果是HTTP1.1协议，那么维持连接（长连接）。

到此，HttpServer也分析完了。

### 五、使用案例代码

```c
#include <muduo/net/http/HttpServer.h>
#include <muduo/net/http/HttpRequest.h>
#include <muduo/net/http/HttpResponse.h>
#include <muduo/net/EventLoop.h>
#include <muduo/base/Logging.h>

#include <iostream>
#include <map>

using namespace muduo;
using namespace muduo::net;

extern char favicon[555];
bool benchmark = false;

void onRequest(const HttpRequest& req, HttpResponse* resp)
{
  std::cout << "Headers " << req.methodString() << " " << req.path() << std::endl;
  if (!benchmark)
  {
    const std::map<string, string>& headers = req.headers();
    for (std::map<string, string>::const_iterator it = headers.begin();
         it != headers.end();
         ++it)
    {
      std::cout << it->first << ": " << it->second << std::endl;
    }
  }

  if (req.path() == "/")
  {
    resp->setStatusCode(HttpResponse::k200Ok);
    resp->setStatusMessage("OK");
    resp->setContentType("text/html");
    resp->addHeader("Server", "Muduo");
    string now = Timestamp::now().toFormattedString();
    resp->setBody("<html><head><title>This is title</title></head>"
        "<body><h1>Hello</h1>Now is " + now +
        "</body></html>");
  }
  else if (req.path() == "/favicon.ico")
  {
    resp->setStatusCode(HttpResponse::k200Ok);
    resp->setStatusMessage("OK");
    resp->setContentType("image/png");
    resp->setBody(string(favicon, sizeof favicon));
  }
  else if (req.path() == "/hello")
  {
    resp->setStatusCode(HttpResponse::k200Ok);
    resp->setStatusMessage("OK");
    resp->setContentType("text/plain");
    resp->addHeader("Server", "Muduo");
    resp->setBody("hello, world!\n");
  }
  else
  {
    resp->setStatusCode(HttpResponse::k404NotFound);
    resp->setStatusMessage("Not Found");
    resp->setCloseConnection(true);
  }
}

int main(int argc, char* argv[])
{
  int numThreads = 0;
  if (argc > 1)
  {
    benchmark = true;
    Logger::setLogLevel(Logger::WARN);
    numThreads = atoi(argv[1]);
  }
  EventLoop loop;
  HttpServer server(&loop, InetAddress(8000), "dummy");
  server.setHttpCallback(onRequest);
  server.setThreadNum(numThreads);
  server.start();
  loop.loop();
}

char favicon[555] = {
  '\x89', 'P', 'N', 'G', '\xD', '\xA', '\x1A', '\xA',
  '\x0', '\x0', '\x0', '\xD', 'I', 'H', 'D', 'R',
  '\x0', '\x0', '\x0', '\x10', '\x0', '\x0', '\x0', '\x10',
  '\x8', '\x6', '\x0', '\x0', '\x0', '\x1F', '\xF3', '\xFF',
  'a', '\x0', '\x0', '\x0', '\x19', 't', 'E', 'X',
  't', 'S', 'o', 'f', 't', 'w', 'a', 'r',
  'e', '\x0', 'A', 'd', 'o', 'b', 'e', '\x20',
  'I', 'm', 'a', 'g', 'e', 'R', 'e', 'a',
  'd', 'y', 'q', '\xC9', 'e', '\x3C', '\x0', '\x0',
  '\x1', '\xCD', 'I', 'D', 'A', 'T', 'x', '\xDA',
  '\x94', '\x93', '9', 'H', '\x3', 'A', '\x14', '\x86',
  '\xFF', '\x5D', 'b', '\xA7', '\x4', 'R', '\xC4', 'm',
  '\x22', '\x1E', '\xA0', 'F', '\x24', '\x8', '\x16', '\x16',
  'v', '\xA', '6', '\xBA', 'J', '\x9A', '\x80', '\x8',
  'A', '\xB4', 'q', '\x85', 'X', '\x89', 'G', '\xB0',
  'I', '\xA9', 'Q', '\x24', '\xCD', '\xA6', '\x8', '\xA4',
  'H', 'c', '\x91', 'B', '\xB', '\xAF', 'V', '\xC1',
  'F', '\xB4', '\x15', '\xCF', '\x22', 'X', '\x98', '\xB',
  'T', 'H', '\x8A', 'd', '\x93', '\x8D', '\xFB', 'F',
  'g', '\xC9', '\x1A', '\x14', '\x7D', '\xF0', 'f', 'v',
  'f', '\xDF', '\x7C', '\xEF', '\xE7', 'g', 'F', '\xA8',
  '\xD5', 'j', 'H', '\x24', '\x12', '\x2A', '\x0', '\x5',
  '\xBF', 'G', '\xD4', '\xEF', '\xF7', '\x2F', '6', '\xEC',
  '\x12', '\x20', '\x1E', '\x8F', '\xD7', '\xAA', '\xD5', '\xEA',
  '\xAF', 'I', '5', 'F', '\xAA', 'T', '\x5F', '\x9F',
  '\x22', 'A', '\x2A', '\x95', '\xA', '\x83', '\xE5', 'r',
  '9', 'd', '\xB3', 'Y', '\x96', '\x99', 'L', '\x6',
  '\xE9', 't', '\x9A', '\x25', '\x85', '\x2C', '\xCB', 'T',
  '\xA7', '\xC4', 'b', '1', '\xB5', '\x5E', '\x0', '\x3',
  'h', '\x9A', '\xC6', '\x16', '\x82', '\x20', 'X', 'R',
  '\x14', 'E', '6', 'S', '\x94', '\xCB', 'e', 'x',
  '\xBD', '\x5E', '\xAA', 'U', 'T', '\x23', 'L', '\xC0',
  '\xE0', '\xE2', '\xC1', '\x8F', '\x0', '\x9E', '\xBC', '\x9',
  'A', '\x7C', '\x3E', '\x1F', '\x83', 'D', '\x22', '\x11',
  '\xD5', 'T', '\x40', '\x3F', '8', '\x80', 'w', '\xE5',
  '3', '\x7', '\xB8', '\x5C', '\x2E', 'H', '\x92', '\x4',
  '\x87', '\xC3', '\x81', '\x40', '\x20', '\x40', 'g', '\x98',
  '\xE9', '6', '\x1A', '\xA6', 'g', '\x15', '\x4', '\xE3',
  '\xD7', '\xC8', '\xBD', '\x15', '\xE1', 'i', '\xB7', 'C',
  '\xAB', '\xEA', 'x', '\x2F', 'j', 'X', '\x92', '\xBB',
  '\x18', '\x20', '\x9F', '\xCF', '3', '\xC3', '\xB8', '\xE9',
  'N', '\xA7', '\xD3', 'l', 'J', '\x0', 'i', '6',
  '\x7C', '\x8E', '\xE1', '\xFE', 'V', '\x84', '\xE7', '\x3C',
  '\x9F', 'r', '\x2B', '\x3A', 'B', '\x7B', '7', 'f',
  'w', '\xAE', '\x8E', '\xE', '\xF3', '\xBD', 'R', '\xA9',
  'd', '\x2', 'B', '\xAF', '\x85', '2', 'f', 'F',
  '\xBA', '\xC', '\xD9', '\x9F', '\x1D', '\x9A', 'l', '\x22',
  '\xE6', '\xC7', '\x3A', '\x2C', '\x80', '\xEF', '\xC1', '\x15',
  '\x90', '\x7', '\x93', '\xA2', '\x28', '\xA0', 'S', 'j',
  '\xB1', '\xB8', '\xDF', '\x29', '5', 'C', '\xE', '\x3F',
  'X', '\xFC', '\x98', '\xDA', 'y', 'j', 'P', '\x40',
  '\x0', '\x87', '\xAE', '\x1B', '\x17', 'B', '\xB4', '\x3A',
  '\x3F', '\xBE', 'y', '\xC7', '\xA', '\x26', '\xB6', '\xEE',
  '\xD9', '\x9A', '\x60', '\x14', '\x93', '\xDB', '\x8F', '\xD',
  '\xA', '\x2E', '\xE9', '\x23', '\x95', '\x29', 'X', '\x0',
  '\x27', '\xEB', 'n', 'V', 'p', '\xBC', '\xD6', '\xCB',
  '\xD6', 'G', '\xAB', '\x3D', 'l', '\x7D', '\xB8', '\xD2',
  '\xDD', '\xA0', '\x60', '\x83', '\xBA', '\xEF', '\x5F', '\xA4',
  '\xEA', '\xCC', '\x2', 'N', '\xAE', '\x5E', 'p', '\x1A',
  '\xEC', '\xB3', '\x40', '9', '\xAC', '\xFE', '\xF2', '\x91',
  '\x89', 'g', '\x91', '\x85', '\x21', '\xA8', '\x87', '\xB7',
  'X', '\x7E', '\x7E', '\x85', '\xBB', '\xCD', 'N', 'N',
  'b', 't', '\x40', '\xFA', '\x93', '\x89', '\xEC', '\x1E',
  '\xEC', '\x86', '\x2', 'H', '\x26', '\x93', '\xD0', 'u',
  '\x1D', '\x7F', '\x9', '2', '\x95', '\xBF', '\x1F', '\xDB',
  '\xD7', 'c', '\x8A', '\x1A', '\xF7', '\x5C', '\xC1', '\xFF',
  '\x22', 'J', '\xC3', '\x87', '\x0', '\x3', '\x0', 'K',
  '\xBB', '\xF8', '\xD6', '\x2A', 'v', '\x98', 'I', '\x0',
  '\x0', '\x0', '\x0', 'I', 'E', 'N', 'D', '\xAE',
  'B', '\x60', '\x82',
};
```