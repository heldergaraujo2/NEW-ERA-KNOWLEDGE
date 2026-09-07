# NEW-ERA 1.3-Y — CLIENT RX F3:03 CharacterInfo (ReceiveJoinMapServer?) — EVIDENCE

- Legacy commit pin: 580472e0d5723f9709cbad594f233deb07f9f351


## Files fetched (sha256)

- WSclient.cpp sha256 `6538d944126181dfa77d4419bf992ac77151eec5411dc3ab243f2926fec868bf` bytes=415884

- WSclient.h   sha256 `19d38a3b6506a745662c14dd7cdd6e41ca3ad29b28b17c76f860cd9e67799c10` bytes=91012


---

## Dispatch hits

### case 0xF3 @ 12942
```
 12862: 				case 0x06:
 12863: 					CUIMng::Instance().PopUpMsgWin(RECEIVE_LOG_IN_FAIL_VERSION);
 12864: 					g_ErrorReport.Write( "Version dismatch. - Login\r\n");
 12865: 					break;
 12866: 				case 0x07:
 12867: 				default:
 12868: 					CUIMng::Instance().PopUpMsgWin(RECEIVE_LOG_IN_FAIL_CONNECT);
 12869: 					break;
 12870: 				case 0x08:
 12871: 					CUIMng::Instance().PopUpMsgWin(RECEIVE_LOG_IN_FAIL_ERROR);
 12872: 					break;
 12873: 				case 0x09:
 12874: 					CUIMng::Instance().PopUpMsgWin(RECEIVE_LOG_IN_FAIL_NO_PAYMENT_INFO);
 12875: 					break;
 12876: 				case 0x0a:
 12877: 					CUIMng::Instance().PopUpMsgWin(RECEIVE_LOG_IN_FAIL_USER_TIME1);
 12878: 					break;
 12879: 				case 0x0b:
 12880: 					CUIMng::Instance().PopUpMsgWin(RECEIVE_LOG_IN_FAIL_USER_TIME2);
 12881: 					break;
 12882: 				case 0x0c:
 12883: 					CUIMng::Instance().PopUpMsgWin(RECEIVE_LOG_IN_FAIL_PC_TIME1);
 12884: 					break;
 12885: 				case 0x0d:
 12886: 					CUIMng::Instance().PopUpMsgWin(RECEIVE_LOG_IN_FAIL_PC_TIME2);
 12887: 					break;
 12888: 				case 0x11:
 12889: 					CUIMng::Instance().PopUpMsgWin(RECEIVE_LOG_IN_FAIL_ONLY_OVER_15);
 12890: 					break;
 12891: 				case 0x40:
 12892: 					CUIMng::Instance().PopUpMsgWin(RECEIVE_LOG_IN_FAIL_CHARGED_CHANNEL);
 12893: 					break;
 12894: 				case 0xc0:
 12895: 				case 0xd0:
 12896: 					CUIMng::Instance().PopUpMsgWin(RECEIVE_LOG_IN_FAIL_POINT_DATE);
 12897: 					break;
 12898: 				case 0xc1:
 12899: 				case 0xd1:
 12900: 					CUIMng::Instance().PopUpMsgWin(RECEIVE_LOG_IN_FAIL_POINT_HOUR);
 12901: 					break;
 12902: 				case 0xc2:
 12903: 				case 0xd2:
 12904: 					CUIMng::Instance().PopUpMsgWin(RECEIVE_LOG_IN_FAIL_INVALID_IP);
 12905: 					break;
 12906: 				}
 12907: 				break;
 12908: 				case 0x02:
 12909: 					if ( !ReceiveLogOut(ReceiveBuffer, bEncrypted))
 12910: 					{
 12911: 						return ( FALSE);
 12912: 					}
 12913: 					break;
 12914: 				case 0x12: //0x02 receive create account
 12915: 					switch(Data->Value)
 12916: 					{
 12917: 					case 0x00:
 12918: 						CurrentProtocolState = RECEIVE_CREATE_ACCOUNT_FAIL_ID;
 12919: 						break;
 12920: 					case 0x01:
 12921: 						CurrentProtocolState = RECEIVE_CREATE_ACCOUNT_SUCCESS;
 12922: 						break;
 12923: 					case 0x02:
 12924: 						CurrentProtocolState = RECEIVE_CREATE_ACCOUNT_FAIL_RESIDENT;
 12925: 						break;
 12926: 					}
 12927: 					break;
 12928: 					case 0x03: //receive confirm password
 12929: 						//AddDebugText(ReceiveBuffer,Size);
 12930: 						ReceiveConfirmPassword(ReceiveBuffer);
 12931: 						break;
 12932: 					case 0x04: //receive confirm password
 12933: 						//AddDebugText(ReceiveBuffer,Size);
 12934: 						ReceiveConfirmPassword2(ReceiveBuffer);
 12935: 						break;
 12936: 					case 0x05: //receive change password
 12937: 						ReceiveChangePassword(ReceiveBuffer);
 12938: 						break;
 12939: 			}
 12940: 			break;
 12941: 		}
 12942: 	case 0xF3:
 12943: 		{
 12944: 			int subcode;
 12945: 			if( ReceiveBuffer[0] == 0xC1 )
 12946: 			{
 12947: 				LPPHEADER_DEFAULT_SUBCODE Data = (LPPHEADER_DEFAULT_SUBCODE)ReceiveBuffer;
 12948: 				subcode = Data->SubCode;
 12949: 			}
 12950: 			else
 12951: 			{
 12952: 				LPPHEADER_DEFAULT_SUBCODE_WORD Data = (LPPHEADER_DEFAULT_SUBCODE_WORD)ReceiveBuffer;
 12953: 				subcode = Data->SubCode;
 12954: 			}
 12955: 
 12956: 			g_ConsoleDebug->Write(MCD_RECEIVE, "Recv [0xF3][0x%02x]", subcode);
 12957: 
 12958: 			switch( subcode )
 12959: 			{
 12960: 			case 0x00: //receive characters list
 12961:                 ReceiveCharacterList(ReceiveBuffer);
 12962: 				break;
 12963: 			case 0x01: //receive create character
 12964:                 ReceiveCreateCharacter(ReceiveBuffer);
 12965: 				break;
 12966: 			case 0x02: //receive delete character
 12967:                 ReceiveDeleteCharacter(ReceiveBuffer);
 12968: 				break;
 12969: 			case 0x03: //receive join map server
 12970:                 if ( !ReceiveJoinMapServer(ReceiveBuffer, bEncrypted))
 12971: 				{
 12972: 					return ( FALSE);
 12973: 				}
 12974: 				break;
 12975: 			case 0x04: //receive revival
 12976:                 ReceiveRevival(ReceiveBuffer);
 12977: 				break;
 12978: 			case 0x10: //receive inventory
 12979:                 //AddDebugText(ReceiveBuffer,Size);
 12980:                 if ( !ReceiveInventory(ReceiveBuffer, bEncrypted))
 12981: 				{
 12982: 					return ( FALSE);
 12983: 				}
 12984: 				break;
 12985: 			case 0x05: //receive level up
 12986:                 ReceiveLevelUp(ReceiveBuffer);
 12987: 				break;
 12988: 			case 0x06: //receive Add Point
 12989:                 ReceiveAddPoint(ReceiveBuffer);
 12990: 				break;
 12991: 			case 0x07: //receive damage
 12992:                 ReceiveDamage(ReceiveBuffer);
 12993: 				break;
 12994: 			case 0x08:
 12995:                 ReceivePK(ReceiveBuffer);
 12996: 				break;
 12997: 			case 0x11:
 12998:                 ReceiveMagicList(ReceiveBuffer);
 12999: 				break;
 13000: 			case 0x13:
 13001:                 ReceiveEquipment(ReceiveBuffer);
 13002: 				break;
 13003: 			case 0x14:
 13004:                 ReceiveModifyItem(ReceiveBuffer);
 13005: 				break;
 13006: 			case 0x20:
 13007:                 ReceiveSummonLife(ReceiveBuffer);
 13008: 				break;
 13009: 			case 0x22:
 13010:                 ReceiveWTTimeLeft(ReceiveBuffer);
 13011: 				break;
 13012: 
 13013: 			case 0x24:
 13014:                 ReceiveWTMatchResult(ReceiveBuffer);
 13015: 				break;
 13016: 
 13017: 			case 0x25:
 13018: 				ReceiveWTBattleSoccerGoalIn(ReceiveBuffer);
 13019: 				break;
 13020: 			case 0x23:
 13021:                 ReceiveSoccerScore(ReceiveBuffer);
 13022: 				break;
 13023:             case 0x30:
 13024:                 ReceiveOption(ReceiveBuffer);
 13025:                 break;
 13026: 			case 0x40:
 13027: 				ReceiveServerCommand(ReceiveBuffer);
 13028: 				break;
 13029: 			case 0x50:
 13030: 				Receive_Master_Level_Exp(ReceiveBuffer);
 13031: 				break;
 13032: 			case 0x51:
 13033: 				Receive_Master_LevelUp(ReceiveBuffer);
 13034: 				break;
 13035: 			case 0x52:
 13036: 				Receive_Master_LevelGetSkill(ReceiveBuffer);
 13037: 				break;
 13038: 			}
 13039: 			break;
 13040: 		}
 13041: 	case 0xF4:
 13042: 		{
 13043: 			int subcode;
 13044: 			if( ReceiveBuffer[0] == 0xC1 )
 13045: 			{
 13046: 				LPPHEADER_DEFAULT_SUBCODE Data = (LPPHEADER_DEFAULT_SUBCODE)ReceiveBuffer;
 13047: 				subcode = Data->SubCode;
 13048: 			}
 13049: 			else
 13050: 			{
 13051: 				LPPHEADER_DEFAULT_SUBCODE_WORD Data = (LPPHEADER_DEFAULT_SUBCODE_WORD)ReceiveBuffer;
 13052: 				subcode = Data->SubCode;
 13053: 			}
 13054: 			switch( subcode )
 13055: 			{
 13056: 			case 0x06:
 13057: 				ReceiveServerList(ReceiveBuffer);
 13058: 				break;
 13059: 			case 0x03:
 13060: 				ReceiveServerConnect(ReceiveBuffer);
 13061: 				break;
 13062: 			case 0x05:
 13063: 				ReceiveServerConnectBusy(ReceiveBuffer);
 13064: 				break;
 13065: 			}
 13066: 			break;
 13067: 		}
 13068: 	case 0x00://chat
 13069:         ReceiveChat(ReceiveBuffer);
 13070: 		break;
 13071: 	case 0x01://chat
 13072:         ReceiveChatKey(ReceiveBuffer);
 13073: 		break;
 13074: 	case 0x02://chat whisper
 13075:         ReceiveChatWhisper(ReceiveBuffer);
 13076: 		break;
 13077: 	case 0x03:
 13078: 		ReceiveCheckSumRequest( ReceiveBuffer);
 13079: 		break;
 13080: 	case 0x0B:
 13081:         AddDebugText(ReceiveBuffer,Size);
 13082:         ReceiveEvent(ReceiveBuffer);
 13083: 		break;
 13084: 	case 0x0C:
 13085:         //AddDebugText(ReceiveBuffer,Size);
 13086:         ReceiveChatWhisperResult(ReceiveBuffer);
 13087: 		break;
 13088: 	case 0x0D://notice
 13089:         ReceiveNotice(ReceiveBuffer);
 13090: 		break;
 13091: 	case 0x0f:
 13092:         ReceiveWeather(ReceiveBuffer);
 13093: 		break;
 13094: 	case PACKET_MOVE: //move character
 13095:         ReceiveMoveCharacter(ReceiveBuffer);
 13096: 		break;
 13097: 	case PACKET_POSITION: //move position
 13098:         ReceiveMovePosition(ReceiveBuffer);
 13099: 		break;
 13100: 	case 0x12: //create characters
 13101:         AddDebugText(ReceiveBuffer,Size);
 13102:         ReceiveCreatePlayerViewport(ReceiveBuffer,Size);
 13103: 		break;
 13104: 	case 0x13: //create monsters
 13105:         //AddDebugText(ReceiveBuffer,Size);
 13106:         ReceiveCreateMonsterViewport(ReceiveBuffer);
 13107: 		break;
 13108: 	case 0x1F: //create monsters
 13109:         //AddDebugText(ReceiveBuffer,Size);
 13110:         ReceiveCreateSummonViewport(ReceiveBuffer);
 13111: 		break;
 13112: 	case 0x45: //create monsters
 13113:         //AddDebugText(ReceiveBuffer,Size);
 13114:         ReceiveCreateTransformViewport(ReceiveBuffer);
 13115: 		break;
 13116: 	case 0x14: //delete characters & monsters
 13117:         //AddDebugText(ReceiveBuffer,Size);
 13118:         ReceiveDeleteCharacterViewport(ReceiveBuffer);
 13119: 		break;
 13120: 	case 0x20: //create item
 13121:         ReceiveCreateItemViewport(ReceiveBuffer);
 13122: 		break;
 13123: 	case 0x21://delete item
 13124:         ReceiveDeleteItemViewport(ReceiveBuffer);
 13125: 		break;
 13126: 	case 0x22://get item
 13127:         //AddDebugText(ReceiveBuffer,Size);
 13128:         ReceiveGetItem(ReceiveBuffer);
 13129: 		break;
 13130: 	case 0x23://drop item
 13131:         ReceiveDropItem(ReceiveBuffer);
 13132: 		break;
 13133: 	case 0x24://equipment item
 13134:         AddDebugText(ReceiveBuffer,Size);
 13135:         if ( !ReceiveEquipmentItem(ReceiveBuffer, bEncrypted))
 13136: 		{
 13137: 			return ( FALSE);
 13138: 		}
 13139: 		break;
 13140: 	case 0x25://change character
 13141:         ReceiveChangePlayer(ReceiveBuffer);
 13142: 		break;
 13143: 	case PACKET_ATTACK://attack character
 13144:         ReceiveAttackDamage(ReceiveBuffer);
 13145: 		break;
 13146: 	case 0x18://action character
 13147:         ReceiveAction(ReceiveBuffer,Size);
 13148: 		break;
 13149: 	case 0x19://magic
 13150:         if ( !ReceiveMagic(ReceiveBuffer,Size, bEncrypted))
 13151: 		{
 13152: 			return ( FALSE);
 13153: 		}
 13154: 		break;
 13155: 	case 0x69:
 13156:         if ( !ReceiveMonsterSkill(ReceiveBuffer,Size, bEncrypted))
 13157: 		{
 13158: 			return ( FALSE);
 13159: 		}
 13160: 		break;
 13161: 	case 0x1A://magic
 13162:         ReceiveMagicPosition(ReceiveBuffer,Size);
 13163: 		break;
 13164: 	case 0x1E://magic
 13165:         if ( !ReceiveMagicContinue(ReceiveBuffer,Size, bEncrypted))
 13166: 		{
 13167: 			return ( FALSE);
 13168: 		}
 13169: 		break;
 13170: 	case 0x1B://magic
 13171:         ReceiveMagicFinish(ReceiveBuffer);
 13172: 		break;
 13173: 	case 0x07://setmagicstatus
 13174: 		ReceiveSkillStatus(ReceiveBuffer);
 13175: 		break;
 13176: 	case 0x16://die character(exp)
 13177:         if ( !ReceiveDieExp(ReceiveBuffer,bEncrypted))
 13178: 		{
 13179: 			return ( FALSE);
 13180: 		}
 13181: 		break;
 13182: 	case 0x9C://die character(exp)
 13183:         if ( !ReceiveDieExpLarge(ReceiveBuffer,bEncrypted))
 13184: 		{
 13185: 			return ( FALSE);
 13186: 		}
 13187: 		break;
 13188: 	case 0x17://die character
 13189:         ReceiveDie(ReceiveBuffer,Size);
 13190: 		break;
 13191: 	case 0x2A:
 13192:         AddDebugText(ReceiveBuffer,Size);
 13193:         ReceiveDurability(ReceiveBuffer);
 13194: 		break;
 13195: 	case 0x26:
 13196:         ReceiveLife(ReceiveBuffer);
 13197: 		break;
 13198: 	case 0x27:
 13199:         ReceiveMana(ReceiveBuffer);
 13200: 		break;
 13201: 	case 0x28:
 13202:         ReceiveDeleteInventory(ReceiveBuffer);
```

