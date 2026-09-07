# NEW-ERA 1.3-W — GS: callers de GCItemListSend / GCItemEquipmentSend (gatilho de inventário/equipment)

- Commit pin: 580472e0d5723f9709cbad594f233deb07f9f351

- Dir API: https://api.github.com/repos/wongddd/muonline/contents/Source%20Server/GameServer/GameServer?ref=580472e0d5723f9709cbad594f233deb07f9f351

- .cpp scanned: 251

- files with hits: 5


---

## HITS (primeiro caller por arquivo)

### Source Server/GameServer/GameServer/BotAlchemist.cpp
- url: https://raw.githubusercontent.com/wongddd/muonline/580472e0d5723f9709cbad594f233deb07f9f351/Source%20Server/GameServer/GameServer/BotAlchemist.cpp
- bytes: 24592
- sha256: cf88ebd3f54f89621f5a00b081661fee51bc02df90c517f57f49903a51f35990

#### GCItemListSend( @ line 742
```
   682: 								gObj[aIndex].Trade[fitem].m_NewOption |= 2;
   683: 							gObj[aIndex].Trade[sitem].m_NewOption &= ~2;
   684: 						}
   685: 					}
   686: 					if(this->AllowExc(BotNum,gObj[aIndex].Trade[fitem].m_NewOption) == true)
   687: 					{
   688: 						if(fMoney == 0 && sMoney != 0)
   689: 						{
   690: 							if(!failed)
   691: 							{
   692: 								gObj[aIndex].Trade[fitem].m_NewOption |= 1;
   693: 							gObj[aIndex].Trade[sitem].m_NewOption &= ~1;
   694: 							}
   695: 						}
   696: 					}
   697: 				}
   698: 			}
   699: 		}
   700: 	}
   701: 	int a = gObjInventoryInsertItem(aIndex,gObj[aIndex].Trade[fitem]);
   702: 	int b = gObjInventoryInsertItem(aIndex,gObj[aIndex].Trade[sitem]);
   703: 
   704: 	if(a == 255 || b == 255)
   705: 	{
   706: 		gNotice.NewMessageDevTeam(gObj[aIndex].Index,"%s Insuficent space in inventory!: %s",this->bot[BotNum].Name,gObj[aIndex].Name);
   707: 		LogAdd(LOG_RED,"[BotAlchemist] Account: %s - Name: %s Doesnt have space on inventory",gObj[aIndex].Account,gObj[aIndex].Name);
   708: 		goto Cancel;
   709: 	}
   710: 	if(failed)
   711: 	{
   712: 		gNotice.NewMessageDevTeam(gObj[aIndex].Index,"%s Transmutation failed!: %s",this->bot[BotNum].Name,gObj[aIndex].Name);
   713: 		LogAdd(LOG_RED,"[BotAlchemist] - [Bot: %d] Name: %s Alchemy Failed",this->bot[BotNum].Name,gObj[aIndex].Name);
   714: 	}
   715: 	else
   716: 	{
   717: 		gNotice.NewMessageDevTeam(gObj[aIndex].Index,"%s Transmutation success!: %s",this->bot[BotNum].Name,gObj[aIndex].Name);
   718: 		LogAdd(LOG_BOT,"[BotAlchemist] - [Bot: %s] Name: %s Alchemy Success",this->bot[BotNum].Name,gObj[aIndex].Name);
   719: 	}
   720: 
   721: 	gObj[aIndex].Money -= this->bot[BotNum].Zen;
   722: 
   723: 	if(this->bot[BotNum].PCPoints > 0)
   724: 	{
   725: 		gObj[aIndex].PCPoint -= this->bot[BotNum].PCPoints;
   726: 		gNotice.NewNoticeSend(gObj[aIndex].Index,0,0,0,0,0,"%s total PCPoint: %d",this->bot[BotNum].Name,gObj[aIndex].PCPoint);
   727: 	}
   728: 
   729: 	gObjInventoryCommit(aIndex);
   730: 	gObjectManager.CharacterMakePreviewCharSet(aIndex);
   731: 	GDCharacterInfoSaveSend(aIndex);
   732: 
   733: 	gItemManager.GCItemEquipmentSend(aIndex);
   734: 
   735: 	lpObj->TargetNumber = -1;
   736: 	lpObj->Interface.use = 0;
   737: 	lpObj->Interface.state = 0;
   738: 	lpObj->TradeOk = 0;
   739: 	lpObj->TradeMoney = 0;
   740: 	GCMoneySend(aIndex,gObj[aIndex].Money);
   741: 	gTrade.GCTradeResultSend(aIndex,1);
   742: 	gItemManager.GCItemListSend(aIndex);
   743: 
   744: 	for(int n = 0; n < TRADE_SIZE; n++)
   745: 	{
   746: 		gObj[aIndex].Trade[n].Clear();
   747: 	}
   748: 	gObjNotifyUpdateUnionV1(&gObj[aIndex]);
   749: 	gObjNotifyUpdateUnionV2(&gObj[aIndex]);
   750: 	return 1;
   751: Cancel:
   752: 	gObjTradeCancel(aIndex);
   753: 	gTrade.GCTradeResultSend(aIndex,3);
   754: 
   755: 	return 0;
   756: }
   757: 
   758: void ObjBotAlchemist::TradeOk(int aIndex)
   759: {
   760: 	int MixNum=-1;
   761: 	int MixItem=-1;
   762: 
   763: 	int number = this->GetBotIndex(gObj[aIndex].TargetNumber);
   764: 
   765: 	if(number == -1)
   766: 	{
   767: 		gObj[aIndex].Interface.state = 0;
   768: 		gObj[aIndex].Interface.type = 0;
   769: 		gObj[aIndex].TargetShopNumber = OBJECT_BOTS;
   770: 		gObj[aIndex].Transaction = 0;
   771: 		gObj[aIndex].TradeOkTime = GetTickCount();
   772: 		gObj[aIndex].TradeMoney = 1;
   773: 		gObj[aIndex].TradeOk = 1;
   774: 
   775: 		return;
   776: 	}
   777: 	this->Alchemy(aIndex,number);
   778: }
   779: 
   780: BOOL ObjBotAlchemist::TradeOpen(int index, int nindex)
   781: {
   782: 	if(gObjIsConnectedGP(index) == 0)
   783: 	{
   784: 		return 0;
   785: 	}
   786: 
   787: 	if(gObjIsConnectedGP(nindex) == 0)
   788: 	{
   789: 
   790: 		return 0;
   791: 	}
   792: 
   793: 	int number = this->GetBotIndex(nindex);
   794: 
   795: 	if(number == -1)
   796: 		return 0;
   797: 
   798: 	LPOBJ lpObj = &gObj[index];
   799: 	LPOBJ lpBot = &gObj[nindex];
   800: 
   801: 	if(this->bot[number].OnlyVip != 0 && lpObj->AccountLevel == 0)
   802: 	{
   803: 		gNotice.NewMessageDevTeam(lpObj->Index,"%s Im Only work for Account Vips!",gObj[nindex].Name);
   804: 		LogAdd(LOG_RED,"[%s] AccountLevel Free ( Account: %s Char: %s )",this->bot[number].Name,lpObj->Account,lpObj->Name);
   805: 		return 1;
   806: 	}
   807: 
   808: 	if(this->bot[number].PCPoints > gObj[index].PCPoint)
   809: 	{
   810: 		gNotice.NewMessageDevTeam(gObj[index].Index,"%s Insuficent PCPoint : %d",this->bot[number].Name,this->bot[number].PCPoints);
   811: 		LogAdd(LOG_RED,"[%s] Account: %s - Insuficent PCPoint : %d",this->bot[number].Name,gObj[index].Account,this->bot[number].PCPoints);
   812: 		return 1;
   813: 	}
   814: 	if(this->bot[number].ActiveGensFamily == 1)
   815: 	{
   816: 		if(this->bot[number].GensFamily != GENS_FAMILY_VARNERT && gObj[index].GensFamily == GENS_FAMILY_NONE )
   817: 		{
   818: 			gNotice.NewMessageDevTeam(gObj[index].Index,"%s You Need be GensFamily Vanert",this->bot[number].Name);
   819: 			return 1;
   820: 		}
   821: 		if (this->bot[number].GensFamily != GENS_FAMILY_DUPRIAN && gObj[index].GensFamily == GENS_FAMILY_NONE)
   822: 		{
   823: 			gNotice.NewMessageDevTeam(gObj[index].Index,"%s You Need be GensFamily Duprian",this->bot[number].Name);
   824: 			return 1;
   825: 		}
   826: 		if (this->bot[number].GensFamily == GENS_FAMILY_VARNERT && gObj[index].GensFamily == GENS_FAMILY_DUPRIAN)
   827: 		{
   828: 			gNotice.NewMessageDevTeam(gObj[index].Index,"%s You Need be GensFamily Duprian",this->bot[number].Name);
   829: 			return 1;
   830: 		}
   831: 		if (this->bot[number].GensFamily == GENS_FAMILY_DUPRIAN && gObj[index].GensFamily == GENS_FAMILY_VARNERT)
   832: 		{
   833: 			gNotice.NewMessageDevTeam(gObj[index].Index,"%s You Need be GensFamily Vanert",this->bot[number].Name);
   834: 			return 1;
   835: 		}
   836: 		if(this->bot[number].ContributionGens > gObj[index].GensContribution)
   837: 		{
   838: 			gNotice.NewMessageDevTeam(gObj[index].Index,"%s Insuficent GensContribution : %d",this->bot[number].Name,this->bot[number].ContributionGens);
   839: 			LogAdd(LOG_RED,"[%s] Account: %s - Insuficent GensContribution : %d",this->bot[number].Name,gObj[index].Account,this->bot[number].ContributionGens);
   840: 			return 1;
   841: 		}
   842: 	}
   843: 
   844: 	if(this->Enabled == TRUE)
   845: 	{
   846: 		if ( lpObj->Interface.use > 0 )
   847: 		{
   848: 			return 0;
   849: 		}else
   850: 		{
   851: 			for(int n = 0; n < TRADE_SIZE; n++)
   852: 			{
   853: 				lpObj->Trade[n].Clear();
   854: 			}
   855: 			memset(lpObj->TradeMap, (BYTE)-1, TRADE_SIZE );
   856: 
   857: 			gObjInventoryTransaction(lpObj->Index);
   858: 			gTrade.GCTradeResponseSendBOT(true, lpObj->Index, lpBot->Name, 400, 0);
   859: 			gTrade.GCTradeOkButtonSend(lpObj->Index, 1);
   860: 			lpObj->Interface.state = 1;
   861: 			lpObj->Interface.use = 1;
   862: 			lpObj->Interface.type = 1;
   863: 			lpObj->TradeMoney = 0;
   864: 			lpObj->TargetNumber = lpBot->Index;
   865: 			lpObj->Transaction = 1;
   866: 
   867: 			gNotice.NewMessageDevTeam(lpObj->Index,"%s I'm Ready: %s",gObj[nindex].Name,gObj[index].Name);
   868: 			LogAdd(LOG_BOT,"[Alchemist] [BotName: %s] CharName: %s OPEN",gObj[nindex].Name,gObj[index].Name);
   869: 
   870: 			if(this->bot[number].Zen > 0)
   871: 			{
   872: 				gNotice.NewMessageDevTeam(lpObj->Index,"%s I Need: %d Zen!",gObj[nindex].Name,this->bot[number].Zen);
   873: 			}
   874: 
   875: 			if(this->bot[number].PCPoints > 0)
   876: 			{
   877: 				gNotice.NewMessageDevTeam(lpObj->Index,"%s I Need: %d PCPoint!",gObj[nindex].Name,this->bot[number].PCPoints);
   878: 			}
   879: 
   880: 		}
   881: 	}
   882: 	return 1;
   883: }
   884: 
   885: void ObjBotAlchemist::TradeCancel(int aIndex)
   886: {
   887: 	gObjTradeCancel(aIndex);
   888: 	gTrade.GCTradeResultSend(aIndex,3);
   889: 
   890: }
   891: 
   892: #endif
```

