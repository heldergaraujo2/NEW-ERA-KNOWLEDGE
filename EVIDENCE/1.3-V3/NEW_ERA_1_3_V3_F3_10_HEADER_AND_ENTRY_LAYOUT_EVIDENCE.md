# NEW-ERA 1.3-V3 — F3:10 (Inventário) — Header C2/C4 + Entry layout (GS) — EVIDENCE

- Goal: confirmar PSWMSG_HEAD (byte inicial/tamanho/head/sub) e confirmar tamanho do item (MAX_ITEM_INFO / 7B).


## Files fetched (sha256)

- `GS_Protocol.h` sha256 `943c59b7a457eccea4953b7410fd2140c6299979782b58ca81a7b42a52d6c3b0` bytes=36305
- `ItemManager.h` sha256 `2ad786682724c2b40fa15c6cd2bf2d25948cdf408173bcc4c8489d2b411e0b64` bytes=12056
- `ItemManager.cpp` sha256 `2ebe79ada6400e0a1d06c92ca76a2513260ea36590258f7ac0ccafdca2656030` bytes=105683

---

## GS_Protocol.h — token `\bPSWMSG_HEAD\b`

- first hit @ line 147
```
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
   361: {
   362: 	PSBMSG_HEAD header; // C1:F3:15
   363: 	char name[10];
   364: };
   365: 
   366: struct PMSG_CHARACTER_NAME_CHANGE_RECV
   367: {
   368: 	PSBMSG_HEAD header; // C1:F3:16
   369: 	char OldName[10];
   370: 	char NewName[10];
   371: };
   372: 
   373: struct PMSG_OPTION_CHANGE_SKIN_RECV
   374: {
   375: 	PSBMSG_HEAD header; // C1:F3:21
   376: 	BYTE ChangeSkin;
   377: };
   378: 
   379: struct PMSG_OPTION_DATA_RECV
   380: {
   381: 	#pragma pack(1)
   382: 	PSBMSG_HEAD header; // C1:F3:30
   383: 	BYTE SkillKey[20];
   384: 	BYTE GameOption;
   385: 	BYTE QKey;
   386: 	BYTE WKey;
   387: 	BYTE EKey;
   388: 	BYTE ChatWindow;
   389: 	BYTE RKey;
   390: 	DWORD QWERLevel;
   391: 	#pragma pack()
   392: };
   393: 
   394: struct PMSG_CLIENT_SECURITY_BREACH_RECV
   395: {
   396: 	PSBMSG_HEAD header; // C1:F3:31
   397: 	BYTE code[4];
   398: };
   399: 
   400: struct PMSG_SNS_DATA_RECV
   401: {
   402: 	PWMSG_HEAD header; // C1:FB
   403: 	BYTE result;
   404: 	BYTE data[256];
   405: };
   406: 
   407: struct PMSG_SNS_DATA_LOG_RECV
   408: {
   409: 	PBMSG_HEAD header; // C1:FC
   410: 	BYTE code[3];
   411: };
   412: 
   413: struct PMSG_OFFTRADE_RECV
   414: {
   415: 	PSBMSG_HEAD header;
   416: 	int Type;
   417: };
   418: 
   419: //**********************************************//
   420: //************ GameServer -> Client ************//
   421: //**********************************************//
   422: 
   423: struct PMSG_CHAT_SEND
   424: {
   425: 	PBMSG_HEAD header; // C1:00
   426: 	char name[10];
   427: 	char message[60];
   428: };
   429: 
   430: struct PMSG_CHAT_TARGET_SEND
   431: {
   432: 	PBMSG_HEAD header; // C1:01
   433: 	BYTE index[2];
   434: 	char message[60];
   435: };
   436: 
   437: struct PMSG_CHAT_WHISPER_SEND
   438: {
   439: 	PBMSG_HEAD header; // C1:02
   440: 	char name[10];
   441: 	char message[60];
   442: };
   443: 
   444: struct PMSG_MAIN_CHECK_SEND
   445: {
   446: 	PBMSG_HEAD header; // C1:03
   447: 	WORD key;
   448: };
   449: 
   450: struct PMSG_EVENT_STATE_SEND
   451: {
   452: 	PBMSG_HEAD header; // C1:0B
   453: 	BYTE state;
   454: 	BYTE event;
   455: };
   456: 
   457: struct PMSG_SERVER_MSG_SEND
   458: {
   459: 	PBMSG_HEAD header; // C1:0C
   460: 	BYTE MsgNumber;
   461: };
   462: 
   463: struct PMSG_WEATHER_SEND
   464: {
   465: 	PBMSG_HEAD header; // C1:0F
   466: 	BYTE weather;
   467: };
```

## GS_Protocol.h — token `\bPWMSG_HEAD\b`

- first hit @ line 124
```
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
   361: {
   362: 	PSBMSG_HEAD header; // C1:F3:15
   363: 	char name[10];
   364: };
   365: 
   366: struct PMSG_CHARACTER_NAME_CHANGE_RECV
   367: {
   368: 	PSBMSG_HEAD header; // C1:F3:16
   369: 	char OldName[10];
   370: 	char NewName[10];
   371: };
   372: 
   373: struct PMSG_OPTION_CHANGE_SKIN_RECV
   374: {
   375: 	PSBMSG_HEAD header; // C1:F3:21
   376: 	BYTE ChangeSkin;
   377: };
   378: 
   379: struct PMSG_OPTION_DATA_RECV
   380: {
   381: 	#pragma pack(1)
   382: 	PSBMSG_HEAD header; // C1:F3:30
   383: 	BYTE SkillKey[20];
   384: 	BYTE GameOption;
   385: 	BYTE QKey;
   386: 	BYTE WKey;
   387: 	BYTE EKey;
   388: 	BYTE ChatWindow;
   389: 	BYTE RKey;
   390: 	DWORD QWERLevel;
   391: 	#pragma pack()
   392: };
   393: 
   394: struct PMSG_CLIENT_SECURITY_BREACH_RECV
   395: {
   396: 	PSBMSG_HEAD header; // C1:F3:31
   397: 	BYTE code[4];
   398: };
   399: 
   400: struct PMSG_SNS_DATA_RECV
   401: {
   402: 	PWMSG_HEAD header; // C1:FB
   403: 	BYTE result;
   404: 	BYTE data[256];
   405: };
   406: 
   407: struct PMSG_SNS_DATA_LOG_RECV
   408: {
   409: 	PBMSG_HEAD header; // C1:FC
   410: 	BYTE code[3];
   411: };
   412: 
   413: struct PMSG_OFFTRADE_RECV
   414: {
   415: 	PSBMSG_HEAD header;
   416: 	int Type;
   417: };
   418: 
   419: //**********************************************//
   420: //************ GameServer -> Client ************//
   421: //**********************************************//
   422: 
   423: struct PMSG_CHAT_SEND
   424: {
   425: 	PBMSG_HEAD header; // C1:00
   426: 	char name[10];
   427: 	char message[60];
   428: };
   429: 
   430: struct PMSG_CHAT_TARGET_SEND
   431: {
   432: 	PBMSG_HEAD header; // C1:01
   433: 	BYTE index[2];
   434: 	char message[60];
   435: };
   436: 
   437: struct PMSG_CHAT_WHISPER_SEND
   438: {
   439: 	PBMSG_HEAD header; // C1:02
   440: 	char name[10];
   441: 	char message[60];
   442: };
   443: 
   444: struct PMSG_MAIN_CHECK_SEND
```

## GS_Protocol.h — token `\bPSBMSG_HEAD\b`

- first hit @ line 100
```
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
   361: {
   362: 	PSBMSG_HEAD header; // C1:F3:15
   363: 	char name[10];
   364: };
   365: 
   366: struct PMSG_CHARACTER_NAME_CHANGE_RECV
   367: {
   368: 	PSBMSG_HEAD header; // C1:F3:16
   369: 	char OldName[10];
   370: 	char NewName[10];
   371: };
   372: 
   373: struct PMSG_OPTION_CHANGE_SKIN_RECV
   374: {
   375: 	PSBMSG_HEAD header; // C1:F3:21
   376: 	BYTE ChangeSkin;
   377: };
   378: 
   379: struct PMSG_OPTION_DATA_RECV
   380: {
   381: 	#pragma pack(1)
   382: 	PSBMSG_HEAD header; // C1:F3:30
   383: 	BYTE SkillKey[20];
   384: 	BYTE GameOption;
   385: 	BYTE QKey;
   386: 	BYTE WKey;
   387: 	BYTE EKey;
   388: 	BYTE ChatWindow;
   389: 	BYTE RKey;
   390: 	DWORD QWERLevel;
   391: 	#pragma pack()
   392: };
   393: 
   394: struct PMSG_CLIENT_SECURITY_BREACH_RECV
   395: {
   396: 	PSBMSG_HEAD header; // C1:F3:31
   397: 	BYTE code[4];
   398: };
   399: 
   400: struct PMSG_SNS_DATA_RECV
   401: {
   402: 	PWMSG_HEAD header; // C1:FB
   403: 	BYTE result;
   404: 	BYTE data[256];
   405: };
   406: 
   407: struct PMSG_SNS_DATA_LOG_RECV
   408: {
   409: 	PBMSG_HEAD header; // C1:FC
   410: 	BYTE code[3];
   411: };
   412: 
   413: struct PMSG_OFFTRADE_RECV
   414: {
   415: 	PSBMSG_HEAD header;
   416: 	int Type;
   417: };
   418: 
   419: //**********************************************//
   420: //************ GameServer -> Client ************//
```

## GS_Protocol.h — token `\bPBMSG_HEAD\b`

- first hit @ line 79
```
     1: #pragma once
     2: 
     3: #if(GAMESERVER_UPDATE>=701)
     4: #include "Util\\mapm\\M_APM.h"
     5: #endif
     6: #include "User.h"
     7: 
     8: #if(GAMESERVER_LANGUAGE==0)
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
   361: {
   362: 	PSBMSG_HEAD header; // C1:F3:15
   363: 	char name[10];
   364: };
   365: 
   366: struct PMSG_CHARACTER_NAME_CHANGE_RECV
   367: {
   368: 	PSBMSG_HEAD header; // C1:F3:16
   369: 	char OldName[10];
   370: 	char NewName[10];
   371: };
   372: 
   373: struct PMSG_OPTION_CHANGE_SKIN_RECV
   374: {
   375: 	PSBMSG_HEAD header; // C1:F3:21
   376: 	BYTE ChangeSkin;
   377: };
   378: 
   379: struct PMSG_OPTION_DATA_RECV
   380: {
   381: 	#pragma pack(1)
   382: 	PSBMSG_HEAD header; // C1:F3:30
   383: 	BYTE SkillKey[20];
   384: 	BYTE GameOption;
   385: 	BYTE QKey;
   386: 	BYTE WKey;
   387: 	BYTE EKey;
   388: 	BYTE ChatWindow;
   389: 	BYTE RKey;
   390: 	DWORD QWERLevel;
   391: 	#pragma pack()
   392: };
   393: 
   394: struct PMSG_CLIENT_SECURITY_BREACH_RECV
   395: {
   396: 	PSBMSG_HEAD header; // C1:F3:31
   397: 	BYTE code[4];
   398: };
   399: 
```

## GS_Protocol.h — token `\bstruct\s+PSWMSG_HEAD\b`

- first hit @ line 147
```
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
   361: {
   362: 	PSBMSG_HEAD header; // C1:F3:15
   363: 	char name[10];
   364: };
   365: 
   366: struct PMSG_CHARACTER_NAME_CHANGE_RECV
   367: {
   368: 	PSBMSG_HEAD header; // C1:F3:16
   369: 	char OldName[10];
   370: 	char NewName[10];
   371: };
   372: 
   373: struct PMSG_OPTION_CHANGE_SKIN_RECV
   374: {
   375: 	PSBMSG_HEAD header; // C1:F3:21
   376: 	BYTE ChangeSkin;
   377: };
   378: 
   379: struct PMSG_OPTION_DATA_RECV
   380: {
   381: 	#pragma pack(1)
   382: 	PSBMSG_HEAD header; // C1:F3:30
   383: 	BYTE SkillKey[20];
   384: 	BYTE GameOption;
   385: 	BYTE QKey;
   386: 	BYTE WKey;
   387: 	BYTE EKey;
   388: 	BYTE ChatWindow;
   389: 	BYTE RKey;
   390: 	DWORD QWERLevel;
   391: 	#pragma pack()
   392: };
   393: 
   394: struct PMSG_CLIENT_SECURITY_BREACH_RECV
   395: {
   396: 	PSBMSG_HEAD header; // C1:F3:31
   397: 	BYTE code[4];
   398: };
   399: 
   400: struct PMSG_SNS_DATA_RECV
   401: {
   402: 	PWMSG_HEAD header; // C1:FB
   403: 	BYTE result;
   404: 	BYTE data[256];
   405: };
   406: 
   407: struct PMSG_SNS_DATA_LOG_RECV
   408: {
   409: 	PBMSG_HEAD header; // C1:FC
   410: 	BYTE code[3];
   411: };
   412: 
   413: struct PMSG_OFFTRADE_RECV
   414: {
   415: 	PSBMSG_HEAD header;
   416: 	int Type;
   417: };
   418: 
   419: //**********************************************//
   420: //************ GameServer -> Client ************//
   421: //**********************************************//
   422: 
   423: struct PMSG_CHAT_SEND
   424: {
   425: 	PBMSG_HEAD header; // C1:00
   426: 	char name[10];
   427: 	char message[60];
   428: };
   429: 
   430: struct PMSG_CHAT_TARGET_SEND
   431: {
   432: 	PBMSG_HEAD header; // C1:01
   433: 	BYTE index[2];
   434: 	char message[60];
   435: };
   436: 
   437: struct PMSG_CHAT_WHISPER_SEND
   438: {
   439: 	PBMSG_HEAD header; // C1:02
   440: 	char name[10];
   441: 	char message[60];
   442: };
   443: 
   444: struct PMSG_MAIN_CHECK_SEND
   445: {
   446: 	PBMSG_HEAD header; // C1:03
   447: 	WORD key;
   448: };
   449: 
   450: struct PMSG_EVENT_STATE_SEND
   451: {
   452: 	PBMSG_HEAD header; // C1:0B
   453: 	BYTE state;
   454: 	BYTE event;
   455: };
   456: 
   457: struct PMSG_SERVER_MSG_SEND
   458: {
   459: 	PBMSG_HEAD header; // C1:0C
   460: 	BYTE MsgNumber;
   461: };
   462: 
   463: struct PMSG_WEATHER_SEND
   464: {
   465: 	PBMSG_HEAD header; // C1:0F
   466: 	BYTE weather;
   467: };
```

## GS_Protocol.h — token `\bclass\s+PSWMSG_HEAD\b`

- no hits


## GS_Protocol.h — token `\bvoid\s+set\s*\(`

- first hit @ line 81
```
     1: #pragma once
     2: 
     3: #if(GAMESERVER_UPDATE>=701)
     4: #include "Util\\mapm\\M_APM.h"
     5: #endif
     6: #include "User.h"
     7: 
     8: #if(GAMESERVER_LANGUAGE==0)
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
   361: {
   362: 	PSBMSG_HEAD header; // C1:F3:15
   363: 	char name[10];
   364: };
   365: 
   366: struct PMSG_CHARACTER_NAME_CHANGE_RECV
   367: {
   368: 	PSBMSG_HEAD header; // C1:F3:16
   369: 	char OldName[10];
   370: 	char NewName[10];
   371: };
   372: 
   373: struct PMSG_OPTION_CHANGE_SKIN_RECV
   374: {
   375: 	PSBMSG_HEAD header; // C1:F3:21
   376: 	BYTE ChangeSkin;
   377: };
   378: 
   379: struct PMSG_OPTION_DATA_RECV
   380: {
   381: 	#pragma pack(1)
   382: 	PSBMSG_HEAD header; // C1:F3:30
   383: 	BYTE SkillKey[20];
   384: 	BYTE GameOption;
   385: 	BYTE QKey;
   386: 	BYTE WKey;
   387: 	BYTE EKey;
   388: 	BYTE ChatWindow;
   389: 	BYTE RKey;
   390: 	DWORD QWERLevel;
   391: 	#pragma pack()
   392: };
   393: 
   394: struct PMSG_CLIENT_SECURITY_BREACH_RECV
   395: {
   396: 	PSBMSG_HEAD header; // C1:F3:31
   397: 	BYTE code[4];
   398: };
   399: 
   400: struct PMSG_SNS_DATA_RECV
   401: {
```