### subcase 0x03 @ 871
```
   751: 
   752:     matchEvent::ClearMatchInfo();
   753: 
   754: 	InitPartyList();
   755: 
   756: 	g_csQuest.clearQuest ();
   757: 
   758: 	g_DuelMgr.Reset();
   759: 	g_pNewUISystem->Hide( SEASON3B::INTERFACE_DUEL_WINDOW );
   760: 
   761: 	if( g_pUIManager )
   762: 		g_pUIManager->Init();
   763: 
   764: 	if( g_pSiegeWarfare )
   765: 		g_pSiegeWarfare->InitMiniMapUI();
   766: 
   767: 	g_Direction.Init();
   768: 	g_Direction.DeleteMonster();
   769: 
   770: 	RemoveAllPerosnalItemPrice(PSHOPWNDTYPE_SALE);
   771: 	RemoveAllPerosnalItemPrice(PSHOPWNDTYPE_PURCHASE);
   772: 
   773: 	g_pNewUIHotKey->SetStateGameOver(false);
   774: 	g_pMyShopInventory->ResetSubject();
   775: 	g_pChatListBox->ResetFilter();
   776: 
   777: 	g_pGuildInfoWindow->NoticeClear();
   778: }
   779: 
   780: BOOL ReceiveLogOut(BYTE *ReceiveBuffer, BOOL bEncrypted)
   781: {
   782: 	LogOut = false;
   783: 	LPPHEADER_DEFAULT_SUBCODE Data = (LPPHEADER_DEFAULT_SUBCODE)ReceiveBuffer;
   784: 	switch(Data->Value)
   785: 	{
   786: 	case 0:
   787: 		SendMessage(g_hWnd, WM_DESTROY, 0, 0);
   788: 		break;
   789: 	case 1:
   790: //#ifndef NEW_PROTOCOL_SYSTEM
   791: //		if ( !bEncrypted)
   792: //		{
   793: //			GO_DEBUG;
   794: //			SendHackingChecked( 0x00, 0xF1);
   795: //			return ( FALSE);
   796: //		}
   797: //#endif
   798: 		StopMusic();
   799:         AllStopSound();
   800: 
   801: 		SEASON3B::CNewUIInventoryCtrl::BackupPickedItem();
   802: 
   803: 		ReleaseMainData();
   804: 		CryWolfMVPInit();
   805: 
   806: 		SceneFlag = CHARACTER_SCENE;
   807: 
   808: 		#ifdef NEW_PROTOCOL_SYSTEM
   809: 			gProtocolSend.SendRequestCharactersListNew();
   810: 		#else
   811: 			SendRequestCharactersList(g_pMultiLanguage->GetLanguage());
   812: 		#endif
   813: 
   814: 		InitCharacterScene = false;
   815:         InitMainScene = false;
   816:         EnableMainRender = false;
   817: 		CurrentProtocolState = REQUEST_JOIN_SERVER;
   818: 		InitGame();
   819: 		break;
   820: 	case 2:
   821: 		if(SceneFlag == MAIN_SCENE)
   822: 		{
   823: 			CryWolfMVPInit();
   824: 			StopMusic();
   825:             AllStopSound();
   826: 			SEASON3B::CNewUIInventoryCtrl::BackupPickedItem();
   827: 			ReleaseMainData();
   828: 		}
   829: 
   830: //#ifndef NEW_PROTOCOL_SYSTEM
   831: //		if ( !bEncrypted)
   832: //		{
   833: //			GO_DEBUG;
   834: //			SendHackingChecked( 0x00, 0xF1);
   835: //			return ( FALSE);
   836: //		}
   837: //#endif
   838: 		g_ErrorReport.Write("[ReceiveLogOut]");
   839: 		SocketClient.Close();
   840: 
   841: 		#ifdef NEW_PROTOCOL_SYSTEM
   842: 			gProtocolSend.DisconnectServer();
   843: 		#endif
   844: 
   845: 		ReleaseCharacterSceneData();
   846: 		SceneFlag = LOG_IN_SCENE;
   847: 
   848: 		InitLogIn = false;
   849: 		InitCharacterScene = false;
   850:         InitMainScene = false;
   851:         EnableMainRender = false;
   852: 		CurrentProtocolState = REQUEST_JOIN_SERVER;
   853: 
   854:         LogIn = 0;
   855:         g_csMapServer.Init ();
   856: 		InitGame();
   857: 		break;
   858: 	}
   859: 
   860: 	g_pWindowMgr->Reset();
   861: 	g_pFriendList->ClearFriendList();
   862: 	g_pLetterList->ClearLetterList();
   863: 
   864: 	g_ConsoleDebug->Write(MCD_RECEIVE, "0x02 [ReceiveServerList(%d)]", Data->Value);
   865: 
   866: 	return ( TRUE);
   867: }
   868: 
   869: int HeroIndex;
   870: 
   871: BOOL ReceiveJoinMapServer(BYTE *ReceiveBuffer, BOOL bEncrypted)
   872: {
   873: //#ifndef NEW_PROTOCOL_SYSTEM
   874: //	if ( !bEncrypted)
   875: //	{
   876: //		GO_DEBUG;
   877: //		SendHackingChecked( 0x00, 0xF3);
   878: //		return ( FALSE);
   879: //	}
   880: //#endif
   881: 	MouseLButton = false;
   882: 
   883: 	LPPRECEIVE_JOIN_MAP_SERVER Data = (LPPRECEIVE_JOIN_MAP_SERVER)ReceiveBuffer;
   884: 
   885: 
   886: 	__int64 Data_Exp = 0x0000000000000000;
   887: 	Master_Level_Data.lMasterLevel_Experince = 0x0000000000000000;
   888: 	Data_Exp |= Data->btMExp1;
   889: 	Data_Exp <<= 8;
   890: 	Data_Exp |= Data->btMExp2;
   891: 	Data_Exp <<= 8;
   892: 	Data_Exp |= Data->btMExp3;
   893: 	Data_Exp <<= 8;
   894: 	Data_Exp |= Data->btMExp4;
   895: 	Data_Exp <<= 8;
   896: 	Data_Exp |= Data->btMExp5;
   897: 	Data_Exp <<= 8;
   898: 	Data_Exp |= Data->btMExp6;
   899: 	Data_Exp <<= 8;
   900: 	Data_Exp |= Data->btMExp7;
   901: 	Data_Exp <<= 8;
   902: 	Data_Exp |= Data->btMExp8;
   903: 
   904: 	if(gCharacterManager.IsMasterLevel(CharacterAttribute->Class) == true)
   905: 	{
   906: 		Master_Level_Data.lMasterLevel_Experince = Data_Exp;
   907: 	}
   908: 	else
   909: 	{
   910: 		CharacterAttribute->Experience = (int)Data_Exp;
   911: 	}
   912: 
   913: 	Data_Exp = 0x0000000000000000;
   914: 	Master_Level_Data.lNext_MasterLevel_Experince = 0x0000000000000000;
   915: 
   916: 	Data_Exp |= Data->btMNextExp1;
   917: 	Data_Exp <<= 8;
   918: 	Data_Exp |= Data->btMNextExp2;
   919: 	Data_Exp <<= 8;
   920: 	Data_Exp |= Data->btMNextExp3;
   921: 	Data_Exp <<= 8;
   922: 	Data_Exp |= Data->btMNextExp4;
   923: 	Data_Exp <<= 8;
   924: 	Data_Exp |= Data->btMNextExp5;
   925: 	Data_Exp <<= 8;
   926: 	Data_Exp |= Data->btMNextExp6;
   927: 	Data_Exp <<= 8;
   928: 	Data_Exp |= Data->btMNextExp7;
   929: 	Data_Exp <<= 8;
   930: 	Data_Exp |= Data->btMNextExp8;
   931: 
   932: 	if(gCharacterManager.IsMasterLevel(CharacterAttribute->Class) == true)
   933: 	{
   934: 		Master_Level_Data.lNext_MasterLevel_Experince = Data_Exp;
   935: 	}
   936: 	else
   937: 	{
   938: 		CharacterAttribute->NextExperince = (int)Data_Exp;
   939: 	}
   940: 
   941: 	CharacterAttribute->LevelUpPoint  = Data->LevelUpPoint;
   942: 	CharacterAttribute->Strength      = Data->Strength;
   943: 	CharacterAttribute->Dexterity     = Data->Dexterity;
   944: 	CharacterAttribute->Vitality      = Data->Vitality;
   945: 	CharacterAttribute->Energy        = Data->Energy;
   946:     CharacterAttribute->Charisma      = Data->Charisma;
   947: 	CharacterAttribute->Life          = Data->Life;
   948: 	CharacterAttribute->LifeMax       = Data->LifeMax;
   949: 	CharacterAttribute->Mana          = Data->Mana;
   950: 	CharacterAttribute->ManaMax       = Data->ManaMax;
   951: 	CharacterAttribute->SkillMana     = Data->SkillMana;
   952: 	CharacterAttribute->SkillManaMax  = Data->SkillManaMax;
   953: 	CharacterAttribute->Ability		  = 0;
   954: 	CharacterAttribute->AbilityTime[0]= 0;
   955: 	CharacterAttribute->AbilityTime[1]= 0;
   956: 	CharacterAttribute->AbilityTime[2]= 0;
   957: 	CharacterAttribute->Shield		  = Data->Shield;
   958: 	CharacterAttribute->ShieldMax	  = Data->ShieldMax;
   959:     CharacterAttribute->AddPoint		= Data->AddPoint;
   960:     CharacterAttribute->MaxAddPoint		= Data->MaxAddPoint;
   961: 	CharacterAttribute->wMinusPoint     = Data->wMinusPoint;
   962:     CharacterAttribute->wMaxMinusPoint  = Data->wMaxMinusPoint;
   963: 
   964: 	CharacterMachine->Gold            = Data->Gold;
   965: 	//CharacterAttribute->SkillMana     = CharacterAttribute->Energy*10+CharacterAttribute->ManaMax*10/6;
   966: 
   967: 	gMapManager.WorldActive = Data->Map;
   968: 
   969: 	gMapManager.LoadWorld(gMapManager.WorldActive);
   970: 
   971: 	if(gMapManager.WorldActive == WD_34CRYWOLF_1ST)
   972: 	{
   973: 		SendRequestCrywolfInfo();
   974: 	}
   975: 
   976:     matchEvent::CreateEventMatch ( gMapManager.WorldActive );
   977: 
   978: 	HeroIndex = rand()%MAX_CHARACTERS_CLIENT;
   979: 	CHARACTER *c = &CharactersClient[HeroIndex];
   980: 	CreateCharacterPointer(c,MODEL_PLAYER,Data->PositionX,Data->PositionY,((float)Data->Angle-1.f)*45.f);
   981: 	c->Key = HeroKey;
   982: 
   983: 	g_ConsoleDebug->Write(MCD_RECEIVE, "0x03 [ReceiveJoinMapServer] Key: %d Map: %d X: %d Y:%d",c->Key,gMapManager.WorldActive,Data->PositionX,Data->PositionY);
   984: 
   985:     OBJECT *o = &c->Object;
   986: 	c->Class = CharacterAttribute->Class;
   987: 	c->Skin = 0;
   988: 	c->PK    = Data->PK;
   989: 	c->CtlCode	= Data->CtlCode;
   990: 	o->Kind  = KIND_PLAYER;
   991:    	SetCharacterClass(c);
   992: 
   993: 	Hero = c;
   994: 
   995: 	memcpy(c->ID,(char *)CharacterAttribute->Name,MAX_ID_SIZE);
   996: 
   997:     for ( int i=0; i<MAX_EQUIPMENT; ++i )
   998:     {
   999:         CharacterMachine->Equipment[i].Type = -1;
  1000:         CharacterMachine->Equipment[i].Level = 0;
  1001:         CharacterMachine->Equipment[i].Option1 = 0;
  1002:     }
  1003: 	c->ID[MAX_ID_SIZE] = NULL;
  1004: 	CreateEffect(BITMAP_MAGIC+2,o->Position,o->Angle,o->Light,0,o);
  1005: 	CurrentProtocolState = RECEIVE_JOIN_MAP_SERVER;
  1006: 
  1007:     LockInputStatus = false;
  1008:     CheckIME_Status(true,0);
  1009: 
  1010:     LoadingWorld = 30;
  1011:     MouseUpdateTime = 0;
  1012:     MouseUpdateTimeMax = 6;
  1013: 
  1014:     CreatePetDarkSpirit_Now ( Hero );
  1015: 
  1016: 	CreateEffect(BITMAP_MAGIC+2,o->Position,o->Angle,o->Light,0,o);
  1017:     o->Alpha = 0.f;
  1018: 
  1019: 	g_pNewUISystem->HideAll();
  1020: 
  1021:     SelectedItem		= -1;
  1022:     SelectedNpc			= -1;
  1023:     SelectedCharacter	= -1;
  1024:     SelectedOperate		= -1;
  1025:     Attacking			= -1;
  1026:     RepairEnable		= 0;
  1027: 
  1028:     Hero->Movement = false;
  1029:     SetPlayerStop(Hero);
  1030: 
  1031:     if ( gMapManager.InBloodCastle() == false )
  1032:     {
  1033:         StopBuffer ( SOUND_BLOODCASTLE, true );
  1034:     }
  1035: 
  1036:     if ( gMapManager.InChaosCastle() == false )
  1037:     {
  1038:         StopBuffer ( SOUND_CHAOSCASTLE, true );
  1039:         StopBuffer ( SOUND_CHAOS_ENVIR, true );
  1040:     }
  1041: 
  1042: 	if( gMapManager.IsEmpireGuardian1() == false &&
  1043: 		gMapManager.IsEmpireGuardian2() == false &&
  1044: 		gMapManager.IsEmpireGuardian3() == false &&
  1045: 		gMapManager.IsEmpireGuardian4() == false )
  1046: 	{
  1047: 		StopBuffer(SOUND_EMPIREGUARDIAN_WEATHER_RAIN, true);
  1048: 		StopBuffer(SOUND_EMPIREGUARDIAN_WEATHER_FOG, true);
  1049: 		StopBuffer(SOUND_EMPIREGUARDIAN_WEATHER_STORM, true);
  1050: 		StopBuffer(SOUND_EMPIREGUARDIAN_INDOOR_SOUND, true);
  1051: 	}
  1052: 
  1053: 	g_pUIMapName->ShowMapName();
  1054: 
  1055: 	CreateMyGensInfluenceGroundEffect();
  1056: 
  1057: 	if (gMapManager.WorldActive >= WD_65DOPPLEGANGER1 && gMapManager.WorldActive <= WD_68DOPPLEGANGER4);
  1058: 	else
  1059: 	{
  1060: 		char Text[256];
  1061: 		sprintf(Text,"%s%s",GlobalText[484],gMapManager.GetMapName(gMapManager.WorldActive));
  1062: 
  1063: 		g_pChatListBox->AddText("", Text, SEASON3B::TYPE_SYSTEM_MESSAGE);
  1064: 	}
  1065: 
  1066: 	if( gMapManager.WorldActive == WD_30BATTLECASTLE )
  1067: 	{
  1068: 		if( g_pSiegeWarfare )
  1069: 			g_pSiegeWarfare->CreateMiniMapUI();
  1070: 	}
  1071: 
  1072: 	if( gMapManager.WorldActive < WD_65DOPPLEGANGER1 || gMapManager.WorldActive > WD_68DOPPLEGANGER4 )
  1073: 	{
  1074: 		g_pNewUISystem->Hide(SEASON3B::INTERFACE_DOPPELGANGER_FRAME);
  1075: 	}
  1076: 
  1077: 	if( gMapManager.WorldActive < WD_69EMPIREGUARDIAN1 || WD_72EMPIREGUARDIAN4 < gMapManager.WorldActive)
  1078: 	{
  1079: 		g_pNewUISystem->Hide(SEASON3B::INTERFACE_EMPIREGUARDIAN_TIMER);
  1080: 	}
  1081: 
  1082: 	g_ConsoleDebug->Write(MCD_RECEIVE, "0x03 [ReceiveJoinMapServer]");
  1083: 
  1084: 	return ( TRUE);
  1085: }
  1086: 
  1087: void ReceiveRevival( BYTE *ReceiveBuffer )
  1088: {
  1089: 	MouseLButton = false;
  1090: 	Teleport = false;
  1091: 	Hero->Object.Live = false;
  1092: 	LPPRECEIVE_REVIVAL Data = (LPPRECEIVE_REVIVAL)ReceiveBuffer;
  1093: 
  1094: 	CharacterAttribute->Life       = Data->Life;
  1095: 	CharacterAttribute->Mana       = Data->Mana;
  1096: 	CharacterAttribute->Shield	   = Data->Shield;
  1097: 	CharacterAttribute->SkillMana  = Data->SkillMana;
  1098: 
  1099: 	__int64 Data_Exp = 0x0000000000000000;
  1100: 	Master_Level_Data.lMasterLevel_Experince = 0x0000000000000000;
  1101: 	Data_Exp |= Data->btMExp1;
  1102: 	Data_Exp <<= 8;
  1103: 	Data_Exp |= Data->btMExp2;
  1104: 	Data_Exp <<= 8;
  1105: 	Data_Exp |= Data->btMExp3;
  1106: 	Data_Exp <<= 8;
  1107: 	Data_Exp |= Data->btMExp4;
  1108: 	Data_Exp <<= 8;
  1109: 	Data_Exp |= Data->btMExp5;
  1110: 	Data_Exp <<= 8;
  1111: 	Data_Exp |= Data->btMExp6;
  1112: 	Data_Exp <<= 8;
  1113: 	Data_Exp |= Data->btMExp7;
  1114: 	Data_Exp <<= 8;
  1115: 	Data_Exp |= Data->btMExp8;
  1116: 
  1117: 
  1118: 	if(gCharacterManager.IsMasterLevel(Hero->Class) == true)
  1119: 	{
  1120: 		Master_Level_Data.lMasterLevel_Experince = Data_Exp;
  1121: 	}
  1122: 	else
  1123: 	{
  1124: 		CharacterAttribute->Experience = (int)Data_Exp;
  1125: 	}
  1126: 
  1127: 	CharacterMachine->Gold         = Data->Gold;
  1128: 	for(int i=0;i<MAX_CHARACTERS_CLIENT;i++)
  1129: 	{
  1130: 		CHARACTER *c = &CharactersClient[i];
  1131: 		c->Object.Live = false;
```