#### GCItemEquipmentSend( @ line 733
```
   673: 								gObj[aIndex].Trade[fitem].m_NewOption |= 4;
   674: 							gObj[aIndex].Trade[sitem].m_NewOption &= ~4;
   675: 						}
   676: 					}
   677: 					if(this->AllowExc(BotNum,gObj[aIndex].Trade[fitem].m_NewOption) == true)
   678: 					{
   679: 						if(fDef == 0 && sDef != 0)
   680: 						{
   681: 							if(!failed)
   682: 								gObj[aIndex].Trade[fitem].m_NewOption |= 2;
   683: 							gObj[aIndex].Trade[sitem].m_NewOption &= ~2;
   684: 						}
   685: 					}
   686: 					if(this->AllowExc(BotNum,gObj[aIndex].Trade[fitem].m_NewOption) == true)
   687: 					{
   688: 						if(fMoney == 0 && sMoney != 0)
   689: 						{
   690: 							if(!failed)
   691: 							{
   692: 								gObj[aIndex].Trade[fitem].m_NewOption |= 1;
   693: 							gObj[aIndex].Trade[sitem].m_NewOption &= ~1;
   694: 							}
   695: 						}
   696: 					}
   697: 				}
   698: 			}
   699: 		}
   700: 	}
   701: 	int a = gObjInventoryInsertItem(aIndex,gObj[aIndex].Trade[fitem]);
   702: 	int b = gObjInventoryInsertItem(aIndex,gObj[aIndex].Trade[sitem]);
   703: 
   704: 	if(a == 255 || b == 255)
   705: 	{
   706: 		gNotice.NewMessageDevTeam(gObj[aIndex].Index,"%s Insuficent space in inventory!: %s",this->bot[BotNum].Name,gObj[aIndex].Name);
   707: 		LogAdd(LOG_RED,"[BotAlchemist] Account: %s - Name: %s Doesnt have space on inventory",gObj[aIndex].Account,gObj[aIndex].Name);
   708: 		goto Cancel;
   709: 	}
   710: 	if(failed)
   711: 	{
   712: 		gNotice.NewMessageDevTeam(gObj[aIndex].Index,"%s Transmutation failed!: %s",this->bot[BotNum].Name,gObj[aIndex].Name);
   713: 		LogAdd(LOG_RED,"[BotAlchemist] - [Bot: %d] Name: %s Alchemy Failed",this->bot[BotNum].Name,gObj[aIndex].Name);
   714: 	}
   715: 	else
   716: 	{
   717: 		gNotice.NewMessageDevTeam(gObj[aIndex].Index,"%s Transmutation success!: %s",this->bot[BotNum].Name,gObj[aIndex].Name);
   718: 		LogAdd(LOG_BOT,"[BotAlchemist] - [Bot: %s] Name: %s Alchemy Success",this->bot[BotNum].Name,gObj[aIndex].Name);
   719: 	}
   720: 
   721: 	gObj[aIndex].Money -= this->bot[BotNum].Zen;
   722: 
   723: 	if(this->bot[BotNum].PCPoints > 0)
   724: 	{
   725: 		gObj[aIndex].PCPoint -= this->bot[BotNum].PCPoints;
   726: 		gNotice.NewNoticeSend(gObj[aIndex].Index,0,0,0,0,0,"%s total PCPoint: %d",this->bot[BotNum].Name,gObj[aIndex].PCPoint);
   727: 	}
   728: 
   729: 	gObjInventoryCommit(aIndex);
   730: 	gObjectManager.CharacterMakePreviewCharSet(aIndex);
   731: 	GDCharacterInfoSaveSend(aIndex);
   732: 
   733: 	gItemManager.GCItemEquipmentSend(aIndex);
   734: 
   735: 	lpObj->TargetNumber = -1;
   736: 	lpObj->Interface.use = 0;
   737: 	lpObj->Interface.state = 0;
   738: 	lpObj->TradeOk = 0;
   739: 	lpObj->TradeMoney = 0;
   740: 	GCMoneySend(aIndex,gObj[aIndex].Money);
   741: 	gTrade.GCTradeResultSend(aIndex,1);
   742: 	gItemManager.GCItemListSend(aIndex);
   743: 
   744: 	for(int n = 0; n < TRADE_SIZE; n++)
   745: 	{
   746: 		gObj[aIndex].Trade[n].Clear();
   747: 	}
   748: 	gObjNotifyUpdateUnionV1(&gObj[aIndex]);
   749: 	gObjNotifyUpdateUnionV2(&gObj[aIndex]);
   750: 	return 1;
   751: Cancel:
   752: 	gObjTradeCancel(aIndex);
   753: 	gTrade.GCTradeResultSend(aIndex,3);
   754: 
   755: 	return 0;
   756: }
   757: 
   758: void ObjBotAlchemist::TradeOk(int aIndex)
   759: {
   760: 	int MixNum=-1;
   761: 	int MixItem=-1;
   762: 
   763: 	int number = this->GetBotIndex(gObj[aIndex].TargetNumber);
   764: 
   765: 	if(number == -1)
   766: 	{
   767: 		gObj[aIndex].Interface.state = 0;
   768: 		gObj[aIndex].Interface.type = 0;
   769: 		gObj[aIndex].TargetShopNumber = OBJECT_BOTS;
   770: 		gObj[aIndex].Transaction = 0;
   771: 		gObj[aIndex].TradeOkTime = GetTickCount();
   772: 		gObj[aIndex].TradeMoney = 1;
   773: 		gObj[aIndex].TradeOk = 1;
   774: 
   775: 		return;
   776: 	}
   777: 	this->Alchemy(aIndex,number);
   778: }
   779: 
   780: BOOL ObjBotAlchemist::TradeOpen(int index, int nindex)
   781: {
   782: 	if(gObjIsConnectedGP(index) == 0)
   783: 	{
   784: 		return 0;
   785: 	}
   786: 
   787: 	if(gObjIsConnectedGP(nindex) == 0)
   788: 	{
   789: 
   790: 		return 0;
   791: 	}
   792: 
   793: 	int number = this->GetBotIndex(nindex);
   794: 
   795: 	if(number == -1)
   796: 		return 0;
   797: 
   798: 	LPOBJ lpObj = &gObj[index];
   799: 	LPOBJ lpBot = &gObj[nindex];
   800: 
   801: 	if(this->bot[number].OnlyVip != 0 && lpObj->AccountLevel == 0)
   802: 	{
   803: 		gNotice.NewMessageDevTeam(lpObj->Index,"%s Im Only work for Account Vips!",gObj[nindex].Name);
   804: 		LogAdd(LOG_RED,"[%s] AccountLevel Free ( Account: %s Char: %s )",this->bot[number].Name,lpObj->Account,lpObj->Name);
   805: 		return 1;
   806: 	}
   807: 
   808: 	if(this->bot[number].PCPoints > gObj[index].PCPoint)
   809: 	{
   810: 		gNotice.NewMessageDevTeam(gObj[index].Index,"%s Insuficent PCPoint : %d",this->bot[number].Name,this->bot[number].PCPoints);
   811: 		LogAdd(LOG_RED,"[%s] Account: %s - Insuficent PCPoint : %d",this->bot[number].Name,gObj[index].Account,this->bot[number].PCPoints);
   812: 		return 1;
   813: 	}
   814: 	if(this->bot[number].ActiveGensFamily == 1)
   815: 	{
   816: 		if(this->bot[number].GensFamily != GENS_FAMILY_VARNERT && gObj[index].GensFamily == GENS_FAMILY_NONE )
   817: 		{
   818: 			gNotice.NewMessageDevTeam(gObj[index].Index,"%s You Need be GensFamily Vanert",this->bot[number].Name);
   819: 			return 1;
   820: 		}
   821: 		if (this->bot[number].GensFamily != GENS_FAMILY_DUPRIAN && gObj[index].GensFamily == GENS_FAMILY_NONE)
   822: 		{
   823: 			gNotice.NewMessageDevTeam(gObj[index].Index,"%s You Need be GensFamily Duprian",this->bot[number].Name);
   824: 			return 1;
   825: 		}
   826: 		if (this->bot[number].GensFamily == GENS_FAMILY_VARNERT && gObj[index].GensFamily == GENS_FAMILY_DUPRIAN)
   827: 		{
   828: 			gNotice.NewMessageDevTeam(gObj[index].Index,"%s You Need be GensFamily Duprian",this->bot[number].Name);
   829: 			return 1;
   830: 		}
   831: 		if (this->bot[number].GensFamily == GENS_FAMILY_DUPRIAN && gObj[index].GensFamily == GENS_FAMILY_VARNERT)
   832: 		{
   833: 			gNotice.NewMessageDevTeam(gObj[index].Index,"%s You Need be GensFamily Vanert",this->bot[number].Name);
   834: 			return 1;
   835: 		}
   836: 		if(this->bot[number].ContributionGens > gObj[index].GensContribution)
   837: 		{
   838: 			gNotice.NewMessageDevTeam(gObj[index].Index,"%s Insuficent GensContribution : %d",this->bot[number].Name,this->bot[number].ContributionGens);
   839: 			LogAdd(LOG_RED,"[%s] Account: %s - Insuficent GensContribution : %d",this->bot[number].Name,gObj[index].Account,this->bot[number].ContributionGens);
   840: 			return 1;
   841: 		}
   842: 	}
   843: 
   844: 	if(this->Enabled == TRUE)
   845: 	{
   846: 		if ( lpObj->Interface.use > 0 )
   847: 		{
   848: 			return 0;
   849: 		}else
   850: 		{
   851: 			for(int n = 0; n < TRADE_SIZE; n++)
   852: 			{
   853: 				lpObj->Trade[n].Clear();
   854: 			}
   855: 			memset(lpObj->TradeMap, (BYTE)-1, TRADE_SIZE );
   856: 
   857: 			gObjInventoryTransaction(lpObj->Index);
   858: 			gTrade.GCTradeResponseSendBOT(true, lpObj->Index, lpBot->Name, 400, 0);
   859: 			gTrade.GCTradeOkButtonSend(lpObj->Index, 1);
   860: 			lpObj->Interface.state = 1;
   861: 			lpObj->Interface.use = 1;
   862: 			lpObj->Interface.type = 1;
   863: 			lpObj->TradeMoney = 0;
   864: 			lpObj->TargetNumber = lpBot->Index;
   865: 			lpObj->Transaction = 1;
   866: 
   867: 			gNotice.NewMessageDevTeam(lpObj->Index,"%s I'm Ready: %s",gObj[nindex].Name,gObj[index].Name);
   868: 			LogAdd(LOG_BOT,"[Alchemist] [BotName: %s] CharName: %s OPEN",gObj[nindex].Name,gObj[index].Name);
   869: 
   870: 			if(this->bot[number].Zen > 0)
   871: 			{
   872: 				gNotice.NewMessageDevTeam(lpObj->Index,"%s I Need: %d Zen!",gObj[nindex].Name,this->bot[number].Zen);
   873: 			}
   874: 
   875: 			if(this->bot[number].PCPoints > 0)
   876: 			{
   877: 				gNotice.NewMessageDevTeam(lpObj->Index,"%s I Need: %d PCPoint!",gObj[nindex].Name,this->bot[number].PCPoints);
   878: 			}
   879: 
   880: 		}
   881: 	}
   882: 	return 1;
   883: }
   884: 
   885: void ObjBotAlchemist::TradeCancel(int aIndex)
   886: {
   887: 	gObjTradeCancel(aIndex);
   888: 	gTrade.GCTradeResultSend(aIndex,3);
   889: 
   890: }
   891: 
   892: #endif
```