---

## ItemManager.h — token `\bPMSG_ITEM_LIST_SEND\b`

- first hit @ line 167
```
    87: };
    88: 
    89: struct PMSG_ITEM_REPAIR_RECV
    90: {
    91: 	PBMSG_HEAD header; // C1:34
    92: 	BYTE slot;
    93: 	BYTE type;
    94: };
    95: 
    96: //**********************************************//
    97: //************ GameServer -> Client ************//
    98: //**********************************************//
    99: 
   100: struct PMSG_ITEM_GET_SEND
   101: {
   102: 	PBMSG_HEAD header; // C3:22
   103: 	BYTE result;
   104: 	BYTE ItemInfo[MAX_ITEM_INFO];
   105: };
   106: 
   107: struct PMSG_ITEM_DROP_SEND
   108: {
   109: 	PBMSG_HEAD header; // C1:23
   110: 	BYTE result;
   111: 	BYTE slot;
   112: };
   113: 
   114: struct PMSG_ITEM_MOVE_SEND
   115: {
   116: 	PBMSG_HEAD header; // C3:24
   117: 	BYTE result;
   118: 	BYTE slot;
   119: 	BYTE ItemInfo[MAX_ITEM_INFO];
   120: };
   121: 
   122: struct PMSG_ITEM_CHANGE_SEND
   123: {
   124: 	PBMSG_HEAD header; // C1:25
   125: 	BYTE index[2];
   126: 	BYTE ItemInfo[MAX_ITEM_INFO];
   127: 	#if(GAMESERVER_UPDATE>=701)
   128: 	BYTE attribute;
   129: 	#endif
   130: };
   131: 
   132: struct PMSG_ITEM_DELETE_SEND
   133: {
   134: 	PBMSG_HEAD header; // C1:28
   135: 	BYTE slot;
   136: 	BYTE flag;
   137: };
   138: 
   139: struct PMSG_ITEM_DUR_SEND
   140: {
   141: 	PBMSG_HEAD header; // C1:2A
   142: 	BYTE slot;
   143: 	BYTE dur;
   144: 	BYTE flag;
   145: };
   146: 
   147: struct PMSG_ITEM_BUY_SEND
   148: {
   149: 	PBMSG_HEAD header; // C1:32
   150: 	BYTE result;
   151: 	BYTE ItemInfo[MAX_ITEM_INFO];
   152: };
   153: 
   154: struct PMSG_ITEM_SELL_SEND
   155: {
   156: 	PBMSG_HEAD header; // C1:33
   157: 	BYTE result;
   158: 	DWORD money;
   159: };
   160: 
   161: struct PMSG_ITEM_REPAIR_SEND
   162: {
   163: 	PBMSG_HEAD header; // C1:34
   164: 	DWORD money;
   165: };
   166: 
   167: struct PMSG_ITEM_LIST_SEND
   168: {
   169: 	PSWMSG_HEAD header; // C4:F3:10
   170: 	BYTE count;
   171: };
   172: 
   173: struct PMSG_ITEM_LIST
   174: {
   175: 	BYTE slot;
   176: 	BYTE ItemInfo[MAX_ITEM_INFO];
   177: };
   178: 
   179: struct PMSG_ITEM_EQUIPMENT_SEND
   180: {
   181: 	PSBMSG_HEAD header; // C1:F3:13
   182: 	BYTE index[2];
   183: 	BYTE CharSet[18];
   184: };
   185: 
   186: struct PMSG_ITEM_MODIFY_SEND
   187: {
   188: 	PSBMSG_HEAD header; // C1:F3:14
   189: 	BYTE slot;
   190: 	BYTE ItemInfo[MAX_ITEM_INFO];
   191: };
   192: 
   193: struct PMSG_ITEM_BUY_NEW
   194: {
   195: 	PSBMSG_HEAD header; // C1:32
   196: 	BYTE slot;
   197: };
   198: 
   199: //**********************************************//
   200: //**********************************************//
   201: //**********************************************//
   202: 
   203: struct ITEM_INFO
   204: {
   205: 	int Index;
   206: 	int Slot;
   207: 	int Skill;
   208: 	int Width;
   209: 	int Height;
   210: 	int HaveSerial;
   211: 	int HaveOption;
   212: 	int DropItem;
   213: 	char Name[32];
   214: 	int Level;
   215: 	int DamageMin;
   216: 	int DamageMax;
   217: 	int MagicDamageRate;
   218: 	int TwoHand;
   219: 	int Defense;
   220: 	int MagicDefense;
   221: 	int DefenseSuccessRate;
   222: 	int AttackSpeed;
   223: 	int WalkSpeed;
   224: 	int Durability;
   225: 	int MagicDurability;
   226: 	int Value;
   227: 	int BuyMoney;
   228: 	int Resistance[MAX_RESISTANCE_TYPE];
   229: 	int RequireLevel;
   230: 	int RequireStrength;
   231: 	int RequireDexterity;
   232: 	int RequireEnergy;
   233: 	int RequireVitality;
   234: 	int RequireLeadership;
   235: 	int RequireClass[MAX_CLASS];
   236: };
   237: 
   238: //MC
   239: typedef struct
   240: {
   241: 	char Name[32]; // 0
   242: 	BYTE HaveItemInfo; // 20
   243: 	BYTE TwoHand;	// 21
   244: 	BYTE Level; // 22
   245: 	BYTE Width;	// 23
   246: 	BYTE Height;	// 24
   247: 	char Serial; // 25
   248: 	BYTE OptionFlag; // 26
   249: 	BYTE MondownFlag; // 27
   250: 	BYTE AttackSpeed; // 28
   251: 	BYTE WalkSpeed; // 29
   252: 	BYTE DamageMin; // 2A
   253: 	BYTE DamageMax; // 2B
   254: 	BYTE SuccessfulBlocking; // 2C
   255: 	BYTE Defense; // 2D
   256: 	BYTE MagicDefense; // 2E
   257: 	BYTE Speed;	// 2F
   258: 	BYTE Durability; // 30
   259: 	BYTE MagicDurability; // 31
   260: 	BYTE AttackDur;	// 32
   261: 	BYTE DefenceDur; // 33
   262: 	WORD RequireStrength; // 34
   263: 	WORD RequireDexterity; // 36
   264: 	WORD RequireEnergy; // 38
   265: 	WORD RequireLevel; // 3A
   266: 	WORD Value; // 3C
   267: 	BYTE RequireClass[MAX_CLASS]; // 3E
   268: 	BYTE Resistance[MAX_RESISTANCE_TYPE]; // unk43 - Ice poisonous lightning non ground wind water
   269: 	WORD RequireVitality;	// 4A
   270: 	WORD RequireLeadership;	// 68
   271: 	int BuyMoney; // 4C
   272: 	int MagicPW; // 50
   273: 	float RepaireMoneyRate;	// 54
   274: 	float AllRepaireMoneyRate; // 58
   275: 	bool QuestItem;	// 5C
   276: 	BYTE SetAttr; // 5D
   277: 	BYTE ResistanceType;	// 5E
   278: 	BYTE ItemSlot; // 60
   279: 	int SkillType; // 64
   280: 	BOOL isSocketItem;
   281: }  ITEM_ATTRIBUTE, * LPITEM_ATTRIBUTE;
   282: 
   283: //MC
   284: 
   285: class CItemManager
   286: {
   287: public:
   288: 	CItemManager();
   289: 	virtual ~CItemManager();
   290: 	void Load(char* path);
   291: 	bool GetInfo(int index,ITEM_INFO* lpInfo);
   292: 	int GetItemSkill(int index);
   293: 	int GetItemTwoHand(int index);
   294: 	int GetItemDurability(int index,int level,int NewOption,int SetOption);
   295: 	int GetItemRepairMoney(CItem* lpItem,int type);
   296: 	int GetInventoryItemSlot(LPOBJ lpObj,int index,int level);
   297: 	int GetInventoryItemCount(LPOBJ lpObj,int index,int level);
   298: 	int GetInventoryEmptySlotCount(LPOBJ lpObj);
   299: 	int GetInventoryMaxValue(LPOBJ lpObj);
   300: 	bool CheckItemRequireLevel(LPOBJ lpObj,CItem* lpItem);
   301: 	bool CheckItemRequireStrength(LPOBJ lpObj,CItem* lpItem);
   302: 	bool CheckItemRequireDexterity(LPOBJ lpObj,CItem* lpItem);
   303: 	bool CheckItemRequireVitality(LPOBJ lpObj,CItem* lpItem);
   304: 	bool CheckItemRequireEnergy(LPOBJ lpObj,CItem* lpItem);
   305: 	bool CheckItemRequireLeadership(LPOBJ lpObj,CItem* lpItem);
   306: 	bool CheckItemRequireClass(LPOBJ lpObj,int index);
   307: 	bool CheckItemMoveToInventory(LPOBJ lpObj,CItem* lpItem,int slot);
   308: 	bool CheckItemMoveToTrade(LPOBJ lpObj,CItem* lpItem,BYTE TargetFlag);
   309: 	bool CheckItemMoveToVault(LPOBJ lpObj,CItem* lpItem,BYTE TargetFlag);
   310: 	bool CheckItemMoveToChaos(LPOBJ lpObj,CItem* lpItem,BYTE TargetFlag);
   311: 	bool CheckItemMoveToBlock(LPOBJ lpObj,CItem* lpItem);
   312: 	bool CheckItemInventorySpace(LPOBJ lpObj,int index);
   313: 	bool CheckItemInventorySpace(LPOBJ lpObj,int width,int height);
   314: 	void InventoryItemSet(int aIndex,int slot,BYTE type);
   315: 	BYTE InventoryRectCheck(int aIndex,int x,int y,int width,int height);
   316: 	BYTE InventoryInsertItem(int aIndex,CItem item);
   317: 	BYTE InventoryAddItem(int aIndex,CItem item,int slot);
   318: 	void InventoryDelItem(int aIndex,int slot);
   319: 	bool InventoryInsertItemStack(LPOBJ lpObj,CItem* lpItem);
   320: 	bool InventoryAddItemStack(LPOBJ lpObj,int SourceSlot,int TargetSlot);
   321: 	void TradeItemSet(int aIndex,int slot,BYTE type) ;
   322: 	BYTE TradeRectCheck(int aIndex,int x,int y,int width,int height);
   323: 	BYTE TradeInsertItem(int aIndex,CItem item);
   324: 	BYTE TradeAddItem(int aIndex,CItem item,int slot);
   325: 	void TradeDelItem(int aIndex,int slot);
   326: 	void WarehouseItemSet(int aIndex,int slot,BYTE type);
   327: 	BYTE WarehouseRectCheck(int aIndex,int x,int y,int width,int height);
   328: 	BYTE WarehouseInsertItem(int aIndex,CItem item);
   329: 	BYTE WarehouseAddItem(int aIndex,CItem item,int slot);
   330: 	void WarehouseDelItem(int aIndex,int slot);
   331: 	void ChaosBoxItemSet(int aIndex,int slot,BYTE type);
   332: 	BYTE ChaosBoxRectCheck(int aIndex,int x,int y,int width,int height);
   333: 	BYTE ChaosBoxInsertItem(int aIndex,CItem item);
   334: 	BYTE ChaosBoxAddItem(int aIndex,CItem item,int slot);
   335: 	void ChaosBoxDelItem(int aIndex,int slot);
   336: 	void ItemByteConvert(BYTE* lpMsg,CItem item);
   337: 	void DBItemByteConvert(BYTE* lpMsg,CItem* lpItem);
   338: 	bool ConvertItemByte(CItem* lpItem,BYTE* lpMsg);
   339: 	bool IsValidItem(LPOBJ lpObj,CItem* lpItem);
   340: 	void UpdateInventoryViewport(int aIndex,int slot);
   341: 	void DeleteInventoryItemCount(LPOBJ lpObj,int index,int level,int count);
   342: 	void DecreaseItemDur(LPOBJ lpObj,int slot,int dur);
   343: 	int RepairItem(LPOBJ lpObj,CItem* lpItem,int slot,int type);
   344: 	BYTE MoveItemToInventoryFromInventory(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
   345: 	BYTE MoveItemToInventoryFromTrade(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
   346: 	BYTE MoveItemToInventoryFromWarehouse(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
   347: 	BYTE MoveItemToInventoryFromChaosBox(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
   348: 	BYTE MoveItemToInventoryFromPersonalShop(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
   349: 	BYTE MoveItemToTradeFromInventory(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
   350: 	BYTE MoveItemToTradeFromTrade(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
   351: 	BYTE MoveItemToTradeFromEventInventory(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
   352: 	BYTE MoveItemToWarehouseFromInventory(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
   353: 	BYTE MoveItemToWarehouseFromWarehouse(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
   354: 	BYTE MoveItemToChaosBoxFromInventory(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
   355: 	BYTE MoveItemToChaosBoxFromChaosBox(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
   356: 	BYTE MoveItemToPersonalShopFromInventory(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
   357: 	BYTE MoveItemToPersonalShopFromPersonalShop(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
   358: 	BYTE MoveItemToEventInventoryFromTrade(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
   359: 	BYTE MoveItemToEventInventoryFromEventInventory(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
   360: 	BYTE MoveItemToMuunInventoryFromMuunInventory(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
   361: 	void CGItemGetRecv(PMSG_ITEM_GET_RECV* lpMsg,int aIndex);
   362: 	void CGItemDropRecv(PMSG_ITEM_DROP_RECV* lpMsg,int aIndex);
   363: 	bool CGPkDrop(PMSG_ITEM_DROP_RECV* lpMsg,int aIndex);
   364: 	void CGItemMoveRecv(PMSG_ITEM_MOVE_RECV* lpMsg,int aIndex);
   365: 	void CGItemUseRecv(PMSG_ITEM_USE_RECV* lpMsg,int aIndex);
   366: 	void CGItemBuyRecv(PMSG_ITEM_BUY_RECV* lpMsg,int aIndex, int ok);
   367: 	void CGItemSellRecv(PMSG_ITEM_SELL_RECV* lpMsg,int aIndex);
   368: 	void CGItemRepairRecv(PMSG_ITEM_REPAIR_RECV* lpMsg,int aIndex);
   369: 	void GCItemMoveSend(int aIndex,BYTE result,BYTE slot,BYTE* ItemInfo);
   370: 	void GCItemChangeSend(int aIndex,BYTE slot);
   371: 	void GCItemDeleteSend(int aIndex,BYTE slot,BYTE flag);
   372: 	void GCItemDurSend(int aIndex,BYTE slot,BYTE dur,BYTE flag);
   373: 	void GCItemListSend(int aIndex);
   374: 	void GCItemEquipmentSend(int aIndex);
   375: 	void GCItemModifySend(int aIndex,BYTE slot);
   376: 	char* GetItemName(int index);
   377: 	void CGItemBuyConfirmRecv(PMSG_ITEM_BUY_NEW* lpMsg,int aIndex);
   378: private:
   379: 	std::map<int,ITEM_INFO> m_ItemInfo;
   380: };
   381: 
   382: extern  ITEM_ATTRIBUTE ItemAttribute[MAX_ITEM];//MC
   383: extern int g_MaxItemIndexOfEachItemType[MAX_ITEM_TYPE];//MC
   384: extern CItemManager gItemManager;
```

