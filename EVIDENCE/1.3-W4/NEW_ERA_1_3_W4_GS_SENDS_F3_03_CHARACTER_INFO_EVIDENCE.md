# NEW-ERA 1.3-W4 — GS envia F3:03 (CharacterInfo) em DGCharacterInfoRecv — EVIDENCE

- Legacy commit pin: `580472e0d5723f9709cbad594f233deb07f9f351`


## Files fetched (sha256)

- `DSProtocol.cpp` sha256 `f1b8aed24bb6c020a14f86a779da300f0faf2bc687e394cb3e15b171162793fc` bytes=108622

- `Protocol.h`     sha256 `943c59b7a457eccea4953b7410fd2140c6299979782b58ca81a7b42a52d6c3b0` bytes=36305


---

## DSProtocol.cpp — DGCharacterInfoRecv (definição real)

- function region: lines 1054..1376

```
  1014: 
  1015: 	PMSG_CHARACTER_CREATE_SEND pMsg;
  1016: 
  1017: 	pMsg.header.set(0xF3,0x01,sizeof(pMsg));
  1018: 
  1019: 	pMsg.result = lpMsg->result;
  1020: 
  1021: 	memcpy(pMsg.name,lpMsg->name,sizeof(pMsg.name));
  1022: 
  1023: 	pMsg.slot = lpMsg->slot;
  1024: 
  1025: 	pMsg.level = lpMsg->level;
  1026: 
  1027: 	pMsg.Class = (lpMsg->Class%16)*16;
  1028: 	pMsg.Class -= (pMsg.Class/32);
  1029: 	pMsg.Class += (lpMsg->Class/16)*32;
  1030: 
  1031: 	memcpy(pMsg.equipment,lpMsg->equipment,sizeof(pMsg.equipment));
  1032: 
  1033: 	DataSend(lpMsg->index,(BYTE*)&pMsg,pMsg.header.size);
  1034: }
  1035: 
  1036: void DGCharacterDeleteRecv(SDHP_CHARACTER_DELETE_RECV* lpMsg) // OK
  1037: {
  1038: 	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
  1039: 	{
  1040: 		LogAdd(LOG_RED,"[DGCharacterDeleteRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
  1041: 		CloseClient(lpMsg->index);
  1042: 		return;
  1043: 	}
  1044: 
  1045: 	PMSG_CHARACTER_DELETE_SEND pMsg;
  1046: 
  1047: 	pMsg.header.set(0xF3,0x02,sizeof(pMsg));
  1048: 
  1049: 	pMsg.result = lpMsg->result;
  1050: 
  1051: 	DataSend(lpMsg->index,(BYTE*)&pMsg,pMsg.header.size);
  1052: }
  1053: 
  1054: void DGCharacterInfoRecv(SDHP_CHARACTER_INFO_RECV* lpMsg) // OK
  1055: {
  1056: 	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
  1057: 	{
  1058: 		LogAdd(LOG_RED,"[DGCharacterInfoRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
  1059: 		CloseClient(lpMsg->index);
  1060: 		return;
  1061: 	}
  1062: 
  1063: 	if(lpMsg->result == 0)
  1064: 	{
  1065: 		CloseClient(lpMsg->index);
  1066: 		return;
  1067: 	}
  1068: 
  1069: 	if((lpMsg->CtlCode & 1) != 0)
  1070: 	{
  1071: 		CloseClient(lpMsg->index);
  1072: 		return;
  1073: 	}
  1074: 
  1075: 	if(gObjectManager.CharacterInfoSet((BYTE*)lpMsg,lpMsg->index) == 0)
  1076: 	{
  1077: 		CloseClient(lpMsg->index);
  1078: 		return;
  1079: 	}
  1080: 
  1081: 	LPOBJ lpObj = &gObj[lpMsg->index];
  1082: 
  1083: 	if(lpObj->MapServerMoveRequest == 0)
  1084: 	{
  1085: 		if((lpObj->NextServerCode=gMapServerManager.CheckMapServerMove(lpObj->Index,lpObj->Map,lpObj->LastServerCode)) != gServerInfo.m_ServerCode)
  1086: 		{
  1087: 			if(lpObj->NextServerCode == -1)
  1088: 			{
  1089: 				CloseClient(lpObj->Index);
  1090: 				return;
  1091: 			}
  1092: 			else
  1093: 			{
  1094: 				GJMapServerMoveSend(lpObj->Index,lpObj->NextServerCode,lpObj->Map,(BYTE)lpObj->X,(BYTE)lpObj->Y);
  1095: 				return;
  1096: 			}
  1097: 		}
  1098: 	}
  1099: 
  1100: 	lpObj->MapMoveDisable = 1;
  1101: 
  1102: 	PMSG_CHARACTER_INFO_SEND pMsg;
  1103: 
  1104: 	pMsg.header.set(0xF3,0x03,sizeof(pMsg));
  1105: 
  1106: 	pMsg.X = (BYTE)lpObj->X;
  1107: 	pMsg.Y = (BYTE)lpObj->Y;
  1108: 	pMsg.Map = lpObj->Map;
  1109: 	pMsg.Dir = lpObj->Dir;
  1110: 
  1111: 	if(gMasterSkillTree.CheckMasterLevel(lpObj) == 0)
  1112: 	{
  1113: 		pMsg.Experience[0] = SET_NUMBERHB(SET_NUMBERHW(SET_NUMBERHDW(lpObj->Experience)));
  1114: 		pMsg.Experience[1] = SET_NUMBERLB(SET_NUMBERHW(SET_NUMBERHDW(lpObj->Experience)));
  1115: 		pMsg.Experience[2] = SET_NUMBERHB(SET_NUMBERLW(SET_NUMBERHDW(lpObj->Experience)));
  1116: 		pMsg.Experience[3] = SET_NUMBERLB(SET_NUMBERLW(SET_NUMBERHDW(lpObj->Experience)));
  1117: 		pMsg.Experience[4] = SET_NUMBERHB(SET_NUMBERHW(SET_NUMBERLDW(lpObj->Experience)));
  1118: 		pMsg.Experience[5] = SET_NUMBERLB(SET_NUMBERHW(SET_NUMBERLDW(lpObj->Experience)));
  1119: 		pMsg.Experience[6] = SET_NUMBERHB(SET_NUMBERLW(SET_NUMBERLDW(lpObj->Experience)));
  1120: 		pMsg.Experience[7] = SET_NUMBERLB(SET_NUMBERLW(SET_NUMBERLDW(lpObj->Experience)));
  1121: 	}
  1122: 	else
  1123: 	{
  1124: 		pMsg.Experience[0] = SET_NUMBERHB(SET_NUMBERHW(SET_NUMBERHDW(lpObj->MasterExperience)));
  1125: 		pMsg.Experience[1] = SET_NUMBERLB(SET_NUMBERHW(SET_NUMBERHDW(lpObj->MasterExperience)));
  1126: 		pMsg.Experience[2] = SET_NUMBERHB(SET_NUMBERLW(SET_NUMBERHDW(lpObj->MasterExperience)));
  1127: 		pMsg.Experience[3] = SET_NUMBERLB(SET_NUMBERLW(SET_NUMBERHDW(lpObj->MasterExperience)));
  1128: 		pMsg.Experience[4] = SET_NUMBERHB(SET_NUMBERHW(SET_NUMBERLDW(lpObj->MasterExperience)));
  1129: 		pMsg.Experience[5] = SET_NUMBERLB(SET_NUMBERHW(SET_NUMBERLDW(lpObj->MasterExperience)));
  1130: 		pMsg.Experience[6] = SET_NUMBERHB(SET_NUMBERLW(SET_NUMBERLDW(lpObj->MasterExperience)));
  1131: 		pMsg.Experience[7] = SET_NUMBERLB(SET_NUMBERLW(SET_NUMBERLDW(lpObj->MasterExperience)));
  1132: 	}
  1133: 
  1134: 	if(gMasterSkillTree.CheckMasterLevel(lpObj) == 0)
  1135: 	{
  1136: 		pMsg.NextExperience[0] = SET_NUMBERHB(SET_NUMBERHW(SET_NUMBERHDW(lpObj->NextExperience)));
  1137: 		pMsg.NextExperience[1] = SET_NUMBERLB(SET_NUMBERHW(SET_NUMBERHDW(lpObj->NextExperience)));
  1138: 		pMsg.NextExperience[2] = SET_NUMBERHB(SET_NUMBERLW(SET_NUMBERHDW(lpObj->NextExperience)));
  1139: 		pMsg.NextExperience[3] = SET_NUMBERLB(SET_NUMBERLW(SET_NUMBERHDW(lpObj->NextExperience)));
  1140: 		pMsg.NextExperience[4] = SET_NUMBERHB(SET_NUMBERHW(SET_NUMBERLDW(lpObj->NextExperience)));
  1141: 		pMsg.NextExperience[5] = SET_NUMBERLB(SET_NUMBERHW(SET_NUMBERLDW(lpObj->NextExperience)));
  1142: 		pMsg.NextExperience[6] = SET_NUMBERHB(SET_NUMBERLW(SET_NUMBERLDW(lpObj->NextExperience)));
  1143: 		pMsg.NextExperience[7] = SET_NUMBERLB(SET_NUMBERLW(SET_NUMBERLDW(lpObj->NextExperience)));
  1144: 	}
  1145: 	else
  1146: 	{
  1147: 		pMsg.NextExperience[0] = SET_NUMBERHB(SET_NUMBERHW(SET_NUMBERHDW(lpObj->MasterNextExperience)));
  1148: 		pMsg.NextExperience[1] = SET_NUMBERLB(SET_NUMBERHW(SET_NUMBERHDW(lpObj->MasterNextExperience)));
  1149: 		pMsg.NextExperience[2] = SET_NUMBERHB(SET_NUMBERLW(SET_NUMBERHDW(lpObj->MasterNextExperience)));
  1150: 		pMsg.NextExperience[3] = SET_NUMBERLB(SET_NUMBERLW(SET_NUMBERHDW(lpObj->MasterNextExperience)));
  1151: 		pMsg.NextExperience[4] = SET_NUMBERHB(SET_NUMBERHW(SET_NUMBERLDW(lpObj->MasterNextExperience)));
  1152: 		pMsg.NextExperience[5] = SET_NUMBERLB(SET_NUMBERHW(SET_NUMBERLDW(lpObj->MasterNextExperience)));
  1153: 		pMsg.NextExperience[6] = SET_NUMBERHB(SET_NUMBERLW(SET_NUMBERLDW(lpObj->MasterNextExperience)));
  1154: 		pMsg.NextExperience[7] = SET_NUMBERLB(SET_NUMBERLW(SET_NUMBERLDW(lpObj->MasterNextExperience)));
  1155: 	}
  1156: 
  1157: 	pMsg.LevelUpPoint = lpObj->LevelUpPoint;
  1158: 	pMsg.Strength = GET_MAX_WORD_VALUE(lpObj->Strength);
  1159: 	pMsg.Dexterity = GET_MAX_WORD_VALUE(lpObj->Dexterity);
  1160: 	pMsg.Vitality = GET_MAX_WORD_VALUE(lpObj->Vitality);
  1161: 	pMsg.Energy = GET_MAX_WORD_VALUE(lpObj->Energy);
  1162: 	pMsg.Life = GET_MAX_WORD_VALUE(lpObj->Life);
  1163: 	pMsg.MaxLife = GET_MAX_WORD_VALUE((lpObj->MaxLife+lpObj->AddLife));
  1164: 	pMsg.Mana = GET_MAX_WORD_VALUE(lpObj->Mana);
  1165: 	pMsg.MaxMana = GET_MAX_WORD_VALUE((lpObj->MaxMana+lpObj->AddMana));
  1166: 	pMsg.Shield = GET_MAX_WORD_VALUE(lpObj->Shield);
  1167: 	pMsg.MaxShield = GET_MAX_WORD_VALUE((lpObj->MaxShield+lpObj->AddShield));
  1168: 	pMsg.BP = GET_MAX_WORD_VALUE(lpObj->BP);
  1169: 	pMsg.MaxBP = GET_MAX_WORD_VALUE((lpObj->MaxBP+lpObj->AddBP));
  1170: 	pMsg.Money = lpObj->Money;
  1171: 	pMsg.PKLevel = lpObj->PKLevel;
  1172: 	pMsg.CtlCode = lpMsg->CtlCode;
  1173: 	pMsg.FruitAddPoint = lpObj->FruitAddPoint;
  1174: 	pMsg.MaxFruitAddPoint = gFruit.GetMaxFruitPoint(lpObj);
  1175: 	pMsg.Leadership = GET_MAX_WORD_VALUE(lpObj->Leadership);
  1176: 	pMsg.FruitSubPoint = lpObj->FruitSubPoint;
  1177: 	pMsg.MaxFruitSubPoint = gFruit.GetMaxFruitPoint(lpObj);
  1178: 	#if(GAMESERVER_UPDATE>=602)
  1179: 	pMsg.ExtInventory = lpObj->ExtInventory;
  1180: 	#endif
  1181: 
  1182: 	#if(GAMESERVER_EXTRA==1)
  1183: 	pMsg.ViewReset = (DWORD)(lpObj->Reset);
  1184: 	pMsg.ViewPoint = (DWORD)(lpObj->LevelUpPoint);
  1185: 	pMsg.ViewCurHP = (DWORD)(lpObj->Life);
  1186: 	pMsg.ViewMaxHP = (DWORD)(lpObj->MaxLife+lpObj->AddLife);
  1187: 	pMsg.ViewCurMP = (DWORD)(lpObj->Mana);
  1188: 	pMsg.ViewMaxMP = (DWORD)(lpObj->MaxMana+lpObj->AddMana);
  1189: 	pMsg.ViewCurBP = (DWORD)(lpObj->BP);
  1190: 	pMsg.ViewMaxBP = (DWORD)(lpObj->MaxBP+lpObj->AddBP);
  1191: 	pMsg.ViewCurSD = (DWORD)(lpObj->Shield);
  1192: 	pMsg.ViewMaxSD = (DWORD)(lpObj->MaxShield+lpObj->AddShield);
  1193: 	pMsg.ViewStrength = lpObj->Strength;
  1194: 	pMsg.ViewDexterity = lpObj->Dexterity;
  1195: 	pMsg.ViewVitality = lpObj->Vitality;
  1196: 	pMsg.ViewEnergy = lpObj->Energy;
  1197: 	pMsg.ViewLeadership = lpObj->Leadership;
  1198: 	#endif
  1199: 
  1200: 	DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);
  1201: 
  1202: 	GDConnectCharacterSend(lpObj->Index);
  1203: 
  1204: 	GDOptionDataSend(lpObj->Index);
  1205: 
  1206: 	DGGuildMemberInfoRequest(lpObj->Index);
  1207: 
  1208: 	FriendListRequest(lpObj->Index);
  1209: 
  1210: 	GDPetItemInfoSend(lpObj->Index,0);
  1211: 
  1212: 	gQuest.GDQuestKillCountSend(lpObj->Index);
  1213: 
  1214: 	#if(GAMESERVER_UPDATE>=401)
  1215: 
  1216: 	gMasterSkillTree.GDMasterSkillTreeSend(lpObj->Index);
  1217: 
  1218: 	#endif
  1219: 
  1220: 	#if(GAMESERVER_UPDATE>=501)
  1221: 
  1222: 	gQuestWorld.GDQuestWorldSend(lpObj->Index);
  1223: 
  1224: 	gGensSystem.GDGensSystemMemberSend(lpObj->Index);
  1225: 
  1226: 	#endif
  1227: 
  1228: 	#if(GAMESERVER_UPDATE>=603)
  1229: 
  1230: 	gHelper.GDHelperDataSend(lpObj->Index);
  1231: 
  1232: 	#endif
  1233: 
  1234: 	gCashShop.GDCashShopPeriodicItemSend(lpObj->Index);
  1235: 
  1236: 	#if(GAMESERVER_UPDATE>=602)
  1237: 
  1238: 	gLuckyItem.GDLuckyItemSend(lpObj->Index);
  1239: 
  1240: 	#endif
  1241: 
  1242: 	#if(GAMESERVER_UPDATE>=701)
  1243: 
  1244: 	gPentagramSystem.GDPentagramJewelInfoSend(lpObj->Index,PENTAGRAM_JEWEL_TYPE_INVENTORY);
  1245: 
  1246: 	//gPentagramSystem.GDPentagramJewelInfoSend(lpObj->Index,PENTAGRAM_JEWEL_TYPE_WAREHOUSE);
  1247: 
  1248: 	#endif
  1249: 
  1250: 	#if(GAMESERVER_UPDATE>=801)
  1251: 
  1252: 	GDSNSDataSend(lpObj->Index);
  1253: 
  1254: 	#endif
  1255: 
  1256: 	#if(GAMESERVER_UPDATE>=802)
  1257: 
  1258: 	gPersonalShop.GDPShopItemValueSend(lpObj->Index);
  1259: 
  1260: 	gEventInventory.GDEventInventorySend(lpObj->Index);
  1261: 
  1262: 	gMuRummy.GDReqCardInfo(lpObj);
  1263: 
  1264: 	#endif
  1265: 
  1266: 	#if(GAMESERVER_UPDATE>=803)
  1267: 
  1268: 	gMuunSystem.GDMuunInventorySend(lpObj->Index);
  1269: 
  1270: 	#endif
  1271: 
  1272: 	gItemManager.GCItemListSend(lpObj->Index);
  1273: 
  1274: 	gSkillManager.GCSkillListSend(lpObj,0);
  1275: 
  1276: 	gQuest.GCQuestInfoSend(lpObj->Index);
  1277: 
  1278: 	gCashShop.GCCashShopInitSend(lpObj);
  1279: 
  1280: 	#if(GAMESERVER_UPDATE>=603)
  1281: 
  1282: 	gHelper.GCHelperStartSend(lpObj->Index,0,0,1);
  1283: 
  1284: 	#endif
  1285: 
  1286: 	GCNewGensBattleInfoSend(lpObj);
  1287: 
  1288: 	gReconnect.GetReconnectInfo(lpObj);
  1289: 
  1290: 	gSkillManager.SkillChangeUse(lpObj->Index);
  1291: 
  1292: 	gObjViewportListProtocolCreate(lpObj);
  1293: 
  1294: 	gObjectManager.CharacterUpdateMapEffect(lpObj);
  1295: 
  1296: 	if (gServerInfo.m_CustomRankUserType != 2)
  1297: 	{
  1298: 		gCustomRankUser.GCReqRankLevelUser(lpObj->Index, lpObj->Index);
  1299: 	}
  1300: 
  1301: 	gNotice.GCNoticeSend(lpObj->Index,0,0,0,0,0,0,gMessage.GetMessage(256),lpObj->Name);
  1302: 
  1303: 	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage((248+lpObj->AccountLevel)),lpObj->AccountExpireDate);
  1304: 
  1305: 	lpObj->MapServerMoveRequest = 0;
  1306: 
  1307: 	if( lpObj->Authority == 32)
  1308: 	{
  1309: 		if (gServerInfo.m_OnlineGmSwitch == 1)
  1310: 		{
  1311: 			#if GAMESERVER_CLIENTE_UPDATE >= 2
  1312: 			gNotice.GCNoticeSendToAll(0,0,0,0,0,0,gMessage.GetMessage(639),lpObj->Name);
  1313: 			#endif
  1314: 		}
  1315: 	}
  1316: 	else
  1317: 	{
  1318: 		if (gServerInfo.m_OnlineUserSwitch == 1)
  1319: 		{
  1320: 			#if GAMESERVER_CLIENTE_UPDATE >= 2
  1321: 			PostMessageUserON(lpObj->Name,gMessage.GetMessage(640));
  1322: 			#endif
  1323: 		}
  1324: 	}
  1325: 
  1326: 	gCustomStore.GCOffActiveSend(lpObj->Index,0);
  1327: 
  1328: 	gCashShop.CGCashShopPointRecv(lpObj->Index);
  1329: 
  1330: 	if(lpObj->CloseCount <= 0)
  1331: 	{
  1332: 		if(gServerInfo.m_ServerMinLevel != 0 && lpObj->Level < gServerInfo.m_ServerMinLevel)
  1333: 		{
  1334: 			lpObj->CloseCount = 6;
  1335: 			lpObj->CloseType = 3;
  1336: 			return;
  1337: 		}
  1338: 
  1339: 		if(gServerInfo.m_ServerMinReset != 0 && lpObj->Reset < gServerInfo.m_ServerMinReset)
  1340: 		{
  1341: 			lpObj->CloseCount = 6;
  1342: 			lpObj->CloseType = 3;
  1343: 			return;
  1344: 		}
  1345: 
  1346: 		if(gServerInfo.m_ServerMinMasterReset != 0 && lpObj->MasterReset < gServerInfo.m_ServerMinMasterReset)
  1347: 		{
  1348: 			lpObj->CloseCount = 6;
  1349: 			lpObj->CloseType = 3;
  1350: 			return;
  1351: 		}
  1352: 
  1353: 		if(gServerInfo.m_ServerMaxLevel != 0 && lpObj->Level > gServerInfo.m_ServerMaxLevel)
  1354: 		{
  1355: 			lpObj->CloseCount = 6;
  1356: 			lpObj->CloseType = 4;
  1357: 			return;
  1358: 		}
  1359: 
  1360: 		if(gServerInfo.m_ServerMaxReset != 0 && lpObj->Reset > gServerInfo.m_ServerMaxReset)
  1361: 		{
  1362: 			lpObj->CloseCount = 6;
  1363: 			lpObj->CloseType = 4;
  1364: 			return;
  1365: 		}
  1366: 
  1367: 		if(gServerInfo.m_ServerMaxMasterReset != 0 && lpObj->MasterReset > gServerInfo.m_ServerMaxMasterReset)
  1368: 		{
  1369: 			lpObj->CloseCount = 6;
  1370: 			lpObj->CloseType = 4;
  1371: 			return;
  1372: 		}
  1373: 	}
  1374: 
  1375: 	LogAddConnect(LOG_BLUE,"[Obj][%d] LogInCharacter (%s)",lpObj->Index,lpObj->Name);
  1376: }
  1377: 
  1378: void DGCreateItemRecv(SDHP_CREATE_ITEM_RECV* lpMsg) // OK
  1379: {
  1380: 	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
  1381: 	{
  1382: 		LogAdd(LOG_RED,"[DGCreateItemRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
  1383: 		CloseClient(lpMsg->index);
  1384: 		return;
  1385: 	}
  1386: 
  1387: 	LPOBJ lpObj = &gObj[lpMsg->index];
  1388: 
  1389: 	if(lpMsg->Map != 0xFF && lpMsg->Map != 0xFE && lpMsg->Map != 0xED && lpMsg->Map != 0xEC && lpMsg->Map != 0xEB && MAP_RANGE(lpMsg->Map) == 0)
  1390: 	{
  1391: 		return;
  1392: 	}
  1393: 
  1394: 	if(lpMsg->Map == 0xFE || lpMsg->Map == 0xFF)
  1395: 	{
  1396: 		if((lpMsg->Map == 0xFE && lpObj->Interface.type != INTERFACE_TRAINER) || (lpMsg->Map == 0xFF && lpObj->Interface.type != INTERFACE_CHAOS_BOX))
  1397: 		{
  1398: 			return;
  1399: 		}
  1400: 
  1401: 		CItem item;
  1402: 
  1403: 		item.m_Level = lpMsg->Level;
  1404: 
  1405: 		item.m_Serial = lpMsg->Serial;
  1406: 
  1407: 		if(lpMsg->ItemIndex == GET_ITEM(13,18) || lpMsg->ItemIndex == GET_ITEM(13,37) || lpMsg->ItemIndex == GET_ITEM(13,51) || lpMsg->ItemIndex == GET_ITEM(14,7) || lpMsg->ItemIndex == GET_ITEM(14,19) || lpMsg->ItemIndex == GET_ITEM(14,109))
  1408: 		{
  1409: 			item.m_Durability = (float)lpMsg->Dur;
  1410: 		}
  1411: 		else
  1412: 		{
  1413: 			item.m_Durability = (float)gItemManager.GetItemDurability(lpMsg->ItemIndex,lpMsg->Level,lpMsg->NewOption,lpMsg->SetOption);
  1414: 		}
  1415: 
  1416: 		item.Convert(lpMsg->ItemIndex,lpMsg->Option1,lpMsg->Option2,lpMsg->Option3,lpMsg->NewOption,lpMsg->SetOption,lpMsg->JewelOfHarmonyOption,lpMsg->ItemOptionEx,lpMsg->SocketOption,lpMsg->SocketOptionBonus);
  1417: 
  1418: 		if(lpMsg->ItemIndex == GET_ITEM(13,4) || lpMsg->ItemIndex == GET_ITEM(13,5))
  1419: 		{
  1420: 			item.SetPetItemInfo(1,0);
  1421: 		}
  1422: 
  1423: 		item.m_IsPeriodicItem = ((lpMsg->Duration>0)?1:0);
  1424: 
  1425: 		item.m_LoadPeriodicItem = ((lpMsg->Duration>0)?1:0);
  1426: 
  1427: 		item.m_PeriodicItemTime = ((lpMsg->Duration>0)?lpMsg->Duration:0);
  1428: 
  1429: 		gChaosBox.ChaosBoxInit(lpObj);
  1430: 
  1431: 		gItemManager.ChaosBoxAddItem(lpObj->Index,item,0);
  1432: 
  1433: 		gChaosBox.GCChaosMixSend(lpObj->Index,((lpMsg->Map==0xFF)?1:100),&item);
  1434: 	}
  1435: 	else if(lpMsg->Map == 0xEB)
  1436: 	{
  1437: 		CItem item;
  1438: 
  1439: 		item.m_Level = lpMsg->Level;
  1440: 
  1441: 		item.m_Serial = lpMsg->Serial;
  1442: 
  1443: 		item.m_Durability = lpMsg->Dur;
  1444: 
  1445: 		item.Convert(lpMsg->ItemIndex,lpMsg->Option1,lpMsg->Option2,lpMsg->Option3,lpMsg->NewOption,lpMsg->SetOption,lpMsg->JewelOfHarmonyOption,lpMsg->ItemOptionEx,lpMsg->SocketOption,lpMsg->SocketOptionBonus);
  1446: 
  1447: 		if(lpMsg->ItemIndex == GET_ITEM(13,4) || lpMsg->ItemIndex == GET_ITEM(13,5))
  1448: 		{
  1449: 			item.SetPetItemInfo(1,0);
  1450: 		}
  1451: 
  1452: 		item.m_IsPeriodicItem = ((lpMsg->Duration>0)?1:0);
  1453: 
  1454: 		item.m_LoadPeriodicItem = ((lpMsg->Duration>0)?1:0);
  1455: 
  1456: 		item.m_PeriodicItemTime = ((lpMsg->Duration>0)?lpMsg->Duration:0);
  1457: 
  1458: 		BYTE slot = gItemManager.InventoryInsertItem(lpObj->Index,item);
  1459: 
  1460: 		if(slot != 0xFF)
  1461: 		{
  1462: 			gItemManager.GCItemModifySend(lpObj->Index,slot);
  1463: 			if(lpMsg->Duration > 0){gCashShop.GCCashShopPeriodicItemSend(lpObj->Index,item.m_Index,slot,item.m_PeriodicItemTime);}
  1464: 		}
  1465: 	}
  1466: 	else if(lpMsg->Map == 0xEC)
  1467: 	{
  1468: 		CItem item;
  1469: 
  1470: 		item.m_Level = lpMsg->Level;
  1471: 
  1472: 		item.m_Serial = lpMsg->Serial;
  1473: 
  1474: 		item.m_Durability = lpMsg->Dur;
  1475: 
  1476: 		item.Convert(lpMsg->ItemIndex,lpMsg->Option1,lpMsg->Option2,lpMsg->Option3,lpMsg->NewOption,lpMsg->SetOption,lpMsg->JewelOfHarmonyOption,lpMsg->ItemOptionEx,lpMsg->SocketOption,lpMsg->SocketOptionBonus);
  1477: 
  1478: 		item.m_IsPeriodicItem = ((lpMsg->Duration>0)?1:0);
  1479: 
  1480: 		item.m_LoadPeriodicItem = ((lpMsg->Duration>0)?1:0);
  1481: 
  1482: 		item.m_PeriodicItemTime = ((lpMsg->Duration>0)?lpMsg->Duration:0);
  1483: 
  1484: 		BYTE slot = gEventInventory.EventInventoryInsertItem(lpObj->Index,item);
  1485: 
  1486: 		if(slot != 0xFF){gEventInventory.GCEventItemModifySend(lpObj->Index,slot);}
  1487: 	}
  1488: 	else if(lpMsg->Map == 0xED)
  1489: 	{
  1490: 		CItem item;
  1491: 
  1492: 		item.m_Level = lpMsg->Level;
  1493: 
  1494: 		item.m_Serial = lpMsg->Serial;
  1495: 
  1496: 		item.m_Durability = lpMsg->Dur;
```