---

### Source Server/GameServer/GameServer/DSProtocol.cpp
- url: https://raw.githubusercontent.com/wongddd/muonline/580472e0d5723f9709cbad594f233deb07f9f351/Source%20Server/GameServer/GameServer/DSProtocol.cpp
- bytes: 108622
- sha256: f1b8aed24bb6c020a14f86a779da300f0faf2bc687e394cb3e15b171162793fc

#### GCItemListSend( @ line 1272
```
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
```


---

### Source Server/GameServer/GameServer/ItemManager.cpp
- url: https://raw.githubusercontent.com/wongddd/muonline/580472e0d5723f9709cbad594f233deb07f9f351/Source%20Server/GameServer/GameServer/ItemManager.cpp
- bytes: 105683
- sha256: 2ebe79ada6400e0a1d06c92ca76a2513260ea36590258f7ac0ccafdca2656030

#### GCItemListSend( @ line 4640
```
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

#### GCItemEquipmentSend( @ line 4682
```
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


---

### Source Server/GameServer/GameServer/PG_Custom.cpp
- url: https://raw.githubusercontent.com/wongddd/muonline/580472e0d5723f9709cbad594f233deb07f9f351/Source%20Server/GameServer/GameServer/PG_Custom.cpp
- bytes: 19165
- sha256: 19e1cdb2f46a2de5e54447b60e3761412eb4aebd965ba0834362f52fdfc4e3e0

