# NEW-ERA 1.3-S — GS EMITTERS 0x18/0x19 (combat ack/anim) — EVIDENCE

- Legacy commit: 580472e0d5723f9709cbad594f233deb07f9f351


## Files fetched (sha256)

- Protocol.cpp sha256 e8bb4dbb28d1baf23222dbc72762e2ebd55b116dbb8d36e5f017dd744e510f13 bytes=135014
- Attack.cpp sha256 dd7459f2d5f520e8a0a8916026d64e036e2300c1936948d7869a319faa2c3090 bytes=85613

---

## FILE: Protocol.cpp

### HIT: Head set to 0x18 / PHeadSet\([^\)]*0x18|\b0x18\b

- line 83: 		if((head != 0xF3 || lpMsg[3] != 0xF1) && head != 0x0E && head != 0x18 && head != PROTOCOL_CODE1)
- line 118: 		case 0x18:
- line 1494: 	pMsg.header.set(0x18,sizeof(pMsg));
- line 2581: 	pMsg.header.set(0x18,sizeof(pMsg));
- line 4817: 	pMsgResult.h.set(0xB2, 0x18, sizeof(pMsgResult));

#### CONTEXT (first hit)

```
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
```

### HIT: Head set to 0x19 / PHeadSet\([^\)]*0x19|\b0x19\b

- line 121: 		case 0x19:

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

### HIT: DataSend calls / \bDataSend\b

- line 1224: 					DataSend(gParty.m_PartyInfo[lpObj->PartyNumber].Index[n],(BYTE*)&pMsg,pMsg.header.size);
- line 1264: 					DataSend(n,(BYTE*)&pMsg,pMsg.header.size);
- line 1271: 		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
- line 1443: 			DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
- line 1456: 					DataSend(lpObj->VpPlayer2[n].index,(BYTE*)&pMsg,pMsg.header.size);
- line 1514: 				DataSend(lpObj->VpPlayer2[n].index,(BYTE*)&pMsg,pMsg.header.size);
- line 1634: 	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
- line 1940: 			DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
- line 1953: 					DataSend(lpObj->VpPlayer2[n].index,(BYTE*)&pMsg,pMsg.header.size);
- line 2051: 		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
- line 2057: 		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
- line 2063: 		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
- line 2069: 		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
- line 2075: 		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);

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

### HIT: Structs / packets / \bPMSG_\w+|\bPBMSG_\w+|\bATTACK\b|\bACTION\b|\bMAGIC\b

- line 101: 			CGChatRecv((PMSG_CHAT_RECV*)lpMsg,aIndex);
- line 104: 			CGChatWhisperRecv((PMSG_CHAT_WHISPER_RECV*)lpMsg,aIndex);
- line 107: 			CGMainCheckRecv((PMSG_MAIN_CHECK_RECV*)lpMsg,aIndex);
- line 110: 			CGLiveClientRecv((PMSG_LIVE_CLIENT_RECV*)lpMsg,aIndex);
- line 113: 			gAttack.CGAttackRecv((PMSG_ATTACK_RECV*)lpMsg,aIndex);
- line 116: 			CGPositionRecv((PMSG_POSITION_RECV*)lpMsg,aIndex);
- line 119: 			CGActionRecv((PMSG_ACTION_RECV*)lpMsg,aIndex);
- line 122: 			gSkillManager.CGSkillAttackRecv((PMSG_SKILL_ATTACK_RECV*)lpMsg,aIndex);
- line 125: 			gSkillManager.CGSkillCancelRecv((PMSG_SKILL_CANCEL_RECV*)lpMsg,aIndex);
- line 128: 			gMove.CGTeleportRecv((PMSG_TELEPORT_RECV*)lpMsg,aIndex);
- line 131: 			gSkillManager.CGDurationSkillAttackRecv((PMSG_DURATION_SKILL_ATTACK_RECV*)lpMsg,aIndex);
- line 134: 			gItemManager.CGItemGetRecv((PMSG_ITEM_GET_RECV*)lpMsg,aIndex);
- line 137: 			gItemManager.CGItemDropRecv((PMSG_ITEM_DROP_RECV*)lpMsg,aIndex);
- line 140: 			gItemManager.CGItemMoveRecv((PMSG_ITEM_MOVE_RECV*)lpMsg,aIndex);

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

