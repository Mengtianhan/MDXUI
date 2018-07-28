#pragma once

#if defined(_MSC_VER) && (_MSC_VER > 1300)
#include <functional>
#endif

static long sComponents = 0; // 组件被引用的次数
static long sLockCounts = 0;

#ifdef _DEBUG
const char g_szFriendlyName[] = "MengJin COM Debug, AsioNetCom";
const char g_szVerIndProgID[] = "MengJin Debug.AsioNetCom";
const char g_szProgID[] = "MengJin AsioNetCom Debug.AsioNetCom.1";
#else
const char g_szFriendlyName[] = "MengJin COM Release, AsioNetCom";
const char g_szVerIndProgID[] = "MengJin Release.AsioNetCom";
const char g_szProgID[] = "MengJin  AsioNetCom Release.AsioNetCom.1";
#endif

#ifdef MAXBUFFERSIZE
#undef MAXBUFFERSIZE
#define MAXBUFFERSIZE 8192000   //默认buffer为8M
#else
#define MAXBUFFERSIZE 8192000   //默认buffer为8M
#endif

#if defined(_MSC_VER) && (_MSC_VER > 1300)
typedef std::function<void(const char*, __int64, const char*)>  SERVICEMSFFUN;  // 服务端处理网络消息的函数类型
typedef std::function<void(const char*, const char*)>			NETINFOFUN;		  // 显示网络状态信息
typedef std::function<void(const char*, __int64)>				CLIENTMSGFUN;	// 客户端和串口处理消息的函数类型
#else
typedef void(*SERVICEMSFFUN)(const char*, __int64, const char*);  // 服务端处理网络消息的函数类型
typedef void(*NETINFOFUN)(const char*, const char*);		  // 显示网络状态信息
typedef void(*CLIENTMSGFUN)(const char*, __int64);	// 客户端和串口处理消息的函数类型
#endif
