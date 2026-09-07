# NEW-ERA 1.3-T — ServerVersion + modo >=701 (impacto em 0x19) — EVIDENCE

- Legacy commit pin: `580472e0d5723f9709cbad594f233deb07f9f351`


## Files fetched (sha256)

- `WSclient.cpp` sha256 `6538d944126181dfa77d4419bf992ac77151eec5411dc3ab243f2926fec868bf` bytes=415884
- `WSclient.h` sha256 `19d38a3b6506a745662c14dd7cdd6e41ca3ad29b28b17c76f860cd9e67799c10` bytes=91012
- `GS_Protocol.cpp` sha256 `e8bb4dbb28d1baf23222dbc72762e2ebd55b116dbb8d36e5f017dd744e510f13` bytes=135014
- `GS_Protocol.h` sha256 `943c59b7a457eccea4953b7410fd2140c6299979782b58ca81a7b42a52d6c3b0` bytes=36305

---

## FILE: WSclient.cpp

### HIT: Client: JoinMapServer / F3:03 parsing / `F3\s*:?\s*03|JoinMapServer|GCJoinMapServer|MapServer|JoinGame`

- line 32: `#include "CSMapServer.h"`
- line 399: `        g_csMapServer.SendChangeMapServer();`
- line 855: `        g_csMapServer.Init ();`
- line 871: `BOOL ReceiveJoinMapServer(BYTE *ReceiveBuffer, BOOL bEncrypted)`
- line 983: `	g_ConsoleDebug->Write(MCD_RECEIVE, "0x03 [ReceiveJoinMapServer] Key: %d Map: %d X: %d Y:%d",c->Key,gMapManager.WorldActive,Data->PositionX,Data->PositionY);`
- line 1082: `	g_ConsoleDebug->Write(MCD_RECEIVE, "0x03 [ReceiveJoinMapServer]");`
- line 10227: `void ReceiveChangeMapServerInfo ( BYTE* ReceiveBuffer )`
- line 10241: `    g_csMapServer.ConnectChangeMapServer( Data->m_vSvrInfo );`
- line 10244: `void ReceiveChangeMapServerResult ( BYTE* ReceiveBuffer )`
- line 10248: `	g_ConsoleDebug->Write(MCD_RECEIVE, "0xB1 [ReceiveChangeMapServerResult]");`
- line 12970: `                if ( !ReceiveJoinMapServer(ReceiveBuffer, bEncrypted))`
- line 13761: `                ReceiveChangeMapServerInfo ( ReceiveBuffer );`
- line 13765: `                ReceiveChangeMapServerResult ( ReceiveBuffer );`

#### CONTEXT (first hit)

```
     1: #include "stdafx.h"
     2: #include "UIManager.h"
     3: #include "GuildCache.h"
     4: #include "ZzzBMD.h"
     5: #include "ZzzInfomation.h"
     6: #include "ZzzObject.h"
     7: #include "ZzzCharacter.h"
     8: #include "ZzzInterface.h"
     9: #include "ZzzInventory.h"
    10: #include "ZzzLodTerrain.h"
    11: #include "ZzzAI.h"
    12: #include "ZzzTexture.h"
    13: #include "ZzzEffect.h"
    14: #include "ZzzOpenglUtil.h"
    15: #include "ZzzOpenData.h"
    16: #include "ZzzScene.h"
    17: #include "wsclientinline.h"
    18: #include "DSPlaySound.h"
    19: #include "./Utilities/Log/DebugAngel.h"
    20: #include "./Utilities/Log/ErrorReport.h"
    21: #include "./Utilities/Memory/MemoryLock.h"
    22: #include "MatchEvent.h"
    23: #include "GOBoid.h"
    24: #ifdef SAVE_PACKET
    25: #include "./ExternalObject/leaf/stdleaf.h"
    26: #endif // SAVE_PACKET
    27: #include "CSQuest.h"
    28: #include "PersonalShopTitleImp.h"
    29: #include "GMHellas.h"
    30: #include "npcBreeder.h"
    31: #include "GIPetManager.h"
    32: #include "CSMapServer.h"
    33: #include "GMBattleCastle.h"
    34: #include "npcGateSwitch.h"
    35: #include "CComGem.h"
    36: #include "UIMapName.h" // rozy
    37: #include "UIMng.h"
    38: #include "GMCrywolf1st.h"
    39: #include "CDirection.h"
    40: #include "GM_Kanturu_3rd.h"
    41: #include "CSParts.h"
    42: #include "PhysicsManager.h"
    43: #include "Event.h"
    44: #include "MixMgr.h"
    45: #include "MapManager.h"
    46: #include "UIGuardsMan.h"
    47: #include "NewUISystem.h"
    48: #include "NewUICommonMessageBox.h"
    49: #include "NewUICustomMessageBox.h"
    50: #include "NewUICustomRanking.h"
    51: #include "NewUICustomEventTime.h"
    52: 
    53: extern bool g_bAutoTest;
    54: #include "NewUICustomBuyVip.h"
    55: #include "NewUIInventoryCtrl.h"
    56: #include "w_CursedTemple.h"
    57: #include "SummonSystem.h"
    58: #include "ProtocolSend.h"
    59: #include "CharacterManager.h"
    60: #include "SkillManager.h"
    61: 
    62: #ifdef KJH_ADD_INGAMESHOP_UI_SYSTEM
    63: #include "GameShop\InGameShopSystem.h"
    64: #include "GameShop\MsgBoxIGSCommon.h"
    65: #endif // KJH_ADD_INGAMESHOP_UI_SYSTEM
    66: 
    67: #include "w_MapHeaders.h"
    68: 
    69: #include "w_PetProcess.h"
    70: #include "PortalMgr.h"
    71: #include "DuelMgr.h"
    72: 
    73: #include "GambleSystem.h"
    74: #include "QuestMng.h"
    75: #ifdef PBG_ADD_SECRETBUFF
    76: #include "FatigueTimeSystem.h"
    77: #endif //PBG_ADD_SECRETBUFF
    78: #include "ServerListManager.h"
    79: #ifdef PBG_ADD_NEWCHAR_MONK_SKILL
    80: #include "MonkSystem.h"
    81: #endif //PBG_ADD_NEWCHAR_MONK_SKILL
    82: 
    83: #define MAX_DEBUG_MAX 10
    84: 
    85: extern BYTE m_AltarState[];
    86: extern int g_iChatInputType;
    87: extern BOOL g_bUseChatListBox;
    88: 
    89: #ifdef WINDOWMODE
    90: extern BOOL g_bUseWindowMode;
    91: #endif //WINDOWMODE
    92: extern CUITextInputBox * g_pSingleTextInputBox;
    93: 
    94: extern CChatRoomSocketList * g_pChatRoomSocketList;
    95: 
    96: #ifdef _PVP_ADD_MOVE_SCROLL
    97: extern CMurdererMove g_MurdererMove;
    98: #endif	// _PVP_ADD_MOVE_SCROLL
    99: 
   100: #ifdef _PVP_DYNAMIC_SERVER_TYPE
   101: extern BOOL g_bIsCurrentServerPvP;
   102: #endif	// _PVP_DYNAMIC_SERVER_TYPE
   103: 
   104: extern  short   g_shCameraLevel;
   105: 
   106: extern BYTE DebugText[MAX_DEBUG_MAX][256];
   107: extern int  DebugTextLength[MAX_DEBUG_MAX];
   108: extern char DebugTextCount;
   109: extern int  TotalPacketSize;
   110: extern int g_iKeyPadEnable;
   111: 
   112: extern CUIMapName* g_pUIMapName; // rozy
   113: 
   114: extern bool g_PetEnableDuel;
   115: 
   116: MASTER_LEVEL_VALUE	Master_Level_Data;
   117: 
   118: BYTE Version[SIZE_PROTOCOLVERSION] = {'1'+1, '0'+2, '4'+3, '0'+4, '5'+5};
   119: BYTE Serial[SIZE_PROTOCOLSERIAL+1] = {"TbYehR2hFUPBKgZj"};
   120: 
   121: CWsctlc     SocketClient;
   122: CWsctlc*    g_pSocketClient = &SocketClient;
   123: CSimpleModulus g_SimpleModulusCS;
   124: CSimpleModulus g_SimpleModulusSC;
   125: 
   126: BYTE    g_byPacketSerialSend = 0;
   127: BYTE    g_byPacketSerialRecv = 0;
   128: 
   129: BOOL    g_bGameServerConnected = FALSE;
   130: DWORD   g_dwLatestMagicTick = 0;
   131: 
   132: PMSG_MATCH_RESULT	g_wtMatchResult;
   133: PMSG_MATCH_TIMEVIEW	g_wtMatchTimeLeft;
   134: int g_iGoalEffect = 0;
   135: 
   136: CROWN_SWITCH_INFO *Switch_Info = NULL;
   137: 
   138: int     HeroKey;
   139: int     CurrentProtocolState;
   140: 
   141: int DirTable[16] = {-1,-1,  0,-1,  1,-1,  1,0,  1,1,  0,1,  -1,1,  -1,0};
   142: 
   143: char    Password[MAX_ID_SIZE+1];
   144: char    QuestionID[MAX_ID_SIZE+1];
   145: char    Question[31];
   146: 
   147: #define FIRST_CROWN_SWITCH_NUMBER	322
   148: 
   149: void AddDebugText(unsigned char *Buffer,int Size)
   150: {
   151: 	if(DebugTextCount > MAX_DEBUG_MAX-1)
   152: 	{
   153: 		DebugTextCount = MAX_DEBUG_MAX-1;
   154: 		for(int i=1;i<MAX_DEBUG_MAX;i++)
   155: 		{
   156: 			memcpy(DebugText[i-1],DebugText[i],DebugTextLength[i]);
   157: 			DebugTextLength[i-1] = DebugTextLength[i];
   158: 		}
   159: 	}
   160: 	if(Size <= 256)
   161: 	{
   162: 		memcpy(DebugText[DebugTextCount],Buffer,Size);
   163: 		DebugTextLength[DebugTextCount] = Size;
   164: 		DebugTextCount++;
   165: 	}
   166: }
   167: 
   168: BOOL CreateSocket(char *IpAddr, unsigned short Port)
   169: {
   170: 	BOOL bResult = TRUE;
   171: 
   172: 	static bool First;
   173: 	if(!First)
   174: 	{
   175: 		First = true;
   176: 		SocketClient.Startup();
   177: #ifdef _DEBUG
   178: 		SocketClient.LogPrintOn();
   179: #endif
   180: 	}
   181: 	g_ErrorReport.Write("[Connect to Server] ip address = %s, port = %d\r\n", IpAddr, Port);
   182: 
   183: 	g_ConsoleDebug->Write(MCD_NORMAL, "[Connect to Server] ip address = %s, port = %d", IpAddr, Port);
   184: 
   185: 	SocketClient.Create(g_hWnd, TRUE);
   186: 	if( SocketClient.Connect(IpAddr,Port,WM_ASYNCSELECTMSG) == FALSE )
   187: 	{
   188: 		g_ErrorReport.Write( "Failed to connect. ");
   189: 		g_ErrorReport.WriteCurrentTime();
   190: 
   191: 		CUIMng::Instance().PopUpMsgWin(MESSAGE_SERVER_LOST);
   192: 
   193: 		bResult = FALSE;
   194: 	}
   195: 	g_byPacketSerialSend = 0;
   196: 	g_byPacketSerialRecv = 0;
   197: 
   198: 	return ( bResult);
   199: }
   200: 
   201: void DeleteSocket()
   202: {
   203: 	SocketClient.Close();
   204: 	#ifdef NEW_PROTOCOL_SYSTEM
   205: 		gProtocolSend.DisconnectServer();
   206: 	#endif
   207: }
   208: 
   209: static BYTE bBuxCode[3] = {0xfc,0xcf,0xab};
   210: 
   211: void BuxConvert(BYTE *Buffer,int Size)
   212: {
   213: 	for(int i=0;i<Size;i++)
   214: 		Buffer[i] ^= bBuxCode[i%3];
   215: }
   216: 
   217: 
   218: int  LogIn = 0;
   219: char LogInID[MAX_ID_SIZE+1] = {0, };
   220: 
   221: bool First = false;
   222: int FirstTime = 0;
   223: 
   224: bool LogOut = false;
   225: 
   226: int  ChatTime = 0;
   227: char ChatText[256];
   228: 
   229: char ChatWhisperID[MAX_ID_SIZE+1];
   230: 
   231: int MoveCount = 0;
   232: 
   233: int CurrentSkill = 0;
   234: 
   235: bool Teleport = false;
   236: 
   237: int BuyCost = 0;
   238: 
   239: int  EnableUse = 0;
   240: 
   241: int SendGetItem = -1;
   242: int SendDropItem = -1;
   243: 
   244: int FindGuildName(char *Name)
   245: {
   246: 	for(int i=0;i<MARK_EDIT;i++)
   247: 	{
   248: 		MARK_t *p = &GuildMark[i];
   249: 		if(strcmp(p->GuildName,Name)==NULL)
   250: 		{
   251: 			return i;
   252: 		}
   253: 	}
   254: 	return -1;
   255: }
   256: 
   257: void GuildTeam( CHARACTER *c )
   258: {
   259: 	OBJECT *o = &c->Object;
   260: 	if(o->Live)
   261: 	{
   262: 		c->GuildTeam = 0;
   263: 
   264: 		if( Hero->GuildMarkIndex!=-1 && c->GuildMarkIndex==Hero->GuildMarkIndex )
   265: 			c->GuildTeam = 1;
   266: 
   267: 		if( EnableGuildWar )
   268: 		{
   269: 			if( GuildWarIndex==-1 && GuildWarName[0] )
   270: 			{
   271: 				GuildWarIndex = FindGuildName( GuildWarName );
   272: 			}
```

