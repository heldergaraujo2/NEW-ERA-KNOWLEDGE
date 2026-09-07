# NEW-ERA 1.3-U2 — FULLSCAN GS: existe emissor F3:03 JoinMapServer?

- Commit pin: `580472e0d5723f9709cbad594f233deb07f9f351`

- Dir API: `https://api.github.com/repos/wongddd/muonline/contents/Source%20Server/GameServer/GameServer?ref=580472e0d5723f9709cbad594f233deb07f9f351`

- Files scanned (.cpp/.h in dir): 506

- Files with hits: 1


---

## HITS

### `Source Server/GameServer/GameServer/Protocol.h`
- url: https://raw.githubusercontent.com/wongddd/muonline/580472e0d5723f9709cbad594f233deb07f9f351/Source%20Server/GameServer/GameServer/Protocol.h
- bytes: 36305
- sha256: `943c59b7a457eccea4953b7410fd2140c6299979782b58ca81a7b42a52d6c3b0`

#### HIT: F3:03 textual (first @ line 350)
```
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
   468: 
   469: struct PMSG_DAMAGE_SEND
   470: {
   471: 	PBMSG_HEAD header; // C1:[PROTOCOL_CODE2]
   472: 	BYTE index[2];
   473: 	BYTE damage[2];
   474: 	#if(GAMESERVER_UPDATE>=701)
   475: 	BYTE type[2];
   476: 	#else
   477: 	BYTE type;
   478: 	#endif
   479: 	BYTE ShieldDamage[2];
   480: 	#if(GAMESERVER_UPDATE>=701)
   481: 	BYTE attribute;
   482: 	#endif
   483: 	#if(GAMESERVER_EXTRA==1)
   484: 	DWORD ViewCurHP;
   485: 	DWORD ViewCurSD;
   486: 	DWORD ViewDamageHP;
   487: 	DWORD ViewDamageSD;
   488: 	#endif
   489: };
   490: 
   491: struct PMSG_POSITION_SEND
   492: {
   493: #if(NEW_PROTOCOL_SYSTEM==0)
   494: 	PBMSG_HEAD header; // C1:[PROTOCOL_CODE3]
   495: #endif
   496: 	BYTE index[2];
   497: 	BYTE x;
   498: 	BYTE y;
   499: };
   500: 
   501: struct PMSG_MONSTER_DIE_SEND
   502: {
   503: 	PBMSG_HEAD header; // C1:16
   504: 	BYTE index[2];
   505: 	BYTE experience[2];
   506: 	BYTE damage[2];
   507: 	#if(GAMESERVER_EXTRA==1)
   508: 	DWORD ViewDamageHP;
   509: 	#endif
   510: };
   511: 
   512: struct PMSG_USER_DIE_SEND
   513: {
   514: 	PBMSG_HEAD header; // C1:17
   515: 	BYTE index[2];
   516: 	BYTE skill[2];
   517: 	BYTE killer[2];
   518: };
   519: 
   520: struct PMSG_ACTION_SEND
   521: {
   522: 	PBMSG_HEAD header; // C1:18
   523: 	BYTE index[2];
   524: 	BYTE dir;
   525: 	BYTE action;
   526: 	BYTE target[2];
   527: };
   528: 
   529: struct PMSG_LIFE_SEND
   530: {
   531: 	PBMSG_HEAD header; // C1:26
   532: 	BYTE type;
   533: 	BYTE life[2];
   534: 	BYTE flag;
   535: 	BYTE shield[2];
   536: 	#if(GAMESERVER_EXTRA==1)
   537: 	DWORD ViewHP;
   538: 	DWORD ViewSD;
   539: 	#endif
   540: };
   541: 
   542: struct PMSG_MANA_SEND
   543: {
   544: 	PBMSG_HEAD header; // C1:27
   545: 	BYTE type;
   546: 	BYTE mana[2];
   547: 	BYTE bp[2];
   548: 	#if(GAMESERVER_EXTRA==1)
   549: 	DWORD ViewMP;
   550: 	DWORD ViewBP;
   551: 	#endif
   552: };
   553: 
   554: struct PMSG_ITEM_SPECIAL_TIME_SEND
   555: {
   556: 	PBMSG_HEAD header; // C3:29
   557: 	BYTE number;
   558: 	WORD time;
   559: };
   560: 
   561: struct PMSG_MAP_ATTR_SEND
   562: {
   563: 	PBMSG_HEAD header; // C1:46
   564: 	BYTE type;
   565: 	BYTE attr;
   566: 	BYTE flag;
   567: 	BYTE count;
   568: };
   569: 
   570: struct PMSG_MAP_ATTR
   571: {
   572: 	BYTE x;
   573: 	BYTE y;
   574: 	BYTE tx;
   575: 	BYTE ty;
   576: };
   577: 
   578: struct PMSG_PARTY_ITEM_INFO_SEND
   579: {
   580: 	PBMSG_HEAD header; // C1:47
   581: 	BYTE index[2];
   582: 	DWORD ItemInfo;
   583: 	BYTE level;
   584: };
   585: 
   586: struct PMSG_EFFECT_INFO_SEND
   587: {
   588: 	PBMSG_HEAD header; // C1:48
   589: 	BYTE index[2];
   590: 	BYTE effect;
   591: };
   592: 
   593: struct PMSG_MONSTER_SKILL_SEND
   594: {
   595: 	PBMSG_HEAD header; // C1:69
   596: 	BYTE skill[2];
   597: 	WORD index;
   598: 	WORD target;
   599: };
   600: 
   601: struct PMSG_EVENT_REMAIN_TIME_SEND
   602: {
   603: 	PBMSG_HEAD header; // C1:91
   604: 	BYTE EventType;
   605: 	BYTE RemainTimeH;
   606: 	BYTE EnteredUser;
   607: 	BYTE RemainTimeL;
   608: };
   609: 
   610: struct PMSG_TIME_COUNT_SEND
```


---

## QUESTIONS (para fechar 1.3-U como CONFIRMED)

1) O scan encontrou algum emissor de F3:03 no GS?

2) Se não, registrar como vestigial no pin e encerrar TODO.