## ItemManager.h — token `\bPMSG_ITEM_LIST\b`

- first hit @ line 173
```
    93: 	BYTE type;
    94: };
    95: 
    96: //**********************************************//
    97: //************ GameServer -> Client ************//
    98: //**********************************************//
    99: 
   100: struct PMSG_ITEM_GET_SEND
   101: {
   102: 	PBMSG_HEAD header; // C3:22
   103: 	BYTE result;
   104: 	BYTE ItemInfo[MAX_ITEM_INFO];
   105: };
   106: 
   107: struct PMSG_ITEM_DROP_SEND
   108: {
   109: 	PBMSG_HEAD header; // C1:23
   110: 	BYTE result;
   111: 	BYTE slot;
   112: };
   113: 
   114: struct PMSG_ITEM_MOVE_SEND
   115: {
   116: 	PBMSG_HEAD header; // C3:24
   117: 	BYTE result;
   118: 	BYTE slot;
   119: 	BYTE ItemInfo[MAX_ITEM_INFO];
   120: };
   121: 
   122: struct PMSG_ITEM_CHANGE_SEND
   123: {
   124: 	PBMSG_HEAD header; // C1:25
   125: 	BYTE index[2];
   126: 	BYTE ItemInfo[MAX_ITEM_INFO];
   127: 	#if(GAMESERVER_UPDATE>=701)
   128: 	BYTE attribute;
   129: 	#endif
   130: };
   131: 
   132: struct PMSG_ITEM_DELETE_SEND
   133: {
   134: 	PBMSG_HEAD header; // C1:28
   135: 	BYTE slot;
   136: 	BYTE flag;
   137: };
   138: 
   139: struct PMSG_ITEM_DUR_SEND
   140: {
   141: 	PBMSG_HEAD header; // C1:2A
   142: 	BYTE slot;
   143: 	BYTE dur;
   144: 	BYTE flag;
   145: };
   146: 
   147: struct PMSG_ITEM_BUY_SEND
   148: {
   149: 	PBMSG_HEAD header; // C1:32
   150: 	BYTE result;
   151: 	BYTE ItemInfo[MAX_ITEM_INFO];
   152: };
   153: 
   154: struct PMSG_ITEM_SELL_SEND
   155: {
   156: 	PBMSG_HEAD header; // C1:33
   157: 	BYTE result;
   158: 	DWORD money;
   159: };
   160: 
   161: struct PMSG_ITEM_REPAIR_SEND
   162: {
   163: 	PBMSG_HEAD header; // C1:34
   164: 	DWORD money;
   165: };
   166: 
   167: struct PMSG_ITEM_LIST_SEND
   168: {
   169: 	PSWMSG_HEAD header; // C4:F3:10
   170: 	BYTE count;
   171: };
   172: 
   173: struct PMSG_ITEM_LIST
   174: {
   175: 	BYTE slot;
   176: 	BYTE ItemInfo[MAX_ITEM_INFO];
   177: };
   178: 
   179: struct PMSG_ITEM_EQUIPMENT_SEND
   180: {
   181: 	PSBMSG_HEAD header; // C1:F3:13
   182: 	BYTE index[2];
   183: 	BYTE CharSet[18];
   184: };
   185: 
   186: struct PMSG_ITEM_MODIFY_SEND
   187: {
   188: 	PSBMSG_HEAD header; // C1:F3:14
   189: 	BYTE slot;
   190: 	BYTE ItemInfo[MAX_ITEM_INFO];
   191: };
   192: 
   193: struct PMSG_ITEM_BUY_NEW
   194: {
   195: 	PSBMSG_HEAD header; // C1:32
   196: 	BYTE slot;
   197: };
   198: 
   199: //**********************************************//
   200: //**********************************************//
   201: //**********************************************//
   202: 
   203: struct ITEM_INFO
   204: {
   205: 	int Index;
   206: 	int Slot;
   207: 	int Skill;
   208: 	int Width;
   209: 	int Height;
   210: 	int HaveSerial;
   211: 	int HaveOption;
   212: 	int DropItem;
   213: 	char Name[32];
   214: 	int Level;
   215: 	int DamageMin;
   216: 	int DamageMax;
   217: 	int MagicDamageRate;
   218: 	int TwoHand;
   219: 	int Defense;
   220: 	int MagicDefense;
   221: 	int DefenseSuccessRate;
   222: 	int AttackSpeed;
   223: 	int WalkSpeed;
   224: 	int Durability;
   225: 	int MagicDurability;
   226: 	int Value;
   227: 	int BuyMoney;
   228: 	int Resistance[MAX_RESISTANCE_TYPE];
   229: 	int RequireLevel;
   230: 	int RequireStrength;
   231: 	int RequireDexterity;
   232: 	int RequireEnergy;
   233: 	int RequireVitality;
   234: 	int RequireLeadership;
   235: 	int RequireClass[MAX_CLASS];
   236: };
   237: 
   238: //MC
   239: typedef struct
   240: {
   241: 	char Name[32]; // 0
   242: 	BYTE HaveItemInfo; // 20
   243: 	BYTE TwoHand;	// 21
   244: 	BYTE Level; // 22
   245: 	BYTE Width;	// 23
   246: 	BYTE Height;	// 24
   247: 	char Serial; // 25
   248: 	BYTE OptionFlag; // 26
   249: 	BYTE MondownFlag; // 27
   250: 	BYTE AttackSpeed; // 28
   251: 	BYTE WalkSpeed; // 29
   252: 	BYTE DamageMin; // 2A
   253: 	BYTE DamageMax; // 2B
   254: 	BYTE SuccessfulBlocking; // 2C
   255: 	BYTE Defense; // 2D
   256: 	BYTE MagicDefense; // 2E
   257: 	BYTE Speed;	// 2F
   258: 	BYTE Durability; // 30
   259: 	BYTE MagicDurability; // 31
   260: 	BYTE AttackDur;	// 32
   261: 	BYTE DefenceDur; // 33
   262: 	WORD RequireStrength; // 34
   263: 	WORD RequireDexterity; // 36
   264: 	WORD RequireEnergy; // 38
   265: 	WORD RequireLevel; // 3A
   266: 	WORD Value; // 3C
   267: 	BYTE RequireClass[MAX_CLASS]; // 3E
   268: 	BYTE Resistance[MAX_RESISTANCE_TYPE]; // unk43 - Ice poisonous lightning non ground wind water
   269: 	WORD RequireVitality;	// 4A
   270: 	WORD RequireLeadership;	// 68
   271: 	int BuyMoney; // 4C
   272: 	int MagicPW; // 50
   273: 	float RepaireMoneyRate;	// 54
   274: 	float AllRepaireMoneyRate; // 58
   275: 	bool QuestItem;	// 5C
   276: 	BYTE SetAttr; // 5D
   277: 	BYTE ResistanceType;	// 5E
   278: 	BYTE ItemSlot; // 60
   279: 	int SkillType; // 64
   280: 	BOOL isSocketItem;
   281: }  ITEM_ATTRIBUTE, * LPITEM_ATTRIBUTE;
   282: 
   283: //MC
   284: 
   285: class CItemManager
   286: {
   287: public:
   288: 	CItemManager();
   289: 	virtual ~CItemManager();
   290: 	void Load(char* path);
   291: 	bool GetInfo(int index,ITEM_INFO* lpInfo);
   292: 	int GetItemSkill(int index);
   293: 	int GetItemTwoHand(int index);
   294: 	int GetItemDurability(int index,int level,int NewOption,int SetOption);
   295: 	int GetItemRepairMoney(CItem* lpItem,int type);
   296: 	int GetInventoryItemSlot(LPOBJ lpObj,int index,int level);
   297: 	int GetInventoryItemCount(LPOBJ lpObj,int index,int level);
   298: 	int GetInventoryEmptySlotCount(LPOBJ lpObj);
   299: 	int GetInventoryMaxValue(LPOBJ lpObj);
   300: 	bool CheckItemRequireLevel(LPOBJ lpObj,CItem* lpItem);
   301: 	bool CheckItemRequireStrength(LPOBJ lpObj,CItem* lpItem);
   302: 	bool CheckItemRequireDexterity(LPOBJ lpObj,CItem* lpItem);
   303: 	bool CheckItemRequireVitality(LPOBJ lpObj,CItem* lpItem);
   304: 	bool CheckItemRequireEnergy(LPOBJ lpObj,CItem* lpItem);
   305: 	bool CheckItemRequireLeadership(LPOBJ lpObj,CItem* lpItem);
   306: 	bool CheckItemRequireClass(LPOBJ lpObj,int index);
   307: 	bool CheckItemMoveToInventory(LPOBJ lpObj,CItem* lpItem,int slot);
   308: 	bool CheckItemMoveToTrade(LPOBJ lpObj,CItem* lpItem,BYTE TargetFlag);
   309: 	bool CheckItemMoveToVault(LPOBJ lpObj,CItem* lpItem,BYTE TargetFlag);
   310: 	bool CheckItemMoveToChaos(LPOBJ lpObj,CItem* lpItem,BYTE TargetFlag);
   311: 	bool CheckItemMoveToBlock(LPOBJ lpObj,CItem* lpItem);
   312: 	bool CheckItemInventorySpace(LPOBJ lpObj,int index);
   313: 	bool CheckItemInventorySpace(LPOBJ lpObj,int width,int height);
   314: 	void InventoryItemSet(int aIndex,int slot,BYTE type);
   315: 	BYTE InventoryRectCheck(int aIndex,int x,int y,int width,int height);
   316: 	BYTE InventoryInsertItem(int aIndex,CItem item);
   317: 	BYTE InventoryAddItem(int aIndex,CItem item,int slot);
   318: 	void InventoryDelItem(int aIndex,int slot);
   319: 	bool InventoryInsertItemStack(LPOBJ lpObj,CItem* lpItem);
   320: 	bool InventoryAddItemStack(LPOBJ lpObj,int SourceSlot,int TargetSlot);
   321: 	void TradeItemSet(int aIndex,int slot,BYTE type) ;
   322: 	BYTE TradeRectCheck(int aIndex,int x,int y,int width,int height);
   323: 	BYTE TradeInsertItem(int aIndex,CItem item);
   324: 	BYTE TradeAddItem(int aIndex,CItem item,int slot);
   325: 	void TradeDelItem(int aIndex,int slot);
   326: 	void WarehouseItemSet(int aIndex,int slot,BYTE type);
   327: 	BYTE WarehouseRectCheck(int aIndex,int x,int y,int width,int height);
   328: 	BYTE WarehouseInsertItem(int aIndex,CItem item);
   329: 	BYTE WarehouseAddItem(int aIndex,CItem item,int slot);
   330: 	void WarehouseDelItem(int aIndex,int slot);
   331: 	void ChaosBoxItemSet(int aIndex,int slot,BYTE type);
   332: 	BYTE ChaosBoxRectCheck(int aIndex,int x,int y,int width,int height);
   333: 	BYTE ChaosBoxInsertItem(int aIndex,CItem item);
   334: 	BYTE ChaosBoxAddItem(int aIndex,CItem item,int slot);
   335: 	void ChaosBoxDelItem(int aIndex,int slot);
   336: 	void ItemByteConvert(BYTE* lpMsg,CItem item);
   337: 	void DBItemByteConvert(BYTE* lpMsg,CItem* lpItem);
   338: 	bool ConvertItemByte(CItem* lpItem,BYTE* lpMsg);
   339: 	bool IsValidItem(LPOBJ lpObj,CItem* lpItem);
   340: 	void UpdateInventoryViewport(int aIndex,int slot);
   341: 	void DeleteInventoryItemCount(LPOBJ lpObj,int index,int level,int count);
   342: 	void DecreaseItemDur(LPOBJ lpObj,int slot,int dur);
   343: 	int RepairItem(LPOBJ lpObj,CItem* lpItem,int slot,int type);
   344: 	BYTE MoveItemToInventoryFromInventory(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
   345: 	BYTE MoveItemToInventoryFromTrade(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
   346: 	BYTE MoveItemToInventoryFromWarehouse(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
   347: 	BYTE MoveItemToInventoryFromChaosBox(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
   348: 	BYTE MoveItemToInventoryFromPersonalShop(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
   349: 	BYTE MoveItemToTradeFromInventory(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
   350: 	BYTE MoveItemToTradeFromTrade(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
   351: 	BYTE MoveItemToTradeFromEventInventory(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
   352: 	BYTE MoveItemToWarehouseFromInventory(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
   353: 	BYTE MoveItemToWarehouseFromWarehouse(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
   354: 	BYTE MoveItemToChaosBoxFromInventory(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
   355: 	BYTE MoveItemToChaosBoxFromChaosBox(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
   356: 	BYTE MoveItemToPersonalShopFromInventory(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
   357: 	BYTE MoveItemToPersonalShopFromPersonalShop(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
   358: 	BYTE MoveItemToEventInventoryFromTrade(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
   359: 	BYTE MoveItemToEventInventoryFromEventInventory(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
   360: 	BYTE MoveItemToMuunInventoryFromMuunInventory(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
   361: 	void CGItemGetRecv(PMSG_ITEM_GET_RECV* lpMsg,int aIndex);
   362: 	void CGItemDropRecv(PMSG_ITEM_DROP_RECV* lpMsg,int aIndex);
   363: 	bool CGPkDrop(PMSG_ITEM_DROP_RECV* lpMsg,int aIndex);
   364: 	void CGItemMoveRecv(PMSG_ITEM_MOVE_RECV* lpMsg,int aIndex);
   365: 	void CGItemUseRecv(PMSG_ITEM_USE_RECV* lpMsg,int aIndex);
   366: 	void CGItemBuyRecv(PMSG_ITEM_BUY_RECV* lpMsg,int aIndex, int ok);
   367: 	void CGItemSellRecv(PMSG_ITEM_SELL_RECV* lpMsg,int aIndex);
   368: 	void CGItemRepairRecv(PMSG_ITEM_REPAIR_RECV* lpMsg,int aIndex);
   369: 	void GCItemMoveSend(int aIndex,BYTE result,BYTE slot,BYTE* ItemInfo);
   370: 	void GCItemChangeSend(int aIndex,BYTE slot);
   371: 	void GCItemDeleteSend(int aIndex,BYTE slot,BYTE flag);
   372: 	void GCItemDurSend(int aIndex,BYTE slot,BYTE dur,BYTE flag);
   373: 	void GCItemListSend(int aIndex);
   374: 	void GCItemEquipmentSend(int aIndex);
   375: 	void GCItemModifySend(int aIndex,BYTE slot);
   376: 	char* GetItemName(int index);
   377: 	void CGItemBuyConfirmRecv(PMSG_ITEM_BUY_NEW* lpMsg,int aIndex);
   378: private:
   379: 	std::map<int,ITEM_INFO> m_ItemInfo;
   380: };
   381: 
   382: extern  ITEM_ATTRIBUTE ItemAttribute[MAX_ITEM];//MC
   383: extern int g_MaxItemIndexOfEachItemType[MAX_ITEM_TYPE];//MC
   384: extern CItemManager gItemManager;
```

## ItemManager.h — token `\bMAX_ITEM_INFO\b`