### HIT: Magic (0x19) tokens / `case\s+0x19\b|\b0x19\b|ReceiveMagic|SKILL_ATTACK_SEND|PMSG_SKILL_ATTACK_SEND`

- line 1245: `void ReceiveMagicList( BYTE *ReceiveBuffer )`
- line 1339: `	g_ConsoleDebug->Write(MCD_RECEIVE, "0x11 [ReceiveMagicList]");`
- line 3501: `void ReceiveMagicFinish( BYTE *ReceiveBuffer )`
- line 3751: `BOOL ReceiveMagic(BYTE *ReceiveBuffer,int Size, BOOL bEncrypted)`
- line 3757: `//		SendHackingChecked( 0x00, 0x19);`
- line 4792: `	g_ConsoleDebug->Write(MCD_RECEIVE, "0x19 [ReceiveMagic(%d)]", MagicNumber);`
- line 4798: `BOOL ReceiveMagicContinue(BYTE *ReceiveBuffer,int Size, BOOL bEncrypted)`
- line 5258: `	g_ConsoleDebug->Write(MCD_RECEIVE, "0x1E [ReceiveMagicContinue(%d)]", MagicNumber);`
- line 5318: `void ReceiveMagicPosition(BYTE *ReceiveBuffer,int Size)`
- line 6007: `	case 0x19:`
- line 12998: `                ReceiveMagicList(ReceiveBuffer);`
- line 13149: `	case 0x19://magic`
- line 13150: `        if ( !ReceiveMagic(ReceiveBuffer,Size, bEncrypted))`
- line 13162: `        ReceiveMagicPosition(ReceiveBuffer,Size);`
- line 13165: `        if ( !ReceiveMagicContinue(ReceiveBuffer,Size, bEncrypted))`
- line 13171: `        ReceiveMagicFinish(ReceiveBuffer);`

#### CONTEXT (first hit)

```
  1175: 			g_pNewUISystem->Hide(SEASON3B::INTERFACE_CURSEDTEMPLE_GAMESYSTEM);
  1176: 		}
  1177: 	}
  1178: 
  1179:     if(gMapManager.WorldActive != Data->Map)
  1180: 	{
  1181:         int OldWorld = gMapManager.WorldActive;
  1182: 
  1183: 		gMapManager.WorldActive = Data->Map;
  1184: 		gMapManager.LoadWorld(gMapManager.WorldActive);
  1185: 
  1186:         if ( ( gMapManager.InChaosCastle( OldWorld ) == true && OldWorld!=gMapManager.WorldActive )
  1187: 			|| gMapManager.InChaosCastle() == true )
  1188:         {
  1189:             SetCharacterClass ( Hero );
  1190:         }
  1191: 
  1192:         if ( gMapManager.InChaosCastle() == false )
  1193:         {
  1194:             StopBuffer ( SOUND_CHAOSCASTLE, true );
  1195:             StopBuffer ( SOUND_CHAOS_ENVIR, true );
  1196: 		}
  1197: 
  1198: 		if( gMapManager.IsEmpireGuardian1() == false &&
  1199: 			gMapManager.IsEmpireGuardian2() == false &&
  1200: 			gMapManager.IsEmpireGuardian3() == false &&
  1201: 			gMapManager.IsEmpireGuardian4() == false )
  1202: 		{
  1203: 			StopBuffer(SOUND_EMPIREGUARDIAN_WEATHER_RAIN, true);
  1204: 			StopBuffer(SOUND_EMPIREGUARDIAN_WEATHER_FOG, true);
  1205: 			StopBuffer(SOUND_EMPIREGUARDIAN_WEATHER_STORM, true);
  1206: 			StopBuffer(SOUND_EMPIREGUARDIAN_INDOOR_SOUND, true);
  1207: 		}
  1208: 
  1209:         if ( gMapManager.WorldActive==-1 || c->Helper.Type!=MODEL_HELPER+3 || c->SafeZone )
  1210:         {
  1211:             o->Position[2] = RequestTerrainHeight(o->Position[0],o->Position[1]);
  1212:         }
  1213:         else
  1214:         {
  1215:             if ( gMapManager.WorldActive==WD_8TARKAN || gMapManager.WorldActive==WD_10HEAVEN )
  1216:                 o->Position[2] = RequestTerrainHeight(o->Position[0],o->Position[1])+90.f;
  1217:             else
  1218:                 o->Position[2] = RequestTerrainHeight(o->Position[0],o->Position[1])+30.f;
  1219:         }
  1220: 	}
  1221: 
  1222: 	CreatePetDarkSpirit_Now ( c );
  1223:     SummonLife = 0;
  1224: 	GuildTeam(c);
  1225: 
  1226: 	g_pUIMapName->ShowMapName();	// rozy
  1227: 
  1228: 	CreateMyGensInfluenceGroundEffect();
  1229: 
  1230: 	if( gMapManager.WorldActive < WD_65DOPPLEGANGER1 || gMapManager.WorldActive > WD_68DOPPLEGANGER4 )
  1231: 	{
  1232: 		g_pNewUISystem->Hide(SEASON3B::INTERFACE_DOPPELGANGER_FRAME);
  1233: 	}
  1234: 	if( gMapManager.WorldActive < WD_69EMPIREGUARDIAN1 || WD_72EMPIREGUARDIAN4 < gMapManager.WorldActive)
  1235: 	{
  1236: 		g_pNewUISystem->Hide(SEASON3B::INTERFACE_EMPIREGUARDIAN_TIMER);
  1237: 	}
  1238: 
  1239: 	g_pNewUISystem->HideAll();
  1240: 
  1241: 	g_ConsoleDebug->Write(MCD_RECEIVE, "0x04 [ReceiveRevival]");
  1242: 
  1243: }
  1244: 
  1245: void ReceiveMagicList( BYTE *ReceiveBuffer )
  1246: {
  1247: 	int Master_Skill_Bool = -1;
  1248: 	int Skill_Bool = -1;
  1249: 
  1250: 	LPPHEADER_MAGIC_LIST_COUNT Data = (LPPHEADER_MAGIC_LIST_COUNT)ReceiveBuffer;
  1251: 	int Offset = sizeof(PHEADER_MAGIC_LIST_COUNT);
  1252: 	if(Data->Value == 0xFF)
  1253: 	{
  1254: 		LPPRECEIVE_MAGIC_LIST Data2 = (LPPRECEIVE_MAGIC_LIST)(ReceiveBuffer+Offset);
  1255: 		CharacterAttribute->Skill[Data2->Index] = 0;
  1256: 	}
  1257: 	else if(Data->Value == 0xFE)
  1258: 	{
  1259: 		LPPRECEIVE_MAGIC_LIST Data2 = (LPPRECEIVE_MAGIC_LIST)(ReceiveBuffer+Offset);
  1260: 		CharacterAttribute->Skill[Data2->Index] = Data2->Type;
  1261: 	}
  1262:     else if ( Data->ListType==0x02 )
  1263:     {
  1264:         for ( int i=0; i<Data->Value; ++i )
  1265:         {
  1266: 			LPPRECEIVE_MAGIC_LIST Data2 = (LPPRECEIVE_MAGIC_LIST)(ReceiveBuffer+Offset);
  1267: 			CharacterAttribute->Skill[Data2->Index] = 0;
  1268:         }
  1269:     }
  1270: 	else
  1271: 	{
  1272:         if ( Data->ListType == 0x00 )
  1273:         {
  1274: 			ZeroMemory( CharacterAttribute->Skill,  MAX_SKILLS * sizeof ( WORD));
  1275:         }
  1276: 		for(int i=0; i<Data->Value; i++)
  1277: 		{
  1278: 			LPPRECEIVE_MAGIC_LIST Data2 = (LPPRECEIVE_MAGIC_LIST)(ReceiveBuffer+Offset);
  1279: 			CharacterAttribute->Skill[Data2->Index] = Data2->Type;
  1280: 			Offset += sizeof(PRECEIVE_MAGIC_LIST);
  1281: 		}
  1282:         if (gCharacterManager.GetBaseClass( Hero->Class )==CLASS_DARK_LORD )
  1283:         {
  1284:             for ( int i=0; i<PET_CMD_END; ++i )
  1285:             {
  1286:                 CharacterAttribute->Skill[AT_PET_COMMAND_DEFAULT+i] = AT_PET_COMMAND_DEFAULT+i;
  1287:             }
  1288:         }
  1289: 	}
  1290: 
  1291: 	CharacterAttribute->SkillNumber = 0;
  1292: 	CharacterAttribute->SkillMasterNumber = 0;
  1293: 
  1294:     int SkillType = 0;
  1295: 	for(int i=0;i<MAX_SKILLS;i++)
  1296: 	{
  1297: 		SkillType = CharacterAttribute->Skill[i];
  1298: 		if ( SkillType!=0 )
  1299: 		{
  1300: 			CharacterAttribute->SkillNumber++;
  1301:             BYTE SkillUseType = SkillAttribute[SkillType].SkillUseType;
  1302:             if ( SkillUseType==SKILL_USE_TYPE_MASTER )
  1303:             {
  1304: 				CharacterAttribute->SkillMasterNumber++;
  1305:             }
  1306: 		}
  1307: 	}
  1308: 	if(Hero->CurrentSkill >= CharacterAttribute->SkillNumber)
  1309: 		Hero->CurrentSkill = 0;
  1310:     if(CharacterAttribute->SkillNumber == 1)
  1311: 		Hero->CurrentSkill = 0;
  1312:     if(Hero->CurrentSkill>=0 && CharacterAttribute->Skill[Hero->CurrentSkill]==0)
  1313: 		Hero->CurrentSkill = 0;
  1314: 	int Skill = 0;
  1315: 
  1316: 	for(int i = 0; i < MAX_SKILLS; i++)
  1317: 	{
  1318: 		Skill = CharacterAttribute->Skill[Hero->CurrentSkill];
  1319: 		if ( Skill>= AT_SKILL_STUN && Skill<=AT_SKILL_REMOVAL_BUFF )
  1320: 			Hero->CurrentSkill++;
  1321: 		else
  1322: 			break;
  1323: 	}
  1324: 	for(int i = 0; i < MAX_SKILLS; i++)
  1325: 	{
  1326: 		Skill = CharacterAttribute->Skill[i];
  1327: 		if((AT_SKILL_POWER_SLASH_UP <= Skill && AT_SKILL_POWER_SLASH_UP+4 >= Skill) || (AT_SKILL_MANY_ARROW_UP <= Skill && AT_SKILL_MANY_ARROW_UP+4 >= Skill))
  1328: 		{
  1329: 			Master_Skill_Bool = i;
  1330: 		}
  1331: 		if (AT_SKILL_ICE_BLADE == Skill || Skill == AT_SKILL_CROSSBOW)
  1332: 		{
  1333: 			Skill_Bool  = i;
  1334: 		}
  1335: 	}
  1336: 	if(Master_Skill_Bool > -1 && Skill_Bool > -1)
  1337: 		CharacterAttribute->Skill[Skill_Bool] = 0;
  1338: 
  1339: 	g_ConsoleDebug->Write(MCD_RECEIVE, "0x11 [ReceiveMagicList]");
  1340: 
  1341: }
  1342: 
  1343: BOOL ReceiveInventory(BYTE *ReceiveBuffer, BOOL bEncrypted)
  1344: {
  1345: 	for(int i=0;i<MAX_EQUIPMENT;i++)
  1346: 	{
  1347: 		CharacterMachine->Equipment[i].Type = -1;
  1348: 		CharacterMachine->Equipment[i].Number = 0;
  1349:         CharacterMachine->Equipment[i].Option1 = 0;
  1350: 	}
  1351: 
  1352: 	g_pMyInventory->UnequipAllItems();
  1353: 	g_pMyInventory->DeleteAllItems();
  1354: 	g_pMyShopInventory->DeleteAllItems();
  1355: 
  1356: //#ifndef NEW_PROTOCOL_SYSTEM
  1357: //	if ( !bEncrypted)
  1358: //	{
  1359: //		GO_DEBUG;
  1360: //		SendHackingChecked( 0x00, 0xF3);
  1361: //		return ( FALSE);
  1362: //	}
  1363: //#endif
  1364: 
  1365: 	LPPHEADER_DEFAULT_SUBCODE_WORD Data = (LPPHEADER_DEFAULT_SUBCODE_WORD)ReceiveBuffer; //LPPHEADER_DEFAULT_SUBCODE_WORD 6byte
  1366: 	int Offset = sizeof(PHEADER_DEFAULT_SUBCODE_WORD);
  1367: 	DeleteBug(&Hero->Object);
  1368:     giPetManager::DeletePet ( Hero );
  1369: 
  1370: 	ThePetProcess().DeletePet( Hero );
  1371: 
  1372: 	for(int i=0;i<Data->Value;i++)
  1373: 	{
  1374: 		LPPRECEIVE_INVENTORY Data2 = (LPPRECEIVE_INVENTORY)(ReceiveBuffer+Offset); //LPPRECEIVE_INVENTORY 8byte
  1375: 
  1376: 		SEASON3B::CNewUIInventoryCtrl::DeletePickedItem();
  1377: 		int itemindex = Data2->Index;
  1378: 		if(itemindex >= 0 && itemindex < MAX_EQUIPMENT_INDEX)
  1379: 		{
  1380: 			g_pMyInventory->EquipItem(itemindex, Data2->Item);
  1381: 		}
  1382: 		else if(itemindex >= MAX_EQUIPMENT_INDEX && itemindex < MAX_MY_INVENTORY_INDEX)
  1383: 		{
  1384: 			itemindex = itemindex - MAX_EQUIPMENT_INDEX;
  1385: 			g_pMyInventory->InsertItem(itemindex, Data2->Item);
  1386: 		}
  1387: 		else if(itemindex >= (MAX_EQUIPMENT_INDEX + MAX_INVENTORY) && itemindex < MAX_MY_SHOP_INVENTORY_INDEX)
  1388: 		{
  1389: 			itemindex = itemindex - (MAX_EQUIPMENT_INDEX + MAX_INVENTORY);
  1390: 			g_pMyShopInventory->InsertItem(itemindex, Data2->Item);
  1391: 		}
  1392: 
  1393: 		Offset += sizeof(PRECEIVE_INVENTORY);
  1394: 	}
  1395: 
  1396: 	g_ConsoleDebug->Write(MCD_RECEIVE, "0x10 [ReceiveInventory]");
  1397: 
  1398: 	return ( TRUE);
  1399: }
  1400: 
  1401: void ReceiveDeleteInventory( BYTE *ReceiveBuffer )
  1402: {
  1403: 	LPPHEADER_DEFAULT_SUBCODE Data = (LPPHEADER_DEFAULT_SUBCODE)ReceiveBuffer;
  1404: 	if(Data->SubCode != 0xff)
  1405: 	{
  1406: 		int itemindex = Data->SubCode;
  1407: 		if(itemindex >= 0 && itemindex < MAX_EQUIPMENT_INDEX)
  1408: 		{
  1409: 			g_pMyInventory->UnequipItem(itemindex);
  1410: 		}
  1411: 		else if(itemindex >= MAX_EQUIPMENT_INDEX && itemindex < MAX_MY_INVENTORY_INDEX)
  1412: 		{
  1413: 			itemindex = itemindex - MAX_EQUIPMENT_INDEX;
  1414: 			g_pMyInventory->DeleteItem(itemindex);
  1415: 		}
  1416: 		else if(itemindex >= (MAX_EQUIPMENT_INDEX + MAX_INVENTORY) && itemindex < MAX_MY_SHOP_INVENTORY_INDEX)
  1417: 		{
  1418: 			itemindex = itemindex - (MAX_EQUIPMENT_INDEX + MAX_INVENTORY);
  1419: 			g_pMyShopInventory->DeleteItem(itemindex);
  1420: 		}
  1421: 	}
  1422: 
  1423: 	if(Data->Value)
  1424: 	{
  1425: 		EnableUse = 0;
  1426: 	}
  1427: 
  1428: 	g_ConsoleDebug->Write(MCD_RECEIVE, "0x28 [ReceiveDeleteInventory(%d %d)]", Data->SubCode, Data->Value);
  1429: }
  1430: 
  1431: void ReceiveTradeInventory( BYTE *ReceiveBuffer )
  1432: {
  1433: 	LPPHEADER_DEFAULT_SUBCODE_WORD Data = (LPPHEADER_DEFAULT_SUBCODE_WORD)ReceiveBuffer;
  1434: 	int Offset = sizeof(PHEADER_DEFAULT_SUBCODE_WORD);
  1435: 
  1436: 	if(Data->SubCode == 3)
  1437: 	{
  1438: 		g_pMixInventory->SetMixState(SEASON3B::CNewUIMixInventory::MIX_FINISHED);
  1439: 		PlayBuffer(SOUND_MIX01);
  1440: 		PlayBuffer(SOUND_BREAK01);
  1441: 		g_pMixInventory->DeleteAllItems();
  1442: 	}
  1443:     else if ( Data->SubCode==5 )
  1444:     {
  1445: 		g_pChatListBox->AddText( "", GlobalText[1208], SEASON3B::TYPE_ERROR_MESSAGE);
  1446: 		PlayBuffer ( SOUND_MIX01 );
  1447: 		PlayBuffer ( SOUND_BREAK01 );
  1448: 		g_pMixInventory->SetMixState(SEASON3B::CNewUIMixInventory::MIX_FINISHED);
  1449: 		g_pMixInventory->DeleteAllItems();
  1450:     }
  1451: 	else
  1452: 	{
  1453: 		for(int i=0;i<MAX_SHOP_INVENTORY;i++)
  1454: 		{
  1455: 			ShopInventory[i].Type = -1;
  1456: 			ShopInventory[i].Number = 0;
  1457: 		}
  1458: 	}
  1459: 
  1460: 	for(int i=0;i<Data->Value;i++)
  1461: 	{
  1462: 		LPPRECEIVE_INVENTORY Data2 = (LPPRECEIVE_INVENTORY)(ReceiveBuffer+Offset);
  1463: 
  1464: 		if(Data->SubCode == 3)
  1465: 		{
  1466: 			g_pMixInventory->InsertItem(Data2->Index, Data2->Item);
  1467: 		}
  1468:         else if ( Data->SubCode==5 )
  1469:         {
  1470: 			g_pMixInventory->InsertItem(Data2->Index, Data2->Item);
  1471:         }
  1472: 		else
  1473: 		{
  1474: 			if(g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_NPCSHOP) == true)
  1475: 			{
  1476: 				g_pNPCShop->InsertItem(Data2->Index, Data2->Item);
  1477: 			}
  1478: 			else if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_STORAGE))
  1479: 			{
  1480: 				g_pStorageInventory->InsertItem(Data2->Index, Data2->Item);
  1481: 			}
  1482: 		}
  1483: 
  1484: 		Offset += sizeof(PRECEIVE_INVENTORY);
  1485: 	}
```

