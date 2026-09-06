# NEW-ERA 1.3-R — BOTH_ATTACK1/2 (0x0008/0x0009) BODY FORMAT — EVIDENCE

- Objective: evidenciar IDs, layout do body e validações no GS moderno (olc/ASIO).

- Legacy commit: 580472e0d5723f9709cbad594f233deb07f9f351

- Extras vs comando: Attack.h + SkillManager.h (structs PMSG_ATTACK_RECV / PMSG_MULTI_SKILL_ATTACK_RECV p/ Q3)


## Files fetched (sha256)

- `SocketManagerModern.cpp` sha256 `82f0629c5c0007fad9a9eb81e6d7eeff96e4e5caeb4523e2f2a5cc51101083cf` bytes=4859
- `SocketManagerEnum.h` sha256 `657272ef42bf802b436d0bfe4df96227fcfe96b730f39c734ffa16a792715156` bytes=377
- `Protocol.h` sha256 `943c59b7a457eccea4953b7410fd2140c6299979782b58ca81a7b42a52d6c3b0` bytes=36305
- `Protocol.cpp` sha256 `e8bb4dbb28d1baf23222dbc72762e2ebd55b116dbb8d36e5f017dd744e510f13` bytes=135014
- `Attack.h` sha256 `52f3dd5bc43f0f5c2aeaabc4d460225e5d4475984be6ff6df9579315ed4aa422` bytes=3142
- `SkillManager.h` sha256 `2d706d9f4ca551a5d3f0b2d8e6a372942102f1d3599b80f769bda98b789b7246` bytes=18550

---

## FILE: SocketManagerModern.cpp

### HIT: Enum BOTH_ATTACK1/2 / `\bBOTH_ATTACK1\b|\bBOTH_ATTACK2\b`

- line 116: `		case ProtocolHead::BOTH_ATTACK1:`
- line 119: `		case ProtocolHead::BOTH_ATTACK2:`

#### CONTEXT (first hit)

```
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

### HIT: case BOTH_ATTACK1 / `case\s+ProtocolHead::BOTH_ATTACK1\b`

- line 116: `		case ProtocolHead::BOTH_ATTACK1:`

#### CONTEXT (first hit)

```
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

### HIT: case BOTH_ATTACK2 / `case\s+ProtocolHead::BOTH_ATTACK2\b`

- line 119: `		case ProtocolHead::BOTH_ATTACK2:`

#### CONTEXT (first hit)

```
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

### HIT: Calls to handlers / `CGAttackRecv|CGMultiSkillAttackRecv`

- line 117: `			gAttack.CGAttackRecv((PMSG_ATTACK_RECV*)msg.body.data(),aIndex);`
- line 120: `			gSkillManager.CGMultiSkillAttackRecv((PMSG_MULTI_SKILL_ATTACK_RECV*)msg.body.data(),aIndex,0);`

#### CONTEXT (first hit)

```
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

### HIT: Body interpretation / casts / `memcpy|reinterpret_cast|static_cast|\bmsg\.body\b|\bbody\b\s*\[|\.data\(`

- line 73: `	std::memcpy(lpMsg.body.data(), message, size);`
- line 105: `			CGConnectAccountRecv((PMSG_CONNECT_ACCOUNT_RECV*)msg.body.data(), aIndex);`
- line 111: `			CGPositionRecv((PMSG_POSITION_RECV*)msg.body.data(),aIndex);`
- line 114: `			CGMoveRecv((PMSG_MOVE_RECV*)msg.body.data(),aIndex);`
- line 117: `			gAttack.CGAttackRecv((PMSG_ATTACK_RECV*)msg.body.data(),aIndex);`
- line 120: `			gSkillManager.CGMultiSkillAttackRecv((PMSG_MULTI_SKILL_ATTACK_RECV*)msg.body.data(),aIndex,0);`
- line 127: `				uint16_t copySize = (msg.body.size() < sizeof(pMsg.name)) ? msg.body.size() : sizeof(pMsg.name);`
- line 128: `				memcpy(pMsg.name, msg.body.data(), copySize);`
- line 137: `					std::memcpy(&recv[start], &msg.body[start], 1);`
- line 200: `//	//memcpy(lpBuff,this->m_buff,size);`

#### CONTEXT (first hit)