- first hit @ line 14
```
     1: // ItemManager.h: interface for the CItemManager class.
     2: //
     3: //////////////////////////////////////////////////////////////////////
     4: 
     5: #pragma once
     6: 
     7: #include "DefaultClassInfo.h"
     8: #include "Protocol.h"
     9: #include "User.h"
    10: 
    11: #define MAX_ITEM_SECTION 16
    12: #define MAX_ITEM_TYPE 512
    13: #define MAX_ITEM (MAX_ITEM_SECTION*MAX_ITEM_TYPE)
    14: #define MAX_ITEM_INFO 12
    15: 
    16: #define GET_ITEM(x,y) (((x)*MAX_ITEM_TYPE)+(y))
    17: #define CHECK_ITEM(x) (((x)<0)?-1:((x)>=MAX_ITEM)?-1:x)
    18: #define INVENTORY_RANGE(x) (((x)<0)?0:((x)>=INVENTORY_SIZE)?0:1)
    19: #if(GAMESERVER_UPDATE>=701)
    20: #define INVENTORY_WEAR_RANGE(x) (((x)<0)?0:((x)>=INVENTORY_WEAR_SIZE)?(((x)==236)?1:0):1)
    21: #define INVENTORY_FULL_RANGE(x) (((x)<0)?0:((x)>=INVENTORY_EXT4_SIZE)?(((x)==236)?1:0):1)
    22: #else
    23: #define INVENTORY_WEAR_RANGE(x) (((x)<0)?0:((x)>=INVENTORY_WEAR_SIZE)?0:1)
    24: #define INVENTORY_FULL_RANGE(x) (((x)<0)?0:((x)>=INVENTORY_EXT4_SIZE)?0:1)
    25: #endif
    26: #define INVENTORY_BASE_RANGE(x) (((x)<INVENTORY_WEAR_SIZE)?0:((x)>=INVENTORY_SIZE)?0:1)
    27: #define INVENTORY_MAIN_RANGE(x) (((x)<INVENTORY_WEAR_SIZE)?0:((x)>=INVENTORY_MAIN_SIZE)?0:1)
    28: #define INVENTORY_EXT1_RANGE(x) (((x)<INVENTORY_MAIN_SIZE)?0:((x)>=INVENTORY_EXT1_SIZE)?0:1)
    29: #define INVENTORY_EXT2_RANGE(x) (((x)<INVENTORY_EXT1_SIZE)?0:((x)>=INVENTORY_EXT2_SIZE)?0:1)
    30: #define INVENTORY_EXT3_RANGE(x) (((x)<INVENTORY_EXT2_SIZE)?0:((x)>=INVENTORY_EXT3_SIZE)?0:1)
    31: #define INVENTORY_EXT4_RANGE(x) (((x)<INVENTORY_EXT3_SIZE)?0:((x)>=INVENTORY_EXT4_SIZE)?0:1)
    32: #define INVENTORY_SHOP_RANGE(x) (((x)<INVENTORY_EXT4_SIZE)?0:((x)>=INVENTORY_FULL_SIZE)?0:1)
    33: #define TRADE_RANGE(x) (((x)<0)?0:((x)>=TRADE_SIZE)?0:1)
    34: #define WAREHOUSE_RANGE(x) (((x)<0)?0:((x)>=WAREHOUSE_SIZE)?0:1)
    35: #define WAREHOUSE_MAIN_RANGE(x) (((x)<0)?0:((x)>=WAREHOUSE_EXT1_SIZE)?0:1)
    36: #define WAREHOUSE_EXT1_RANGE(x) (((x)<WAREHOUSE_EXT1_SIZE)?0:((x)>=WAREHOUSE_SIZE)?0:1)
    37: #define CHAOS_BOX_RANGE(x) (((x)<0)?0:((x)>=CHAOS_BOX_SIZE)?0:1)
    38: 
    39: #define MAX_ITEM_SPECIAL_ATTRIBUTE 7 //MC bot
    40: 
    41: //**********************************************//
    42: //************ Client -> GameServer ************//
    43: //**********************************************//
    44: 
    45: struct PMSG_ITEM_GET_RECV
    46: {
    47: 	PBMSG_HEAD header; // C1:22
    48: 	BYTE index[2];
    49: };
    50: 
    51: struct PMSG_ITEM_DROP_RECV
    52: {
    53: 	PBMSG_HEAD header; // C1:23
    54: 	BYTE x;
    55: 	BYTE y;
    56: 	BYTE slot;
    57: };
    58: 
    59: struct PMSG_ITEM_MOVE_RECV
    60: {
    61: 	PBMSG_HEAD header; // C1:24
    62: 	BYTE SourceFlag;
    63: 	BYTE SourceSlot;
    64: 	BYTE ItemInfo[MAX_ITEM_INFO];
    65: 	BYTE TargetFlag;
    66: 	BYTE TargetSlot;
    67: };
    68: 
    69: struct PMSG_ITEM_USE_RECV
    70: {
    71: 	PBMSG_HEAD header; // C1:26
    72: 	BYTE SourceSlot;
    73: 	BYTE TargetSlot;
    74: 	BYTE type;
    75: };
    76: 
    77: struct PMSG_ITEM_BUY_RECV
    78: {
    79: 	PBMSG_HEAD header; // C1:32
    80: 	BYTE slot;
    81: };
    82: 
    83: struct PMSG_ITEM_SELL_RECV
    84: {
    85: 	PBMSG_HEAD header; // C1:33
    86: 	BYTE slot;
    87: };
    88: 
    89: struct PMSG_ITEM_REPAIR_RECV
    90: {
    91: 	PBMSG_HEAD header; // C1:34
    92: 	BYTE slot;
    93: 	BYTE type;
    94: };
    95: 
    96: //**********************************************//
    97: //************ GameServer -> Client ************//
    98: //**********************************************//
    99: 
   100: struct PMSG_ITEM_GET_SEND
   101: {
   102: 	PBMSG_HEAD header; // C3:22
   103: 	BYTE result;
   104: 	BYTE ItemInfo[MAX_ITEM_INFO];
   105: };
   106: 
   107: struct PMSG_ITEM_DROP_SEND
   108: {
   109: 	PBMSG_HEAD header; // C1:23
   110: 	BYTE result;
   111: 	BYTE slot;
   112: };
   113: 
   114: struct PMSG_ITEM_MOVE_SEND
   115: {
   116: 	PBMSG_HEAD header; // C3:24
   117: 	BYTE result;
   118: 	BYTE slot;
   119: 	BYTE ItemInfo[MAX_ITEM_INFO];
   120: };
   121: 
   122: struct PMSG_ITEM_CHANGE_SEND
   123: {
   124: 	PBMSG_HEAD header; // C1:25
   125: 	BYTE index[2];
   126: 	BYTE ItemInfo[MAX_ITEM_INFO];
   127: 	#if(GAMESERVER_UPDATE>=701)
   128: 	BYTE attribute;
   129: 	#endif
   130: };
   131: 
   132: struct PMSG_ITEM_DELETE_SEND
   133: {
   134: 	PBMSG_HEAD header; // C1:28
   135: 	BYTE slot;
   136: 	BYTE flag;
   137: };
   138: 
   139: struct PMSG_ITEM_DUR_SEND
   140: {
   141: 	PBMSG_HEAD header; // C1:2A
   142: 	BYTE slot;
   143: 	BYTE dur;
   144: 	BYTE flag;
   145: };
   146: 
   147: struct PMSG_ITEM_BUY_SEND
   148: {
   149: 	PBMSG_HEAD header; // C1:32
   150: 	BYTE result;
   151: 	BYTE ItemInfo[MAX_ITEM_INFO];
   152: };
   153: 
   154: struct PMSG_ITEM_SELL_SEND
   155: {
   156: 	PBMSG_HEAD header; // C1:33
   157: 	BYTE result;
   158: 	DWORD money;
   159: };
   160: 
   161: struct PMSG_ITEM_REPAIR_SEND
   162: {
   163: 	PBMSG_HEAD header; // C1:34
   164: 	DWORD money;
   165: };
   166: 
   167: struct PMSG_ITEM_LIST_SEND
   168: {
   169: 	PSWMSG_HEAD header; // C4:F3:10
   170: 	BYTE count;
   171: };
   172: 
   173: struct PMSG_ITEM_LIST
   174: {
   175: 	BYTE slot;
   176: 	BYTE ItemInfo[MAX_ITEM_INFO];
   177: };
   178: 
   179: struct PMSG_ITEM_EQUIPMENT_SEND
   180: {
   181: 	PSBMSG_HEAD header; // C1:F3:13
   182: 	BYTE index[2];
   183: 	BYTE CharSet[18];
   184: };
   185: 
   186: struct PMSG_ITEM_MODIFY_SEND
   187: {
   188: 	PSBMSG_HEAD header; // C1:F3:14
   189: 	BYTE slot;
   190: 	BYTE ItemInfo[MAX_ITEM_INFO];
   191: };
   192: 
   193: struct PMSG_ITEM_BUY_NEW
   194: {
   195: 	PSBMSG_HEAD header; // C1:32
   196: 	BYTE slot;
   197: };
   198: 
   199: //**********************************************//
   200: //**********************************************//
   201: //**********************************************//
   202: 
   203: struct ITEM_INFO
   204: {
   205: 	int Index;
   206: 	int Slot;
   207: 	int Skill;
   208: 	int Width;
   209: 	int Height;
   210: 	int HaveSerial;
   211: 	int HaveOption;
   212: 	int DropItem;
   213: 	char Name[32];
   214: 	int Level;
   215: 	int DamageMin;
   216: 	int DamageMax;
   217: 	int MagicDamageRate;
   218: 	int TwoHand;
   219: 	int Defense;
   220: 	int MagicDefense;
   221: 	int DefenseSuccessRate;
   222: 	int AttackSpeed;
   223: 	int WalkSpeed;
   224: 	int Durability;
   225: 	int MagicDurability;
   226: 	int Value;
   227: 	int BuyMoney;
   228: 	int Resistance[MAX_RESISTANCE_TYPE];
   229: 	int RequireLevel;
   230: 	int RequireStrength;
   231: 	int RequireDexterity;
   232: 	int RequireEnergy;
   233: 	int RequireVitality;
   234: 	int RequireLeadership;
   235: 	int RequireClass[MAX_CLASS];
   236: };
   237: 
   238: //MC
   239: typedef struct
   240: {
   241: 	char Name[32]; // 0
   242: 	BYTE HaveItemInfo; // 20
   243: 	BYTE TwoHand;	// 21
   244: 	BYTE Level; // 22
   245: 	BYTE Width;	// 23
   246: 	BYTE Height;	// 24
   247: 	char Serial; // 25
   248: 	BYTE OptionFlag; // 26
   249: 	BYTE MondownFlag; // 27
   250: 	BYTE AttackSpeed; // 28
   251: 	BYTE WalkSpeed; // 29
   252: 	BYTE DamageMin; // 2A
   253: 	BYTE DamageMax; // 2B
   254: 	BYTE SuccessfulBlocking; // 2C
   255: 	BYTE Defense; // 2D
   256: 	BYTE MagicDefense; // 2E
   257: 	BYTE Speed;	// 2F
   258: 	BYTE Durability; // 30
   259: 	BYTE MagicDurability; // 31
   260: 	BYTE AttackDur;	// 32
   261: 	BYTE DefenceDur; // 33
   262: 	WORD RequireStrength; // 34
   263: 	WORD RequireDexterity; // 36
   264: 	WORD RequireEnergy; // 38
   265: 	WORD RequireLevel; // 3A
   266: 	WORD Value; // 3C
   267: 	BYTE RequireClass[MAX_CLASS]; // 3E
   268: 	BYTE Resistance[MAX_RESISTANCE_TYPE]; // unk43 - Ice poisonous lightning non ground wind water
   269: 	WORD RequireVitality;	// 4A
   270: 	WORD RequireLeadership;	// 68
   271: 	int BuyMoney; // 4C
   272: 	int MagicPW; // 50
   273: 	float RepaireMoneyRate;	// 54
   274: 	float AllRepaireMoneyRate; // 58
   275: 	bool QuestItem;	// 5C
   276: 	BYTE SetAttr; // 5D
   277: 	BYTE ResistanceType;	// 5E
   278: 	BYTE ItemSlot; // 60
   279: 	int SkillType; // 64
   280: 	BOOL isSocketItem;
   281: }  ITEM_ATTRIBUTE, * LPITEM_ATTRIBUTE;
   282: 
   283: //MC
   284: 
   285: class CItemManager
   286: {
   287: public:
   288: 	CItemManager();
   289: 	virtual ~CItemManager();
   290: 	void Load(char* path);
   291: 	bool GetInfo(int index,ITEM_INFO* lpInfo);
   292: 	int GetItemSkill(int index);
   293: 	int GetItemTwoHand(int index);
   294: 	int GetItemDurability(int index,int level,int NewOption,int SetOption);
   295: 	int GetItemRepairMoney(CItem* lpItem,int type);
   296: 	int GetInventoryItemSlot(LPOBJ lpObj,int index,int level);
   297: 	int GetInventoryItemCount(LPOBJ lpObj,int index,int level);
   298: 	int GetInventoryEmptySlotCount(LPOBJ lpObj);
   299: 	int GetInventoryMaxValue(LPOBJ lpObj);
   300: 	bool CheckItemRequireLevel(LPOBJ lpObj,CItem* lpItem);
   301: 	bool CheckItemRequireStrength(LPOBJ lpObj,CItem* lpItem);
   302: 	bool CheckItemRequireDexterity(LPOBJ lpObj,CItem* lpItem);
   303: 	bool CheckItemRequireVitality(LPOBJ lpObj,CItem* lpItem);
   304: 	bool CheckItemRequireEnergy(LPOBJ lpObj,CItem* lpItem);
   305: 	bool CheckItemRequireLeadership(LPOBJ lpObj,CItem* lpItem);
   306: 	bool CheckItemRequireClass(LPOBJ lpObj,int index);
   307: 	bool CheckItemMoveToInventory(LPOBJ lpObj,CItem* lpItem,int slot);
   308: 	bool CheckItemMoveToTrade(LPOBJ lpObj,CItem* lpItem,BYTE TargetFlag);
   309: 	bool CheckItemMoveToVault(LPOBJ lpObj,CItem* lpItem,BYTE TargetFlag);
   310: 	bool CheckItemMoveToChaos(LPOBJ lpObj,CItem* lpItem,BYTE TargetFlag);
   311: 	bool CheckItemMoveToBlock(LPOBJ lpObj,CItem* lpItem);
   312: 	bool CheckItemInventorySpace(LPOBJ lpObj,int index);
   313: 	bool CheckItemInventorySpace(LPOBJ lpObj,int width,int height);
   314: 	void InventoryItemSet(int aIndex,int slot,BYTE type);
   315: 	BYTE InventoryRectCheck(int aIndex,int x,int y,int width,int height);
   316: 	BYTE InventoryInsertItem(int aIndex,CItem item);
   317: 	BYTE InventoryAddItem(int aIndex,CItem item,int slot);
   318: 	void InventoryDelItem(int aIndex,int slot);
   319: 	bool InventoryInsertItemStack(LPOBJ lpObj,CItem* lpItem);
   320: 	bool InventoryAddItemStack(LPOBJ lpObj,int SourceSlot,int TargetSlot);
   321: 	void TradeItemSet(int aIndex,int slot,BYTE type) ;
   322: 	BYTE TradeRectCheck(int aIndex,int x,int y,int width,int height);
   323: 	BYTE TradeInsertItem(int aIndex,CItem item);
   324: 	BYTE TradeAddItem(int aIndex,CItem item,int slot);
   325: 	void TradeDelItem(int aIndex,int slot);
   326: 	void WarehouseItemSet(int aIndex,int slot,BYTE type);
   327: 	BYTE WarehouseRectCheck(int aIndex,int x,int y,int width,int height);
   328: 	BYTE WarehouseInsertItem(int aIndex,CItem item);
   329: 	BYTE WarehouseAddItem(int aIndex,CItem item,int slot);
   330: 	void WarehouseDelItem(int aIndex,int slot);
   331: 	void ChaosBoxItemSet(int aIndex,int slot,BYTE type);
   332: 	BYTE ChaosBoxRectCheck(int aIndex,int x,int y,int width,int height);
   333: 	BYTE ChaosBoxInsertItem(int aIndex,CItem item);
   334: 	BYTE ChaosBoxAddItem(int aIndex,CItem item,int slot);
```