---

## Function body: ReceiveJoinMapServer

- region 871..1085
```
   851:         EnableMainRender = false;
   852: 		CurrentProtocolState = REQUEST_JOIN_SERVER;
   853: 
   854:         LogIn = 0;
   855:         g_csMapServer.Init ();
   856: 		InitGame();
   857: 		break;
   858: 	}
   859: 
   860: 	g_pWindowMgr->Reset();
   861: 	g_pFriendList->ClearFriendList();
   862: 	g_pLetterList->ClearLetterList();
   863: 
   864: 	g_ConsoleDebug->Write(MCD_RECEIVE, "0x02 [ReceiveServerList(%d)]", Data->Value);
   865: 
   866: 	return ( TRUE);
   867: }
   868: 
   869: int HeroIndex;
   870: 
   871: BOOL ReceiveJoinMapServer(BYTE *ReceiveBuffer, BOOL bEncrypted)
   872: {
   873: //#ifndef NEW_PROTOCOL_SYSTEM
   874: //	if ( !bEncrypted)
   875: //	{
   876: //		GO_DEBUG;
   877: //		SendHackingChecked( 0x00, 0xF3);
   878: //		return ( FALSE);
   879: //	}
   880: //#endif
   881: 	MouseLButton = false;
   882: 
   883: 	LPPRECEIVE_JOIN_MAP_SERVER Data = (LPPRECEIVE_JOIN_MAP_SERVER)ReceiveBuffer;
   884: 
   885: 
   886: 	__int64 Data_Exp = 0x0000000000000000;
   887: 	Master_Level_Data.lMasterLevel_Experince = 0x0000000000000000;
   888: 	Data_Exp |= Data->btMExp1;
   889: 	Data_Exp <<= 8;
   890: 	Data_Exp |= Data->btMExp2;
   891: 	Data_Exp <<= 8;
   892: 	Data_Exp |= Data->btMExp3;
   893: 	Data_Exp <<= 8;
   894: 	Data_Exp |= Data->btMExp4;
   895: 	Data_Exp <<= 8;
   896: 	Data_Exp |= Data->btMExp5;
   897: 	Data_Exp <<= 8;
   898: 	Data_Exp |= Data->btMExp6;
   899: 	Data_Exp <<= 8;
   900: 	Data_Exp |= Data->btMExp7;
   901: 	Data_Exp <<= 8;
   902: 	Data_Exp |= Data->btMExp8;
   903: 
   904: 	if(gCharacterManager.IsMasterLevel(CharacterAttribute->Class) == true)
   905: 	{
   906: 		Master_Level_Data.lMasterLevel_Experince = Data_Exp;
   907: 	}
   908: 	else
   909: 	{
   910: 		CharacterAttribute->Experience = (int)Data_Exp;
   911: 	}
   912: 
   913: 	Data_Exp = 0x0000000000000000;
   914: 	Master_Level_Data.lNext_MasterLevel_Experince = 0x0000000000000000;
   915: 
   916: 	Data_Exp |= Data->btMNextExp1;
   917: 	Data_Exp <<= 8;
   918: 	Data_Exp |= Data->btMNextExp2;
   919: 	Data_Exp <<= 8;
   920: 	Data_Exp |= Data->btMNextExp3;
   921: 	Data_Exp <<= 8;
   922: 	Data_Exp |= Data->btMNextExp4;
   923: 	Data_Exp <<= 8;
   924: 	Data_Exp |= Data->btMNextExp5;
   925: 	Data_Exp <<= 8;
   926: 	Data_Exp |= Data->btMNextExp6;
   927: 	Data_Exp <<= 8;
   928: 	Data_Exp |= Data->btMNextExp7;
   929: 	Data_Exp <<= 8;
   930: 	Data_Exp |= Data->btMNextExp8;
   931: 
   932: 	if(gCharacterManager.IsMasterLevel(CharacterAttribute->Class) == true)
   933: 	{
   934: 		Master_Level_Data.lNext_MasterLevel_Experince = Data_Exp;
   935: 	}
   936: 	else
   937: 	{
   938: 		CharacterAttribute->NextExperince = (int)Data_Exp;
   939: 	}
   940: 
   941: 	CharacterAttribute->LevelUpPoint  = Data->LevelUpPoint;
   942: 	CharacterAttribute->Strength      = Data->Strength;
   943: 	CharacterAttribute->Dexterity     = Data->Dexterity;
   944: 	CharacterAttribute->Vitality      = Data->Vitality;
   945: 	CharacterAttribute->Energy        = Data->Energy;
   946:     CharacterAttribute->Charisma      = Data->Charisma;
   947: 	CharacterAttribute->Life          = Data->Life;
   948: 	CharacterAttribute->LifeMax       = Data->LifeMax;
   949: 	CharacterAttribute->Mana          = Data->Mana;
   950: 	CharacterAttribute->ManaMax       = Data->ManaMax;
   951: 	CharacterAttribute->SkillMana     = Data->SkillMana;
   952: 	CharacterAttribute->SkillManaMax  = Data->SkillManaMax;
   953: 	CharacterAttribute->Ability		  = 0;
   954: 	CharacterAttribute->AbilityTime[0]= 0;
   955: 	CharacterAttribute->AbilityTime[1]= 0;
   956: 	CharacterAttribute->AbilityTime[2]= 0;
   957: 	CharacterAttribute->Shield		  = Data->Shield;
   958: 	CharacterAttribute->ShieldMax	  = Data->ShieldMax;
   959:     CharacterAttribute->AddPoint		= Data->AddPoint;
   960:     CharacterAttribute->MaxAddPoint		= Data->MaxAddPoint;
   961: 	CharacterAttribute->wMinusPoint     = Data->wMinusPoint;
   962:     CharacterAttribute->wMaxMinusPoint  = Data->wMaxMinusPoint;
   963: 
   964: 	CharacterMachine->Gold            = Data->Gold;
   965: 	//CharacterAttribute->SkillMana     = CharacterAttribute->Energy*10+CharacterAttribute->ManaMax*10/6;
   966: 
   967: 	gMapManager.WorldActive = Data->Map;
   968: 
   969: 	gMapManager.LoadWorld(gMapManager.WorldActive);
   970: 
   971: 	if(gMapManager.WorldActive == WD_34CRYWOLF_1ST)
   972: 	{
   973: 		SendRequestCrywolfInfo();
   974: 	}
   975: 
   976:     matchEvent::CreateEventMatch ( gMapManager.WorldActive );
   977: 
   978: 	HeroIndex = rand()%MAX_CHARACTERS_CLIENT;
   979: 	CHARACTER *c = &CharactersClient[HeroIndex];
   980: 	CreateCharacterPointer(c,MODEL_PLAYER,Data->PositionX,Data->PositionY,((float)Data->Angle-1.f)*45.f);
   981: 	c->Key = HeroKey;
   982: 
   983: 	g_ConsoleDebug->Write(MCD_RECEIVE, "0x03 [ReceiveJoinMapServer] Key: %d Map: %d X: %d Y:%d",c->Key,gMapManager.WorldActive,Data->PositionX,Data->PositionY);
   984: 
   985:     OBJECT *o = &c->Object;
   986: 	c->Class = CharacterAttribute->Class;
   987: 	c->Skin = 0;
   988: 	c->PK    = Data->PK;
   989: 	c->CtlCode	= Data->CtlCode;
   990: 	o->Kind  = KIND_PLAYER;
   991:    	SetCharacterClass(c);
   992: 
   993: 	Hero = c;
   994: 
   995: 	memcpy(c->ID,(char *)CharacterAttribute->Name,MAX_ID_SIZE);
   996: 
   997:     for ( int i=0; i<MAX_EQUIPMENT; ++i )
   998:     {
   999:         CharacterMachine->Equipment[i].Type = -1;
  1000:         CharacterMachine->Equipment[i].Level = 0;
  1001:         CharacterMachine->Equipment[i].Option1 = 0;
  1002:     }
  1003: 	c->ID[MAX_ID_SIZE] = NULL;
  1004: 	CreateEffect(BITMAP_MAGIC+2,o->Position,o->Angle,o->Light,0,o);
  1005: 	CurrentProtocolState = RECEIVE_JOIN_MAP_SERVER;
  1006: 
  1007:     LockInputStatus = false;
  1008:     CheckIME_Status(true,0);
  1009: 
  1010:     LoadingWorld = 30;
  1011:     MouseUpdateTime = 0;
  1012:     MouseUpdateTimeMax = 6;
  1013: 
  1014:     CreatePetDarkSpirit_Now ( Hero );
  1015: 
  1016: 	CreateEffect(BITMAP_MAGIC+2,o->Position,o->Angle,o->Light,0,o);
  1017:     o->Alpha = 0.f;
  1018: 
  1019: 	g_pNewUISystem->HideAll();
  1020: 
  1021:     SelectedItem		= -1;
  1022:     SelectedNpc			= -1;
  1023:     SelectedCharacter	= -1;
  1024:     SelectedOperate		= -1;
  1025:     Attacking			= -1;
  1026:     RepairEnable		= 0;
  1027: 
  1028:     Hero->Movement = false;
  1029:     SetPlayerStop(Hero);
  1030: 
  1031:     if ( gMapManager.InBloodCastle() == false )
  1032:     {
  1033:         StopBuffer ( SOUND_BLOODCASTLE, true );
  1034:     }
  1035: 
  1036:     if ( gMapManager.InChaosCastle() == false )
  1037:     {
  1038:         StopBuffer ( SOUND_CHAOSCASTLE, true );
  1039:         StopBuffer ( SOUND_CHAOS_ENVIR, true );
  1040:     }
  1041: 
  1042: 	if( gMapManager.IsEmpireGuardian1() == false &&
  1043: 		gMapManager.IsEmpireGuardian2() == false &&
  1044: 		gMapManager.IsEmpireGuardian3() == false &&
  1045: 		gMapManager.IsEmpireGuardian4() == false )
  1046: 	{
  1047: 		StopBuffer(SOUND_EMPIREGUARDIAN_WEATHER_RAIN, true);
  1048: 		StopBuffer(SOUND_EMPIREGUARDIAN_WEATHER_FOG, true);
  1049: 		StopBuffer(SOUND_EMPIREGUARDIAN_WEATHER_STORM, true);
  1050: 		StopBuffer(SOUND_EMPIREGUARDIAN_INDOOR_SOUND, true);
  1051: 	}
  1052: 
  1053: 	g_pUIMapName->ShowMapName();
  1054: 
  1055: 	CreateMyGensInfluenceGroundEffect();
  1056: 
  1057: 	if (gMapManager.WorldActive >= WD_65DOPPLEGANGER1 && gMapManager.WorldActive <= WD_68DOPPLEGANGER4);
  1058: 	else
  1059: 	{
  1060: 		char Text[256];
  1061: 		sprintf(Text,"%s%s",GlobalText[484],gMapManager.GetMapName(gMapManager.WorldActive));
  1062: 
  1063: 		g_pChatListBox->AddText("", Text, SEASON3B::TYPE_SYSTEM_MESSAGE);
  1064: 	}
  1065: 
  1066: 	if( gMapManager.WorldActive == WD_30BATTLECASTLE )
  1067: 	{
  1068: 		if( g_pSiegeWarfare )
  1069: 			g_pSiegeWarfare->CreateMiniMapUI();
  1070: 	}
  1071: 
  1072: 	if( gMapManager.WorldActive < WD_65DOPPLEGANGER1 || gMapManager.WorldActive > WD_68DOPPLEGANGER4 )
  1073: 	{
  1074: 		g_pNewUISystem->Hide(SEASON3B::INTERFACE_DOPPELGANGER_FRAME);
  1075: 	}
  1076: 
  1077: 	if( gMapManager.WorldActive < WD_69EMPIREGUARDIAN1 || WD_72EMPIREGUARDIAN4 < gMapManager.WorldActive)
  1078: 	{
  1079: 		g_pNewUISystem->Hide(SEASON3B::INTERFACE_EMPIREGUARDIAN_TIMER);
  1080: 	}
  1081: 
  1082: 	g_ConsoleDebug->Write(MCD_RECEIVE, "0x03 [ReceiveJoinMapServer]");
  1083: 
  1084: 	return ( TRUE);
  1085: }
  1086: 
  1087: void ReceiveRevival( BYTE *ReceiveBuffer )
  1088: {
  1089: 	MouseLButton = false;
  1090: 	Teleport = false;
  1091: 	Hero->Object.Live = false;
  1092: 	LPPRECEIVE_REVIVAL Data = (LPPRECEIVE_REVIVAL)ReceiveBuffer;
  1093: 
  1094: 	CharacterAttribute->Life       = Data->Life;
  1095: 	CharacterAttribute->Mana       = Data->Mana;
  1096: 	CharacterAttribute->Shield	   = Data->Shield;
  1097: 	CharacterAttribute->SkillMana  = Data->SkillMana;
  1098: 
  1099: 	__int64 Data_Exp = 0x0000000000000000;
  1100: 	Master_Level_Data.lMasterLevel_Experince = 0x0000000000000000;
  1101: 	Data_Exp |= Data->btMExp1;
  1102: 	Data_Exp <<= 8;
  1103: 	Data_Exp |= Data->btMExp2;
  1104: 	Data_Exp <<= 8;
  1105: 	Data_Exp |= Data->btMExp3;
  1106: 	Data_Exp <<= 8;
  1107: 	Data_Exp |= Data->btMExp4;
  1108: 	Data_Exp <<= 8;
  1109: 	Data_Exp |= Data->btMExp5;
  1110: 	Data_Exp <<= 8;
  1111: 	Data_Exp |= Data->btMExp6;
  1112: 	Data_Exp <<= 8;
  1113: 	Data_Exp |= Data->btMExp7;
  1114: 	Data_Exp <<= 8;
  1115: 	Data_Exp |= Data->btMExp8;
  1116: 
  1117: 
  1118: 	if(gCharacterManager.IsMasterLevel(Hero->Class) == true)
  1119: 	{
  1120: 		Master_Level_Data.lMasterLevel_Experince = Data_Exp;
  1121: 	}
  1122: 	else
  1123: 	{
  1124: 		CharacterAttribute->Experience = (int)Data_Exp;
  1125: 	}
  1126: 
  1127: 	CharacterMachine->Gold         = Data->Gold;
  1128: 	for(int i=0;i<MAX_CHARACTERS_CLIENT;i++)
  1129: 	{
  1130: 		CHARACTER *c = &CharactersClient[i];
  1131: 		c->Object.Live = false;
  1132: 	}
  1133: 
  1134: 	BYTE Temp = Hero->PK;
  1135: 	int TempGuild = Hero->GuildMarkIndex;
  1136: 
  1137: 	CHARACTER *c = &CharactersClient[HeroIndex];
  1138: 
  1139: 	BYTE BackUpGuildStatus = c->GuildStatus;
  1140: 	BYTE BackUpGuildType = c->GuildType;
  1141: 	BYTE BackUpGuildRelationShip = c->GuildRelationShip;
  1142: 	BYTE byBackupEtcPart = c->EtcPart;
  1143: 
  1144: 	CreateCharacterPointer(c,MODEL_PLAYER,Data->PositionX,Data->PositionY,((float)Data->Angle-1.f)*45.f);
  1145: 	c->Key = HeroKey;
  1146: 	c->GuildStatus = BackUpGuildStatus;
  1147: 	c->GuildType = BackUpGuildType;
  1148: 	c->GuildRelationShip = BackUpGuildRelationShip;
  1149: 	c->EtcPart = byBackupEtcPart;
  1150: 
  1151: 	OBJECT *o = &c->Object;
  1152: 	c->Class    = CharacterAttribute->Class;
  1153: 	c->Skin = 0;
  1154: 	c->PK       = Temp;
  1155: 	o->Kind     = KIND_PLAYER;
  1156: 	c->GuildMarkIndex = TempGuild;
  1157: 	c->SafeZone = true;
  1158: 	SetCharacterClass(c);
  1159: 
  1160: #ifdef PK_ATTACK_TESTSERVER_LOG
  1161: 	PrintPKLog(c);
  1162: #endif // PK_ATTACK_TESTSERVER_LOG
  1163: 
  1164: 	SetPlayerStop(c);
  1165: 	CreateEffect(BITMAP_MAGIC+2,o->Position,o->Angle,o->Light,0,o);
  1166:     ClearItems();
  1167: 	ClearCharacters(HeroKey);
  1168: 	RemoveAllShopTitleExceptHero();
  1169: 
  1170: 	if( gMapManager.WorldActive >= WD_45CURSEDTEMPLE_LV1 && gMapManager.WorldActive <= WD_45CURSEDTEMPLE_LV6 )
  1171: 	{
  1172: 		if( !(Data->Map >= WD_45CURSEDTEMPLE_LV1 && Data->Map <= WD_45CURSEDTEMPLE_LV6) )
  1173: 		{
  1174: 			g_CursedTemple->ResetCursedTemple();
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
```