### HIT: Server: F3:03 sending / `F3\s*:?\s*03|PHeadSubSet|PHeadSet|JoinMapServer|GCJoinMapServer|MapServer`

- line 32: `#include "CSMapServer.h"`
- line 399: `        g_csMapServer.SendChangeMapServer();`
- line 855: `        g_csMapServer.Init ();`
- line 871: `BOOL ReceiveJoinMapServer(BYTE *ReceiveBuffer, BOOL bEncrypted)`
- line 983: `	g_ConsoleDebug->Write(MCD_RECEIVE, "0x03 [ReceiveJoinMapServer] Key: %d Map: %d X: %d Y:%d",c->Key,gMapManager.WorldActive,Data->PositionX,Data->PositionY);`
- line 1082: `	g_ConsoleDebug->Write(MCD_RECEIVE, "0x03 [ReceiveJoinMapServer]");`
- line 10227: `void ReceiveChangeMapServerInfo ( BYTE* ReceiveBuffer )`
- line 10241: `    g_csMapServer.ConnectChangeMapServer( Data->m_vSvrInfo );`
- line 10244: `void ReceiveChangeMapServerResult ( BYTE* ReceiveBuffer )`
- line 10248: `	g_ConsoleDebug->Write(MCD_RECEIVE, "0xB1 [ReceiveChangeMapServerResult]");`
- line 12970: `                if ( !ReceiveJoinMapServer(ReceiveBuffer, bEncrypted))`
- line 13761: `                ReceiveChangeMapServerInfo ( ReceiveBuffer );`
- line 13765: `                ReceiveChangeMapServerResult ( ReceiveBuffer );`

#### CONTEXT (first hit)

```
     1: #include "stdafx.h"
     2: #include "UIManager.h"
     3: #include "GuildCache.h"
     4: #include "ZzzBMD.h"
     5: #include "ZzzInfomation.h"
     6: #include "ZzzObject.h"
     7: #include "ZzzCharacter.h"
     8: #include "ZzzInterface.h"
     9: #include "ZzzInventory.h"
    10: #include "ZzzLodTerrain.h"
    11: #include "ZzzAI.h"
    12: #include "ZzzTexture.h"
    13: #include "ZzzEffect.h"
    14: #include "ZzzOpenglUtil.h"
    15: #include "ZzzOpenData.h"
    16: #include "ZzzScene.h"
    17: #include "wsclientinline.h"
    18: #include "DSPlaySound.h"
    19: #include "./Utilities/Log/DebugAngel.h"
    20: #include "./Utilities/Log/ErrorReport.h"
    21: #include "./Utilities/Memory/MemoryLock.h"
    22: #include "MatchEvent.h"
    23: #include "GOBoid.h"
    24: #ifdef SAVE_PACKET
    25: #include "./ExternalObject/leaf/stdleaf.h"
    26: #endif // SAVE_PACKET
    27: #include "CSQuest.h"
    28: #include "PersonalShopTitleImp.h"
    29: #include "GMHellas.h"
    30: #include "npcBreeder.h"
    31: #include "GIPetManager.h"
    32: #include "CSMapServer.h"
    33: #include "GMBattleCastle.h"
    34: #include "npcGateSwitch.h"
    35: #include "CComGem.h"
    36: #include "UIMapName.h" // rozy
    37: #include "UIMng.h"
    38: #include "GMCrywolf1st.h"
    39: #include "CDirection.h"
    40: #include "GM_Kanturu_3rd.h"
    41: #include "CSParts.h"
    42: #include "PhysicsManager.h"
    43: #include "Event.h"
    44: #include "MixMgr.h"
    45: #include "MapManager.h"
    46: #include "UIGuardsMan.h"
    47: #include "NewUISystem.h"
    48: #include "NewUICommonMessageBox.h"
    49: #include "NewUICustomMessageBox.h"
    50: #include "NewUICustomRanking.h"
    51: #include "NewUICustomEventTime.h"
    52: 
    53: extern bool g_bAutoTest;
    54: #include "NewUICustomBuyVip.h"
    55: #include "NewUIInventoryCtrl.h"
    56: #include "w_CursedTemple.h"
    57: #include "SummonSystem.h"
    58: #include "ProtocolSend.h"
    59: #include "CharacterManager.h"
    60: #include "SkillManager.h"
    61: 
    62: #ifdef KJH_ADD_INGAMESHOP_UI_SYSTEM
    63: #include "GameShop\InGameShopSystem.h"
    64: #include "GameShop\MsgBoxIGSCommon.h"
    65: #endif // KJH_ADD_INGAMESHOP_UI_SYSTEM
    66: 
    67: #include "w_MapHeaders.h"
    68: 
    69: #include "w_PetProcess.h"
    70: #include "PortalMgr.h"
    71: #include "DuelMgr.h"
    72: 
    73: #include "GambleSystem.h"
    74: #include "QuestMng.h"
    75: #ifdef PBG_ADD_SECRETBUFF
    76: #include "FatigueTimeSystem.h"
    77: #endif //PBG_ADD_SECRETBUFF
    78: #include "ServerListManager.h"
    79: #ifdef PBG_ADD_NEWCHAR_MONK_SKILL
    80: #include "MonkSystem.h"
    81: #endif //PBG_ADD_NEWCHAR_MONK_SKILL
    82: 
    83: #define MAX_DEBUG_MAX 10
    84: 
    85: extern BYTE m_AltarState[];
    86: extern int g_iChatInputType;
    87: extern BOOL g_bUseChatListBox;
    88: 
    89: #ifdef WINDOWMODE
    90: extern BOOL g_bUseWindowMode;
    91: #endif //WINDOWMODE
    92: extern CUITextInputBox * g_pSingleTextInputBox;
    93: 
    94: extern CChatRoomSocketList * g_pChatRoomSocketList;
    95: 
    96: #ifdef _PVP_ADD_MOVE_SCROLL
    97: extern CMurdererMove g_MurdererMove;
    98: #endif	// _PVP_ADD_MOVE_SCROLL
    99: 
   100: #ifdef _PVP_DYNAMIC_SERVER_TYPE
   101: extern BOOL g_bIsCurrentServerPvP;
   102: #endif	// _PVP_DYNAMIC_SERVER_TYPE
   103: 
   104: extern  short   g_shCameraLevel;
   105: 
   106: extern BYTE DebugText[MAX_DEBUG_MAX][256];
   107: extern int  DebugTextLength[MAX_DEBUG_MAX];
   108: extern char DebugTextCount;
   109: extern int  TotalPacketSize;
   110: extern int g_iKeyPadEnable;
   111: 
   112: extern CUIMapName* g_pUIMapName; // rozy
   113: 
   114: extern bool g_PetEnableDuel;
   115: 
   116: MASTER_LEVEL_VALUE	Master_Level_Data;
   117: 
   118: BYTE Version[SIZE_PROTOCOLVERSION] = {'1'+1, '0'+2, '4'+3, '0'+4, '5'+5};
   119: BYTE Serial[SIZE_PROTOCOLSERIAL+1] = {"TbYehR2hFUPBKgZj"};
   120: 
   121: CWsctlc     SocketClient;
   122: CWsctlc*    g_pSocketClient = &SocketClient;
   123: CSimpleModulus g_SimpleModulusCS;
   124: CSimpleModulus g_SimpleModulusSC;
   125: 
   126: BYTE    g_byPacketSerialSend = 0;
   127: BYTE    g_byPacketSerialRecv = 0;
   128: 
   129: BOOL    g_bGameServerConnected = FALSE;
   130: DWORD   g_dwLatestMagicTick = 0;
   131: 
   132: PMSG_MATCH_RESULT	g_wtMatchResult;
   133: PMSG_MATCH_TIMEVIEW	g_wtMatchTimeLeft;
   134: int g_iGoalEffect = 0;
   135: 
   136: CROWN_SWITCH_INFO *Switch_Info = NULL;
   137: 
   138: int     HeroKey;
   139: int     CurrentProtocolState;
   140: 
   141: int DirTable[16] = {-1,-1,  0,-1,  1,-1,  1,0,  1,1,  0,1,  -1,1,  -1,0};
   142: 
   143: char    Password[MAX_ID_SIZE+1];
   144: char    QuestionID[MAX_ID_SIZE+1];
   145: char    Question[31];
   146: 
   147: #define FIRST_CROWN_SWITCH_NUMBER	322
   148: 
   149: void AddDebugText(unsigned char *Buffer,int Size)
   150: {
   151: 	if(DebugTextCount > MAX_DEBUG_MAX-1)
   152: 	{
   153: 		DebugTextCount = MAX_DEBUG_MAX-1;
   154: 		for(int i=1;i<MAX_DEBUG_MAX;i++)
   155: 		{
   156: 			memcpy(DebugText[i-1],DebugText[i],DebugTextLength[i]);
   157: 			DebugTextLength[i-1] = DebugTextLength[i];
   158: 		}
   159: 	}
   160: 	if(Size <= 256)
   161: 	{
   162: 		memcpy(DebugText[DebugTextCount],Buffer,Size);
   163: 		DebugTextLength[DebugTextCount] = Size;
   164: 		DebugTextCount++;
   165: 	}
   166: }
   167: 
   168: BOOL CreateSocket(char *IpAddr, unsigned short Port)
   169: {
   170: 	BOOL bResult = TRUE;
   171: 
   172: 	static bool First;
   173: 	if(!First)
   174: 	{
   175: 		First = true;
   176: 		SocketClient.Startup();
   177: #ifdef _DEBUG
   178: 		SocketClient.LogPrintOn();
   179: #endif
   180: 	}
   181: 	g_ErrorReport.Write("[Connect to Server] ip address = %s, port = %d\r\n", IpAddr, Port);
   182: 
   183: 	g_ConsoleDebug->Write(MCD_NORMAL, "[Connect to Server] ip address = %s, port = %d", IpAddr, Port);
   184: 
   185: 	SocketClient.Create(g_hWnd, TRUE);
   186: 	if( SocketClient.Connect(IpAddr,Port,WM_ASYNCSELECTMSG) == FALSE )
   187: 	{
   188: 		g_ErrorReport.Write( "Failed to connect. ");
   189: 		g_ErrorReport.WriteCurrentTime();
   190: 
   191: 		CUIMng::Instance().PopUpMsgWin(MESSAGE_SERVER_LOST);
   192: 
   193: 		bResult = FALSE;
   194: 	}
   195: 	g_byPacketSerialSend = 0;
   196: 	g_byPacketSerialRecv = 0;
   197: 
   198: 	return ( bResult);
   199: }
   200: 
   201: void DeleteSocket()
   202: {
   203: 	SocketClient.Close();
   204: 	#ifdef NEW_PROTOCOL_SYSTEM
   205: 		gProtocolSend.DisconnectServer();
   206: 	#endif
   207: }
   208: 
   209: static BYTE bBuxCode[3] = {0xfc,0xcf,0xab};
   210: 
   211: void BuxConvert(BYTE *Buffer,int Size)
   212: {
   213: 	for(int i=0;i<Size;i++)
   214: 		Buffer[i] ^= bBuxCode[i%3];
   215: }
   216: 
   217: 
   218: int  LogIn = 0;
   219: char LogInID[MAX_ID_SIZE+1] = {0, };
   220: 
   221: bool First = false;
   222: int FirstTime = 0;
   223: 
   224: bool LogOut = false;
   225: 
   226: int  ChatTime = 0;
   227: char ChatText[256];
   228: 
   229: char ChatWhisperID[MAX_ID_SIZE+1];
   230: 
   231: int MoveCount = 0;
   232: 
   233: int CurrentSkill = 0;
   234: 
   235: bool Teleport = false;
   236: 
   237: int BuyCost = 0;
   238: 
   239: int  EnableUse = 0;
   240: 
   241: int SendGetItem = -1;
   242: int SendDropItem = -1;
   243: 
   244: int FindGuildName(char *Name)
   245: {
   246: 	for(int i=0;i<MARK_EDIT;i++)
   247: 	{
   248: 		MARK_t *p = &GuildMark[i];
   249: 		if(strcmp(p->GuildName,Name)==NULL)
   250: 		{
   251: 			return i;
   252: 		}
   253: 	}
   254: 	return -1;
   255: }
   256: 
   257: void GuildTeam( CHARACTER *c )
   258: {
   259: 	OBJECT *o = &c->Object;
   260: 	if(o->Live)
   261: 	{
   262: 		c->GuildTeam = 0;
   263: 
   264: 		if( Hero->GuildMarkIndex!=-1 && c->GuildMarkIndex==Hero->GuildMarkIndex )
   265: 			c->GuildTeam = 1;
   266: 
   267: 		if( EnableGuildWar )
   268: 		{
   269: 			if( GuildWarIndex==-1 && GuildWarName[0] )
   270: 			{
   271: 				GuildWarIndex = FindGuildName( GuildWarName );
   272: 			}
```


