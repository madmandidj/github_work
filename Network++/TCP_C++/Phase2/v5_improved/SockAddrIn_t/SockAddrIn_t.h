#ifndef __SOCKADDR_T_H__
#define __SOCKADDR_T_H__

#include <netinet/in.h>

namespace netcpp
{
class SockAddrIn_t
{
public:
    void SetSockAddrIn(short _family, unsigned short _port, const char* _addr);
    void Reset();
    friend class ServerSock;
    friend class ClientSock;
private:
	sockaddr_in m_rawSin;
};
}//namespace netcpp
#endif//#ifndef __SOCKADDR_T_H__