---

## WSclient.h — struct tokens

### TOKEN `\bJOIN_MAP_SERVER\b`

- no hits


### TOKEN `\bJoinMapServer\b`

- no hits


### TOKEN `\bPRECEIVE_JOIN_MAP_SERVER\b`

- hit @ 452: `} PRECEIVE_JOIN_MAP_SERVER, * LPPRECEIVE_JOIN_MAP_SERVER;`
```
   392: 	BYTE         SubCode;
   393: 	BYTE         ID[MAX_ID_SIZE];
   394: 	BYTE         Resident[10];
   395: } PREQUEST_DELETE_CHARACTER, * LPREQUEST_DELETE_CHARACTER;
   396: 
   397: //request join map server
   398: typedef struct
   399: {
   400: 	PBMSG_HEADER Header;
   401: 	BYTE		 SubCode;
   402: 	BYTE         ID[MAX_ID_SIZE];
   403: } PREQUEST_JOIN_MAP_SERVER, * LPPREQUEST_JOIN_MAP_SERVER;
   404: 
   405: //receive join map server
   406: typedef struct
   407: {
   408: 	PBMSG_HEADER Header;
   409: 	BYTE         SubCode;
   410: 	BYTE         PositionX;
   411: 	BYTE         PositionY;
   412: 	BYTE         Map;
   413: 	BYTE         Angle;
   414: 	BYTE		btMExp1;
   415: 	BYTE		btMExp2;
   416: 	BYTE		btMExp3;
   417: 	BYTE		btMExp4;
   418: 	BYTE		btMExp5;
   419: 	BYTE		btMExp6;
   420: 	BYTE		btMExp7;
   421: 	BYTE		btMExp8;
   422: 	BYTE		btMNextExp1;
   423: 	BYTE		btMNextExp2;
   424: 	BYTE		btMNextExp3;
   425: 	BYTE		btMNextExp4;
   426: 	BYTE		btMNextExp5;
   427: 	BYTE		btMNextExp6;
   428: 	BYTE		btMNextExp7;
   429: 	BYTE		btMNextExp8;
   430: 	WORD         LevelUpPoint;
   431: 	//BYTE         LevelUpPoint;
   432: 	WORD         Strength;
   433: 	WORD         Dexterity;
   434: 	WORD         Vitality;
   435: 	WORD         Energy;
   436: 	WORD         Life;
   437: 	WORD         LifeMax;
   438: 	WORD         Mana;
   439: 	WORD         ManaMax;
   440: 	WORD		 Shield;
   441: 	WORD		 ShieldMax;
   442: 	WORD		 SkillMana;
   443: 	WORD		 SkillManaMax;
   444: 	DWORD        Gold;
   445: 	BYTE         PK;
   446: 	BYTE		 CtlCode;
   447:     short        AddPoint;
   448:     short        MaxAddPoint;
   449:     WORD         Charisma;
   450: 	WORD		 wMinusPoint;
   451: 	WORD		 wMaxMinusPoint;
   452: } PRECEIVE_JOIN_MAP_SERVER, * LPPRECEIVE_JOIN_MAP_SERVER;
   453: 
   454: //receive revival
   455: typedef struct
   456: {
   457: 	PBMSG_HEADER Header;
   458: 	BYTE         SubCode;
   459: 	BYTE         PositionX;
   460: 	BYTE         PositionY;
   461: 	BYTE         Map;
   462: 	BYTE         Angle;
   463: 	WORD         Life;
   464: 	WORD         Mana;
   465: 	WORD		 Shield;
   466: 	WORD		 SkillMana;
   467: 	BYTE		btMExp1;
   468: 	BYTE		btMExp2;
   469: 	BYTE		btMExp3;
   470: 	BYTE		btMExp4;
   471: 	BYTE		btMExp5;
   472: 	BYTE		btMExp6;
   473: 	BYTE		btMExp7;
   474: 	BYTE		btMExp8;
   475: 
   476: 	DWORD        Gold;
   477: } PRECEIVE_REVIVAL, * LPPRECEIVE_REVIVAL;
   478: 
   479: //inventory
   480: typedef struct {
   481: 	BYTE          Index;
   482: 	BYTE          Item[PACKET_ITEM_LENGTH];
   483: } PRECEIVE_INVENTORY, * LPPRECEIVE_INVENTORY;
   484: 
   485: 
   486: ///////////////////////////////////////////////////////////////////////////////
   487: // trade
   488: ///////////////////////////////////////////////////////////////////////////////
   489: typedef struct {
   490: 	PBMSG_HEADER Header;
   491: 	BYTE         SubCode;
   492: 	BYTE         ID[MAX_ID_SIZE];
   493:     WORD         Level;
   494:     DWORD        GuildKey;
   495: } PTRADE, * LPPTRADE;
   496: 
   497: 
   498: ///////////////////////////////////////////////////////////////////////////////
   499: // game
   500: ///////////////////////////////////////////////////////////////////////////////
   501: 
   502: //request chat
   503: typedef struct {
   504: 	PBMSG_HEADER Header;
   505: 	BYTE         ID[MAX_ID_SIZE];
   506: 	BYTE         ChatText[MAX_CHAT_SIZE];
   507: } PCHATING, * LPPCHATING;
   508: 
   509: typedef struct {
   510: 	PBMSG_HEADER Header;
   511: 	BYTE         KeyH;
   512: 	BYTE         KeyL;
   513: 	BYTE         ChatText[MAX_CHAT_SIZE];
   514: } PCHATING_KEY, * LPPCHATING_KEY;
   515: 
   516: typedef struct {
   517: 	PBMSG_HEADER Header;
   518: 	BYTE         Result;
   519: 	BYTE		 Count;
   520: 	WORD		 Delay;
   521: 	DWORD		 Color;
   522: 	BYTE		 Speed;
   523: 	BYTE         Notice[256];
   524: } PRECEIVE_NOTICE, * LPPRECEIVE_NOTICE;
   525: 
   526: //receive equipment
   527: typedef struct {
   528: 	PBMSG_HEADER Header;
   529: 	BYTE         SubCode;
   530: 	BYTE         KeyH;
   531: 	BYTE         KeyL;
   532: 	BYTE         Class;
   533: 	BYTE         Equipment[EQUIPMENT_LENGTH];
   534: } PRECEIVE_EQUIPMENT, * LPPRECEIVE_EQUIPMENT;
   535: 
   536: //receive other map character
   537: typedef struct {
   538: 	BYTE         KeyH;
   539: 	BYTE         KeyL;
   540: 	BYTE         PositionX;
   541: 	BYTE         PositionY;
   542: 	BYTE         Class;
   543: 	BYTE         Equipment[EQUIPMENT_LENGTH];
   544: 	BYTE         ID[MAX_ID_SIZE];
   545: 	BYTE         TargetX;
   546: 	BYTE         TargetY;
   547: 	BYTE         Path;
   548: 	BYTE         s_BuffCount;
   549: 	BYTE		 s_BuffEffectState[MAX_BUFF_SLOT_INDEX];
   550: } PCREATE_CHARACTER, * LPPCREATE_CHARACTER;
   551: 
   552: //receive other map character
   553: typedef struct
   554: {
   555: 	BYTE         KeyH;
   556: 	BYTE         KeyL;
   557: 	BYTE         PositionX;
   558: 	BYTE         PositionY;
   559: 	BYTE         TypeH;
   560: 	BYTE         TypeL;
   561: 	BYTE         ID[MAX_ID_SIZE];
   562: 	BYTE         TargetX;
   563: 	BYTE         TargetY;
   564: 	BYTE         Path;
   565: 	BYTE         Class;
   566: 	BYTE         Equipment[EQUIPMENT_LENGTH];
   567: 	BYTE         s_BuffCount;
   568: 	BYTE		 s_BuffEffectState[MAX_BUFF_SLOT_INDEX];
   569: } PCREATE_TRANSFORM, * LPPCREATE_TRANSFORM;
   570: 
   571: //receive other map character
   572: typedef struct {
   573: 	BYTE         KeyH;
   574: 	BYTE         KeyL;
   575: 	BYTE         TypeH;
   576: 	BYTE         TypeL;
   577: 	BYTE         PositionX;
   578: 	BYTE         PositionY;
   579: 	BYTE         TargetX;
   580: 	BYTE         TargetY;
   581: 	BYTE         Path;
   582: 	BYTE         ID[MAX_ID_SIZE];
   583: 	BYTE         s_BuffCount;
   584: 	BYTE		 s_BuffEffectState[MAX_BUFF_SLOT_INDEX];
   585: } PCREATE_SUMMON, * LPPCREATE_SUMMON;
   586: 
   587: //receive other map character
   588: typedef struct {
   589: 	BYTE         KeyH;
   590: 	BYTE         KeyL;
   591: 	BYTE         TypeH;
   592: 	BYTE         TypeL;
   593: 	BYTE         PositionX;
   594: 	BYTE         PositionY;
   595: 	BYTE         TargetX;
   596: 	BYTE         TargetY;
   597: 	BYTE         Path;
   598: 	BYTE         s_BuffCount;
   599: 	BYTE		 s_BuffEffectState[MAX_BUFF_SLOT_INDEX];
   600: } PCREATE_MONSTER, * LPPCREATE_MONSTER;
   601: 
   602: //send move my character
   603: typedef struct {
   604: 	PBMSG_HEADER  Header;
   605: 	BYTE          PositionX;
   606: 	BYTE          PositionY;
   607: 	BYTE          Path[8];
   608: } PMOVE_MY_CHARACTER, * LPPMOVE_MY_CHARACTER;
   609: 
   610: //receive move character
   611: typedef struct {
   612: #ifndef NEW_PROTOCOL_SYSTEM
   613: 	PBMSG_HEADER  Header;
   614: #endif
   615: 	BYTE          KeyH;
   616: 	BYTE          KeyL;
   617: 	BYTE          PositionX;
   618: 	BYTE          PositionY;
   619: 	BYTE          Path[1];
   620: } PMOVE_CHARACTER, * LPPMOVE_CHARACTER;
   621: 
   622: //delete character and item
   623: typedef struct {
   624: 	BYTE         KeyH;
   625: 	BYTE         KeyL;
   626: } PDELETE_CHARACTER, * LPPDELETE_CHARACTER;
   627: 
   628: //create item
   629: typedef struct {
   630: 	BYTE          KeyH;
   631: 	BYTE          KeyL;
   632: 	BYTE          PositionX;
   633: 	BYTE          PositionY;
   634: 	BYTE          Item[PACKET_ITEM_LENGTH];
   635: } PCREATE_ITEM, * LPPCREATE_ITEM;
   636: 
   637: //request drop item
   638: typedef struct {
   639: 	PBMSG_HEADER  Header;
   640: 	BYTE          PositionX;
   641: 	BYTE          PositionY;
   642: 	BYTE          InventoryIndex;
   643: } PREQUEST_DROP_ITEM, * LPPREQUEST_DROP_ITEM;
   644: 
   645: //request drop item
   646: typedef struct {
   647: 	PBMSG_HEADER  Header;
   648: 	BYTE          SrcIndex;
   649: 	BYTE          DstIndex;
   650: } PREQUEST_EQUIPMENT_ITEM, * LPPREQUEST_EQUIPMENT_ITEM;
   651: 
   652: //change character
   653: typedef struct {
   654: 	PBMSG_HEADER  Header;
   655: 	BYTE          KeyH;
   656: 	BYTE          KeyL;
   657: 	BYTE          Item[PACKET_ITEM_LENGTH];
   658: } PCHANGE_CHARACTER, * LPPCHANGE_CHARACTER;
   659: 
   660: typedef struct {
   661: 	PBMSG_HEADER  Header;
   662: 	BYTE          KeyH;
   663: 	BYTE          KeyL;
   664: 	BYTE          Action;
   665: 	BYTE          Dir;
   666: } PREQUEST_ATTACK, * LPPREQUEST_ATTACK;
   667: 
   668: //receive get item
   669: typedef struct {
   670: 	PBMSG_HEADER  Header;
   671: 	BYTE          Result;
   672: 	BYTE          Item[PACKET_ITEM_LENGTH];
   673: } PRECEIVE_GET_ITEM, * LPPRECEIVE_GET_ITEM;
   674: 
   675: //receive attack
   676: typedef struct {
   677: 	PBMSG_HEADER  Header;
   678: 	BYTE          KeyH;
   679: 	BYTE          KeyL;
   680: 	BYTE          DamageH;
   681: 	BYTE          DamageL;
   682: 	BYTE		  DamageType;
   683: 	BYTE		  ShieldDamageH;
   684: 	BYTE		  ShieldDamageL;
   685: } PRECEIVE_ATTACK, * LPPRECEIVE_ATTACK;
   686: 
   687: //receive die
   688: typedef struct {
   689: 	PBMSG_HEADER  Header;
   690: 	BYTE          KeyH;
   691: 	BYTE          KeyL;
   692: 	BYTE          ExpH;
```

### TOKEN `\bPMSG_.*F3.*03\b`

- no hits


### TOKEN `\bCHARACTER_INFO\b`

- no hits


### TOKEN `\bPMSG_CHARACTER_INFO\b`

- no hits


### TOKEN `\bPRECEIVE_CHARACTER\b`

- no hits



---

## QUESTIONS

1) O client trata `F3:03` como JoinMapServer ou CharacterInfo? Quais campos ele lê?

2) Qual struct/typedef define o layout (tamanho, offsets)?

3) Existe bifurcação por versão para esse pacote no client? (provável não)
