#pragma pack(1)
struct Header {
        int num ;//包id
        int index ;//学生编号
};
struct PkgContent {
        char sex ;//性别
        int score ;//分数
        char address [100];//地址
		int age;
};
struct Pkg {
        Header head;
        PkgContent content ;
};

int MySend( int iSock, char * pchBuf, size_t tLen){
        int iThisSend;
        unsigned int iSended=0;
        if(tLen == 0)
               return 0;
        while(iSended<tLen){
               do{
                     iThisSend = send(iSock, pchBuf, tLen-iSended, 0);
              } while((iThisSend<0) && (errno==EINTR));
               if(iThisSend < 0){
                      return(iSended);
              }
              iSended += iThisSend;
              pchBuf += iThisSend;
       }
        return(tLen);
}

int MyRecv( int iSock, char * pchBuf, size_t tCount){
        size_t tBytesRead=0;
        int iThisRead;
        while(tBytesRead < tCount){
               do{
                     iThisRead = read(iSock, pchBuf, tCount-tBytesRead);
              } while((iThisRead<0) && (errno==EINTR));
               if(iThisRead < 0){
                      return(iThisRead);
              }
               else if (iThisRead == 0)
                      return(tBytesRead);
              tBytesRead += iThisRead;
              pchBuf += iThisRead;
       }
}
