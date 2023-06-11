#include "pch.h"
#include "NetAddress.h"

NetAddress::NetAddress(SOCKADDR_IN sockAddr) : _sockAddr(sockAddr)
{
}

NetAddress::NetAddress(wstring ip, uint16 port)
{
	memset(&_sockAddr, 0, sizeof(_sockAddr));
	_sockAddr.sin_family = AF_INET;
	_sockAddr.sin_addr = ip2Addres(ip.c_str());
	_sockAddr.sin_port = htons(port);
}

wstring NetAddress::GetIpAddress()
{
	WCHAR buffer[100] = {};
	InetNtop(AF_INET, &_sockAddr.sin_addr, buffer, len32(buffer));
	return wstring(buffer);
}

IN_ADDR NetAddress::ip2Addres(const WCHAR* ip)
{
	IN_ADDR address;
	InetPton(AF_INET, ip, &address);

	return address;
}