---

## FILE: WSclient.h

### HIT: Client: JoinMapServer / F3:03 parsing / `F3\s*:?\s*03|JoinMapServer|GCJoinMapServer|MapServer|JoinGame`

- line 8: `#include "CSMapServer.h"`

#### CONTEXT (first hit)

```
     1: #ifndef __SOCKETCLIENT_H__
     2: #define __SOCKETCLIENT_H__
     3: 
     4: #pragma once
     5: 
     6: #include "wsctlc.h"
     7: #include "SimpleModulus.h"
     8: #include "CSMapServer.h"
     9: 
    10: #define WM_ASYNCSELECTMSG (WM_USER+0)
    11: 
    12: #define MAX_CHAT_SIZE 90
    13: #define SIZE_PROTOCOLVERSION	( 5)
    14: #define SIZE_PROTOCOLSERIAL		( 16)
    15: #define MAX_GUILDNAME			8
    16: 
    17: #define	REQUEST_JOIN_SERVER      		        0
    18: #define RECEIVE_JOIN_SERVER_WAITING				1
    19: #define	RECEIVE_JOIN_SERVER_SUCCESS		        2
    20: #define	RECEIVE_JOIN_SERVER_FAIL_VERSION        3
    21: #define	REQUEST_CREATE_ACCOUNT			        10
    22: #define	RECEIVE_CREATE_ACCOUNT_SUCCESS		    11
    23: #define	RECEIVE_CREATE_ACCOUNT_FAIL_ID          12
    24: #define	RECEIVE_CREATE_ACCOUNT_FAIL_RESIDENT    13
    25: 
    26: #define	REQUEST_LOG_IN					        19
    27: #define	RECEIVE_LOG_IN_SUCCESS			        20
    28: #define	RECEIVE_LOG_IN_FAIL_PASSWORD	        21
    29: #define	RECEIVE_LOG_IN_FAIL_ID			        22
    30: #define RECEIVE_LOG_IN_FAIL_ID_CONNECTED        23
    31: #define RECEIVE_LOG_IN_FAIL_SERVER_BUSY         24
    32: #define RECEIVE_LOG_IN_FAIL_ID_BLOCK            25
    33: #define RECEIVE_LOG_IN_FAIL_VERSION				26
    34: #define RECEIVE_LOG_IN_FAIL_CONNECT				27
    35: #define RECEIVE_LOG_IN_FAIL_ERROR				28
    36: #define RECEIVE_LOG_IN_FAIL_USER_TIME1          29
    37: #define RECEIVE_LOG_IN_FAIL_USER_TIME2          30
    38: #define RECEIVE_LOG_IN_FAIL_PC_TIME1            31
    39: #define RECEIVE_LOG_IN_FAIL_PC_TIME2            32
    40: #define RECEIVE_LOG_IN_FAIL_DATE				33
    41: #define RECEIVE_LOG_IN_FAIL_POINT_DATE			34
    42: #define RECEIVE_LOG_IN_FAIL_POINT_HOUR			35
    43: #define RECEIVE_LOG_IN_FAIL_INVALID_IP			36
    44: #define RECEIVE_LOG_IN_FAIL_NO_PAYMENT_INFO		37
    45: #define RECEIVE_LOG_IN_FAIL_ONLY_OVER_15		38
    46: #define RECEIVE_LOG_IN_FAIL_CHARGED_CHANNEL		39
    47: 
    48: #define	REQUEST_CHARACTERS_LIST			        50
    49: #define	RECEIVE_CHARACTERS_LIST			        51
    50: #define	REQUEST_CREATE_CHARACTER		        52
    51: #define	RECEIVE_CREATE_CHARACTER_SUCCESS        53
    52: #define	RECEIVE_CREATE_CHARACTER_FAIL           54
    53: #define	RECEIVE_CREATE_CHARACTER_FAIL2          55
    54: #define	REQUEST_DELETE_CHARACTER	            56
    55: #define	RECEIVE_DELETE_CHARACTER_SUCCESS        57
    56: #define REQUEST_JOIN_MAP_SERVER			        60
    57: #define RECEIVE_JOIN_MAP_SERVER			        61
    58: #define	RECEIVE_CONFIRM_PASSWORD_SUCCESS        62
    59: #define	RECEIVE_CONFIRM_PASSWORD_FAIL_ID        63
    60: #define	RECEIVE_CONFIRM_PASSWORD2_SUCCESS       64
    61: #define	RECEIVE_CONFIRM_PASSWORD2_FAIL_ID       65
    62: #define	RECEIVE_CONFIRM_PASSWORD2_FAIL_ANSWER   66
    63: #define	RECEIVE_CONFIRM_PASSWORD2_FAIL_RESIDENT 67
    64: #define RECEIVE_CHANGE_PASSWORD_SUCCESS         68
    65: #define RECEIVE_CHANGE_PASSWORD_FAIL_ID         69
    66: #define RECEIVE_CHANGE_PASSWORD_FAIL_RESIDENT   70
    67: #define RECEIVE_CHANGE_PASSWORD_FAIL_PASSWORD   71
    68: 
    69: #define PACKET_ITEM_LENGTH  12
    70: 
    71: #define EQUIPMENT_LENGTH    17
    72: #define MAX_SPE_BUFFERSIZE_	( 2048)
    73: 
    74: extern int CurrentProtocolState;
    75: 
    76: typedef struct
    77: {
    78: 	BYTE Code;
    79: 	BYTE Size;
    80: 	BYTE HeadCode;
    81: } PBMSG_HEADER, * LPPBMSG_HEADER;
    82: 
    83: typedef struct
    84: {
    85: 	BYTE Code;
    86: 	BYTE SizeH;
    87: 	BYTE SizeL;
    88: 	BYTE HeadCode;
    89: } PWMSG_HEADER, * LPPWMSG_HEADER;
    90: 
    91: typedef struct
    92: {
    93: 	BYTE Code;
    94: 	BYTE Size;
    95: 	BYTE byBuffer[255];
    96: } PBMSG_ENCRYPTED, * LPPBMSG_ENCRYPTED;
    97: 
    98: typedef struct
    99: {
   100: 	BYTE Code;
   101: 	BYTE SizeH;
   102: 	BYTE SizeL;
   103: 	BYTE byBuffer[MAX_SPE_BUFFERSIZE_];
   104: } PWMSG_ENCRYPTED, * LPWBMSG_ENCRYPTED;
   105: 
   106: //request default SubCode
   107: typedef struct
   108: {
   109: 	PBMSG_HEADER Header;
   110: 	BYTE         SubCode;
   111: } PREQUEST_DEFAULT_SUBCODE, * LPPREQUEST_DEFAULT_SUBCODE;
   112: 
   113: //receive default
   114: typedef struct
   115: {
   116: 	PBMSG_HEADER Header;
   117: 	BYTE         Value;
   118: } PHEADER_DEFAULT, * LPPHEADER_DEFAULT;
   119: 
   120: typedef struct
   121: {
   122: 	PBMSG_HEADER	Header;
   123: 	BYTE			result;
   124: 	WORD			btStatValue;
   125: 	BYTE			btFruitType;
   126: } PMSG_USE_STAT_FRUIT, * LPPMSG_USE_STAT_FRUIT;
   127: 
   128: //receive default subcode
   129: typedef struct
   130: {
   131: 	PBMSG_HEADER Header;
   132: 	BYTE         SubCode;
   133: 	BYTE         Value;
   134: } PHEADER_DEFAULT_SUBCODE, * LPPHEADER_DEFAULT_SUBCODE;
   135: 
   136: //receive Character List
   137: typedef struct
   138: {
   139: #ifndef NEW_PROTOCOL_SYSTEM
   140: 	PBMSG_HEADER Header;
   141: 	BYTE         SubCode;
   142: #endif
   143:     BYTE         MaxClass;
   144: 	BYTE		 MoveCount;
   145: 	BYTE         Value;
   146: } PHEADER_DEFAULT_CHARACTER_LIST, * LPPHEADER_DEFAULT_CHARACTER_LIST;
   147: 
   148: #define CLASS_SUMMONER_CARD		0x01
   149: #define CLASS_DARK_LORD_CARD	0x02
   150: #define CLASS_DARK_CARD			0x04
   151: #define CLASS_CHARACTERCARD_TOTALCNT	3
   152: 
   153: typedef struct
   154: {
   155: 	PBMSG_HEADER	header;
   156: 	BYTE			Flag;
   157: 	BYTE			CharacterCard;
   158: } PHEADER_CHARACTERCARD, *LPPHEADER_CHARACTERCARD;
   159: 
   160: typedef struct
   161: {
   162: 	bool bCharacterEnable[CLASS_CHARACTERCARD_TOTALCNT];
   163: }CHARACTER_ENABLE, *LPCHARACTER_ENABLE;
   164: 
   165: extern CHARACTER_ENABLE g_CharCardEnable;
   166: 
   167: //receive default key
   168: typedef struct {
   169: 	PBMSG_HEADER Header;
   170: 	BYTE         KeyH;
   171: 	BYTE         KeyL;
   172: } PHEADER_DEFAULT_KEY, * LPPHEADER_DEFAULT_KEY;
   173: 
   174: //receive default key
   175: typedef struct {
   176: 	PBMSG_HEADER Header;
   177: 	BYTE         Value;
   178: 	BYTE         KeyH;
   179: 	BYTE         KeyL;
   180: } PHEADER_DEFAULT_VALUE_KEY, * LPPHEADER_DEFAULT_VALUE_KEY;
   181: 
   182: typedef struct {
   183:     PBMSG_HEADER Header;
   184:     BYTE         Value;
   185:     BYTE         KeyH;
   186:     BYTE         KeyM;
   187:     BYTE         KeyL;
   188: } PHEADER_MATCH_OPEN_VALUE, * LPPHEADER_MATCH_OPEN_VALUE;
   189: 
   190: typedef struct
   191: {
   192: 	PBMSG_HEADER Header;
   193: 	BYTE		State;
   194: 	BYTE		KeyH;
   195: 	BYTE		KeyL;
   196: 	BYTE		BuffIndex;
   197: } PMSG_VIEWSKILLSTATE, *LPPMSG_VIEWSKILLSTATE;
   198: 
   199: //receive default(word)
   200: typedef struct {
   201: 	PWMSG_HEADER Header;
   202: 	BYTE         Value;
   203: } PWHEADER_DEFAULT_WORD, * LPPWHEADER_DEFAULT_WORD;
   204: 
   205: //receive default(word)
   206: typedef struct {
   207: 	PWMSG_HEADER Header;
   208: 	INT          Value;
   209: } PWHEADER_DEFAULT_WORD2, * LPPWHEADER_DEFAULT_WORD2;
   210: 
   211: 
   212: 
   213: //receive default subcode(word)
   214: typedef struct {
   215: 	PWMSG_HEADER Header;
   216: 	BYTE         SubCode;
   217: 	BYTE         Value;
   218: 
   219: } PHEADER_DEFAULT_SUBCODE_WORD, * LPPHEADER_DEFAULT_SUBCODE_WORD;
   220: 
   221: typedef struct {
   222: 	PBMSG_HEADER Header;
   223: 	WORD        Value;
   224: } PHEADER_DEFAULT_WORD, * LPPHEADER_DEFAULT_WORD;
   225: 
   226: typedef struct {
   227: 	PBMSG_HEADER Header;
   228: 	DWORD        Value;
   229: } PHEADER_DEFAULT_DWORD, * LPPHEADER_DEFAULT_DWORD;
   230: 
   231: typedef struct {
   232: 	PBMSG_HEADER Header;
   233: 	BYTE        byBuffer[1024];
   234: } PHEADER_DEFAULT_CUSTOM, * LPPHEADER_DEFAULT_CUSTOM;
   235: 
   236: 
   237: typedef struct {
   238: 	PBMSG_HEADER  Header;
   239: 	BYTE          Index;
   240: 	BYTE          Item[PACKET_ITEM_LENGTH];
   241: } PHEADER_DEFAULT_ITEM, * LPPHEADER_DEFAULT_ITEM;
   242: 
   243: typedef struct {
   244: 	PBMSG_HEADER  Header;
   245: 	BYTE          SubCode;
   246: 	BYTE          Index;
   247: 	BYTE          Item[PACKET_ITEM_LENGTH];
   248: } PHEADER_DEFAULT_SUBCODE_ITEM, * LPPHEADER_DEFAULT_SUBCODE_ITEM;
```