#### GCItemListSend( @ line 303
```
   243: 
   244: struct PMSG_EQUIPMENTLIST
   245: {
   246: 	PBMSG_HEAD h;	// C1:F3:13
   247: 	BYTE subcode;	// 3
   248: 	BYTE NumberH;	// 4
   249: 	BYTE NumberL;	// 5
   250: 	BYTE Equipment[18];	// 6 //CHAR_SET_SIZE
   251: 
   252: };
   253: 
   254: void GCEquipmentSend(int aIndex)
   255: {
   256: 	PMSG_EQUIPMENTLIST pMsg;
   257: 
   258: 	PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x13, sizeof(pMsg));
   259: 	pMsg.NumberH = SET_NUMBERHB(aIndex);
   260: 	pMsg.NumberL = SET_NUMBERLB(aIndex);
   261: 	gObjectManager.CharacterMakePreviewCharSet(aIndex);
   262: 	LPOBJ lpObj = &gObj[aIndex];
   263: 	memcpy(pMsg.Equipment, lpObj->CharSet, sizeof(lpObj->CharSet));
   264: 
   265: 	MsgSendV2(lpObj, (LPBYTE)&pMsg, pMsg.h.size);
   266: }
   267: void gObjTradeCancel(int aIndex)
   268: {
   269: 	if(OBJECT_RANGE(aIndex) == 0)
   270: 	{
   271: 		//if(ReadConfig.TradeLog == TRUE)
   272: 			LogAdd(LOG_BLUE,"Error : index[%d] error %s %d",aIndex,__FILE__,__LINE__);
   273: 		//LogAdd("Error : index error %s %d",__FILE__,__LINE__);
   274: 		return;
   275: 	}
   276: 	if(gObj[aIndex].Type != OBJECT_USER || gObj[aIndex].Connected != OBJECT_ONLINE)
   277: 	{
   278: 		//if(ReadConfig.TradeLog == TRUE)
   279: 		//	TRADE_LOG.Output(lMsg.Get(535),gObj[aIndex].AccountID,gObj[aIndex].Name);
   280: 		//LogAdd(lMsg.Get(535),gObj[aIndex].AccountID,gObj[aIndex].Name);
   281: 		return;
   282: 	}
   283: 	if(gObj[aIndex].Interface.use != 1)
   284: 	{
   285: 		//if(ReadConfig.TradeLog == TRUE)
   286: 		//	TRADE_LOG.Output(lMsg.Get(536),gObj[aIndex].AccountID,gObj[aIndex].Name);
   287: 		//LogAdd(lMsg.Get(536),gObj[aIndex].AccountID,gObj[aIndex].Name);
   288: 		return;
   289: 	}
   290: 	if(gObj[aIndex].Interface.type != 1)
   291: 	{
   292: 		//if(ReadConfig.TradeLog == TRUE)
   293: 		//	TRADE_LOG.Output(lMsg.Get(537),gObj[aIndex].AccountID,gObj[aIndex].Name);
   294: 		//LogAdd(lMsg.Get(537),gObj[aIndex].AccountID,gObj[aIndex].Name);
   295: 		return;
   296: 	}
   297: 
   298: 	gObjInventoryRollback(aIndex);	//OK
   299: 	gObj[aIndex].TargetNumber = -1;	//OK
   300: 	gObj[aIndex].Interface.use = 0;	//OK
   301: 	gObjCharTradeClear(&gObj[aIndex]);	//OK
   302: 	GCMoneySend(aIndex,gObj[aIndex].Money);	//NO VISTO
   303: 	gItemManager.GCItemListSend(aIndex);	//OK
   304: 	GCEquipmentSend(aIndex);	//OK
   305: 	//GCMagicListMultiSend(&gObj[aIndex],0);
   306: 	//gObjUseSkill.SkillChangeUse(aIndex);
   307: 
   308: 	if(gObj[aIndex].GuildNumber > 0)
   309: 	{
   310: 	//	GCGuildViewportNowPaint(aIndex,gObj[aIndex].GuildName,0,0);
   311: 		gObjNotifyUpdateUnionV1(&gObj[aIndex]);
   312: 		gObjNotifyUpdateUnionV2(&gObj[aIndex]);
   313: 	}
   314: }
   315: 
   316: void gObjCharTradeClear(LPOBJ lpObj)
   317: {
   318: 	if ( lpObj->Type != OBJECT_USER && lpObj->Type != OBJECT_USER )
   319: 	{
   320: 		return;
   321: 	}
   322: 
   323: 	memset(lpObj->TradeMap, 0xFF , 32); //TRADE_BOX_SIZE
   324: 
   325: 	for ( int i=0 ; i< 32 ; i++)
   326: 	{
   327: 		lpObj->Trade[i].Clear();
   328: 	}
   329: 
   330: 	lpObj->TradeMoney = 0;
   331: 	lpObj->TradeOk = false;
   332: }
   333: 
   334: int getNumberOfExcOptions(int checksum)
   335: {
   336: 	int optionscount=0;
   337: 	int ExcOrgArr[6];
   338: 
   339: 	ExcOrgArr[0]=1;
   340: 	ExcOrgArr[1]=2;
   341: 	ExcOrgArr[2]=4;
   342: 	ExcOrgArr[3]=8;
   343: 	ExcOrgArr[4]=16;
   344: 	ExcOrgArr[5]=32;
   345: 
   346: 	if (checksum>0)
   347: 	{
   348: 		for (int i=0;i<6;i++)
   349: 		{
   350: 			int and_val=checksum & ExcOrgArr[i];
   351: 			if (and_val != 0)
   352: 				optionscount+=1;
   353: 		}
   354: 	}
   355: 
   356: 	return optionscount;
   357: }
   358: 
   359: DWORD CItem::GetNumber()
   360: {
   361: 	__try
   362: 	{
   363: 		if (this->m_Index < 0)
   364: 		{
   365: 			return 0;
   366: 		}
   367: 		if (this->m_IsValidItem == false)
   368: 		{
   369: 			return 0;
   370: 		}
   371: 
   372: 		return this->m_Number;
   373: 	}
   374: 	__except( EXCEPTION_ACCESS_VIOLATION == GetExceptionCode() )
   375: 	{
   376: 		return 0;
   377: 	}
   378: }
   379: 
   380: BOOL CItem::GetSize(int & w, int & h)
   381: {
   382: 	w=ItemAttribute[this->m_Index].Width  ;
   383: 	h=ItemAttribute[this->m_Index].Height  ;
   384: 
   385: 	return 1;
   386: }
   387: void gObjInventoryItemBoxSet(int aIndex, int itempos, int xl, int yl, BYTE set_byte)
   388: {
   389: 	int S6E2 =1;
   390: 	int InventoryRows = 12;
   391: 	if(S6E2 == 1)
   392: 		InventoryRows = 28;
   393: 
   394: 	int itemposx = (itempos - INVENTORY_WEAR_SIZE)%8;
   395: 	int itemposy = (itempos - INVENTORY_WEAR_SIZE)/8;
   396: 
   397: 	int xx,yy;
   398: 
   399: 	for(int y = 0; y < yl; y ++)
   400: 	{
   401: 		yy = itemposy + y;
   402: 
   403: 		for(int x = 0; x < xl; x++)
   404: 		{
   405: 			xx = itemposx + x;
   406: 
   407: 			if((ExtentCheck(xx,yy,8,InventoryRows)==1)
   408: #if (PACK_EDITION>=3)
   409: 				|| (gObj[aIndex].IsBot >= 1)
   410: #endif
   411: 				)
   412: 			{
   413: 				*(BYTE*)(gObj[aIndex].InventoryMap + (itemposy + y)*8+(itemposx + x)) = set_byte;
   414: 			}
   415: 			else
   416: 			{
   417: 			//	LogAdd("error : %s %d",__FILE__,__LINE__);
   418: 				return;
   419: 			}
   420: 		}
   421: 	}
   422: }
   423: 
   424: void gObjInventoryItemSet(int aIndex, int itempos, BYTE set_byte)
   425: {
   426: 	int width;
   427: 	int height;
   428: 	if(itempos < INVENTORY_WEAR_SIZE)
   429: 	{
   430: 		return;
   431: 	}
   432: 	else if(itempos > (INVENTORY_MAIN_SIZE-1))
   433: 	{
   434: 		return;
   435: 	}
   436: 
   437: 	if(gObj[aIndex].Inventory[itempos].GetSize((int&)width,(int &)height)==0)
   438: 	{
   439: //		LogAdd(lMsg.Get(527),__FILE__,__LINE__);
   440: 		return;
   441: 	}
   442: 
   443: 	gObjInventoryItemBoxSet(aIndex,itempos,width,height,set_byte);
   444: }
   445: 
   446: BOOL gItemSerialCheck=1;
   447: BOOL gItemZeroSerialCheck;
   448: 
   449: int gObjCheckSerial0ItemList(class CItem* lpItem)
   450: {
   451: 	if(gItemSerialCheck == 0)
   452: 	{
   453: 		return false;
   454: 	}
   455: 	if(gItemZeroSerialCheck == 0)
   456: 	{
   457: 		return false;
   458: 	}
   459: 
   460: 	if(lpItem->m_Index == GET_ITEM(19,13)
   461: 		|| lpItem->m_Index == GET_ITEM(19,14)
   462: 		|| lpItem->m_Index == GET_ITEM(19,16)
   463: 		|| lpItem->m_Index == GET_ITEM(19,22)
   464: 		|| lpItem->m_Index == GET_ITEM(19,15)
   465: 		|| lpItem->m_Index == GET_ITEM(19,30)
   466: 		|| lpItem->m_Index == GET_ITEM(19,31)
   467: 		|| lpItem->m_Index == GET_ITEM(19,31))
   468: 	{
   469: 		if(lpItem->GetNumber() == 0)
   470: 		{
   471: 			return true;
   472: 		}
   473: 	}
   474: 	return false;
   475: }
   476: 
   477: BOOL IS_EXTENDED_INV(int aIndex, BYTE INVNUM)
   478: {
   479: 	int S6E2 = 1;
   480: 	if(OBJECT_RANGE(aIndex) == false)
   481: 		return FALSE;
   482: 
   483: 	if (aIndex < OBJECT_START_USER || aIndex > MAX_OBJECT)
   484: 		return FALSE;
   485: 
   486: 	if(S6E2 == 1)
   487: 	{
   488: 		if(gObj[aIndex].ExInventory >= 1 && INVNUM == 1)
   489: 			return TRUE;
   490: 		else if(gObj[aIndex].ExInventory == 2 && INVNUM == 2)
   491: 			return TRUE;
   492: 		else if(gObj[aIndex].ExInventory == 3 && INVNUM == 3)
   493: 			return TRUE;
   494: 		else if(gObj[aIndex].ExInventory == 4 && INVNUM == 4)
   495: 			return TRUE;
   496: 	}
   497: 	return FALSE;
   498: }
   499: 
   500: BYTE gObjInventoryRectCheck(int aIndex, int sx, int sy, int width, int height)
   501: {
   502: 	int x,y;
   503: 	int blank = 0;
```