## ItemManager.h — token `\bPACKET_ITEM_LENGTH\b`

- no hits


## ItemManager.h — token `\bItemInfo\s*\[\s*MAX_ITEM_INFO\s*\]`

- first hit @ line 64
```
     1: // ItemManager.h: interface for the CItemManager class.
     2: //
     3: //////////////////////////////////////////////////////////////////////
     4: 
     5: #pragma once
     6: 
     7: #include "DefaultClassInfo.h"
     8: #include "Protocol.h"
     9: #include "User.h"
    10: 
    11: #define MAX_ITEM_SECTION 16
    12: #define MAX_ITEM_TYPE 512
    13: #define MAX_ITEM (MAX_ITEM_SECTION*MAX_ITEM_TYPE)
    14: #define MAX_ITEM_INFO 12
    15: 
    16: #define GET_ITEM(x,y) (((x)*MAX_ITEM_TYPE)+(y))
    17: #define CHECK_ITEM(x) (((x)<0)?-1:((x)>=MAX_ITEM)?-1:x)
    18: #define INVENTORY_RANGE(x) (((x)<0)?0:((x)>=INVENTORY_SIZE)?0:1)
    19: #if(GAMESERVER_UPDATE>=701)
    20: #define INVENTORY_WEAR_RANGE(x) (((x)<0)?0:((x)>=INVENTORY_WEAR_SIZE)?(((x)==236)?1:0):1)
    21: #define INVENTORY_FULL_RANGE(x) (((x)<0)?0:((x)>=INVENTORY_EXT4_SIZE)?(((x)==236)?1:0):1)
    22: #else
    23: #define INVENTORY_WEAR_RANGE(x) (((x)<0)?0:((x)>=INVENTORY_WEAR_SIZE)?0:1)
    24: #define INVENTORY_FULL_RANGE(x) (((x)<0)?0:((x)>=INVENTORY_EXT4_SIZE)?0:1)
    25: #endif
    26: #define INVENTORY_BASE_RANGE(x) (((x)<INVENTORY_WEAR_SIZE)?0:((x)>=INVENTORY_SIZE)?0:1)
    27: #define INVENTORY_MAIN_RANGE(x) (((x)<INVENTORY_WEAR_SIZE)?0:((x)>=INVENTORY_MAIN_SIZE)?0:1)
    28: #define INVENTORY_EXT1_RANGE(x) (((x)<INVENTORY_MAIN_SIZE)?0:((x)>=INVENTORY_EXT1_SIZE)?0:1)
    29: #define INVENTORY_EXT2_RANGE(x) (((x)<INVENTORY_EXT1_SIZE)?0:((x)>=INVENTORY_EXT2_SIZE)?0:1)
    30: #define INVENTORY_EXT3_RANGE(x) (((x)<INVENTORY_EXT2_SIZE)?0:((x)>=INVENTORY_EXT3_SIZE)?0:1)
    31: #define INVENTORY_EXT4_RANGE(x) (((x)<INVENTORY_EXT3_SIZE)?0:((x)>=INVENTORY_EXT4_SIZE)?0:1)
    32: #define INVENTORY_SHOP_RANGE(x) (((x)<INVENTORY_EXT4_SIZE)?0:((x)>=INVENTORY_FULL_SIZE)?0:1)
    33: #define TRADE_RANGE(x) (((x)<0)?0:((x)>=TRADE_SIZE)?0:1)
    34: #define WAREHOUSE_RANGE(x) (((x)<0)?0:((x)>=WAREHOUSE_SIZE)?0:1)
    35: #define WAREHOUSE_MAIN_RANGE(x) (((x)<0)?0:((x)>=WAREHOUSE_EXT1_SIZE)?0:1)
    36: #define WAREHOUSE_EXT1_RANGE(x) (((x)<WAREHOUSE_EXT1_SIZE)?0:((x)>=WAREHOUSE_SIZE)?0:1)
    37: #define CHAOS_BOX_RANGE(x) (((x)<0)?0:((x)>=CHAOS_BOX_SIZE)?0:1)
    38: 
    39: #define MAX_ITEM_SPECIAL_ATTRIBUTE 7 //MC bot
    40: 
    41: //**********************************************//
    42: //************ Client -> GameServer ************//
    43: //**********************************************//
    44: 
    45: struct PMSG_ITEM_GET_RECV
    46: {
    47: 	PBMSG_HEAD header; // C1:22
    48: 	BYTE index[2];
    49: };
    50: 
    51: struct PMSG_ITEM_DROP_RECV
    52: {
    53: 	PBMSG_HEAD header; // C1:23
    54: 	BYTE x;
    55: 	BYTE y;
    56: 	BYTE slot;
    57: };
    58: 
    59: struct PMSG_ITEM_MOVE_RECV
    60: {
    61: 	PBMSG_HEAD header; // C1:24
    62: 	BYTE SourceFlag;
    63: 	BYTE SourceSlot;
    64: 	BYTE ItemInfo[MAX_ITEM_INFO];
    65: 	BYTE TargetFlag;
    66: 	BYTE TargetSlot;
    67: };
    68: 
    69: struct PMSG_ITEM_USE_RECV
    70: {
    71: 	PBMSG_HEAD header; // C1:26
    72: 	BYTE SourceSlot;
    73: 	BYTE TargetSlot;
    74: 	BYTE type;
    75: };
    76: 
    77: struct PMSG_ITEM_BUY_RECV
    78: {
    79: 	PBMSG_HEAD header; // C1:32
    80: 	BYTE slot;
    81: };
    82: 
    83: struct PMSG_ITEM_SELL_RECV
    84: {
    85: 	PBMSG_HEAD header; // C1:33
    86: 	BYTE slot;
    87: };
    88: 
    89: struct PMSG_ITEM_REPAIR_RECV
    90: {
    91: 	PBMSG_HEAD header; // C1:34
    92: 	BYTE slot;
    93: 	BYTE type;
    94: };
    95: 
    96: //**********************************************//
    97: //************ GameServer -> Client ************//
    98: //**********************************************//
    99: 
   100: struct PMSG_ITEM_GET_SEND
   101: {
   102: 	PBMSG_HEAD header; // C3:22
   103: 	BYTE result;
   104: 	BYTE ItemInfo[MAX_ITEM_INFO];
   105: };
   106: 
   107: struct PMSG_ITEM_DROP_SEND
   108: {
   109: 	PBMSG_HEAD header; // C1:23
   110: 	BYTE result;
   111: 	BYTE slot;
   112: };
   113: 
   114: struct PMSG_ITEM_MOVE_SEND
   115: {
   116: 	PBMSG_HEAD header; // C3:24
   117: 	BYTE result;
   118: 	BYTE slot;
   119: 	BYTE ItemInfo[MAX_ITEM_INFO];
   120: };
   121: 
   122: struct PMSG_ITEM_CHANGE_SEND
   123: {
   124: 	PBMSG_HEAD header; // C1:25
   125: 	BYTE index[2];
   126: 	BYTE ItemInfo[MAX_ITEM_INFO];
   127: 	#if(GAMESERVER_UPDATE>=701)
   128: 	BYTE attribute;
   129: 	#endif
   130: };
   131: 
   132: struct PMSG_ITEM_DELETE_SEND
   133: {
   134: 	PBMSG_HEAD header; // C1:28
   135: 	BYTE slot;
   136: 	BYTE flag;
   137: };
   138: 
   139: struct PMSG_ITEM_DUR_SEND
   140: {
   141: 	PBMSG_HEAD header; // C1:2A
   142: 	BYTE slot;
   143: 	BYTE dur;
   144: 	BYTE flag;
   145: };
   146: 
   147: struct PMSG_ITEM_BUY_SEND
   148: {
   149: 	PBMSG_HEAD header; // C1:32
   150: 	BYTE result;
   151: 	BYTE ItemInfo[MAX_ITEM_INFO];
   152: };
   153: 
   154: struct PMSG_ITEM_SELL_SEND
   155: {
   156: 	PBMSG_HEAD header; // C1:33
   157: 	BYTE result;
   158: 	DWORD money;
   159: };
   160: 
   161: struct PMSG_ITEM_REPAIR_SEND
   162: {
   163: 	PBMSG_HEAD header; // C1:34
   164: 	DWORD money;
   165: };
   166: 
   167: struct PMSG_ITEM_LIST_SEND
   168: {
   169: 	PSWMSG_HEAD header; // C4:F3:10
   170: 	BYTE count;
   171: };
   172: 
   173: struct PMSG_ITEM_LIST
   174: {
   175: 	BYTE slot;
   176: 	BYTE ItemInfo[MAX_ITEM_INFO];
   177: };
   178: 
   179: struct PMSG_ITEM_EQUIPMENT_SEND
   180: {
   181: 	PSBMSG_HEAD header; // C1:F3:13
   182: 	BYTE index[2];
   183: 	BYTE CharSet[18];
   184: };
   185: 
   186: struct PMSG_ITEM_MODIFY_SEND
   187: {
   188: 	PSBMSG_HEAD header; // C1:F3:14
   189: 	BYTE slot;
   190: 	BYTE ItemInfo[MAX_ITEM_INFO];
   191: };
   192: 
   193: struct PMSG_ITEM_BUY_NEW
   194: {
   195: 	PSBMSG_HEAD header; // C1:32
   196: 	BYTE slot;
   197: };
   198: 
   199: //**********************************************//
   200: //**********************************************//
   201: //**********************************************//
   202: 
   203: struct ITEM_INFO
   204: {
   205: 	int Index;
   206: 	int Slot;
   207: 	int Skill;
   208: 	int Width;
   209: 	int Height;
   210: 	int HaveSerial;
   211: 	int HaveOption;
   212: 	int DropItem;
   213: 	char Name[32];
   214: 	int Level;
   215: 	int DamageMin;
   216: 	int DamageMax;
   217: 	int MagicDamageRate;
   218: 	int TwoHand;
   219: 	int Defense;
   220: 	int MagicDefense;
   221: 	int DefenseSuccessRate;
   222: 	int AttackSpeed;
   223: 	int WalkSpeed;
   224: 	int Durability;
   225: 	int MagicDurability;
   226: 	int Value;
   227: 	int BuyMoney;
   228: 	int Resistance[MAX_RESISTANCE_TYPE];
   229: 	int RequireLevel;
   230: 	int RequireStrength;
   231: 	int RequireDexterity;
   232: 	int RequireEnergy;
   233: 	int RequireVitality;
   234: 	int RequireLeadership;
   235: 	int RequireClass[MAX_CLASS];
   236: };
   237: 
   238: //MC
   239: typedef struct
   240: {
   241: 	char Name[32]; // 0
   242: 	BYTE HaveItemInfo; // 20
   243: 	BYTE TwoHand;	// 21
   244: 	BYTE Level; // 22
   245: 	BYTE Width;	// 23
   246: 	BYTE Height;	// 24
   247: 	char Serial; // 25
   248: 	BYTE OptionFlag; // 26
   249: 	BYTE MondownFlag; // 27
   250: 	BYTE AttackSpeed; // 28
   251: 	BYTE WalkSpeed; // 29
   252: 	BYTE DamageMin; // 2A
   253: 	BYTE DamageMax; // 2B
   254: 	BYTE SuccessfulBlocking; // 2C
   255: 	BYTE Defense; // 2D
   256: 	BYTE MagicDefense; // 2E
   257: 	BYTE Speed;	// 2F
   258: 	BYTE Durability; // 30
   259: 	BYTE MagicDurability; // 31
   260: 	BYTE AttackDur;	// 32
   261: 	BYTE DefenceDur; // 33
   262: 	WORD RequireStrength; // 34
   263: 	WORD RequireDexterity; // 36
   264: 	WORD RequireEnergy; // 38
   265: 	WORD RequireLevel; // 3A
   266: 	WORD Value; // 3C
   267: 	BYTE RequireClass[MAX_CLASS]; // 3E
   268: 	BYTE Resistance[MAX_RESISTANCE_TYPE]; // unk43 - Ice poisonous lightning non ground wind water
   269: 	WORD RequireVitality;	// 4A
   270: 	WORD RequireLeadership;	// 68
   271: 	int BuyMoney; // 4C
   272: 	int MagicPW; // 50
   273: 	float RepaireMoneyRate;	// 54
   274: 	float AllRepaireMoneyRate; // 58
   275: 	bool QuestItem;	// 5C
   276: 	BYTE SetAttr; // 5D
   277: 	BYTE ResistanceType;	// 5E
   278: 	BYTE ItemSlot; // 60
   279: 	int SkillType; // 64
   280: 	BOOL isSocketItem;
   281: }  ITEM_ATTRIBUTE, * LPITEM_ATTRIBUTE;
   282: 
   283: //MC
   284: 
   285: class CItemManager
   286: {
   287: public:
   288: 	CItemManager();
   289: 	virtual ~CItemManager();
   290: 	void Load(char* path);
   291: 	bool GetInfo(int index,ITEM_INFO* lpInfo);
   292: 	int GetItemSkill(int index);
   293: 	int GetItemTwoHand(int index);
   294: 	int GetItemDurability(int index,int level,int NewOption,int SetOption);
   295: 	int GetItemRepairMoney(CItem* lpItem,int type);
   296: 	int GetInventoryItemSlot(LPOBJ lpObj,int index,int level);
   297: 	int GetInventoryItemCount(LPOBJ lpObj,int index,int level);
   298: 	int GetInventoryEmptySlotCount(LPOBJ lpObj);
   299: 	int GetInventoryMaxValue(LPOBJ lpObj);
   300: 	bool CheckItemRequireLevel(LPOBJ lpObj,CItem* lpItem);
   301: 	bool CheckItemRequireStrength(LPOBJ lpObj,CItem* lpItem);
   302: 	bool CheckItemRequireDexterity(LPOBJ lpObj,CItem* lpItem);
   303: 	bool CheckItemRequireVitality(LPOBJ lpObj,CItem* lpItem);
   304: 	bool CheckItemRequireEnergy(LPOBJ lpObj,CItem* lpItem);
   305: 	bool CheckItemRequireLeadership(LPOBJ lpObj,CItem* lpItem);
   306: 	bool CheckItemRequireClass(LPOBJ lpObj,int index);
   307: 	bool CheckItemMoveToInventory(LPOBJ lpObj,CItem* lpItem,int slot);
   308: 	bool CheckItemMoveToTrade(LPOBJ lpObj,CItem* lpItem,BYTE TargetFlag);
   309: 	bool CheckItemMoveToVault(LPOBJ lpObj,CItem* lpItem,BYTE TargetFlag);
   310: 	bool CheckItemMoveToChaos(LPOBJ lpObj,CItem* lpItem,BYTE TargetFlag);
   311: 	bool CheckItemMoveToBlock(LPOBJ lpObj,CItem* lpItem);
   312: 	bool CheckItemInventorySpace(LPOBJ lpObj,int index);
   313: 	bool CheckItemInventorySpace(LPOBJ lpObj,int width,int height);
   314: 	void InventoryItemSet(int aIndex,int slot,BYTE type);
   315: 	BYTE InventoryRectCheck(int aIndex,int x,int y,int width,int height);
   316: 	BYTE InventoryInsertItem(int aIndex,CItem item);
   317: 	BYTE InventoryAddItem(int aIndex,CItem item,int slot);
   318: 	void InventoryDelItem(int aIndex,int slot);
   319: 	bool InventoryInsertItemStack(LPOBJ lpObj,CItem* lpItem);
   320: 	bool InventoryAddItemStack(LPOBJ lpObj,int SourceSlot,int TargetSlot);
   321: 	void TradeItemSet(int aIndex,int slot,BYTE type) ;
   322: 	BYTE TradeRectCheck(int aIndex,int x,int y,int width,int height);
   323: 	BYTE TradeInsertItem(int aIndex,CItem item);
   324: 	BYTE TradeAddItem(int aIndex,CItem item,int slot);
   325: 	void TradeDelItem(int aIndex,int slot);
   326: 	void WarehouseItemSet(int aIndex,int slot,BYTE type);
   327: 	BYTE WarehouseRectCheck(int aIndex,int x,int y,int width,int height);
   328: 	BYTE WarehouseInsertItem(int aIndex,CItem item);
   329: 	BYTE WarehouseAddItem(int aIndex,CItem item,int slot);
   330: 	void WarehouseDelItem(int aIndex,int slot);
   331: 	void ChaosBoxItemSet(int aIndex,int slot,BYTE type);
   332: 	BYTE ChaosBoxRectCheck(int aIndex,int x,int y,int width,int height);
   333: 	BYTE ChaosBoxInsertItem(int aIndex,CItem item);
   334: 	BYTE ChaosBoxAddItem(int aIndex,CItem item,int slot);
   335: 	void ChaosBoxDelItem(int aIndex,int slot);
   336: 	void ItemByteConvert(BYTE* lpMsg,CItem item);
   337: 	void DBItemByteConvert(BYTE* lpMsg,CItem* lpItem);
   338: 	bool ConvertItemByte(CItem* lpItem,BYTE* lpMsg);
   339: 	bool IsValidItem(LPOBJ lpObj,CItem* lpItem);
   340: 	void UpdateInventoryViewport(int aIndex,int slot);
   341: 	void DeleteInventoryItemCount(LPOBJ lpObj,int index,int level,int count);
   342: 	void DecreaseItemDur(LPOBJ lpObj,int slot,int dur);
   343: 	int RepairItem(LPOBJ lpObj,CItem* lpItem,int slot,int type);
   344: 	BYTE MoveItemToInventoryFromInventory(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
   345: 	BYTE MoveItemToInventoryFromTrade(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
   346: 	BYTE MoveItemToInventoryFromWarehouse(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
   347: 	BYTE MoveItemToInventoryFromChaosBox(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
   348: 	BYTE MoveItemToInventoryFromPersonalShop(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
   349: 	BYTE MoveItemToTradeFromInventory(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
   350: 	BYTE MoveItemToTradeFromTrade(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
   351: 	BYTE MoveItemToTradeFromEventInventory(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
   352: 	BYTE MoveItemToWarehouseFromInventory(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
   353: 	BYTE MoveItemToWarehouseFromWarehouse(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
   354: 	BYTE MoveItemToChaosBoxFromInventory(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
   355: 	BYTE MoveItemToChaosBoxFromChaosBox(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
   356: 	BYTE MoveItemToPersonalShopFromInventory(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
   357: 	BYTE MoveItemToPersonalShopFromPersonalShop(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
   358: 	BYTE MoveItemToEventInventoryFromTrade(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
   359: 	BYTE MoveItemToEventInventoryFromEventInventory(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
   360: 	BYTE MoveItemToMuunInventoryFromMuunInventory(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
   361: 	void CGItemGetRecv(PMSG_ITEM_GET_RECV* lpMsg,int aIndex);
   362: 	void CGItemDropRecv(PMSG_ITEM_DROP_RECV* lpMsg,int aIndex);
   363: 	bool CGPkDrop(PMSG_ITEM_DROP_RECV* lpMsg,int aIndex);
   364: 	void CGItemMoveRecv(PMSG_ITEM_MOVE_RECV* lpMsg,int aIndex);
   365: 	void CGItemUseRecv(PMSG_ITEM_USE_RECV* lpMsg,int aIndex);
   366: 	void CGItemBuyRecv(PMSG_ITEM_BUY_RECV* lpMsg,int aIndex, int ok);
   367: 	void CGItemSellRecv(PMSG_ITEM_SELL_RECV* lpMsg,int aIndex);
   368: 	void CGItemRepairRecv(PMSG_ITEM_REPAIR_RECV* lpMsg,int aIndex);
   369: 	void GCItemMoveSend(int aIndex,BYTE result,BYTE slot,BYTE* ItemInfo);
   370: 	void GCItemChangeSend(int aIndex,BYTE slot);
   371: 	void GCItemDeleteSend(int aIndex,BYTE slot,BYTE flag);
   372: 	void GCItemDurSend(int aIndex,BYTE slot,BYTE dur,BYTE flag);
   373: 	void GCItemListSend(int aIndex);
   374: 	void GCItemEquipmentSend(int aIndex);
   375: 	void GCItemModifySend(int aIndex,BYTE slot);
   376: 	char* GetItemName(int index);
   377: 	void CGItemBuyConfirmRecv(PMSG_ITEM_BUY_NEW* lpMsg,int aIndex);
   378: private:
   379: 	std::map<int,ITEM_INFO> m_ItemInfo;
   380: };
   381: 
   382: extern  ITEM_ATTRIBUTE ItemAttribute[MAX_ITEM];//MC
   383: extern int g_MaxItemIndexOfEachItemType[MAX_ITEM_TYPE];//MC
   384: extern CItemManager gItemManager;
```