### Anchors inside DGCharacterInfoRecv

- pMsg decl @ 1102: `	PMSG_CHARACTER_INFO_SEND pMsg;`

```
  1072: 		return;
  1073: 	}
  1074: 
  1075: 	if(gObjectManager.CharacterInfoSet((BYTE*)lpMsg,lpMsg->index) == 0)
  1076: 	{
  1077: 		CloseClient(lpMsg->index);
  1078: 		return;
  1079: 	}
  1080: 
  1081: 	LPOBJ lpObj = &gObj[lpMsg->index];
  1082: 
  1083: 	if(lpObj->MapServerMoveRequest == 0)
  1084: 	{
  1085: 		if((lpObj->NextServerCode=gMapServerManager.CheckMapServerMove(lpObj->Index,lpObj->Map,lpObj->LastServerCode)) != gServerInfo.m_ServerCode)
  1086: 		{
  1087: 			if(lpObj->NextServerCode == -1)
  1088: 			{
  1089: 				CloseClient(lpObj->Index);
  1090: 				return;
  1091: 			}
  1092: 			else
  1093: 			{
  1094: 				GJMapServerMoveSend(lpObj->Index,lpObj->NextServerCode,lpObj->Map,(BYTE)lpObj->X,(BYTE)lpObj->Y);
  1095: 				return;
  1096: 			}
  1097: 		}
  1098: 	}
  1099: 
  1100: 	lpObj->MapMoveDisable = 1;
  1101: 
  1102: 	PMSG_CHARACTER_INFO_SEND pMsg;
  1103: 
  1104: 	pMsg.header.set(0xF3,0x03,sizeof(pMsg));
  1105: 
  1106: 	pMsg.X = (BYTE)lpObj->X;
  1107: 	pMsg.Y = (BYTE)lpObj->Y;
  1108: 	pMsg.Map = lpObj->Map;
  1109: 	pMsg.Dir = lpObj->Dir;
  1110: 
  1111: 	if(gMasterSkillTree.CheckMasterLevel(lpObj) == 0)
  1112: 	{
  1113: 		pMsg.Experience[0] = SET_NUMBERHB(SET_NUMBERHW(SET_NUMBERHDW(lpObj->Experience)));
  1114: 		pMsg.Experience[1] = SET_NUMBERLB(SET_NUMBERHW(SET_NUMBERHDW(lpObj->Experience)));
  1115: 		pMsg.Experience[2] = SET_NUMBERHB(SET_NUMBERLW(SET_NUMBERHDW(lpObj->Experience)));
  1116: 		pMsg.Experience[3] = SET_NUMBERLB(SET_NUMBERLW(SET_NUMBERHDW(lpObj->Experience)));
  1117: 		pMsg.Experience[4] = SET_NUMBERHB(SET_NUMBERHW(SET_NUMBERLDW(lpObj->Experience)));
  1118: 		pMsg.Experience[5] = SET_NUMBERLB(SET_NUMBERHW(SET_NUMBERLDW(lpObj->Experience)));
  1119: 		pMsg.Experience[6] = SET_NUMBERHB(SET_NUMBERLW(SET_NUMBERLDW(lpObj->Experience)));
  1120: 		pMsg.Experience[7] = SET_NUMBERLB(SET_NUMBERLW(SET_NUMBERLDW(lpObj->Experience)));
  1121: 	}
  1122: 	else
  1123: 	{
  1124: 		pMsg.Experience[0] = SET_NUMBERHB(SET_NUMBERHW(SET_NUMBERHDW(lpObj->MasterExperience)));
  1125: 		pMsg.Experience[1] = SET_NUMBERLB(SET_NUMBERHW(SET_NUMBERHDW(lpObj->MasterExperience)));
  1126: 		pMsg.Experience[2] = SET_NUMBERHB(SET_NUMBERLW(SET_NUMBERHDW(lpObj->MasterExperience)));
  1127: 		pMsg.Experience[3] = SET_NUMBERLB(SET_NUMBERLW(SET_NUMBERHDW(lpObj->MasterExperience)));
  1128: 		pMsg.Experience[4] = SET_NUMBERHB(SET_NUMBERHW(SET_NUMBERLDW(lpObj->MasterExperience)));
  1129: 		pMsg.Experience[5] = SET_NUMBERLB(SET_NUMBERHW(SET_NUMBERLDW(lpObj->MasterExperience)));
  1130: 		pMsg.Experience[6] = SET_NUMBERHB(SET_NUMBERLW(SET_NUMBERLDW(lpObj->MasterExperience)));
  1131: 		pMsg.Experience[7] = SET_NUMBERLB(SET_NUMBERLW(SET_NUMBERLDW(lpObj->MasterExperience)));
  1132: 	}
  1133: 
  1134: 	if(gMasterSkillTree.CheckMasterLevel(lpObj) == 0)
  1135: 	{
  1136: 		pMsg.NextExperience[0] = SET_NUMBERHB(SET_NUMBERHW(SET_NUMBERHDW(lpObj->NextExperience)));
  1137: 		pMsg.NextExperience[1] = SET_NUMBERLB(SET_NUMBERHW(SET_NUMBERHDW(lpObj->NextExperience)));
  1138: 		pMsg.NextExperience[2] = SET_NUMBERHB(SET_NUMBERLW(SET_NUMBERHDW(lpObj->NextExperience)));
  1139: 		pMsg.NextExperience[3] = SET_NUMBERLB(SET_NUMBERLW(SET_NUMBERHDW(lpObj->NextExperience)));
  1140: 		pMsg.NextExperience[4] = SET_NUMBERHB(SET_NUMBERHW(SET_NUMBERLDW(lpObj->NextExperience)));
  1141: 		pMsg.NextExperience[5] = SET_NUMBERLB(SET_NUMBERHW(SET_NUMBERLDW(lpObj->NextExperience)));
  1142: 		pMsg.NextExperience[6] = SET_NUMBERHB(SET_NUMBERLW(SET_NUMBERLDW(lpObj->NextExperience)));
  1143: 		pMsg.NextExperience[7] = SET_NUMBERLB(SET_NUMBERLW(SET_NUMBERLDW(lpObj->NextExperience)));
  1144: 	}
  1145: 	else
  1146: 	{
  1147: 		pMsg.NextExperience[0] = SET_NUMBERHB(SET_NUMBERHW(SET_NUMBERHDW(lpObj->MasterNextExperience)));
  1148: 		pMsg.NextExperience[1] = SET_NUMBERLB(SET_NUMBERHW(SET_NUMBERHDW(lpObj->MasterNextExperience)));
  1149: 		pMsg.NextExperience[2] = SET_NUMBERHB(SET_NUMBERLW(SET_NUMBERHDW(lpObj->MasterNextExperience)));
  1150: 		pMsg.NextExperience[3] = SET_NUMBERLB(SET_NUMBERLW(SET_NUMBERHDW(lpObj->MasterNextExperience)));
  1151: 		pMsg.NextExperience[4] = SET_NUMBERHB(SET_NUMBERHW(SET_NUMBERLDW(lpObj->MasterNextExperience)));
  1152: 		pMsg.NextExperience[5] = SET_NUMBERLB(SET_NUMBERHW(SET_NUMBERLDW(lpObj->MasterNextExperience)));
  1153: 		pMsg.NextExperience[6] = SET_NUMBERHB(SET_NUMBERLW(SET_NUMBERLDW(lpObj->MasterNextExperience)));
  1154: 		pMsg.NextExperience[7] = SET_NUMBERLB(SET_NUMBERLW(SET_NUMBERLDW(lpObj->MasterNextExperience)));
  1155: 	}
  1156: 
  1157: 	pMsg.LevelUpPoint = lpObj->LevelUpPoint;
  1158: 	pMsg.Strength = GET_MAX_WORD_VALUE(lpObj->Strength);
  1159: 	pMsg.Dexterity = GET_MAX_WORD_VALUE(lpObj->Dexterity);
  1160: 	pMsg.Vitality = GET_MAX_WORD_VALUE(lpObj->Vitality);
  1161: 	pMsg.Energy = GET_MAX_WORD_VALUE(lpObj->Energy);
  1162: 	pMsg.Life = GET_MAX_WORD_VALUE(lpObj->Life);
  1163: 	pMsg.MaxLife = GET_MAX_WORD_VALUE((lpObj->MaxLife+lpObj->AddLife));
  1164: 	pMsg.Mana = GET_MAX_WORD_VALUE(lpObj->Mana);
  1165: 	pMsg.MaxMana = GET_MAX_WORD_VALUE((lpObj->MaxMana+lpObj->AddMana));
  1166: 	pMsg.Shield = GET_MAX_WORD_VALUE(lpObj->Shield);
  1167: 	pMsg.MaxShield = GET_MAX_WORD_VALUE((lpObj->MaxShield+lpObj->AddShield));
  1168: 	pMsg.BP = GET_MAX_WORD_VALUE(lpObj->BP);
  1169: 	pMsg.MaxBP = GET_MAX_WORD_VALUE((lpObj->MaxBP+lpObj->AddBP));
  1170: 	pMsg.Money = lpObj->Money;
  1171: 	pMsg.PKLevel = lpObj->PKLevel;
  1172: 	pMsg.CtlCode = lpMsg->CtlCode;
  1173: 	pMsg.FruitAddPoint = lpObj->FruitAddPoint;
  1174: 	pMsg.MaxFruitAddPoint = gFruit.GetMaxFruitPoint(lpObj);
  1175: 	pMsg.Leadership = GET_MAX_WORD_VALUE(lpObj->Leadership);
  1176: 	pMsg.FruitSubPoint = lpObj->FruitSubPoint;
  1177: 	pMsg.MaxFruitSubPoint = gFruit.GetMaxFruitPoint(lpObj);
  1178: 	#if(GAMESERVER_UPDATE>=602)
  1179: 	pMsg.ExtInventory = lpObj->ExtInventory;
  1180: 	#endif
  1181: 
  1182: 	#if(GAMESERVER_EXTRA==1)
  1183: 	pMsg.ViewReset = (DWORD)(lpObj->Reset);
  1184: 	pMsg.ViewPoint = (DWORD)(lpObj->LevelUpPoint);
  1185: 	pMsg.ViewCurHP = (DWORD)(lpObj->Life);
  1186: 	pMsg.ViewMaxHP = (DWORD)(lpObj->MaxLife+lpObj->AddLife);
  1187: 	pMsg.ViewCurMP = (DWORD)(lpObj->Mana);
  1188: 	pMsg.ViewMaxMP = (DWORD)(lpObj->MaxMana+lpObj->AddMana);
  1189: 	pMsg.ViewCurBP = (DWORD)(lpObj->BP);
  1190: 	pMsg.ViewMaxBP = (DWORD)(lpObj->MaxBP+lpObj->AddBP);
  1191: 	pMsg.ViewCurSD = (DWORD)(lpObj->Shield);
  1192: 	pMsg.ViewMaxSD = (DWORD)(lpObj->MaxShield+lpObj->AddShield);
  1193: 	pMsg.ViewStrength = lpObj->Strength;
  1194: 	pMsg.ViewDexterity = lpObj->Dexterity;
  1195: 	pMsg.ViewVitality = lpObj->Vitality;
  1196: 	pMsg.ViewEnergy = lpObj->Energy;
  1197: 	pMsg.ViewLeadership = lpObj->Leadership;
  1198: 	#endif
  1199: 
  1200: 	DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);
  1201: 
  1202: 	GDConnectCharacterSend(lpObj->Index);
  1203: 
  1204: 	GDOptionDataSend(lpObj->Index);
  1205: 
  1206: 	DGGuildMemberInfoRequest(lpObj->Index);
  1207: 
  1208: 	FriendListRequest(lpObj->Index);
  1209: 
  1210: 	GDPetItemInfoSend(lpObj->Index,0);
  1211: 
  1212: 	gQuest.GDQuestKillCountSend(lpObj->Index);
  1213: 
  1214: 	#if(GAMESERVER_UPDATE>=401)
  1215: 
  1216: 	gMasterSkillTree.GDMasterSkillTreeSend(lpObj->Index);
  1217: 
  1218: 	#endif
  1219: 
  1220: 	#if(GAMESERVER_UPDATE>=501)
  1221: 
  1222: 	gQuestWorld.GDQuestWorldSend(lpObj->Index);
```