### HIT: Magic (0x19) tokens / `case\s+0x19\b|\b0x19\b|ReceiveMagic|SKILL_ATTACK_SEND|PMSG_SKILL_ATTACK_SEND`

- line 1089: `	GUILD_ANS_NOTEXIST_UNION			= 0x19,`

#### CONTEXT (first hit)

```
  1019: } PRECEIVE_GUILD_LIST, * LPPRECEIVE_GUILD_LIST;
  1020: 
  1021: //   Ʈ
  1022: typedef struct {
  1023: 	PWMSG_HEADER Header;
  1024: 	BYTE         Result;
  1025: 	BYTE         Count;
  1026: 	DWORD        TotalScore;
  1027: 	BYTE         Score;
  1028: 	char		szRivalGuildName[MAX_GUILDNAME];
  1029: } PRECEIVE_GUILD_LISTS, * LPPRECEIVE_GUILD_LISTS;
  1030: 
  1031: //send guild leave
  1032: typedef struct {
  1033: 	PBMSG_HEADER  Header;
  1034: 	BYTE          ID[MAX_ID_SIZE];
  1035: 	BYTE          ResidentNumber[10];
  1036: } PSEND_GUILD_LEAVE, * LPPSEND_GUILD_LEAVE;
  1037: 
  1038: typedef struct {
  1039: 	PBMSG_HEADER Header;
  1040: 	BYTE         Name[8];
  1041: } PSEND_GUILD_WAR, * LPPSEND_GUILD_WAR;
  1042: 
  1043: //receive guild war
  1044: typedef struct {
  1045: 	PBMSG_HEADER Header;
  1046: 	BYTE         Name[8];
  1047: 	BYTE         Type;
  1048: 	BYTE         Team;
  1049: } PRECEIVE_WAR, * LPPRECEIVE_WAR;
  1050: 
  1051: typedef struct {
  1052: 	PBMSG_HEADER Header;
  1053: 	BYTE         Score1;
  1054: 	BYTE         Score2;
  1055: 	BYTE         Type;
  1056: } PRECEIVE_WAR_SCORE, * LPPRECEIVE_WAR_SCORE;
  1057: 
  1058: typedef struct
  1059: {
  1060: 	int			 GuildKey;
  1061: 	BYTE		 GuildStatus;
  1062: 	BYTE		 GuildType;
  1063: 	BYTE		 GuildRelationShip;
  1064: 	BYTE		 KeyH;
  1065: 	BYTE		 KeyL;
  1066: } PRECEIVE_GUILD_ID, *LPPRECEIVE_GUILD_ID;
  1067: 
  1068: typedef struct
  1069: {
  1070: 	PBMSG_HEADER Header;
  1071: 	int			GuildKey;
  1072: 	BYTE		GuildType;
  1073: 	BYTE		UnionName[MAX_GUILDNAME];
  1074: 	BYTE		GuildName[MAX_GUILDNAME];
  1075: 	BYTE		Mark[32];
  1076: } PPRECEIVE_GUILDINFO, *LPPPRECEIVE_GUILDINFO;
  1077: 
  1078: enum GUILD_REQ_COMMON_RESULT
  1079: {
  1080: 	GUILD_ANS_NOTEXIST_GUILD			= 0x10,
  1081: 	GUILD_ANS_UNIONFAIL_BY_CASTLE		= 0x10,
  1082: 	GUILD_ANS_NOTEXIST_PERMISSION		= 0x11,
  1083: 	GUILD_ANS_NOTEXIST_EXTRA_STATUS		= 0x12,
  1084: 	GUILD_ANS_NOTEXIST_EXTRA_TYPE		= 0x13,
  1085: 	GUILD_ANS_EXIST_RELATIONSHIP_UNION	= 0x15,
  1086: 	GUILD_ANS_EXIST_RELATIONSHIP_RIVAL	= 0x16,
  1087: 	GUILD_ANS_EXIST_UNION				= 0x17,
  1088: 	GUILD_ANS_EXIST_RIVAL				= 0x18,
  1089: 	GUILD_ANS_NOTEXIST_UNION			= 0x19,
  1090: 	GUILD_ANS_NOTEXIST_RIVAL			= 0x1A,
  1091: 	GUILD_ANS_NOT_UNION_MASTER			= 0x1B,
  1092: 	GUILD_ANS_NOT_GUILD_RIVAL			= 0x1C,
  1093: 	GUILD_ANS_CANNOT_BE_UNION_MASTER_GUILD = 0x1D,
  1094: 	GUILD_ANS_EXCEED_MAX_UNION_MEMBER	= 0x1E,
  1095: 	GUILD_ANS_CANCEL_REQUEST			= 0x20,
  1096: 	GUILD_ANS_UNION_MASTER_NOT_GENS		= 0xA1,
  1097: 	GUILD_ANS_GUILD_MASTER_NOT_GENS		= 0xA2,
  1098: 	GUILD_ANS_UNION_MASTER_DISAGREE_GENS = 0xA3,
  1099: };
  1100: 
  1101: typedef struct
  1102: {
  1103: 	PBMSG_HEADER	Header;
  1104: 	BYTE			Value;
  1105: 	BYTE			GuildType;
  1106: } PMSG_GUILD_CREATE_RESULT, *LPPMSG_GUILD_CREATE_RESULT;
  1107: 
  1108: typedef struct
  1109: {
  1110: 	PBMSG_HEADER	Header;
  1111: 	BYTE			byGuildType;
  1112: 	BYTE			byResult;
  1113: } PMSG_GUILD_ASSIGN_TYPE_RESULT, *LPPMSG_GUILD_ASSIGN_TYPE_RESULT;
  1114: 
  1115: typedef struct
  1116: {
  1117: 	PBMSG_HEADER	Header;
  1118: 	BYTE			byType;
  1119: 	BYTE			byResult;
  1120: 	char			szTargetName[MAX_ID_SIZE];
  1121: } PRECEIVE_GUILD_ASSIGN, *LPPRECEIVE_GUILD_ASSIGN;
  1122: 
  1123: typedef struct
  1124: {
  1125: 	PWMSG_HEADER	Header;
  1126: 	BYTE			byCount;
  1127: 	BYTE			byResult;
  1128: 	BYTE			byRivalCount;
  1129: 	BYTE			byUnionCount;
  1130: } PMSG_UNIONLIST_COUNT, *LPPMSG_UNIONLIST_COUNT;
  1131: 
  1132: typedef struct
  1133: {
  1134: 	BYTE			byMemberCount;
  1135: 	BYTE			GuildMark[32];
  1136: 	char			szGuildName[MAX_GUILDNAME];
  1137: } PMSG_UNIONLIST, *LPPMSG_UNIONLIST;
  1138: 
  1139: typedef struct
  1140: {
  1141: 	PBMSG_HEADER	Header;
  1142: 	BYTE			byRelationShipType;
  1143: 	BYTE			byRequestType;
  1144: 	BYTE			byTargetUserIndexH;
  1145: 	BYTE			byTargetUserIndexL;
  1146: } PMSG_GUILD_RELATIONSHIP, *LPPMSG_GUILD_RELATIONSHIP;
  1147: 
  1148: typedef struct
  1149: {
  1150: 	PBMSG_HEADER	Header;
  1151: 	BYTE			byRelationShipType;
  1152: 	BYTE			byRequestType;
  1153: 	BYTE			byResult;
  1154: 	BYTE			byTargetUserIndexH;
  1155: 	BYTE			byTargetUserIndexL;
  1156: } PMSG_GUILD_RELATIONSHIP_RESULT, *LPPMSG_GUILD_RELATIONSHIP_RESULT;
  1157: 
  1158: typedef struct
  1159: {
  1160: 	PREQUEST_DEFAULT_SUBCODE	Header;
  1161: 	BYTE			byResult;
  1162: 	BYTE			byRequestType;
  1163: 	BYTE			byRelationShipType;
  1164: } PMSG_BAN_UNIONGUILD, *LPPMSG_BAN_UNIONGUILD;
  1165: 
  1166: typedef struct
  1167: {
  1168: 	PWMSG_HEADER	Header;
  1169: 	BYTE			byCount;
  1170: } PMSG_UNION_VIEWPORT_NOTIFY_COUNT, *LPPMSG_UNION_VIEWPORT_NOTIFY_COUNT;
  1171: typedef struct {
  1172: 	BYTE			byKeyH;
  1173: 	BYTE			byKeyL;
  1174: 	int				nGuildKey;
  1175: 	BYTE			byGuildRelationShip;
  1176: 	char			szUnionName[MAX_GUILDNAME];
  1177: } PMSG_UNION_VIEWPORT_NOTIFY, *LPPMSG_UNION_VIEWPORT_NOTIFY;
  1178: 
  1179: //receive gold
  1180: typedef struct
  1181: {
  1182: 	PBMSG_HEADER Header;
  1183: 	BYTE         Result;
  1184: 	DWORD        StorageGold;
  1185: 	DWORD        Gold;
  1186: } PRECEIVE_STORAGE_GOLD, * LPPRECEIVE_STORAGE_GOLD;
  1187: 
  1188: //receive soccer time
  1189: typedef struct {
  1190: 	PBMSG_HEADER Header;
  1191: 	BYTE         SubCode;
  1192: 	WORD         Time;
  1193: } PRECEIVE_SOCCER_TIME, * LPPRECEIVE_SOCCER_TIME;
  1194: 
  1195: typedef struct {
  1196: 	PBMSG_HEADER Header;
  1197: 	BYTE         SubCode;
  1198: 	BYTE         Name1[8];
  1199: 	BYTE         Score1;
  1200: 	BYTE         Name2[8];
  1201: 	BYTE         Score2;
  1202: } PRECEIVE_SOCCER_SCORE, * LPPRECEIVE_SOCCER_SCORE;
  1203: 
  1204: #pragma pack(push, 1)
  1205: typedef struct
  1206: {
  1207:     PBMSG_HEADER Header;
  1208:     BYTE         SubCode;
  1209: 	BYTE         HotKey[20];
  1210:     BYTE         GameOption;
  1211:     BYTE         KeyQWE[3];
  1212: 	BYTE		 ChatLogBox;
  1213: 	BYTE		 KeyR;
  1214: 	int			 QWERLevel;
  1215: } PRECEIVE_OPTION, * LPPRECEIVE_OPTION;
  1216: #pragma pack(pop)
  1217: 
  1218: typedef struct
  1219: {
  1220: 	PBMSG_HEADER Header;
  1221: 	BYTE         SubCode;
  1222: 	BYTE		 Cmd1;
  1223: 	BYTE		 Cmd2;
  1224: 	BYTE		 Cmd3;
  1225: } PRECEIVE_SERVER_COMMAND, * LPPRECEIVE_SERVER_COMMAND;
  1226: 
  1227: 
  1228: typedef struct
  1229: {
  1230: 	PBMSG_HEADER Header;
  1231: 	BYTE		 m_MyRank;
  1232: 	BYTE         m_Count;
  1233: 	BYTE		 m_byRank;
  1234: } PDEVILRANK, * LPPDEVILRANK;
  1235: 
  1236: typedef struct
  1237: {
  1238:     PBMSG_HEADER Header;
  1239:     BYTE        m_byType;
  1240: 	int			m_nChipCount;
  1241:     short       m_shMutoNum[3];
  1242: } PRECEIVE_EVENT_CHIP_INFO, * LPPRECEIVE_EVENT_CHIP_INFO;
  1243: 
  1244: typedef struct
  1245: {
  1246:     PBMSG_HEADER Header;
  1247:     BYTE        m_byType;
  1248: 	unsigned int  m_unChipCount;
  1249: } PRECEIVE_ENVET_CHIP, * LPPRECEIVE_EVENT_CHIP;
  1250: 
  1251: typedef struct
  1252: {
  1253:     PBMSG_HEADER Header;
  1254:     short        m_shMutoNum[3];
  1255: } PRECEIVE_MUTONUMBER, * LPPRECEIVE_MUTONUMBER;
  1256: 
  1257: typedef struct
  1258: {
  1259: 	PBMSG_HEADER Header;
  1260: 	BYTE		 m_byIsRegistered;
  1261: 	char		 m_strGiftName[64];
  1262: } PRECEIVE_SCRATCH_TICKET_EVENT, *LPPRECEIVE_SCRATCH_TICKET_EVENT;
  1263: 
  1264: typedef struct {
  1265: 	PBMSG_HEADER Header;
  1266: 	WORD		 wEffectNum;
  1267: } PRECEIVE_PLAY_SOUND_EFFECT, *LPPRECEIVE_PLAY_SOUND_EFFECT;
  1268: 
  1269: typedef struct {
  1270: 	PBMSG_HEADER Header;
  1271: 	BYTE         m_byValue;
  1272:     BYTE         m_byNumber;
  1273: } PHEADER_EVENT, * LPPHEADER_EVENT;
  1274: 
  1275: typedef struct {
  1276:     PBMSG_HEADER    Header;
  1277:     BYTE            m_wEventType;
  1278:     BYTE            m_wLeftEnterCount;
  1279: }PRECEIVE_EVENT_COUNT, *LPPRECEIVE_EVENT_COUNT;
  1280: 
  1281: typedef struct {
  1282:     PBMSG_HEADER Header;
  1283:     BYTE         m_byCount;
  1284:     BYTE         m_byQuest[50];
  1285: } PRECEIVE_QUEST_HISTORY, * LPPRECEIVE_QUEST_HISTORY;
  1286: 
  1287: typedef struct {
  1288:     PBMSG_HEADER Header;
  1289:     BYTE         m_byQuestIndex;
  1290:     BYTE         m_byState;
  1291: } PRECEIVE_QUEST_STATE, * LPPRECEIVE_QUEST_STATE;
  1292: 
  1293: typedef struct {
  1294:     PBMSG_HEADER Header;
  1295:     BYTE         m_byQuestIndex;
  1296:     BYTE         m_byResult;
  1297:     BYTE         m_byState;
  1298: } PRECEIVE_QUEST_RESULT, * LPPRECEIVE_QUEST_RESULT;
  1299: 
  1300: typedef struct {
  1301:     PBMSG_HEADER Header;
  1302:     BYTE         m_byKeyH;
  1303:     BYTE         m_byKeyL;
  1304:     BYTE         m_byReparation;
  1305:     BYTE         m_byNumber;
  1306: } PRECEIVE_QUEST_REPARATION, * LPPRECEIVE_QUEST_REPARATION;
  1307: 
  1308: // GC[0xF6][0x0A]
  1309: typedef struct
  1310: {
  1311: 	PBMSG_HEADER	Header;
  1312: 	BYTE			SubCode;
  1313: 
  1314: 	WORD			m_wNPCIndex;
  1315: 	WORD			m_wQuestCount;
  1316: } PMSG_NPCTALK_QUESTLIST, *LPPMSG_NPCTALK_QUESTLIST;
  1317: 
  1318: #pragma pack(push, 1)
  1319: typedef struct
  1320: {
  1321: 	PBMSG_HEADER	Header;
  1322: 	BYTE			SubCode;
  1323: 	DWORD			m_dwQuestIndex;
  1324: 	BYTE			m_byRequestCount;
  1325: 	BYTE			m_byRewardCount;
  1326: 	BYTE			m_byRandRewardCount;
  1327: } PMSG_NPC_QUESTEXP_INFO, *LPPMSG_NPC_QUESTEXP_INFO;
  1328: #pragma pack(pop)
  1329: 
```