```
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

### HIT: Size/guard checks / `\bif\s*\(.*(size|length|body\.size)\b|\bbody\.size\(\)|\bheader\.size\b`

- line 61: `	if (size > MAX_MAIN_PACKET_SIZE)`
- line 71: `	lpMsg.body.resize(lpMsg.body.size() + size);`
- line 75: `	lpMsg.header.size = lpMsg.size();`
- line 127: `				uint16_t copySize = (msg.body.size() < sizeof(pMsg.name)) ? msg.body.size() : sizeof(pMsg.name);`
- line 136: `				for (uint16_t start = 0; start < msg.header.size; start++) {`
- line 166: `				if(gPacketManager.AddData(&recv[count],size) == 0 || gPacketManager.ExtractPacket(DecBuff) == 0)`

#### CONTEXT (first hit)

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

### HIT: Struct hints / `\bPMSG_\w+|\bPBMSG_\w+|\bATTACK\b`

- line 86: `			PMSG_TESTE_RECV lpMsg;`
- line 97: `			PMSG_TESTE_SEND TesteSend;`
- line 105: `			CGConnectAccountRecv((PMSG_CONNECT_ACCOUNT_RECV*)msg.body.data(), aIndex);`
- line 111: `			CGPositionRecv((PMSG_POSITION_RECV*)msg.body.data(),aIndex);`
- line 114: `			CGMoveRecv((PMSG_MOVE_RECV*)msg.body.data(),aIndex);`
- line 117: `			gAttack.CGAttackRecv((PMSG_ATTACK_RECV*)msg.body.data(),aIndex);`
- line 120: `			gSkillManager.CGMultiSkillAttackRecv((PMSG_MULTI_SKILL_ATTACK_RECV*)msg.body.data(),aIndex,0);`
- line 124: `				PMSG_CHARACTER_INFO_RECV pMsg;`

#### CONTEXT (first hit)

```
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


---

## FILE: SocketManagerEnum.h

### HIT: Enum BOTH_ATTACK1/2 / `\bBOTH_ATTACK1\b|\bBOTH_ATTACK2\b`

- line 15: `	BOTH_ATTACK1,`
- line 16: `	BOTH_ATTACK2,`

#### CONTEXT (first hit)

```
     1: #pragma once
     2: 
     3: enum class ProtocolHead : uint16_t
     4: {
     5: 	CLIENT_ACCEPT,			//Enviado pelo cliente
     6: 	CLIENT_LIVE_CLIENT,
     7: 
     8: 	SERVER_CONNECT,			//Enviado pelo servidor
     9: 	SERVER_DISCONNECT,
    10: 
    11: 	BOTH_CONNECT_LOGIN,			//Enviado pelo cliente e servidor
    12: 	BOTH_CONNECT_CHARACTER,
    13: 	BOTH_POSITION,
    14: 	BOTH_MOVE,
    15: 	BOTH_ATTACK1,
    16: 	BOTH_ATTACK2,
    17: 	BOTH_ATTACK3,
    18: 
    19: 	BOTH_CONNECT_JOIN_GAME,
    20: 
    21: 	BOTH_MESSAGE,
    22: };
```


---

## FILE: Protocol.h

### HIT: Struct hints / `\bPMSG_\w+|\bPBMSG_\w+|\bATTACK\b`

- line 79: `struct PBMSG_HEAD`
- line 173: `//struct PBMSG_HEAD2	// Packet - Byte Type`
- line 196: `struct PMSG_CHAT_RECV`
- line 198: `	PBMSG_HEAD header; // C1:00`
- line 203: `struct PMSG_CHAT_WHISPER_RECV`
- line 205: `	PBMSG_HEAD header; // C1:00`
- line 210: `struct PMSG_MAIN_CHECK_RECV`
- line 212: `	PBMSG_HEAD header; // C1:03`
- line 216: `struct PMSG_LIVE_CLIENT_RECV`
- line 218: `	PBMSG_HEAD header; // C1:0E`
- line 224: `struct PMSG_POSITION_RECV`
- line 227: `	PBMSG_HEAD header; // C1:[PROTOCOL_CODE3]`
- line 233: `struct PMSG_ACTION_RECV`
- line 235: `	PBMSG_HEAD header; // C1:18`

#### CONTEXT (first hit)

```
     9: #define PROTOCOL_CODE1 0xD3
    10: #define PROTOCOL_CODE2 0xD7
    11: #define PROTOCOL_CODE3 0xDF
    12: #define PROTOCOL_CODE4 0x10
    13: #elif(GAMESERVER_LANGUAGE==1)
    14: #define PROTOCOL_CODE1 0xD4
    15: #define PROTOCOL_CODE2 0x11
    16: #define PROTOCOL_CODE3 0x15
    17: #define PROTOCOL_CODE4 0xDB
    18: #elif(GAMESERVER_LANGUAGE==2)
    19: #define PROTOCOL_CODE1 0x1D
    20: #define PROTOCOL_CODE2 0xDC
    21: #define PROTOCOL_CODE3 0xD6
    22: #define PROTOCOL_CODE4 0xD7
    23: #elif(GAMESERVER_LANGUAGE==3)
    24: #define PROTOCOL_CODE1 0xD9
    25: #define PROTOCOL_CODE2 0xD7
    26: #define PROTOCOL_CODE3 0xD0
    27: #define PROTOCOL_CODE4 0x1D
    28: #elif(GAMESERVER_LANGUAGE==4)
    29: #define PROTOCOL_CODE1 0x00
    30: #define PROTOCOL_CODE2 0x00
    31: #define PROTOCOL_CODE3 0x00
    32: #define PROTOCOL_CODE4 0x00
    33: #elif(GAMESERVER_LANGUAGE==5)
    34: #define PROTOCOL_CODE1 0xD6
    35: #define PROTOCOL_CODE2 0xDD
    36: #define PROTOCOL_CODE3 0xDF
    37: #define PROTOCOL_CODE4 0xD2
    38: #elif(GAMESERVER_LANGUAGE==6)
    39: #define PROTOCOL_CODE1 0xDD
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
```


---

## FILE: Protocol.cpp

### HIT: Calls to handlers / `CGAttackRecv|CGMultiSkillAttackRecv`

- line 113: `			gAttack.CGAttackRecv((PMSG_ATTACK_RECV*)lpMsg,aIndex);`
- line 772: `			gSkillManager.CGMultiSkillAttackRecv((PMSG_MULTI_SKILL_ATTACK_RECV*)lpMsg,aIndex,0);`

#### CONTEXT (first hit)

```
    43: #include "ItemManager.h"
    44: #include "JewelMix.h"
    45: #include "JSProtocol.h"
    46: #include "KanturuEntranceNPC.h"
    47: #include "Log.h"
    48: #include "LuckyCoin.h"
    49: #include "Map.h"
    50: #include "MapServerManager.h"
    51: #include "MasterSkillTree.h"
    52: #include "Message.h"
    53: #include "MiniMap.h"
    54: #include "MiningSystem.h"
    55: #include "Move.h"
    56: #include "MuRummy.h"
    57: #include "MuunSystem.h"
    58: #include "Notice.h"
    59: #include "NpcTalk.h"
    60: #include "ObjectManager.h"
    61: #include "Party.h"
    62: #include "PartyMatching.h"
    63: #include "PcPoint.h"
    64: #include "PentagramSystem.h"
    65: #include "PersonalShop.h"
    66: #include "SocketManagerModern.h"
    67: #include "Quest.h"
    68: #include "QuestWorld.h"
    69: #include "ServerInfo.h"
    70: #include "SkillManager.h"
    71: #include "Trade.h"
    72: #include "Util.h"
    73: #include "Viewport.h"
    74: #include "Warehouse.h"
    75: 
    76: void ProtocolCore(BYTE head,BYTE* lpMsg,int size,int aIndex,int encrypt,int serial) // OK
    77: {
    78: 	//if(gObj[aIndex].Type == OBJECT_USER && gHackPacketCheck.CheckPacketHack(aIndex,head,((lpMsg[0]==0xC1)?lpMsg[3]:lpMsg[4]),encrypt,serial) == 0)
    79: 	//{
    80: 	//	return;
    81: 	//}
    82: 
    83: 		if((head != 0xF3 || lpMsg[3] != 0xF1) && head != 0x0E && head != 0x18 && head != PROTOCOL_CODE1)
    84: 		{
    85: 			char String[1024];
    86: 			char hex_chars[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    87: 
    88: 			for( int i = 0; i < size; ++i )
    89: 			{
    90: 				sprintf(&String[2*i], "%02X", lpMsg[i]);
    91: 			}
    92: 			LogAdd(LOG_RED,"PacketIN: INDEX: %d TYPE: %#X, HEAD: %#04X, SIZE: %02d, PACKET: %s",aIndex,lpMsg[0],head,size,String);
    93: 		}
    94: 
    95: 	//LogAdd(LOG_BLACK,"PROTO head: %x, 1: %x, 2: %x, 3: %x, 4: %x, 5: %x",head,lpMsg[1],lpMsg[2],lpMsg[3],lpMsg[4],lpMsg[5]);
    96: 
    97: 
    98: 	switch(head)
    99: 	{
   100: 		case 0x00:
   101: 			CGChatRecv((PMSG_CHAT_RECV*)lpMsg,aIndex);
   102: 			break;
   103: 		case 0x02:
   104: 			CGChatWhisperRecv((PMSG_CHAT_WHISPER_RECV*)lpMsg,aIndex);
   105: 			break;
   106: 		case 0x03:
   107: 			CGMainCheckRecv((PMSG_MAIN_CHECK_RECV*)lpMsg,aIndex);
   108: 			break;
   109: 		case 0x0E:
   110: 			CGLiveClientRecv((PMSG_LIVE_CLIENT_RECV*)lpMsg,aIndex);
   111: 			break;
   112: 		case PROTOCOL_CODE2:
   113: 			gAttack.CGAttackRecv((PMSG_ATTACK_RECV*)lpMsg,aIndex);
   114: 			break;
   115: 		case PROTOCOL_CODE3:
   116: 			CGPositionRecv((PMSG_POSITION_RECV*)lpMsg,aIndex);
   117: 			break;
   118: 		case 0x18:
   119: 			CGActionRecv((PMSG_ACTION_RECV*)lpMsg,aIndex);
   120: 			break;
   121: 		case 0x19:
   122: 			gSkillManager.CGSkillAttackRecv((PMSG_SKILL_ATTACK_RECV*)lpMsg,aIndex);
   123: 			break;
   124: 		case 0x1B:
   125: 			gSkillManager.CGSkillCancelRecv((PMSG_SKILL_CANCEL_RECV*)lpMsg,aIndex);
   126: 			break;
   127: 		case 0x1C:
   128: 			gMove.CGTeleportRecv((PMSG_TELEPORT_RECV*)lpMsg,aIndex);
   129: 			break;
   130: 		case 0x1E:
   131: 			gSkillManager.CGDurationSkillAttackRecv((PMSG_DURATION_SKILL_ATTACK_RECV*)lpMsg,aIndex);
   132: 			break;
   133: 		case 0x22:
   134: 			gItemManager.CGItemGetRecv((PMSG_ITEM_GET_RECV*)lpMsg,aIndex);
   135: 			break;
   136: 		case 0x23:
   137: 			gItemManager.CGItemDropRecv((PMSG_ITEM_DROP_RECV*)lpMsg,aIndex);
   138: 			break;
   139: 		case 0x24:
   140: 			gItemManager.CGItemMoveRecv((PMSG_ITEM_MOVE_RECV*)lpMsg,aIndex);
   141: 			break;
   142: 		case 0x26:
   143: 			gItemManager.CGItemUseRecv((PMSG_ITEM_USE_RECV*)lpMsg,aIndex);
   144: 			break;
   145: 		case 0x30:
   146: 			gNpcTalk.CGNpcTalkRecv((PMSG_NPC_TALK_RECV*)lpMsg,aIndex);
   147: 			break;
   148: 		case 0x31:
   149: 			gNpcTalk.CGNpcTalkCloseRecv(aIndex);
   150: 			break;
   151: 		case 0x32:
   152: 			gItemManager.CGItemBuyRecv((PMSG_ITEM_BUY_RECV*)lpMsg,aIndex,0);
   153: 			break;
   154: 		case 0x33:
   155: 			gItemManager.CGItemSellRecv((PMSG_ITEM_SELL_RECV*)lpMsg,aIndex);
   156: 			break;
   157: 		case 0x34:
   158: 			gItemManager.CGItemRepairRecv((PMSG_ITEM_REPAIR_RECV*)lpMsg,aIndex);
   159: 			break;
   160: 		case 0x36:
   161: 			gTrade.CGTradeRequestRecv((PMSG_TRADE_REQUEST_RECV*)lpMsg,aIndex);
   162: 			break;
   163: 		case 0x37:
   164: 			gTrade.CGTradeResponseRecv((PMSG_TRADE_RESPONSE_RECV*)lpMsg,aIndex);
   165: 			break;
   166: 		case 0x3A:
   167: 			gTrade.CGTradeMoneyRecv((PMSG_TRADE_MONEY_RECV*)lpMsg,aIndex);
   168: 			break;
   169: 		case 0x3C:
   170: 			gTrade.CGTradeOkButtonRecv((PMSG_TRADE_OK_BUTTON_RECV*)lpMsg,aIndex);
   171: 			break;
   172: 		case 0x3D:
   173: 			gTrade.CGTradeCancelButtonRecv(aIndex);
   174: 			break;
   175: 		case 0x3F:
   176: 			switch(lpMsg[3])
   177: 			{
   178: 				case 0x01:
   179: 					gPersonalShop.CGPShopSetItemPriceRecv((PMSG_PSHOP_SET_ITEM_PRICE_RECV*)lpMsg,aIndex);
   180: 					break;
   181: 				case 0x02:
   182: 					gPersonalShop.CGPShopOpenRecv((PMSG_PSHOP_OPEN_RECV*)lpMsg,aIndex);
   183: 					break;
   184: 				case 0x03:
   185: 					gPersonalShop.CGPShopCloseRecv(aIndex);
   186: 					break;
   187: 				case 0x05:
   188: 					gPersonalShop.CGPShopItemListRecv((PMSG_PSHOP_ITEM_LIST_RECV*)lpMsg,aIndex);
   189: 					break;
   190: 				case 0x06:
   191: 					gPersonalShop.CGPShopBuyItemRecv((PMSG_PSHOP_BUY_ITEM_RECV*)lpMsg,aIndex);
   192: 					break;
   193: 				case 0x07:
   194: 					gPersonalShop.CGPShopLeaveRecv((PMSG_PSHOP_LEAVE_RECV*)lpMsg,aIndex);
   195: 					break;
   196: 			}
   197: 			break;
   198: 		case 0x40:
   199: 			gParty.CGPartyRequestRecv((PMSG_PARTY_REQUEST_RECV*)lpMsg,aIndex);
   200: 			break;
   201: 		case 0x41:
   202: 			gParty.CGPartyRequestResultRecv((PMSG_PARTY_REQUEST_RESULT_RECV*)lpMsg,aIndex);
   203: 			break;
   204: 		case 0x42:
   205: 			gParty.CGPartyListRecv(aIndex);
   206: 			break;
   207: 		case 0x43:
   208: 			gParty.CGPartyDelMemberRecv((PMSG_PARTY_DEL_MEMBER_RECV*)lpMsg,aIndex);
   209: 			break;
   210: 		case 0x4A:
   211: 			#if(GAMESERVER_UPDATE>=601)
   212: 			gSkillManager.CGRageFighterSkillAttackRecv((PMSG_RAGE_FIGHTER_SKILL_ATTACK_RECV*)lpMsg,aIndex);
   213: 			#endif
   214: 			break;
   215: 		case 0x4B:
   216: 			#if(GAMESERVER_UPDATE>=601)
   217: 			gSkillManager.CGSkillDarkSideRecv((PMSG_SKILL_DARK_SIDE_RECV*)lpMsg,aIndex);
   218: 			#endif
   219: 			break;
   220: 		case 0x4C:
   221: 			switch(lpMsg[3])
   222: 			{
   223: 				case 0x00:
   224: 					#if(GAMESERVER_UPDATE>=801)
   225: 					gMiningSystem.CGMiningStartRecv((PMSG_MINING_START_RECV*)lpMsg,aIndex);
   226: 					#endif
   227: 					break;
   228: 				case 0x01:
   229: 					#if(GAMESERVER_UPDATE>=801)
   230: 					gMiningSystem.CGMiningSuccessRecv((PMSG_MINING_SUCCESS_RECV*)lpMsg,aIndex);
   231: 					#endif
   232: 					break;
   233: 				case 0x03:
   234: 					#if(GAMESERVER_UPDATE>=801)
   235: 					gMiningSystem.CGMiningFailureRecv((PMSG_MINING_FAILURE_RECV*)lpMsg,aIndex);
   236: 					#endif
   237: 					break;
   238: 			}
   239: 			break;
   240: 		case 0x4D:
   241: 			switch(lpMsg[3])
   242: 			{
   243: 				case 0x00:
   244: 					#if(GAMESERVER_UPDATE>=802)
   245: 					gEventInventory.CGEventItemGetRecv((PMSG_EVENT_ITEM_GET_RECV*)lpMsg,aIndex);
   246: 					#endif
   247: 					break;
   248: 				case 0x01:
   249: 					#if(GAMESERVER_UPDATE>=802)
   250: 					gEventInventory.CGEventItemDropRecv((PMSG_EVENT_ITEM_DROP_RECV*)lpMsg,aIndex);
   251: 					#endif
   252: 					break;
   253: 				case 0x0F:
   254: 					#if(GAMESERVER_UPDATE>=802)
   255: 					gEventInventory.CGEventInventoryOpenRecv((PMSG_EVENT_INVENTORY_OPEN_RECV*)lpMsg,aIndex);
   256: 					#endif
   257: 					break;
   258: 				case 0x10:
   259: 					#if(GAMESERVER_UPDATE>=802)
   260: 					gMuRummy.CGReqMuRummyStart((_tagPMSG_REQ_MURUMMY_INFO*)lpMsg,aIndex);
   261: 					#endif
   262: 					break;
   263: 				case 0x11:
   264: 					#if(GAMESERVER_UPDATE>=802)
   265: 					gMuRummy.CGReqCardReveal((_tagPMSG_REQ_REVEAL_CARD*)lpMsg,aIndex);
   266: 					#endif
   267: 					break;
   268: 				case 0x12:
   269: 					#if(GAMESERVER_UPDATE>=802)
   270: 					gMuRummy.CGReqCardMove((_tagPMSG_REQ_CARD_MOVE*)lpMsg,aIndex);
   271: 					#endif
   272: 					break;
   273: 				case 0x13:
   274: 					#if(GAMESERVER_UPDATE>=802)
   275: 					gMuRummy.CGReqCardReMove((_tagPMSG_REQ_CARD_REMOVE*)lpMsg,aIndex);
   276: 					#endif
   277: 					break;
   278: 				case 0x14:
   279: 					#if(GAMESERVER_UPDATE>=802)
   280: 					gMuRummy.CGReqCardMatch((_tagPMSG_REQ_CARD_MATCH*)lpMsg,aIndex);
   281: 					#endif
   282: 					break;
   283: 				case 0x15:
   284: 					#if(GAMESERVER_UPDATE>=802)
   285: 					gMuRummy.CGReqMuRummyEnd((_tagPMSG_REQ_MURUMMY_END*)lpMsg,aIndex);
   286: 					#endif
   287: 					break;
   288: 			}
   289: 			break;
   290: 		case 0x4E:
   291: 			switch(lpMsg[3])
   292: 			{
   293: 				case 0x00:
   294: 					#if(GAMESERVER_UPDATE>=803)
   295: 					gMuunSystem.CGMuunItemGetRecv((PMSG_MUUN_ITEM_GET_RECV*)lpMsg,aIndex);
   296: 					#endif
   297: 					break;
   298: 				case 0x01:
   299: 					break;
   300: 				case 0x08:
   301: 					#if(GAMESERVER_UPDATE>=803)
   302: 					gMuunSystem.CGMuunItemUseRecv((PMSG_MUUN_ITEM_USE_RECV*)lpMsg,aIndex);
   303: 					#endif
   304: 				case 0x09:
   305: 					#if(GAMESERVER_UPDATE>=803)
   306: 					gMuunSystem.CGMuunItemSellRecv((PMSG_MUUN_ITEM_SELL_RECV*)lpMsg,aIndex);
   307: 					#endif
   308: 					break;
   309: 			}
   310: 			break;
   311: 		case 0x50:
   312: 			gGuild.CGGuildRequestRecv((PMSG_GUILD_REQUEST_RECV*)lpMsg,aIndex);
   313: 			break;
   314: 		case 0x51:
   315: 			gGuild.CGGuildResultRecv((PMSG_GUILD_RESULT_RECV*)lpMsg,aIndex);
   316: 			break;
   317: 		case 0x52:
   318: 			gGuild.CGGuildListRecv(aIndex);
   319: 			break;
   320: 		case 0x53:
   321: 			gGuild.CGGuildDeleteRecv((PMSG_GUILD_DELETE_RECV*)lpMsg,aIndex);
   322: 			break;
   323: 		case 0x54:
   324: 			CGGuildMasterAnswerRecv((PMSG_GUILDMASTERANSWER*)lpMsg,aIndex);
   325: 			break;
   326: 		case 0x55:
   327: 			CGGuildMasterInfoSave(aIndex,(PMSG_GUILDINFOSAVE*)lpMsg);
   328: 			break;
   329: 		case 0x57:
   330: 			CGGuildMasterCreateCancel(aIndex);
   331: 			break;
   332: 		case 0x61:
   333: 			GCGuildWarRequestSendRecv((PMSG_GUILDWARSEND_RESULT*)lpMsg,aIndex);
   334: 			break;
   335: 		case 0x66:
   336: 			GCGuildViewportInfo((PMSG_REQ_GUILDVIEWPORT*)lpMsg,aIndex);
   337: 			break;
   338: 		case 0x81:
   339: 			gWarehouse.CGWarehouseMoneyRecv((PMSG_WAREHOUSE_MONEY_RECV*)lpMsg,aIndex);
   340: 			break;
   341: 		case 0x82:
   342: 			gWarehouse.CGWarehouseClose(aIndex);
   343: 			break;
   344: 		case 0x83:
   345: 			gWarehouse.CGWarehousePasswordRecv((PMSG_WAREHOUSE_PASSWORD_RECV*)lpMsg,aIndex);
   346: 			break;
   347: 		case 0x86:
   348: 			gChaosBox.CGChaosMixRecv((PMSG_CHAOS_MIX_RECV*)lpMsg,aIndex);
   349: 			break;
   350: 		case 0x87:
   351: 			gChaosBox.CGChaosMixCloseRecv(aIndex);
   352: 			break;
   353: 		case 0x8A:
```

### HIT: Body interpretation / casts / `memcpy|reinterpret_cast|static_cast|\bmsg\.body\b|\bbody\b\s*\[|\.data\(`

- line 1178: `	memcpy(name,lpMsg->name,sizeof(lpMsg->name));`
- line 1212: `	memcpy(pMsg.name,lpMsg->name,sizeof(pMsg.name));`
- line 1214: `	memcpy(pMsg.message,lpMsg->message,sizeof(pMsg.message));`
- line 1287: `	memcpy(name,lpMsg->name,sizeof(lpMsg->name));`
- line 2047: `	memcpy(pMsg.name,lpMsg->name,sizeof(pMsg.name));`
- line 2087: `	memcpy(name,lpMsg->name,sizeof(lpMsg->name));`
- line 2130: `	memcpy(name,lpMsg->name,sizeof(lpMsg->name));`
- line 2148: `	memcpy(name,lpMsg->name,sizeof(lpMsg->name));`
- line 2228: `	memcpy(name,lpMsg->name,sizeof(lpMsg->name));`
- line 2250: `	memcpy(OldName,lpMsg->OldName,sizeof(lpMsg->OldName));`
- line 2252: `	memcpy(NewName,lpMsg->NewName,sizeof(lpMsg->NewName));`
- line 2385: `	memcpy(pMsg.message,message,size);`
- line 2413: `	memcpy(pMsg.message,buff,size);`
- line 2430: `	memcpy(pMsg.name,name,sizeof(pMsg.name));`

#### CONTEXT (first hit)

```
  1108: 					break;
  1109: 				case 0x0B:
  1110: 					#if(GAMESERVER_UPDATE>=501)
  1111: 					gGensSystem.CGGensSystemMemberRecv((PMSG_GENS_SYSTEM_MEMBER_RECV*)lpMsg,aIndex);
  1112: 					#endif
  1113: 					break;
  1114: 				case 0x20:
  1115: 					#if(GAMESERVER_UPDATE>=701)
  1116: 					CGAcheronEnterRecv(aIndex);
  1117: 					#endif
  1118: 					break;
  1119: 				case 0x30:
  1120: 					#if(GAMESERVER_UPDATE>=702)
  1121: 					gArcaBattle.CGArcaBattleGuildMasterRegisterRecv(aIndex);
  1122: 					#endif
  1123: 					break;
  1124: 				case 0x32:
  1125: 					#if(GAMESERVER_UPDATE>=702)
  1126: 					gArcaBattle.CGArcaBattleGuildMemberRegisterRecv(aIndex);
  1127: 					#endif
  1128: 					break;
  1129: 				case 0x34:
  1130: 					//CGReqArcaBattleEnter((_tagPMSG_REQ_ARCA_BATTLE_ENTER *)aRecv, aIndex);
  1131: 					break;
  1132: 				case 0x36:
  1133: 					//CGReqArcaBattleBootyExchange(aIndex);
  1134: 					break;
  1135: 				case 0x3C:
  1136: 					//CGReqSpritemapExchange(aIndex);
  1137: 					break;
  1138: 				case 0x41:
  1139: 					//CGReqRegisteredMemberCnt(aIndex);
  1140: 					break;
  1141: 				case 0x43:
  1142: 					#if(GAMESERVER_UPDATE>=702)
  1143: 					gArcaBattle.CGArcaBattleRegisterMarkRecv(aIndex);
  1144: 					#endif
  1145: 					break;
  1146: 				case 0x45:
  1147: 					//CArcaBattle::CGReqMarkRank(&g_ArcaBattle, aIndex);
  1148: 					break;
  1149: 				case 0x4B:
  1150: 					//CAcheronGuardianEvent::CGReqAcheronEventEnter(&g_AcheronGuardianEvent,(_tagPMSG_REQ_ACHERON_EVENT_ENTER *)aRecv,aIndex);
  1151: 					break;
  1152: 			}
  1153: 			break;
  1154: 		case 0xFB:
  1155: 			#if(GAMESERVER_UPDATE>=801)
  1156: 			CGSNSDataRecv((PMSG_SNS_DATA_RECV*)lpMsg,aIndex);
  1157: 			#endif
  1158: 			break;
  1159: 		case 0xFC:
  1160: 			#if(GAMESERVER_UPDATE>=801)
  1161: 			CGSNSDataLogRecv((PMSG_SNS_DATA_LOG_RECV*)lpMsg,aIndex);
  1162: 			#endif
  1163: 			break;
  1164: 	}
  1165: }
  1166: 
  1167: void CGChatRecv(PMSG_CHAT_RECV* lpMsg,int aIndex) // OK
  1168: {
  1169: 	LPOBJ lpObj = &gObj[aIndex];
  1170: 
  1171: 	if(gObjIsConnectedGP(aIndex) == 0)
  1172: 	{
  1173: 		return;
  1174: 	}
  1175: 
  1176: 	char name[11] = {0};
  1177: 
  1178: 	memcpy(name,lpMsg->name,sizeof(lpMsg->name));
  1179: 
  1180: 	if(strcmp(name,lpObj->Name) != 0)
  1181: 	{
  1182: 		return;
  1183: 	}
  1184: 
  1185: 	lpMsg->message[(sizeof(lpMsg->message)-1)] = 0;
  1186: 
  1187: 	gLog.Output(LOG_CHAT,"[General][%s][%s] - (Message: %s)",lpObj->Account,lpObj->Name,lpMsg->message);
  1188: 
  1189: 	if(lpMsg->message[0] == '/')
  1190: 	{
  1191: 		CommandSelect(lpObj,lpMsg->message,-1);
  1192: 		return;
  1193: 	}
  1194: 
  1195: 	gFilter.CheckSyntax(lpMsg->message);
  1196: 
  1197: 	if(lpObj->ChatLimitTime > 0)
  1198: 	{
  1199: 		gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,gMessage.GetMessage(269),lpObj->ChatLimitTime);
  1200: 		return;
  1201: 	}
  1202: 
  1203: 	if((lpObj->Penalty & 2) != 0)
  1204: 	{
  1205: 		return;
  1206: 	}
  1207: 
  1208: 	PMSG_CHAT_SEND pMsg;
  1209: 
  1210: 	pMsg.header.set(0x00,sizeof(pMsg));
  1211: 
  1212: 	memcpy(pMsg.name,lpMsg->name,sizeof(pMsg.name));
  1213: 
  1214: 	memcpy(pMsg.message,lpMsg->message,sizeof(pMsg.message));
  1215: 
  1216: 	if(lpMsg->message[0] == '~')
  1217: 	{
  1218: 		if(OBJECT_RANGE(lpObj->PartyNumber) != 0)
  1219: 		{
  1220: 			for(int n=0;n < MAX_PARTY_USER;n++)
  1221: 			{
  1222: 				if(OBJECT_RANGE(gParty.m_PartyInfo[lpObj->PartyNumber].Index[n]) != 0)
  1223: 				{
  1224: 					DataSend(gParty.m_PartyInfo[lpObj->PartyNumber].Index[n],(BYTE*)&pMsg,pMsg.header.size);
  1225: 				}
  1226: 			}
  1227: 		}
  1228: 	}
  1229: 	else if(lpMsg->message[0] == '@')
  1230: 	{
  1231: 		if(lpObj->Guild != 0)
  1232: 		{
  1233: 			if(lpMsg->message[1] == '>')
  1234: 			{
  1235: 				if(strcmp(lpObj->Name,lpObj->Guild->Names[0]) == 0)
  1236: 				{
  1237: 					GDGuildNoticeSave(lpObj->Guild->Name,&lpMsg->message[2]);
  1238: 				}
  1239: 			}
  1240: 			else if(lpMsg->message[1] == '@')
  1241: 			{
  1242: 				if(lpObj->Guild->GuildUnion != 0)
  1243: 				{
  1244: 					GDUnionServerGroupChattingSend(lpObj->Guild->GuildUnion,lpMsg->name,lpMsg->message);
  1245: 				}
  1246: 			}
  1247: 			else
  1248: 			{
  1249: 				if(lpObj->Guild->Number != 0)
  1250: 				{
  1251: 					GDGuildServerGroupChattingSend(lpObj->Guild->Number,lpMsg->name,lpMsg->message);
  1252: 				}
  1253: 			}
  1254: 		}
  1255: 	}
  1256: 	else if(lpMsg->message[0] == '$')
  1257: 	{
  1258: 		if(lpObj->GensFamily != 0)
  1259: 		{
  1260: 			for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
  1261: 			{
  1262: 				if(gObj[n].GensFamily == lpObj->GensFamily)
  1263: 				{
  1264: 					DataSend(n,(BYTE*)&pMsg,pMsg.header.size);
  1265: 				}
  1266: 			}
  1267: 		}
  1268: 	}
  1269: 	else
  1270: 	{
  1271: 		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
  1272: 		MsgSendV2(lpObj,(BYTE*)&pMsg,pMsg.header.size);
  1273: 	}
  1274: }
  1275: 
  1276: void CGChatWhisperRecv(PMSG_CHAT_WHISPER_RECV* lpMsg,int aIndex) // OK
  1277: {
  1278: 	LPOBJ lpObj = &gObj[aIndex];
  1279: 
  1280: 	if(gObjIsConnectedGP(aIndex) == 0)
  1281: 	{
  1282: 		return;
  1283: 	}
  1284: 
  1285: 	char name[11] = {0};
  1286: 
  1287: 	memcpy(name,lpMsg->name,sizeof(lpMsg->name));
  1288: 
  1289: 	if(lpObj->ChatLimitTime > 0)
  1290: 	{
  1291: 		gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,gMessage.GetMessage(269),lpObj->ChatLimitTime);
  1292: 		return;
  1293: 	}
  1294: 
  1295: 	if((lpObj->Penalty & 2) != 0)
  1296: 	{
  1297: 		return;
  1298: 	}
  1299: 
  1300: 	LPOBJ lpTarget = gObjFind(name);
  1301: 
  1302: 	if(lpTarget == 0)
  1303: 	{
  1304: 		GDGlobalWhisperSend(aIndex,name,lpMsg->message);
  1305: 		return;
  1306: 	}
  1307: 
  1308: 	if(aIndex == lpTarget->Index)
  1309: 	{
  1310: 		gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,gMessage.GetMessage(270));
  1311: 		return;
  1312: 	}
  1313: 
  1314: 	lpMsg->message[(sizeof(lpMsg->message)-1)] = 0;
  1315: 
  1316: 	gLog.Output(LOG_CHAT,"[Whisper][%s][%s] - (Name: %s, Message: %s)",lpObj->Account,lpObj->Name,name,lpMsg->message);
  1317: 
  1318: 	gFilter.CheckSyntax(lpMsg->message);
  1319: 
  1320: 	GCChatWhisperSend(lpTarget->Index,lpObj->Name,lpMsg->message);
  1321: }
  1322: 
  1323: void CGMainCheckRecv(PMSG_MAIN_CHECK_RECV* lpMsg,int aIndex) // OK
  1324: {
  1325: 	LPOBJ lpObj = &gObj[aIndex];
  1326: 
  1327: 	if(gServerInfo.m_MainChecksum != 0)
  1328: 	{
  1329: 		if(lpObj->CheckSumTableNum < 0 || lpObj->CheckSumTableNum >= MAX_CHECKSUM_KEY)
  1330: 		{
  1331: 			gLog.Output(LOG_HACK,"[%s][%s] Main CheckSum error",lpObj->Account,lpObj->Name);
  1332: 			GCCloseClientSend(aIndex,0);
  1333: 			return;
  1334: 		}
  1335: 
  1336: 		if(gCheckSum[lpObj->CheckSumTableNum] != lpMsg->key)
  1337: 		{
  1338: 			gLog.Output(LOG_HACK,"[%s][%s] Invalid Main CheckSum",lpObj->Account,lpObj->Name);
  1339: 			GCCloseClientSend(aIndex,0);
  1340: 			return;
  1341: 		}
  1342: 	}
  1343: 
  1344: 	lpObj->CheckSumTime = 0;
  1345: }
  1346: 
  1347: void CGLiveClientRecv(PMSG_LIVE_CLIENT_RECV* lpMsg,int aIndex) // OK
  1348: {
  1349: 	LPOBJ lpObj = &gObj[aIndex];
  1350: 
  1351: 	if(lpObj->Connected != OBJECT_LOGGED && lpObj->Connected != OBJECT_ONLINE)
  1352: 	{
  1353: 		return;
  1354: 	}
  1355: 
  1356: 	if(gServerInfo.m_CheckLatencyHack != 0 && abs((int)((GetTickCount()-lpObj->ServerTickCount)-(lpMsg->TickCount-lpObj->ClientTickCount))) > gServerInfo.m_CheckLatencyHackTolerance)
  1357: 	{
  1358: 		gLog.Output(LOG_HACK,"[%s][%s] Client Latency Hack Detected [%d][%d]",lpObj->Account,lpObj->Name,(GetTickCount()-lpObj->ServerTickCount),(lpMsg->TickCount-lpObj->ClientTickCount));
  1359: 		GCCloseClientSend(aIndex,0);
  1360: 		return;
  1361: 	}
  1362: 
  1363: 	if(lpObj->Connected == OBJECT_ONLINE && gServerInfo.m_CheckSpeedHack != 0 && lpMsg->PhysiSpeed > lpObj->PhysiSpeed && abs((int)(lpObj->PhysiSpeed-lpMsg->PhysiSpeed)) > gServerInfo.m_CheckSpeedHackTolerance)
  1364: 	{
  1365: 		gLog.Output(LOG_HACK,"[%s][%s] Client Speed Hack Detected [%d][%d]",lpObj->Account,lpObj->Name,lpObj->PhysiSpeed,lpMsg->PhysiSpeed);
  1366: 		GCCloseClientSend(aIndex,0);
  1367: 		return;
  1368: 	}
  1369: 
  1370: 	if(lpObj->Connected == OBJECT_ONLINE && gServerInfo.m_CheckSpeedHack != 0 && lpMsg->MagicSpeed > lpObj->MagicSpeed && abs((int)(lpObj->MagicSpeed-lpMsg->MagicSpeed)) > gServerInfo.m_CheckSpeedHackTolerance)
  1371: 	{
  1372: 		gLog.Output(LOG_HACK,"[%s][%s] Client Speed Hack Detected [%d][%d]",lpObj->Account,lpObj->Name,lpObj->MagicSpeed,lpMsg->MagicSpeed);
  1373: 		GCCloseClientSend(aIndex,0);
  1374: 		return;
  1375: 	}
  1376: 
  1377: 	lpObj->ConnectTickCount = GetTickCount();
  1378: }
  1379: 
  1380: void CGPositionRecv(PMSG_POSITION_RECV* lpMsg,int aIndex) // OK
  1381: {
  1382: 	LPOBJ lpObj = &gObj[aIndex];
  1383: 
  1384: 	if(gObjIsConnectedGS(aIndex) == 0)
  1385: 	{
  1386: 		return;
  1387: 	}
  1388: 
  1389: 	if(lpObj->Teleport != 0)
  1390: 	{
  1391: 		return;
  1392: 	}
  1393: 
  1394: 	if(gObjCheckMapTile(lpObj,2) != 0)
  1395: 	{
  1396: 		return;
  1397: 	}
  1398: 
  1399: 	if(CC_MAP_RANGE(lpObj->Map) != 0 && (GetTickCount()-lpObj->ChaosCastleBlowTime) < 1000)
  1400: 	{
  1401: 		return;
  1402: 	}
  1403: 
  1404: 	if(lpMsg->x < (lpObj->X-15) || lpMsg->x > (lpObj->X+15) || lpMsg->y < (lpObj->Y-15) || lpMsg->y > (lpObj->Y+15))
  1405: 	{
  1406: 		return;
  1407: 	}
  1408: 
  1409: 	gMap[lpObj->Map].DelStandAttr(lpObj->OldX,lpObj->OldY);
  1410: 
  1411: 	lpObj->X = lpMsg->x;
  1412: 	lpObj->Y = lpMsg->y;
  1413: 	lpObj->TX = lpMsg->x;
  1414: 	lpObj->TY = lpMsg->y;
  1415: 	lpObj->OldX = lpMsg->x;
  1416: 	lpObj->OldY = lpMsg->y;
  1417: 
  1418: 	gMap[lpObj->Map].SetStandAttr(lpObj->TX,lpObj->TY);
```

### HIT: Size/guard checks / `\bif\s*\(.*(size|length|body\.size)\b|\bbody\.size\(\)|\bheader\.size\b`

- line 1224: `					DataSend(gParty.m_PartyInfo[lpObj->PartyNumber].Index[n],(BYTE*)&pMsg,pMsg.header.size);`
- line 1264: `					DataSend(n,(BYTE*)&pMsg,pMsg.header.size);`
- line 1271: `		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);`
- line 1272: `		MsgSendV2(lpObj,(BYTE*)&pMsg,pMsg.header.size);`
- line 1443: `			DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);`
- line 1456: `					DataSend(lpObj->VpPlayer2[n].index,(BYTE*)&pMsg,pMsg.header.size);`
- line 1514: `				DataSend(lpObj->VpPlayer2[n].index,(BYTE*)&pMsg,pMsg.header.size);`
- line 1634: `	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);`
- line 1940: `			DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);`
- line 1953: `					DataSend(lpObj->VpPlayer2[n].index,(BYTE*)&pMsg,pMsg.header.size);`
- line 2051: `		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);`
- line 2057: `		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);`
- line 2063: `		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);`
- line 2069: `		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);`

#### CONTEXT (first hit)

```
  1154: 		case 0xFB:
  1155: 			#if(GAMESERVER_UPDATE>=801)
  1156: 			CGSNSDataRecv((PMSG_SNS_DATA_RECV*)lpMsg,aIndex);
  1157: 			#endif
  1158: 			break;
  1159: 		case 0xFC:
  1160: 			#if(GAMESERVER_UPDATE>=801)
  1161: 			CGSNSDataLogRecv((PMSG_SNS_DATA_LOG_RECV*)lpMsg,aIndex);
  1162: 			#endif
  1163: 			break;
  1164: 	}
  1165: }
  1166: 
  1167: void CGChatRecv(PMSG_CHAT_RECV* lpMsg,int aIndex) // OK
  1168: {
  1169: 	LPOBJ lpObj = &gObj[aIndex];
  1170: 
  1171: 	if(gObjIsConnectedGP(aIndex) == 0)
  1172: 	{
  1173: 		return;
  1174: 	}
  1175: 
  1176: 	char name[11] = {0};
  1177: 
  1178: 	memcpy(name,lpMsg->name,sizeof(lpMsg->name));
  1179: 
  1180: 	if(strcmp(name,lpObj->Name) != 0)
  1181: 	{
  1182: 		return;
  1183: 	}
  1184: 
  1185: 	lpMsg->message[(sizeof(lpMsg->message)-1)] = 0;
  1186: 
  1187: 	gLog.Output(LOG_CHAT,"[General][%s][%s] - (Message: %s)",lpObj->Account,lpObj->Name,lpMsg->message);
  1188: 
  1189: 	if(lpMsg->message[0] == '/')
  1190: 	{
  1191: 		CommandSelect(lpObj,lpMsg->message,-1);
  1192: 		return;
  1193: 	}
  1194: 
  1195: 	gFilter.CheckSyntax(lpMsg->message);
  1196: 
  1197: 	if(lpObj->ChatLimitTime > 0)
  1198: 	{
  1199: 		gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,gMessage.GetMessage(269),lpObj->ChatLimitTime);
  1200: 		return;
  1201: 	}
  1202: 
  1203: 	if((lpObj->Penalty & 2) != 0)
  1204: 	{
  1205: 		return;
  1206: 	}
  1207: 
  1208: 	PMSG_CHAT_SEND pMsg;
  1209: 
  1210: 	pMsg.header.set(0x00,sizeof(pMsg));
  1211: 
  1212: 	memcpy(pMsg.name,lpMsg->name,sizeof(pMsg.name));
  1213: 
  1214: 	memcpy(pMsg.message,lpMsg->message,sizeof(pMsg.message));
  1215: 
  1216: 	if(lpMsg->message[0] == '~')
  1217: 	{
  1218: 		if(OBJECT_RANGE(lpObj->PartyNumber) != 0)
  1219: 		{
  1220: 			for(int n=0;n < MAX_PARTY_USER;n++)
  1221: 			{
  1222: 				if(OBJECT_RANGE(gParty.m_PartyInfo[lpObj->PartyNumber].Index[n]) != 0)
  1223: 				{
  1224: 					DataSend(gParty.m_PartyInfo[lpObj->PartyNumber].Index[n],(BYTE*)&pMsg,pMsg.header.size);
  1225: 				}
  1226: 			}
  1227: 		}
  1228: 	}
  1229: 	else if(lpMsg->message[0] == '@')
  1230: 	{
  1231: 		if(lpObj->Guild != 0)
  1232: 		{
  1233: 			if(lpMsg->message[1] == '>')
  1234: 			{
  1235: 				if(strcmp(lpObj->Name,lpObj->Guild->Names[0]) == 0)
  1236: 				{
  1237: 					GDGuildNoticeSave(lpObj->Guild->Name,&lpMsg->message[2]);
  1238: 				}
  1239: 			}
  1240: 			else if(lpMsg->message[1] == '@')
  1241: 			{
  1242: 				if(lpObj->Guild->GuildUnion != 0)
  1243: 				{
  1244: 					GDUnionServerGroupChattingSend(lpObj->Guild->GuildUnion,lpMsg->name,lpMsg->message);
  1245: 				}
  1246: 			}
  1247: 			else
  1248: 			{
  1249: 				if(lpObj->Guild->Number != 0)
  1250: 				{
  1251: 					GDGuildServerGroupChattingSend(lpObj->Guild->Number,lpMsg->name,lpMsg->message);
  1252: 				}
  1253: 			}
  1254: 		}
  1255: 	}
  1256: 	else if(lpMsg->message[0] == '$')
  1257: 	{
  1258: 		if(lpObj->GensFamily != 0)
  1259: 		{
  1260: 			for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
  1261: 			{
  1262: 				if(gObj[n].GensFamily == lpObj->GensFamily)
  1263: 				{
  1264: 					DataSend(n,(BYTE*)&pMsg,pMsg.header.size);
  1265: 				}
  1266: 			}
  1267: 		}
  1268: 	}
  1269: 	else
  1270: 	{
  1271: 		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
  1272: 		MsgSendV2(lpObj,(BYTE*)&pMsg,pMsg.header.size);
  1273: 	}
  1274: }
  1275: 
  1276: void CGChatWhisperRecv(PMSG_CHAT_WHISPER_RECV* lpMsg,int aIndex) // OK
  1277: {
  1278: 	LPOBJ lpObj = &gObj[aIndex];
  1279: 
  1280: 	if(gObjIsConnectedGP(aIndex) == 0)
  1281: 	{
  1282: 		return;
  1283: 	}
  1284: 
  1285: 	char name[11] = {0};
  1286: 
  1287: 	memcpy(name,lpMsg->name,sizeof(lpMsg->name));
  1288: 
  1289: 	if(lpObj->ChatLimitTime > 0)
  1290: 	{
  1291: 		gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,gMessage.GetMessage(269),lpObj->ChatLimitTime);
  1292: 		return;
  1293: 	}
  1294: 
  1295: 	if((lpObj->Penalty & 2) != 0)
  1296: 	{
  1297: 		return;
  1298: 	}
  1299: 
  1300: 	LPOBJ lpTarget = gObjFind(name);
  1301: 
  1302: 	if(lpTarget == 0)
  1303: 	{
  1304: 		GDGlobalWhisperSend(aIndex,name,lpMsg->message);
  1305: 		return;
  1306: 	}
  1307: 
  1308: 	if(aIndex == lpTarget->Index)
  1309: 	{
  1310: 		gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,gMessage.GetMessage(270));
  1311: 		return;
  1312: 	}
  1313: 
  1314: 	lpMsg->message[(sizeof(lpMsg->message)-1)] = 0;
  1315: 
  1316: 	gLog.Output(LOG_CHAT,"[Whisper][%s][%s] - (Name: %s, Message: %s)",lpObj->Account,lpObj->Name,name,lpMsg->message);
  1317: 
  1318: 	gFilter.CheckSyntax(lpMsg->message);
  1319: 
  1320: 	GCChatWhisperSend(lpTarget->Index,lpObj->Name,lpMsg->message);
  1321: }
  1322: 
  1323: void CGMainCheckRecv(PMSG_MAIN_CHECK_RECV* lpMsg,int aIndex) // OK
  1324: {
  1325: 	LPOBJ lpObj = &gObj[aIndex];
  1326: 
  1327: 	if(gServerInfo.m_MainChecksum != 0)
  1328: 	{
  1329: 		if(lpObj->CheckSumTableNum < 0 || lpObj->CheckSumTableNum >= MAX_CHECKSUM_KEY)
  1330: 		{
  1331: 			gLog.Output(LOG_HACK,"[%s][%s] Main CheckSum error",lpObj->Account,lpObj->Name);
  1332: 			GCCloseClientSend(aIndex,0);
  1333: 			return;
  1334: 		}
  1335: 
  1336: 		if(gCheckSum[lpObj->CheckSumTableNum] != lpMsg->key)
  1337: 		{
  1338: 			gLog.Output(LOG_HACK,"[%s][%s] Invalid Main CheckSum",lpObj->Account,lpObj->Name);
  1339: 			GCCloseClientSend(aIndex,0);
  1340: 			return;
  1341: 		}
  1342: 	}
  1343: 
  1344: 	lpObj->CheckSumTime = 0;
  1345: }
  1346: 
  1347: void CGLiveClientRecv(PMSG_LIVE_CLIENT_RECV* lpMsg,int aIndex) // OK
  1348: {
  1349: 	LPOBJ lpObj = &gObj[aIndex];
  1350: 
  1351: 	if(lpObj->Connected != OBJECT_LOGGED && lpObj->Connected != OBJECT_ONLINE)
  1352: 	{
  1353: 		return;
  1354: 	}
  1355: 
  1356: 	if(gServerInfo.m_CheckLatencyHack != 0 && abs((int)((GetTickCount()-lpObj->ServerTickCount)-(lpMsg->TickCount-lpObj->ClientTickCount))) > gServerInfo.m_CheckLatencyHackTolerance)
  1357: 	{
  1358: 		gLog.Output(LOG_HACK,"[%s][%s] Client Latency Hack Detected [%d][%d]",lpObj->Account,lpObj->Name,(GetTickCount()-lpObj->ServerTickCount),(lpMsg->TickCount-lpObj->ClientTickCount));
  1359: 		GCCloseClientSend(aIndex,0);
  1360: 		return;
  1361: 	}
  1362: 
  1363: 	if(lpObj->Connected == OBJECT_ONLINE && gServerInfo.m_CheckSpeedHack != 0 && lpMsg->PhysiSpeed > lpObj->PhysiSpeed && abs((int)(lpObj->PhysiSpeed-lpMsg->PhysiSpeed)) > gServerInfo.m_CheckSpeedHackTolerance)
  1364: 	{
  1365: 		gLog.Output(LOG_HACK,"[%s][%s] Client Speed Hack Detected [%d][%d]",lpObj->Account,lpObj->Name,lpObj->PhysiSpeed,lpMsg->PhysiSpeed);
  1366: 		GCCloseClientSend(aIndex,0);
  1367: 		return;
  1368: 	}
  1369: 
  1370: 	if(lpObj->Connected == OBJECT_ONLINE && gServerInfo.m_CheckSpeedHack != 0 && lpMsg->MagicSpeed > lpObj->MagicSpeed && abs((int)(lpObj->MagicSpeed-lpMsg->MagicSpeed)) > gServerInfo.m_CheckSpeedHackTolerance)
  1371: 	{
  1372: 		gLog.Output(LOG_HACK,"[%s][%s] Client Speed Hack Detected [%d][%d]",lpObj->Account,lpObj->Name,lpObj->MagicSpeed,lpMsg->MagicSpeed);
  1373: 		GCCloseClientSend(aIndex,0);
  1374: 		return;
  1375: 	}
  1376: 
  1377: 	lpObj->ConnectTickCount = GetTickCount();
  1378: }
  1379: 
  1380: void CGPositionRecv(PMSG_POSITION_RECV* lpMsg,int aIndex) // OK
  1381: {
  1382: 	LPOBJ lpObj = &gObj[aIndex];
  1383: 
  1384: 	if(gObjIsConnectedGS(aIndex) == 0)
  1385: 	{
  1386: 		return;
  1387: 	}
  1388: 
  1389: 	if(lpObj->Teleport != 0)
  1390: 	{
  1391: 		return;
  1392: 	}
  1393: 
  1394: 	if(gObjCheckMapTile(lpObj,2) != 0)
  1395: 	{
  1396: 		return;
  1397: 	}
  1398: 
  1399: 	if(CC_MAP_RANGE(lpObj->Map) != 0 && (GetTickCount()-lpObj->ChaosCastleBlowTime) < 1000)
  1400: 	{
  1401: 		return;
  1402: 	}
  1403: 
  1404: 	if(lpMsg->x < (lpObj->X-15) || lpMsg->x > (lpObj->X+15) || lpMsg->y < (lpObj->Y-15) || lpMsg->y > (lpObj->Y+15))
  1405: 	{
  1406: 		return;
  1407: 	}
  1408: 
  1409: 	gMap[lpObj->Map].DelStandAttr(lpObj->OldX,lpObj->OldY);
  1410: 
  1411: 	lpObj->X = lpMsg->x;
  1412: 	lpObj->Y = lpMsg->y;
  1413: 	lpObj->TX = lpMsg->x;
  1414: 	lpObj->TY = lpMsg->y;
  1415: 	lpObj->OldX = lpMsg->x;
  1416: 	lpObj->OldY = lpMsg->y;
  1417: 
  1418: 	gMap[lpObj->Map].SetStandAttr(lpObj->TX,lpObj->TY);
  1419: 
  1420: 	PMSG_POSITION_SEND pMsg = { 0 };
  1421: 
  1422: 	#if(NEW_PROTOCOL_SYSTEM==0)
  1423: 		pMsg.header.set(PROTOCOL_CODE3,sizeof(pMsg));
  1424: 	#endif
  1425: 
  1426: 	pMsg.index[0] = SET_NUMBERHB(aIndex);
  1427: 
  1428: 	pMsg.index[1] = SET_NUMBERLB(aIndex);
  1429: 
  1430: 	pMsg.x = (BYTE)lpObj->TX;
  1431: 
  1432: 	pMsg.y = (BYTE)lpObj->TY;
  1433: 
  1434: 	#if(NEW_PROTOCOL_SYSTEM==1)
  1435: 		uint16_t size = sizeof(pMsg);
  1436: 	#endif
  1437: 
  1438: 	if(lpObj->Type == OBJECT_USER)
  1439: 	{
  1440: 		#if(NEW_PROTOCOL_SYSTEM==1)
  1441: 			gSocketManagerModern.PacketSend(aIndex,ProtocolHead::BOTH_POSITION,(uint8_t*)&pMsg,size);
  1442: 		#else
  1443: 			DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
  1444: 		#endif
  1445: 	}
  1446: 
  1447: 	for(int n=0;n < MAX_VIEWPORT;n++)
  1448: 	{
  1449: 		if(lpObj->VpPlayer2[n].type == OBJECT_USER)
  1450: 		{
  1451: 			if(lpObj->VpPlayer2[n].state != OBJECT_EMPTY && lpObj->VpPlayer2[n].state != OBJECT_DIECMD && lpObj->VpPlayer2[n].state != OBJECT_DIED)
  1452: 			{
  1453: 				#if(NEW_PROTOCOL_SYSTEM==1)
  1454: 					gSocketManagerModern.PacketSend(lpObj->VpPlayer2[n].index,ProtocolHead::BOTH_POSITION,(uint8_t*)&pMsg,size);
  1455: 				#else
  1456: 					DataSend(lpObj->VpPlayer2[n].index,(BYTE*)&pMsg,pMsg.header.size);
  1457: 				#endif
  1458: 			}
  1459: 		}
  1460: 	}
  1461: }
  1462: 
  1463: void CGActionRecv(PMSG_ACTION_RECV* lpMsg,int aIndex) // OK
  1464: {
```

### HIT: Struct hints / `\bPMSG_\w+|\bPBMSG_\w+|\bATTACK\b`

- line 101: `			CGChatRecv((PMSG_CHAT_RECV*)lpMsg,aIndex);`
- line 104: `			CGChatWhisperRecv((PMSG_CHAT_WHISPER_RECV*)lpMsg,aIndex);`
- line 107: `			CGMainCheckRecv((PMSG_MAIN_CHECK_RECV*)lpMsg,aIndex);`
- line 110: `			CGLiveClientRecv((PMSG_LIVE_CLIENT_RECV*)lpMsg,aIndex);`
- line 113: `			gAttack.CGAttackRecv((PMSG_ATTACK_RECV*)lpMsg,aIndex);`
- line 116: `			CGPositionRecv((PMSG_POSITION_RECV*)lpMsg,aIndex);`
- line 119: `			CGActionRecv((PMSG_ACTION_RECV*)lpMsg,aIndex);`
- line 122: `			gSkillManager.CGSkillAttackRecv((PMSG_SKILL_ATTACK_RECV*)lpMsg,aIndex);`
- line 125: `			gSkillManager.CGSkillCancelRecv((PMSG_SKILL_CANCEL_RECV*)lpMsg,aIndex);`
- line 128: `			gMove.CGTeleportRecv((PMSG_TELEPORT_RECV*)lpMsg,aIndex);`
- line 131: `			gSkillManager.CGDurationSkillAttackRecv((PMSG_DURATION_SKILL_ATTACK_RECV*)lpMsg,aIndex);`
- line 134: `			gItemManager.CGItemGetRecv((PMSG_ITEM_GET_RECV*)lpMsg,aIndex);`
- line 137: `			gItemManager.CGItemDropRecv((PMSG_ITEM_DROP_RECV*)lpMsg,aIndex);`
- line 140: `			gItemManager.CGItemMoveRecv((PMSG_ITEM_MOVE_RECV*)lpMsg,aIndex);`

#### CONTEXT (first hit)

```
    31: #include "EventInventory.h"
    32: #include "Filter.h"
    33: #include "Fruit.h"
    34: #include "GensSystem.h"
    35: #include "Guild.h"
    36: #include "GuildClass.h"
    37: #include "GuildMatching.h"
    38: #include "HackPacketCheck.h"
    39: #include "Helper.h"
    40: #include "IllusionTemple.h"
    41: #include "ImperialGuardian.h"
    42: #include "InventoryEquipment.h"
    43: #include "ItemManager.h"
    44: #include "JewelMix.h"
    45: #include "JSProtocol.h"
    46: #include "KanturuEntranceNPC.h"
    47: #include "Log.h"
    48: #include "LuckyCoin.h"
    49: #include "Map.h"
    50: #include "MapServerManager.h"
    51: #include "MasterSkillTree.h"
    52: #include "Message.h"
    53: #include "MiniMap.h"
    54: #include "MiningSystem.h"
    55: #include "Move.h"
    56: #include "MuRummy.h"
    57: #include "MuunSystem.h"
    58: #include "Notice.h"
    59: #include "NpcTalk.h"
    60: #include "ObjectManager.h"
    61: #include "Party.h"
    62: #include "PartyMatching.h"
    63: #include "PcPoint.h"
    64: #include "PentagramSystem.h"
    65: #include "PersonalShop.h"
    66: #include "SocketManagerModern.h"
    67: #include "Quest.h"
    68: #include "QuestWorld.h"
    69: #include "ServerInfo.h"
    70: #include "SkillManager.h"
    71: #include "Trade.h"
    72: #include "Util.h"
    73: #include "Viewport.h"
    74: #include "Warehouse.h"
    75: 
    76: void ProtocolCore(BYTE head,BYTE* lpMsg,int size,int aIndex,int encrypt,int serial) // OK
    77: {
    78: 	//if(gObj[aIndex].Type == OBJECT_USER && gHackPacketCheck.CheckPacketHack(aIndex,head,((lpMsg[0]==0xC1)?lpMsg[3]:lpMsg[4]),encrypt,serial) == 0)
    79: 	//{
    80: 	//	return;
    81: 	//}
    82: 
    83: 		if((head != 0xF3 || lpMsg[3] != 0xF1) && head != 0x0E && head != 0x18 && head != PROTOCOL_CODE1)
    84: 		{
    85: 			char String[1024];
    86: 			char hex_chars[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    87: 
    88: 			for( int i = 0; i < size; ++i )
    89: 			{
    90: 				sprintf(&String[2*i], "%02X", lpMsg[i]);
    91: 			}
    92: 			LogAdd(LOG_RED,"PacketIN: INDEX: %d TYPE: %#X, HEAD: %#04X, SIZE: %02d, PACKET: %s",aIndex,lpMsg[0],head,size,String);
    93: 		}
    94: 
    95: 	//LogAdd(LOG_BLACK,"PROTO head: %x, 1: %x, 2: %x, 3: %x, 4: %x, 5: %x",head,lpMsg[1],lpMsg[2],lpMsg[3],lpMsg[4],lpMsg[5]);
    96: 
    97: 
    98: 	switch(head)
    99: 	{
   100: 		case 0x00:
   101: 			CGChatRecv((PMSG_CHAT_RECV*)lpMsg,aIndex);
   102: 			break;
   103: 		case 0x02:
   104: 			CGChatWhisperRecv((PMSG_CHAT_WHISPER_RECV*)lpMsg,aIndex);
   105: 			break;
   106: 		case 0x03:
   107: 			CGMainCheckRecv((PMSG_MAIN_CHECK_RECV*)lpMsg,aIndex);
   108: 			break;
   109: 		case 0x0E:
   110: 			CGLiveClientRecv((PMSG_LIVE_CLIENT_RECV*)lpMsg,aIndex);
   111: 			break;
   112: 		case PROTOCOL_CODE2:
   113: 			gAttack.CGAttackRecv((PMSG_ATTACK_RECV*)lpMsg,aIndex);
   114: 			break;
   115: 		case PROTOCOL_CODE3:
   116: 			CGPositionRecv((PMSG_POSITION_RECV*)lpMsg,aIndex);
   117: 			break;
   118: 		case 0x18:
   119: 			CGActionRecv((PMSG_ACTION_RECV*)lpMsg,aIndex);
   120: 			break;
   121: 		case 0x19:
   122: 			gSkillManager.CGSkillAttackRecv((PMSG_SKILL_ATTACK_RECV*)lpMsg,aIndex);
   123: 			break;
   124: 		case 0x1B:
   125: 			gSkillManager.CGSkillCancelRecv((PMSG_SKILL_CANCEL_RECV*)lpMsg,aIndex);
   126: 			break;
   127: 		case 0x1C:
   128: 			gMove.CGTeleportRecv((PMSG_TELEPORT_RECV*)lpMsg,aIndex);
   129: 			break;
   130: 		case 0x1E:
   131: 			gSkillManager.CGDurationSkillAttackRecv((PMSG_DURATION_SKILL_ATTACK_RECV*)lpMsg,aIndex);
   132: 			break;
   133: 		case 0x22:
   134: 			gItemManager.CGItemGetRecv((PMSG_ITEM_GET_RECV*)lpMsg,aIndex);
   135: 			break;
   136: 		case 0x23:
   137: 			gItemManager.CGItemDropRecv((PMSG_ITEM_DROP_RECV*)lpMsg,aIndex);
   138: 			break;
   139: 		case 0x24:
   140: 			gItemManager.CGItemMoveRecv((PMSG_ITEM_MOVE_RECV*)lpMsg,aIndex);
   141: 			break;
   142: 		case 0x26:
   143: 			gItemManager.CGItemUseRecv((PMSG_ITEM_USE_RECV*)lpMsg,aIndex);
   144: 			break;
   145: 		case 0x30:
   146: 			gNpcTalk.CGNpcTalkRecv((PMSG_NPC_TALK_RECV*)lpMsg,aIndex);
   147: 			break;
   148: 		case 0x31:
   149: 			gNpcTalk.CGNpcTalkCloseRecv(aIndex);
   150: 			break;
   151: 		case 0x32:
   152: 			gItemManager.CGItemBuyRecv((PMSG_ITEM_BUY_RECV*)lpMsg,aIndex,0);
   153: 			break;
   154: 		case 0x33:
   155: 			gItemManager.CGItemSellRecv((PMSG_ITEM_SELL_RECV*)lpMsg,aIndex);
   156: 			break;
   157: 		case 0x34:
   158: 			gItemManager.CGItemRepairRecv((PMSG_ITEM_REPAIR_RECV*)lpMsg,aIndex);
   159: 			break;
   160: 		case 0x36:
   161: 			gTrade.CGTradeRequestRecv((PMSG_TRADE_REQUEST_RECV*)lpMsg,aIndex);
   162: 			break;
   163: 		case 0x37:
   164: 			gTrade.CGTradeResponseRecv((PMSG_TRADE_RESPONSE_RECV*)lpMsg,aIndex);
   165: 			break;
   166: 		case 0x3A:
   167: 			gTrade.CGTradeMoneyRecv((PMSG_TRADE_MONEY_RECV*)lpMsg,aIndex);
   168: 			break;
   169: 		case 0x3C:
   170: 			gTrade.CGTradeOkButtonRecv((PMSG_TRADE_OK_BUTTON_RECV*)lpMsg,aIndex);
   171: 			break;
   172: 		case 0x3D:
   173: 			gTrade.CGTradeCancelButtonRecv(aIndex);
   174: 			break;
   175: 		case 0x3F:
   176: 			switch(lpMsg[3])
   177: 			{
   178: 				case 0x01:
   179: 					gPersonalShop.CGPShopSetItemPriceRecv((PMSG_PSHOP_SET_ITEM_PRICE_RECV*)lpMsg,aIndex);
   180: 					break;
   181: 				case 0x02:
   182: 					gPersonalShop.CGPShopOpenRecv((PMSG_PSHOP_OPEN_RECV*)lpMsg,aIndex);
   183: 					break;
   184: 				case 0x03:
   185: 					gPersonalShop.CGPShopCloseRecv(aIndex);
   186: 					break;
   187: 				case 0x05:
   188: 					gPersonalShop.CGPShopItemListRecv((PMSG_PSHOP_ITEM_LIST_RECV*)lpMsg,aIndex);
   189: 					break;
   190: 				case 0x06:
   191: 					gPersonalShop.CGPShopBuyItemRecv((PMSG_PSHOP_BUY_ITEM_RECV*)lpMsg,aIndex);
   192: 					break;
   193: 				case 0x07:
   194: 					gPersonalShop.CGPShopLeaveRecv((PMSG_PSHOP_LEAVE_RECV*)lpMsg,aIndex);
   195: 					break;
   196: 			}
   197: 			break;
   198: 		case 0x40:
   199: 			gParty.CGPartyRequestRecv((PMSG_PARTY_REQUEST_RECV*)lpMsg,aIndex);
   200: 			break;
   201: 		case 0x41:
   202: 			gParty.CGPartyRequestResultRecv((PMSG_PARTY_REQUEST_RESULT_RECV*)lpMsg,aIndex);
   203: 			break;
   204: 		case 0x42:
   205: 			gParty.CGPartyListRecv(aIndex);
   206: 			break;
   207: 		case 0x43:
   208: 			gParty.CGPartyDelMemberRecv((PMSG_PARTY_DEL_MEMBER_RECV*)lpMsg,aIndex);
   209: 			break;
   210: 		case 0x4A:
   211: 			#if(GAMESERVER_UPDATE>=601)
   212: 			gSkillManager.CGRageFighterSkillAttackRecv((PMSG_RAGE_FIGHTER_SKILL_ATTACK_RECV*)lpMsg,aIndex);
   213: 			#endif
   214: 			break;
   215: 		case 0x4B:
   216: 			#if(GAMESERVER_UPDATE>=601)
   217: 			gSkillManager.CGSkillDarkSideRecv((PMSG_SKILL_DARK_SIDE_RECV*)lpMsg,aIndex);
   218: 			#endif
   219: 			break;
   220: 		case 0x4C:
   221: 			switch(lpMsg[3])
   222: 			{
   223: 				case 0x00:
   224: 					#if(GAMESERVER_UPDATE>=801)
   225: 					gMiningSystem.CGMiningStartRecv((PMSG_MINING_START_RECV*)lpMsg,aIndex);
   226: 					#endif
   227: 					break;
   228: 				case 0x01:
   229: 					#if(GAMESERVER_UPDATE>=801)
   230: 					gMiningSystem.CGMiningSuccessRecv((PMSG_MINING_SUCCESS_RECV*)lpMsg,aIndex);
   231: 					#endif
   232: 					break;
   233: 				case 0x03:
   234: 					#if(GAMESERVER_UPDATE>=801)
   235: 					gMiningSystem.CGMiningFailureRecv((PMSG_MINING_FAILURE_RECV*)lpMsg,aIndex);
   236: 					#endif
   237: 					break;
   238: 			}
   239: 			break;
   240: 		case 0x4D:
   241: 			switch(lpMsg[3])
   242: 			{
   243: 				case 0x00:
   244: 					#if(GAMESERVER_UPDATE>=802)
   245: 					gEventInventory.CGEventItemGetRecv((PMSG_EVENT_ITEM_GET_RECV*)lpMsg,aIndex);
   246: 					#endif
   247: 					break;
   248: 				case 0x01:
   249: 					#if(GAMESERVER_UPDATE>=802)
   250: 					gEventInventory.CGEventItemDropRecv((PMSG_EVENT_ITEM_DROP_RECV*)lpMsg,aIndex);
   251: 					#endif
   252: 					break;
   253: 				case 0x0F:
   254: 					#if(GAMESERVER_UPDATE>=802)
   255: 					gEventInventory.CGEventInventoryOpenRecv((PMSG_EVENT_INVENTORY_OPEN_RECV*)lpMsg,aIndex);
   256: 					#endif
   257: 					break;
   258: 				case 0x10:
   259: 					#if(GAMESERVER_UPDATE>=802)
   260: 					gMuRummy.CGReqMuRummyStart((_tagPMSG_REQ_MURUMMY_INFO*)lpMsg,aIndex);
   261: 					#endif
   262: 					break;
   263: 				case 0x11:
   264: 					#if(GAMESERVER_UPDATE>=802)
   265: 					gMuRummy.CGReqCardReveal((_tagPMSG_REQ_REVEAL_CARD*)lpMsg,aIndex);
   266: 					#endif
   267: 					break;
   268: 				case 0x12:
   269: 					#if(GAMESERVER_UPDATE>=802)
   270: 					gMuRummy.CGReqCardMove((_tagPMSG_REQ_CARD_MOVE*)lpMsg,aIndex);
   271: 					#endif
   272: 					break;
   273: 				case 0x13:
   274: 					#if(GAMESERVER_UPDATE>=802)
   275: 					gMuRummy.CGReqCardReMove((_tagPMSG_REQ_CARD_REMOVE*)lpMsg,aIndex);
   276: 					#endif
   277: 					break;
   278: 				case 0x14:
   279: 					#if(GAMESERVER_UPDATE>=802)
   280: 					gMuRummy.CGReqCardMatch((_tagPMSG_REQ_CARD_MATCH*)lpMsg,aIndex);
   281: 					#endif
   282: 					break;
   283: 				case 0x15:
   284: 					#if(GAMESERVER_UPDATE>=802)
   285: 					gMuRummy.CGReqMuRummyEnd((_tagPMSG_REQ_MURUMMY_END*)lpMsg,aIndex);
   286: 					#endif
   287: 					break;
   288: 			}
   289: 			break;
   290: 		case 0x4E:
   291: 			switch(lpMsg[3])
   292: 			{
   293: 				case 0x00:
   294: 					#if(GAMESERVER_UPDATE>=803)
   295: 					gMuunSystem.CGMuunItemGetRecv((PMSG_MUUN_ITEM_GET_RECV*)lpMsg,aIndex);
   296: 					#endif
   297: 					break;
   298: 				case 0x01:
   299: 					break;
   300: 				case 0x08:
   301: 					#if(GAMESERVER_UPDATE>=803)
   302: 					gMuunSystem.CGMuunItemUseRecv((PMSG_MUUN_ITEM_USE_RECV*)lpMsg,aIndex);
   303: 					#endif
   304: 				case 0x09:
   305: 					#if(GAMESERVER_UPDATE>=803)
   306: 					gMuunSystem.CGMuunItemSellRecv((PMSG_MUUN_ITEM_SELL_RECV*)lpMsg,aIndex);
   307: 					#endif
   308: 					break;
   309: 			}
   310: 			break;
   311: 		case 0x50:
   312: 			gGuild.CGGuildRequestRecv((PMSG_GUILD_REQUEST_RECV*)lpMsg,aIndex);
   313: 			break;
   314: 		case 0x51:
   315: 			gGuild.CGGuildResultRecv((PMSG_GUILD_RESULT_RECV*)lpMsg,aIndex);
   316: 			break;
   317: 		case 0x52:
   318: 			gGuild.CGGuildListRecv(aIndex);
   319: 			break;
   320: 		case 0x53:
   321: 			gGuild.CGGuildDeleteRecv((PMSG_GUILD_DELETE_RECV*)lpMsg,aIndex);
   322: 			break;
   323: 		case 0x54:
   324: 			CGGuildMasterAnswerRecv((PMSG_GUILDMASTERANSWER*)lpMsg,aIndex);
   325: 			break;
   326: 		case 0x55:
   327: 			CGGuildMasterInfoSave(aIndex,(PMSG_GUILDINFOSAVE*)lpMsg);
   328: 			break;
   329: 		case 0x57:
   330: 			CGGuildMasterCreateCancel(aIndex);
   331: 			break;
   332: 		case 0x61:
   333: 			GCGuildWarRequestSendRecv((PMSG_GUILDWARSEND_RESULT*)lpMsg,aIndex);
   334: 			break;
   335: 		case 0x66:
   336: 			GCGuildViewportInfo((PMSG_REQ_GUILDVIEWPORT*)lpMsg,aIndex);
   337: 			break;
   338: 		case 0x81:
   339: 			gWarehouse.CGWarehouseMoneyRecv((PMSG_WAREHOUSE_MONEY_RECV*)lpMsg,aIndex);
   340: 			break;
   341: 		case 0x82:
```


---

## FILE: Attack.h

### HIT: Calls to handlers / `CGAttackRecv|CGMultiSkillAttackRecv`

- line 59: `	void CGAttackRecv(PMSG_ATTACK_RECV* lpMsg,int aIndex);`

#### CONTEXT (first hit)

```
     1: // Attack.h: interface for the CAttack class.
     2: //
     3: //////////////////////////////////////////////////////////////////////
     4: 
     5: #pragma once
     6: 
     7: #include "Protocol.h"
     8: 
     9: //**********************************************//
    10: //************ Client -> GameServer ************//
    11: //**********************************************//
    12: 
    13: struct PMSG_ATTACK_RECV
    14: {
    15: 	PBMSG_HEAD header; // C1:[PROTOCOL_CODE2]
    16: 	BYTE index[2];
    17: 	BYTE action;
    18: 	BYTE dir;
    19: };
    20: 
    21: //**********************************************//
    22: //**********************************************//
    23: //**********************************************//
    24: 
    25: class CAttack
    26: {
    27: public:
    28: 	CAttack();
    29: 	virtual ~CAttack();
    30: 	bool Attack(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,bool send,BYTE flag,int damage,int count,bool combo);
    31: 	bool AttackElemental(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,bool send,BYTE flag,int damage,int count,bool combo);
    32: 	bool DecreaseArrow(LPOBJ lpObj);
    33: 	void WingSprite(LPOBJ lpObj,LPOBJ lpTarget,int* damage);
    34: 	void HelperSprite(LPOBJ lpObj,LPOBJ lpTarget,int* damage);
    35: 	void DamageSprite(LPOBJ lpObj,int damage);
    36: 	bool DarkHorseSprite(LPOBJ lpObj,int damage);
    37: 	bool FenrirSprite(LPOBJ lpObj,int damage);
    38: 	void WeaponDurabilityDown(LPOBJ lpObj,LPOBJ lpTarget);
    39: 	void ArmorDurabilityDown(LPOBJ lpObj,LPOBJ lpTarget);
    40: 	bool CheckPlayerTarget(LPOBJ lpObj,LPOBJ lpTarget);
    41: 	void MissSend(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,int send,int count);
    42: 	bool MissCheck(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,int send,int count,BYTE* miss);
    43: 	bool MissCheckPvP(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,int send,int count,BYTE* miss);
    44: 	bool MissCheckElemental(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,int send,int count,BYTE* miss);
    45: 	bool ApplySkillEffect(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,int damage);
    46: 	int GetTargetDefense(LPOBJ lpObj,LPOBJ lpTarget,WORD* effect);
    47: 	int GetTargetElementalDefense(LPOBJ lpObj,LPOBJ lpTarget,WORD* effect);
    48: 	int GetAttackDamage(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,WORD* effect,int TargetDefense);
    49: 	int GetAttackDamageWizard(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,WORD* effect,int TargetDefense);
    50: 	int GetAttackDamageCursed(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,WORD* effect,int TargetDefense);
    51: 	int GetAttackDamageFenrir(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,WORD* effect,int TargetDefense);
    52: 	int GetAttackDamageElemental(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,WORD* effect,int AttackDamage,int TargetDefense);
    53: 	int GetShieldDamage(LPOBJ lpObj,LPOBJ lpTarget,int damage);
    54: 	void GetPreviewDefense(LPOBJ lpObj,DWORD* defense);
    55: 	void GetPreviewPhysiDamage(LPOBJ lpObj,DWORD* DamageMin,DWORD* DamageMax,DWORD* MulDamage,DWORD* DivDamage);
    56: 	void GetPreviewMagicDamage(LPOBJ lpObj,DWORD* DamageMin,DWORD* DamageMax,DWORD* MulDamage,DWORD* DivDamage,DWORD* DamageRate);
    57: 	void GetPreviewCurseDamage(LPOBJ lpObj,DWORD* DamageMin,DWORD* DamageMax,DWORD* MulDamage,DWORD* DivDamage,DWORD* DamageRate);
    58: 	void GetPreviewDamageMultiplier(LPOBJ lpObj,DWORD* DamageMultiplier,DWORD* RFDamageMultiplierA,DWORD* RFDamageMultiplierB,DWORD* RFDamageMultiplierC);
    59: 	void CGAttackRecv(PMSG_ATTACK_RECV* lpMsg,int aIndex);
    60: };
    61: 
    62: extern CAttack gAttack;
```

### HIT: Struct hints / `\bPMSG_\w+|\bPBMSG_\w+|\bATTACK\b`

- line 13: `struct PMSG_ATTACK_RECV`
- line 15: `	PBMSG_HEAD header; // C1:[PROTOCOL_CODE2]`
- line 59: `	void CGAttackRecv(PMSG_ATTACK_RECV* lpMsg,int aIndex);`

#### CONTEXT (first hit)

```
     1: // Attack.h: interface for the CAttack class.
     2: //
     3: //////////////////////////////////////////////////////////////////////
     4: 
     5: #pragma once
     6: 
     7: #include "Protocol.h"
     8: 
     9: //**********************************************//
    10: //************ Client -> GameServer ************//
    11: //**********************************************//
    12: 
    13: struct PMSG_ATTACK_RECV
    14: {
    15: 	PBMSG_HEAD header; // C1:[PROTOCOL_CODE2]
    16: 	BYTE index[2];
    17: 	BYTE action;
    18: 	BYTE dir;
    19: };
    20: 
    21: //**********************************************//
    22: //**********************************************//
    23: //**********************************************//
    24: 
    25: class CAttack
    26: {
    27: public:
    28: 	CAttack();
    29: 	virtual ~CAttack();
    30: 	bool Attack(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,bool send,BYTE flag,int damage,int count,bool combo);
    31: 	bool AttackElemental(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,bool send,BYTE flag,int damage,int count,bool combo);
    32: 	bool DecreaseArrow(LPOBJ lpObj);
    33: 	void WingSprite(LPOBJ lpObj,LPOBJ lpTarget,int* damage);
    34: 	void HelperSprite(LPOBJ lpObj,LPOBJ lpTarget,int* damage);
    35: 	void DamageSprite(LPOBJ lpObj,int damage);
    36: 	bool DarkHorseSprite(LPOBJ lpObj,int damage);
    37: 	bool FenrirSprite(LPOBJ lpObj,int damage);
    38: 	void WeaponDurabilityDown(LPOBJ lpObj,LPOBJ lpTarget);
    39: 	void ArmorDurabilityDown(LPOBJ lpObj,LPOBJ lpTarget);
    40: 	bool CheckPlayerTarget(LPOBJ lpObj,LPOBJ lpTarget);
    41: 	void MissSend(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,int send,int count);
    42: 	bool MissCheck(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,int send,int count,BYTE* miss);
    43: 	bool MissCheckPvP(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,int send,int count,BYTE* miss);
    44: 	bool MissCheckElemental(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,int send,int count,BYTE* miss);
    45: 	bool ApplySkillEffect(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,int damage);
    46: 	int GetTargetDefense(LPOBJ lpObj,LPOBJ lpTarget,WORD* effect);
    47: 	int GetTargetElementalDefense(LPOBJ lpObj,LPOBJ lpTarget,WORD* effect);
    48: 	int GetAttackDamage(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,WORD* effect,int TargetDefense);
    49: 	int GetAttackDamageWizard(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,WORD* effect,int TargetDefense);
    50: 	int GetAttackDamageCursed(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,WORD* effect,int TargetDefense);
    51: 	int GetAttackDamageFenrir(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,WORD* effect,int TargetDefense);
    52: 	int GetAttackDamageElemental(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,WORD* effect,int AttackDamage,int TargetDefense);
    53: 	int GetShieldDamage(LPOBJ lpObj,LPOBJ lpTarget,int damage);
    54: 	void GetPreviewDefense(LPOBJ lpObj,DWORD* defense);
    55: 	void GetPreviewPhysiDamage(LPOBJ lpObj,DWORD* DamageMin,DWORD* DamageMax,DWORD* MulDamage,DWORD* DivDamage);
    56: 	void GetPreviewMagicDamage(LPOBJ lpObj,DWORD* DamageMin,DWORD* DamageMax,DWORD* MulDamage,DWORD* DivDamage,DWORD* DamageRate);
    57: 	void GetPreviewCurseDamage(LPOBJ lpObj,DWORD* DamageMin,DWORD* DamageMax,DWORD* MulDamage,DWORD* DivDamage,DWORD* DamageRate);
    58: 	void GetPreviewDamageMultiplier(LPOBJ lpObj,DWORD* DamageMultiplier,DWORD* RFDamageMultiplierA,DWORD* RFDamageMultiplierB,DWORD* RFDamageMultiplierC);
    59: 	void CGAttackRecv(PMSG_ATTACK_RECV* lpMsg,int aIndex);
    60: };
    61: 
    62: extern CAttack gAttack;
```


---

## FILE: SkillManager.h

### HIT: Calls to handlers / `CGAttackRecv|CGMultiSkillAttackRecv`

- line 556: `	void CGMultiSkillAttackRecv(PMSG_MULTI_SKILL_ATTACK_RECV* lpMsg,int aIndex,bool type);`

#### CONTEXT (first hit)

```
   486: 	bool SkillOrderOfProtection(int aIndex,int bIndex);
   487: 	bool SkillOrderOfRestraint(int aIndex,int bIndex);
   488: 	bool SkillOrderOfTracking(int aIndex,int bIndex);
   489: 	bool SkillOrderOfWeaken(int aIndex,int bIndex);
   490: 	bool SkillChainLightning(int aIndex,int bIndex,CSkill* lpSkill,bool combo);
   491: 	bool SkillDamageReflect(int aIndex,int bIndex,CSkill* lpSkill);
   492: 	bool SkillSwordPower(int aIndex,int bIndex,CSkill* lpSkill,bool combo);
   493: 	bool SkillSwordPowerGetDefense(int aIndex,int* defense);
   494: 	bool SkillSwordPowerGetPhysiDamage(int aIndex,int* DamageMin,int* DamageMax);
   495: 	bool SkillSwordPowerGetMagicDamage(int aIndex,int* DamageMin,int* DamageMax);
   496: 	bool SkillSwordPowerGetCurseDamage(int aIndex,int* DamageMin,int* DamageMax);
   497: 	bool SkillSleep(int aIndex,int bIndex,CSkill* lpSkill);
   498: 	bool SkillLesserDefense(int aIndex,int bIndex,CSkill* lpSkill);
   499: 	bool SkillLesserDamage(int aIndex,int bIndex,CSkill* lpSkill);
   500: 	bool SkillSahamutt(int aIndex,int bIndex,CSkill* lpSkill,BYTE x,BYTE y,bool combo);
   501: 	bool SkillNeil(int aIndex,int bIndex,CSkill* lpSkill,BYTE x,BYTE y);
   502: 	bool SkillGhostPhantom(int aIndex,int bIndex,CSkill* lpSkill,BYTE x,BYTE y);
   503: 	bool SkillGhostPhantomAreaAttack(int aIndex,int bIndex,CSkill* lpSkill,BYTE x,BYTE y);
   504: 	bool SkillRedStorm(int aIndex,int bIndex,CSkill* lpSkill,bool combo);
   505: 	bool SkillFrozenStab(int aIndex,int bIndex,CSkill* lpSkill,BYTE x,BYTE y,bool combo);
   506: 	bool SkillMagicCircle(int aIndex,int bIndex,CSkill* lpSkill);
   507: 	bool SkillShieldRecover(int aIndex,int bIndex,CSkill* lpSkill);
   508: 	bool SkillFiveShot(int aIndex,int bIndex,CSkill* lpSkill,BYTE angle,bool combo);
   509: 	bool SkillSwordSlash(int aIndex,int bIndex,CSkill* lpSkill,BYTE angle,bool combo);
   510: 	bool SkillLightningStorm(int aIndex,int bIndex,CSkill* lpSkill,bool combo);
   511: 	bool SkillBirds(int aIndex,int bIndex,CSkill* lpSkill,BYTE angle,bool combo);
   512: 	bool SkillSelfExplosion(int aIndex,int bIndex,CSkill* lpSkill);
   513: 	bool SkillLargeRingBlower(int aIndex,int bIndex,CSkill* lpSkill,bool combo);
   514: 	bool SkillUpperBeast(int aIndex,int bIndex,CSkill* lpSkill,bool combo);
   515: 	bool SkillChainDriver(int aIndex,int bIndex,CSkill* lpSkill,bool combo);
   516: 	bool SkillDarkSide(int aIndex,int bIndex,CSkill* lpSkill,bool combo);
   517: 	bool SkillDarkSideGetTargetIndex(int aIndex,int bIndex,CSkill* lpSkill,WORD* target);
   518: 	bool SkillDragonLore(int aIndex,int bIndex,CSkill* lpSkill,bool combo);
   519: 	bool SkillGreaterIgnoreDefenseRate(int aIndex,int bIndex,CSkill* lpSkill);
   520: 	bool SkillFitness(int aIndex,int bIndex,CSkill* lpSkill);
   521: 	bool SkillGreaterDefenseSuccessRate(int aIndex,int bIndex,CSkill* lpSkill);
   522: 	bool SkillPhoenixShot(int aIndex,int bIndex,CSkill* lpSkill,bool combo);
   523: 	bool SkillBloodStorm(int aIndex,int bIndex,CSkill* lpSkill,BYTE x,BYTE y,bool combo);
   524: 	bool SkillCure(int aIndex,int bIndex,CSkill* lpSkill);
   525: 	bool SkillPartyHeal(int aIndex,int bIndex,CSkill* lpSkill);
   526: 	bool SkillPoisonArrow(int aIndex,int bIndex,CSkill* lpSkill,bool combo);
   527: 	bool SkillBless(int aIndex,int bIndex,CSkill* lpSkill);
   528: 	bool SkillBlind(int aIndex,int bIndex,CSkill* lpSkill);
   529: 	bool SkillEarthPrison(int aIndex,int bIndex,CSkill* lpSkill,bool combo);
   530: 	bool SkillIronDefense(int aIndex,int bIndex,CSkill* lpSkill);
   531: 	bool SkillBloodHowling(int aIndex,int bIndex,CSkill* lpSkill);
   532: 	void ApplyMeteoriteEffect(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,int damage);
   533: 	void ApplyIceStormEffect(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,int damage);
   534: 	void ApplyTwistingSlashEffect(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,int damage);
   535: 	void ApplyRagefulBlowEffect(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,int damage);
   536: 	void ApplyDeathStabEffect(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,int damage);
   537: 	void ApplyFireSlashEffect(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,int damage);
   538: 	void ApplyFireBurstEffect(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,int damage);
   539: 	void ApplyPlasmaStormEffect(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,int damage);
   540: 	void ApplyFireScreamEffect(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,int damage);
   541: 	void ApplyEarthquakeEffect(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,int damage);
   542: 	void ApplyDrainLifeEffect(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,int damage);
   543: 	void ApplySahamuttEffect(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,int damage);
   544: 	void ApplyNeilEffect(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,int damage);
   545: 	void ApplyGhostPhantomEffect(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,int damage);
   546: 	void ApplyFrozenStabEffect(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,int damage);
   547: 	void ApplyFiveShotEffect(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,int damage);
   548: 	void ApplySwordSlashEffect(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,int damage);
   549: 	void ApplyLargeRingBlowerEffect(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,int damage);
   550: 	void ApplyUpperBeastEffect(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,int damage);
   551: 	void ApplyChainDriverEffect(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,int damage);
   552: 	void ApplyDragonLoreEffect(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,int damage);
   553: 	void ApplyDragonSlayerEffect(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,int damage);
   554: 	void ApplyPhoenixShotEffect(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,int damage);
   555: 	void ApplyEarthPrisonEffect(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,int damage);
   556: 	void CGMultiSkillAttackRecv(PMSG_MULTI_SKILL_ATTACK_RECV* lpMsg,int aIndex,bool type);
   557: 	void CGSkillAttackRecv(PMSG_SKILL_ATTACK_RECV* lpMsg,int aIndex);
   558: 	void CGDurationSkillAttackRecv(PMSG_DURATION_SKILL_ATTACK_RECV* lpMsg,int aIndex);
   559: 	void CGSkillCancelRecv(PMSG_SKILL_CANCEL_RECV* lpMsg,int aIndex);
   560: 	void CGRageFighterSkillAttackRecv(PMSG_RAGE_FIGHTER_SKILL_ATTACK_RECV* lpMsg,int aIndex);
   561: 	void CGSkillDarkSideRecv(PMSG_SKILL_DARK_SIDE_RECV* lpMsg,int aIndex);
   562: 	void CGSkillTeleportAllyRecv(PMSG_SKILL_TELEPORT_ALLY_RECV* lpMsg,int aIndex);
   563: 	void GCSkillAttackSend(LPOBJ lpObj,int skill,int aIndex,BYTE type);
   564: 	void GCSkillCancelSend(LPOBJ lpObj,int skill);
   565: 	void GCDurationSkillAttackSend(LPOBJ lpObj,int skill,BYTE x,BYTE y,BYTE dir);
   566: 	void GCRageFighterSkillAttackSend(LPOBJ lpObj,int skill,int aIndex,BYTE type);
   567: 	void GCSkillAddSend(int aIndex,BYTE slot,int skill,BYTE level,BYTE type);
   568: 	void GCSkillDelSend(int aIndex,BYTE slot,int skill,BYTE level,BYTE type);
   569: 	void GCSkillListSend(LPOBJ lpObj,BYTE type);
   570: private:
   571: 	std::map<int,SKILL_INFO> m_SkillInfo;
   572: };
   573: 
   574: extern CSkillManager gSkillManager;
```

### HIT: Struct hints / `\bPMSG_\w+|\bPBMSG_\w+|\bATTACK\b`

- line 166: `struct PMSG_MULTI_SKILL_ATTACK_RECV`
- line 168: `	PBMSG_HEAD header; // C3:[PROTOCOL_CODE4]`
- line 185: `struct PMSG_MULTI_SKILL_ATTACK`
- line 197: `struct PMSG_SKILL_ATTACK_RECV`
- line 199: `	PBMSG_HEAD header; // C3:19`
- line 213: `struct PMSG_SKILL_CANCEL_RECV`
- line 215: `	PBMSG_HEAD header; // C3:1B`
- line 220: `struct PMSG_DURATION_SKILL_ATTACK_RECV`
- line 222: `	PBMSG_HEAD header; // C3:1E`
- line 246: `struct PMSG_RAGE_FIGHTER_SKILL_ATTACK_RECV`
- line 248: `	PBMSG_HEAD header; // C1:4A`
- line 262: `struct PMSG_SKILL_DARK_SIDE_RECV`
- line 264: `	PBMSG_HEAD header; // C1:4B`
- line 269: `struct PMSG_SKILL_TELEPORT_ALLY_RECV`

#### CONTEXT (first hit)

```
    96: 	SKILL_REMOVE_ALL_EFFECT = 72,
    97: 	SKILL_MANA_RAYS = 73,
    98: 	SKILL_FIRE_BLAST = 74,
    99: 	SKILL_BRAND = 75,
   100: 	SKILL_PLASMA_STORM = 76,
   101: 	SKILL_INFINITY_ARROW = 77,
   102: 	SKILL_FIRE_SCREAM = 78,
   103: 	SKILL_EXPLOSION = 79,
   104: 	SKILL_MONSTER_SUMMON = 200,
   105: 	SKILL_MAGIC_DAMAGE_IMMUNITY = 201,
   106: 	SKILL_PHYSI_DAMAGE_IMMUNITY = 202,
   107: 	SKILL_ORDER_OF_PROTECTION = 210,
   108: 	SKILL_ORDER_OF_RESTRAINT = 211,
   109: 	SKILL_ORDER_OF_TRACKING = 212,
   110: 	SKILL_ORDER_OF_WEAKEN = 213,
   111: 	SKILL_DRAIN_LIFE = 214,
   112: 	SKILL_CHAIN_LIGHTNING = 215,
   113: 	SKILL_ELECTRIC_SURGE = 216,
   114: 	SKILL_DAMAGE_REFLECT = 217,
   115: 	SKILL_SWORD_POWER = 218,
   116: 	SKILL_SLEEP = 219,
   117: 	SKILL_LESSER_DAMAGE = 221,
   118: 	SKILL_LESSER_DEFENSE = 222,
   119: 	SKILL_SAHAMUTT = 223,
   120: 	SKILL_NEIL = 224,
   121: 	SKILL_GHOST_PHANTOM = 225,
   122: 	SKILL_RED_STORM = 230,
   123: 	SKILL_FROZEN_STAB = 232,
   124: 	SKILL_MAGIC_CIRCLE = 233,
   125: 	SKILL_SHIELD_RECOVER = 234,
   126: 	SKILL_FIVE_SHOT = 235,
   127: 	SKILL_SWORD_SLASH = 236,
   128: 	SKILL_LIGHTNING_STORM = 237,
   129: 	SKILL_BIRDS = 238,
   130: 	SKILL_SELF_EXPLOSION = 239,
   131: 	SKILL_RAKLION_SELUPAN1 = 250,
   132: 	SKILL_RAKLION_SELUPAN2 = 251,
   133: 	SKILL_RAKLION_SELUPAN3 = 252,
   134: 	SKILL_RAKLION_SELUPAN4 = 253,
   135: 	SKILL_LARGE_RING_BLOWER = 260,
   136: 	SKILL_UPPER_BEAST = 261,
   137: 	SKILL_CHAIN_DRIVER = 262,
   138: 	SKILL_DARK_SIDE = 263,
   139: 	SKILL_DRAGON_LORE = 264,
   140: 	SKILL_DRAGON_SLAYER = 265,
   141: 	SKILL_GREATER_IGNORE_DEFENSE_RATE = 266,
   142: 	SKILL_FITNESS = 267,
   143: 	SKILL_GREATER_DEFENSE_SUCCESS_RATE = 268,
   144: 	SKILL_CHARGE = 269,
   145: 	SKILL_PHOENIX_SHOT = 270,
   146: 	SKILL_BLOOD_STORM = 344,
   147: 	SKILL_CURE = 425,
   148: 	SKILL_PARTY_HEAL = 426,
   149: 	SKILL_POISON_ARROW = 427,
   150: 	SKILL_BLESS = 430,
   151: 	SKILL_SUMMON_SATYROS = 432,
   152: 	SKILL_BLIND = 461,
   153: 	SKILL_EARTH_PRISON = 495,
   154: 	SKILL_IRON_DEFENSE = 521,
   155: 	SKILL_BLOOD_HOWLING = 565,
   156: 	SKILL_SPROTECTION = 618,
   157: 	SKILL_SRESTRICTION = 619,
   158: 	SKILL_SPURSUIT = 620,
   159: 	SKILL_SSHIELD_BURN = 621,
   160: };
   161: 
   162: //**********************************************//
   163: //************ Client -> GameServer ************//
   164: //**********************************************//
   165: 
   166: struct PMSG_MULTI_SKILL_ATTACK_RECV
   167: {
   168: 	PBMSG_HEAD header; // C3:[PROTOCOL_CODE4]
   169: 	#if(GAMESERVER_UPDATE>=701)
   170: 	BYTE skillH;
   171: 	BYTE count;
   172: 	BYTE skillL;
   173: 	BYTE x;
   174: 	BYTE serial;
   175: 	BYTE y;
   176: 	#else
   177: 	BYTE skill[2];
   178: 	BYTE x;
   179: 	BYTE y;
   180: 	BYTE serial;
   181: 	BYTE count;
   182: 	#endif
   183: };
   184: 
   185: struct PMSG_MULTI_SKILL_ATTACK
   186: {
   187: 	#if(GAMESERVER_UPDATE>=701)
   188: 	BYTE indexH;
   189: 	BYTE MagicKey;
   190: 	BYTE indexL;
   191: 	#else
   192: 	BYTE index[2];
   193: 	BYTE MagicKey;
   194: 	#endif
   195: };
   196: 
   197: struct PMSG_SKILL_ATTACK_RECV
   198: {
   199: 	PBMSG_HEAD header; // C3:19
   200: 	#if(GAMESERVER_UPDATE>=701)
   201: 	BYTE indexH;
   202: 	BYTE skillH;
   203: 	BYTE indexL;
   204: 	BYTE skillL;
   205: 	BYTE dis;
   206: 	#else
   207: 	BYTE skill[2];
   208: 	BYTE index[2];
   209: 	BYTE dis;
   210: 	#endif
   211: };
   212: 
   213: struct PMSG_SKILL_CANCEL_RECV
   214: {
   215: 	PBMSG_HEAD header; // C3:1B
   216: 	BYTE skill[2];
   217: 	BYTE index[2];
   218: };
   219: 
   220: struct PMSG_DURATION_SKILL_ATTACK_RECV
   221: {
   222: 	PBMSG_HEAD header; // C3:1E
   223: 	#if(GAMESERVER_UPDATE>=701)
   224: 	BYTE x;
   225: 	BYTE skillH;
   226: 	BYTE y;
   227: 	BYTE skillL;
   228: 	BYTE dir;
   229: 	BYTE indexH;
   230: 	BYTE dis;
   231: 	BYTE indexL;
   232: 	BYTE angle;
   233: 	BYTE MagicKey;
   234: 	#else
   235: 	BYTE skill[2];
   236: 	BYTE x;
   237: 	BYTE y;
   238: 	BYTE dir;
   239: 	BYTE dis;
   240: 	BYTE angle;
   241: 	BYTE index[2];
   242: 	BYTE MagicKey;
   243: 	#endif
   244: };
   245: 
   246: struct PMSG_RAGE_FIGHTER_SKILL_ATTACK_RECV
   247: {
   248: 	PBMSG_HEAD header; // C1:4A
   249: 	#if(GAMESERVER_UPDATE>=701)
   250: 	BYTE indexH;
   251: 	BYTE skillH;
   252: 	BYTE indexL;
   253: 	BYTE skillL;
   254: 	BYTE dis;
   255: 	#else
   256: 	BYTE skill[2];
   257: 	BYTE dis;
   258: 	BYTE index[2];
   259: 	#endif
   260: };
   261: 
   262: struct PMSG_SKILL_DARK_SIDE_RECV
   263: {
   264: 	PBMSG_HEAD header; // C1:4B
   265: 	BYTE skill[2];
   266: 	BYTE index[2];
   267: };
   268: 
   269: struct PMSG_SKILL_TELEPORT_ALLY_RECV
   270: {
   271: 	#pragma pack(1)
   272: 	PBMSG_HEAD header; // C3:B0
   273: 	WORD index;
   274: 	BYTE x;
   275: 	BYTE y;
   276: 	#pragma pack()
   277: };
   278: 
   279: //**********************************************//
   280: //************ GameServer -> Client ************//
   281: //**********************************************//
   282: 
   283: struct PMSG_SKILL_ATTACK_SEND
   284: {
   285: 	PBMSG_HEAD header; // C3:19
   286: 	#if(GAMESERVER_UPDATE>=701)
   287: 	BYTE index[2];
   288: 	BYTE skill[2];
   289: 	BYTE target[2];
   290: 	#else
   291: 	BYTE skill[2];
   292: 	BYTE index[2];
   293: 	BYTE target[2];
   294: 	#endif
   295: };
   296: 
   297: struct PMSG_SKILL_CANCEL_SEND
   298: {
   299: 	PBMSG_HEAD header; // C1:1B
   300: 	BYTE skill[2];
   301: 	BYTE index[2];
   302: };
   303: 
   304: struct PMSG_DURATION_SKILL_ATTACK_SEND
   305: {
   306: 	PBMSG_HEAD header; // C3:1E
   307: 	#if(GAMESERVER_UPDATE>=701)
   308: 	BYTE x;
   309: 	BYTE y;
   310: 	BYTE dir;
   311: 	BYTE skillH;
   312: 	BYTE indexH;
   313: 	BYTE skillL;
   314: 	BYTE indexL;
   315: 	#else
   316: 	BYTE skill[2];
   317: 	BYTE index[2];
   318: 	BYTE x;
   319: 	BYTE y;
   320: 	BYTE dir;
   321: 	#endif
   322: };
   323: 
   324: struct PMSG_RAGE_FIGHTER_SKILL_ATTACK_SEND
   325: {
   326: 	PBMSG_HEAD header; // C3:4A
   327: 	#if(GAMESERVER_UPDATE>=701)
   328: 	BYTE index[2];
   329: 	BYTE skill[2];
   330: 	BYTE target[2];
   331: 	#else
   332: 	BYTE skill[2];
   333: 	BYTE index[2];
   334: 	BYTE target[2];
   335: 	#endif
   336: };
   337: 
   338: struct PMSG_SKILL_DARK_SIDE_SEND
   339: {
   340: 	PBMSG_HEAD header; // C3:4B
   341: 	WORD skill;
   342: 	WORD index[5];
   343: };
   344: 
   345: struct PMSG_SKILL_NOVA_SEND
   346: {
   347: 	PBMSG_HEAD header; // C1:BA
   348: 	BYTE index[2];
   349: 	BYTE type;
   350: 	BYTE count;
   351: };
   352: 
   353: struct PMSG_SKILL_CHAIN_SKILL_SEND
   354: {
   355: 	PSBMSG_HEAD header; // C1:BF:[0A:18]
   356: 	BYTE skill[2];
   357: 	WORD index;
   358: 	BYTE count;
   359: };
   360: 
   361: struct PMSG_SKILL_CHAIN_SKILL
   362: {
   363: 	WORD index;
   364: };
   365: 
   366: struct PMSG_SKILL_LIST_SEND
   367: {
   368: 	PSBMSG_HEAD header; // C1:F3:11
   369: 	BYTE count;
   370: 	BYTE type;
   371: };
   372: 
   373: struct PMSG_SKILL_LIST
   374: {
   375: 	#pragma pack(1)
   376: 	BYTE slot;
   377: 	WORD skill;
   378: 	BYTE level;
   379: 	#pragma pack()
   380: };
   381: 
   382: //**********************************************//
   383: //**********************************************//
   384: //**********************************************//
   385: 
   386: struct SKILL_INFO
   387: {
   388: 	int Index;
   389: 	char Name[32];
   390: 	int Damage;
   391: 	int Mana;
   392: 	int BP;
   393: 	int Range;
   394: 	int Radio;
   395: 	int Delay;
   396: 	int Type;
   397: 	int Effect;
   398: 	int RequireLevel;
   399: 	int RequireEnergy;
   400: 	int RequireLeadership;
   401: 	int RequireKillCount;
   402: 	int RequireGuildStatus;
   403: 	int RequireClass[MAX_CLASS];
   404: };
   405: 
   406: class CSkillManager
```


---

## QUESTIONS (preencher como CONFIRMED/PROBABLE com âncora)

1) Quais valores (u16) são BOTH_ATTACK1 e BOTH_ATTACK2 no enum ProtocolHead?

2) O body dessas mensagens é headerless, ou embute PBMSG (C1/size/head) dentro?