- header.set(F3,03,...) @ 1104: `	pMsg.header.set(0xF3,0x03,sizeof(pMsg));`

```
  1054: void DGCharacterInfoRecv(SDHP_CHARACTER_INFO_RECV* lpMsg) // OK
  1055: {
  1056: 	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
  1057: 	{
  1058: 		LogAdd(LOG_RED,"[DGCharacterInfoRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
  1059: 		CloseClient(lpMsg->index);
  1060: 		return;
  1061: 	}
  1062: 
  1063: 	if(lpMsg->result == 0)
  1064: 	{
  1065: 		CloseClient(lpMsg->index);
  1066: 		return;
  1067: 	}
  1068: 
  1069: 	if((lpMsg->CtlCode & 1) != 0)
  1070: 	{
  1071: 		CloseClient(lpMsg->index);
  1072: 		return;
  1073: 	}
  1074: 
  1075: 	if(gObjectManager.CharacterInfoSet((BYTE*)lpMsg,lpMsg->index) == 0)
  1076: 	{
  1077: 		CloseClient(lpMsg->index);
  1078: 		return;
  1079: 	}
  1080: 
  1081: 	LPOBJ lpObj = &gObj[lpMsg->index];
  1082: 
  1083: 	if(lpObj->MapServerMoveRequest == 0)
  1084: 	{
  1085: 		if((lpObj->NextServerCode=gMapServerManager.CheckMapServerMove(lpObj->Index,lpObj->Map,lpObj->LastServerCode)) != gServerInfo.m_ServerCode)
  1086: 		{
  1087: 			if(lpObj->NextServerCode == -1)
  1088: 			{
  1089: 				CloseClient(lpObj->Index);
  1090: 				return;
  1091: 			}
  1092: 			else
  1093: 			{
  1094: 				GJMapServerMoveSend(lpObj->Index,lpObj->NextServerCode,lpObj->Map,(BYTE)lpObj->X,(BYTE)lpObj->Y);
  1095: 				return;
  1096: 			}
  1097: 		}
  1098: 	}
  1099: 
  1100: 	lpObj->MapMoveDisable = 1;
  1101: 
  1102: 	PMSG_CHARACTER_INFO_SEND pMsg;
  1103: 
  1104: 	pMsg.header.set(0xF3,0x03,sizeof(pMsg));
  1105: 
  1106: 	pMsg.X = (BYTE)lpObj->X;
  1107: 	pMsg.Y = (BYTE)lpObj->Y;
  1108: 	pMsg.Map = lpObj->Map;
  1109: 	pMsg.Dir = lpObj->Dir;
  1110: 
  1111: 	if(gMasterSkillTree.CheckMasterLevel(lpObj) == 0)
  1112: 	{
  1113: 		pMsg.Experience[0] = SET_NUMBERHB(SET_NUMBERHW(SET_NUMBERHDW(lpObj->Experience)));
  1114: 		pMsg.Experience[1] = SET_NUMBERLB(SET_NUMBERHW(SET_NUMBERHDW(lpObj->Experience)));
  1115: 		pMsg.Experience[2] = SET_NUMBERHB(SET_NUMBERLW(SET_NUMBERHDW(lpObj->Experience)));
  1116: 		pMsg.Experience[3] = SET_NUMBERLB(SET_NUMBERLW(SET_NUMBERHDW(lpObj->Experience)));
  1117: 		pMsg.Experience[4] = SET_NUMBERHB(SET_NUMBERHW(SET_NUMBERLDW(lpObj->Experience)));
  1118: 		pMsg.Experience[5] = SET_NUMBERLB(SET_NUMBERHW(SET_NUMBERLDW(lpObj->Experience)));
  1119: 		pMsg.Experience[6] = SET_NUMBERHB(SET_NUMBERLW(SET_NUMBERLDW(lpObj->Experience)));
  1120: 		pMsg.Experience[7] = SET_NUMBERLB(SET_NUMBERLW(SET_NUMBERLDW(lpObj->Experience)));
  1121: 	}
  1122: 	else
  1123: 	{
  1124: 		pMsg.Experience[0] = SET_NUMBERHB(SET_NUMBERHW(SET_NUMBERHDW(lpObj->MasterExperience)));
  1125: 		pMsg.Experience[1] = SET_NUMBERLB(SET_NUMBERHW(SET_NUMBERHDW(lpObj->MasterExperience)));
  1126: 		pMsg.Experience[2] = SET_NUMBERHB(SET_NUMBERLW(SET_NUMBERHDW(lpObj->MasterExperience)));
  1127: 		pMsg.Experience[3] = SET_NUMBERLB(SET_NUMBERLW(SET_NUMBERHDW(lpObj->MasterExperience)));
  1128: 		pMsg.Experience[4] = SET_NUMBERHB(SET_NUMBERHW(SET_NUMBERLDW(lpObj->MasterExperience)));
  1129: 		pMsg.Experience[5] = SET_NUMBERLB(SET_NUMBERHW(SET_NUMBERLDW(lpObj->MasterExperience)));
  1130: 		pMsg.Experience[6] = SET_NUMBERHB(SET_NUMBERLW(SET_NUMBERLDW(lpObj->MasterExperience)));
  1131: 		pMsg.Experience[7] = SET_NUMBERLB(SET_NUMBERLW(SET_NUMBERLDW(lpObj->MasterExperience)));
  1132: 	}
  1133: 
  1134: 	if(gMasterSkillTree.CheckMasterLevel(lpObj) == 0)
  1135: 	{
  1136: 		pMsg.NextExperience[0] = SET_NUMBERHB(SET_NUMBERHW(SET_NUMBERHDW(lpObj->NextExperience)));
  1137: 		pMsg.NextExperience[1] = SET_NUMBERLB(SET_NUMBERHW(SET_NUMBERHDW(lpObj->NextExperience)));
  1138: 		pMsg.NextExperience[2] = SET_NUMBERHB(SET_NUMBERLW(SET_NUMBERHDW(lpObj->NextExperience)));
  1139: 		pMsg.NextExperience[3] = SET_NUMBERLB(SET_NUMBERLW(SET_NUMBERHDW(lpObj->NextExperience)));
  1140: 		pMsg.NextExperience[4] = SET_NUMBERHB(SET_NUMBERHW(SET_NUMBERLDW(lpObj->NextExperience)));
  1141: 		pMsg.NextExperience[5] = SET_NUMBERLB(SET_NUMBERHW(SET_NUMBERLDW(lpObj->NextExperience)));
  1142: 		pMsg.NextExperience[6] = SET_NUMBERHB(SET_NUMBERLW(SET_NUMBERLDW(lpObj->NextExperience)));
  1143: 		pMsg.NextExperience[7] = SET_NUMBERLB(SET_NUMBERLW(SET_NUMBERLDW(lpObj->NextExperience)));
  1144: 	}
  1145: 	else
  1146: 	{
  1147: 		pMsg.NextExperience[0] = SET_NUMBERHB(SET_NUMBERHW(SET_NUMBERHDW(lpObj->MasterNextExperience)));
  1148: 		pMsg.NextExperience[1] = SET_NUMBERLB(SET_NUMBERHW(SET_NUMBERHDW(lpObj->MasterNextExperience)));
  1149: 		pMsg.NextExperience[2] = SET_NUMBERHB(SET_NUMBERLW(SET_NUMBERHDW(lpObj->MasterNextExperience)));
  1150: 		pMsg.NextExperience[3] = SET_NUMBERLB(SET_NUMBERLW(SET_NUMBERHDW(lpObj->MasterNextExperience)));
  1151: 		pMsg.NextExperience[4] = SET_NUMBERHB(SET_NUMBERHW(SET_NUMBERLDW(lpObj->MasterNextExperience)));
  1152: 		pMsg.NextExperience[5] = SET_NUMBERLB(SET_NUMBERHW(SET_NUMBERLDW(lpObj->MasterNextExperience)));
  1153: 		pMsg.NextExperience[6] = SET_NUMBERHB(SET_NUMBERLW(SET_NUMBERLDW(lpObj->MasterNextExperience)));
  1154: 		pMsg.NextExperience[7] = SET_NUMBERLB(SET_NUMBERLW(SET_NUMBERLDW(lpObj->MasterNextExperience)));
  1155: 	}
  1156: 
  1157: 	pMsg.LevelUpPoint = lpObj->LevelUpPoint;
  1158: 	pMsg.Strength = GET_MAX_WORD_VALUE(lpObj->Strength);
  1159: 	pMsg.Dexterity = GET_MAX_WORD_VALUE(lpObj->Dexterity);
  1160: 	pMsg.Vitality = GET_MAX_WORD_VALUE(lpObj->Vitality);
  1161: 	pMsg.Energy = GET_MAX_WORD_VALUE(lpObj->Energy);
  1162: 	pMsg.Life = GET_MAX_WORD_VALUE(lpObj->Life);
  1163: 	pMsg.MaxLife = GET_MAX_WORD_VALUE((lpObj->MaxLife+lpObj->AddLife));
  1164: 	pMsg.Mana = GET_MAX_WORD_VALUE(lpObj->Mana);
  1165: 	pMsg.MaxMana = GET_MAX_WORD_VALUE((lpObj->MaxMana+lpObj->AddMana));
  1166: 	pMsg.Shield = GET_MAX_WORD_VALUE(lpObj->Shield);
  1167: 	pMsg.MaxShield = GET_MAX_WORD_VALUE((lpObj->MaxShield+lpObj->AddShield));
  1168: 	pMsg.BP = GET_MAX_WORD_VALUE(lpObj->BP);
  1169: 	pMsg.MaxBP = GET_MAX_WORD_VALUE((lpObj->MaxBP+lpObj->AddBP));
  1170: 	pMsg.Money = lpObj->Money;
  1171: 	pMsg.PKLevel = lpObj->PKLevel;
  1172: 	pMsg.CtlCode = lpMsg->CtlCode;
  1173: 	pMsg.FruitAddPoint = lpObj->FruitAddPoint;
  1174: 	pMsg.MaxFruitAddPoint = gFruit.GetMaxFruitPoint(lpObj);
  1175: 	pMsg.Leadership = GET_MAX_WORD_VALUE(lpObj->Leadership);
  1176: 	pMsg.FruitSubPoint = lpObj->FruitSubPoint;
  1177: 	pMsg.MaxFruitSubPoint = gFruit.GetMaxFruitPoint(lpObj);
  1178: 	#if(GAMESERVER_UPDATE>=602)
  1179: 	pMsg.ExtInventory = lpObj->ExtInventory;
  1180: 	#endif
  1181: 
  1182: 	#if(GAMESERVER_EXTRA==1)
  1183: 	pMsg.ViewReset = (DWORD)(lpObj->Reset);
  1184: 	pMsg.ViewPoint = (DWORD)(lpObj->LevelUpPoint);
  1185: 	pMsg.ViewCurHP = (DWORD)(lpObj->Life);
  1186: 	pMsg.ViewMaxHP = (DWORD)(lpObj->MaxLife+lpObj->AddLife);
  1187: 	pMsg.ViewCurMP = (DWORD)(lpObj->Mana);
  1188: 	pMsg.ViewMaxMP = (DWORD)(lpObj->MaxMana+lpObj->AddMana);
  1189: 	pMsg.ViewCurBP = (DWORD)(lpObj->BP);
  1190: 	pMsg.ViewMaxBP = (DWORD)(lpObj->MaxBP+lpObj->AddBP);
  1191: 	pMsg.ViewCurSD = (DWORD)(lpObj->Shield);
  1192: 	pMsg.ViewMaxSD = (DWORD)(lpObj->MaxShield+lpObj->AddShield);
  1193: 	pMsg.ViewStrength = lpObj->Strength;
  1194: 	pMsg.ViewDexterity = lpObj->Dexterity;
  1195: 	pMsg.ViewVitality = lpObj->Vitality;
  1196: 	pMsg.ViewEnergy = lpObj->Energy;
  1197: 	pMsg.ViewLeadership = lpObj->Leadership;
  1198: 	#endif
  1199: 
  1200: 	DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);
  1201: 
  1202: 	GDConnectCharacterSend(lpObj->Index);
  1203: 
  1204: 	GDOptionDataSend(lpObj->Index);
  1205: 
  1206: 	DGGuildMemberInfoRequest(lpObj->Index);
  1207: 
  1208: 	FriendListRequest(lpObj->Index);
  1209: 
  1210: 	GDPetItemInfoSend(lpObj->Index,0);
  1211: 
  1212: 	gQuest.GDQuestKillCountSend(lpObj->Index);
  1213: 
  1214: 	#if(GAMESERVER_UPDATE>=401)
  1215: 
  1216: 	gMasterSkillTree.GDMasterSkillTreeSend(lpObj->Index);
  1217: 
  1218: 	#endif
  1219: 
  1220: 	#if(GAMESERVER_UPDATE>=501)
  1221: 
  1222: 	gQuestWorld.GDQuestWorldSend(lpObj->Index);
  1223: 
  1224: 	gGensSystem.GDGensSystemMemberSend(lpObj->Index);
  1225: 
  1226: 	#endif
  1227: 
  1228: 	#if(GAMESERVER_UPDATE>=603)
  1229: 
  1230: 	gHelper.GDHelperDataSend(lpObj->Index);
  1231: 
  1232: 	#endif
  1233: 
  1234: 	gCashShop.GDCashShopPeriodicItemSend(lpObj->Index);
  1235: 
  1236: 	#if(GAMESERVER_UPDATE>=602)
  1237: 
  1238: 	gLuckyItem.GDLuckyItemSend(lpObj->Index);
  1239: 
  1240: 	#endif
  1241: 
  1242: 	#if(GAMESERVER_UPDATE>=701)
  1243: 
  1244: 	gPentagramSystem.GDPentagramJewelInfoSend(lpObj->Index,PENTAGRAM_JEWEL_TYPE_INVENTORY);
  1245: 
  1246: 	//gPentagramSystem.GDPentagramJewelInfoSend(lpObj->Index,PENTAGRAM_JEWEL_TYPE_WAREHOUSE);
  1247: 
  1248: 	#endif
  1249: 
  1250: 	#if(GAMESERVER_UPDATE>=801)
  1251: 
  1252: 	GDSNSDataSend(lpObj->Index);
  1253: 
  1254: 	#endif
  1255: 
  1256: 	#if(GAMESERVER_UPDATE>=802)
  1257: 
  1258: 	gPersonalShop.GDPShopItemValueSend(lpObj->Index);
  1259: 
  1260: 	gEventInventory.GDEventInventorySend(lpObj->Index);
  1261: 
  1262: 	gMuRummy.GDReqCardInfo(lpObj);
  1263: 
  1264: 	#endif
  1265: 
  1266: 	#if(GAMESERVER_UPDATE>=803)
  1267: 
  1268: 	gMuunSystem.GDMuunInventorySend(lpObj->Index);
  1269: 
  1270: 	#endif
  1271: 
  1272: 	gItemManager.GCItemListSend(lpObj->Index);
  1273: 
  1274: 	gSkillManager.GCSkillListSend(lpObj,0);
  1275: 
  1276: 	gQuest.GCQuestInfoSend(lpObj->Index);
  1277: 
  1278: 	gCashShop.GCCashShopInitSend(lpObj);
  1279: 
  1280: 	#if(GAMESERVER_UPDATE>=603)
  1281: 
  1282: 	gHelper.GCHelperStartSend(lpObj->Index,0,0,1);
  1283: 
  1284: 	#endif
  1285: 
  1286: 	GCNewGensBattleInfoSend(lpObj);
  1287: 
  1288: 	gReconnect.GetReconnectInfo(lpObj);
  1289: 
  1290: 	gSkillManager.SkillChangeUse(lpObj->Index);
  1291: 
  1292: 	gObjViewportListProtocolCreate(lpObj);
  1293: 
  1294: 	gObjectManager.CharacterUpdateMapEffect(lpObj);
  1295: 
  1296: 	if (gServerInfo.m_CustomRankUserType != 2)
  1297: 	{
  1298: 		gCustomRankUser.GCReqRankLevelUser(lpObj->Index, lpObj->Index);
  1299: 	}
  1300: 
  1301: 	gNotice.GCNoticeSend(lpObj->Index,0,0,0,0,0,0,gMessage.GetMessage(256),lpObj->Name);
  1302: 
  1303: 	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage((248+lpObj->AccountLevel)),lpObj->AccountExpireDate);
  1304: 
  1305: 	lpObj->MapServerMoveRequest = 0;
  1306: 
  1307: 	if( lpObj->Authority == 32)
  1308: 	{
  1309: 		if (gServerInfo.m_OnlineGmSwitch == 1)
  1310: 		{
  1311: 			#if GAMESERVER_CLIENTE_UPDATE >= 2
  1312: 			gNotice.GCNoticeSendToAll(0,0,0,0,0,0,gMessage.GetMessage(639),lpObj->Name);
  1313: 			#endif
  1314: 		}
  1315: 	}
  1316: 	else
  1317: 	{
  1318: 		if (gServerInfo.m_OnlineUserSwitch == 1)
  1319: 		{
  1320: 			#if GAMESERVER_CLIENTE_UPDATE >= 2
  1321: 			PostMessageUserON(lpObj->Name,gMessage.GetMessage(640));
  1322: 			#endif
  1323: 		}
  1324: 	}
```