### HIT: Server: F3:03 sending / `F3\s*:?\s*03|PHeadSubSet|PHeadSet|JoinMapServer|GCJoinMapServer|MapServer`

- line 8: `#include "CSMapServer.h"`

#### CONTEXT (first hit)

```
     1: #ifndef __SOCKETCLIENT_H__
     2: #define __SOCKETCLIENT_H__
     3: 
     4: #pragma once
     5: 
     6: #include "wsctlc.h"
     7: #include "SimpleModulus.h"
     8: #include "CSMapServer.h"
     9: 
    10: #define WM_ASYNCSELECTMSG (WM_USER+0)
    11: 
    12: #define MAX_CHAT_SIZE 90
    13: #define SIZE_PROTOCOLVERSION	( 5)
    14: #define SIZE_PROTOCOLSERIAL		( 16)
    15: #define MAX_GUILDNAME			8
    16: 
    17: #define	REQUEST_JOIN_SERVER      		        0
    18: #define RECEIVE_JOIN_SERVER_WAITING				1
    19: #define	RECEIVE_JOIN_SERVER_SUCCESS		        2
    20: #define	RECEIVE_JOIN_SERVER_FAIL_VERSION        3
    21: #define	REQUEST_CREATE_ACCOUNT			        10
    22: #define	RECEIVE_CREATE_ACCOUNT_SUCCESS		    11
    23: #define	RECEIVE_CREATE_ACCOUNT_FAIL_ID          12
    24: #define	RECEIVE_CREATE_ACCOUNT_FAIL_RESIDENT    13
    25: 
    26: #define	REQUEST_LOG_IN					        19
    27: #define	RECEIVE_LOG_IN_SUCCESS			        20
    28: #define	RECEIVE_LOG_IN_FAIL_PASSWORD	        21
    29: #define	RECEIVE_LOG_IN_FAIL_ID			        22
    30: #define RECEIVE_LOG_IN_FAIL_ID_CONNECTED        23
    31: #define RECEIVE_LOG_IN_FAIL_SERVER_BUSY         24
    32: #define RECEIVE_LOG_IN_FAIL_ID_BLOCK            25
    33: #define RECEIVE_LOG_IN_FAIL_VERSION				26
    34: #define RECEIVE_LOG_IN_FAIL_CONNECT				27
    35: #define RECEIVE_LOG_IN_FAIL_ERROR				28
    36: #define RECEIVE_LOG_IN_FAIL_USER_TIME1          29
    37: #define RECEIVE_LOG_IN_FAIL_USER_TIME2          30
    38: #define RECEIVE_LOG_IN_FAIL_PC_TIME1            31
    39: #define RECEIVE_LOG_IN_FAIL_PC_TIME2            32
    40: #define RECEIVE_LOG_IN_FAIL_DATE				33
    41: #define RECEIVE_LOG_IN_FAIL_POINT_DATE			34
    42: #define RECEIVE_LOG_IN_FAIL_POINT_HOUR			35
    43: #define RECEIVE_LOG_IN_FAIL_INVALID_IP			36
    44: #define RECEIVE_LOG_IN_FAIL_NO_PAYMENT_INFO		37
    45: #define RECEIVE_LOG_IN_FAIL_ONLY_OVER_15		38
    46: #define RECEIVE_LOG_IN_FAIL_CHARGED_CHANNEL		39
    47: 
    48: #define	REQUEST_CHARACTERS_LIST			        50
    49: #define	RECEIVE_CHARACTERS_LIST			        51
    50: #define	REQUEST_CREATE_CHARACTER		        52
    51: #define	RECEIVE_CREATE_CHARACTER_SUCCESS        53
    52: #define	RECEIVE_CREATE_CHARACTER_FAIL           54
    53: #define	RECEIVE_CREATE_CHARACTER_FAIL2          55
    54: #define	REQUEST_DELETE_CHARACTER	            56
    55: #define	RECEIVE_DELETE_CHARACTER_SUCCESS        57
    56: #define REQUEST_JOIN_MAP_SERVER			        60
    57: #define RECEIVE_JOIN_MAP_SERVER			        61
    58: #define	RECEIVE_CONFIRM_PASSWORD_SUCCESS        62
    59: #define	RECEIVE_CONFIRM_PASSWORD_FAIL_ID        63
    60: #define	RECEIVE_CONFIRM_PASSWORD2_SUCCESS       64
    61: #define	RECEIVE_CONFIRM_PASSWORD2_FAIL_ID       65
    62: #define	RECEIVE_CONFIRM_PASSWORD2_FAIL_ANSWER   66
    63: #define	RECEIVE_CONFIRM_PASSWORD2_FAIL_RESIDENT 67
    64: #define RECEIVE_CHANGE_PASSWORD_SUCCESS         68
    65: #define RECEIVE_CHANGE_PASSWORD_FAIL_ID         69
    66: #define RECEIVE_CHANGE_PASSWORD_FAIL_RESIDENT   70
    67: #define RECEIVE_CHANGE_PASSWORD_FAIL_PASSWORD   71
    68: 
    69: #define PACKET_ITEM_LENGTH  12
    70: 
    71: #define EQUIPMENT_LENGTH    17
    72: #define MAX_SPE_BUFFERSIZE_	( 2048)
    73: 
    74: extern int CurrentProtocolState;
    75: 
    76: typedef struct
    77: {
    78: 	BYTE Code;
    79: 	BYTE Size;
    80: 	BYTE HeadCode;
    81: } PBMSG_HEADER, * LPPBMSG_HEADER;
    82: 
    83: typedef struct
    84: {
    85: 	BYTE Code;
    86: 	BYTE SizeH;
    87: 	BYTE SizeL;
    88: 	BYTE HeadCode;
    89: } PWMSG_HEADER, * LPPWMSG_HEADER;
    90: 
    91: typedef struct
    92: {
    93: 	BYTE Code;
    94: 	BYTE Size;
    95: 	BYTE byBuffer[255];
    96: } PBMSG_ENCRYPTED, * LPPBMSG_ENCRYPTED;
    97: 
    98: typedef struct
    99: {
   100: 	BYTE Code;
   101: 	BYTE SizeH;
   102: 	BYTE SizeL;
   103: 	BYTE byBuffer[MAX_SPE_BUFFERSIZE_];
   104: } PWMSG_ENCRYPTED, * LPWBMSG_ENCRYPTED;
   105: 
   106: //request default SubCode
   107: typedef struct
   108: {
   109: 	PBMSG_HEADER Header;
   110: 	BYTE         SubCode;
   111: } PREQUEST_DEFAULT_SUBCODE, * LPPREQUEST_DEFAULT_SUBCODE;
   112: 
   113: //receive default
   114: typedef struct
   115: {
   116: 	PBMSG_HEADER Header;
   117: 	BYTE         Value;
   118: } PHEADER_DEFAULT, * LPPHEADER_DEFAULT;
   119: 
   120: typedef struct
   121: {
   122: 	PBMSG_HEADER	Header;
   123: 	BYTE			result;
   124: 	WORD			btStatValue;
   125: 	BYTE			btFruitType;
   126: } PMSG_USE_STAT_FRUIT, * LPPMSG_USE_STAT_FRUIT;
   127: 
   128: //receive default subcode
   129: typedef struct
   130: {
   131: 	PBMSG_HEADER Header;
   132: 	BYTE         SubCode;
   133: 	BYTE         Value;
   134: } PHEADER_DEFAULT_SUBCODE, * LPPHEADER_DEFAULT_SUBCODE;
   135: 
   136: //receive Character List
   137: typedef struct
   138: {
   139: #ifndef NEW_PROTOCOL_SYSTEM
   140: 	PBMSG_HEADER Header;
   141: 	BYTE         SubCode;
   142: #endif
   143:     BYTE         MaxClass;
   144: 	BYTE		 MoveCount;
   145: 	BYTE         Value;
   146: } PHEADER_DEFAULT_CHARACTER_LIST, * LPPHEADER_DEFAULT_CHARACTER_LIST;
   147: 
   148: #define CLASS_SUMMONER_CARD		0x01
   149: #define CLASS_DARK_LORD_CARD	0x02
   150: #define CLASS_DARK_CARD			0x04
   151: #define CLASS_CHARACTERCARD_TOTALCNT	3
   152: 
   153: typedef struct
   154: {
   155: 	PBMSG_HEADER	header;
   156: 	BYTE			Flag;
   157: 	BYTE			CharacterCard;
   158: } PHEADER_CHARACTERCARD, *LPPHEADER_CHARACTERCARD;
   159: 
   160: typedef struct
   161: {
   162: 	bool bCharacterEnable[CLASS_CHARACTERCARD_TOTALCNT];
   163: }CHARACTER_ENABLE, *LPCHARACTER_ENABLE;
   164: 
   165: extern CHARACTER_ENABLE g_CharCardEnable;
   166: 
   167: //receive default key
   168: typedef struct {
   169: 	PBMSG_HEADER Header;
   170: 	BYTE         KeyH;
   171: 	BYTE         KeyL;
   172: } PHEADER_DEFAULT_KEY, * LPPHEADER_DEFAULT_KEY;
   173: 
   174: //receive default key
   175: typedef struct {
   176: 	PBMSG_HEADER Header;
   177: 	BYTE         Value;
   178: 	BYTE         KeyH;
   179: 	BYTE         KeyL;
   180: } PHEADER_DEFAULT_VALUE_KEY, * LPPHEADER_DEFAULT_VALUE_KEY;
   181: 
   182: typedef struct {
   183:     PBMSG_HEADER Header;
   184:     BYTE         Value;
   185:     BYTE         KeyH;
   186:     BYTE         KeyM;
   187:     BYTE         KeyL;
   188: } PHEADER_MATCH_OPEN_VALUE, * LPPHEADER_MATCH_OPEN_VALUE;
   189: 
   190: typedef struct
   191: {
   192: 	PBMSG_HEADER Header;
   193: 	BYTE		State;
   194: 	BYTE		KeyH;
   195: 	BYTE		KeyL;
   196: 	BYTE		BuffIndex;
   197: } PMSG_VIEWSKILLSTATE, *LPPMSG_VIEWSKILLSTATE;
   198: 
   199: //receive default(word)
   200: typedef struct {
   201: 	PWMSG_HEADER Header;
   202: 	BYTE         Value;
   203: } PWHEADER_DEFAULT_WORD, * LPPWHEADER_DEFAULT_WORD;
   204: 
   205: //receive default(word)
   206: typedef struct {
   207: 	PWMSG_HEADER Header;
   208: 	INT          Value;
   209: } PWHEADER_DEFAULT_WORD2, * LPPWHEADER_DEFAULT_WORD2;
   210: 
   211: 
   212: 
   213: //receive default subcode(word)
   214: typedef struct {
   215: 	PWMSG_HEADER Header;
   216: 	BYTE         SubCode;
   217: 	BYTE         Value;
   218: 
   219: } PHEADER_DEFAULT_SUBCODE_WORD, * LPPHEADER_DEFAULT_SUBCODE_WORD;
   220: 
   221: typedef struct {
   222: 	PBMSG_HEADER Header;
   223: 	WORD        Value;
   224: } PHEADER_DEFAULT_WORD, * LPPHEADER_DEFAULT_WORD;
   225: 
   226: typedef struct {
   227: 	PBMSG_HEADER Header;
   228: 	DWORD        Value;
   229: } PHEADER_DEFAULT_DWORD, * LPPHEADER_DEFAULT_DWORD;
   230: 
   231: typedef struct {
   232: 	PBMSG_HEADER Header;
   233: 	BYTE        byBuffer[1024];
   234: } PHEADER_DEFAULT_CUSTOM, * LPPHEADER_DEFAULT_CUSTOM;
   235: 
   236: 
   237: typedef struct {
   238: 	PBMSG_HEADER  Header;
   239: 	BYTE          Index;
   240: 	BYTE          Item[PACKET_ITEM_LENGTH];
   241: } PHEADER_DEFAULT_ITEM, * LPPHEADER_DEFAULT_ITEM;
   242: 
   243: typedef struct {
   244: 	PBMSG_HEADER  Header;
   245: 	BYTE          SubCode;
   246: 	BYTE          Index;
   247: 	BYTE          Item[PACKET_ITEM_LENGTH];
   248: } PHEADER_DEFAULT_SUBCODE_ITEM, * LPPHEADER_DEFAULT_SUBCODE_ITEM;
```