---

## ItemManager.cpp — token `\bGCItemListSend\s*\(`

- first hit @ line 4640
```
  4560: 		return;
  4561: 	}
  4562: 
  4563: 	pMsg.money = this->RepairItem(lpObj,&lpObj->Inventory[lpMsg->slot],lpMsg->slot,lpMsg->type);
  4564: 
  4565: 	if(pMsg.money != 0)
  4566: 	{
  4567: 		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
  4568: 		gObjectManager.CharacterCalcAttribute(aIndex);
  4569: 	}
  4570: }
  4571: 
  4572: void CItemManager::GCItemMoveSend(int aIndex,BYTE result,BYTE slot,BYTE* ItemInfo) // OK
  4573: {
  4574: 	PMSG_ITEM_MOVE_SEND pMsg;
  4575: 
  4576: 	pMsg.header.set(0x24,sizeof(pMsg));
  4577: 
  4578: 	pMsg.result = result;
  4579: 
  4580: 	pMsg.slot = slot;
  4581: 
  4582: 	memcpy(pMsg.ItemInfo,ItemInfo,MAX_ITEM_INFO);
  4583: 
  4584: 	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
  4585: }
  4586: 
  4587: void CItemManager::GCItemChangeSend(int aIndex,BYTE slot) // OK
  4588: {
  4589: 	LPOBJ lpObj = &gObj[aIndex];
  4590: 
  4591: 	PMSG_ITEM_CHANGE_SEND pMsg;
  4592: 
  4593: 	pMsg.header.set(0x25,sizeof(pMsg));
  4594: 
  4595: 	pMsg.index[0] = SET_NUMBERHB(aIndex);
  4596: 	pMsg.index[1] = SET_NUMBERLB(aIndex);
  4597: 
  4598: 	this->ItemByteConvert(pMsg.ItemInfo,lpObj->Inventory[slot]);
  4599: 
  4600: 	pMsg.ItemInfo[1] = slot*16;
  4601: 	pMsg.ItemInfo[1] |= ((lpObj->Inventory[slot].m_Level-1)/2) & 0x0F;
  4602: 
  4603: 	#if(GAMESERVER_UPDATE>=701)
  4604: 
  4605: 	pMsg.attribute = lpObj->ElementalAttribute;
  4606: 
  4607: 	#endif
  4608: 
  4609: 	MsgSendV2(lpObj,(BYTE*)&pMsg,pMsg.header.size);
  4610: }
  4611: 
  4612: void CItemManager::GCItemDeleteSend(int aIndex,BYTE slot,BYTE flag) // OK
  4613: {
  4614: 	PMSG_ITEM_DELETE_SEND pMsg;
  4615: 
  4616: 	pMsg.header.set(0x28,sizeof(pMsg));
  4617: 
  4618: 	pMsg.slot = slot;
  4619: 
  4620: 	pMsg.flag = flag;
  4621: 
  4622: 	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
  4623: }
  4624: 
  4625: void CItemManager::GCItemDurSend(int aIndex,BYTE slot,BYTE dur,BYTE flag) // OK
  4626: {
  4627: 	PMSG_ITEM_DUR_SEND pMsg;
  4628: 
  4629: 	pMsg.header.set(0x2A,sizeof(pMsg));
  4630: 
  4631: 	pMsg.slot = slot;
  4632: 
  4633: 	pMsg.dur = dur;
  4634: 
  4635: 	pMsg.flag = flag;
  4636: 
  4637: 	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
  4638: }
  4639: 
  4640: void CItemManager::GCItemListSend(int aIndex) // OK
  4641: {
  4642: 	LPOBJ lpObj = &gObj[aIndex];
  4643: 
  4644: 	BYTE send[4096];
  4645: 
  4646: 	PMSG_ITEM_LIST_SEND pMsg;
  4647: 
  4648: 	pMsg.header.set(0xF3,0x10,0);
  4649: 
  4650: 	int size = sizeof(pMsg);
  4651: 
  4652: 	pMsg.count = 0;
  4653: 
  4654: 	PMSG_ITEM_LIST info;
  4655: 
  4656: 	for(int n=0;n < INVENTORY_SIZE;n++)
  4657: 	{
  4658: 		if(lpObj->Inventory[n].IsItem() != 0)
  4659: 		{
  4660: 			if(lpObj->Inventory[n].m_ItemExist != 0)
  4661: 			{
  4662: 				info.slot = n;
  4663: 
  4664: 				this->ItemByteConvert(info.ItemInfo,lpObj->Inventory[n]);
  4665: 
  4666: 				memcpy(&send[size],&info,sizeof(info));
  4667: 				size += sizeof(info);
  4668: 
  4669: 				pMsg.count++;
  4670: 			}
  4671: 		}
  4672: 	}
  4673: 
  4674: 	pMsg.header.size[0] = SET_NUMBERHB(size);
  4675: 	pMsg.header.size[1] = SET_NUMBERLB(size);
  4676: 
  4677: 	memcpy(send,&pMsg,sizeof(pMsg));
  4678: 
  4679: 	DataSend(aIndex,send,size);
  4680: }
  4681: 
  4682: void CItemManager::GCItemEquipmentSend(int aIndex) // OK
  4683: {
  4684: 	PMSG_ITEM_EQUIPMENT_SEND pMsg;
  4685: 
  4686: 	pMsg.header.set(0xF3,0x13,sizeof(pMsg));
  4687: 
  4688: 	pMsg.index[0] = SET_NUMBERHB(aIndex);
  4689: 
  4690: 	pMsg.index[1] = SET_NUMBERLB(aIndex);
  4691: 
  4692: 	memcpy(pMsg.CharSet,gObj[aIndex].CharSet,sizeof(pMsg.CharSet));
  4693: 
  4694: 	MsgSendV2(&gObj[aIndex],(BYTE*)&pMsg,pMsg.header.size);
  4695: }
  4696: 
  4697: void CItemManager::GCItemModifySend(int aIndex,BYTE slot) // OK
  4698: {
  4699: 	LPOBJ lpObj = &gObj[aIndex];
  4700: 
  4701: 	if(lpObj->Inventory[slot].IsItem() == 0)
  4702: 	{
  4703: 		return;
  4704: 	}
  4705: 
  4706: 	PMSG_ITEM_MODIFY_SEND pMsg;
  4707: 
  4708: 	pMsg.header.set(0xF3,0x14,sizeof(pMsg));
  4709: 
  4710: 	pMsg.slot = slot;
  4711: 
  4712: 	this->ItemByteConvert(pMsg.ItemInfo,lpObj->Inventory[slot]);
  4713: 
  4714: 	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
  4715: }
  4716: 
  4717: void CItemManager::CGItemBuyConfirmRecv(PMSG_ITEM_BUY_NEW* lpMsg,int aIndex) // OK
  4718: {
  4719: 	PMSG_ITEM_BUY_RECV pMsg;
  4720: 
  4721: 	pMsg.header.set(0x32,sizeof(pMsg));
  4722: 
  4723: 	pMsg.slot = lpMsg->slot;
  4724: 
  4725: 	CGItemBuyRecv(&pMsg,aIndex,1);
  4726: }
```

## ItemManager.cpp — token `header\.set\s*\(\s*0xF3\s*,\s*0x10`

- first hit @ line 4648
```
  4568: 		gObjectManager.CharacterCalcAttribute(aIndex);
  4569: 	}
  4570: }
  4571: 
  4572: void CItemManager::GCItemMoveSend(int aIndex,BYTE result,BYTE slot,BYTE* ItemInfo) // OK
  4573: {
  4574: 	PMSG_ITEM_MOVE_SEND pMsg;
  4575: 
  4576: 	pMsg.header.set(0x24,sizeof(pMsg));
  4577: 
  4578: 	pMsg.result = result;
  4579: 
  4580: 	pMsg.slot = slot;
  4581: 
  4582: 	memcpy(pMsg.ItemInfo,ItemInfo,MAX_ITEM_INFO);
  4583: 
  4584: 	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
  4585: }
  4586: 
  4587: void CItemManager::GCItemChangeSend(int aIndex,BYTE slot) // OK
  4588: {
  4589: 	LPOBJ lpObj = &gObj[aIndex];
  4590: 
  4591: 	PMSG_ITEM_CHANGE_SEND pMsg;
  4592: 
  4593: 	pMsg.header.set(0x25,sizeof(pMsg));
  4594: 
  4595: 	pMsg.index[0] = SET_NUMBERHB(aIndex);
  4596: 	pMsg.index[1] = SET_NUMBERLB(aIndex);
  4597: 
  4598: 	this->ItemByteConvert(pMsg.ItemInfo,lpObj->Inventory[slot]);
  4599: 
  4600: 	pMsg.ItemInfo[1] = slot*16;
  4601: 	pMsg.ItemInfo[1] |= ((lpObj->Inventory[slot].m_Level-1)/2) & 0x0F;
  4602: 
  4603: 	#if(GAMESERVER_UPDATE>=701)
  4604: 
  4605: 	pMsg.attribute = lpObj->ElementalAttribute;
  4606: 
  4607: 	#endif
  4608: 
  4609: 	MsgSendV2(lpObj,(BYTE*)&pMsg,pMsg.header.size);
  4610: }
  4611: 
  4612: void CItemManager::GCItemDeleteSend(int aIndex,BYTE slot,BYTE flag) // OK
  4613: {
  4614: 	PMSG_ITEM_DELETE_SEND pMsg;
  4615: 
  4616: 	pMsg.header.set(0x28,sizeof(pMsg));
  4617: 
  4618: 	pMsg.slot = slot;
  4619: 
  4620: 	pMsg.flag = flag;
  4621: 
  4622: 	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
  4623: }
  4624: 
  4625: void CItemManager::GCItemDurSend(int aIndex,BYTE slot,BYTE dur,BYTE flag) // OK
  4626: {
  4627: 	PMSG_ITEM_DUR_SEND pMsg;
  4628: 
  4629: 	pMsg.header.set(0x2A,sizeof(pMsg));
  4630: 
  4631: 	pMsg.slot = slot;
  4632: 
  4633: 	pMsg.dur = dur;
  4634: 
  4635: 	pMsg.flag = flag;
  4636: 
  4637: 	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
  4638: }
  4639: 
  4640: void CItemManager::GCItemListSend(int aIndex) // OK
  4641: {
  4642: 	LPOBJ lpObj = &gObj[aIndex];
  4643: 
  4644: 	BYTE send[4096];
  4645: 
  4646: 	PMSG_ITEM_LIST_SEND pMsg;
  4647: 
  4648: 	pMsg.header.set(0xF3,0x10,0);
  4649: 
  4650: 	int size = sizeof(pMsg);
  4651: 
  4652: 	pMsg.count = 0;
  4653: 
  4654: 	PMSG_ITEM_LIST info;
  4655: 
  4656: 	for(int n=0;n < INVENTORY_SIZE;n++)
  4657: 	{
  4658: 		if(lpObj->Inventory[n].IsItem() != 0)
  4659: 		{
  4660: 			if(lpObj->Inventory[n].m_ItemExist != 0)
  4661: 			{
  4662: 				info.slot = n;
  4663: 
  4664: 				this->ItemByteConvert(info.ItemInfo,lpObj->Inventory[n]);
  4665: 
  4666: 				memcpy(&send[size],&info,sizeof(info));
  4667: 				size += sizeof(info);
  4668: 
  4669: 				pMsg.count++;
  4670: 			}
  4671: 		}
  4672: 	}
  4673: 
  4674: 	pMsg.header.size[0] = SET_NUMBERHB(size);
  4675: 	pMsg.header.size[1] = SET_NUMBERLB(size);
  4676: 
  4677: 	memcpy(send,&pMsg,sizeof(pMsg));
  4678: 
  4679: 	DataSend(aIndex,send,size);
  4680: }
  4681: 
  4682: void CItemManager::GCItemEquipmentSend(int aIndex) // OK
  4683: {
  4684: 	PMSG_ITEM_EQUIPMENT_SEND pMsg;
  4685: 
  4686: 	pMsg.header.set(0xF3,0x13,sizeof(pMsg));
  4687: 
  4688: 	pMsg.index[0] = SET_NUMBERHB(aIndex);
  4689: 
  4690: 	pMsg.index[1] = SET_NUMBERLB(aIndex);
  4691: 
  4692: 	memcpy(pMsg.CharSet,gObj[aIndex].CharSet,sizeof(pMsg.CharSet));
  4693: 
  4694: 	MsgSendV2(&gObj[aIndex],(BYTE*)&pMsg,pMsg.header.size);
  4695: }
  4696: 
  4697: void CItemManager::GCItemModifySend(int aIndex,BYTE slot) // OK
  4698: {
  4699: 	LPOBJ lpObj = &gObj[aIndex];
  4700: 
  4701: 	if(lpObj->Inventory[slot].IsItem() == 0)
  4702: 	{
  4703: 		return;
  4704: 	}
  4705: 
  4706: 	PMSG_ITEM_MODIFY_SEND pMsg;
  4707: 
  4708: 	pMsg.header.set(0xF3,0x14,sizeof(pMsg));
  4709: 
  4710: 	pMsg.slot = slot;
  4711: 
  4712: 	this->ItemByteConvert(pMsg.ItemInfo,lpObj->Inventory[slot]);
  4713: 
  4714: 	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
  4715: }
  4716: 
  4717: void CItemManager::CGItemBuyConfirmRecv(PMSG_ITEM_BUY_NEW* lpMsg,int aIndex) // OK
  4718: {
  4719: 	PMSG_ITEM_BUY_RECV pMsg;
  4720: 
  4721: 	pMsg.header.set(0x32,sizeof(pMsg));
  4722: 
  4723: 	pMsg.slot = lpMsg->slot;
  4724: 
  4725: 	CGItemBuyRecv(&pMsg,aIndex,1);
  4726: }
```