- DataSend(...) @ 1200: `	DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);`

```
  1140: 		pMsg.NextExperience[4] = SET_NUMBERHB(SET_NUMBERHW(SET_NUMBERLDW(lpObj->NextExperience)));
  1141: 		pMsg.NextExperience[5] = SET_NUMBERLB(SET_NUMBERHW(SET_NUMBERLDW(lpObj->NextExperience)));
  1142: 		pMsg.NextExperience[6] = SET_NUMBERHB(SET_NUMBERLW(SET_NUMBERLDW(lpObj->NextExperience)));
  1143: 		pMsg.NextExperience[7] = SET_NUMBERLB(SET_NUMBERLW(SET_NUMBERLDW(lpObj->NextExperience)));
  1144: 	}
  1145: 	else
  1146: 	{
  1147: 		pMsg.NextExperience[0] = SET_NUMBERHB(SET_NUMBERHW(SET_NUMBERHDW(lpObj->MasterNextExperience)));
  1148: 		pMsg.NextExperience[1] = SET_NUMBERLB(SET_NUMBERHW(SET_NUMBERHDW(lpObj->MasterNextExperience)));
  1149: 		pMsg.NextExperience[2] = SET_NUMBERHB(SET_NUMBERLW(SET_NUMBERHDW(lpObj->MasterNextExperience)));
  1150: 		pMsg.NextExperience[3] = SET_NUMBERLB(SET_NUMBERLW(SET_NUMBERHDW(lpObj->MasterNextExperience)));
  1151: 		pMsg.NextExperience[4] = SET_NUMBERHB(SET_NUMBERHW(SET_NUMBERLDW(lpObj->MasterNextExperience)));
  1152: 		pMsg.NextExperience[5] = SET_NUMBERLB(SET_NUMBERHW(SET_NUMBERLDW(lpObj->MasterNextExperience)));
  1153: 		pMsg.NextExperience[6] = SET_NUMBERHB(SET_NUMBERLW(SET_NUMBERLDW(lpObj->MasterNextExperience)));
  1154: 		pMsg.NextExperience[7] = SET_NUMBERLB(SET_NUMBERLW(SET_NUMBERLDW(lpObj->MasterNextExperience)));
  1155: 	}
  1156: 
  1157: 	pMsg.LevelUpPoint = lpObj->LevelUpPoint;
  1158: 	pMsg.Strength = GET_MAX_WORD_VALUE(lpObj->Strength);
  1159: 	pMsg.Dexterity = GET_MAX_WORD_VALUE(lpObj->Dexterity);
  1160: 	pMsg.Vitality = GET_MAX_WORD_VALUE(lpObj->Vitality);
  1161: 	pMsg.Energy = GET_MAX_WORD_VALUE(lpObj->Energy);
  1162: 	pMsg.Life = GET_MAX_WORD_VALUE(lpObj->Life);
  1163: 	pMsg.MaxLife = GET_MAX_WORD_VALUE((lpObj->MaxLife+lpObj->AddLife));
  1164: 	pMsg.Mana = GET_MAX_WORD_VALUE(lpObj->Mana);
  1165: 	pMsg.MaxMana = GET_MAX_WORD_VALUE((lpObj->MaxMana+lpObj->AddMana));
  1166: 	pMsg.Shield = GET_MAX_WORD_VALUE(lpObj->Shield);
  1167: 	pMsg.MaxShield = GET_MAX_WORD_VALUE((lpObj->MaxShield+lpObj->AddShield));
  1168: 	pMsg.BP = GET_MAX_WORD_VALUE(lpObj->BP);
  1169: 	pMsg.MaxBP = GET_MAX_WORD_VALUE((lpObj->MaxBP+lpObj->AddBP));
  1170: 	pMsg.Money = lpObj->Money;
  1171: 	pMsg.PKLevel = lpObj->PKLevel;
  1172: 	pMsg.CtlCode = lpMsg->CtlCode;
  1173: 	pMsg.FruitAddPoint = lpObj->FruitAddPoint;
  1174: 	pMsg.MaxFruitAddPoint = gFruit.GetMaxFruitPoint(lpObj);
  1175: 	pMsg.Leadership = GET_MAX_WORD_VALUE(lpObj->Leadership);
  1176: 	pMsg.FruitSubPoint = lpObj->FruitSubPoint;
  1177: 	pMsg.MaxFruitSubPoint = gFruit.GetMaxFruitPoint(lpObj);
  1178: 	#if(GAMESERVER_UPDATE>=602)
  1179: 	pMsg.ExtInventory = lpObj->ExtInventory;
  1180: 	#endif
  1181: 
  1182: 	#if(GAMESERVER_EXTRA==1)
  1183: 	pMsg.ViewReset = (DWORD)(lpObj->Reset);
  1184: 	pMsg.ViewPoint = (DWORD)(lpObj->LevelUpPoint);
  1185: 	pMsg.ViewCurHP = (DWORD)(lpObj->Life);
  1186: 	pMsg.ViewMaxHP = (DWORD)(lpObj->MaxLife+lpObj->AddLife);
  1187: 	pMsg.ViewCurMP = (DWORD)(lpObj->Mana);
  1188: 	pMsg.ViewMaxMP = (DWORD)(lpObj->MaxMana+lpObj->AddMana);
  1189: 	pMsg.ViewCurBP = (DWORD)(lpObj->BP);
  1190: 	pMsg.ViewMaxBP = (DWORD)(lpObj->MaxBP+lpObj->AddBP);
  1191: 	pMsg.ViewCurSD = (DWORD)(lpObj->Shield);
  1192: 	pMsg.ViewMaxSD = (DWORD)(lpObj->MaxShield+lpObj->AddShield);
  1193: 	pMsg.ViewStrength = lpObj->Strength;
  1194: 	pMsg.ViewDexterity = lpObj->Dexterity;
  1195: 	pMsg.ViewVitality = lpObj->Vitality;
  1196: 	pMsg.ViewEnergy = lpObj->Energy;
  1197: 	pMsg.ViewLeadership = lpObj->Leadership;
  1198: 	#endif
  1199: 
  1200: 	DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);
  1201: 
  1202: 	GDConnectCharacterSend(lpObj->Index);
  1203: 
  1204: 	GDOptionDataSend(lpObj->Index);
  1205: 
  1206: 	DGGuildMemberInfoRequest(lpObj->Index);
  1207: 
  1208: 	FriendListRequest(lpObj->Index);
  1209: 
  1210: 	GDPetItemInfoSend(lpObj->Index,0);
  1211: 
  1212: 	gQuest.GDQuestKillCountSend(lpObj->Index);
  1213: 
  1214: 	#if(GAMESERVER_UPDATE>=401)
  1215: 
  1216: 	gMasterSkillTree.GDMasterSkillTreeSend(lpObj->Index);
  1217: 
  1218: 	#endif
  1219: 
  1220: 	#if(GAMESERVER_UPDATE>=501)
  1221: 
  1222: 	gQuestWorld.GDQuestWorldSend(lpObj->Index);
  1223: 
  1224: 	gGensSystem.GDGensSystemMemberSend(lpObj->Index);
  1225: 
  1226: 	#endif
  1227: 
  1228: 	#if(GAMESERVER_UPDATE>=603)
  1229: 
  1230: 	gHelper.GDHelperDataSend(lpObj->Index);
  1231: 
  1232: 	#endif
  1233: 
  1234: 	gCashShop.GDCashShopPeriodicItemSend(lpObj->Index);
  1235: 
  1236: 	#if(GAMESERVER_UPDATE>=602)
  1237: 
  1238: 	gLuckyItem.GDLuckyItemSend(lpObj->Index);
  1239: 
  1240: 	#endif
  1241: 
  1242: 	#if(GAMESERVER_UPDATE>=701)
  1243: 
  1244: 	gPentagramSystem.GDPentagramJewelInfoSend(lpObj->Index,PENTAGRAM_JEWEL_TYPE_INVENTORY);
  1245: 
  1246: 	//gPentagramSystem.GDPentagramJewelInfoSend(lpObj->Index,PENTAGRAM_JEWEL_TYPE_WAREHOUSE);
  1247: 
  1248: 	#endif
  1249: 
  1250: 	#if(GAMESERVER_UPDATE>=801)
  1251: 
  1252: 	GDSNSDataSend(lpObj->Index);
  1253: 
  1254: 	#endif
  1255: 
  1256: 	#if(GAMESERVER_UPDATE>=802)
  1257: 
  1258: 	gPersonalShop.GDPShopItemValueSend(lpObj->Index);
  1259: 
  1260: 	gEventInventory.GDEventInventorySend(lpObj->Index);
  1261: 
  1262: 	gMuRummy.GDReqCardInfo(lpObj);
  1263: 
  1264: 	#endif
  1265: 
  1266: 	#if(GAMESERVER_UPDATE>=803)
  1267: 
  1268: 	gMuunSystem.GDMuunInventorySend(lpObj->Index);
  1269: 
  1270: 	#endif
  1271: 
  1272: 	gItemManager.GCItemListSend(lpObj->Index);
  1273: 
  1274: 	gSkillManager.GCSkillListSend(lpObj,0);
  1275: 
  1276: 	gQuest.GCQuestInfoSend(lpObj->Index);
  1277: 
  1278: 	gCashShop.GCCashShopInitSend(lpObj);
  1279: 
  1280: 	#if(GAMESERVER_UPDATE>=603)
  1281: 
  1282: 	gHelper.GCHelperStartSend(lpObj->Index,0,0,1);
  1283: 
  1284: 	#endif
  1285: 
  1286: 	GCNewGensBattleInfoSend(lpObj);
  1287: 
  1288: 	gReconnect.GetReconnectInfo(lpObj);
  1289: 
  1290: 	gSkillManager.SkillChangeUse(lpObj->Index);
  1291: 
  1292: 	gObjViewportListProtocolCreate(lpObj);
  1293: 
  1294: 	gObjectManager.CharacterUpdateMapEffect(lpObj);
  1295: 
  1296: 	if (gServerInfo.m_CustomRankUserType != 2)
  1297: 	{
  1298: 		gCustomRankUser.GCReqRankLevelUser(lpObj->Index, lpObj->Index);
  1299: 	}
  1300: 
  1301: 	gNotice.GCNoticeSend(lpObj->Index,0,0,0,0,0,0,gMessage.GetMessage(256),lpObj->Name);
  1302: 
  1303: 	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage((248+lpObj->AccountLevel)),lpObj->AccountExpireDate);
  1304: 
  1305: 	lpObj->MapServerMoveRequest = 0;
  1306: 
  1307: 	if( lpObj->Authority == 32)
  1308: 	{
  1309: 		if (gServerInfo.m_OnlineGmSwitch == 1)
  1310: 		{
  1311: 			#if GAMESERVER_CLIENTE_UPDATE >= 2
  1312: 			gNotice.GCNoticeSendToAll(0,0,0,0,0,0,gMessage.GetMessage(639),lpObj->Name);
  1313: 			#endif
  1314: 		}
  1315: 	}
  1316: 	else
  1317: 	{
  1318: 		if (gServerInfo.m_OnlineUserSwitch == 1)
  1319: 		{
  1320: 			#if GAMESERVER_CLIENTE_UPDATE >= 2
  1321: 			PostMessageUserON(lpObj->Name,gMessage.GetMessage(640));
  1322: 			#endif
  1323: 		}
  1324: 	}
  1325: 
  1326: 	gCustomStore.GCOffActiveSend(lpObj->Index,0);
  1327: 
  1328: 	gCashShop.CGCashShopPointRecv(lpObj->Index);
  1329: 
  1330: 	if(lpObj->CloseCount <= 0)
  1331: 	{
  1332: 		if(gServerInfo.m_ServerMinLevel != 0 && lpObj->Level < gServerInfo.m_ServerMinLevel)
  1333: 		{
  1334: 			lpObj->CloseCount = 6;
  1335: 			lpObj->CloseType = 3;
  1336: 			return;
  1337: 		}
  1338: 
  1339: 		if(gServerInfo.m_ServerMinReset != 0 && lpObj->Reset < gServerInfo.m_ServerMinReset)
  1340: 		{
  1341: 			lpObj->CloseCount = 6;
  1342: 			lpObj->CloseType = 3;
  1343: 			return;
  1344: 		}
  1345: 
  1346: 		if(gServerInfo.m_ServerMinMasterReset != 0 && lpObj->MasterReset < gServerInfo.m_ServerMinMasterReset)
  1347: 		{
  1348: 			lpObj->CloseCount = 6;
  1349: 			lpObj->CloseType = 3;
  1350: 			return;
  1351: 		}
  1352: 
  1353: 		if(gServerInfo.m_ServerMaxLevel != 0 && lpObj->Level > gServerInfo.m_ServerMaxLevel)
  1354: 		{
  1355: 			lpObj->CloseCount = 6;
  1356: 			lpObj->CloseType = 4;
  1357: 			return;
  1358: 		}
  1359: 
  1360: 		if(gServerInfo.m_ServerMaxReset != 0 && lpObj->Reset > gServerInfo.m_ServerMaxReset)
  1361: 		{
  1362: 			lpObj->CloseCount = 6;
  1363: 			lpObj->CloseType = 4;
  1364: 			return;
  1365: 		}
  1366: 
  1367: 		if(gServerInfo.m_ServerMaxMasterReset != 0 && lpObj->MasterReset > gServerInfo.m_ServerMaxMasterReset)
  1368: 		{
  1369: 			lpObj->CloseCount = 6;
  1370: 			lpObj->CloseType = 4;
  1371: 			return;
  1372: 		}
  1373: 	}
  1374: 
  1375: 	LogAddConnect(LOG_BLUE,"[Obj][%d] LogInCharacter (%s)",lpObj->Index,lpObj->Name);
  1376: }
  1377: 
  1378: void DGCreateItemRecv(SDHP_CREATE_ITEM_RECV* lpMsg) // OK
  1379: {
  1380: 	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
  1381: 	{
  1382: 		LogAdd(LOG_RED,"[DGCreateItemRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
  1383: 		CloseClient(lpMsg->index);
  1384: 		return;
  1385: 	}
  1386: 
  1387: 	LPOBJ lpObj = &gObj[lpMsg->index];
  1388: 
  1389: 	if(lpMsg->Map != 0xFF && lpMsg->Map != 0xFE && lpMsg->Map != 0xED && lpMsg->Map != 0xEC && lpMsg->Map != 0xEB && MAP_RANGE(lpMsg->Map) == 0)
  1390: 	{
  1391: 		return;
  1392: 	}
  1393: 
  1394: 	if(lpMsg->Map == 0xFE || lpMsg->Map == 0xFF)
  1395: 	{
  1396: 		if((lpMsg->Map == 0xFE && lpObj->Interface.type != INTERFACE_TRAINER) || (lpMsg->Map == 0xFF && lpObj->Interface.type != INTERFACE_CHAOS_BOX))
  1397: 		{
  1398: 			return;
  1399: 		}
  1400: 
  1401: 		CItem item;
  1402: 
  1403: 		item.m_Level = lpMsg->Level;
  1404: 
  1405: 		item.m_Serial = lpMsg->Serial;
  1406: 
  1407: 		if(lpMsg->ItemIndex == GET_ITEM(13,18) || lpMsg->ItemIndex == GET_ITEM(13,37) || lpMsg->ItemIndex == GET_ITEM(13,51) || lpMsg->ItemIndex == GET_ITEM(14,7) || lpMsg->ItemIndex == GET_ITEM(14,19) || lpMsg->ItemIndex == GET_ITEM(14,109))
  1408: 		{
  1409: 			item.m_Durability = (float)lpMsg->Dur;
  1410: 		}
  1411: 		else
  1412: 		{
  1413: 			item.m_Durability = (float)gItemManager.GetItemDurability(lpMsg->ItemIndex,lpMsg->Level,lpMsg->NewOption,lpMsg->SetOption);
  1414: 		}
  1415: 
  1416: 		item.Convert(lpMsg->ItemIndex,lpMsg->Option1,lpMsg->Option2,lpMsg->Option3,lpMsg->NewOption,lpMsg->SetOption,lpMsg->JewelOfHarmonyOption,lpMsg->ItemOptionEx,lpMsg->SocketOption,lpMsg->SocketOptionBonus);
  1417: 
  1418: 		if(lpMsg->ItemIndex == GET_ITEM(13,4) || lpMsg->ItemIndex == GET_ITEM(13,5))
  1419: 		{
  1420: 			item.SetPetItemInfo(1,0);
```