3) Campos exatos (ordem, tamanho, endianness) do body de BOTH_ATTACK1 e BOTH_ATTACK2.

4) Existe validação de tamanho antes de acessar/castar o body?

5) O GS chama CGAttackRecv/CGMultiSkillAttackRecv com buffer clássico reconstituído, ou interpreta campos diretamente do body olc?

---

## ANSWERS (1.3-R)

### Q1) Valores u16 — CONFIRMADO
`SocketManagerEnum.h :3-:22` é **token-idêntico** ao enum do client (ProtocolSend.h :7-:26) ⇒
**BOTH_ATTACK1 = ordinal 8 = 0x0008 · BOTH_ATTACK2 = 9 = 0x0009** (BOTH_ATTACK3=10=0x000A existe no enum
mas NÃO tem case; BOTH_MESSAGE=12=0x000C ✔ revalida 1.3-M/1.3-Q). Protocolo simétrico client↔GS.

### Q2) Headerless ou embute PBMSG? — CONFIRMADO: EMBUTE
`PMSG_ATTACK_RECV` (Attack.h :13-:19) = **`PBMSG_HEAD header; // C1:[PROTOCOL_CODE2]`** + campos;
`PMSG_MULTI_SKILL_ATTACK_RECV` (SkillManager.h :166-:183) = **`PBMSG_HEAD header; // C3:[PROTOCOL_CODE4]`** + campos.
ⓘ Mesmo padrão do BOTH_POSITION (1.3-L): o body olc é a **struct clássica inteira COM os 3 B de header
embutidos**; os handlers **não leem** os bytes de header (CGAttackRecv lê index/action/dir;
CGMulti lê skillH/count/skillL/x/serial/y) ⇒ header **OPACO** no olc.