## FILE: Attack.cpp

### HIT: Structs / packets / \bPMSG_\w+|\bPBMSG_\w+|\bATTACK\b|\bACTION\b|\bMAGIC\b

- line 2971: void CAttack::CGAttackRecv(PMSG_ATTACK_RECV* lpMsg,int aIndex) // OK

#### CONTEXT (first hit)

```
  2901: 	(*DamageMin) += lpObj->EffectOption.AddCurseDamage;
  2902: 	(*DamageMax) += lpObj->EffectOption.AddCurseDamage;
  2903: 
  2904: 	(*DamageMin) += lpObj->EffectOption.AddMinCurseDamage;
  2905: 	(*DamageMax) += lpObj->EffectOption.AddMaxCurseDamage;
  2906: 
  2907: 	gSkillManager.SkillSwordPowerGetCurseDamage(lpObj->Index,(int*)DamageMin,(int*)DamageMax);
  2908: 
  2909: 	(*MulDamage) = lpObj->EffectOption.MulCurseDamage;
  2910: 	(*DivDamage) = lpObj->EffectOption.DivCurseDamage;
  2911: 
  2912: 	if(Left->IsItem() != 0 && Left->m_IsValidItem != 0 && Left->m_Index >= GET_ITEM(5,21) && Left->m_Index <= GET_ITEM(5,23))
  2913: 	{
  2914: 		(*DamageRate) = (int)(((Left->m_MagicDamageRate/2)+(Left->m_Level*2))*Left->m_CurrentDurabilityState);
  2915: 	}
  2916: 	else
  2917: 	{
  2918: 		(*DamageRate) = 0;
  2919: 	}
  2920: }
  2921: 
  2922: void CAttack::GetPreviewDamageMultiplier(LPOBJ lpObj,DWORD* DamageMultiplier,DWORD* RFDamageMultiplierA,DWORD* RFDamageMultiplierB,DWORD* RFDamageMultiplierC) // OK
  2923: {
  2924: 	switch(lpObj->Class)
  2925: 	{
  2926: 		case CLASS_DW:
  2927: 			(*DamageMultiplier) = 200;
  2928: 			(*RFDamageMultiplierA) = 100;
  2929: 			(*RFDamageMultiplierB) = 100;
  2930: 			(*RFDamageMultiplierC) = 100;
  2931: 			break;
  2932: 		case CLASS_DK:
  2933: 			(*DamageMultiplier) = lpObj->DKDamageMultiplierRate;
  2934: 			(*RFDamageMultiplierA) = 100;
  2935: 			(*RFDamageMultiplierB) = 100;
  2936: 			(*RFDamageMultiplierC) = 100;
  2937: 			break;
  2938: 		case CLASS_FE:
  2939: 			(*DamageMultiplier) = 200;
  2940: 			(*RFDamageMultiplierA) = 100;
  2941: 			(*RFDamageMultiplierB) = 100;
  2942: 			(*RFDamageMultiplierC) = 100;
  2943: 			break;
  2944: 		case CLASS_MG:
  2945: 			(*DamageMultiplier) = 200;
  2946: 			(*RFDamageMultiplierA) = 100;
  2947: 			(*RFDamageMultiplierB) = 100;
  2948: 			(*RFDamageMultiplierC) = 100;
  2949: 			break;
  2950: 		case CLASS_DL:
  2951: 			(*DamageMultiplier) = lpObj->DLDamageMultiplierRate;
  2952: 			(*RFDamageMultiplierA) = 100;
  2953: 			(*RFDamageMultiplierB) = 100;
  2954: 			(*RFDamageMultiplierC) = 100;
  2955: 			break;
  2956: 		case CLASS_SU:
  2957: 			(*DamageMultiplier) = 200;
  2958: 			(*RFDamageMultiplierA) = 100;
  2959: 			(*RFDamageMultiplierB) = 100;
  2960: 			(*RFDamageMultiplierC) = 100;
  2961: 			break;
  2962: 		case CLASS_RF:
  2963: 			(*DamageMultiplier) = 200;
  2964: 			(*RFDamageMultiplierA) = lpObj->RFDamageMultiplierRate[0];
  2965: 			(*RFDamageMultiplierB) = lpObj->RFDamageMultiplierRate[1];
  2966: 			(*RFDamageMultiplierC) = lpObj->RFDamageMultiplierRate[2];
  2967: 			break;
  2968: 	}
  2969: }
  2970: 
  2971: void CAttack::CGAttackRecv(PMSG_ATTACK_RECV* lpMsg,int aIndex) // OK
  2972: {
  2973: 	LPOBJ lpObj = &gObj[aIndex];
  2974: 
  2975: 	if(gObjIsConnected(aIndex) == 0)
  2976: 	{
  2977: 		return;
  2978: 	}
  2979: 
  2980: 	int bIndex = MAKE_NUMBERW(lpMsg->index[0],lpMsg->index[1]);
  2981: 
  2982: 	if(OBJECT_RANGE(bIndex) == 0)
  2983: 	{
  2984: 		return;
  2985: 	}
  2986: 
  2987: 	LPOBJ lpTarget = &gObj[bIndex];
  2988: 
  2989: 	if(lpTarget->Live == 0)
  2990: 	{
  2991: 		return;
  2992: 	}
  2993: 
  2994: 	if(lpObj->Map != lpTarget->Map)
  2995: 	{
  2996: 		return;
  2997: 	}
  2998: 
  2999: 	#if(GAMESERVER_UPDATE>=402)
  3000: 
  3001: 	if(gDuel.GetDuelArenaBySpectator(aIndex) != 0 || gDuel.GetDuelArenaBySpectator(bIndex) != 0)
  3002: 	{
  3003: 		return;
  3004: 	}
  3005: 
  3006: 	#endif
  3007: 
  3008: 	if(gMap[lpObj->Map].CheckAttr(lpObj->X,lpObj->Y,1) != 0 || gMap[lpTarget->Map].CheckAttr(lpTarget->X,lpTarget->Y,1) != 0)
  3009: 	{
  3010: 		return;
  3011: 	}
  3012: 
  3013: 	if(lpObj->Type == OBJECT_USER && sqrt(pow(((float)lpObj->X-(float)lpTarget->X),2)+pow(((float)lpObj->Y-(float)lpTarget->Y),2)) > ((lpObj->Class==CLASS_FE)?6:3))
  3014: 	{
  3015: 		return;
  3016: 	}
  3017: 
  3018: 	lpObj->Dir = lpMsg->dir;
  3019: 
  3020: 	lpObj->MultiSkillIndex = 0;
  3021: 
  3022: 	lpObj->MultiSkillCount = 0;
  3023: 
  3024: 	GCActionSend(lpObj,lpMsg->action,aIndex,bIndex);
  3025: 
  3026: 	lpObj->ComboSkill.Init();
  3027: 
  3028: 	this->Attack(lpObj,lpTarget,0,0,0,0,0,0);
  3029: }
```


---

## QUESTIONS (responder com CONFIRMED/PROBABLE + âncora)

1) Onde no GS são montados os pacotes 0x18 e 0x19? (função/arquivo/linhas)

2) Quais structs PMSG_* são usadas para montá-los? (campos/ordem/tamanho)

3) PHeadSet/PHeadSubSet/DataSend são usados? Como o tamanho é definido?

4) Qual condição de jogo dispara o envio (ex.: ataque recebido, skill, ack)?