---

## Protocol.h — struct enviada (layout)

### struct PMSG_CHARACTER_INFO_SEND @ line 835
```
   795: struct PMSG_CHARACTER_LIST_SEND
   796: {
   797: 	#if(NEW_PROTOCOL_SYSTEM==0)
   798: 	PSBMSG_HEAD header; // C1:F3:00
   799: 	#endif
   800: 	BYTE ClassCode;
   801: 	BYTE MoveCnt;
   802: 	BYTE count;
   803: 	//#if(GAMESERVER_UPDATE>=602)
   804: 	//BYTE ExtWarehouse;
   805: 	//#endif
   806: };
   807: 
   808: struct PMSG_CHARACTER_LIST
   809: {
   810: 	BYTE slot;
   811: 	char Name[10];
   812: 	WORD Level;
   813: 	BYTE CtlCode;
   814: 	BYTE CharSet[18];
   815: 	BYTE GuildStatus;
   816: };
   817: 
   818: struct PMSG_CHARACTER_CREATE_SEND
   819: {
   820: 	PSBMSG_HEAD header; // C1:F3:01
   821: 	BYTE result;
   822: 	char name[10];
   823: 	BYTE slot;
   824: 	WORD level;
   825: 	BYTE Class;
   826: 	BYTE equipment[24];
   827: };
   828: 
   829: struct PMSG_CHARACTER_DELETE_SEND
   830: {
   831: 	PSBMSG_HEAD header; // C1:F3:02
   832: 	BYTE result;
   833: };
   834: 
   835: struct PMSG_CHARACTER_INFO_SEND
   836: {
   837: 	PSBMSG_HEAD header; // C3:F3:03
   838: 	BYTE X;
   839: 	BYTE Y;
   840: 	BYTE Map;
   841: 	BYTE Dir;
   842: 	BYTE Experience[8];
   843: 	BYTE NextExperience[8];
   844: 	WORD LevelUpPoint;
   845: 	WORD Strength;
   846: 	WORD Dexterity;
   847: 	WORD Vitality;
   848: 	WORD Energy;
   849: 	WORD Life;
   850: 	WORD MaxLife;
   851: 	WORD Mana;
   852: 	WORD MaxMana;
   853: 	WORD Shield;
   854: 	WORD MaxShield;
   855: 	WORD BP;
   856: 	WORD MaxBP;
   857: 	DWORD Money;
   858: 	BYTE PKLevel;
   859: 	BYTE CtlCode;
   860: 	WORD FruitAddPoint;
   861: 	WORD MaxFruitAddPoint;
   862: 	WORD Leadership;
   863: 	WORD FruitSubPoint;
   864: 	WORD MaxFruitSubPoint;
   865: 	#if(GAMESERVER_UPDATE>=602)
   866: 	BYTE ExtInventory;
   867: 	#endif
   868: 	#if(GAMESERVER_EXTRA==1)
   869: 	DWORD ViewReset;
   870: 	DWORD ViewPoint;
   871: 	DWORD ViewCurHP;
   872: 	DWORD ViewMaxHP;
   873: 	DWORD ViewCurMP;
   874: 	DWORD ViewMaxMP;
   875: 	DWORD ViewCurBP;
   876: 	DWORD ViewMaxBP;
   877: 	DWORD ViewCurSD;
   878: 	DWORD ViewMaxSD;
   879: 	DWORD ViewStrength;
   880: 	DWORD ViewDexterity;
   881: 	DWORD ViewVitality;
   882: 	DWORD ViewEnergy;
   883: 	DWORD ViewLeadership;
   884: 	#endif
   885: };
   886: 
   887: struct PMSG_CHARACTER_REGEN_SEND
   888: {
   889: 	PSBMSG_HEAD header; // C3:F3:04
   890: 	BYTE X;
   891: 	BYTE Y;
   892: 	BYTE Map;
   893: 	BYTE Dir;
   894: 	WORD Life;
   895: 	WORD Mana;
   896: 	WORD Shield;
   897: 	WORD BP;
   898: 	BYTE Experience[8];
   899: 	DWORD Money;
   900: 	#if(GAMESERVER_EXTRA==1)
   901: 	DWORD ViewCurHP;
   902: 	DWORD ViewCurMP;
   903: 	DWORD ViewCurBP;
   904: 	DWORD ViewCurSD;
   905: 	#endif
   906: };
   907: 
   908: struct PMSG_LEVEL_UP_SEND
   909: {
   910: 	PSBMSG_HEAD header; // C1:F3:05
   911: 	WORD Level;
   912: 	WORD LevelUpPoint;
   913: 	WORD MaxLife;
   914: 	WORD MaxMana;
   915: 	WORD MaxShield;
   916: 	WORD MaxBP;
   917: 	WORD FruitAddPoint;
   918: 	WORD MaxFruitAddPoint;
   919: 	WORD FruitSubPoint;
   920: 	WORD MaxFruitSubPoint;
   921: 	#if(GAMESERVER_EXTRA==1)
   922: 	DWORD ViewPoint;
   923: 	DWORD ViewMaxHP;
   924: 	DWORD ViewMaxMP;
   925: 	DWORD ViewMaxBP;
   926: 	DWORD ViewMaxSD;
   927: 	DWORD ViewExperience;
   928: 	DWORD ViewNextExperience;
   929: 	#endif
   930: };
   931: 
   932: struct PMSG_LEVEL_UP_POINT_SEND
   933: {
   934: 	PSBMSG_HEAD header; // C1:F3:06
   935: 	BYTE result;
   936: 	WORD MaxLifeAndMana;
   937: 	WORD MaxShield;
   938: 	WORD MaxBP;
   939: 	#if(GAMESERVER_EXTRA==1)
   940: 	DWORD ViewPoint;
   941: 	DWORD ViewMaxHP;
   942: 	DWORD ViewMaxMP;
   943: 	DWORD ViewMaxBP;
   944: 	DWORD ViewMaxSD;
   945: 	DWORD ViewStrength;
   946: 	DWORD ViewDexterity;
   947: 	DWORD ViewVitality;
   948: 	DWORD ViewEnergy;
   949: 	DWORD ViewLeadership;
   950: 	#endif
   951: };
   952: 
   953: struct PMSG_MONSTER_DAMAGE_SEND
   954: {
   955: 	PSBMSG_HEAD header; // C1:F3:07
   956: 	BYTE damage[2];
   957: 	BYTE ShieldDamage[2];
   958: 	#if(GAMESERVER_EXTRA==1)
   959: 	DWORD ViewCurHP;
   960: 	DWORD ViewCurSD;
   961: 	DWORD ViewDamageHP;
   962: 	DWORD ViewDamageSD;
   963: 	#endif
   964: };
   965: 
   966: struct PMSG_PK_LEVEL_SEND
   967: {
   968: 	PSBMSG_HEAD header; // C1:F3:08
   969: 	BYTE index[2];
   970: 	BYTE PKLevel;
   971: };
   972: 
   973: struct PMSG_CHARACTER_NAME_CHECK_SEND
   974: {
   975: 	PSBMSG_HEAD header; // C1:F3:15
   976: 	char name[10];
   977: 	BYTE result;
   978: };
   979: 
   980: struct PMSG_CHARACTER_NAME_CHANGE_SEND
   981: {
   982: 	PSBMSG_HEAD header; // C1:F3:16
   983: 	char OldName[10];
   984: 	char NewName[10];
   985: 	BYTE result;
   986: };
   987: 
   988: struct PMSG_SUMMON_LIFE_SEND
   989: {
   990: 	PSBMSG_HEAD header; // C1:F3:20
   991: 	BYTE life;
   992: };
   993: 
   994: struct PMSG_TIME_VIEW_SEND
   995: {
   996: 	PSBMSG_HEAD header; // C1:F3:22
   997: 	WORD time;
   998: };
   999: 
  1000: struct PMSG_OPTION_DATA_SEND
  1001: {
  1002: 	#pragma pack(1)
  1003: 	PSBMSG_HEAD header; // C1:F3:30
  1004: 	BYTE SkillKey[20];
  1005: 	BYTE GameOption;
  1006: 	BYTE QKey;
  1007: 	BYTE WKey;
  1008: 	BYTE EKey;
  1009: 	BYTE ChatWindow;
  1010: 	BYTE RKey;
  1011: 	DWORD QWERLevel;
  1012: 	#pragma pack()
  1013: };
  1014: 
  1015: struct PMSG_MATH_AUTHENTICATOR_SEND
  1016: {
  1017: 	PSBMSG_HEAD header; // C1:F3:32
  1018: 	DWORD function;
  1019: 	float value;
  1020: 	char result[64];
  1021: };
  1022: 
  1023: struct PMSG_FIREWORKS_SEND
  1024: {
  1025: 	PSBMSG_HEAD header; // C1:F3:40
  1026: 	BYTE type;
  1027: 	BYTE x;
  1028: 	BYTE y;
  1029: };
  1030: 
  1031: struct PMSG_SERVER_COMMAND_SEND
  1032: {
  1033: 	PSBMSG_HEAD header; // C1:F3:40
  1034: 	BYTE type;
  1035: 	BYTE cmd1;
  1036: 	BYTE cmd2;
  1037: };
  1038: 
  1039: struct PMSG_ACHERON_ENTER_SEND
  1040: {
  1041: 	PSBMSG_HEAD header; // C1:F8:21
  1042: 	BYTE result;
  1043: };
  1044: 
  1045: struct PMSG_SNS_DATA_SEND
  1046: {
  1047: 	PWMSG_HEAD header; // C1:FB
  1048: 	BYTE result;
  1049: 	BYTE data[256];
  1050: };
  1051: 
  1052: struct PMSG_NEW_CHARACTER_INFO_SEND
  1053: {
  1054: 	PSBMSG_HEAD header; // C1:F3:E0
  1055: 	WORD Level;
  1056: 	WORD LevelUpPoint;
  1057: 	DWORD Experience;
  1058: 	DWORD NextExperience;
  1059: 	WORD Strength;
  1060: 	WORD Dexterity;
  1061: 	WORD Vitality;
  1062: 	WORD Energy;
  1063: 	WORD Leadership;
  1064: 	WORD Life;
  1065: 	WORD MaxLife;
  1066: 	WORD Mana;
  1067: 	WORD MaxMana;
  1068: 	WORD BP;
  1069: 	WORD MaxBP;
  1070: 	WORD Shield;
  1071: 	WORD MaxShield;
  1072: 	WORD FruitAddPoint;
  1073: 	WORD MaxFruitAddPoint;
  1074: 	WORD FruitSubPoint;
  1075: 	WORD MaxFruitSubPoint;
  1076: 	//EXTRA
  1077: 	DWORD ViewReset;
  1078: 	DWORD ViewPoint;
  1079: 	DWORD ViewCurHP;
  1080: 	DWORD ViewMaxHP;
  1081: 	DWORD ViewCurMP;
  1082: 	DWORD ViewMaxMP;
  1083: 	DWORD ViewCurBP;
  1084: 	DWORD ViewMaxBP;
  1085: 	DWORD ViewCurSD;
  1086: 	DWORD ViewMaxSD;
  1087: 	DWORD ViewStrength;
  1088: 	DWORD ViewDexterity;
  1089: 	DWORD ViewVitality;
  1090: 	DWORD ViewEnergy;
  1091: 	DWORD ViewLeadership;
  1092: };
  1093: 
  1094: struct PMSG_NEW_CHARACTER_CALC_SEND
  1095: {
  1096: 	PSBMSG_HEAD header; // C1:F3:E1
  1097: 	DWORD ViewCurHP;
  1098: 	DWORD ViewMaxHP;
  1099: 	DWORD ViewCurMP;
  1100: 	DWORD ViewMaxMP;
  1101: 	DWORD ViewCurBP;
  1102: 	DWORD ViewMaxBP;
  1103: 	DWORD ViewCurSD;
  1104: 	DWORD ViewMaxSD;
  1105: 	DWORD ViewAddStrength;
  1106: 	DWORD ViewAddDexterity;
  1107: 	DWORD ViewAddVitality;
  1108: 	DWORD ViewAddEnergy;
  1109: 	DWORD ViewAddLeadership;
  1110: 	DWORD ViewPhysiDamageMin;
  1111: 	DWORD ViewPhysiDamageMax;
  1112: 	DWORD ViewMagicDamageMin;
  1113: 	DWORD ViewMagicDamageMax;
  1114: 	DWORD ViewCurseDamageMin;
  1115: 	DWORD ViewCurseDamageMax;
  1116: 	DWORD ViewMulPhysiDamage;
  1117: 	DWORD ViewDivPhysiDamage;
  1118: 	DWORD ViewMulMagicDamage;
  1119: 	DWORD ViewDivMagicDamage;
  1120: 	DWORD ViewMulCurseDamage;
  1121: 	DWORD ViewDivCurseDamage;
  1122: 	DWORD ViewMagicDamageRate;
  1123: 	DWORD ViewCurseDamageRate;
  1124: 	DWORD ViewPhysiSpeed;
  1125: 	DWORD ViewMagicSpeed;
  1126: 	DWORD ViewAttackSuccessRate;
  1127: 	DWORD ViewAttackSuccessRatePvP;
  1128: 	DWORD ViewDefense;
  1129: 	DWORD ViewDefenseSuccessRate;
  1130: 	DWORD ViewDefenseSuccessRatePvP;
  1131: 	DWORD ViewDamageMultiplier;
  1132: 	DWORD ViewRFDamageMultiplierA;
  1133: 	DWORD ViewRFDamageMultiplierB;
  1134: 	DWORD ViewRFDamageMultiplierC;
  1135: 	DWORD ViewDarkSpiritAttackDamageMin;
  1136: 	DWORD ViewDarkSpiritAttackDamageMax;
  1137: 	DWORD ViewDarkSpiritAttackSpeed;
  1138: 	DWORD ViewDarkSpiritAttackSuccessRate;
  1139: };
  1140: 
  1141: struct PMSG_NEW_HEALTH_BAR_SEND
  1142: {
  1143: 	PSWMSG_HEAD header; // C2:F3:E2
  1144: 	BYTE count;
  1145: };
  1146: 
  1147: struct PMSG_NEW_HEALTH_BAR
  1148: {
  1149: 	WORD index;
  1150: 	BYTE type;
  1151: 	BYTE rate;
  1152: 	BYTE rate2;
  1153: };
  1154: 
  1155: struct PMSG_NEW_GENS_BATTLE_INFO_SEND
  1156: {
  1157: 	PSBMSG_HEAD header; // C1:F3:E3
  1158: 	BYTE GensBattleMapCount;
  1159: 	BYTE GensMoveIndexCount;
  1160: 	BYTE GensBattleMap[120];
  1161: 	BYTE GensMoveIndex[120];
  1162: };
  1163: 
  1164: struct PMSG_NEW_MESSAGE_SEND
  1165: {
  1166: 	PSBMSG_HEAD header; // C1:F3:E4
  1167: 	char message[128];
  1168: };
  1169: 
  1170: struct PMSG_OFFTRADE_SEND
  1171: {
  1172: 	PSBMSG_HEAD header;
  1173: 	int Type;
  1174: };
  1175: 
```


---

## Protocol.h — PSBMSG_HEAD (tipo C1 vs C3)

### PSBMSG_HEAD @ line 100
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
```


---

## QUESTIONS (para governança)

1) CONFIRMED: existe envio GS->client de `F3:03` (CharacterInfo) neste pin?

2) Isso SUPERSEDE apenas a parte do §86 que dizia "GS não emite F3:03" (JoinMapServer continua inexistente).

3) O byte inicial on-wire é C1 ou C3 neste envio? (ver PSBMSG_HEAD::set vs comentários)