---

### Source Server/GameServer/GameServer/Trade.cpp
- url: https://raw.githubusercontent.com/wongddd/muonline/580472e0d5723f9709cbad594f233deb07f9f351/Source%20Server/GameServer/GameServer/Trade.cpp
- bytes: 19485
- sha256: d1dc3944e4b592c61767b026f809f3845c41513ba140117fa8b6f2013fcb9894

#### GCItemListSend( @ line 62
```
     2: //
     3: //////////////////////////////////////////////////////////////////////
     4: 
     5: #include "stdafx.h"
     6: #include "Trade.h"
     7: #include "Duel.h"
     8: #include "DSProtocol.h"
     9: #include "EventHideAndSeek.h"
    10: #include "EventPvP.h"
    11: #include "EventRussianRoulette.h"
    12: #include "EventInventory.h"
    13: #include "ItemValueTrade.h"
    14: #include "Log.h"
    15: #include "Map.h"
    16: #include "Message.h"
    17: #include "Notice.h"
    18: #include "ObjectManager.h"
    19: #include "PentagramSystem.h"
    20: #include "ServerInfo.h"
    21: #include "User.h"
    22: #include "Util.h"
    23: //MC
    24: #include "BotBuffer.h"
    25: 
    26: CTrade gTrade;
    27: //////////////////////////////////////////////////////////////////////
    28: // Construction/Destruction
    29: //////////////////////////////////////////////////////////////////////
    30: 
    31: CTrade::CTrade() // OK
    32: {
    33: 
    34: }
    35: 
    36: CTrade::~CTrade() // OK
    37: {
    38: 
    39: }
    40: 
    41: void CTrade::ClearTrade(LPOBJ lpObj) // OK
    42: {
    43: 	for(int n=0;n < TRADE_SIZE;n++)
    44: 	{
    45: 		lpObj->Trade[n].Clear();
    46: 	}
    47: 
    48: 	memset(lpObj->TradeMap,0xFF,TRADE_SIZE);
    49: }
    50: 
    51: void CTrade::ResetTrade(int aIndex) // OK
    52: {
    53: 	LPOBJ lpObj = &gObj[aIndex];
    54: 
    55: 	if(lpObj->Interface.use == 0 || lpObj->Interface.type != INTERFACE_TRADE || lpObj->Interface.state == 0)
    56: 	{
    57: 		return;
    58: 	}
    59: 
    60: 	gObjInventoryRollback(aIndex);
    61: 
    62: 	gItemManager.GCItemListSend(aIndex);
    63: 
    64: 	#if(GAMESERVER_UPDATE>=802)
    65: 
    66: 	gEventInventory.GCEventItemListSend(aIndex);
    67: 
    68: 	#endif
    69: 
    70: 	GCMoneySend(aIndex,lpObj->Money);
    71: 
    72: 	gObjectManager.CharacterMakePreviewCharSet(aIndex);
    73: 
    74: 	gItemManager.GCItemEquipmentSend(aIndex);
    75: 
    76: 	this->ClearTrade(lpObj);
    77: 
    78: 	lpObj->Interface.use = 0;
    79: 	lpObj->Interface.type = INTERFACE_NONE;
    80: 	lpObj->Interface.state = 0;
    81: 	lpObj->TargetNumber = -1;
    82: 	lpObj->TradeOk = 0;
    83: 	lpObj->TradeOkTime = 0;
    84: 	lpObj->TradeMoney = 0;
    85: }
    86: 
    87: bool CTrade::ExchangeTradeItem(LPOBJ lpObj,LPOBJ lpTarget) // OK
    88: {
    89: 	for(int n=0;n < TRADE_SIZE;n++)
    90: 	{
    91: 		if(lpObj->Trade[n].IsItem() != 0)
    92: 		{
    93: 			if(lpObj->Trade[n].IsEventItem() == 0)
    94: 			{
    95: 				if((lpObj->Trade[n].m_Slot=gItemManager.InventoryInsertItem(lpTarget->Index,lpObj->Trade[n])) == 0xFF)
    96: 				{
    97: 					return 0;
    98: 				}
    99: 			}
   100: 			else
   101: 			{
   102: 				if((lpObj->Trade[n].m_Slot=gEventInventory.EventInventoryInsertItem(lpTarget->Index,lpObj->Trade[n])) == 0xFF)
   103: 				{
   104: 					return 0;
   105: 				}
   106: 			}
   107: 		}
   108: 	}
   109: 
   110: 	return 1;
   111: }
   112: 
   113: void CTrade::ExchangeTradeItemLog(LPOBJ lpObj,LPOBJ lpTarget) // OK
   114: {
   115: 	for(int n=0;n < TRADE_SIZE;n++)
   116: 	{
   117: 		if(lpObj->Trade[n].IsItem() != 0)
   118: 		{
   119: 			gLog.Output(LOG_TRADE,"[ExchangeTradeItem][%s][%s] - (Account: %s, Name: %s, Index: %04d, Level: %02d, Serial: %08X, Option1: %01d, Option2: %01d, Option3: %01d, NewOption: %03d, JewelOfHarmonyOption: %03d, ItemOptionEx: %03d, SocketOption: %03d, %03d, %03d, %03d, %03d)",lpObj->Account,lpObj->Name,lpTarget->Account,lpTarget->Name,lpObj->Trade[n].m_Index,lpObj->Trade[n].m_Level,lpObj->Trade[n].m_Serial,lpObj->Trade[n].m_Option1,lpObj->Trade[n].m_Option2,lpObj->Trade[n].m_Option3,lpObj->Trade[n].m_NewOption,lpObj->Trade[n].m_JewelOfHarmonyOption,lpObj->Trade[n].m_ItemOptionEx,lpObj->Trade[n].m_SocketOption[0],lpObj->Trade[n].m_SocketOption[1],lpObj->Trade[n].m_SocketOption[2],lpObj->Trade[n].m_SocketOption[3],lpObj->Trade[n].m_SocketOption[4]);
   120: 		}
   121: 	}
   122: }
   123: 
   124: void CTrade::CGTradeRequestRecv(PMSG_TRADE_REQUEST_RECV* lpMsg,int aIndex) // OK
   125: {
   126: 	if(gServerInfo.m_TradeSwitch == 0)
   127: 	{
   128: 		return;
   129: 	}
   130: 
   131: 	LPOBJ lpObj = &gObj[aIndex];
   132: 
   133: 	if(gObjIsConnectedGP(aIndex) == 0)
   134: 	{
   135: 		return;
   136: 	}
   137: 
   138: 	int bIndex = MAKE_NUMBERW(lpMsg->index[0],lpMsg->index[1]);
   139: 
   140: 	if(gObjIsConnectedGP(bIndex) == 0)
   141: 	{
   142: 		return;
   143: 	}
   144: 
   145: 	LPOBJ lpTarget = &gObj[bIndex];
   146: 
   147: 	if(lpObj->Interface.use != 0 || lpTarget->Interface.use != 0)
   148: 	{
   149: 		return;
   150: 	}
   151: 
   152: 	if(lpObj->TradeDuel != 0)
   153: 	{
   154: 		return;
   155: 	}
   156: 
   157: 	if(lpTarget->TradeDuel != 0)
   158: 	{
   159: 		return;
   160: 	}
   161: 
   162: 	if(CA_MAP_RANGE(lpTarget->Map) != 0 || DS_MAP_RANGE(lpTarget->Map) != 0 || BC_MAP_RANGE(lpTarget->Map) != 0 || CC_MAP_RANGE(lpTarget->Map) != 0 || IT_MAP_RANGE(lpTarget->Map) != 0 || DA_MAP_RANGE(lpTarget->Map) != 0 || DG_MAP_RANGE(lpTarget->Map) != 0 || IG_MAP_RANGE(lpTarget->Map) != 0)
   163: 	{
   164: 		this->GCTradeResponseSend(aIndex,0,lpTarget->Name,0,0);
   165: 		return;
   166: 	}
   167: 
   168: 	if(lpObj->X < (lpTarget->X-2) || lpObj->X > (lpTarget->X+2) || lpObj->Y < (lpTarget->Y-2) || lpObj->Y > (lpTarget->Y+2))
   169: 	{
   170: 		this->GCTradeResponseSend(aIndex,0,lpTarget->Name,0,0);
   171: 		return;
   172: 	}
   173: 
   174: 	if(lpObj->PShopOpen != 0 || lpTarget->PShopOpen != 0)
   175: 	{
   176: 		this->GCTradeResponseSend(aIndex,0,lpTarget->Name,0,0);
   177: 		return;
   178: 	}
   179: 
   180: 	if(gServerInfo.m_PKDisableTrade == 1 && lpObj->PKLevel >= 5)
   181: 	{
   182: 		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(861));
   183: 		return;
   184: 	}
   185: 
   186: 	if(gServerInfo.m_PKDisableTrade == 1 && lpTarget->PKLevel >= 5)
   187: 	{
   188: 		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(862));
   189: 		return;
   190: 	}
   191: 
   192: 	if(gEventHideAndSeek.EventHideAndSeekTrade(aIndex,bIndex) == 1)
   193: 	{
   194: 		return;
   195: 	}
   196: 
   197: 	if(gEventPvP.EventPvPTradeJoin(aIndex,bIndex) == 1)
   198: 	{
   199: 		return;
   200: 	}
   201: 
   202: 	if(gEventRussianRoulette.EventRussianRouletteTrade(aIndex,bIndex) == 1)
   203: 	{
   204: 		return;
   205: 	}
   206: 
   207: 	if((lpTarget->Option & 1) == 0)
   208: 	{
   209: 		this->GCTradeResponseSend(aIndex,0,lpTarget->Name,0,0);
   210: 		return;
   211: 	}
   212: 
   213: 	//MC bot
   214: 	// SCF BOTS
   215: 
   216: 	int	number = MAKE_NUMBERW(lpMsg->index[0],lpMsg->index[1]);
   217: 
   218: 	if ( OBJMAX_RANGE(number) == FALSE )
   219: 	{
   220: 		LogAdd(LOG_BLACK,"Error : %s %d (%d)", __FILE__, __LINE__, number);
   221: 		return;
   222: 	}
   223: 	if ( number == aIndex )
   224: 	{
   225: 		return;
   226: 	}
   227: 
   228: 	if(ObjBotBuff.TradeOpen(aIndex,number) == 1)
   229: 	{
   230: 		return;
   231: 	}
   232: 
   233: 	lpObj->Interface.use = 1;
   234: 	lpObj->Interface.type = INTERFACE_TRADE;
   235: 	lpObj->Interface.state = 0;
   236: 	lpObj->InterfaceTime = GetTickCount();
   237: 	lpObj->TargetNumber = bIndex;
   238: 
   239: 	lpTarget->Interface.use = 1;
   240: 	lpTarget->Interface.type = INTERFACE_TRADE;
   241: 	lpTarget->Interface.state = 0;
   242: 	lpTarget->InterfaceTime = GetTickCount();
   243: 	lpTarget->TargetNumber = aIndex;
   244: 
   245: 	this->GCTradeRequestSend(bIndex,lpObj->Name);
   246: }
   247: 
   248: void CTrade::CGTradeResponseRecv(PMSG_TRADE_RESPONSE_RECV* lpMsg,int aIndex) // OK
   249: {
   250: 	LPOBJ lpObj = &gObj[aIndex];
   251: 
   252: 	if(gObjIsConnectedGP(aIndex) == 0)
   253: 	{
   254: 		return;
   255: 	}
   256: 
   257: 	int bIndex = lpObj->TargetNumber;
   258: 
   259: 	if(gObjIsConnectedGP(bIndex) == 0)
   260: 	{
   261: 		return;
   262: 	}
```

