# NEW-ERA 1.3-V — CLIENT: RX Inventário (F3:10) — EVIDENCE

- Legacy commit pin: 580472e0d5723f9709cbad594f233deb07f9f351


## Files fetched (sha256)

- `WSclient.cpp` sha256 `6538d944126181dfa77d4419bf992ac77151eec5411dc3ab243f2926fec868bf` bytes=415884

- `WSclient.h`   sha256 `19d38a3b6506a745662c14dd7cdd6e41ca3ad29b28b17c76f860cd9e67799c10` bytes=91012


---

## Dispatch (TranslateProtocol)

### case 0xF3 @ line 12942
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

### subcase ~0x10 / ReceiveInventory @ line 1343
```
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
  1486: 
  1487: 	g_ConsoleDebug->Write(MCD_RECEIVE, "0x31 [ReceiveTradeInventory]");
  1488: }
  1489: 
  1490: void ReceiveChat( BYTE *ReceiveBuffer )
  1491: {
  1492: 	if(SceneFlag == LOG_IN_SCENE)
  1493: 	{
  1494: 		g_ErrorReport.Write ( "Send Request Server List.\r\n");
  1495: 		SendRequestServerList();
  1496: 	}
  1497: 	else
  1498: 	{
  1499: 		LPPCHATING Data = (LPPCHATING)ReceiveBuffer;
  1500: 
  1501: 		char ID[MAX_ID_SIZE+1];
  1502: 		memset(ID, 0, MAX_ID_SIZE+1);
  1503: 		memcpy(ID,(char *)Data->ID,MAX_ID_SIZE);
  1504: 
  1505: 		char Text[MAX_CHAT_SIZE+1];
  1506: 		memset(Text, 0, MAX_CHAT_SIZE+1);
  1507: 
  1508: 		if(Data->ChatText[0]=='~')
  1509: 		{
  1510: 			for(int i=0;i<MAX_CHAT_SIZE-1;i++)
  1511: 				Text[i] = Data->ChatText[i+1];
  1512: 			g_pChatListBox->AddText(ID, Text, SEASON3B::TYPE_PARTY_MESSAGE);
  1513: 		}
  1514: 		else if(Data->ChatText[0]=='@' && Data->ChatText[1]=='@')
  1515: 		{
  1516: 			for(int i=0;i<MAX_CHAT_SIZE-2;i++)
  1517: 				Text[i] = Data->ChatText[i+2];
  1518: 			g_pChatListBox->AddText(ID, Text, SEASON3B::TYPE_UNION_MESSAGE);
  1519: 		}
  1520: 		else if(Data->ChatText[0]=='@')
  1521: 		{
  1522: 			for(int i=0;i<MAX_CHAT_SIZE-1;i++)
  1523: 				Text[i] = Data->ChatText[i+1];
  1524: 			g_pChatListBox->AddText(ID, Text, SEASON3B::TYPE_GUILD_MESSAGE);
  1525: 		}
  1526: 		else if(Data->ChatText[0]=='#')
  1527: 		{
  1528: 			for(int i=0;i<MAX_CHAT_SIZE-1;i++)
  1529: 				Text[i] = Data->ChatText[i+1];
  1530: 
  1531: 			CHARACTER* pFindGm = NULL;
  1532: 
  1533: 			for(int i=0;i<MAX_CHARACTERS_CLIENT;i++)
  1534: 			{
  1535: 				CHARACTER *c = &CharactersClient[i];
  1536: 				OBJECT *o = &c->Object;
  1537: 				if( o->Live && o->Kind==KIND_PLAYER && ( g_isCharacterBuff( (&c->Object), eBuff_GMEffect) || (c->CtlCode == CTLCODE_20OPERATOR) || (c->CtlCode == CTLCODE_08OPERATOR) ) )
  1538: 				{
  1539: 					if(strcmp(c->ID,ID) == NULL)
  1540: 					{
  1541: 						pFindGm = c;
  1542: 						break;
  1543: 					}
  1544: 				}
  1545: 			}
  1546: 			if(pFindGm)
  1547: 			{
  1548: 				AssignChat(ID,Text);
  1549: 				g_pChatListBox->AddText(ID,Text, SEASON3B::TYPE_GM_MESSAGE);
  1550: 			}
  1551: 			else
  1552: 			{
  1553: 				AssignChat(ID,Text,1);
  1554: 			}
  1555: 		}
  1556:         else
  1557: 		{
  1558: 			memcpy(Text,(char *)Data->ChatText,MAX_CHAT_SIZE);
  1559: 			CHARACTER* pFindGm = NULL;
  1560: 			for(int i=0;i<MAX_CHARACTERS_CLIENT;i++)
  1561: 			{
  1562: 				CHARACTER *c = &CharactersClient[i];
  1563: 				OBJECT *o = &c->Object;
  1564: 				if( o->Live && o->Kind==KIND_PLAYER && g_isCharacterBuff((&c->Object), eBuff_GMEffect) || (c->CtlCode == CTLCODE_20OPERATOR) || (c->CtlCode == CTLCODE_08OPERATOR) )
  1565: 				{
  1566: 					if(strcmp(c->ID,ID) == NULL)
  1567: 					{
  1568: 						pFindGm = c;
  1569: 						break;
  1570: 					}
  1571: 				}
  1572: 			}
  1573: 			if(pFindGm)
  1574: 			{
  1575: 				AssignChat(ID,Text);
  1576: 				g_pChatListBox->AddText(ID, Text, SEASON3B::TYPE_GM_MESSAGE);
  1577: 			}
  1578: 			else
  1579: 			{
  1580: 				AssignChat(ID,Text);
  1581: 				g_pChatListBox->AddText(ID, Text, SEASON3B::TYPE_CHAT_MESSAGE);
  1582: 			}
  1583: 		}
  1584: 	}
  1585: }
  1586: 
  1587: void ReceiveChatWhisper( BYTE *ReceiveBuffer )
  1588: {
  1589: 	if(g_pChatInputBox->IsBlockWhisper() == true)
  1590: 	{
  1591: 		return;
  1592: 	}
  1593: 
  1594: 	LPPCHATING Data = (LPPCHATING)ReceiveBuffer;
  1595: 
  1596: 	char ID[MAX_ID_SIZE+1];
  1597: 	memset(ID, 0, MAX_ID_SIZE+1);
  1598: 	memcpy(ID,(char *)Data->ID,MAX_ID_SIZE);
  1599: 
  1600: 	char Text[MAX_CHAT_SIZE+1];
  1601: 	memset(Text, 0, MAX_CHAT_SIZE+1);
  1602: 	memcpy(Text,(char *)Data->ChatText,MAX_CHAT_SIZE);
  1603: 
```