## ItemManager.cpp — token `size\s*\[\s*0\s*\]\s*=`

- first hit @ line 4674
```
  4594: 
  4595: 	pMsg.index[0] = SET_NUMBERHB(aIndex);
  4596: 	pMsg.index[1] = SET_NUMBERLB(aIndex);
  4597: 
  4598: 	this->ItemByteConvert(pMsg.ItemInfo,lpObj->Inventory[slot]);
  4599: 
  4600: 	pMsg.ItemInfo[1] = slot*16;
  4601: 	pMsg.ItemInfo[1] |= ((lpObj->Inventory[slot].m_Level-1)/2) & 0x0F;
  4602: 
  4603: 	#if(GAMESERVER_UPDATE>=701)
  4604: 
  4605: 	pMsg.attribute = lpObj->ElementalAttribute;
  4606: 
  4607: 	#endif
  4608: 
  4609: 	MsgSendV2(lpObj,(BYTE*)&pMsg,pMsg.header.size);
  4610: }
  4611: 
  4612: void CItemManager::GCItemDeleteSend(int aIndex,BYTE slot,BYTE flag) // OK
  4613: {
  4614: 	PMSG_ITEM_DELETE_SEND pMsg;
  4615: 
  4616: 	pMsg.header.set(0x28,sizeof(pMsg));
  4617: 
  4618: 	pMsg.slot = slot;
  4619: 
  4620: 	pMsg.flag = flag;
  4621: 
  4622: 	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
  4623: }
  4624: 
  4625: void CItemManager::GCItemDurSend(int aIndex,BYTE slot,BYTE dur,BYTE flag) // OK
  4626: {
  4627: 	PMSG_ITEM_DUR_SEND pMsg;
  4628: 
  4629: 	pMsg.header.set(0x2A,sizeof(pMsg));
  4630: 
  4631: 	pMsg.slot = slot;
  4632: 
  4633: 	pMsg.dur = dur;
  4634: 
  4635: 	pMsg.flag = flag;
  4636: 
  4637: 	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
  4638: }
  4639: 
  4640: void CItemManager::GCItemListSend(int aIndex) // OK
  4641: {
  4642: 	LPOBJ lpObj = &gObj[aIndex];
  4643: 
  4644: 	BYTE send[4096];
  4645: 
  4646: 	PMSG_ITEM_LIST_SEND pMsg;
  4647: 
  4648: 	pMsg.header.set(0xF3,0x10,0);
  4649: 
  4650: 	int size = sizeof(pMsg);
  4651: 
  4652: 	pMsg.count = 0;
  4653: 
  4654: 	PMSG_ITEM_LIST info;
  4655: 
  4656: 	for(int n=0;n < INVENTORY_SIZE;n++)
  4657: 	{
  4658: 		if(lpObj->Inventory[n].IsItem() != 0)
  4659: 		{
  4660: 			if(lpObj->Inventory[n].m_ItemExist != 0)
  4661: 			{
  4662: 				info.slot = n;
  4663: 
  4664: 				this->ItemByteConvert(info.ItemInfo,lpObj->Inventory[n]);
  4665: 
  4666: 				memcpy(&send[size],&info,sizeof(info));
  4667: 				size += sizeof(info);
  4668: 
  4669: 				pMsg.count++;
  4670: 			}
  4671: 		}
  4672: 	}
  4673: 
  4674: 	pMsg.header.size[0] = SET_NUMBERHB(size);
  4675: 	pMsg.header.size[1] = SET_NUMBERLB(size);
  4676: 
  4677: 	memcpy(send,&pMsg,sizeof(pMsg));
  4678: 
  4679: 	DataSend(aIndex,send,size);
  4680: }
  4681: 
  4682: void CItemManager::GCItemEquipmentSend(int aIndex) // OK
  4683: {
  4684: 	PMSG_ITEM_EQUIPMENT_SEND pMsg;
  4685: 
  4686: 	pMsg.header.set(0xF3,0x13,sizeof(pMsg));
  4687: 
  4688: 	pMsg.index[0] = SET_NUMBERHB(aIndex);
  4689: 
  4690: 	pMsg.index[1] = SET_NUMBERLB(aIndex);
  4691: 
  4692: 	memcpy(pMsg.CharSet,gObj[aIndex].CharSet,sizeof(pMsg.CharSet));
  4693: 
  4694: 	MsgSendV2(&gObj[aIndex],(BYTE*)&pMsg,pMsg.header.size);
  4695: }
  4696: 
  4697: void CItemManager::GCItemModifySend(int aIndex,BYTE slot) // OK
  4698: {
  4699: 	LPOBJ lpObj = &gObj[aIndex];
  4700: 
  4701: 	if(lpObj->Inventory[slot].IsItem() == 0)
  4702: 	{
  4703: 		return;
  4704: 	}
  4705: 
  4706: 	PMSG_ITEM_MODIFY_SEND pMsg;
  4707: 
  4708: 	pMsg.header.set(0xF3,0x14,sizeof(pMsg));
  4709: 
  4710: 	pMsg.slot = slot;
  4711: 
  4712: 	this->ItemByteConvert(pMsg.ItemInfo,lpObj->Inventory[slot]);
  4713: 
  4714: 	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
  4715: }
  4716: 
  4717: void CItemManager::CGItemBuyConfirmRecv(PMSG_ITEM_BUY_NEW* lpMsg,int aIndex) // OK
  4718: {
  4719: 	PMSG_ITEM_BUY_RECV pMsg;
  4720: 
  4721: 	pMsg.header.set(0x32,sizeof(pMsg));
  4722: 
  4723: 	pMsg.slot = lpMsg->slot;
  4724: 
  4725: 	CGItemBuyRecv(&pMsg,aIndex,1);
  4726: }
```

## ItemManager.cpp — token `size\s*\[\s*1\s*\]\s*=`

- first hit @ line 4675
```
  4595: 	pMsg.index[0] = SET_NUMBERHB(aIndex);
  4596: 	pMsg.index[1] = SET_NUMBERLB(aIndex);
  4597: 
  4598: 	this->ItemByteConvert(pMsg.ItemInfo,lpObj->Inventory[slot]);
  4599: 
  4600: 	pMsg.ItemInfo[1] = slot*16;
  4601: 	pMsg.ItemInfo[1] |= ((lpObj->Inventory[slot].m_Level-1)/2) & 0x0F;
  4602: 
  4603: 	#if(GAMESERVER_UPDATE>=701)
  4604: 
  4605: 	pMsg.attribute = lpObj->ElementalAttribute;
  4606: 
  4607: 	#endif
  4608: 
  4609: 	MsgSendV2(lpObj,(BYTE*)&pMsg,pMsg.header.size);
  4610: }
  4611: 
  4612: void CItemManager::GCItemDeleteSend(int aIndex,BYTE slot,BYTE flag) // OK
  4613: {
  4614: 	PMSG_ITEM_DELETE_SEND pMsg;
  4615: 
  4616: 	pMsg.header.set(0x28,sizeof(pMsg));
  4617: 
  4618: 	pMsg.slot = slot;
  4619: 
  4620: 	pMsg.flag = flag;
  4621: 
  4622: 	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
  4623: }
  4624: 
  4625: void CItemManager::GCItemDurSend(int aIndex,BYTE slot,BYTE dur,BYTE flag) // OK
  4626: {
  4627: 	PMSG_ITEM_DUR_SEND pMsg;
  4628: 
  4629: 	pMsg.header.set(0x2A,sizeof(pMsg));
  4630: 
  4631: 	pMsg.slot = slot;
  4632: 
  4633: 	pMsg.dur = dur;
  4634: 
  4635: 	pMsg.flag = flag;
  4636: 
  4637: 	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
  4638: }
  4639: 
  4640: void CItemManager::GCItemListSend(int aIndex) // OK
  4641: {
  4642: 	LPOBJ lpObj = &gObj[aIndex];
  4643: 
  4644: 	BYTE send[4096];
  4645: 
  4646: 	PMSG_ITEM_LIST_SEND pMsg;
  4647: 
  4648: 	pMsg.header.set(0xF3,0x10,0);
  4649: 
  4650: 	int size = sizeof(pMsg);
  4651: 
  4652: 	pMsg.count = 0;
  4653: 
  4654: 	PMSG_ITEM_LIST info;
  4655: 
  4656: 	for(int n=0;n < INVENTORY_SIZE;n++)
  4657: 	{
  4658: 		if(lpObj->Inventory[n].IsItem() != 0)
  4659: 		{
  4660: 			if(lpObj->Inventory[n].m_ItemExist != 0)
  4661: 			{
  4662: 				info.slot = n;
  4663: 
  4664: 				this->ItemByteConvert(info.ItemInfo,lpObj->Inventory[n]);
  4665: 
  4666: 				memcpy(&send[size],&info,sizeof(info));
  4667: 				size += sizeof(info);
  4668: 
  4669: 				pMsg.count++;
  4670: 			}
  4671: 		}
  4672: 	}
  4673: 
  4674: 	pMsg.header.size[0] = SET_NUMBERHB(size);
  4675: 	pMsg.header.size[1] = SET_NUMBERLB(size);
  4676: 
  4677: 	memcpy(send,&pMsg,sizeof(pMsg));
  4678: 
  4679: 	DataSend(aIndex,send,size);
  4680: }
  4681: 
  4682: void CItemManager::GCItemEquipmentSend(int aIndex) // OK
  4683: {
  4684: 	PMSG_ITEM_EQUIPMENT_SEND pMsg;
  4685: 
  4686: 	pMsg.header.set(0xF3,0x13,sizeof(pMsg));
  4687: 
  4688: 	pMsg.index[0] = SET_NUMBERHB(aIndex);
  4689: 
  4690: 	pMsg.index[1] = SET_NUMBERLB(aIndex);
  4691: 
  4692: 	memcpy(pMsg.CharSet,gObj[aIndex].CharSet,sizeof(pMsg.CharSet));
  4693: 
  4694: 	MsgSendV2(&gObj[aIndex],(BYTE*)&pMsg,pMsg.header.size);
  4695: }
  4696: 
  4697: void CItemManager::GCItemModifySend(int aIndex,BYTE slot) // OK
  4698: {
  4699: 	LPOBJ lpObj = &gObj[aIndex];
  4700: 
  4701: 	if(lpObj->Inventory[slot].IsItem() == 0)
  4702: 	{
  4703: 		return;
  4704: 	}
  4705: 
  4706: 	PMSG_ITEM_MODIFY_SEND pMsg;
  4707: 
  4708: 	pMsg.header.set(0xF3,0x14,sizeof(pMsg));
  4709: 
  4710: 	pMsg.slot = slot;
  4711: 
  4712: 	this->ItemByteConvert(pMsg.ItemInfo,lpObj->Inventory[slot]);
  4713: 
  4714: 	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
  4715: }
  4716: 
  4717: void CItemManager::CGItemBuyConfirmRecv(PMSG_ITEM_BUY_NEW* lpMsg,int aIndex) // OK
  4718: {
  4719: 	PMSG_ITEM_BUY_RECV pMsg;
  4720: 
  4721: 	pMsg.header.set(0x32,sizeof(pMsg));
  4722: 
  4723: 	pMsg.slot = lpMsg->slot;
  4724: 
  4725: 	CGItemBuyRecv(&pMsg,aIndex,1);
  4726: }
```

## ItemManager.cpp — token `\bDataSend\s*\(`