### Q3) Campos exatos — CONFIRMADO
- **BOTH_ATTACK1 body = 7 B** (frame olc 13 B): `[hdr3 opaco][indexH][indexL][action][dir]`
  — index = alvo **BE** (`MAKE_NUMBERW(index[0],index[1])`; macro Protocol.h :71 = `(y)|(x<<8)` ⇒ BE);
  action → `GCActionSend` (Attack.cpp :3024 — client manda AT_ATTACK1=0x78); dir → `lpObj->Dir` (:3018).
- **BOTH_ATTACK2 body = 9 B** (frame olc 15 B) — branch **GAMESERVER_UPDATE>=701 ATIVO** (fork tem cases 802):
  `[hdr3 opaco][skillH][count][skillL][x][serial][y]` — skill = **MAKE_NUMBERW(skillH,skillL) = BE apesar do
  layout intercalado** (skillH…skillL separados por count). Branch <701 (8 B: skill[2]/x/y/serial/count)
  documentado porém INATIVO neste fork.
- ⚠ DIFERENÇA vs TX clássico 0xDB (1.3-O): `0xDB = [TypeH][TypeL][x][y][Serial][Count]` ≠
  `olc 0x0009 = [skillH][count][skillL][x][serial][y]` — **ordem remapeada** (não é o mesmo body!).