#### GCItemEquipmentSend( @ line 74
```
    14: #include "Log.h"
    15: #include "Map.h"
    16: #include "Message.h"
    17: #include "Notice.h"
    18: #include "ObjectManager.h"
    19: #include "PentagramSystem.h"
    20: #include "ServerInfo.h"
    21: #include "User.h"
    22: #include "Util.h"
    23: //MC
    24: #include "BotBuffer.h"
    25: 
    26: CTrade gTrade;
    27: //////////////////////////////////////////////////////////////////////
    28: // Construction/Destruction
    29: //////////////////////////////////////////////////////////////////////
    30: 
    31: CTrade::CTrade() // OK
    32: {
    33: 
    34: }
    35: 
    36: CTrade::~CTrade() // OK
    37: {
    38: 
    39: }
    40: 
    41: void CTrade::ClearTrade(LPOBJ lpObj) // OK
    42: {
    43: 	for(int n=0;n < TRADE_SIZE;n++)
    44: 	{
    45: 		lpObj->Trade[n].Clear();
    46: 	}
    47: 
    48: 	memset(lpObj->TradeMap,0xFF,TRADE_SIZE);
    49: }
    50: 
    51: void CTrade::ResetTrade(int aIndex) // OK
    52: {
    53: 	LPOBJ lpObj = &gObj[aIndex];
    54: 
    55: 	if(lpObj->Interface.use == 0 || lpObj->Interface.type != INTERFACE_TRADE || lpObj->Interface.state == 0)
    56: 	{
    57: 		return;
    58: 	}
    59: 
    60: 	gObjInventoryRollback(aIndex);
    61: 
    62: 	gItemManager.GCItemListSend(aIndex);
    63: 
    64: 	#if(GAMESERVER_UPDATE>=802)
    65: 
    66: 	gEventInventory.GCEventItemListSend(aIndex);
    67: 
    68: 	#endif
    69: 
    70: 	GCMoneySend(aIndex,lpObj->Money);
    71: 
    72: 	gObjectManager.CharacterMakePreviewCharSet(aIndex);
    73: 
    74: 	gItemManager.GCItemEquipmentSend(aIndex);
    75: 
    76: 	this->ClearTrade(lpObj);
    77: 
    78: 	lpObj->Interface.use = 0;
    79: 	lpObj->Interface.type = INTERFACE_NONE;
    80: 	lpObj->Interface.state = 0;
    81: 	lpObj->TargetNumber = -1;
    82: 	lpObj->TradeOk = 0;
    83: 	lpObj->TradeOkTime = 0;
    84: 	lpObj->TradeMoney = 0;
    85: }
    86: 
    87: bool CTrade::ExchangeTradeItem(LPOBJ lpObj,LPOBJ lpTarget) // OK
    88: {
    89: 	for(int n=0;n < TRADE_SIZE;n++)
    90: 	{
    91: 		if(lpObj->Trade[n].IsItem() != 0)
    92: 		{
    93: 			if(lpObj->Trade[n].IsEventItem() == 0)
    94: 			{
    95: 				if((lpObj->Trade[n].m_Slot=gItemManager.InventoryInsertItem(lpTarget->Index,lpObj->Trade[n])) == 0xFF)
    96: 				{
    97: 					return 0;
    98: 				}
    99: 			}
   100: 			else
   101: 			{
   102: 				if((lpObj->Trade[n].m_Slot=gEventInventory.EventInventoryInsertItem(lpTarget->Index,lpObj->Trade[n])) == 0xFF)
   103: 				{
   104: 					return 0;
   105: 				}
   106: 			}
   107: 		}
   108: 	}
   109: 
   110: 	return 1;
   111: }
   112: 
   113: void CTrade::ExchangeTradeItemLog(LPOBJ lpObj,LPOBJ lpTarget) // OK
   114: {
   115: 	for(int n=0;n < TRADE_SIZE;n++)
   116: 	{
   117: 		if(lpObj->Trade[n].IsItem() != 0)
   118: 		{
   119: 			gLog.Output(LOG_TRADE,"[ExchangeTradeItem][%s][%s] - (Account: %s, Name: %s, Index: %04d, Level: %02d, Serial: %08X, Option1: %01d, Option2: %01d, Option3: %01d, NewOption: %03d, JewelOfHarmonyOption: %03d, ItemOptionEx: %03d, SocketOption: %03d, %03d, %03d, %03d, %03d)",lpObj->Account,lpObj->Name,lpTarget->Account,lpTarget->Name,lpObj->Trade[n].m_Index,lpObj->Trade[n].m_Level,lpObj->Trade[n].m_Serial,lpObj->Trade[n].m_Option1,lpObj->Trade[n].m_Option2,lpObj->Trade[n].m_Option3,lpObj->Trade[n].m_NewOption,lpObj->Trade[n].m_JewelOfHarmonyOption,lpObj->Trade[n].m_ItemOptionEx,lpObj->Trade[n].m_SocketOption[0],lpObj->Trade[n].m_SocketOption[1],lpObj->Trade[n].m_SocketOption[2],lpObj->Trade[n].m_SocketOption[3],lpObj->Trade[n].m_SocketOption[4]);
   120: 		}
   121: 	}
   122: }
   123: 
   124: void CTrade::CGTradeRequestRecv(PMSG_TRADE_REQUEST_RECV* lpMsg,int aIndex) // OK
   125: {
   126: 	if(gServerInfo.m_TradeSwitch == 0)
   127: 	{
   128: 		return;
   129: 	}
   130: 
   131: 	LPOBJ lpObj = &gObj[aIndex];
   132: 
   133: 	if(gObjIsConnectedGP(aIndex) == 0)
   134: 	{
   135: 		return;
   136: 	}
   137: 
   138: 	int bIndex = MAKE_NUMBERW(lpMsg->index[0],lpMsg->index[1]);
   139: 
   140: 	if(gObjIsConnectedGP(bIndex) == 0)
   141: 	{
   142: 		return;
   143: 	}
   144: 
   145: 	LPOBJ lpTarget = &gObj[bIndex];
   146: 
   147: 	if(lpObj->Interface.use != 0 || lpTarget->Interface.use != 0)
   148: 	{
   149: 		return;
   150: 	}
   151: 
   152: 	if(lpObj->TradeDuel != 0)
   153: 	{
   154: 		return;
   155: 	}
   156: 
   157: 	if(lpTarget->TradeDuel != 0)
   158: 	{
   159: 		return;
   160: 	}
   161: 
   162: 	if(CA_MAP_RANGE(lpTarget->Map) != 0 || DS_MAP_RANGE(lpTarget->Map) != 0 || BC_MAP_RANGE(lpTarget->Map) != 0 || CC_MAP_RANGE(lpTarget->Map) != 0 || IT_MAP_RANGE(lpTarget->Map) != 0 || DA_MAP_RANGE(lpTarget->Map) != 0 || DG_MAP_RANGE(lpTarget->Map) != 0 || IG_MAP_RANGE(lpTarget->Map) != 0)
   163: 	{
   164: 		this->GCTradeResponseSend(aIndex,0,lpTarget->Name,0,0);
   165: 		return;
   166: 	}
   167: 
   168: 	if(lpObj->X < (lpTarget->X-2) || lpObj->X > (lpTarget->X+2) || lpObj->Y < (lpTarget->Y-2) || lpObj->Y > (lpTarget->Y+2))
   169: 	{
   170: 		this->GCTradeResponseSend(aIndex,0,lpTarget->Name,0,0);
   171: 		return;
   172: 	}
   173: 
   174: 	if(lpObj->PShopOpen != 0 || lpTarget->PShopOpen != 0)
   175: 	{
   176: 		this->GCTradeResponseSend(aIndex,0,lpTarget->Name,0,0);
   177: 		return;
   178: 	}
   179: 
   180: 	if(gServerInfo.m_PKDisableTrade == 1 && lpObj->PKLevel >= 5)
   181: 	{
   182: 		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(861));
   183: 		return;
   184: 	}
   185: 
   186: 	if(gServerInfo.m_PKDisableTrade == 1 && lpTarget->PKLevel >= 5)
   187: 	{
   188: 		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(862));
   189: 		return;
   190: 	}
   191: 
   192: 	if(gEventHideAndSeek.EventHideAndSeekTrade(aIndex,bIndex) == 1)
   193: 	{
   194: 		return;
   195: 	}
   196: 
   197: 	if(gEventPvP.EventPvPTradeJoin(aIndex,bIndex) == 1)
   198: 	{
   199: 		return;
   200: 	}
   201: 
   202: 	if(gEventRussianRoulette.EventRussianRouletteTrade(aIndex,bIndex) == 1)
   203: 	{
   204: 		return;
   205: 	}
   206: 
   207: 	if((lpTarget->Option & 1) == 0)
   208: 	{
   209: 		this->GCTradeResponseSend(aIndex,0,lpTarget->Name,0,0);
   210: 		return;
   211: 	}
   212: 
   213: 	//MC bot
   214: 	// SCF BOTS
   215: 
   216: 	int	number = MAKE_NUMBERW(lpMsg->index[0],lpMsg->index[1]);
   217: 
   218: 	if ( OBJMAX_RANGE(number) == FALSE )
   219: 	{
   220: 		LogAdd(LOG_BLACK,"Error : %s %d (%d)", __FILE__, __LINE__, number);
   221: 		return;
   222: 	}
   223: 	if ( number == aIndex )
   224: 	{
   225: 		return;
   226: 	}
   227: 
   228: 	if(ObjBotBuff.TradeOpen(aIndex,number) == 1)
   229: 	{
   230: 		return;
   231: 	}
   232: 
   233: 	lpObj->Interface.use = 1;
   234: 	lpObj->Interface.type = INTERFACE_TRADE;
   235: 	lpObj->Interface.state = 0;
   236: 	lpObj->InterfaceTime = GetTickCount();
   237: 	lpObj->TargetNumber = bIndex;
   238: 
   239: 	lpTarget->Interface.use = 1;
   240: 	lpTarget->Interface.type = INTERFACE_TRADE;
   241: 	lpTarget->Interface.state = 0;
   242: 	lpTarget->InterfaceTime = GetTickCount();
   243: 	lpTarget->TargetNumber = aIndex;
   244: 
   245: 	this->GCTradeRequestSend(bIndex,lpObj->Name);
   246: }
   247: 
   248: void CTrade::CGTradeResponseRecv(PMSG_TRADE_RESPONSE_RECV* lpMsg,int aIndex) // OK
   249: {
   250: 	LPOBJ lpObj = &gObj[aIndex];
   251: 
   252: 	if(gObjIsConnectedGP(aIndex) == 0)
   253: 	{
   254: 		return;
   255: 	}
   256: 
   257: 	int bIndex = lpObj->TargetNumber;
   258: 
   259: 	if(gObjIsConnectedGP(bIndex) == 0)
   260: 	{
   261: 		return;
   262: 	}
   263: 
   264: 	LPOBJ lpTarget = &gObj[bIndex];
   265: 
   266: 	if(lpObj->Interface.use == 0 || lpObj->Interface.type != INTERFACE_TRADE || lpObj->Interface.state != 0)
   267: 	{
   268: 		return;
   269: 	}
   270: 
   271: 	if(lpTarget->Interface.use == 0 || lpTarget->Interface.type != INTERFACE_TRADE || lpTarget->Interface.state != 0)
   272: 	{
   273: 		return;
   274: 	}
```


---

## QUESTIONS

1) Em quais pontos do fluxo (login/join/respawn/warp) o GS chama GCItemListSend?

2) O GS chama GCItemEquipmentSend junto? Em que ordem?

3) Há condições (ex.: somente OBJECT_USER, somente após auth) antes do envio?