---

## Function body: ReceiveInventory

- approx signature line: 1343
- approx end line: 1399
```
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
```


---

## Header scan: WSclient.h tokens (first hits + context)

### TOKEN RX: `\bINVENTORY\b`

- no hits


### TOKEN RX: `\bReceiveInventory\b`

- no hits


### TOKEN RX: `\bPMSG_.*INVENTORY.*\b`

- first hit @ line 3087: `} PMSG_ANS_INVENTORY_EQUIPMENT_ITEM, *LPPMSG_ANS_INVENTORY_EQUIPMENT_ITEM;`
```
  3027: } PMSG_DOPPELGANGER_ICEWORKER_STATE, *LPPMSG_DOPPELGANGER_ICEWORKER_STATE;
  3028: //----------------------------------------------------------------------------
  3029: // GC [0xBF][0x12]
  3030: //----------------------------------------------------------------------------
  3031: typedef struct
  3032: {
  3033: 	PBMSG_HEADER2	h;
  3034: 	WORD		wRemainSec;
  3035: 	BYTE		btUserCount;
  3036: 	BYTE		btDummy;
  3037: 	BYTE		UserPosData;
  3038: } PMSG_DOPPELGANGER_PLAY_INFO, *LPPMSG_DOPPELGANGER_PLAY_INFO;
  3039: 
  3040: typedef struct
  3041: {
  3042: 	WORD		wUserIndex;
  3043: 	BYTE		byMapNumber;
  3044: 	BYTE		btPosIndex;
  3045: } PMSG_DOPPELGANGER_USER_POS, *LPPMSG_DOPPELGANGER_USER_POS;
  3046: //----------------------------------------------------------------------------
  3047: // GC [0xBF][0x13]
  3048: //----------------------------------------------------------------------------
  3049: typedef struct
  3050: {
  3051: 	PBMSG_HEADER2	h;
  3052: 	BYTE		btResult;
  3053: 	DWORD		dwRewardExp;
  3054: }PMSG_DOPPELGANGER_RESULT, *LPPMSG_DOPPELGANGER_RESULT;
  3055: //----------------------------------------------------------------------------
  3056: // GC [0xBF][0x14]
  3057: //----------------------------------------------------------------------------
  3058: typedef struct
  3059: {
  3060: 	PBMSG_HEADER2 h;
  3061: 	BYTE		btMaxGoalCnt;
  3062: 	BYTE		btGoalCnt;
  3063: }PMSG_DOPPELGANGER_MONSTER_GOAL, * LPPMSG_DOPPELGANGER_MONSTER_GOAL;
  3064: 
  3065: #ifdef PBG_ADD_SECRETBUFF
  3066: //----------------------------------------------------------------------------
  3067: // GC [0xBF][0x15]
  3068: //----------------------------------------------------------------------------
  3069: typedef struct
  3070: {
  3071: 	PBMSG_HEADER2 h;
  3072: 	BYTE		btFatiguePercentage;
  3073: }PMSG_FATIGUEPERCENTAGE, * LPPMSG_FATIGUEPERCENTAGE;
  3074: #endif //PBG_ADD_SECRETBUFF
  3075: 
  3076: 
  3077: #ifdef LJH_ADD_SYSTEM_OF_EQUIPPING_ITEM_FROM_INVENTORY
  3078: //----------------------------------------------------------------------------
  3079: // GC [0xBF][0x20]
  3080: //----------------------------------------------------------------------------
  3081: typedef struct _tagPMSG_ANS_INVENTORY_EQUIPMENT_ITEM
  3082: {
  3083: 	PBMSG_HEADER2	h;
  3084: 
  3085: 	BYTE	btItemPos;
  3086: 	BYTE	btResult;
  3087: } PMSG_ANS_INVENTORY_EQUIPMENT_ITEM, *LPPMSG_ANS_INVENTORY_EQUIPMENT_ITEM;
  3088: #endif //LJH_ADD_SYSTEM_OF_EQUIPPING_ITEM_FROM_INVENTORY
  3089: 
  3090: //----------------------------------------------------------------------------
  3091: // GC[0x8E][0x01]
  3092: //----------------------------------------------------------------------------
  3093: typedef struct
  3094: {
  3095: 	PBMSG_HEADER2	h;
  3096: 	DWORD		dwKeyValue;
  3097: }PMSG_MAPMOVE_CHECKSUM, *LPPMSG_MAPMOVE_CHECKSUM;
  3098: 
  3099: //----------------------------------------------------------------------------
  3100: // GC[0x8E][0x03]
  3101: //----------------------------------------------------------------------------
  3102: typedef struct
  3103: {
  3104: 	PBMSG_HEADER2	h;
  3105: 	BYTE		btResult;
  3106: }PMSG_ANS_MAPMOVE, *LPPMSG_ANS_MAPMOVE;
  3107: 
  3108: //----------------------------------------------------------------------------
  3109: // GC [0xF7][0x02]
  3110: //----------------------------------------------------------------------------
  3111: typedef struct
  3112: {
  3113: 	PBMSG_HEADER2	h;
  3114: 	BYTE		Result;
  3115: 	BYTE		Day;
  3116: 	BYTE		Zone;
  3117: 	BYTE		Wheather;
  3118: 	DWORD		RemainTick;
  3119: } PMSG_RESULT_ENTER_EMPIREGUARDIAN, *LPPMSG_RESULT_ENTER_EMPIREGUARDIAN;
  3120: 
  3121: //----------------------------------------------------------------------------
  3122: // GC [0xF7][0x04]
  3123: //----------------------------------------------------------------------------
  3124: typedef struct
  3125: {
  3126: 	PBMSG_HEADER2	h;
  3127: 	BYTE		Type;
  3128: 	DWORD		RemainTick;
  3129: 	BYTE		MonsterCount;
  3130: } PMSG_REMAINTICK_EMPIREGUARDIAN, *LPPMSG_REMAINTICK_EMPIREGUARDIAN;
  3131: 
  3132: //----------------------------------------------------------------------------
  3133: // GC [0xF7][0x06]
  3134: //----------------------------------------------------------------------------
  3135: typedef struct
  3136: {
  3137: 	PBMSG_HEADER2	h;
  3138: 	BYTE		Result;
  3139: 	DWORD		Exp;
  3140: } PMSG_CLEAR_RESULT_EMPIREGUARDIAN, *LPPMSG_CLEAR_RESULT_EMPIREGUARDIAN;
  3141: 
  3142: #ifdef KJH_ADD_INGAMESHOP_UI_SYSTEM
  3143: 
  3144: #pragma pack(push, 1)
  3145: 
  3146: //----------------------------------------------------------------------------
  3147: // (0xD2)(0x01)
  3148: //----------------------------------------------------------------------------
  3149: typedef struct
  3150: {
  3151: 	PBMSG_HEADER2		h;
  3152: }PMSG_CASHSHOP_CASHPOINT_REQ, *LPPMSG_CASHSHOP_CASHPOINT_REQ;
  3153: 
  3154: //----------------------------------------------------------------------------
  3155: // (0xD2)(0x01)
  3156: //----------------------------------------------------------------------------
  3157: typedef struct
  3158: {
  3159: 	PBMSG_HEADER2		h;
  3160: 
  3161: 	BYTE				btViewType;
  3162: 
  3163: 	double				dTotalCash;
  3164: 	double				dCashCredit;		// C (CreditCard)
  3165: 	double				dCashPrepaid;		// P (PrepaidCard)
  3166: 	double				dTotalPoint;
  3167: 	double				dTotalMileage;
  3168: }PMSG_CASHSHOP_CASHPOINT_ANS, *LPPMSG_CASHSHOP_CASHPOINT_ANS;
  3169: 
  3170: //----------------------------------------------------------------------------
  3171: // (0xD2)(0x02)
  3172: //----------------------------------------------------------------------------
  3173: typedef struct
  3174: {
  3175: 	PBMSG_HEADER2		h;
  3176: 
  3177: 	BYTE				byShopOpenType;
  3178: }PMSG_CASHSHOP_SHOPOPEN_REQ, *LPPMSG_CASHSHOP_SHOPOPEN_REQ;
  3179: 
  3180: //----------------------------------------------------------------------------
  3181: // (0xD2)(0x02)
  3182: //----------------------------------------------------------------------------
  3183: typedef struct
  3184: {
  3185: 	PBMSG_HEADER2		h;
  3186: 
  3187: 	BYTE				byShopOpenResult;
  3188: }PMSG_CASHSHOP_SHOPOPEN_ANS, *LPPMSG_CASHSHOP_SHOPOPEN_ANS;
  3189: 
  3190: //----------------------------------------------------------------------------
  3191: // (0xD2)(0x03)
  3192: //----------------------------------------------------------------------------
  3193: typedef struct
  3194: {
  3195: 	PBMSG_HEADER2		h;
  3196: 
  3197: 	long				lBuyItemPackageSeq;
  3198: 	long				lBuyItemDisplaySeq;
  3199: 	long				lBuyItemPriceSeq;
  3200: 	WORD				wItemCode;
  3201: }PMSG_CASHSHOP_BUYITEM_REQ, *LPPMSG_CASHSHOP_BUYITEM_REQ;
  3202: 
  3203: //----------------------------------------------------------------------------
  3204: //    (0xD2)(0x03)
  3205: //----------------------------------------------------------------------------
  3206: typedef struct
  3207: {
  3208: 	PBMSG_HEADER2		h;
  3209: 
  3210: 	BYTE				byResultCode;
  3211: 	long				lItemLeftCount;
  3212: }PMSG_CASHSHOP_BUYITEM_ANS, *LPPMSG_CASHSHOP_BUYITEM_ANS;
  3213: 
  3214: //----------------------------------------------------------------------------
  3215: // (0xD2)(0x04)
  3216: //----------------------------------------------------------------------------
  3217: typedef struct
  3218: {
  3219: 	PBMSG_HEADER2		h;
  3220: 
  3221: 	long				lGiftItemPackageSeq;
  3222: 	long				lDiftItemDisplaySeq;
  3223: 	long				lGiftItemPriceSeq;
  3224: 	long				lSaleZone;
  3225: 	char				chReceiveUserID[MAX_ID_SIZE+1];
  3226: 	char				chMessage[MAX_GIFT_MESSAGE_SIZE];
  3227: }PMSG_CASHSHOP_GIFTSEND_REQ, *LPPMSG_CASHSHOP_GIFTSEND_REQ;
  3228: 
  3229: //----------------------------------------------------------------------------
  3230: // (0xD2)(0x04)
  3231: //----------------------------------------------------------------------------
  3232: typedef struct
  3233: {
  3234: 	PBMSG_HEADER2		h;
  3235: 
  3236: 	BYTE				byResultCode;
  3237: 	long				lItemLeftCount;
  3238: 	double				dLimitedCash;
  3239: }PMSG_CASHSHOP_GIFTSEND_ANS, *LPPMSG_CASHSHOP_GIFTSEND_ANS;
  3240: 
  3241: //----------------------------------------------------------------------------
  3242: // (0xD2)(0x05)
  3243: //----------------------------------------------------------------------------
  3244: typedef struct
  3245: {
  3246: 	PBMSG_HEADER2		h;
  3247: 	int					iPageIndex;
  3248: 	char				chStorageType;
  3249: }PMSG_CASHSHOP_STORAGELIST_REQ, *LPPMSG_CASHSHOP_STORAGELIST_REQ;
  3250: 
  3251: //----------------------------------------------------------------------------
  3252: // (0xD2)(0x06)
  3253: //----------------------------------------------------------------------------
  3254: typedef struct
  3255: {
  3256: 	PBMSG_HEADER2		h;
  3257: 	WORD				wTotalItemCount;
  3258: 	WORD				wCurrentItemCount;
  3259: 	WORD				wPageIndex;
  3260: 	WORD				wTotalPage;
  3261: }PMSG_CASHSHOP_STORAGECOUNT, *LPPMSG_CASHSHOP_STORAGECOUNT;
  3262: 
  3263: //----------------------------------------------------------------------------
  3264: // (0xD2)(0x0D)
  3265: //----------------------------------------------------------------------------
  3266: typedef struct
  3267: {
  3268: 	PBMSG_HEADER2		h;
  3269: 
  3270: 	long				lStorageIndex;
  3271: 	long				lItemSeq;
  3272: 	long				lStorageGroupCode;
  3273: 	long				lProductSeq;
  3274: 	long				lPriceSeq;
  3275: 	double				dCashPoint;
  3276: 	char				chItemType;
  3277: }PMSG_CASHSHOP_STORAGELIST, *LPPMSG_CASHSHOP_STORAGELIST;
  3278: 
  3279: //----------------------------------------------------------------------------
  3280: // (0xD2)(0x0E)
  3281: //----------------------------------------------------------------------------
  3282: typedef struct
  3283: {
  3284: 	PBMSG_HEADER2		h;
  3285: 
  3286: 	long				lStorageIndex;
  3287: 	long				lItemSeq;
  3288: 	long				lStorageGroupCode;
  3289: 	long				lProductSeq;
  3290: 	long				lPriceSeq;
  3291: 	double				dCashPoint;
  3292: 	char				chItemType;
  3293: 
  3294: 	char				chSendUserName[MAX_ID_SIZE+1];
  3295: 	char				chMessage[MAX_GIFT_MESSAGE_SIZE];
  3296: }PMSG_CASHSHOP_GIFTSTORAGELIST, *LPPMSG_CASHSHOP_GIFTSTORAGELIST;
  3297: 
  3298: //----------------------------------------------------------------------------
  3299: // (0xD2)(0x07)
  3300: //----------------------------------------------------------------------------
  3301: typedef struct
  3302: {
  3303: 	PBMSG_HEADER2		h;
  3304: 
  3305: 	double				dCashValue;
  3306: 
  3307: 	char				chReceiveUserID[MAX_ID_SIZE+1];
```