### Q4) Validação de tamanho — CONFIRMADO: NENHUMA
Dispatcher moderno (SocketManagerModern.cpp :116-:121) faz **cast direto** de `msg.body.data()` sem checar
`body.size()`/`header.size`. Guards apenas semânticos DENTRO dos handlers (gObjIsConnected/Live/Teleport/
OBJECT_RANGE/CheckAttr). Risco documentado (qualidade do fork) — um body curto = leitura OOB no GS real.

### Q5) Buffer clássico reconstituído ou interpretação direta? — CONFIRMADO: DIRETA
Handlers chamados com `(PMSG_*)msg.body.data()` — **interpretação direta do body olc como a struct clássica**
(sem reconstituir buffer C1, sem passar pelo ProtocolCore; bypass total do switch clássico).

## Fetches extras desta análise
- Attack.cpp 85.613 B sha256 dd7459f2d5f520e8a0a8916026d64e036e2300c1936948d7869a319faa2c3090 (CGAttackRecv)
- SkillManager.cpp 146.528 B sha256 83b537dddc552400a39043ed064e6394ad9e95bec585ccd073aede2c73d27b46 (CGMultiSkillAttackRecv)
- Structs: Attack.h 52f3dd5b… · SkillManager.h 2d706d9f… (shas na seção Files fetched)

## Implicação p/ o MVP (builders futuros)
- TX moderno nativo do attack: `[0800][07000000][hdr3][idxH][idxL][action][dir]` (13 B) —_action=0x78_.
- TX moderno nativo do skill: `[0900][09000000][hdr3][skillH][count][skillL][x][serial][y]` (15 B).
- Ambos com hdr3 opaco (não lido). Túnel BOTH_MESSAGE (C1 0x11/0xDB) permanece válido em paralelo (Q5 1.3-Q).