---

## FILE: GS_Protocol.cpp

### HIT: Client: server version tokens / `m_ServerVersion|ServerVersion|gServerInfo|GAMESERVER_UPDATE|>=\s*701|\b701\b`

- line 211: `			#if(GAMESERVER_UPDATE>=601)`
- line 216: `			#if(GAMESERVER_UPDATE>=601)`
- line 224: `					#if(GAMESERVER_UPDATE>=801)`
- line 229: `					#if(GAMESERVER_UPDATE>=801)`
- line 234: `					#if(GAMESERVER_UPDATE>=801)`
- line 244: `					#if(GAMESERVER_UPDATE>=802)`
- line 249: `					#if(GAMESERVER_UPDATE>=802)`
- line 254: `					#if(GAMESERVER_UPDATE>=802)`
- line 259: `					#if(GAMESERVER_UPDATE>=802)`
- line 264: `					#if(GAMESERVER_UPDATE>=802)`
- line 269: `					#if(GAMESERVER_UPDATE>=802)`
- line 274: `					#if(GAMESERVER_UPDATE>=802)`
- line 279: `					#if(GAMESERVER_UPDATE>=802)`
- line 284: `					#if(GAMESERVER_UPDATE>=802)`
- line 294: `					#if(GAMESERVER_UPDATE>=803)`
- line 301: `					#if(GAMESERVER_UPDATE>=803)`

#### CONTEXT (first hit)

```
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
   354: 			break;
   355: 		case 0x8E:
   356: 			gMove.CGTeleportMoveRecv((PMSG_TELEPORT_MOVE_RECV*)lpMsg,aIndex);
   357: 			break;
   358: 		case 0x90:
   359: 			gDevilSquare.CGDevilSquareEnterRecv((PMSG_DEVIL_SQUARE_ENTER_RECV*)lpMsg,aIndex);
   360: 			break;
   361: 		case 0x91:
   362: 			CGEventRemainTimeRecv((PMSG_EVENT_REMAIN_TIME_RECV*)lpMsg,aIndex);
   363: 			break;
   364: 		case 0x9A:
   365: 			gBloodCastle.CGBloodCastleEnterRecv((PMSG_BLOOD_CASTLE_ENTER_RECV*)lpMsg,aIndex);
   366: 			break;
   367: 		case 0xA0:
   368: 			gQuest.CGQuestInfoRecv(aIndex);
   369: 			break;
   370: 		case 0xA2:
   371: 			gQuest.CGQuestStateRecv((PMSG_QUEST_STATE_RECV*)lpMsg,aIndex);
   372: 			break;
   373: 		case 0xA7:
   374: 			CGPetItemCommandRecv((PMSG_PET_ITEM_COMMAND_RECV*)lpMsg,aIndex);
   375: 			break;
   376: 		case 0xA9:
   377: 			CGPetItemInfoRecv((PMSG_PET_ITEM_INFO_RECV*)lpMsg,aIndex);
   378: 			break;
   379: 		case 0xAA:
   380: 			#if(GAMESERVER_UPDATE>=402)
   381: 			switch(lpMsg[3])
   382: 			{
   383: 				case 0x01:
   384: 					gDuel.CGDuelStartRecv((PMSG_DUEL_START_RECV*)lpMsg,aIndex);
   385: 					break;
   386: 				case 0x02:
   387: 					gDuel.CGDuelOkRecv((PMSG_DUEL_OK_RECV*)lpMsg,aIndex);
   388: 					break;
   389: 				case 0x03:
   390: 					//gDuel.CGDuelEndRecv(aIndex);
   391: 					break;
   392: 				case 0x07:
   393: 					gDuel.CGDuelWatchRecv((PMSG_DUEL_WATCH_RECV*)lpMsg,aIndex);
   394: 					break;
   395: 				case 0x09:
   396: 					gDuel.CGDuelButtonRecv((PMSG_DUEL_BUTTON_RECV*)lpMsg,aIndex);
   397: 					break;
   398: 			}
   399: 			#else
   400: 			gDuel.CGDuelStartRecv((PMSG_DUEL_START_RECV*)lpMsg,aIndex);
   401: 			#endif
   402: 			break;
   403: 		case 0xAB:
   404: 			#if(GAMESERVER_UPDATE<=401)
   405: 			gDuel.CGDuelEndRecv(aIndex);
   406: 			#endif
   407: 			break;
   408: 		case 0xAC:
   409: 			#if(GAMESERVER_UPDATE<=401)
   410: 			gDuel.CGDuelOkRecv((PMSG_DUEL_OK_RECV*)lpMsg,aIndex);
   411: 			#endif
   412: 			break;
   413: 		case 0xAE:
   414: 			#if(GAMESERVER_UPDATE>=603)
   415: 			gHelper.CGHelperDataRecv((PMSG_HELPER_DATA_RECV*)lpMsg,aIndex);
   416: 			#endif
   417: 			break;
   418: 		case 0xAF:
   419: 			switch(lpMsg[3])
   420: 			{
   421: 				case 0x01:
   422: 					gChaosCastle.CGChaosCastleEnterRecv((PMSG_CHAOS_CASTLE_ENTER_RECV*)lpMsg,aIndex);
   423: 					break;
   424: 				case 0x02:
   425: 					gChaosCastle.CGChaosCastlePositionRecv((PMSG_CHAOS_CASTLE_POSITION_RECV*)lpMsg,aIndex);
   426: 					break;
   427: 			}
   428: 			break;
   429: 		case 0xB0:
   430: 			gSkillManager.CGSkillTeleportAllyRecv((PMSG_SKILL_TELEPORT_ALLY_RECV*)lpMsg,aIndex);
   431: 			break;
   432: 		case 0xB1:
   433: 			switch(lpMsg[3])
   434: 			{
   435: 				case 0x01:
   436: 					CGMapServerMoveAuthRecv((PMSG_MAP_SERVER_MOVE_AUTH_RECV*)lpMsg,aIndex);
   437: 					break;
   438: 			}
   439: 			break;
   440: 		case 0xB2:
   441: 			switch(lpMsg[3])
   442: 			{
   443: 				case 0x00:
   444: 					CGReqCastleSiegeState((PMSG_REQ_CASTLESIEGESTATE*)lpMsg,aIndex);
   445: 					break;
   446: 				case 0x01:
   447: 					CGReqRegCastleSiege((PMSG_REQ_REGCASTLESIEGE*)lpMsg,aIndex);
   448: 					break;
   449: 				case 0x02:
   450: 					CGReqGiveUpCastleSiege((PMSG_REQ_GIVEUPCASTLESIEGE*)lpMsg,aIndex);
   451: 					break;
```

### HIT: Client: JoinMapServer / F3:03 parsing / `F3\s*:?\s*03|JoinMapServer|GCJoinMapServer|MapServer|JoinGame`

- line 50: `#include "MapServerManager.h"`
- line 436: `					CGMapServerMoveAuthRecv((PMSG_MAP_SERVER_MOVE_AUTH_RECV*)lpMsg,aIndex);`
- line 1758: `void CGMapServerMoveAuthRecv(PMSG_MAP_SERVER_MOVE_AUTH_RECV* lpMsg,int aIndex) // OK`
- line 1770: `		GCMapServerMoveAuthSend(aIndex,6);`
- line 1776: `		GCMapServerMoveAuthSend(aIndex,6);`
- line 1792: `		lpObj->MapServerMoveRequest = 1;`
- line 1794: `		GJMapServerMoveAuthSend(aIndex,account,lpMsg->name,lpMsg->AuthCode1,lpMsg->AuthCode2,lpMsg->AuthCode3,lpMsg->AuthCode4);`
- line 1998: `		lpObj->MapServerMoveRequest = 0;`
- line 2912: `void GCMapServerMoveAuthSend(int aIndex,BYTE result) // OK`
- line 3663: `	GS_GDReqOwnerGuildMaster(gMapServerManager.GetMapServerGroup(), iIndex);`
- line 3757: `		GS_GDReqRegAttackGuild(gMapServerManager.GetMapServerGroup(), iIndex);`
- line 3809: `	GS_GDReqGuildSetGiveUp(gMapServerManager.GetMapServerGroup(), iIndex, lpMsg->btGiveUp);`
- line 3866: `	GS_GDReqGuildMarkRegInfo(gMapServerManager.GetMapServerGroup(), iIndex);`
- line 3965: `				GS_GDReqRegGuildMark(gMapServerManager.GetMapServerGroup(), iIndex, iItemPos);`
- line 4175: `					GS_GDReqCastleNpcRepair(gMapServerManager.GetMapServerGroup(), iIndex, lpMsg->iNpcNumber, lpMsg->iNpcIndex, iRepairCost);`
- line 4434: `					GS_GDReqCastleNpcUpgrade(gMapServerManager.GetMapServerGroup(),iIndex,lpMsg->iNpcNumber,lpMsg->iNpcIndex,lpMsg->iNpcUpType,iNXT_UPVALUE,(iNXT_UPLVL-1));`

#### CONTEXT (first hit)

```
     1: #include "stdafx.h"
     2: #include "Protocol.h"
     3: #include "ArcaBattle.h"
     4: #include "Attack.h"
     5: #include "BloodCastle.h"
     6: #include "CashShop.h"
     7: #include "CastleSiege.h"
     8: #include "CastleSiegeSync.h"
     9: #include "CastleSiegeWeapon.h"
    10: #include "ChaosBox.h"
    11: #include "ChaosCastle.h"
    12: #include "CommandManager.h"
    13: #include "CustomBuyVip.h"
    14: #include "CustomArena.h"
    15: #include "CustomCommandDescription.h"
    16: #include "CustomEventTime.h"
    17: #include "CustomExchangeCoin.h"
    18: #include "CustomMove.h"
    19: #include "CustomStore.h"
    20: #include "CustomRanking.h"
    21: #include "Crywolf.h"
    22: #include "CSProtocol.h"
    23: #include "DarkSpirit.h"
    24: #include "DefaultClassInfo.h"
    25: #include "DevilSquare.h"
    26: #include "DoubleGoer.h"
    27: #include "DSProtocol.h"
    28: #include "Duel.h"
    29: #include "EffectManager.h"
    30: #include "ESProtocol.h"
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
```

### HIT: Magic (0x19) tokens / `case\s+0x19\b|\b0x19\b|ReceiveMagic|SKILL_ATTACK_SEND|PMSG_SKILL_ATTACK_SEND`