### TOKEN RX: `\bPMSG_.*ITEM.*\b`

- first hit @ line 1866: `} PMSG_ITEMEFFECTCANCEL, *LPPMSG_ITEMEFFECTCANCEL;`
```
  1806: } FS_LETTER_RESULT, * LPFS_LETTER_RESULT;
  1807: 
  1808: typedef struct {
  1809: 	PBMSG_HEADER    Header;
  1810: 	BYTE			IP[15];
  1811: 	WORD			RoomNumber;
  1812: 	DWORD			Ticket;
  1813: 	BYTE			Type;
  1814: 	BYTE			ID[10];
  1815: 	BYTE			Result;
  1816: } FS_CHAT_CREATE_RESULT, * LPFS_CHAT_CREATE_RESULT;
  1817: 
  1818: typedef struct {
  1819: 	PBMSG_HEADER    Header;
  1820: 	BYTE			Result;
  1821: } FS_CHAT_JOIN_RESULT, * LPFS_CHAT_JOIN_RESULT;
  1822: 
  1823: typedef struct {
  1824: 	PBMSG_HEADER    Header;
  1825: 	BYTE			Type;
  1826: 	BYTE			Index;
  1827: 	BYTE			Name[MAX_ID_SIZE];
  1828: } FS_CHAT_CHANGE_STATE, * LPFS_CHAT_CHANGE_STATE;
  1829: 
  1830: typedef struct {
  1831: 	PWMSG_HEADER    Header;
  1832: 	WORD			RoomNumber;
  1833: 	BYTE			Count;
  1834: } FS_CHAT_USERLIST_HEADER, * LPFS_CHAT_USERLIST_HEADER;
  1835: 
  1836: typedef struct {
  1837: 	BYTE			Index;
  1838: 	BYTE			Name[MAX_ID_SIZE];
  1839: } FS_CHAT_USERLIST_DATA, * LPFS_CHAT_USERLIST_DATA;
  1840: 
  1841: typedef struct {
  1842: 	PBMSG_HEADER    Header;
  1843: 	BYTE			Result;
  1844: 	DWORD			WindowGuid;
  1845: } FS_CHAT_INVITE_RESULT, * LPFS_CHAT_INVITE_RESULT;
  1846: 
  1847: typedef struct {
  1848: 	PBMSG_HEADER    Header;
  1849: 	BYTE			Index;
  1850: 	BYTE			MsgSize;
  1851: 	BYTE			Msg[100];
  1852: } FS_CHAT_TEXT, * LPFS_CHAT_TEXT;
  1853: 
  1854: //----------------------------------------------------------------------------
  1855: // GC [0x2D]
  1856: //----------------------------------------------------------------------------
  1857: typedef struct
  1858: {
  1859: 	PBMSG_HEADER	h;
  1860: 
  1861: 	WORD		wOptionType;
  1862: 	WORD		wEffectType;
  1863: 	BYTE		byEffectOption;
  1864: 	int			wEffectTime;
  1865: 	BYTE		byBuffType;
  1866: } PMSG_ITEMEFFECTCANCEL, *LPPMSG_ITEMEFFECTCANCEL;
  1867: 
  1868: typedef struct
  1869: {
  1870:     PBMSG_HEADER    m_Header;
  1871:     BYTE            m_byPetType;
  1872:     BYTE            m_byCommand;
  1873:     BYTE            m_byKeyH;
  1874:     BYTE            m_byKeyL;
  1875: }PRECEIVE_PET_COMMAND, *LPPRECEIVE_PET_COMMAND;
  1876: 
  1877: 
  1878: typedef struct
  1879: {
  1880:     PBMSG_HEADER    m_Header;
  1881:     BYTE            m_byPetType;
  1882:     BYTE            m_bySkillType;
  1883:     BYTE            m_byKeyH;
  1884:     BYTE            m_byKeyL;
  1885:     BYTE            m_byTKeyH;
  1886:     BYTE            m_byTKeyL;
  1887: }PRECEIVE_PET_ATTACK, *LPPRECEIVE_PET_ATTACK;
  1888: 
  1889: 
  1890: typedef struct
  1891: {
  1892:     PBMSG_HEADER    m_Header;
  1893:     BYTE            m_byPetType;
  1894:     BYTE            m_byInvType;
  1895:     BYTE            m_byPos;
  1896:     BYTE            m_byLevel;
  1897:     int             m_iExp;
  1898: 	BYTE			m_byLife;
  1899: }PRECEIVE_PET_INFO, *LPPRECEIVE_PET_INFO;
  1900: 
  1901: typedef struct
  1902: {
  1903: 	PBMSG_HEADER    m_Header;
  1904: 	BYTE			m_subCode;
  1905: 	BYTE			m_Type;
  1906: 	WORD			m_Time;
  1907: }PMSG_MATCH_TIMEVIEW, *LPPMSG_MATCH_TIMEVIEW;
  1908: 
  1909: typedef struct
  1910: {
  1911: 	void			Clear()
  1912: 	{
  1913: 		memset(&m_MatchTeamName1, NULL, MAX_ID_SIZE);
  1914: 		memset(&m_MatchTeamName2, NULL, MAX_ID_SIZE);
  1915: 		m_Score1 = 0;
  1916: 		m_Score2 = 0;
  1917: 		m_Type = 0;
  1918: 	}
  1919: 
  1920: 	PBMSG_HEADER	m_Header;
  1921: 	BYTE			m_subCode;
  1922: 	BYTE			m_Type;
  1923: 
  1924: 	char			m_MatchTeamName1[MAX_ID_SIZE];
  1925: 	WORD			m_Score1;
  1926: 
  1927: 	char			m_MatchTeamName2[MAX_ID_SIZE];
  1928: 	WORD			m_Score2;
  1929: 
  1930: }PMSG_MATCH_RESULT, *LPPMSG_MATCH_RESULT;
  1931: 
  1932: typedef struct
  1933: {
  1934: 	PBMSG_HEADER	m_Header;
  1935: 	BYTE			m_subCode;
  1936: 	BYTE			m_x;
  1937: 	BYTE			m_y;
  1938: }PMSG_SOCCER_GOALIN, *LPPMSG_SOCCER_GOALIN;
  1939: 
  1940: extern PMSG_MATCH_RESULT		g_wtMatchResult;
  1941: extern PMSG_MATCH_TIMEVIEW		g_wtMatchTimeLeft;
  1942: extern int g_iGoalEffect;
  1943: 
  1944: typedef struct
  1945: {
  1946:     PBMSG_HEADER    m_Header;
  1947:     MServerInfo     m_vSvrInfo;
  1948: }PHEADER_MAP_CHANGESERVER_INFO, *LPPHEADER_MAP_CHANGESERVER_INFO;
  1949: 
  1950: 
  1951: enum CASTLESIEGE_STATE
  1952: {
  1953: 	CASTLESIEGE_STATE_NONE					= -1,
  1954: 	CASTLESIEGE_STATE_IDLE_1				= 0,
  1955: 	CASTLESIEGE_STATE_REGSIEGE				= 1,
  1956: 	CASTLESIEGE_STATE_IDLE_2				= 2,
  1957: 	CASTLESIEGE_STATE_REGMARK				= 3,
  1958: 	CASTLESIEGE_STATE_IDLE_3				= 4,
  1959: 	CASTLESIEGE_STATE_NOTIFY				= 5,
  1960: 	CASTLESIEGE_STATE_READYSIEGE			= 6,
  1961: 	CASTLESIEGE_STATE_STARTSIEGE			= 7,
  1962: 	CASTLESIEGE_STATE_ENDSIEGE				= 8,
  1963: 	CASTLESIEGE_STATE_ENDCYCLE				= 9,
  1964: };
  1965: 
  1966: //----------------------------------------------------------------------------
  1967: // GC [0xB2][0x00]
  1968: //----------------------------------------------------------------------------
  1969: typedef struct
  1970: {
  1971: 	PREQUEST_DEFAULT_SUBCODE	Header;
  1972: 	BYTE		btResult;
  1973: 	CHAR		cCastleSiegeState;
  1974: 	BYTE		btStartYearH;
  1975: 	BYTE		btStartYearL;
  1976: 	BYTE		btStartMonth;
  1977: 	BYTE		btStartDay;
  1978: 	BYTE		btStartHour;
  1979: 	BYTE		btStartMinute;
  1980: 	BYTE		btEndYearH;
  1981: 	BYTE		btEndYearL;
  1982: 	BYTE		btEndMonth;
  1983: 	BYTE		btEndDay;
  1984: 	BYTE		btEndHour;
  1985: 	BYTE		btEndMinute;
  1986: 	BYTE		btSiegeStartYearH;
  1987: 	BYTE		btSiegeStartYearL;
  1988: 	BYTE		btSiegeStartMonth;
  1989: 	BYTE		btSiegeStartDay;
  1990: 	BYTE		btSiegeStartHour;
  1991: 	BYTE		btSiegeStartMinute;
  1992: 	CHAR		cOwnerGuild[8];
  1993: 	CHAR		cOwnerGuildMaster[10];
  1994: 
  1995: 	CHAR		btStateLeftSec1;
  1996: 	CHAR		btStateLeftSec2;
  1997: 	CHAR		btStateLeftSec3;
  1998: 	CHAR		btStateLeftSec4;
  1999: } PMSG_ANS_CASTLESIEGESTATE, *LPPMSG_ANS_CASTLESIEGESTATE;
  2000: 
  2001: //----------------------------------------------------------------------------
  2002: // GC [0xB2][0x01]
  2003: //----------------------------------------------------------------------------
  2004: typedef struct
  2005: {
  2006: 	PREQUEST_DEFAULT_SUBCODE	Header;
  2007: 	BYTE		btResult;
  2008: 	CHAR		szGuildName[8];
  2009: } PMSG_ANS_REGCASTLESIEGE, *LPPMSG_ANS_REGCASTLESIEGE;
  2010: 
  2011: //----------------------------------------------------------------------------
  2012: // GC [0xB2][0x02]
  2013: //----------------------------------------------------------------------------
  2014: typedef struct
  2015: {
  2016: 	PREQUEST_DEFAULT_SUBCODE	Header;
  2017: 	BYTE		btResult;
  2018: 	BYTE		btIsGiveUp;
  2019: 	CHAR		szGuildName[8];
  2020: } PMSG_ANS_GIVEUPCASTLESIEGE, *LPPMSG_ANS_GIVEUPCASTLESIEGE;
  2021: 
  2022: //----------------------------------------------------------------------------
  2023: // GC [0xB2][0x03]
  2024: //----------------------------------------------------------------------------
  2025: typedef struct
  2026: {
  2027: 	PREQUEST_DEFAULT_SUBCODE	Header;
  2028: 	BYTE		btResult;
  2029: 	CHAR		szGuildName[8];
  2030: 	BYTE		btGuildMark1;
  2031: 	BYTE		btGuildMark2;
  2032: 	BYTE		btGuildMark3;
  2033: 	BYTE		btGuildMark4;
  2034: 	BYTE		btIsGiveUp;
  2035: 	BYTE		btRegRank;
  2036: } PMSG_ANS_GUILDREGINFO, *LPPMSG_ANS_GUILDREGINFO;
  2037: 
  2038: //----------------------------------------------------------------------------
  2039: // GC [0xB2][0x04]
  2040: //----------------------------------------------------------------------------
  2041: typedef struct
  2042: {
  2043: 	PREQUEST_DEFAULT_SUBCODE	Header;
  2044: 	BYTE		btResult;
  2045: 	CHAR		szGuildName[8];
  2046: 	BYTE		btGuildMark1;
  2047: 	BYTE		btGuildMark2;
  2048: 	BYTE		btGuildMark3;
  2049: 	BYTE		btGuildMark4;
  2050: } PMSG_ANS_REGGUILDMARK, *LPPMSG_ANS_REGGUILDMARK;
  2051: 
  2052: //----------------------------------------------------------------------------
  2053: // GC [0xB2][0x05]
  2054: //----------------------------------------------------------------------------
  2055: typedef struct
  2056: {
  2057: 	PREQUEST_DEFAULT_SUBCODE	Header;
  2058: 	BYTE		btResult;
  2059: 	INT			iNpcNumber;
  2060: 	INT			iNpcIndex;
  2061: } PMSG_ANS_NPCBUY, *LPPMSG_ANS_NPCBUY;
  2062: 
  2063: //----------------------------------------------------------------------------
  2064: // GC [0xB2][0x06]
  2065: //----------------------------------------------------------------------------
  2066: typedef struct
  2067: {
  2068: 	PREQUEST_DEFAULT_SUBCODE	Header;
  2069: 	BYTE		btResult;
  2070: 	INT			iNpcNumber;
  2071: 	INT			iNpcIndex;
  2072: 	INT			iNpcHP;
  2073: 	INT			iNpcMaxHP;
  2074: } PMSG_ANS_NPCREPAIR, *LPPMSG_ANS_NPCREPAIR;
  2075: 
  2076: //----------------------------------------------------------------------------
  2077: // GC [0xB2][0x07]
  2078: //----------------------------------------------------------------------------
  2079: typedef struct
  2080: {
  2081: 	PREQUEST_DEFAULT_SUBCODE	Header;
  2082: 	BYTE		btResult;
  2083: 	INT			iNpcNumber;
  2084: 	INT			iNpcIndex;
  2085: 	INT			iNpcUpType;
  2086: 	INT			iNpcUpValue;
```

### TOKEN RX: `\bITEM_`

- no hits


### TOKEN RX: `\bMAX_INVENTORY\b`

- no hits



---

## QUESTIONS (responder como CONFIRMED/PROBABLE com âncora)

1) No client, qual o caminho exato do dispatch para `F3:10` (inventário)?

2) `ReceiveInventory` valida tamanho? Qual é o layout (tamanho fixo/stride/loops)?

3) Quais structs/defines do header descrevem esse pacote (nome, campos, tamanhos)?

4) Próximo microteste após este: achar o emissor no GS do `F3:10` (Inventory list) e fechar end-to-end.