- first hit @ line 3142
```
  3062: 	#if(GAMESERVER_UPDATE>=803)
  3063: 
  3064: 	if(MUUN_INVENTORY_RANGE(SourceSlot) == 0)
  3065: 	{
  3066: 		return 0xFF;
  3067: 	}
  3068: 
  3069: 	if(MUUN_INVENTORY_RANGE(TargetSlot) == 0)
  3070: 	{
  3071: 		return 0xFF;
  3072: 	}
  3073: 
  3074: 	if(SourceSlot == TargetSlot)
  3075: 	{
  3076: 		return 0xFF;
  3077: 	}
  3078: 
  3079: 	if(gMuunSystem.CheckItemMoveToMuunInventory(lpObj,&lpObj->MuunInventory[SourceSlot],TargetSlot) == 0)
  3080: 	{
  3081: 		return 0xFF;
  3082: 	}
  3083: 
  3084: 	BYTE map[MUUN_INVENTORY_SIZE];
  3085: 
  3086: 	memcpy(map,lpObj->MuunInventoryMap,MUUN_INVENTORY_SIZE);
  3087: 
  3088: 	gMuunSystem.MuunInventoryItemSet(lpObj->Index,SourceSlot,0xFF);
  3089: 
  3090: 	if(gMuunSystem.MuunInventoryAddItem(lpObj->Index,lpObj->MuunInventory[SourceSlot],TargetSlot) == 0xFF)
  3091: 	{
  3092: 		memcpy(lpObj->MuunInventoryMap,map,MUUN_INVENTORY_SIZE);
  3093: 		return 0xFF;
  3094: 	}
  3095: 
  3096: 	gMuunSystem.MuunInventoryDelItem(lpObj->Index,SourceSlot);
  3097: 
  3098: 	gMuunSystem.MuunInventoryItemSet(lpObj->Index,TargetSlot,1);
  3099: 
  3100: 	if(MUUN_INVENTORY_WEAR_RANGE(SourceSlot) != 0)
  3101: 	{
  3102: 		gObjectManager.CharacterMakePreviewCharSet(lpObj->Index);
  3103: 		gMuunSystem.GCMuunItemChangeSend(lpObj->Index,SourceSlot);
  3104: 		gMuunSystem.GCMuunItemStatusSend(lpObj->Index,SourceSlot,0);
  3105: 		lpObj->MuunItemStatus[SourceSlot] = 0;
  3106: 	}
  3107: 
  3108: 	if(MUUN_INVENTORY_WEAR_RANGE(TargetSlot) != 0)
  3109: 	{
  3110: 		gObjectManager.CharacterMakePreviewCharSet(lpObj->Index);
  3111: 		gMuunSystem.GCMuunItemChangeSend(lpObj->Index,TargetSlot);
  3112: 	}
  3113: 
  3114: 	return TargetFlag;
  3115: 
  3116: 	#else
  3117: 
  3118: 	return 0xFF;
  3119: 
  3120: 	#endif
  3121: }
  3122: 
  3123: void CItemManager::CGItemGetRecv(PMSG_ITEM_GET_RECV* lpMsg,int aIndex) // OK
  3124: {
  3125: 	LPOBJ lpObj = &gObj[aIndex];
  3126: 
  3127: 	if(gObjIsConnectedGP(aIndex) == 0)
  3128: 	{
  3129: 		return;
  3130: 	}
  3131: 
  3132: 	PMSG_ITEM_GET_SEND pMsg;
  3133: 
  3134: 	pMsg.header.set(0x22,sizeof(pMsg));
  3135: 
  3136: 	pMsg.result = 0xFF;
  3137: 
  3138: 	memset(pMsg.ItemInfo,0,sizeof(pMsg.ItemInfo));
  3139: 
  3140: 	if(lpObj->DieRegen != 0)
  3141: 	{
  3142: 		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
  3143: 		return;
  3144: 	}
  3145: 
  3146: 	if(lpObj->Interface.use != 0 && lpObj->Interface.type != INTERFACE_SHOP)
  3147: 	{
  3148: 		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
  3149: 		return;
  3150: 	}
  3151: 
  3152: 	#if(GAMESERVER_UPDATE>=402)
  3153: 
  3154: 	if(gDuel.GetDuelArenaBySpectator(aIndex) != 0)
  3155: 	{
  3156: 		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
  3157: 		return;
  3158: 	}
  3159: 
  3160: 	#endif
  3161: 
  3162: 	gObjFixInventoryPointer(aIndex);
  3163: 
  3164: 	if(lpObj->Transaction == 1)
  3165: 	{
  3166: 		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
  3167: 		return;
  3168: 	}
  3169: 
  3170: 	int index = MAKE_NUMBERW(lpMsg->index[0],lpMsg->index[1]);
  3171: 
  3172: 	if(MAP_ITEM_RANGE(index) == 0)
  3173: 	{
  3174: 		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
  3175: 		return;
  3176: 	}
  3177: 
  3178: 	if(MAP_RANGE(lpObj->Map) == 0)
  3179: 	{
  3180: 		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
  3181: 		return;
  3182: 	}
  3183: 
  3184: 	if(gMap[lpObj->Map].CheckItemGive(aIndex,index) == 0)
  3185: 	{
  3186: 		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
  3187: 		return;
  3188: 	}
  3189: 
  3190: 	CMapItem* lpItem = &gMap[lpObj->Map].m_Item[index];
  3191: 
  3192: 	if(lpItem->IsEventItem() != 0)
  3193: 	{
  3194: 		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
  3195: 		return;
  3196: 	}
  3197: 
  3198: 	if(lpItem->IsMuunItem() != 0 || lpItem->IsMuunUtil() != 0)
  3199: 	{
  3200: 		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
  3201: 		return;
  3202: 	}
  3203: 
  3204: 	if(gQuestObjective.CheckQuestObjectiveItemCount(lpObj,lpItem->m_Index,lpItem->m_Level) == 0)
  3205: 	{
  3206: 		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
  3207: 		return;
  3208: 	}
  3209: 
  3210: 	if((lpItem->m_Index == GET_ITEM(13,20) && lpItem->m_Level == 0) || lpItem->m_Index == GET_ITEM(13,38)) // Rings
  3211: 	{
  3212: 		if(this->GetInventoryItemCount(lpObj,lpItem->m_Index,lpItem->m_Level) > 0)
  3213: 		{
  3214: 			DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
  3215: 			return;
  3216: 		}
  3217: 	}
  3218: 
  3219: 	if(lpItem->m_Index == GET_ITEM(14,15)) // Money
  3220: 	{
  3221: 		gMap[lpObj->Map].ItemGive(aIndex,index);
  3222: 
  3223: 		if(gObjCheckMaxMoney(aIndex,lpItem->m_BuyMoney) == 0)
  3224: 		{
  3225: 			lpObj->Money = MAX_MONEY;
  3226: 		}
  3227: 		else
  3228: 		{
  3229: 			lpObj->Money += lpItem->m_BuyMoney;
  3230: 		}
  3231: 
  3232: 		pMsg.result = 0xFE;
  3233: 
  3234: 		pMsg.ItemInfo[0] = SET_NUMBERHB(SET_NUMBERHW(lpObj->Money));
  3235: 		pMsg.ItemInfo[1] = SET_NUMBERLB(SET_NUMBERHW(lpObj->Money));
  3236: 		pMsg.ItemInfo[2] = SET_NUMBERHB(SET_NUMBERLW(lpObj->Money));
  3237: 		pMsg.ItemInfo[3] = SET_NUMBERLB(SET_NUMBERLW(lpObj->Money));
  3238: 
  3239: 		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
  3240: 		return;
  3241: 	}
  3242: 
  3243: 	CItem item = (*lpItem);
  3244: 
  3245: 	if(this->InventoryInsertItemStack(lpObj,&item) != 0)
  3246: 	{
  3247: 		gMap[lpObj->Map].ItemGive(aIndex,index);
  3248: 
  3249: 		pMsg.result = 0xFD;
  3250: 		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
  3251: 		return;
  3252: 	}
  3253: 
  3254: 	pMsg.result = this->InventoryInsertItem(aIndex,item);
  3255: 
  3256: 	if(pMsg.result == 0xFF)
  3257: 	{
  3258: 		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
  3259: 		return;
  3260: 	}
  3261: 
  3262: 	gMap[lpObj->Map].ItemGive(aIndex,index);
  3263: 
  3264: 	this->ItemByteConvert(pMsg.ItemInfo,item);
  3265: 
  3266: 	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
  3267: 
  3268: 	GCPartyItemInfoSend(aIndex,&item);
  3269: 
  3270: 	if(item.m_IsPeriodicItem != 0)
  3271: 	{
  3272: 		gCashShop.GCCashShopPeriodicItemSend(aIndex,item.m_Index,pMsg.result,item.m_PeriodicItemTime);
  3273: 	}
  3274: 
  3275: 	if(BC_MAP_RANGE(lpObj->Map) != 0)
  3276: 	{
  3277: 		if(gBloodCastle.CheckEventItemSerial(lpObj->Map,lpItem) != 0)
  3278: 		{
  3279: 			gBloodCastle.GetEventItem(lpObj->Map,aIndex,lpItem);
  3280: 		}
  3281: 	}
  3282: 
  3283: 	if(IT_MAP_RANGE(lpObj->Map) != 0)
  3284: 	{
  3285: 		if(gIllusionTemple.CheckEventItemSerial(lpObj->Map,lpItem) != 0)
  3286: 		{
  3287: 			gIllusionTemple.GetEventItem(lpObj->Map,aIndex,lpItem);
  3288: 		}
  3289: 	}
  3290: }
  3291: 
  3292: void CItemManager::CGItemDropRecv(PMSG_ITEM_DROP_RECV* lpMsg,int aIndex) // OK
  3293: {
  3294: 	LPOBJ lpObj = &gObj[aIndex];
  3295: 
  3296: 	if(gObjIsConnectedGP(aIndex) == 0)
  3297: 	{
  3298: 		return;
  3299: 	}
  3300: 
  3301: 	PMSG_ITEM_DROP_SEND pMsg;
  3302: 
  3303: 	pMsg.header.set(0x23,sizeof(pMsg));
  3304: 
  3305: 	pMsg.result = 0;
  3306: 
  3307: 	pMsg.slot = lpMsg->slot;
  3308: 
  3309: 	if(lpObj->DieRegen != 0)
  3310: 	{
  3311: 		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
  3312: 		return;
  3313: 	}
  3314: 
  3315: 	if(lpObj->Interface.use != 0 && lpObj->Interface.type != INTERFACE_SHOP)
  3316: 	{
  3317: 		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
  3318: 		return;
  3319: 	}
  3320: 
  3321: 	#if(GAMESERVER_UPDATE>=402)
  3322: 
  3323: 	if(gDuel.GetDuelArenaBySpectator(aIndex) != 0)
  3324: 	{
  3325: 		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
  3326: 		return;
  3327: 	}
  3328: 
  3329: 	#endif
  3330: 
  3331: 	gObjFixInventoryPointer(aIndex);
  3332: 
  3333: 	if(lpObj->Transaction == 1)
  3334: 	{
  3335: 		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
  3336: 		return;
  3337: 	}
  3338: 
  3339: 	if(INVENTORY_FULL_RANGE(lpMsg->slot) == 0)
  3340: 	{
  3341: 		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
  3342: 		return;
  3343: 	}
  3344: 
  3345: 	CItem* lpItem = &lpObj->Inventory[lpMsg->slot];
  3346: 
  3347: 	if(lpItem->IsItem() == 0)
  3348: 	{
  3349: 		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
  3350: 		return;
  3351: 	}
  3352: 
  3353: 	if(lpItem->IsLuckyItem() != 0)
  3354: 	{
  3355: 		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
  3356: 		return;
  3357: 	}
  3358: 
  3359: 	if(lpItem->m_IsPeriodicItem != 0)
  3360: 	{
  3361: 		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
  3362: 		return;
  3363: 	}
  3364: 
  3365: 	//Item Lock
  3366: 	if(lpObj->Lock > 0)
  3367: 	{
  3368: 		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,2,0,gMessage.GetMessage(778));
  3369: 		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
  3370: 		return;
  3371: 	}
  3372: 
  3373: 	if(gItemMove.CheckItemMoveAllowDrop(lpItem->m_Index) == 0)
  3374: 	{
  3375: 		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
  3376: 		return;
  3377: 	}
  3378: 
  3379: 	if((lpItem->m_Index < GET_ITEM(12,0) && lpItem->m_Level > 4) || lpItem->IsExcItem() != 0 || lpItem->IsSetItem() != 0 || lpItem->IsJewelOfHarmonyItem() != 0)
  3380: 	{
  3381: 		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
  3382: 		return;
  3383: 	}
  3384: 
  3385: 	if(gItemBagManager.DropItemByItemIndex(lpItem->m_Index,lpItem->m_Level,lpObj,lpObj->Map,lpMsg->x,lpMsg->y) != 0)
  3386: 	{
  3387: 		this->InventoryDelItem(aIndex,lpMsg->slot);
  3388: 	}
  3389: 	else if(lpItem->m_Index == GET_ITEM(13,7) && (lpItem->m_Level == 0 || lpItem->m_Level == 1)) // Siege Summon
  3390: 	{
  3391: 		#if(GAMESERVER_TYPE==1)
  3392: 
  3393: 		if(gMercenary.CreateMercenary(aIndex,286+lpItem->m_Level,lpMsg->x,lpMsg->y) == 0)
  3394: 		{
  3395: 			DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
  3396: 			return;
  3397: 		}
  3398: 
  3399: 		this->InventoryDelItem(aIndex,lpMsg->slot);
  3400: 
  3401: 		#endif
  3402: 	}
  3403: 	else if(lpItem->m_Index == GET_ITEM(13,11) && lpItem->m_Level == 1) // Life Stone
  3404: 	{
  3405: 		#if(GAMESERVER_TYPE==1)
  3406: 
  3407: 		if(gLifeStone.CreateLifeStone(aIndex) == 0)
  3408: 		{
  3409: 			DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
  3410: 			return;
  3411: 		}
  3412: 
  3413: 		this->InventoryDelItem(aIndex,lpMsg->slot);
  3414: 
  3415: 		#endif
  3416: 	}
  3417: 	else if(lpItem->m_Index == GET_ITEM(13,20) && lpItem->m_Level == 1) // Starter Ring 40
  3418: 	{
  3419: 		if(lpObj->Level < 40)
  3420: 		{
  3421: 			gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,gMessage.GetMessage(257));
  3422: 			DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
  3423: 			return;
  3424: 		}
  3425: 
  3426: 		this->InventoryDelItem(aIndex,lpMsg->slot);
  3427: 		gItemBagManager.DropItemBySpecialValue(ITEM_BAG_STARTER_RING1,lpObj,lpObj->Map,lpMsg->x,lpMsg->y);
  3428: 	}
  3429: 	else if(lpItem->m_Index == GET_ITEM(13,20) && lpItem->m_Level == 2) // Starter Ring 80
  3430: 	{
  3431: 		if(lpObj->Level < 80)
  3432: 		{
  3433: 			gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,gMessage.GetMessage(258));
  3434: 			DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
  3435: 			return;
  3436: 		}
  3437: 
  3438: 		this->InventoryDelItem(aIndex,lpMsg->slot);
  3439: 		gItemBagManager.DropItemBySpecialValue(ITEM_BAG_STARTER_RING2,lpObj,lpObj->Map,lpMsg->x,lpMsg->y);
  3440: 	}
  3441: 	else if(lpItem->m_Index == GET_ITEM(14,28)) // Lost Map
  3442: 	{
  3443: 		if(gKalima.CreateKalimaGate(aIndex,lpItem->m_Level,lpMsg->x,lpMsg->y) == 0)
  3444: 		{
  3445: 			DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
  3446: 			return;
  3447: 		}
  3448: 
  3449: 		this->InventoryDelItem(aIndex,lpMsg->slot);
  3450: 	}
  3451: 	else if(lpItem->m_Index == GET_ITEM(14,63)) // Width Gruel
  3452: 	{
  3453: 		GCFireworksSend(lpObj,lpMsg->x,lpMsg->y);
  3454: 		this->InventoryDelItem(aIndex,lpMsg->slot);
  3455: 	}
  3456: 	else if(lpItem->m_Index == GET_ITEM(14,99)) // Christmas Firecracker
  3457: 	{
  3458: 		GCFireworksSend(lpObj,lpMsg->x,lpMsg->y);
  3459: 		this->InventoryDelItem(aIndex,lpMsg->slot);
  3460: 	}
  3461: 	else if(gSummonScroll.CheckSummonScroll(lpItem->m_Index) != 0)
  3462: 	{
```


---

## QUESTIONS (responder como CONFIRMED/PROBABLE com âncora)

1) O header do F3:10 começa com C2 ou C4? (qual byte é setado em PSWMSG_HEAD::set)

2) Em que offsets ficam: size[2], head (0xF3), sub (0x10), count?

3) MAX_ITEM_INFO é 7? (confirma item on-wire 7 bytes)

4) O emissor usa buffer variável (append de entries) e size BE 16-bit? (como no código)