- line 121: `		case 0x19:`

#### CONTEXT (first hit)

```
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
   354: 			break;
   355: 		case 0x8E:
   356: 			gMove.CGTeleportMoveRecv((PMSG_TELEPORT_MOVE_RECV*)lpMsg,aIndex);
   357: 			break;
   358: 		case 0x90:
   359: 			gDevilSquare.CGDevilSquareEnterRecv((PMSG_DEVIL_SQUARE_ENTER_RECV*)lpMsg,aIndex);
   360: 			break;
   361: 		case 0x91:
```

### HIT: Server: F3:03 sending / `F3\s*:?\s*03|PHeadSubSet|PHeadSet|JoinMapServer|GCJoinMapServer|MapServer`

- line 50: `#include "MapServerManager.h"`
- line 436: `					CGMapServerMoveAuthRecv((PMSG_MAP_SERVER_MOVE_AUTH_RECV*)lpMsg,aIndex);`
- line 1758: `void CGMapServerMoveAuthRecv(PMSG_MAP_SERVER_MOVE_AUTH_RECV* lpMsg,int aIndex) // OK`
- line 1770: `		GCMapServerMoveAuthSend(aIndex,6);`
- line 1776: `		GCMapServerMoveAuthSend(aIndex,6);`
- line 1792: `		lpObj->MapServerMoveRequest = 1;`
- line 1794: `		GJMapServerMoveAuthSend(aIndex,account,lpMsg->name,lpMsg->AuthCode1,lpMsg->AuthCode2,lpMsg->AuthCode3,lpMsg->AuthCode4);`
- line 1998: `		lpObj->MapServerMoveRequest = 0;`
- line 2912: `void GCMapServerMoveAuthSend(int aIndex,BYTE result) // OK`
- line 3663: `	GS_GDReqOwnerGuildMaster(gMapServerManager.GetMapServerGroup(), iIndex);`
- line 3757: `		GS_GDReqRegAttackGuild(gMapServerManager.GetMapServerGroup(), iIndex);`
- line 3809: `	GS_GDReqGuildSetGiveUp(gMapServerManager.GetMapServerGroup(), iIndex, lpMsg->btGiveUp);`
- line 3866: `	GS_GDReqGuildMarkRegInfo(gMapServerManager.GetMapServerGroup(), iIndex);`
- line 3965: `				GS_GDReqRegGuildMark(gMapServerManager.GetMapServerGroup(), iIndex, iItemPos);`
- line 4175: `					GS_GDReqCastleNpcRepair(gMapServerManager.GetMapServerGroup(), iIndex, lpMsg->iNpcNumber, lpMsg->iNpcIndex, iRepairCost);`
- line 4434: `					GS_GDReqCastleNpcUpgrade(gMapServerManager.GetMapServerGroup(),iIndex,lpMsg->iNpcNumber,lpMsg->iNpcIndex,lpMsg->iNpcUpType,iNXT_UPVALUE,(iNXT_UPLVL-1));`

#### CONTEXT (first hit)

```
     1: #include "stdafx.h"
     2: #include "Protocol.h"
     3: #include "ArcaBattle.h"
     4: #include "Attack.h"
     5: #include "BloodCastle.h"
     6: #include "CashShop.h"
     7: #include "CastleSiege.h"
     8: #include "CastleSiegeSync.h"
     9: #include "CastleSiegeWeapon.h"
    10: #include "ChaosBox.h"
    11: #include "ChaosCastle.h"
    12: #include "CommandManager.h"
    13: #include "CustomBuyVip.h"
    14: #include "CustomArena.h"
    15: #include "CustomCommandDescription.h"
    16: #include "CustomEventTime.h"
    17: #include "CustomExchangeCoin.h"
    18: #include "CustomMove.h"
    19: #include "CustomStore.h"
    20: #include "CustomRanking.h"
    21: #include "Crywolf.h"
    22: #include "CSProtocol.h"
    23: #include "DarkSpirit.h"
    24: #include "DefaultClassInfo.h"
    25: #include "DevilSquare.h"
    26: #include "DoubleGoer.h"
    27: #include "DSProtocol.h"
    28: #include "Duel.h"
    29: #include "EffectManager.h"
    30: #include "ESProtocol.h"
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
```

### HIT: Server: 701/update macros / `GAMESERVER_UPDATE|>=\s*701|\b701\b`

- line 211: `			#if(GAMESERVER_UPDATE>=601)`
- line 216: `			#if(GAMESERVER_UPDATE>=601)`
- line 224: `					#if(GAMESERVER_UPDATE>=801)`
- line 229: `					#if(GAMESERVER_UPDATE>=801)`
- line 234: `					#if(GAMESERVER_UPDATE>=801)`
- line 244: `					#if(GAMESERVER_UPDATE>=802)`
- line 249: `					#if(GAMESERVER_UPDATE>=802)`
- line 254: `					#if(GAMESERVER_UPDATE>=802)`
- line 259: `					#if(GAMESERVER_UPDATE>=802)`
- line 264: `					#if(GAMESERVER_UPDATE>=802)`
- line 269: `					#if(GAMESERVER_UPDATE>=802)`
- line 274: `					#if(GAMESERVER_UPDATE>=802)`
- line 279: `					#if(GAMESERVER_UPDATE>=802)`
- line 284: `					#if(GAMESERVER_UPDATE>=802)`
- line 294: `					#if(GAMESERVER_UPDATE>=803)`
- line 301: `					#if(GAMESERVER_UPDATE>=803)`

#### CONTEXT (first hit)

```
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
   354: 			break;
   355: 		case 0x8E:
   356: 			gMove.CGTeleportMoveRecv((PMSG_TELEPORT_MOVE_RECV*)lpMsg,aIndex);
   357: 			break;
   358: 		case 0x90:
   359: 			gDevilSquare.CGDevilSquareEnterRecv((PMSG_DEVIL_SQUARE_ENTER_RECV*)lpMsg,aIndex);
   360: 			break;
   361: 		case 0x91:
   362: 			CGEventRemainTimeRecv((PMSG_EVENT_REMAIN_TIME_RECV*)lpMsg,aIndex);
   363: 			break;
   364: 		case 0x9A:
   365: 			gBloodCastle.CGBloodCastleEnterRecv((PMSG_BLOOD_CASTLE_ENTER_RECV*)lpMsg,aIndex);
   366: 			break;
   367: 		case 0xA0:
   368: 			gQuest.CGQuestInfoRecv(aIndex);
   369: 			break;
   370: 		case 0xA2:
   371: 			gQuest.CGQuestStateRecv((PMSG_QUEST_STATE_RECV*)lpMsg,aIndex);
   372: 			break;
   373: 		case 0xA7:
   374: 			CGPetItemCommandRecv((PMSG_PET_ITEM_COMMAND_RECV*)lpMsg,aIndex);
   375: 			break;
   376: 		case 0xA9:
   377: 			CGPetItemInfoRecv((PMSG_PET_ITEM_INFO_RECV*)lpMsg,aIndex);
   378: 			break;
   379: 		case 0xAA:
   380: 			#if(GAMESERVER_UPDATE>=402)
   381: 			switch(lpMsg[3])
   382: 			{
   383: 				case 0x01:
   384: 					gDuel.CGDuelStartRecv((PMSG_DUEL_START_RECV*)lpMsg,aIndex);
   385: 					break;
   386: 				case 0x02:
   387: 					gDuel.CGDuelOkRecv((PMSG_DUEL_OK_RECV*)lpMsg,aIndex);
   388: 					break;
   389: 				case 0x03:
   390: 					//gDuel.CGDuelEndRecv(aIndex);
   391: 					break;
   392: 				case 0x07:
   393: 					gDuel.CGDuelWatchRecv((PMSG_DUEL_WATCH_RECV*)lpMsg,aIndex);
   394: 					break;
   395: 				case 0x09:
   396: 					gDuel.CGDuelButtonRecv((PMSG_DUEL_BUTTON_RECV*)lpMsg,aIndex);
   397: 					break;
   398: 			}
   399: 			#else
   400: 			gDuel.CGDuelStartRecv((PMSG_DUEL_START_RECV*)lpMsg,aIndex);
   401: 			#endif
   402: 			break;
   403: 		case 0xAB:
   404: 			#if(GAMESERVER_UPDATE<=401)
   405: 			gDuel.CGDuelEndRecv(aIndex);
   406: 			#endif
   407: 			break;
   408: 		case 0xAC:
   409: 			#if(GAMESERVER_UPDATE<=401)
   410: 			gDuel.CGDuelOkRecv((PMSG_DUEL_OK_RECV*)lpMsg,aIndex);
   411: 			#endif
   412: 			break;
   413: 		case 0xAE:
   414: 			#if(GAMESERVER_UPDATE>=603)
   415: 			gHelper.CGHelperDataRecv((PMSG_HELPER_DATA_RECV*)lpMsg,aIndex);
   416: 			#endif
   417: 			break;
   418: 		case 0xAF:
   419: 			switch(lpMsg[3])
   420: 			{
   421: 				case 0x01:
   422: 					gChaosCastle.CGChaosCastleEnterRecv((PMSG_CHAOS_CASTLE_ENTER_RECV*)lpMsg,aIndex);
   423: 					break;
   424: 				case 0x02:
   425: 					gChaosCastle.CGChaosCastlePositionRecv((PMSG_CHAOS_CASTLE_POSITION_RECV*)lpMsg,aIndex);
   426: 					break;
   427: 			}
   428: 			break;
   429: 		case 0xB0:
   430: 			gSkillManager.CGSkillTeleportAllyRecv((PMSG_SKILL_TELEPORT_ALLY_RECV*)lpMsg,aIndex);
   431: 			break;
   432: 		case 0xB1:
   433: 			switch(lpMsg[3])
   434: 			{
   435: 				case 0x01:
   436: 					CGMapServerMoveAuthRecv((PMSG_MAP_SERVER_MOVE_AUTH_RECV*)lpMsg,aIndex);
   437: 					break;
   438: 			}
   439: 			break;
   440: 		case 0xB2:
   441: 			switch(lpMsg[3])
   442: 			{
   443: 				case 0x00:
   444: 					CGReqCastleSiegeState((PMSG_REQ_CASTLESIEGESTATE*)lpMsg,aIndex);
   445: 					break;
   446: 				case 0x01:
   447: 					CGReqRegCastleSiege((PMSG_REQ_REGCASTLESIEGE*)lpMsg,aIndex);
   448: 					break;
   449: 				case 0x02:
   450: 					CGReqGiveUpCastleSiege((PMSG_REQ_GIVEUPCASTLESIEGE*)lpMsg,aIndex);
   451: 					break;
```


---

## FILE: GS_Protocol.h

### HIT: Client: server version tokens / `m_ServerVersion|ServerVersion|gServerInfo|GAMESERVER_UPDATE|>=\s*701|\b701\b`

- line 3: `#if(GAMESERVER_UPDATE>=701)`
- line 312: `	#if(GAMESERVER_UPDATE>=803)`
- line 474: `	#if(GAMESERVER_UPDATE>=701)`
- line 480: `	#if(GAMESERVER_UPDATE>=701)`
- line 642: `	#if(GAMESERVER_UPDATE>=401)`
- line 803: `	//#if(GAMESERVER_UPDATE>=602)`
- line 865: `	#if(GAMESERVER_UPDATE>=602)`

#### CONTEXT (first hit)

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
```

### HIT: Client: JoinMapServer / F3:03 parsing / `F3\s*:?\s*03|JoinMapServer|GCJoinMapServer|MapServer|JoinGame`

- line 350: `	PSBMSG_HEAD header; // C1:F3:03`
- line 837: `	PSBMSG_HEAD header; // C3:F3:03`
- line 1203: `void CGMapServerMoveAuthRecv(PMSG_MAP_SERVER_MOVE_AUTH_RECV* lpMsg,int aIndex);`
- line 1245: `void GCMapServerMoveAuthSend(int aIndex,BYTE result);`

#### CONTEXT (first hit)

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
```

### HIT: Server: F3:03 sending / `F3\s*:?\s*03|PHeadSubSet|PHeadSet|JoinMapServer|GCJoinMapServer|MapServer`

- line 350: `	PSBMSG_HEAD header; // C1:F3:03`
- line 837: `	PSBMSG_HEAD header; // C3:F3:03`
- line 1203: `void CGMapServerMoveAuthRecv(PMSG_MAP_SERVER_MOVE_AUTH_RECV* lpMsg,int aIndex);`
- line 1245: `void GCMapServerMoveAuthSend(int aIndex,BYTE result);`

#### CONTEXT (first hit)

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
```

### HIT: Server: 701/update macros / `GAMESERVER_UPDATE|>=\s*701|\b701\b`

- line 3: `#if(GAMESERVER_UPDATE>=701)`
- line 312: `	#if(GAMESERVER_UPDATE>=803)`
- line 474: `	#if(GAMESERVER_UPDATE>=701)`
- line 480: `	#if(GAMESERVER_UPDATE>=701)`
- line 642: `	#if(GAMESERVER_UPDATE>=401)`
- line 803: `	//#if(GAMESERVER_UPDATE>=602)`
- line 865: `	#if(GAMESERVER_UPDATE>=602)`

#### CONTEXT (first hit)

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
```


---

## QUESTIONS (responder com CONFIRMED/PROBABLE + âncora)

1) O client recebe a versão do servidor por qual pacote/campo? Onde isso é guardado (ex.: m_ServerVersion)?

2) Existe lógica no client que muda parsing/formatos quando versão >=701?

3) O pacote 0x19 (magic) tem parsing dependente de versão no client legado?

4) No GS, onde é decidido/compilado o modo >=701 (macro) e onde isso afeta o formato enviado?
