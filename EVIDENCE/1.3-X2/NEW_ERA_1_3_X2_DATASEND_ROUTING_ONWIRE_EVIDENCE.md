# NEW-ERA 1.3-X2 — GS DataSend global + roteamento NEW_PROTOCOL_SYSTEM + on-wire C1/C3 — EVIDENCE

- Legacy commit pin: `580472e0d5723f9709cbad594f233deb07f9f351`


## Files fetched (sha256)

- `Util.h` bytes=1225 sha256 `a0d05d58bbada7ea132c07eb839473acb6002343db8ac4da1853ea7cdf533c02`  (https://raw.githubusercontent.com/wongddd/muonline/580472e0d5723f9709cbad594f233deb07f9f351/Source%20Server/GameServer/GameServer/Util.h)
- `Util.cpp` bytes=11245 sha256 `6271b7befc4a905ec3a1b7a66163502971c79981557b859f546cb08f3d53d395`  (https://raw.githubusercontent.com/wongddd/muonline/580472e0d5723f9709cbad594f233deb07f9f351/Source%20Server/GameServer/GameServer/Util.cpp)
- `stdafx.h` bytes=2590 sha256 `d23c35c3c250502e3777810827ed689f6c3c8dad5312ee6de3c34cd82a0cce0b`  (https://raw.githubusercontent.com/wongddd/muonline/580472e0d5723f9709cbad594f233deb07f9f351/Source%20Server/GameServer/GameServer/stdafx.h)
- `SocketManager.cpp` bytes=21153 sha256 `8492b66a5954d7b2a910dae36dae57cb2c80b40da7c9404fe93b69e0d7e64408`  (https://raw.githubusercontent.com/wongddd/muonline/580472e0d5723f9709cbad594f233deb07f9f351/Source%20Server/GameServer/GameServer/SocketManager.cpp)
- `SocketManagerModern.cpp` bytes=4859 sha256 `82f0629c5c0007fad9a9eb81e6d7eeff96e4e5caeb4523e2f2a5cc51101083cf`  (https://raw.githubusercontent.com/wongddd/muonline/580472e0d5723f9709cbad594f233deb07f9f351/Source%20Server/GameServer/GameServer/SocketManagerModern.cpp)
- `Protocol.h` bytes=36305 sha256 `943c59b7a457eccea4953b7410fd2140c6299979782b58ca81a7b42a52d6c3b0`  (https://raw.githubusercontent.com/wongddd/muonline/580472e0d5723f9709cbad594f233deb07f9f351/Source%20Server/GameServer/GameServer/Protocol.h)

---

## Util.h — declaração do DataSend global

- hit @ line 15: `bool DataSend(int aIndex,BYTE* lpMsg,DWORD size);`
```
     1: #pragma once
     2: 
     3: #include "ServerDisplayer.h"
     4: #include "User.h"
     5: 
     6: int SafeGetItem(int index);
     7: flt GetRoundValue(float value);
     8: BYTE GetNewOptionCount(BYTE NewOption);
     9: BYTE GetSocketOptionCount(BYTE SocketOption[5]);
    10: BYTE GetPathPacketDirPos(int px,int py);
    11: void PacketArgumentDecrypt(char* out_buff,char* in_buff,int size);
    12: void ErrorMessageBox(char* message,...);
    13: void LogAdd(eLogColor color,char* text,...);
    14: void LogAddConnect(eLogColor color,char* text,...);
    15: bool DataSend(int aIndex,BYTE* lpMsg,DWORD size);
    16: void DataSendAll(BYTE* lpMsg,int size);
    17: bool DataSendSocket(SOCKET socket,BYTE* lpMsg,DWORD size);
    18: void MsgSendV2(LPOBJ lpObj,BYTE* lpMsg,int size);
    19: void SendMonsterV2Msg(LPOBJ lpObj,LPBYTE lpMsg,int size);
    20: void CloseClient(int aIndex);
    21: void PostMessage1(char* name,char* message,char* text);
    22: void PostMessage2(char* name,char* message,char* text);
    23: void PostMessage3(char* name,char* message,char* text);
    24: void PostMessage4(char* name,char* message,char* text);
    25: void PostMessagePK(char* name,char* message,char* text);
    26: void PostMessageUserON(char* name,char* message);
    27: void PostMessageNew(char* name,char* message,char* text);
    28: void SetLargeRand();
    29: long GetLargeRand();
    30: 
    31: extern short RoadPathTable[MAX_ROAD_PATH_TABLE];
```

## Util.cpp — definição do DataSend global (roteador)

- first hit @ line 195: `bool DataSend(int aIndex,BYTE* lpMsg,DWORD size) // OK`
```
   115: 	char buff[256];
   116: 
   117: 	memset(buff,0,sizeof(buff));
   118: 
   119: 	va_list arg;
   120: 	va_start(arg,message);
   121: 	vsprintf_s(buff,message,arg);
   122: 	va_end(arg);
   123: 
   124: 	MessageBox(0,buff,"Error",MB_OK | MB_ICONERROR);
   125: 
   126: 	VM_END
   127: 
   128: 	ExitProcess(0);
   129: }
   130: 
   131: void LogAdd(eLogColor color,char* text,...) // OK
   132: {
   133: 	tm today;
   134: 	time_t ltime;
   135: 	time(&ltime);
   136: 
   137: 	if(localtime_s(&today,&ltime) != 0)
   138: 	{
   139: 		return;
   140: 	}
   141: 
   142: 	char time[32];
   143: 
   144: 	if(asctime_s(time,sizeof(time),&today) != 0)
   145: 	{
   146: 		return;
   147: 	}
   148: 
   149: 	char temp[1024];
   150: 
   151: 	va_list arg;
   152: 	va_start(arg,text);
   153: 	vsprintf_s(temp,text,arg);
   154: 	va_end(arg);
   155: 
   156: 	char log[1024];
   157: 
   158: 	wsprintf(log,"%.8s %s",&time[11],temp);
   159: 
   160: 	gServerDisplayer.LogAddText(color,log,strlen(log));
   161: }
   162: 
   163: void LogAddConnect(eLogColor color,char* text,...) // OK
   164: {
   165: 	tm today;
   166: 	time_t ltime;
   167: 	time(&ltime);
   168: 
   169: 	if(localtime_s(&today,&ltime) != 0)
   170: 	{
   171: 		return;
   172: 	}
   173: 
   174: 	char time[32];
   175: 
   176: 	if(asctime_s(time,sizeof(time),&today) != 0)
   177: 	{
   178: 		return;
   179: 	}
   180: 
   181: 	char temp[1024];
   182: 
   183: 	va_list arg;
   184: 	va_start(arg,text);
   185: 	vsprintf_s(temp,text,arg);
   186: 	va_end(arg);
   187: 
   188: 	char log[1024];
   189: 
   190: 	wsprintf(log,"%.8s %s",&time[11],temp);
   191: 
   192: 	gServerDisplayer.LogAddTextConnect(color,log,strlen(log));
   193: }
   194: 
   195: bool DataSend(int aIndex,BYTE* lpMsg,DWORD size) // OK
   196: {
   197: 	//BYTE head		= ((lpMsg[0]==0xC1 || lpMsg[0]==0xC3)?lpMsg[2]:lpMsg[3]);
   198: 	//BYTE subhead	= ((lpMsg[0]==0xC1 || lpMsg[0]==0xC3)?lpMsg[3]:lpMsg[4]);
   199: 
   200: 	//if((head != 0xF3 || subhead != 0xF1) && (head != 0xF3 || subhead != 0xE2) && head != 0x0D && head != 0x0E && head != 0x0F && head != 0x18 && head != PROTOCOL_CODE1)
   201: 	//{
   202: 	//	char String[8092];
   203: 	//	char hex_chars[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
   204: 
   205: 	//	for( int i = 0; i < (int)size; ++i )
   206: 	//	{
   207: 	//		sprintf(&String[2*i], "%02X", lpMsg[i]);
   208: 	//	}
   209: 
   210: 	//	if(strlen(String) <= 1024)
   211: 	//		LogAdd(LOG_RED,"PacketOUT: ID: %d T: %#X, H: %#04X, SH: %#04X, SIZE: %03d, PACKET: %s",aIndex,lpMsg[0],head,subhead,size,String);
   212: 	//}
   213: 
   214: 	#if (NEW_PROTOCOL_SYSTEM==1)
   215: 		gSocketManagerModern.PacketSend(aIndex, ProtocolHead::BOTH_MESSAGE, lpMsg,size);
   216: 		return 1;
   217: 	#else
   218: 		return gSocketManager.DataSend(aIndex,lpMsg,size);
   219: 	#endif
   220: }
   221: 
   222: void DataSendAll(BYTE* lpMsg,int size) // OK
   223: {
   224: 	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
   225: 	{
   226: 		if(gObjIsConnected(n) != 0)
   227: 		{
   228: 			DataSend(n,lpMsg,size);
   229: 		}
   230: 	}
   231: }
   232: 
   233: bool DataSendSocket(SOCKET socket,BYTE* lpMsg,DWORD size) // OK
   234: {
   235: 	if(socket == INVALID_SOCKET)
   236: 	{
   237: 		return 0;
   238: 	}
   239: 
   240: 	#if(ENCRYPT_STATE==1)
   241: 
   242: 	EncryptData(lpMsg,size);
   243: 
   244: 	#endif
   245: 
   246: 	int count=0,result=0;
   247: 
   248: 	while(size > 0)
   249: 	{
   250: 		if((result=send(socket,(char*)&lpMsg[count],size,0)) == SOCKET_ERROR)
   251: 		{
   252: 			if(WSAGetLastError() != WSAEWOULDBLOCK)
   253: 			{
   254: 				return 0;
   255: 			}
   256: 		}
   257: 		else
   258: 		{
   259: 			count += result;
   260: 			size -= result;
   261: 		}
   262: 	}
   263: 
   264: 	return 1;
   265: }
   266: 
   267: void MsgSendV2(LPOBJ lpObj,BYTE* lpMsg,int size) // OK
   268: {
   269: 	for(int n=0;n < MAX_VIEWPORT;n++)
   270: 	{
   271: 		if(lpObj->VpPlayer2[n].state != VIEWPORT_NONE && lpObj->VpPlayer2[n].type == OBJECT_USER && OBJECT_BOTS)
   272: 		{
   273: 			DataSend(lpObj->VpPlayer2[n].index,lpMsg,size);
   274: 		}
   275: 	}
   276: }
   277: 
   278: void SendMonsterV2Msg(LPOBJ lpObj, LPBYTE lpMsg, int size)
   279: {
   280:     for( int i=0; i< MAX_VIEWPORT; i++ )
   281:     {
   282:         if( lpObj->VpPlayer2[i].type == OBJECT_USER )
   283:         {
   284:             if( lpObj->VpPlayer2[i].state )
   285:             {
   286:                 if( (lpObj->Connected > 1) && (lpObj->Live) )
   287:                 {
   288:                     DataSend(lpObj->VpPlayer2[i].index, lpMsg, size);
   289:                 }
   290:                 else
   291:                 {
   292:                     lpObj->VpPlayer2[i].index = -1;
   293:                     lpObj->VpPlayer2[i].state  = VIEWPORT_NONE;
   294:                     lpObj->VPCount2--;
   295:                 }
   296:             }
   297:         }
   298: 
   299:     }
   300: }
   301: 
   302: void CloseClient(int aIndex) // OK
   303: {
   304: 	#if(NEW_PROTOCOL_SYSTEM==1)
   305: 		gSocketManagerModern.DisconenctClient(aIndex);
   306: 	#else
   307: 		gSocketManager.Disconnect(aIndex);
   308: 	#endif
   309: }
   310: 
   311: void PostMessage1(char* name,char* message,char* text) // OK
   312: {
   313: 	#if(GAMESERVER_UPDATE>=701)
   314: 
   315: 	char buff[256] = {0};
   316: 
   317: 	wsprintf(buff,message,name,text);
   318: 
   319: 	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
   320: 	{
   321: 		if(gObjIsConnectedGP(n) != 0)
   322: 		{
   323: 			GCNewMessageSend(&gObj[n],buff);
   324: 		}
   325: 	}
   326: 
   327: 	#else
   328: 
   329: 	char buff[256] = {0};
   330: 
   331: 	wsprintf(buff,message,text);
   332: 
   333: 	int size = strlen(buff);
   334: 
   335: 	size = ((size>MAX_CHAT_MESSAGE_SIZE)?MAX_CHAT_MESSAGE_SIZE:size);
   336: 
   337: 	PMSG_CHAT_WHISPER_SEND pMsg;
   338: 
   339: 	pMsg.header.set(0x02,(sizeof(pMsg)-(sizeof(pMsg.message)-(size+1))));
   340: 
   341: 	memcpy(pMsg.name,name,sizeof(pMsg.name));
   342: 
   343: 	memcpy(pMsg.message,buff,size);
   344: 
   345: 	pMsg.message[size] = 0;
   346: 
   347: 	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
   348: 	{
   349: 		if(gObjIsConnectedGP(n) != 0)
   350: 		{
   351: 			DataSend(n,(BYTE*)&pMsg,pMsg.header.size);
   352: 		}
   353: 	}
   354: 
   355: 	#endif
   356: }
   357: 
   358: void PostMessage2(char* name,char* message,char* text) // OK
   359: {
   360: 	#if(GAMESERVER_UPDATE>=701)
   361: 
   362: 	char buff[256] = {0};
   363: 
   364: 	wsprintf(buff,message,name,text);
   365: 
   366: 	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
   367: 	{
   368: 		if(gObjIsConnectedGP(n) != 0)
   369: 		{
   370: 			GCNewMessageSend(&gObj[n],buff);
   371: 		}
   372: 	}
   373: 
   374: 	#else
   375: 
   376: 	char buff[256] = {'~'};
   377: 
   378: 	wsprintf(&buff[1],message,text);
   379: 
   380: 	int size = strlen(buff);
   381: 
   382: 	size = ((size>MAX_CHAT_MESSAGE_SIZE)?MAX_CHAT_MESSAGE_SIZE:size);
   383: 
   384: 	PMSG_CHAT_SEND pMsg;
   385: 
   386: 	pMsg.header.set(0x00,(sizeof(pMsg)-(sizeof(pMsg.message)-(size+1))));
   387: 
   388: 	memcpy(pMsg.name,name,sizeof(pMsg.name));
   389: 
   390: 	memcpy(pMsg.message,buff,size);
   391: 
   392: 	pMsg.message[size] = 0;
   393: 
   394: 	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
   395: 	{
   396: 		if(gObjIsConnectedGP(n) != 0)
   397: 		{
   398: 			DataSend(n,(BYTE*)&pMsg,pMsg.header.size);
   399: 		}
   400: 	}
   401: 
   402: 	#endif
   403: }
   404: 
   405: void PostMessage3(char* name,char* message,char* text) // OK
   406: {
   407: 	#if(GAMESERVER_UPDATE>=701)
   408: 
   409: 	char buff[256] = {0};
   410: 
   411: 	wsprintf(buff,message,name,text);
   412: 
   413: 	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
   414: 	{
   415: 		if(gObjIsConnectedGP(n) != 0)
   416: 		{
   417: 			GCNewMessageSend(&gObj[n],buff);
   418: 		}
   419: 	}
   420: 
   421: 	#else
   422: 
   423: 	char buff[256] = {'@'};
   424: 
   425: 	wsprintf(&buff[1],message,text);
   426: 
   427: 	int size = strlen(buff);
   428: 
   429: 	size = ((size>MAX_CHAT_MESSAGE_SIZE)?MAX_CHAT_MESSAGE_SIZE:size);
   430: 
   431: 	PMSG_CHAT_SEND pMsg;
   432: 
   433: 	pMsg.header.set(0x00,(sizeof(pMsg)-(sizeof(pMsg.message)-(size+1))));
   434: 
   435: 	memcpy(pMsg.name,name,sizeof(pMsg.name));
   436: 
   437: 	memcpy(pMsg.message,buff,size);
   438: 
   439: 	pMsg.message[size] = 0;
   440: 
   441: 	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
   442: 	{
   443: 		if(gObjIsConnectedGP(n) != 0)
   444: 		{
   445: 			DataSend(n,(BYTE*)&pMsg,pMsg.header.size);
   446: 		}
   447: 	}
   448: 
   449: 	#endif
   450: }
   451: 
   452: void PostMessage4(char* name,char* message,char* text) // OK
   453: {
   454: 	#if(GAMESERVER_UPDATE>=701)
   455: 
   456: 	char buff[256] = {0};
   457: 
   458: 	wsprintf(buff,message,name,text);
   459: 
   460: 	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
   461: 	{
   462: 		if(gObjIsConnectedGP(n) != 0)
   463: 		{
   464: 			GCNewMessageSend(&gObj[n],buff);
   465: 		}
   466: 	}
   467: 
   468: 	#else
   469: 
   470: 	char buff[256] = {'$'};
   471: 
   472: 	wsprintf(&buff[1],message,text);
   473: 
   474: 	int size = strlen(buff);
   475: 
   476: 	size = ((size>MAX_CHAT_MESSAGE_SIZE)?MAX_CHAT_MESSAGE_SIZE:size);
   477: 
   478: 	PMSG_CHAT_SEND pMsg;
   479: 
   480: 	pMsg.header.set(0x00,(sizeof(pMsg)-(sizeof(pMsg.message)-(size+1))));
   481: 
   482: 	memcpy(pMsg.name,name,sizeof(pMsg.name));
   483: 
   484: 	memcpy(pMsg.message,buff,size);
   485: 
   486: 	pMsg.message[size] = 0;
   487: 
   488: 	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
   489: 	{
   490: 		if(gObjIsConnectedGP(n) != 0)
   491: 		{
   492: 			DataSend(n,(BYTE*)&pMsg,pMsg.header.size);
   493: 		}
   494: 	}
   495: 
   496: 	#endif
   497: }
   498: 
   499: void PostMessagePK(char* name,char* message,char* text) // OK
   500: {
   501: 	#if(GAMESERVER_UPDATE>=701)
   502: 
   503: 	char buff[256] = {0};
   504: 
   505: 	wsprintf(buff,message,name,text);
   506: 
   507: 	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
   508: 	{
   509: 		if(gObjIsConnectedGP(n) != 0)
   510: 		{
   511: 			GCNewMessageSend(&gObj[n],buff);
   512: 		}
   513: 	}
   514: 
   515: 	#else
```

## stdafx.h — defines de modo

### `#\s*define\s+NEW_PROTOCOL_SYSTEM\s+\d+`

- hit @ line 48: `#define NEW_PROTOCOL_SYSTEM 1`
```
     8: #define GAMESERVER_CLIENT "WWW.CREANDOMU.COM"
     9: 
    10: #define GAMESERVER_CLIENTE_UPDATE 15
    11: #define GAMESERVER_CLIENTE_PREMIUM 0
    12: 
    13: 
    14: #if(GAMESERVER_TYPE==0)
    15: #define GAMESERVER_VERSION "GS"
    16: #else
    17: #define GAMESERVER_VERSION "GSCS"
    18: #endif
    19: 
    20: #ifndef GAMESERVER_TYPE
    21: #define GAMESERVER_TYPE 0
    22: #endif
    23: 
    24: #ifndef GAMESERVER_EXTRA
    25: #define GAMESERVER_EXTRA 0
    26: #endif
    27: 
    28: #ifndef GAMESERVER_UPDATE
    29: #define GAMESERVER_UPDATE 502
    30: #endif
    31: 
    32: #if(GAMESERVER_UPDATE==401)
    33: #define GAMESERVER_SEASON "SEASON 4"
    34: #endif
    35: 
    36: #if(GAMESERVER_UPDATE==502)
    37: #define GAMESERVER_SEASON "SEASON 5"
    38: #endif
    39: 
    40: #if(GAMESERVER_UPDATE==603)
    41: #define GAMESERVER_SEASON "SEASON 6"
    42: #endif
    43: 
    44: #if(GAMESERVER_UPDATE==803)
    45: #define GAMESERVER_SEASON "SEASON 8"
    46: #endif
    47: 
    48: #define NEW_PROTOCOL_SYSTEM 1
    49: 
    50: //#ifndef GAMESERVER_LANGUAGE
    51: #define GAMESERVER_LANGUAGE 1
    52: //#endif
    53: 
    54: #ifndef PROTECT_STATE
    55: #define PROTECT_STATE 1
    56: #endif
    57: 
    58: #ifndef ENCRYPT_STATE
    59: #define ENCRYPT_STATE 0
    60: #endif
    61: 
    62: #if(PROTECT_STATE==0)
    63: #define GAMESERVER_NAME "Free"
    64: #else
    65: #define GAMESERVER_NAME "Premium"
    66: #endif
    67: 
    68: //MC bot
    69: 
    70: //----------------------------
    71: //	 ZG-Defines			     -
    72: //	 1:ON  (CPP AND HEADER!) -
    73: //	 2:OFF (CPP AND HEADER!) -
    74: //----------------------------
    75: 
    76: #ifndef POINT_CUSTOM
    77: #define POINT_CUSTOM 1 // ShopPointEx
    78: #endif
    79: 
    80: #define PC_POINT			 2 //Agregar algun npc o bot con PCP
    81: #define HAPPY_HOURS			 2 //[80%]->Falta Arreglar Los Vip!
    82: #define MASTER_HOURS		 2
    83: #define ACHERON_GUARDIAN	 2
    84: 
    85: #define BOT_BUFFER			 1
    86: 
    87: #define ARCA_WAR			 2	// Mix nomas!!
    88: #define GOLDEN_ARCHER		 2	//
    89: 
    90: 
    91: //-- Need All Types
    92: #define PG_CUSTOM			 1
    93: #define	ALLBOTSSTRUC		 1
    94: 
    95: //MC bot
    96: 
    97: // System Include
    98: #include <windows.h>
    99: #include <winsock2.h>
   100: #include <mswSock.h>
   101: #include <commctrl.h>
   102: #include <iostream>
   103: #include <stdlib.h>
   104: #include <time.h>
   105: #include <math.h>
   106: #include <map>
   107: #include <vector>
   108: #include <queue>
   109: #include <random>
   110: #include <Rpc.h>
   111: #include <algorithm>
   112: #include <string>
   113: #include <atltime.h>
   114: #include <dbghelp.h>
   115: #include <Psapi.h>
   116: #include <csignal>
   117: #include <thread>
   118: 
   119: #pragma comment(lib,"ws2_32.lib")
   120: #pragma comment(lib,"Rpcrt4.lib")
   121: #pragma comment(lib,"dbghelp.lib")
   122: #pragma comment(lib,"Psapi.lib")
   123: 
   124: #if(GAMESERVER_UPDATE>=701)
   125: #if(NDEBUG==0)
   126: #pragma comment(lib,"Util\\cryptopp\\Debug\\cryptlib.lib")
   127: #else
   128: #pragma comment(lib,"Util\\cryptopp\\Release\\cryptlib.lib")
   129: #endif
   130: #pragma comment(lib,"Util\\mapm\\mapm.lib")
   131: #endif
   132: 
   133: typedef char chr;
   134: 
   135: typedef float flt;
   136: 
   137: typedef short shrt;
   138: 
   139: typedef unsigned __int64 QWORD;
   140: 
   141: __pragma(warning(disable:28159))
   142: __pragma(warning(disable:26495))
```

### `#\s*define\s+ENCRYPT_STATE\s+\d+`

- hit @ line 59: `#define ENCRYPT_STATE 0`
```
    19: 
    20: #ifndef GAMESERVER_TYPE
    21: #define GAMESERVER_TYPE 0
    22: #endif
    23: 
    24: #ifndef GAMESERVER_EXTRA
    25: #define GAMESERVER_EXTRA 0
    26: #endif
    27: 
    28: #ifndef GAMESERVER_UPDATE
    29: #define GAMESERVER_UPDATE 502
    30: #endif
    31: 
    32: #if(GAMESERVER_UPDATE==401)
    33: #define GAMESERVER_SEASON "SEASON 4"
    34: #endif
    35: 
    36: #if(GAMESERVER_UPDATE==502)
    37: #define GAMESERVER_SEASON "SEASON 5"
    38: #endif
    39: 
    40: #if(GAMESERVER_UPDATE==603)
    41: #define GAMESERVER_SEASON "SEASON 6"
    42: #endif
    43: 
    44: #if(GAMESERVER_UPDATE==803)
    45: #define GAMESERVER_SEASON "SEASON 8"
    46: #endif
    47: 
    48: #define NEW_PROTOCOL_SYSTEM 1
    49: 
    50: //#ifndef GAMESERVER_LANGUAGE
    51: #define GAMESERVER_LANGUAGE 1
    52: //#endif
    53: 
    54: #ifndef PROTECT_STATE
    55: #define PROTECT_STATE 1
    56: #endif
    57: 
    58: #ifndef ENCRYPT_STATE
    59: #define ENCRYPT_STATE 0
    60: #endif
    61: 
    62: #if(PROTECT_STATE==0)
    63: #define GAMESERVER_NAME "Free"
    64: #else
    65: #define GAMESERVER_NAME "Premium"
    66: #endif
    67: 
    68: //MC bot
    69: 
    70: //----------------------------
    71: //	 ZG-Defines			     -
    72: //	 1:ON  (CPP AND HEADER!) -
    73: //	 2:OFF (CPP AND HEADER!) -
    74: //----------------------------
    75: 
    76: #ifndef POINT_CUSTOM
    77: #define POINT_CUSTOM 1 // ShopPointEx
    78: #endif
    79: 
    80: #define PC_POINT			 2 //Agregar algun npc o bot con PCP
    81: #define HAPPY_HOURS			 2 //[80%]->Falta Arreglar Los Vip!
    82: #define MASTER_HOURS		 2
    83: #define ACHERON_GUARDIAN	 2
    84: 
    85: #define BOT_BUFFER			 1
    86: 
    87: #define ARCA_WAR			 2	// Mix nomas!!
    88: #define GOLDEN_ARCHER		 2	//
    89: 
    90: 
    91: //-- Need All Types
    92: #define PG_CUSTOM			 1
    93: #define	ALLBOTSSTRUC		 1
    94: 
    95: //MC bot
    96: 
    97: // System Include
    98: #include <windows.h>
    99: #include <winsock2.h>
   100: #include <mswSock.h>
   101: #include <commctrl.h>
   102: #include <iostream>
   103: #include <stdlib.h>
   104: #include <time.h>
   105: #include <math.h>
   106: #include <map>
   107: #include <vector>
   108: #include <queue>
   109: #include <random>
   110: #include <Rpc.h>
   111: #include <algorithm>
   112: #include <string>
   113: #include <atltime.h>
   114: #include <dbghelp.h>
   115: #include <Psapi.h>
   116: #include <csignal>
   117: #include <thread>
   118: 
   119: #pragma comment(lib,"ws2_32.lib")
   120: #pragma comment(lib,"Rpcrt4.lib")
   121: #pragma comment(lib,"dbghelp.lib")
   122: #pragma comment(lib,"Psapi.lib")
   123: 
   124: #if(GAMESERVER_UPDATE>=701)
   125: #if(NDEBUG==0)
   126: #pragma comment(lib,"Util\\cryptopp\\Debug\\cryptlib.lib")
   127: #else
   128: #pragma comment(lib,"Util\\cryptopp\\Release\\cryptlib.lib")
   129: #endif
   130: #pragma comment(lib,"Util\\mapm\\mapm.lib")
   131: #endif
   132: 
   133: typedef char chr;
   134: 
   135: typedef float flt;
   136: 
   137: typedef short shrt;
   138: 
   139: typedef unsigned __int64 QWORD;
   140: 
   141: __pragma(warning(disable:28159))
   142: __pragma(warning(disable:26495))
```

### `#\s*define\s+GAMESERVER_UPDATE\s+\d+`

- hit @ line 29: `#define GAMESERVER_UPDATE 502`
```
     1: #pragma once
     2: 
     3: #define WIN32_LEAN_AND_MEAN
     4: 
     5: #define _WIN32_WINNT _WIN32_WINNT_WINXP
     6: 
     7: 
     8: #define GAMESERVER_CLIENT "WWW.CREANDOMU.COM"
     9: 
    10: #define GAMESERVER_CLIENTE_UPDATE 15
    11: #define GAMESERVER_CLIENTE_PREMIUM 0
    12: 
    13: 
    14: #if(GAMESERVER_TYPE==0)
    15: #define GAMESERVER_VERSION "GS"
    16: #else
    17: #define GAMESERVER_VERSION "GSCS"
    18: #endif
    19: 
    20: #ifndef GAMESERVER_TYPE
    21: #define GAMESERVER_TYPE 0
    22: #endif
    23: 
    24: #ifndef GAMESERVER_EXTRA
    25: #define GAMESERVER_EXTRA 0
    26: #endif
    27: 
    28: #ifndef GAMESERVER_UPDATE
    29: #define GAMESERVER_UPDATE 502
    30: #endif
    31: 
    32: #if(GAMESERVER_UPDATE==401)
    33: #define GAMESERVER_SEASON "SEASON 4"
    34: #endif
    35: 
    36: #if(GAMESERVER_UPDATE==502)
    37: #define GAMESERVER_SEASON "SEASON 5"
    38: #endif
    39: 
    40: #if(GAMESERVER_UPDATE==603)
    41: #define GAMESERVER_SEASON "SEASON 6"
    42: #endif
    43: 
    44: #if(GAMESERVER_UPDATE==803)
    45: #define GAMESERVER_SEASON "SEASON 8"
    46: #endif
    47: 
    48: #define NEW_PROTOCOL_SYSTEM 1
    49: 
    50: //#ifndef GAMESERVER_LANGUAGE
    51: #define GAMESERVER_LANGUAGE 1
    52: //#endif
    53: 
    54: #ifndef PROTECT_STATE
    55: #define PROTECT_STATE 1
    56: #endif
    57: 
    58: #ifndef ENCRYPT_STATE
    59: #define ENCRYPT_STATE 0
    60: #endif
    61: 
    62: #if(PROTECT_STATE==0)
    63: #define GAMESERVER_NAME "Free"
    64: #else
    65: #define GAMESERVER_NAME "Premium"
    66: #endif
    67: 
    68: //MC bot
    69: 
    70: //----------------------------
    71: //	 ZG-Defines			     -
    72: //	 1:ON  (CPP AND HEADER!) -
    73: //	 2:OFF (CPP AND HEADER!) -
    74: //----------------------------
    75: 
    76: #ifndef POINT_CUSTOM
    77: #define POINT_CUSTOM 1 // ShopPointEx
    78: #endif
    79: 
    80: #define PC_POINT			 2 //Agregar algun npc o bot con PCP
    81: #define HAPPY_HOURS			 2 //[80%]->Falta Arreglar Los Vip!
    82: #define MASTER_HOURS		 2
    83: #define ACHERON_GUARDIAN	 2
    84: 
    85: #define BOT_BUFFER			 1
    86: 
    87: #define ARCA_WAR			 2	// Mix nomas!!
    88: #define GOLDEN_ARCHER		 2	//
    89: 
    90: 
    91: //-- Need All Types
    92: #define PG_CUSTOM			 1
    93: #define	ALLBOTSSTRUC		 1
    94: 
    95: //MC bot
    96: 
    97: // System Include
    98: #include <windows.h>
    99: #include <winsock2.h>
   100: #include <mswSock.h>
   101: #include <commctrl.h>
   102: #include <iostream>
   103: #include <stdlib.h>
   104: #include <time.h>
   105: #include <math.h>
   106: #include <map>
   107: #include <vector>
   108: #include <queue>
   109: #include <random>
   110: #include <Rpc.h>
   111: #include <algorithm>
   112: #include <string>
   113: #include <atltime.h>
   114: #include <dbghelp.h>
   115: #include <Psapi.h>
   116: #include <csignal>
   117: #include <thread>
   118: 
   119: #pragma comment(lib,"ws2_32.lib")
   120: #pragma comment(lib,"Rpcrt4.lib")
   121: #pragma comment(lib,"dbghelp.lib")
   122: #pragma comment(lib,"Psapi.lib")
   123: 
   124: #if(GAMESERVER_UPDATE>=701)
   125: #if(NDEBUG==0)
   126: #pragma comment(lib,"Util\\cryptopp\\Debug\\cryptlib.lib")
   127: #else
   128: #pragma comment(lib,"Util\\cryptopp\\Release\\cryptlib.lib")
   129: #endif
   130: #pragma comment(lib,"Util\\mapm\\mapm.lib")
   131: #endif
   132: 
   133: typedef char chr;
   134: 
   135: typedef float flt;
   136: 
   137: typedef short shrt;
   138: 
   139: typedef unsigned __int64 QWORD;
   140: 
   141: __pragma(warning(disable:28159))
   142: __pragma(warning(disable:26495))
```

## SocketManager.cpp — path legado (C1/C2 vs C3/C4)

### `#\s*if\s*\(\s*NEW_PROTOCOL_SYSTEM\s*==\s*0\s*\)`

- hit @ line 7: `#if(NEW_PROTOCOL_SYSTEM==0)`
```
     1: //////////////////////////////////////////////////////////////////////
     2: // SocketManager.cpp: implementation of the CSocketManager class.
     3: //////////////////////////////////////////////////////////////////////
     4: 
     5: #include "stdafx.h"
     6: 
     7: #if(NEW_PROTOCOL_SYSTEM==0)
     8: 
     9: #include "SocketManager.h"
    10: #include "HackCheck.h"
    11: #include "IpManager.h"
    12: #include "Log.h"
    13: #include "PacketManager.h"
    14: #include "Protect.h"
    15: #include "Protocol.h"
    16: #include "SerialCheck.h"
    17: #include "User.h"
    18: #include "Util.h"
    19: 
    20: CSocketManager gSocketManager;
    21: 
    22: CSocketManager::CSocketManager() // OK
    23: {
    24: 	this->m_listen = INVALID_SOCKET;
    25: 
    26: 	this->m_CompletionPort = 0;
    27: 
    28: 	this->m_port = 0;
    29: 
    30: 	this->m_ServerAcceptThread = 0;
    31: 
    32: 	for(int n=0;n < MAX_SERVER_WORKER_THREAD;n++)
    33: 	{
    34: 		this->m_ServerWorkerThread[n] = 0;
    35: 	}
    36: 
    37: 	this->m_ServerWorkerThreadCount = 0;
    38: 
    39: 	this->m_ServerQueueSemaphore = 0;
    40: 
    41: 	this->m_ServerQueueThread = 0;
    42: }
    43: 
    44: CSocketManager::~CSocketManager() // OK
    45: {
    46: 	this->Clean();
    47: }
    48: 
    49: bool CSocketManager::Start(WORD port) // OK
    50: {
    51: 	PROTECT_START
    52: 
    53: 	this->m_port = port;
    54: 
    55: 	if(this->CreateListenSocket() == 0)
    56: 	{
    57: 		this->Clean();
    58: 		return 0;
    59: 	}
    60: 
    61: 	if(this->CreateCompletionPort() == 0)
    62: 	{
    63: 		this->Clean();
    64: 		return 0;
    65: 	}
    66: 
    67: 	if(this->CreateAcceptThread() == 0)
    68: 	{
    69: 		this->Clean();
    70: 		return 0;
    71: 	}
    72: 
    73: 	if(this->CreateWorkerThread() == 0)
    74: 	{
    75: 		this->Clean();
    76: 		return 0;
    77: 	}
    78: 
    79: 	if(this->CreateServerQueue() == 0)
    80: 	{
    81: 		this->Clean();
    82: 		return 0;
    83: 	}
    84: 
    85: 	PROTECT_FINAL
    86: 
    87: 	gLog.Output(LOG_CONNECT,"[SocketManager] Server started at port [%d]",this->m_port);
    88: 
    89: 	return 1;
    90: }
    91: 
    92: void CSocketManager::Clean() // OK
    93: {
    94: 	if(this->m_ServerQueueThread != 0)
    95: 	{
    96: 		TerminateThread(this->m_ServerQueueThread,0);
    97: 		CloseHandle(this->m_ServerQueueThread);
    98: 		this->m_ServerQueueThread = 0;
    99: 	}
   100: 
   101: 	if(this->m_ServerQueueSemaphore != 0)
   102: 	{
   103: 		CloseHandle(this->m_ServerQueueSemaphore);
   104: 		this->m_ServerQueueSemaphore = 0;
   105: 	}
   106: 
   107: 	this->m_ServerQueue.ClearQueue();
   108: 
   109: 	for(DWORD n=0;n < MAX_SERVER_WORKER_THREAD;n++)
   110: 	{
   111: 		if(this->m_ServerWorkerThread[n] != 0)
   112: 		{
   113: 			TerminateThread(this->m_ServerWorkerThread[n],0);
   114: 			CloseHandle(this->m_ServerWorkerThread[n]);
   115: 			this->m_ServerWorkerThread[n] = 0;
   116: 		}
   117: 	}
   118: 
   119: 	if(this->m_ServerAcceptThread != 0)
   120: 	{
   121: 		TerminateThread(this->m_ServerAcceptThread,0);
   122: 		CloseHandle(this->m_ServerAcceptThread);
   123: 		this->m_ServerAcceptThread = 0;
   124: 	}
   125: 
   126: 	if(this->m_CompletionPort != 0)
   127: 	{
   128: 		CloseHandle(this->m_CompletionPort);
   129: 		this->m_CompletionPort = 0;
   130: 	}
   131: 
   132: 	if(this->m_listen != INVALID_SOCKET)
   133: 	{
   134: 		closesocket(this->m_listen);
   135: 		this->m_listen = INVALID_SOCKET;
   136: 	}
   137: }
   138: 
   139: bool CSocketManager::CreateListenSocket() // OK
   140: {
   141: 	if((this->m_listen=WSASocket(AF_INET,SOCK_STREAM,0,0,0,WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
   142: 	{
   143: 		gLog.Output(LOG_CONNECT,"[SocketManager] WSASocket() failed with error: %d",WSAGetLastError());
   144: 		return 0;
   145: 	}
   146: 
   147: 	SOCKADDR_IN SocketAddr;
   148: 
   149: 	SocketAddr.sin_family = AF_INET;
   150: 	SocketAddr.sin_addr.s_addr = htonl(0);
   151: 	SocketAddr.sin_port = htons(this->m_port);
   152: 
   153: 	if(bind(this->m_listen,(sockaddr*)&SocketAddr,sizeof(SocketAddr)) == SOCKET_ERROR)
   154: 	{
   155: 		gLog.Output(LOG_CONNECT,"[SocketManager] bind() failed with error: %d",WSAGetLastError());
   156: 		return 0;
   157: 	}
   158: 
   159: 	if(listen(this->m_listen,5) == SOCKET_ERROR)
   160: 	{
   161: 		gLog.Output(LOG_CONNECT,"[SocketManager] listen() failed with error: %d",WSAGetLastError());
   162: 		return 0;
   163: 	}
   164: 
   165: 	return 1;
   166: }
   167: 
   168: bool CSocketManager::CreateCompletionPort() // OK
   169: {
   170: 	SOCKET socket = ::socket(AF_INET,SOCK_STREAM,IPPROTO_IP);
   171: 
   172: 	if(socket == INVALID_SOCKET)
   173: 	{
   174: 		gLog.Output(LOG_CONNECT,"[SocketManager] socket() failed with error: %d",WSAGetLastError());
   175: 		return 0;
   176: 	}
   177: 
   178: 	if((this->m_CompletionPort=CreateIoCompletionPort((HANDLE)socket,0,0,0)) == 0)
   179: 	{
   180: 		gLog.Output(LOG_CONNECT,"[SocketManager] CreateIoCompletionPort() failed with error: %d",GetLastError());
   181: 		closesocket(socket);
   182: 		return 0;
   183: 	}
   184: 
   185: 	closesocket(socket);
   186: 	return 1;
   187: }
   188: 
   189: bool CSocketManager::CreateAcceptThread() // OK
   190: {
   191: 	if((this->m_ServerAcceptThread=CreateThread(0,0,(LPTHREAD_START_ROUTINE)this->ServerAcceptThread,this,0,0)) == 0)
   192: 	{
   193: 		gLog.Output(LOG_CONNECT,"[SocketManager] CreateThread() failed with error: %d",GetLastError());
   194: 		return 0;
   195: 	}
   196: 
   197: 	if(SetThreadPriority(this->m_ServerAcceptThread,THREAD_PRIORITY_HIGHEST) == 0)
   198: 	{
   199: 		gLog.Output(LOG_CONNECT,"[SocketManager] SetThreadPriority() failed with error: %d",GetLastError());
   200: 		return 0;
   201: 	}
   202: 
   203: 	return 1;
   204: }
   205: 
   206: bool CSocketManager::CreateWorkerThread() // OK
   207: {
   208: 	SYSTEM_INFO SystemInfo;
   209: 
   210: 	GetSystemInfo(&SystemInfo);
   211: 
   212: 	this->m_ServerWorkerThreadCount = ((SystemInfo.dwNumberOfProcessors>MAX_SERVER_WORKER_THREAD)?MAX_SERVER_WORKER_THREAD:SystemInfo.dwNumberOfProcessors);
   213: 
   214: 	for(DWORD n=0;n < this->m_ServerWorkerThreadCount;n++)
   215: 	{
   216: 		if((this->m_ServerWorkerThread[n]=CreateThread(0,0,(LPTHREAD_START_ROUTINE)this->ServerWorkerThread,this,0,0)) == 0)
   217: 		{
   218: 			gLog.Output(LOG_CONNECT,"[SocketManager] CreateThread() failed with error: %d",GetLastError());
   219: 			return 0;
   220: 		}
   221: 
   222: 		if(SetThreadPriority(this->m_ServerWorkerThread[n],THREAD_PRIORITY_HIGHEST) == 0)
   223: 		{
   224: 			gLog.Output(LOG_CONNECT,"[SocketManager] SetThreadPriority() failed with error: %d",GetLastError());
   225: 			return 0;
   226: 		}
   227: 	}
   228: 
   229: 	return 1;
   230: }
   231: 
   232: bool CSocketManager::CreateServerQueue() // OK
   233: {
   234: 	if((this->m_ServerQueueSemaphore=CreateSemaphore(0,0,MAX_QUEUE_SIZE,0)) == 0)
   235: 	{
   236: 		gLog.Output(LOG_CONNECT,"[SocketManager] CreateSemaphore() failed with error: %d",GetLastError());
   237: 		return 0;
   238: 	}
   239: 
   240: 	if((this->m_ServerQueueThread=CreateThread(0,0,(LPTHREAD_START_ROUTINE)this->ServerQueueThread,this,0,0)) == 0)
   241: 	{
   242: 		gLog.Output(LOG_CONNECT,"[SocketManager] CreateThread() failed with error: %d",GetLastError());
   243: 		return 0;
   244: 	}
   245: 
   246: 	if(SetThreadPriority(this->m_ServerQueueThread,THREAD_PRIORITY_HIGHEST) == 0)
   247: 	{
   248: 		gLog.Output(LOG_CONNECT,"[SocketManager] SetThreadPriority() failed with error: %d",GetLastError());
   249: 		return 0;
   250: 	}
   251: 
   252: 	return 1;
   253: }
   254: 
   255: bool CSocketManager::DataRecv(int index,IO_MAIN_BUFFER* lpIoBuffer) // OK
   256: {
   257: 	if(lpIoBuffer->size < 3)
   258: 	{
   259: 		return 1;
   260: 	}
   261: 
   262: 	BYTE* lpMsg = lpIoBuffer->buff;
   263: 
   264: 	int count=0,size=0,DecSize=0,DecEncrypt=0,DecSerial=0;
   265: 	static BYTE DecBuff[MAX_MAIN_PACKET_SIZE];
   266: 	static QUEUE_INFO QueueInfo;
   267: 	BYTE header,head;
```

### `gPacketManager\.Encrypt|EncryptData|SimpleModulus|gSerialCheck|g_byPacketSerialSend`

- hit @ line 449: `			lpMsg[1] = gSerialCheck[index].GetSendSerial();`
```
   379: 
   380: 				memcpy(QueueInfo.buff,DecBuff,size);
   381: 
   382: 				QueueInfo.size = size;
   383: 
   384: 				QueueInfo.encrypt = 0;
   385: 
   386: 				QueueInfo.serial = -1;
   387: 
   388: 				if(this->m_ServerQueue.AddToQueue(&QueueInfo) != 0)
   389: 				{
   390: 					ReleaseSemaphore(this->m_ServerQueueSemaphore,1,0);
   391: 				}
   392: 			}
   393: 
   394: 			count += size;
   395: 
   396: 			lpIoBuffer->size -= size;
   397: 
   398: 			if(lpIoBuffer->size <= 0)
   399: 			{
   400: 				break;
   401: 			}
   402: 		}
   403: 		else
   404: 		{
   405: 			if(count > 0 && lpIoBuffer->size > 0 && lpIoBuffer->size <= (MAX_MAIN_PACKET_SIZE-count))
   406: 			{
   407: 				memmove(lpMsg,&lpMsg[count],lpIoBuffer->size);
   408: 			}
   409: 
   410: 			break;
   411: 		}
   412: 	}
   413: 
   414: 	return 1;
   415: }
   416: 
   417: bool CSocketManager::DataSend(int index,BYTE* lpMsg,int size) // OK
   418: {
   419: 	this->m_critical.lock();
   420: 
   421: 	if(OBJECT_USER_RANGE(index) == 0)
   422: 	{
   423: 		this->m_critical.unlock();
   424: 		return 0;
   425: 	}
   426: 
   427: 	if(gObj[index].Socket == INVALID_SOCKET)
   428: 	{
   429: 		this->m_critical.unlock();
   430: 		return 0;
   431: 	}
   432: 
   433: 	if(gObj[index].Connected == OBJECT_OFFLINE)
   434: 	{
   435: 		this->m_critical.unlock();
   436: 		return 0;
   437: 	}
   438: 
   439: 	static BYTE send[MAX_MAIN_PACKET_SIZE];
   440: 
   441: 	memcpy(send,lpMsg,size);
   442: 
   443: 	if(lpMsg[0] == 0xC3 || lpMsg[0] == 0xC4)
   444: 	{
   445: 		if(lpMsg[0] == 0xC3)
   446: 		{
   447: 			BYTE save = lpMsg[1];
   448: 
   449: 			lpMsg[1] = gSerialCheck[index].GetSendSerial();
   450: 
   451: 			size = gPacketManager.Encrypt(&send[2],&lpMsg[1],(size-1))+2;
   452: 
   453: 			lpMsg[1] = save;
   454: 
   455: 			send[0] = 0xC3;
   456: 			send[1] = size;
   457: 		}
   458: 		else
   459: 		{
   460: 			BYTE save = lpMsg[2];
   461: 
   462: 			lpMsg[2] = gSerialCheck[index].GetSendSerial();
   463: 
   464: 			size = gPacketManager.Encrypt(&send[3],&lpMsg[2],(size-2))+3;
   465: 
   466: 			lpMsg[2] = save;
   467: 
   468: 			send[0] = 0xC4;
   469: 			send[1] = HIBYTE(size);
   470: 			send[2] = LOBYTE(size);
   471: 		}
   472: 	}
   473: 
   474: 	if(size > MAX_MAIN_PACKET_SIZE)
   475: 	{
   476: 		gLog.Output(LOG_CONNECT,"[SocketManager] Max msg size (Type: 1, Index: %d, Size: %d)",index,size);
   477: 		this->Disconnect(index);
   478: 		this->m_critical.unlock();
   479: 		return 0;
   480: 	}
   481: 
   482: 	#if(ENCRYPT_STATE==1)
   483: 
   484: 	EncryptData(send,size);
   485: 
   486: 	#endif
   487: 
   488: 	IO_SEND_CONTEXT* lpIoContext = &gObj[index].PerSocketContext->IoSendContext;
   489: 
   490: 	if(lpIoContext->IoSize > 0)
   491: 	{
   492: 		if((lpIoContext->IoSideBuffer.size+size) > MAX_SIDE_PACKET_SIZE)
   493: 		{
   494: 			gLog.Output(LOG_CONNECT,"[SocketManager] Max msg size (Type: 2, Index: %d, Size: %d)",index,(lpIoContext->IoSideBuffer.size+size));
   495: 			this->Disconnect(index);
   496: 			this->m_critical.unlock();
   497: 			return 0;
   498: 		}
   499: 
   500: 		memcpy(&lpIoContext->IoSideBuffer.buff[lpIoContext->IoSideBuffer.size],send,size);
   501: 		lpIoContext->IoSideBuffer.size += size;
   502: 		this->m_critical.unlock();
   503: 		return 1;
   504: 	}
   505: 
   506: 	memcpy(lpIoContext->IoMainBuffer.buff,send,size);
   507: 
   508: 	lpIoContext->wsabuf.buf = (char*)lpIoContext->IoMainBuffer.buff;
   509: 
   510: 	lpIoContext->wsabuf.len = size;
   511: 
   512: 	lpIoContext->IoType = IO_SEND;
   513: 
   514: 	lpIoContext->IoSize = size;
   515: 
   516: 	lpIoContext->IoMainBuffer.size = 0;
   517: 
   518: 	DWORD SendSize=0,Flags=0;
   519: 
   520: 	if(WSASend(gObj[index].Socket,&lpIoContext->wsabuf,1,&SendSize,Flags,&lpIoContext->overlapped,0) == SOCKET_ERROR)
   521: 	{
   522: 		if(WSAGetLastError() != WSA_IO_PENDING)
   523: 		{
   524: 			gLog.Output(LOG_CONNECT,"[SocketManager] WSASend() failed with error: %d",WSAGetLastError());
   525: 			this->Disconnect(index);
   526: 			this->m_critical.unlock();
   527: 			return 0;
   528: 		}
   529: 	}
   530: 
   531: 	this->m_critical.unlock();
   532: 	return 1;
   533: }
   534: 
   535: void CSocketManager::Disconnect(int index) // OK
   536: {
   537: 	this->m_critical.lock();
   538: 
   539: 	if(OBJECT_USER_RANGE(index) == 0)
   540: 	{
   541: 		this->m_critical.unlock();
   542: 		return;
   543: 	}
   544: 
   545: 	if(gObj[index].Socket == INVALID_SOCKET)
   546: 	{
   547: 		this->m_critical.unlock();
   548: 		return;
   549: 	}
   550: 
   551: 	if(gObj[index].Connected == OBJECT_OFFLINE)
   552: 	{
   553: 		this->m_critical.unlock();
   554: 		return;
   555: 	}
   556: 
   557: 	if(closesocket(gObj[index].Socket) == SOCKET_ERROR && WSAGetLastError() != WSAENOTSOCK)
   558: 	{
   559: 		gLog.Output(LOG_CONNECT,"[SocketManager] closesocket() failed with error: %d",WSAGetLastError());
   560: 		this->m_critical.unlock();
   561: 		return;
   562: 	}
   563: 
   564: 	gObj[index].Socket = INVALID_SOCKET;
   565: 
   566: 	gObjDel(index);
   567: 
   568: 	this->m_critical.unlock();
   569: }
   570: 
   571: void CSocketManager::OnRecv(int index,DWORD IoSize,IO_RECV_CONTEXT* lpIoContext) // OK
   572: {
   573: 	this->m_critical.lock();
   574: 
   575: 	if(OBJECT_USER_RANGE(index) == 0)
   576: 	{
   577: 		this->m_critical.unlock();
   578: 		return;
   579: 	}
   580: 
   581: 	if(IoSize == 0)
   582: 	{
   583: 		this->Disconnect(index);
   584: 		this->m_critical.unlock();
   585: 		return;
   586: 	}
   587: 
   588: 	LPOBJ lpObj = &gObj[index];
   589: 
   590: 	#if(ENCRYPT_STATE==1)
   591: 
   592: 	DecryptData(&lpIoContext->IoMainBuffer.buff[lpIoContext->IoMainBuffer.size],IoSize);
   593: 
   594: 	#endif
   595: 
   596: 	lpIoContext->IoMainBuffer.size += IoSize;
   597: 
   598: 	if(this->DataRecv(index,&lpIoContext->IoMainBuffer) == 0)
   599: 	{
   600: 		this->Disconnect(index);
   601: 		this->m_critical.unlock();
   602: 		return;
   603: 	}
   604: 
   605: 	lpIoContext->wsabuf.buf = (char*)&lpIoContext->IoMainBuffer.buff[lpIoContext->IoMainBuffer.size];
   606: 
   607: 	lpIoContext->wsabuf.len = MAX_MAIN_PACKET_SIZE-lpIoContext->IoMainBuffer.size;
   608: 
   609: 	lpIoContext->IoType = IO_RECV;
   610: 
   611: 	DWORD RecvSize=0,Flags=0;
   612: 
   613: 	if(WSARecv(lpObj->Socket,&lpIoContext->wsabuf,1,&RecvSize,&Flags,&lpIoContext->overlapped,0) == SOCKET_ERROR)
   614: 	{
   615: 		if(WSAGetLastError() != WSA_IO_PENDING)
   616: 		{
   617: 			gLog.Output(LOG_CONNECT,"[SocketManager] WSARecv() failed with error: %d",WSAGetLastError());
   618: 			this->Disconnect(index);
   619: 			this->m_critical.unlock();
   620: 			return;
   621: 		}
   622: 	}
   623: 
   624: 	this->m_critical.unlock();
   625: }
   626: 
   627: void CSocketManager::OnSend(int index,DWORD IoSize,IO_SEND_CONTEXT* lpIoContext) // OK
   628: {
   629: 	this->m_critical.lock();
   630: 
   631: 	if(OBJECT_USER_RANGE(index) == 0)
   632: 	{
   633: 		this->m_critical.unlock();
   634: 		return;
   635: 	}
   636: 
   637: 	if(IoSize == 0)
   638: 	{
   639: 		this->Disconnect(index);
   640: 		this->m_critical.unlock();
   641: 		return;
   642: 	}
   643: 
   644: 	LPOBJ lpObj = &gObj[index];
   645: 
   646: 	lpIoContext->IoMainBuffer.size += IoSize;
   647: 
   648: 	if(lpIoContext->IoMainBuffer.size >= lpIoContext->IoSize)
   649: 	{
   650: 		if(lpIoContext->IoSideBuffer.size <= 0)
   651: 		{
   652: 			lpIoContext->IoSize = 0;
   653: 			this->m_critical.unlock();
   654: 			return;
   655: 		}
   656: 
   657: 		if(lpIoContext->IoSideBuffer.size > MAX_MAIN_PACKET_SIZE)
   658: 		{
   659: 			memcpy(lpIoContext->IoMainBuffer.buff,lpIoContext->IoSideBuffer.buff,MAX_MAIN_PACKET_SIZE);
   660: 
   661: 			lpIoContext->wsabuf.buf = (char*)lpIoContext->IoMainBuffer.buff;
   662: 
   663: 			lpIoContext->wsabuf.len = MAX_MAIN_PACKET_SIZE;
   664: 
   665: 			lpIoContext->IoType = IO_SEND;
   666: 
   667: 			lpIoContext->IoSize = MAX_MAIN_PACKET_SIZE;
   668: 
   669: 			lpIoContext->IoMainBuffer.size = 0;
   670: 
   671: 			memmove(lpIoContext->IoSideBuffer.buff,&lpIoContext->IoSideBuffer.buff[MAX_MAIN_PACKET_SIZE],(lpIoContext->IoSideBuffer.size-MAX_MAIN_PACKET_SIZE));
   672: 
   673: 			lpIoContext->IoSideBuffer.size = lpIoContext->IoSideBuffer.size-MAX_MAIN_PACKET_SIZE;
   674: 		}
   675: 		else
   676: 		{
   677: 			memcpy(lpIoContext->IoMainBuffer.buff,lpIoContext->IoSideBuffer.buff,lpIoContext->IoSideBuffer.size);
   678: 
   679: 			lpIoContext->wsabuf.buf = (char*)lpIoContext->IoMainBuffer.buff;
   680: 
   681: 			lpIoContext->wsabuf.len = lpIoContext->IoSideBuffer.size;
   682: 
   683: 			lpIoContext->IoType = IO_SEND;
   684: 
   685: 			lpIoContext->IoSize = lpIoContext->IoSideBuffer.size;
   686: 
   687: 			lpIoContext->IoMainBuffer.size = 0;
   688: 
   689: 			lpIoContext->IoSideBuffer.size = 0;
   690: 		}
   691: 	}
   692: 	else
   693: 	{
   694: 		lpIoContext->wsabuf.buf = (char*)&lpIoContext->IoMainBuffer.buff[lpIoContext->IoMainBuffer.size];
   695: 
   696: 		lpIoContext->wsabuf.len = lpIoContext->IoSize-lpIoContext->IoMainBuffer.size;
   697: 
   698: 		lpIoContext->IoType = IO_SEND;
   699: 	}
   700: 
   701: 	DWORD SendSize=0,Flags=0;
   702: 
   703: 	if(WSASend(lpObj->Socket,&lpIoContext->wsabuf,1,&SendSize,Flags,&lpIoContext->overlapped,0) == SOCKET_ERROR)
   704: 	{
   705: 		if(WSAGetLastError() != WSA_IO_PENDING)
   706: 		{
   707: 			gLog.Output(LOG_CONNECT,"[SocketManager] WSASend() failed with error: %d",WSAGetLastError());
   708: 			this->Disconnect(index);
   709: 			this->m_critical.unlock();
```

### `\b0xC3\b|\b0xC4\b|\b0xC1\b|\b0xC2\b`

- hit @ line 271: `		if(lpMsg[count] == 0xC1 || lpMsg[count] == 0xC3)`
```
   201: 	}
   202: 
   203: 	return 1;
   204: }
   205: 
   206: bool CSocketManager::CreateWorkerThread() // OK
   207: {
   208: 	SYSTEM_INFO SystemInfo;
   209: 
   210: 	GetSystemInfo(&SystemInfo);
   211: 
   212: 	this->m_ServerWorkerThreadCount = ((SystemInfo.dwNumberOfProcessors>MAX_SERVER_WORKER_THREAD)?MAX_SERVER_WORKER_THREAD:SystemInfo.dwNumberOfProcessors);
   213: 
   214: 	for(DWORD n=0;n < this->m_ServerWorkerThreadCount;n++)
   215: 	{
   216: 		if((this->m_ServerWorkerThread[n]=CreateThread(0,0,(LPTHREAD_START_ROUTINE)this->ServerWorkerThread,this,0,0)) == 0)
   217: 		{
   218: 			gLog.Output(LOG_CONNECT,"[SocketManager] CreateThread() failed with error: %d",GetLastError());
   219: 			return 0;
   220: 		}
   221: 
   222: 		if(SetThreadPriority(this->m_ServerWorkerThread[n],THREAD_PRIORITY_HIGHEST) == 0)
   223: 		{
   224: 			gLog.Output(LOG_CONNECT,"[SocketManager] SetThreadPriority() failed with error: %d",GetLastError());
   225: 			return 0;
   226: 		}
   227: 	}
   228: 
   229: 	return 1;
   230: }
   231: 
   232: bool CSocketManager::CreateServerQueue() // OK
   233: {
   234: 	if((this->m_ServerQueueSemaphore=CreateSemaphore(0,0,MAX_QUEUE_SIZE,0)) == 0)
   235: 	{
   236: 		gLog.Output(LOG_CONNECT,"[SocketManager] CreateSemaphore() failed with error: %d",GetLastError());
   237: 		return 0;
   238: 	}
   239: 
   240: 	if((this->m_ServerQueueThread=CreateThread(0,0,(LPTHREAD_START_ROUTINE)this->ServerQueueThread,this,0,0)) == 0)
   241: 	{
   242: 		gLog.Output(LOG_CONNECT,"[SocketManager] CreateThread() failed with error: %d",GetLastError());
   243: 		return 0;
   244: 	}
   245: 
   246: 	if(SetThreadPriority(this->m_ServerQueueThread,THREAD_PRIORITY_HIGHEST) == 0)
   247: 	{
   248: 		gLog.Output(LOG_CONNECT,"[SocketManager] SetThreadPriority() failed with error: %d",GetLastError());
   249: 		return 0;
   250: 	}
   251: 
   252: 	return 1;
   253: }
   254: 
   255: bool CSocketManager::DataRecv(int index,IO_MAIN_BUFFER* lpIoBuffer) // OK
   256: {
   257: 	if(lpIoBuffer->size < 3)
   258: 	{
   259: 		return 1;
   260: 	}
   261: 
   262: 	BYTE* lpMsg = lpIoBuffer->buff;
   263: 
   264: 	int count=0,size=0,DecSize=0,DecEncrypt=0,DecSerial=0;
   265: 	static BYTE DecBuff[MAX_MAIN_PACKET_SIZE];
   266: 	static QUEUE_INFO QueueInfo;
   267: 	BYTE header,head;
   268: 
   269: 	while(true)
   270: 	{
   271: 		if(lpMsg[count] == 0xC1 || lpMsg[count] == 0xC3)
   272: 		{
   273: 			header = lpMsg[count];
   274: 			size = lpMsg[count+1];
   275: 			head = lpMsg[count+2];
   276: 		}
   277: 		else if(lpMsg[count] == 0xC2 || lpMsg[count] == 0xC4)
   278: 		{
   279: 			header = lpMsg[count];
   280: 			size = MAKEWORD(lpMsg[count+2],lpMsg[count+1]);
   281: 			head = lpMsg[count+3];
   282: 		}
   283: 		else
   284: 		{
   285: 			gLog.Output(LOG_CONNECT,"[SocketManager] Protocol header error (Index: %d, Header: %x)",index,lpMsg[count]);
   286: 			return 0;
   287: 		}
   288: 
   289: 		if(size < 3 || size > MAX_MAIN_PACKET_SIZE)
   290: 		{
   291: 			gLog.Output(LOG_CONNECT,"[SocketManager] Protocol size error (Index: %d, Header: %x, Size: %d, Head: %x)",index,header,size,head);
   292: 			return 0;
   293: 		}
   294: 
   295: 		if(size <= lpIoBuffer->size)
   296: 		{
   297: 			if(header == 0xC3 || header == 0xC4)
   298: 			{
   299: 				if(header == 0xC3)
   300: 				{
   301: 					DecSize = gPacketManager.Decrypt(&DecBuff[1],&lpMsg[count+2],(size-2))+1;
   302: 
   303: 					DecSerial = DecBuff[1];
   304: 
   305: 					header = 0xC1;
   306: 					head = DecBuff[2];
   307: 
   308: 					DecBuff[0] = header;
   309: 					DecBuff[1] = DecSize;
   310: 
   311: 					if(gPacketManager.AddData(&DecBuff[0],DecSize) == 0 || gPacketManager.ExtractPacket(DecBuff) == 0)
   312: 					{
   313: 						return 0;
   314: 					}
   315: 
   316: 					QueueInfo.index = index;
   317: 
   318: 					QueueInfo.head = head;
   319: 
   320: 					memcpy(QueueInfo.buff,DecBuff,DecSize);
   321: 
   322: 					QueueInfo.size = DecSize;
   323: 
   324: 					QueueInfo.encrypt = 1;
   325: 
   326: 					QueueInfo.serial = DecSerial;
   327: 
   328: 					if(this->m_ServerQueue.AddToQueue(&QueueInfo) != 0)
   329: 					{
   330: 						ReleaseSemaphore(this->m_ServerQueueSemaphore,1,0);
   331: 					}
   332: 				}
   333: 				else
   334: 				{
   335: 					DecSize = gPacketManager.Decrypt(&DecBuff[2],&lpMsg[count+3],(size-3))+2;
   336: 					DecSize = size;
   337: 					DecSerial = DecBuff[2];
   338: 
   339: 					header = 0xC2;
   340: 					head = DecBuff[3];
   341: 
   342: 					DecBuff[0] = header;
   343: 					DecBuff[1] = HIBYTE(DecSize);
   344: 					DecBuff[2] = LOBYTE(DecSize);
   345: 
   346: 					if(gPacketManager.AddData(DecBuff,DecSize) == 0 || gPacketManager.ExtractPacket(DecBuff) == 0)
   347: 					{
   348: 						return 0;
   349: 					}
   350: 
   351: 					QueueInfo.index = index;
   352: 
   353: 					QueueInfo.head = head;
   354: 
   355: 					memcpy(QueueInfo.buff,DecBuff,DecSize);
   356: 
   357: 					QueueInfo.size = DecSize;
   358: 
   359: 					QueueInfo.encrypt = 1;
   360: 
   361: 					QueueInfo.serial = DecSerial;
   362: 
   363: 					if(this->m_ServerQueue.AddToQueue(&QueueInfo) != 0)
   364: 					{
   365: 						ReleaseSemaphore(this->m_ServerQueueSemaphore,1,0);
   366: 					}
   367: 				}
   368: 			}
   369: 			else
   370: 			{
   371: 				if(gPacketManager.AddData(&lpMsg[count],size) == 0 || gPacketManager.ExtractPacket(DecBuff) == 0)
   372: 				{
   373: 					return 0;
   374: 				}
   375: 
   376: 				QueueInfo.index = index;
   377: 
   378: 				QueueInfo.head = head;
   379: 
   380: 				memcpy(QueueInfo.buff,DecBuff,size);
   381: 
   382: 				QueueInfo.size = size;
   383: 
   384: 				QueueInfo.encrypt = 0;
   385: 
   386: 				QueueInfo.serial = -1;
   387: 
   388: 				if(this->m_ServerQueue.AddToQueue(&QueueInfo) != 0)
   389: 				{
   390: 					ReleaseSemaphore(this->m_ServerQueueSemaphore,1,0);
   391: 				}
   392: 			}
   393: 
   394: 			count += size;
   395: 
   396: 			lpIoBuffer->size -= size;
   397: 
   398: 			if(lpIoBuffer->size <= 0)
   399: 			{
   400: 				break;
   401: 			}
   402: 		}
   403: 		else
   404: 		{
   405: 			if(count > 0 && lpIoBuffer->size > 0 && lpIoBuffer->size <= (MAX_MAIN_PACKET_SIZE-count))
   406: 			{
   407: 				memmove(lpMsg,&lpMsg[count],lpIoBuffer->size);
   408: 			}
   409: 
   410: 			break;
   411: 		}
   412: 	}
   413: 
   414: 	return 1;
   415: }
   416: 
   417: bool CSocketManager::DataSend(int index,BYTE* lpMsg,int size) // OK
   418: {
   419: 	this->m_critical.lock();
   420: 
   421: 	if(OBJECT_USER_RANGE(index) == 0)
   422: 	{
   423: 		this->m_critical.unlock();
   424: 		return 0;
   425: 	}
   426: 
   427: 	if(gObj[index].Socket == INVALID_SOCKET)
   428: 	{
   429: 		this->m_critical.unlock();
   430: 		return 0;
   431: 	}
   432: 
   433: 	if(gObj[index].Connected == OBJECT_OFFLINE)
   434: 	{
   435: 		this->m_critical.unlock();
   436: 		return 0;
   437: 	}
   438: 
   439: 	static BYTE send[MAX_MAIN_PACKET_SIZE];
   440: 
   441: 	memcpy(send,lpMsg,size);
   442: 
   443: 	if(lpMsg[0] == 0xC3 || lpMsg[0] == 0xC4)
   444: 	{
   445: 		if(lpMsg[0] == 0xC3)
   446: 		{
   447: 			BYTE save = lpMsg[1];
   448: 
   449: 			lpMsg[1] = gSerialCheck[index].GetSendSerial();
   450: 
   451: 			size = gPacketManager.Encrypt(&send[2],&lpMsg[1],(size-1))+2;
   452: 
   453: 			lpMsg[1] = save;
   454: 
   455: 			send[0] = 0xC3;
   456: 			send[1] = size;
   457: 		}
   458: 		else
   459: 		{
   460: 			BYTE save = lpMsg[2];
   461: 
   462: 			lpMsg[2] = gSerialCheck[index].GetSendSerial();
   463: 
   464: 			size = gPacketManager.Encrypt(&send[3],&lpMsg[2],(size-2))+3;
   465: 
   466: 			lpMsg[2] = save;
   467: 
   468: 			send[0] = 0xC4;
   469: 			send[1] = HIBYTE(size);
   470: 			send[2] = LOBYTE(size);
   471: 		}
   472: 	}
   473: 
   474: 	if(size > MAX_MAIN_PACKET_SIZE)
   475: 	{
   476: 		gLog.Output(LOG_CONNECT,"[SocketManager] Max msg size (Type: 1, Index: %d, Size: %d)",index,size);
   477: 		this->Disconnect(index);
   478: 		this->m_critical.unlock();
   479: 		return 0;
   480: 	}
   481: 
   482: 	#if(ENCRYPT_STATE==1)
   483: 
   484: 	EncryptData(send,size);
   485: 
   486: 	#endif
   487: 
   488: 	IO_SEND_CONTEXT* lpIoContext = &gObj[index].PerSocketContext->IoSendContext;
   489: 
   490: 	if(lpIoContext->IoSize > 0)
   491: 	{
   492: 		if((lpIoContext->IoSideBuffer.size+size) > MAX_SIDE_PACKET_SIZE)
   493: 		{
   494: 			gLog.Output(LOG_CONNECT,"[SocketManager] Max msg size (Type: 2, Index: %d, Size: %d)",index,(lpIoContext->IoSideBuffer.size+size));
   495: 			this->Disconnect(index);
   496: 			this->m_critical.unlock();
   497: 			return 0;
   498: 		}
   499: 
   500: 		memcpy(&lpIoContext->IoSideBuffer.buff[lpIoContext->IoSideBuffer.size],send,size);
   501: 		lpIoContext->IoSideBuffer.size += size;
   502: 		this->m_critical.unlock();
   503: 		return 1;
   504: 	}
   505: 
   506: 	memcpy(lpIoContext->IoMainBuffer.buff,send,size);
   507: 
   508: 	lpIoContext->wsabuf.buf = (char*)lpIoContext->IoMainBuffer.buff;
   509: 
   510: 	lpIoContext->wsabuf.len = size;
   511: 
   512: 	lpIoContext->IoType = IO_SEND;
   513: 
   514: 	lpIoContext->IoSize = size;
   515: 
   516: 	lpIoContext->IoMainBuffer.size = 0;
   517: 
   518: 	DWORD SendSize=0,Flags=0;
   519: 
   520: 	if(WSASend(gObj[index].Socket,&lpIoContext->wsabuf,1,&SendSize,Flags,&lpIoContext->overlapped,0) == SOCKET_ERROR)
   521: 	{
   522: 		if(WSAGetLastError() != WSA_IO_PENDING)
   523: 		{
   524: 			gLog.Output(LOG_CONNECT,"[SocketManager] WSASend() failed with error: %d",WSAGetLastError());
   525: 			this->Disconnect(index);
   526: 			this->m_critical.unlock();
   527: 			return 0;
   528: 		}
   529: 	}
   530: 
   531: 	this->m_critical.unlock();
```

## SocketManagerModern.cpp — path moderno (BOTH_MESSAGE)

### `ProtocolHead::BOTH_MESSAGE`

- hit @ line 132: `		case ProtocolHead::BOTH_MESSAGE:`
```
    62: 	{
    63: 		LogAdd(LOG_RED,"[SocketManager] ERROR: Max msg size %d (Msg size:%d)",MAX_MAIN_PACKET_SIZE,size);
    64: 		return;
    65: 	}
    66: 
    67: 	PacketMsg lpMsg;
    68: 
    69: 	lpMsg.header.id = head;
    70: 
    71: 	lpMsg.body.resize(lpMsg.body.size() + size);
    72: 
    73: 	std::memcpy(lpMsg.body.data(), message, size);
    74: 
    75: 	lpMsg.header.size = lpMsg.size();
    76: 
    77: 	this->connection->ProtocolSend(aIndex, lpMsg);
    78: }
    79: 
    80: void CSocketManagerModern::DataReceived(uint16_t aIndex,olc::net::message<ProtocolHead> msg)
    81: {
    82: 	switch (msg.header.id)
    83: 	{
    84: 		case ProtocolHead::CLIENT_LIVE_CLIENT:
    85: 		{
    86: 			PMSG_TESTE_RECV lpMsg;
    87: 			msg >> lpMsg;
    88: 
    89: 			std::cout << "[" << aIndex << "]: [" << lpMsg.TickCount << "][" << lpMsg.PhysiSpeed << "][" << lpMsg.MagicSpeed << "]:Data Recv\n";
    90: 
    91: 			auto lpObj = &gObj[aIndex];
    92: 
    93: 			lpObj->ConnectTickCount = GetTickCount();
    94: 
    95: 			//std::cout << "DataSend [" << GetTickCount() << "]\n";
    96: 
    97: 			PMSG_TESTE_SEND TesteSend;
    98: 
    99: 			TesteSend.TickCount = GetTickCount();
   100: 
   101: 			this->PacketSend(aIndex,ProtocolHead::SERVER_DISCONNECT,(uint8_t*)&TesteSend,sizeof(TesteSend));
   102: 			break;
   103: 		}
   104: 		case ProtocolHead::BOTH_CONNECT_LOGIN:
   105: 			CGConnectAccountRecv((PMSG_CONNECT_ACCOUNT_RECV*)msg.body.data(), aIndex);
   106: 			break;
   107: 		case ProtocolHead::BOTH_CONNECT_CHARACTER:
   108: 			CGCharacterListRecv(aIndex);
   109: 			break;
   110: 		case ProtocolHead::BOTH_POSITION:
   111: 			CGPositionRecv((PMSG_POSITION_RECV*)msg.body.data(),aIndex);
   112: 			break;
   113: 		case ProtocolHead::BOTH_MOVE:
   114: 			CGMoveRecv((PMSG_MOVE_RECV*)msg.body.data(),aIndex);
   115: 			break;
   116: 		case ProtocolHead::BOTH_ATTACK1:
   117: 			gAttack.CGAttackRecv((PMSG_ATTACK_RECV*)msg.body.data(),aIndex);
   118: 			break;
   119: 		case ProtocolHead::BOTH_ATTACK2:
   120: 			gSkillManager.CGMultiSkillAttackRecv((PMSG_MULTI_SKILL_ATTACK_RECV*)msg.body.data(),aIndex,0);
   121: 			break;
   122: 		case ProtocolHead::BOTH_CONNECT_JOIN_GAME:
   123: 			{
   124: 				PMSG_CHARACTER_INFO_RECV pMsg;
   125: 				pMsg.header.set(0xF3, 0x03, sizeof(pMsg));
   126: 				memset(pMsg.name, 0, sizeof(pMsg.name));
   127: 				uint16_t copySize = (msg.body.size() < sizeof(pMsg.name)) ? msg.body.size() : sizeof(pMsg.name);
   128: 				memcpy(pMsg.name, msg.body.data(), copySize);
   129: 				CGCharacterInfoRecv(&pMsg, aIndex);
   130: 			}
   131: 			break;
   132: 		case ProtocolHead::BOTH_MESSAGE:
   133: 			{
   134: 				static uint8_t recv[8024];
   135: 
   136: 				for (uint16_t start = 0; start < msg.header.size; start++) {
   137: 					std::memcpy(&recv[start], &msg.body[start], 1);
   138: 				}
   139: 
   140: 				//uint8_t head = 0xFF;
   141: 
   142: 				//if (recv[0] == 0xC1 || recv[0] == 0xC3) {
   143: 				//	head = recv[2];
   144: 				//}
   145: 				//else if (recv[0] == 0xC2 || recv[0] == 0xC4) {
   146: 				//	head = recv[3];
   147: 				//}
   148: 
   149: 				int count=0,size=0,DecSize=0,DecEncrypt=0,DecSerial=0;
   150: 				static BYTE DecBuff[MAX_MAIN_PACKET_SIZE];
   151: 				BYTE header,head;
   152: 
   153: 				if(recv[count] == 0xC1 || recv[count] == 0xC3)
   154: 				{
   155: 					header = recv[count];
   156: 					size = recv[count+1];
   157: 					head = recv[count+2];
   158: 				}
   159: 				else if(recv[count] == 0xC2 || recv[count] == 0xC4)
   160: 				{
   161: 					header = recv[count];
   162: 					size = MAKEWORD(recv[count+2],recv[count+1]);
   163: 					head = recv[count+3];
   164: 				}
   165: 
   166: 				if(gPacketManager.AddData(&recv[count],size) == 0 || gPacketManager.ExtractPacket(DecBuff) == 0)
   167: 				{
   168: 					return;
   169: 				}
   170: 
   171: 				ProtocolCore(head,DecBuff,size,aIndex, 0, 0);
   172: 
   173: 			}
   174: 			break;
   175: 
   176: 
   177: 	}
   178: }
   179: 
   180: //bool CSocketManagerModern::ExtractPacket(BYTE* lpBuff) // OK
   181: //{
   182: //	int size,end;
   183: //
   184: //	switch(lpBuff[0])
   185: //	{
   186: //		case 0xC1:
   187: //			size = lpBuff[1];
   188: //			end = 2;
   189: //			break;
   190: //		case 0xC2:
   191: //			size = MAKEWORD(lpBuff[2],lpBuff[1]);
   192: //			end = 3;
   193: //			break;
   194: //		default:
   195: //			return 0;
   196: //	}
   197: //
   198: //	this->XorData((size-1),end);
   199: //
   200: //	//memcpy(lpBuff,this->m_buff,size);
   201: //
   202: //	return 1;
   203: //}
   204: 
   205: #endif
```

### `\bPacketSend\s*\(`

- hit @ line 59: `void CSocketManagerModern::PacketSend(uint16_t aIndex, ProtocolHead head,uint8_t* message, uint16_t size)`
```
     1: #include "stdafx.h"
     2: #if(NEW_PROTOCOL_SYSTEM==1)
     3: #include "SocketManagerModern.h"
     4: #include "Attack.h"
     5: #include "ConsoleDebug.h"
     6: #include "PacketManager.h"
     7: #include "Protocol.h"
     8: #include "SkillManager.h"
     9: #include "User.h"
    10: #include "Util.h"
    11: 
    12: CSocketManagerModern gSocketManagerModern;
    13: 
    14: bool CSocketManagerModern::StartServer(uint16_t nPort)
    15: {
    16: 	if (!this->CheckPortUse(nPort)) //mudar para nPort
    17: 	{
    18: 		g_ConsoleDebug; //comment to hide the console
    19: 
    20: 		this->connection = new CSocketConnection(nPort);
    21: 
    22: 		this->connection->Start();
    23: 
    24: 		this->RunServerThread();
    25: 		return true;
    26: 	}
    27: 	std::cout << "Port already in use!\n";
    28: 	return false;
    29: }
    30: 
    31: bool CSocketManagerModern::CheckPortUse(unsigned short port)
    32: {
    33: 	using namespace ::asio;
    34: 	using ip::tcp;
    35: 
    36: 	io_service svc;
    37: 	tcp::acceptor a(svc);
    38: 
    39: 	::asio::error_code ec;
    40: 	a.open(tcp::v4(), ec) || a.bind({ tcp::v4(), port }, ec);
    41: 
    42: 	return ec == error::address_in_use;
    43: }
    44: 
    45: void CSocketManagerModern::ListenServer()
    46: {
    47: 	while (1)
    48: 	{
    49: 		gSocketManagerModern.connection->Update(-1, true);
    50: 		std::this_thread::sleep_for(std::chrono::milliseconds(10));
    51: 	}
    52: }
    53: 
    54: void CSocketManagerModern::DisconenctClient(uint16_t aIndex)
    55: {
    56: 	this->connection->DisconnectClient(aIndex);
    57: }
    58: 
    59: void CSocketManagerModern::PacketSend(uint16_t aIndex, ProtocolHead head,uint8_t* message, uint16_t size)
    60: {
    61: 	if (size > MAX_MAIN_PACKET_SIZE)
    62: 	{
    63: 		LogAdd(LOG_RED,"[SocketManager] ERROR: Max msg size %d (Msg size:%d)",MAX_MAIN_PACKET_SIZE,size);
    64: 		return;
    65: 	}
    66: 
    67: 	PacketMsg lpMsg;
    68: 
    69: 	lpMsg.header.id = head;
    70: 
    71: 	lpMsg.body.resize(lpMsg.body.size() + size);
    72: 
    73: 	std::memcpy(lpMsg.body.data(), message, size);
    74: 
    75: 	lpMsg.header.size = lpMsg.size();
    76: 
    77: 	this->connection->ProtocolSend(aIndex, lpMsg);
    78: }
    79: 
    80: void CSocketManagerModern::DataReceived(uint16_t aIndex,olc::net::message<ProtocolHead> msg)
    81: {
    82: 	switch (msg.header.id)
    83: 	{
    84: 		case ProtocolHead::CLIENT_LIVE_CLIENT:
    85: 		{
    86: 			PMSG_TESTE_RECV lpMsg;
    87: 			msg >> lpMsg;
    88: 
    89: 			std::cout << "[" << aIndex << "]: [" << lpMsg.TickCount << "][" << lpMsg.PhysiSpeed << "][" << lpMsg.MagicSpeed << "]:Data Recv\n";
    90: 
    91: 			auto lpObj = &gObj[aIndex];
    92: 
    93: 			lpObj->ConnectTickCount = GetTickCount();
    94: 
    95: 			//std::cout << "DataSend [" << GetTickCount() << "]\n";
    96: 
    97: 			PMSG_TESTE_SEND TesteSend;
    98: 
    99: 			TesteSend.TickCount = GetTickCount();
   100: 
   101: 			this->PacketSend(aIndex,ProtocolHead::SERVER_DISCONNECT,(uint8_t*)&TesteSend,sizeof(TesteSend));
   102: 			break;
   103: 		}
   104: 		case ProtocolHead::BOTH_CONNECT_LOGIN:
   105: 			CGConnectAccountRecv((PMSG_CONNECT_ACCOUNT_RECV*)msg.body.data(), aIndex);
   106: 			break;
   107: 		case ProtocolHead::BOTH_CONNECT_CHARACTER:
   108: 			CGCharacterListRecv(aIndex);
   109: 			break;
   110: 		case ProtocolHead::BOTH_POSITION:
   111: 			CGPositionRecv((PMSG_POSITION_RECV*)msg.body.data(),aIndex);
   112: 			break;
   113: 		case ProtocolHead::BOTH_MOVE:
   114: 			CGMoveRecv((PMSG_MOVE_RECV*)msg.body.data(),aIndex);
   115: 			break;
   116: 		case ProtocolHead::BOTH_ATTACK1:
   117: 			gAttack.CGAttackRecv((PMSG_ATTACK_RECV*)msg.body.data(),aIndex);
   118: 			break;
   119: 		case ProtocolHead::BOTH_ATTACK2:
   120: 			gSkillManager.CGMultiSkillAttackRecv((PMSG_MULTI_SKILL_ATTACK_RECV*)msg.body.data(),aIndex,0);
   121: 			break;
   122: 		case ProtocolHead::BOTH_CONNECT_JOIN_GAME:
   123: 			{
   124: 				PMSG_CHARACTER_INFO_RECV pMsg;
   125: 				pMsg.header.set(0xF3, 0x03, sizeof(pMsg));
   126: 				memset(pMsg.name, 0, sizeof(pMsg.name));
   127: 				uint16_t copySize = (msg.body.size() < sizeof(pMsg.name)) ? msg.body.size() : sizeof(pMsg.name);
   128: 				memcpy(pMsg.name, msg.body.data(), copySize);
   129: 				CGCharacterInfoRecv(&pMsg, aIndex);
   130: 			}
   131: 			break;
   132: 		case ProtocolHead::BOTH_MESSAGE:
   133: 			{
   134: 				static uint8_t recv[8024];
   135: 
   136: 				for (uint16_t start = 0; start < msg.header.size; start++) {
   137: 					std::memcpy(&recv[start], &msg.body[start], 1);
   138: 				}
   139: 
   140: 				//uint8_t head = 0xFF;
   141: 
   142: 				//if (recv[0] == 0xC1 || recv[0] == 0xC3) {
   143: 				//	head = recv[2];
   144: 				//}
   145: 				//else if (recv[0] == 0xC2 || recv[0] == 0xC4) {
   146: 				//	head = recv[3];
   147: 				//}
   148: 
   149: 				int count=0,size=0,DecSize=0,DecEncrypt=0,DecSerial=0;
   150: 				static BYTE DecBuff[MAX_MAIN_PACKET_SIZE];
   151: 				BYTE header,head;
   152: 
   153: 				if(recv[count] == 0xC1 || recv[count] == 0xC3)
   154: 				{
   155: 					header = recv[count];
   156: 					size = recv[count+1];
   157: 					head = recv[count+2];
   158: 				}
   159: 				else if(recv[count] == 0xC2 || recv[count] == 0xC4)
   160: 				{
   161: 					header = recv[count];
   162: 					size = MAKEWORD(recv[count+2],recv[count+1]);
   163: 					head = recv[count+3];
   164: 				}
   165: 
   166: 				if(gPacketManager.AddData(&recv[count],size) == 0 || gPacketManager.ExtractPacket(DecBuff) == 0)
   167: 				{
   168: 					return;
   169: 				}
   170: 
   171: 				ProtocolCore(head,DecBuff,size,aIndex, 0, 0);
   172: 
   173: 			}
   174: 			break;
   175: 
   176: 
   177: 	}
   178: }
   179: 
   180: //bool CSocketManagerModern::ExtractPacket(BYTE* lpBuff) // OK
   181: //{
   182: //	int size,end;
   183: //
   184: //	switch(lpBuff[0])
   185: //	{
   186: //		case 0xC1:
   187: //			size = lpBuff[1];
   188: //			end = 2;
   189: //			break;
   190: //		case 0xC2:
   191: //			size = MAKEWORD(lpBuff[2],lpBuff[1]);
   192: //			end = 3;
   193: //			break;
   194: //		default:
   195: //			return 0;
   196: //	}
   197: //
   198: //	this->XorData((size-1),end);
   199: //
   200: //	//memcpy(lpBuff,this->m_buff,size);
   201: //
   202: //	return 1;
   203: //}
   204: 
   205: #endif
```

### `\bDataSend\s*\(`

- no hits


## Protocol.h — PSBMSG_HEAD (C1 vs C3)

- hit @ line 100: `struct PSBMSG_HEAD`
```
    40: #define PROTOCOL_CODE2 0xD6
    41: #define PROTOCOL_CODE3 0xDF
    42: #define PROTOCOL_CODE4 0x11
    43: #elif(GAMESERVER_LANGUAGE==7)
    44: #define PROTOCOL_CODE1 0xD9
    45: #define PROTOCOL_CODE2 0x15
    46: #define PROTOCOL_CODE3 0xDC
    47: #define PROTOCOL_CODE4 0x1D
    48: #endif
    49: 
    50: // ============================================================
    51: // Compile-time validation: PROTOCOL_CODE must match client
    52: // Client uses: PACKET_MOVE=0xD4, PACKET_ATTACK=0x11, PACKET_POSITION=0x15
    53: // ============================================================
    54: #if GAMESERVER_LANGUAGE == 1
    55: static_assert(PROTOCOL_CODE1 == 0xD4, "PROTOCOL_CODE1 mismatch: client expects 0xD4 (PACKET_MOVE)");
    56: static_assert(PROTOCOL_CODE2 == 0x11, "PROTOCOL_CODE2 mismatch: client expects 0x11 (PACKET_ATTACK)");
    57: static_assert(PROTOCOL_CODE3 == 0x15, "PROTOCOL_CODE3 mismatch: client expects 0x15 (PACKET_POSITION)");
    58: static_assert(PROTOCOL_CODE4 == 0xDB, "PROTOCOL_CODE4 mismatch with client");
    59: #elif GAMESERVER_LANGUAGE != 1
    60: #pragma message("WARNING: GAMESERVER_LANGUAGE is not 1 - PROTOCOL_CODE may not match client!")
    61: #pragma message("WARNING: Client uses PACKET_MOVE=0xD4, PACKET_ATTACK=0x11, PACKET_POSITION=0x15")
    62: #endif
    63: 
    64: #define SET_NUMBERHB(x) ((BYTE)((DWORD)(x)>>(DWORD)8))
    65: #define SET_NUMBERLB(x) ((BYTE)((DWORD)(x)&0xFF))
    66: #define SET_NUMBERHW(x) ((WORD)((DWORD)(x)>>(DWORD)16))
    67: #define SET_NUMBERLW(x) ((WORD)((DWORD)(x)&0xFFFF))
    68: #define SET_NUMBERHDW(x) ((DWORD)((QWORD)(x)>>(QWORD)32))
    69: #define SET_NUMBERLDW(x) ((DWORD)((QWORD)(x)&0xFFFFFFFF))
    70: 
    71: #define MAKE_NUMBERW(x,y) ((WORD)(((BYTE)((y)&0xFF))|((BYTE)((x)&0xFF)<<8)))
    72: #define MAKE_NUMBERDW(x,y) ((DWORD)(((WORD)((y)&0xFFFF))|((WORD)((x)&0xFFFF)<<16)))
    73: #define MAKE_NUMBERQW(x,y) ((QWORD)(((DWORD)((y)&0xFFFFFFFF))|((DWORD)((x)&0xFFFFFFFF)<<32)))
    74: 
    75: //**********************************************//
    76: //************ Packet Base *********************//
    77: //**********************************************//
    78: 
    79: struct PBMSG_HEAD
    80: {
    81: 	void set(BYTE head,BYTE size) // OK
    82: 	{
    83: 		this->type = 0xC1;
    84: 		this->size = size;
    85: 		this->head = head;
    86: 	}
    87: 
    88: 	void setE(BYTE head,BYTE size) // OK
    89: 	{
    90: 		this->type = 0xC3;
    91: 		this->size = size;
    92: 		this->head = head;
    93: 	}
    94: 
    95: 	BYTE type;
    96: 	BYTE size;
    97: 	BYTE head;
    98: };
    99: 
   100: struct PSBMSG_HEAD
   101: {
   102: 	void set(BYTE head,BYTE subh,BYTE size) // OK
   103: 	{
   104: 		this->type = 0xC1;
   105: 		this->size = size;
   106: 		this->head = head;
   107: 		this->subh = subh;
   108: 	}
   109: 
   110: 	void setE(BYTE head,BYTE subh,BYTE size) // OK
   111: 	{
   112: 		this->type = 0xC3;
   113: 		this->size = size;
   114: 		this->head = head;
   115: 		this->subh = subh;
   116: 	}
   117: 
   118: 	BYTE type;
   119: 	BYTE size;
   120: 	BYTE head;
   121: 	BYTE subh;
   122: };
   123: 
   124: struct PWMSG_HEAD
   125: {
   126: 	void set(BYTE head,WORD size) // OK
   127: 	{
   128: 		this->type = 0xC2;
   129: 		this->size[0] = SET_NUMBERHB(size);
   130: 		this->size[1] = SET_NUMBERLB(size);
   131: 		this->head = head;
   132: 	}
   133: 
   134: 	void setE(BYTE head,WORD size) // OK
   135: 	{
   136: 		this->type = 0xC4;
   137: 		this->size[0] = SET_NUMBERHB(size);
   138: 		this->size[1] = SET_NUMBERLB(size);
   139: 		this->head = head;
   140: 	}
   141: 
   142: 	BYTE type;
   143: 	BYTE size[2];
   144: 	BYTE head;
   145: };
   146: 
   147: struct PSWMSG_HEAD
   148: {
   149: 	void set(BYTE head,BYTE subh,WORD size) // OK
   150: 	{
   151: 		this->type = 0xC2;
   152: 		this->size[0] = SET_NUMBERHB(size);
   153: 		this->size[1] = SET_NUMBERLB(size);
   154: 		this->head = head;
   155: 		this->subh = subh;
   156: 	}
   157: 
   158: 	void setE(BYTE head,BYTE subh,WORD size) // OK
   159: 	{
   160: 		this->type = 0xC4;
   161: 		this->size[0] = SET_NUMBERHB(size);
   162: 		this->size[1] = SET_NUMBERLB(size);
   163: 		this->head = head;
   164: 		this->subh = subh;
   165: 	}
   166: 
   167: 	BYTE type;
   168: 	BYTE size[2];
   169: 	BYTE head;
   170: 	BYTE subh;
   171: };
   172: 
   173: //struct PBMSG_HEAD2	// Packet - Byte Type
   174: //{
   175: //
   176: //public:
   177: //
   178: //	void set(BYTE head,BYTE subh,BYTE size)
   179: //	{
   180: //		this->type = 0xC1;
   181: //		this->size = size;
   182: //		this->head = head;
   183: //		this->subh = subh;
   184: //	};
   185: //
   186: //	BYTE type;
   187: //	BYTE size;
   188: //	BYTE head;
   189: //	BYTE subh;
   190: //};
   191: 
   192: //**********************************************//
   193: //************ Client -> GameServer ************//
   194: //**********************************************//
   195: 
   196: struct PMSG_CHAT_RECV
   197: {
   198: 	PBMSG_HEAD header; // C1:00
   199: 	char name[10];
   200: 	char message[60];
   201: };
   202: 
   203: struct PMSG_CHAT_WHISPER_RECV
   204: {
   205: 	PBMSG_HEAD header; // C1:00
   206: 	char name[10];
   207: 	char message[60];
   208: };
   209: 
   210: struct PMSG_MAIN_CHECK_RECV
   211: {
   212: 	PBMSG_HEAD header; // C1:03
   213: 	DWORD key;
   214: };
   215: 
   216: struct PMSG_LIVE_CLIENT_RECV
   217: {
   218: 	PBMSG_HEAD header; // C1:0E
   219: 	DWORD TickCount;
   220: 	WORD PhysiSpeed;
   221: 	WORD MagicSpeed;
   222: };
   223: 
   224: struct PMSG_POSITION_RECV
   225: {
   226: 	#if(NEW_PROTOCOL_SYSTEM==0)
   227: 	PBMSG_HEAD header; // C1:[PROTOCOL_CODE3]
   228: 	#endif
   229: 	BYTE x;
   230: 	BYTE y;
   231: };
   232: 
   233: struct PMSG_ACTION_RECV
   234: {
   235: 	PBMSG_HEAD header; // C1:18
   236: 	BYTE dir;
   237: 	BYTE action;
   238: 	BYTE index[2];
   239: };
   240: 
   241: struct PMSG_EVENT_REMAIN_TIME_RECV
   242: {
   243: 	PBMSG_HEAD header; // C1:91
   244: 	BYTE EventType;
   245: 	BYTE ItemLevel;
   246: };
   247: 
   248: struct PMSG_PET_ITEM_COMMAND_RECV
   249: {
   250: 	PBMSG_HEAD header; // C1:A7
   251: 	BYTE type;
   252: 	BYTE command;
   253: 	BYTE index[2];
   254: };
   255: 
   256: struct PMSG_PET_ITEM_INFO_RECV
   257: {
   258: 	PBMSG_HEAD header; // C1:A9
   259: 	BYTE type;
   260: 	BYTE flag;
   261: 	BYTE slot;
   262: };
   263: 
   264: struct PMSG_MAP_SERVER_MOVE_AUTH_RECV
   265: {
   266: 	PSBMSG_HEAD header; // C1:B1:01
   267: 	char account[12];
   268: 	char name[12];
   269: 	DWORD AuthCode1;
   270: 	DWORD AuthCode2;
   271: 	DWORD AuthCode3;
   272: 	DWORD AuthCode4;
   273: 	DWORD TickCount;
   274: 	BYTE ClientVersion[5];
   275: 	BYTE ClientSerial[16];
   276: };
   277: 
   278: struct PMSG_FRIEND_MESSAGE_RECV
   279: {
   280: 	PWMSG_HEAD header; // C1:C5
   281: 	DWORD guid;
   282: 	char name[10];
   283: 	char subject[60];
   284: 	BYTE dir;
   285: 	BYTE action;
   286: 	WORD size;
   287: 	char text[1000];
   288: };
   289: 
   290: struct PMSG_MOVE_RECV
   291: {
   292: 	#if(NEW_PROTOCOL_SYSTEM==0)
   293: 	PBMSG_HEAD header; // C1:[PROTOCOL_CODE1]
   294: 	#endif
   295: 	BYTE x;
   296: 	BYTE y;
   297: 	BYTE path[8];
   298: };
   299: 
   300: struct PMSG_CONNECT_ACCOUNT_RECV
   301: {
   302: 	#if(GAMESERVER_LANGUAGE==1)
   303: 	#pragma pack(1)
   304: 	#if(NEW_PROTOCOL_SYSTEM==0)
   305: 	PSBMSG_HEAD header; // C3:F1:01
   306: 	#endif
   307: 	char account[10];
   308: 	char password[20];
   309: 	DWORD TickCount;
   310: 	BYTE ClientVersion[5];
   311: 	BYTE ClientSerial[16];
   312: 	#if(GAMESERVER_UPDATE>=803)
   313: 	BYTE LanguageCode;
   314: 	#endif
   315: 	#pragma pack()
   316: 	#else
   317: 	#pragma pack(1)
   318: 	PSBMSG_HEAD header; // C3:F1:01
   319: 	char account[10];
   320: 	char password[12];
   321: 	DWORD TickCount;
   322: 	BYTE ClientVersion[5];
   323: 	BYTE ClientSerial[16];
   324: 	#pragma pack()
   325: 	#endif
   326: };
   327: 
   328: struct PMSG_CLOSE_CLIENT_RECV
   329: {
   330: 	PSBMSG_HEAD header; // C1:F1:02
   331: 	BYTE type;
   332: };
   333: 
   334: struct PMSG_CHARACTER_CREATE_RECV
   335: {
   336: 	PSBMSG_HEAD header; // C1:F3:01
   337: 	char name[10];
   338: 	BYTE Class;
   339: };
   340: 
   341: struct PMSG_CHARACTER_DELETE_RECV
   342: {
   343: 	PSBMSG_HEAD header; // C1:F3:02
   344: 	char name[10];
   345: 	char PersonalCode[10];
   346: };
   347: 
   348: struct PMSG_CHARACTER_INFO_RECV
   349: {
   350: 	PSBMSG_HEAD header; // C1:F3:03
   351: 	char name[10];
   352: };
   353: 
   354: struct PMSG_LEVEL_UP_POINT_RECV
   355: {
   356: 	PSBMSG_HEAD header; // C1:F3:06
   357: 	BYTE type;
   358: };
   359: 
   360: struct PMSG_CHARACTER_NAME_CHECK_RECV
```


---

## QUESTIONS (responder como CONFIRMED/PROBABLE com âncora)

1) O DataSend global transforma C1->C3 ou só roteia? Onde?

2) Com NEW_PROTOCOL_SYSTEM=1, o que sai no socket: pacote clássico direto ou encapsulado (BOTH_MESSAGE)?

3) Para F3:03 CharacterInfo: o header set() é C1, mas existe setE() C3; qual está ativo no pin?
