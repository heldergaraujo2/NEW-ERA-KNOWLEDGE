# NEW-ERA 1.3-S — GS emitters do RX 0x19 (magic) — EVIDENCE

- Legacy commit: 580472e0d5723f9709cbad594f233deb07f9f351


## Files fetched (sha256)

- SkillManager.cpp sha256 83b537dddc552400a39043ed064e6394ad9e95bec585ccd073aede2c73d27b46 bytes=146528
- SkillManager.h sha256 2d706d9f4ca551a5d3f0b2d8e6a372942102f1d3599b80f769bda98b789b7246 bytes=18550
- Protocol.cpp sha256 e8bb4dbb28d1baf23222dbc72762e2ebd55b116dbb8d36e5f017dd744e510f13 bytes=135014

---

## FILE: SkillManager.cpp

### HIT: Emitter header.set(0x19) / \.header\.set\s*\(\s*0x19\s*,|header\.set\s*\(\s*0x19\s*,

- line 6413: 	pMsg.header.set(0x19,sizeof(pMsg));

#### CONTEXT (first hit)

```
  6343: 	if(lpObj->PartyNumber != lpTarget->PartyNumber)
  6344: 	{
  6345: 		return;
  6346: 	}
  6347: 
  6348: 	if(gEffectManager.CheckStunEffect(lpObj) != 0)
  6349: 	{
  6350: 		return;
  6351: 	}
  6352: 
  6353: 	if(lpObj->Map != lpTarget->Map)
  6354: 	{
  6355: 		return;
  6356: 	}
  6357: 
  6358: 	if(gObjCheckTeleportArea(lpMsg->index,lpMsg->x,lpMsg->y) == 0)
  6359: 	{
  6360: 		return;
  6361: 	}
  6362: 
  6363: 	#if(GAMESERVER_TYPE==1)
  6364: 
  6365: 	if(lpObj->Map == MAP_CASTLE_SIEGE && gCastleSiege.CheckTeleportMagicAxisY(lpObj->Y,lpMsg->x,lpMsg->y) == 0)
  6366: 	{
  6367: 		lpMsg->y = (BYTE)lpObj->Y;
  6368: 	}
  6369: 
  6370: 	#endif
  6371: 
  6372: 	CSkill* lpSkill = gSkillManager.GetSkill(lpObj,SKILL_TELEPORT_ALLY);
  6373: 
  6374: 	if(lpSkill != 0)
  6375: 	{
  6376: 		if(IT_MAP_RANGE(lpObj->Map) != 0 && gIllusionTemple.GetState(GET_IT_LEVEL(lpObj->Map)) != IT_STATE_START)
  6377: 		{
  6378: 			lpMsg->x = (BYTE)lpObj->X;
  6379: 			lpMsg->y = (BYTE)lpObj->Y;
  6380: 		}
  6381: 
  6382: 		#if(GAMESERVER_UPDATE>=501)
  6383: 
  6384: 		if(DG_MAP_RANGE(lpObj->Map) != 0 && gDoubleGoer.GetState() != DG_STATE_START)
  6385: 		{
  6386: 			lpMsg->x = (BYTE)lpObj->X;
  6387: 			lpMsg->y = (BYTE)lpObj->Y;
  6388: 		}
  6389: 
  6390: 		#endif
  6391: 
  6392: 		if(gSkillManager.CheckSkillMana(lpObj,lpSkill->m_index) == 0 || gSkillManager.CheckSkillBP(lpObj,lpSkill->m_index) == 0)
  6393: 		{
  6394: 			return;
  6395: 		}
  6396: 
  6397: 		gSkillManager.GCSkillAttackSend(lpObj,lpSkill->m_index,lpMsg->index,1);
  6398: 
  6399: 		gObjTeleportMagicUse(lpMsg->index,lpMsg->x,lpMsg->y);
  6400: 
  6401: 		lpObj->Mana -= (gSkillManager.GetSkillMana(lpSkill->m_index)*lpObj->MPConsumptionRate)/100;
  6402: 
  6403: 		lpObj->BP -= (gSkillManager.GetSkillBP(lpSkill->m_index)*lpObj->BPConsumptionRate)/100;
  6404: 
  6405: 		GCManaSend(lpObj->Index,0xFF,(int)lpObj->Mana,lpObj->BP);
  6406: 	}
  6407: }
  6408: 
  6409: void CSkillManager::GCSkillAttackSend(LPOBJ lpObj,int skill,int aIndex,BYTE type) // OK
  6410: {
  6411: 	PMSG_SKILL_ATTACK_SEND pMsg;
  6412: 
  6413: 	pMsg.header.set(0x19,sizeof(pMsg));
  6414: 
  6415: 	pMsg.skill[0] = SET_NUMBERHB(skill);
  6416: 	pMsg.skill[1] = SET_NUMBERLB(skill);
  6417: 
  6418: 	pMsg.index[0] = SET_NUMBERHB(lpObj->Index);
  6419: 	pMsg.index[1] = SET_NUMBERLB(lpObj->Index);
  6420: 
  6421: 	pMsg.target[0] = SET_NUMBERHB(aIndex) | (type*0x80);
  6422: 	pMsg.target[1] = SET_NUMBERLB(aIndex);
  6423: 
  6424: 	if(lpObj->Type == OBJECT_USER)
  6425: 	{
  6426: 		DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);
  6427: 	}
  6428: 
  6429: 	MsgSendV2(lpObj,(BYTE*)&pMsg,pMsg.header.size);
  6430: 
  6431: }
  6432: 
  6433: void CSkillManager::GCSkillCancelSend(LPOBJ lpObj,int skill) // OK
  6434: {
  6435: 	PMSG_SKILL_CANCEL_SEND pMsg;
  6436: 
  6437: 	pMsg.header.set(0x1B,sizeof(pMsg));
  6438: 
  6439: 	pMsg.skill[0] = SET_NUMBERHB(skill);
  6440: 	pMsg.skill[1] = SET_NUMBERLB(skill);
  6441: 
  6442: 	pMsg.index[0] = SET_NUMBERHB(lpObj->Index);
  6443: 	pMsg.index[1] = SET_NUMBERLB(lpObj->Index);
  6444: 
  6445: 	if(lpObj->Type == OBJECT_USER)
  6446: 	{
  6447: 		DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);
  6448: 	}
  6449: 
  6450: 	MsgSendV2(lpObj,(BYTE*)&pMsg,pMsg.header.size);
  6451: }
  6452: 
  6453: void CSkillManager::GCDurationSkillAttackSend(LPOBJ lpObj,int skill,BYTE x,BYTE y,BYTE dir) // OK
  6454: {
  6455: 	PMSG_DURATION_SKILL_ATTACK_SEND pMsg;
  6456: 
  6457: 	pMsg.header.set(0x1E,sizeof(pMsg));
  6458: 
  6459: 	#if(GAMESERVER_UPDATE>=701)
  6460: 
  6461: 	pMsg.skillH = SET_NUMBERHB(skill);
  6462: 	pMsg.skillL = SET_NUMBERLB(skill);
  6463: 
  6464: 	pMsg.indexH = SET_NUMBERHB(lpObj->Index);
  6465: 	pMsg.indexL = SET_NUMBERLB(lpObj->Index);
  6466: 
  6467: 	#else
  6468: 
  6469: 	pMsg.skill[0] = SET_NUMBERHB(skill);
  6470: 	pMsg.skill[1] = SET_NUMBERLB(skill);
  6471: 
  6472: 	pMsg.index[0] = SET_NUMBERHB(lpObj->Index);
  6473: 	pMsg.index[1] = SET_NUMBERLB(lpObj->Index);
  6474: 
  6475: 	#endif
  6476: 
  6477: 	pMsg.x = x;
  6478: 	pMsg.y = y;
  6479: 	pMsg.dir = dir;
  6480: 
  6481: 	if(lpObj->Type == OBJECT_USER)
  6482: 	{
  6483: 		DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);
  6484: 	}
  6485: 
  6486: 	MsgSendV2(lpObj,(BYTE*)&pMsg,pMsg.header.size);
  6487: }
  6488: 
  6489: void CSkillManager::GCRageFighterSkillAttackSend(LPOBJ lpObj,int skill,int aIndex,BYTE type) // OK
  6490: {
  6491: 	#if(GAMESERVER_UPDATE>=601)
  6492: 
  6493: 	PMSG_RAGE_FIGHTER_SKILL_ATTACK_SEND pMsg;
  6494: 
  6495: 	pMsg.header.set(0x4A,sizeof(pMsg));
  6496: 
  6497: 	pMsg.skill[0] = SET_NUMBERHB(skill);
  6498: 	pMsg.skill[1] = SET_NUMBERLB(skill);
  6499: 
  6500: 	pMsg.index[0] = SET_NUMBERHB(lpObj->Index);
  6501: 	pMsg.index[1] = SET_NUMBERLB(lpObj->Index);
  6502: 
  6503: 	pMsg.target[0] = SET_NUMBERHB(aIndex) | (type*0x80);
  6504: 	pMsg.target[1] = SET_NUMBERLB(aIndex);
  6505: 
  6506: 	if(lpObj->Type == OBJECT_USER)
  6507: 	{
  6508: 		DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);
  6509: 	}
  6510: 
  6511: 	MsgSendV2(lpObj,(BYTE*)&pMsg,pMsg.header.size);
  6512: 
  6513: 	#endif
  6514: }
  6515: 
  6516: void CSkillManager::GCSkillAddSend(int aIndex,BYTE slot,int skill,BYTE level,BYTE type) // OK
  6517: {
  6518: 	BYTE send[256];
  6519: 
  6520: 	PMSG_SKILL_LIST_SEND pMsg;
  6521: 
  6522: 	pMsg.header.set(0xF3,0x11,0);
  6523: 
  6524: 	int size = sizeof(pMsg);
  6525: 
  6526: 	pMsg.count = 0xFE;
  6527: 
  6528: 	pMsg.type = type;
  6529: 
  6530: 	PMSG_SKILL_LIST info;
  6531: 
  6532: 	info.slot = slot;
  6533: 
  6534: 	info.skill = skill;
  6535: 
  6536: 	info.level = (level << 3) | ((skill/255) & 7);
  6537: 
  6538: 	memcpy(&send[size],&info,sizeof(info));
  6539: 	size += sizeof(info);
  6540: 
  6541: 	pMsg.header.size = size;
  6542: 
  6543: 	memcpy(send,&pMsg,sizeof(pMsg));
  6544: 
  6545: 	DataSend(aIndex,send,size);
  6546: }
  6547: 
  6548: void CSkillManager::GCSkillDelSend(int aIndex,BYTE slot,int skill,BYTE level,BYTE type) // OK
  6549: {
  6550: 	BYTE send[256];
  6551: 
  6552: 	PMSG_SKILL_LIST_SEND pMsg;
  6553: 
  6554: 	pMsg.header.set(0xF3,0x11,0);
  6555: 
  6556: 	int size = sizeof(pMsg);
  6557: 
  6558: 	pMsg.count = 0xFF;
  6559: 
  6560: 	pMsg.type = type;
  6561: 
  6562: 	PMSG_SKILL_LIST info;
  6563: 
  6564: 	info.slot = slot;
  6565: 
  6566: 	info.skill = skill;
  6567: 
  6568: 	info.level = (level << 3) | ((skill/255) & 7);
  6569: 
  6570: 	memcpy(&send[size],&info,sizeof(info));
  6571: 	size += sizeof(info);
  6572: 
  6573: 	pMsg.header.size = size;
  6574: 
  6575: 	memcpy(send,&pMsg,sizeof(pMsg));
  6576: 
  6577: 	DataSend(aIndex,send,size);
  6578: }
  6579: 
  6580: void CSkillManager::GCSkillListSend(LPOBJ lpObj,BYTE type) // OK
  6581: {
  6582: 	BYTE send[1024];
  6583: 
  6584: 	PMSG_SKILL_LIST_SEND pMsg;
  6585: 
  6586: 	pMsg.header.set(0xF3,0x11,0);
  6587: 
  6588: 	int size = sizeof(pMsg);
  6589: 
  6590: 	pMsg.count = 0;
  6591: 
  6592: 	pMsg.type = type;
  6593: 
  6594: 	PMSG_SKILL_LIST info;
  6595: 
  6596: 	for(int n=0;n < MAX_SKILL_LIST;n++)
  6597: 	{
  6598: 		if(lpObj->Skill[n].IsSkill() == 0)
  6599: 		{
  6600: 			continue;
  6601: 		}
  6602: 
  6603: 		info.slot = n;
  6604: 
  6605: 		info.skill = lpObj->Skill[n].m_index;
  6606: 
  6607: 		info.level = (lpObj->Skill[n].m_level << 3) | ((lpObj->Skill[n].m_index/255) & 7);
  6608: 
  6609: 		memcpy(&send[size],&info,sizeof(info));
  6610: 		size += sizeof(info);
  6611: 
  6612: 		pMsg.count++;
  6613: 	}
  6614: 
  6615: 	pMsg.header.size = size;
  6616: 
  6617: 	memcpy(send,&pMsg,sizeof(pMsg));
  6618: 
  6619: 	DataSend(lpObj->Index,send,size);
  6620: }
```

### HIT: Any 0x19 mention / \b0x19\b

- line 6413: 	pMsg.header.set(0x19,sizeof(pMsg));

#### CONTEXT (first hit)

```
  6343: 	if(lpObj->PartyNumber != lpTarget->PartyNumber)
  6344: 	{
  6345: 		return;
  6346: 	}
  6347: 
  6348: 	if(gEffectManager.CheckStunEffect(lpObj) != 0)
  6349: 	{
  6350: 		return;
  6351: 	}
  6352: 
  6353: 	if(lpObj->Map != lpTarget->Map)
  6354: 	{
  6355: 		return;
  6356: 	}
  6357: 
  6358: 	if(gObjCheckTeleportArea(lpMsg->index,lpMsg->x,lpMsg->y) == 0)
  6359: 	{
  6360: 		return;
  6361: 	}
  6362: 
  6363: 	#if(GAMESERVER_TYPE==1)
  6364: 
  6365: 	if(lpObj->Map == MAP_CASTLE_SIEGE && gCastleSiege.CheckTeleportMagicAxisY(lpObj->Y,lpMsg->x,lpMsg->y) == 0)
  6366: 	{
  6367: 		lpMsg->y = (BYTE)lpObj->Y;
  6368: 	}
  6369: 
  6370: 	#endif
  6371: 
  6372: 	CSkill* lpSkill = gSkillManager.GetSkill(lpObj,SKILL_TELEPORT_ALLY);
  6373: 
  6374: 	if(lpSkill != 0)
  6375: 	{
  6376: 		if(IT_MAP_RANGE(lpObj->Map) != 0 && gIllusionTemple.GetState(GET_IT_LEVEL(lpObj->Map)) != IT_STATE_START)
  6377: 		{
  6378: 			lpMsg->x = (BYTE)lpObj->X;
  6379: 			lpMsg->y = (BYTE)lpObj->Y;
  6380: 		}
  6381: 
  6382: 		#if(GAMESERVER_UPDATE>=501)
  6383: 
  6384: 		if(DG_MAP_RANGE(lpObj->Map) != 0 && gDoubleGoer.GetState() != DG_STATE_START)
  6385: 		{
  6386: 			lpMsg->x = (BYTE)lpObj->X;
  6387: 			lpMsg->y = (BYTE)lpObj->Y;
  6388: 		}
  6389: 
  6390: 		#endif
  6391: 
  6392: 		if(gSkillManager.CheckSkillMana(lpObj,lpSkill->m_index) == 0 || gSkillManager.CheckSkillBP(lpObj,lpSkill->m_index) == 0)
  6393: 		{
  6394: 			return;
  6395: 		}
  6396: 
  6397: 		gSkillManager.GCSkillAttackSend(lpObj,lpSkill->m_index,lpMsg->index,1);
  6398: 
  6399: 		gObjTeleportMagicUse(lpMsg->index,lpMsg->x,lpMsg->y);
  6400: 
  6401: 		lpObj->Mana -= (gSkillManager.GetSkillMana(lpSkill->m_index)*lpObj->MPConsumptionRate)/100;
  6402: 
  6403: 		lpObj->BP -= (gSkillManager.GetSkillBP(lpSkill->m_index)*lpObj->BPConsumptionRate)/100;
  6404: 
  6405: 		GCManaSend(lpObj->Index,0xFF,(int)lpObj->Mana,lpObj->BP);
  6406: 	}
  6407: }
  6408: 
  6409: void CSkillManager::GCSkillAttackSend(LPOBJ lpObj,int skill,int aIndex,BYTE type) // OK
  6410: {
  6411: 	PMSG_SKILL_ATTACK_SEND pMsg;
  6412: 
  6413: 	pMsg.header.set(0x19,sizeof(pMsg));
  6414: 
  6415: 	pMsg.skill[0] = SET_NUMBERHB(skill);
  6416: 	pMsg.skill[1] = SET_NUMBERLB(skill);
  6417: 
  6418: 	pMsg.index[0] = SET_NUMBERHB(lpObj->Index);
  6419: 	pMsg.index[1] = SET_NUMBERLB(lpObj->Index);
  6420: 
  6421: 	pMsg.target[0] = SET_NUMBERHB(aIndex) | (type*0x80);
  6422: 	pMsg.target[1] = SET_NUMBERLB(aIndex);
  6423: 
  6424: 	if(lpObj->Type == OBJECT_USER)
  6425: 	{
  6426: 		DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);
  6427: 	}
  6428: 
  6429: 	MsgSendV2(lpObj,(BYTE*)&pMsg,pMsg.header.size);
  6430: 
  6431: }
  6432: 
  6433: void CSkillManager::GCSkillCancelSend(LPOBJ lpObj,int skill) // OK
  6434: {
  6435: 	PMSG_SKILL_CANCEL_SEND pMsg;
  6436: 
  6437: 	pMsg.header.set(0x1B,sizeof(pMsg));
  6438: 
  6439: 	pMsg.skill[0] = SET_NUMBERHB(skill);
  6440: 	pMsg.skill[1] = SET_NUMBERLB(skill);
  6441: 
  6442: 	pMsg.index[0] = SET_NUMBERHB(lpObj->Index);
  6443: 	pMsg.index[1] = SET_NUMBERLB(lpObj->Index);
  6444: 
  6445: 	if(lpObj->Type == OBJECT_USER)
  6446: 	{
  6447: 		DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);
  6448: 	}
  6449: 
  6450: 	MsgSendV2(lpObj,(BYTE*)&pMsg,pMsg.header.size);
  6451: }
  6452: 
  6453: void CSkillManager::GCDurationSkillAttackSend(LPOBJ lpObj,int skill,BYTE x,BYTE y,BYTE dir) // OK
  6454: {
  6455: 	PMSG_DURATION_SKILL_ATTACK_SEND pMsg;
  6456: 
  6457: 	pMsg.header.set(0x1E,sizeof(pMsg));
  6458: 
  6459: 	#if(GAMESERVER_UPDATE>=701)
  6460: 
  6461: 	pMsg.skillH = SET_NUMBERHB(skill);
  6462: 	pMsg.skillL = SET_NUMBERLB(skill);
  6463: 
  6464: 	pMsg.indexH = SET_NUMBERHB(lpObj->Index);
  6465: 	pMsg.indexL = SET_NUMBERLB(lpObj->Index);
  6466: 
  6467: 	#else
  6468: 
  6469: 	pMsg.skill[0] = SET_NUMBERHB(skill);
  6470: 	pMsg.skill[1] = SET_NUMBERLB(skill);
  6471: 
  6472: 	pMsg.index[0] = SET_NUMBERHB(lpObj->Index);
  6473: 	pMsg.index[1] = SET_NUMBERLB(lpObj->Index);
  6474: 
  6475: 	#endif
  6476: 
  6477: 	pMsg.x = x;
  6478: 	pMsg.y = y;
  6479: 	pMsg.dir = dir;
  6480: 
  6481: 	if(lpObj->Type == OBJECT_USER)
  6482: 	{
  6483: 		DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);
  6484: 	}
  6485: 
  6486: 	MsgSendV2(lpObj,(BYTE*)&pMsg,pMsg.header.size);
  6487: }
  6488: 
  6489: void CSkillManager::GCRageFighterSkillAttackSend(LPOBJ lpObj,int skill,int aIndex,BYTE type) // OK
  6490: {
  6491: 	#if(GAMESERVER_UPDATE>=601)
  6492: 
  6493: 	PMSG_RAGE_FIGHTER_SKILL_ATTACK_SEND pMsg;
  6494: 
  6495: 	pMsg.header.set(0x4A,sizeof(pMsg));
  6496: 
  6497: 	pMsg.skill[0] = SET_NUMBERHB(skill);
  6498: 	pMsg.skill[1] = SET_NUMBERLB(skill);
  6499: 
  6500: 	pMsg.index[0] = SET_NUMBERHB(lpObj->Index);
  6501: 	pMsg.index[1] = SET_NUMBERLB(lpObj->Index);
  6502: 
  6503: 	pMsg.target[0] = SET_NUMBERHB(aIndex) | (type*0x80);
  6504: 	pMsg.target[1] = SET_NUMBERLB(aIndex);
  6505: 
  6506: 	if(lpObj->Type == OBJECT_USER)
  6507: 	{
  6508: 		DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);
  6509: 	}
  6510: 
  6511: 	MsgSendV2(lpObj,(BYTE*)&pMsg,pMsg.header.size);
  6512: 
  6513: 	#endif
  6514: }
  6515: 
  6516: void CSkillManager::GCSkillAddSend(int aIndex,BYTE slot,int skill,BYTE level,BYTE type) // OK
  6517: {
  6518: 	BYTE send[256];
  6519: 
  6520: 	PMSG_SKILL_LIST_SEND pMsg;
  6521: 
  6522: 	pMsg.header.set(0xF3,0x11,0);
  6523: 
  6524: 	int size = sizeof(pMsg);
  6525: 
  6526: 	pMsg.count = 0xFE;
  6527: 
  6528: 	pMsg.type = type;
  6529: 
  6530: 	PMSG_SKILL_LIST info;
  6531: 
  6532: 	info.slot = slot;
  6533: 
  6534: 	info.skill = skill;
  6535: 
  6536: 	info.level = (level << 3) | ((skill/255) & 7);
  6537: 
  6538: 	memcpy(&send[size],&info,sizeof(info));
  6539: 	size += sizeof(info);
  6540: 
  6541: 	pMsg.header.size = size;
  6542: 
  6543: 	memcpy(send,&pMsg,sizeof(pMsg));
  6544: 
  6545: 	DataSend(aIndex,send,size);
  6546: }
  6547: 
  6548: void CSkillManager::GCSkillDelSend(int aIndex,BYTE slot,int skill,BYTE level,BYTE type) // OK
  6549: {
  6550: 	BYTE send[256];
  6551: 
  6552: 	PMSG_SKILL_LIST_SEND pMsg;
  6553: 
  6554: 	pMsg.header.set(0xF3,0x11,0);
  6555: 
  6556: 	int size = sizeof(pMsg);
  6557: 
  6558: 	pMsg.count = 0xFF;
  6559: 
  6560: 	pMsg.type = type;
  6561: 
  6562: 	PMSG_SKILL_LIST info;
  6563: 
  6564: 	info.slot = slot;
  6565: 
  6566: 	info.skill = skill;
  6567: 
  6568: 	info.level = (level << 3) | ((skill/255) & 7);
  6569: 
  6570: 	memcpy(&send[size],&info,sizeof(info));
  6571: 	size += sizeof(info);
  6572: 
  6573: 	pMsg.header.size = size;
  6574: 
  6575: 	memcpy(send,&pMsg,sizeof(pMsg));
  6576: 
  6577: 	DataSend(aIndex,send,size);
  6578: }
  6579: 
  6580: void CSkillManager::GCSkillListSend(LPOBJ lpObj,BYTE type) // OK
  6581: {
  6582: 	BYTE send[1024];
  6583: 
  6584: 	PMSG_SKILL_LIST_SEND pMsg;
  6585: 
  6586: 	pMsg.header.set(0xF3,0x11,0);
  6587: 
  6588: 	int size = sizeof(pMsg);
  6589: 
  6590: 	pMsg.count = 0;
  6591: 
  6592: 	pMsg.type = type;
  6593: 
  6594: 	PMSG_SKILL_LIST info;
  6595: 
  6596: 	for(int n=0;n < MAX_SKILL_LIST;n++)
  6597: 	{
  6598: 		if(lpObj->Skill[n].IsSkill() == 0)
  6599: 		{
  6600: 			continue;
  6601: 		}
  6602: 
  6603: 		info.slot = n;
  6604: 
  6605: 		info.skill = lpObj->Skill[n].m_index;
  6606: 
  6607: 		info.level = (lpObj->Skill[n].m_level << 3) | ((lpObj->Skill[n].m_index/255) & 7);
  6608: 
  6609: 		memcpy(&send[size],&info,sizeof(info));
  6610: 		size += sizeof(info);
  6611: 
  6612: 		pMsg.count++;
  6613: 	}
  6614: 
  6615: 	pMsg.header.size = size;
  6616: 
  6617: 	memcpy(send,&pMsg,sizeof(pMsg));
  6618: 
  6619: 	DataSend(lpObj->Index,send,size);
  6620: }
```

### HIT: Client-visible magic struct tokens / \bPMSG_\w*MAGIC\w*\b|\bMAGIC\b|\bSkill\b

- line 779: 		if(lpObj->Skill[n].IsSkill() == 0)
- line 781: 			return ((lpObj->Skill[n].Set(index,level)==0)?-1:n);
- line 792: 		if(lpObj->Skill[n].IsSkill() != 0)
- line 794: 			if(lpObj->Skill[n].m_index == index)
- line 796: 				lpObj->Skill[n].Clear();
- line 811: 		if(lpObj->Skill[n].IsSkill() != 0)
- line 813: 			if(lpObj->Skill[n].m_index == index || lpObj->Skill[n].m_skill == index)
- line 815: 				return &lpObj->Skill[n];
- line 5861: 		lpSkill = &lpObj->Skill[skill];
- line 6059: 		lpSkill = &lpObj->Skill[skill];
- line 6154: 		lpSkill = &lpObj->Skill[skill];
- line 6598: 		if(lpObj->Skill[n].IsSkill() == 0)
- line 6605: 		info.skill = lpObj->Skill[n].m_index;
- line 6607: 		info.level = (lpObj->Skill[n].m_level << 3) | ((lpObj->Skill[n].m_index/255) & 7);

#### CONTEXT (first hit)

```
   709: 		{
   710: 			if(lpObj->Inventory[n].m_Option1 != 0)
   711: 			{
   712: 				if(lpObj->Inventory[n].m_SpecialIndex[SPECIAL_OPTION1] == index)
   713: 				{
   714: 					return 1;
   715: 				}
   716: 			}
   717: 		}
   718: 	}
   719: 
   720: 	return 0;
   721: }
   722: 
   723: int CSkillManager::AddSkillWeapon(LPOBJ lpObj,int index,int level) // OK
   724: {
   725: 	int slot=0,skill=0;
   726: 
   727: 	if((slot=this->AddSkill(lpObj,(skill=gMasterSkillTree.GetMasterSkillWeapon(lpObj,index)),level)) >= 0)
   728: 	{
   729: 		this->GCSkillAddSend(lpObj->Index,slot,skill,(BYTE)level,0);return 1;
   730: 	}
   731: 	else
   732: 	{
   733: 		return 0;
   734: 	}
   735: }
   736: 
   737: int CSkillManager::DelSkillWeapon(LPOBJ lpObj,int index,int level) // OK
   738: {
   739: 	int slot=0,skill=0,count=0;
   740: 
   741: 	for(int n=0;n < INVENTORY_WEAR_SIZE;n++)
   742: 	{
   743: 		if(lpObj->Inventory[n].IsItem() != 0)
   744: 		{
   745: 			if(lpObj->Inventory[n].m_Option1 != 0)
   746: 			{
   747: 				if(lpObj->Inventory[n].m_SpecialIndex[SPECIAL_OPTION1] == index)
   748: 				{
   749: 					count++;
   750: 				}
   751: 			}
   752: 		}
   753: 	}
   754: 
   755: 	if(count == 1 && (slot=this->DelSkill(lpObj,(skill=gMasterSkillTree.GetMasterSkillWeapon(lpObj,index)))) >= 0)
   756: 	{
   757: 		this->GCSkillDelSend(lpObj->Index,slot,skill,(BYTE)level,0);return 1;
   758: 	}
   759: 	else
   760: 	{
   761: 		return 0;
   762: 	}
   763: }
   764: 
   765: int CSkillManager::AddSkill(LPOBJ lpObj,int index,int level) // OK
   766: {
   767: 	if(lpObj->Type == OBJECT_USER && (this->CheckSkillRequireLevel(lpObj,index) == 0 || this->CheckSkillRequireEnergy(lpObj,index) == 0 || this->CheckSkillRequireLeadership(lpObj,index) == 0 || this->CheckSkillRequireClass(lpObj,index) == 0))
   768: 	{
   769: 		return -1;
   770: 	}
   771: 
   772: 	if(this->GetSkill(lpObj,index) != 0)
   773: 	{
   774: 		return -1;
   775: 	}
   776: 
   777: 	for(int n=0;n < MAX_SKILL_LIST;n++)
   778: 	{
   779: 		if(lpObj->Skill[n].IsSkill() == 0)
   780: 		{
   781: 			return ((lpObj->Skill[n].Set(index,level)==0)?-1:n);
   782: 		}
   783: 	}
   784: 
   785: 	return -1;
   786: }
   787: 
   788: int CSkillManager::DelSkill(LPOBJ lpObj,int index) // OK
   789: {
   790: 	for(int n=0;n < MAX_SKILL_LIST;n++)
   791: 	{
   792: 		if(lpObj->Skill[n].IsSkill() != 0)
   793: 		{
   794: 			if(lpObj->Skill[n].m_index == index)
   795: 			{
   796: 				lpObj->Skill[n].Clear();
   797: 				return n;
   798: 			}
   799: 		}
   800: 	}
   801: 
   802: 	return -1;
   803: }
   804: 
   805: CSkill* CSkillManager::GetSkill(LPOBJ lpObj,int index) // OK
   806: {
   807: 	index = ((index==SKILL_NOVA_START)?SKILL_NOVA:index);
   808: 
   809: 	for(int n=0;n < MAX_SKILL_LIST;n++)
   810: 	{
   811: 		if(lpObj->Skill[n].IsSkill() != 0)
   812: 		{
   813: 			if(lpObj->Skill[n].m_index == index || lpObj->Skill[n].m_skill == index)
   814: 			{
   815: 				return &lpObj->Skill[n];
   816: 			}
   817: 		}
   818: 	}
   819: 
   820: 	return 0;
   821: }
   822: 
   823: int CSkillManager::AddMasterSkill(LPOBJ lpObj,int index,int level) // OK
   824: {
   825: 	#if(GAMESERVER_UPDATE>=401)
   826: 
   827: 	if(lpObj->Type == OBJECT_USER && (this->CheckSkillRequireLevel(lpObj,index) == 0 || this->CheckSkillRequireEnergy(lpObj,index) == 0 || this->CheckSkillRequireLeadership(lpObj,index) == 0 || this->CheckSkillRequireClass(lpObj,index) == 0))
   828: 	{
   829: 		return -1;
   830: 	}
   831: 
   832: 	if(this->GetMasterSkill(lpObj,index) != 0)
   833: 	{
   834: 		return -1;
   835: 	}
   836: 
   837: 	for(int n=0;n < MAX_MASTER_SKILL_LIST;n++)
   838: 	{
   839: 		if(lpObj->MasterSkill[n].IsMasterSkill() == 0)
   840: 		{
   841: 			return ((lpObj->MasterSkill[n].Set(index,level)==0)?-1:n);
   842: 		}
   843: 	}
   844: 
   845: 	return -1;
   846: 
   847: 	#else
   848: 
   849: 	return -1;
   850: 
   851: 	#endif
   852: }
   853: 
   854: int CSkillManager::DelMasterSkill(LPOBJ lpObj,int index) // OK
   855: {
   856: 	#if(GAMESERVER_UPDATE>=401)
   857: 
   858: 	for(int n=0;n < MAX_MASTER_SKILL_LIST;n++)
   859: 	{
   860: 		if(lpObj->MasterSkill[n].IsMasterSkill() != 0)
   861: 		{
   862: 			if(lpObj->MasterSkill[n].m_index == index)
   863: 			{
   864: 				lpObj->MasterSkill[n].Clear();
   865: 				return n;
   866: 			}
   867: 		}
   868: 	}
   869: 
   870: 	return -1;
   871: 
   872: 	#else
   873: 
   874: 	return -1;
   875: 
   876: 	#endif
   877: }
   878: 
   879: CSkill* CSkillManager::GetMasterSkill(LPOBJ lpObj,int index) // OK
   880: {
   881: 	#if(GAMESERVER_UPDATE>=401)
   882: 
   883: 	index = ((index==SKILL_NOVA_START)?SKILL_NOVA:index);
   884: 
   885: 	for(int n=0;n < MAX_MASTER_SKILL_LIST;n++)
   886: 	{
   887: 		if(lpObj->MasterSkill[n].IsMasterSkill() != 0)
   888: 		{
   889: 			if(lpObj->MasterSkill[n].m_index == index || lpObj->MasterSkill[n].m_skill == index)
   890: 			{
   891: 				return &lpObj->MasterSkill[n];
   892: 			}
   893: 		}
   894: 	}
   895: 
   896: 	return 0;
   897: 
   898: 	#else
   899: 
   900: 	return 0;
   901: 
   902: 	#endif
   903: }
   904: 
   905: bool CSkillManager::ConvertSkillByte(CSkill* lpSkill,BYTE* lpMsg) // OK
   906: {
   907: 	WORD index = MAKE_NUMBERW(((lpMsg[0]==0xFF&&lpMsg[2]==0x00)?0xFF:lpMsg[2]),lpMsg[0]);
   908: 
   909: 	if(index == 0xFFFF)
   910: 	{
   911: 		return 0;
   912: 	}
   913: 
   914: 	lpSkill->m_level = lpMsg[1];
   915: 	lpSkill->m_skill = index;
   916: 	lpSkill->m_index = index;
   917: 	return 1;
   918: }
   919: 
   920: void CSkillManager::SkillByteConvert(BYTE* lpMsg,CSkill* lpSkill) // OK
   921: {
   922: 	lpMsg[0] = SET_NUMBERLB(lpSkill->m_index);
   923: 	lpMsg[1] = lpSkill->m_level;
   924: 	lpMsg[2] = SET_NUMBERHB(lpSkill->m_index);
   925: }
   926: 
   927: void CSkillManager::UseAttackSkill(int aIndex,int bIndex,CSkill* lpSkill) // OK
   928: {
   929: 	LPOBJ lpObj = &gObj[aIndex];
   930: 
   931: 	if(lpObj->Type == OBJECT_USER && this->CheckSkillRequireWeapon(lpObj,lpSkill->m_skill) == 0)
   932: 	{
   933: 		return;
   934: 	}
   935: 
   936: 	if(lpObj->Type == OBJECT_USER && lpSkill->m_skill == SKILL_TWISTING_SLASH && lpObj->Inventory[0].IsItem() == 0)
   937: 	{
   938: 		return;
   939: 	}
   940: 
   941: 	bool combo = 0;
   942: 
   943: 	if((gServerInfo.m_EnableComboToAllSwitch == 1 || gQuest.CheckQuestListState(lpObj,3,QUEST_FINISH) != 0) && lpObj->ComboSkill.CheckCombo(lpSkill->m_skill) != 0)
   944: 	{
   945: 		if(gServerInfo.m_CheckAutoComboHack == 0 || (GetTickCount()-lpObj->ComboTime) > ((DWORD)gServerInfo.m_CheckAutoComboHackTolerance))
   946: 		{
   947: 			combo = 1;
   948: 			lpObj->ComboTime = GetTickCount();
   949: 		}
   950: 
   951: 	}
   952: 	//LogAdd(LOG_ALERT,"%d",lpSkill->m_skill);
   953: 
   954: 	if(lpObj->SkillNovaState != 0 && lpSkill->m_skill != SKILL_NOVA && this->GetSkill(lpObj,SKILL_NOVA) != 0)
   955: 	{
   956: 		this->RunningSkill(aIndex,0,this->GetSkill(lpObj,SKILL_NOVA),(BYTE)lpObj->X,(BYTE)lpObj->Y,0,combo);
   957: 		return;
   958: 	}
   959: 
   960: 	#if(GAMESERVER_UPDATE>=602)
   961: 	if(lpSkill->m_skill == SKILL_NOVA && bIndex != SKILL_NOVA_START && bIndex != MASTER_SKILL_ADD_NOVA_START_IMPROVED)
   962: 	#else
   963: 	if(lpSkill->m_skill == SKILL_NOVA && bIndex != SKILL_NOVA_START)
   964: 	#endif
   965: 	{
   966: 		this->RunningSkill(aIndex,bIndex,lpSkill,(BYTE)lpObj->X,(BYTE)lpObj->Y,0,combo);
   967: 		return;
   968: 	}
   969: 
   970: 	if((lpSkill->m_skill == SKILL_UPPER_BEAST || lpSkill->m_skill == SKILL_CHAIN_DRIVER || lpSkill->m_skill == SKILL_DARK_SIDE) && lpObj->RageFighterSkillIndex == lpSkill->m_index && (lpObj->RageFighterSkillCount++) > 0)
   971: 	{
   972: 		this->RunningSkill(aIndex,bIndex,lpSkill,(BYTE)lpObj->X,(BYTE)lpObj->Y,0,combo);
   973: 		return;
   974: 	}
   975: 
   976: 	if(lpObj->Type != OBJECT_USER || (this->CheckSkillMana(lpObj,lpSkill->m_index) != 0 && this->CheckSkillBP(lpObj,lpSkill->m_index) != 0))
   977: 	{
   978: 		if(this->RunningSkill(aIndex,bIndex,lpSkill,(BYTE)lpObj->X,(BYTE)lpObj->Y,0,combo) != 0 && lpObj->Type == OBJECT_USER)
   979: 		{
   980: 			lpObj->Mana -= ((this->GetSkillMana(lpSkill->m_index)*lpObj->MPConsumptionRate)/100);
   981: 			lpObj->BP -= ((this->GetSkillBP(lpSkill->m_index)*lpObj->BPConsumptionRate)/100);
   982: 			GCManaSend(aIndex,0xFF,(int)lpObj->Mana,lpObj->BP);
   983: 		}
   984: 	}
   985: }
   986: 
   987: void CSkillManager::UseDurationSkillAttack(int aIndex,int bIndex,CSkill* lpSkill,BYTE x,BYTE y,BYTE dir,BYTE angle) // OK
   988: {
   989: 	LPOBJ lpObj = &gObj[aIndex];
   990: 
   991: 	if(lpObj->Type == OBJECT_USER && this->CheckSkillRequireWeapon(lpObj,lpSkill->m_skill) == 0)
   992: 	{
   993: 		return;
   994: 	}
   995: 
   996: 	if(lpObj->Type == OBJECT_USER && lpSkill->m_skill == SKILL_TWISTING_SLASH && lpObj->Inventory[0].IsItem() == 0)
   997: 	{
   998: 		return;
   999: 	}
  1000: 
  1001: 	bool combo = 0;
  1002: 
  1003: 
  1004: 	if(lpSkill->m_skill != SKILL_FLAME && lpSkill->m_skill != SKILL_TWISTER && lpSkill->m_skill != SKILL_EVIL_SPIRIT && lpSkill->m_skill != SKILL_HELL_FIRE && lpSkill->m_skill != SKILL_AQUA_BEAM && lpSkill->m_skill != SKILL_BLAST && lpSkill->m_skill != SKILL_INFERNO && lpSkill->m_skill != SKILL_TRIPLE_SHOT && lpSkill->m_skill != SKILL_IMPALE && lpSkill->m_skill != SKILL_MONSTER_AREA_ATTACK && lpSkill->m_skill != SKILL_PENETRATION && lpSkill->m_skill != SKILL_FIRE_SLASH && lpSkill->m_skill != SKILL_FIRE_SCREAM)
  1005: 	{
  1006: 		if((gServerInfo.m_EnableComboToAllSwitch == 1 || gQuest.CheckQuestListState(lpObj,3,QUEST_FINISH) != 0) && lpObj->ComboSkill.CheckCombo(lpSkill->m_skill) != 0)
  1007: 		{
  1008: 			if(gServerInfo.m_CheckAutoComboHack == 0 || (GetTickCount()-lpObj->ComboTime) > ((DWORD)gServerInfo.m_CheckAutoComboHackTolerance))
  1009: 			{
  1010: 				combo = 1;
  1011: 				lpObj->ComboTime = GetTickCount();
  1012: 			}
  1013: 		}
  1014: 	}
  1015: 
  1016: 	if(lpObj->SkillNovaState != 0 && lpSkill->m_skill != SKILL_NOVA && this->GetSkill(lpObj,SKILL_NOVA) != 0)
  1017: 	{
  1018: 		this->RunningSkill(aIndex,0,this->GetSkill(lpObj,SKILL_NOVA),x,y,0,combo);
  1019: 		return;
```

### HIT: Send APIs / \bDataSend\b|\bMsgSendV2\b|\bsend\(|\bGCSend\b

- line 3376: 	DataSend(aIndex,send,size);
- line 3378: 	MsgSendV2(lpObj,send,size);
- line 4495: 	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
- line 5106: 	DataSend(aIndex,send,size);
- line 5108: 	MsgSendV2(lpObj,send,size);
- line 6312: 	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
- line 6426: 		DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);
- line 6429: 	MsgSendV2(lpObj,(BYTE*)&pMsg,pMsg.header.size);
- line 6447: 		DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);
- line 6450: 	MsgSendV2(lpObj,(BYTE*)&pMsg,pMsg.header.size);
- line 6483: 		DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);
- line 6486: 	MsgSendV2(lpObj,(BYTE*)&pMsg,pMsg.header.size);
- line 6508: 		DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);
- line 6511: 	MsgSendV2(lpObj,(BYTE*)&pMsg,pMsg.header.size);

#### CONTEXT (first hit)

```
  3306: 	{
  3307: 		if(lpObj->VpPlayer2[n].state == VIEWPORT_NONE)
  3308: 		{
  3309: 			continue;
  3310: 		}
  3311: 
  3312: 		int index = lpObj->VpPlayer2[n].index;
  3313: 
  3314: 		if(bIndex == index)
  3315: 		{
  3316: 			continue;
  3317: 		}
  3318: 
  3319: 		if(this->CheckSkillTarget(lpObj,index,bIndex,lpObj->VpPlayer2[n].type) == 0)
  3320: 		{
  3321: 			continue;
  3322: 		}
  3323: 
  3324: 		if(this->CheckSkillRadio(lpSkill->m_index,lpTarget->X,lpTarget->Y,gObj[index].X,gObj[index].Y) == 0)
  3325: 		{
  3326: 			continue;
  3327: 		}
  3328: 
  3329: 		target[count++] = index;
  3330: 
  3331: 		if(count >= _countof(target))
  3332: 		{
  3333: 			break;
  3334: 		}
  3335: 	}
  3336: 
  3337: 	BYTE send[256];
  3338: 
  3339: 	PMSG_SKILL_CHAIN_SKILL_SEND pMsg;
  3340: 
  3341: 	pMsg.header.set(0xBF,0x0A,0);
  3342: 
  3343: 	int size = sizeof(pMsg);
  3344: 
  3345: 	pMsg.skill[0] = SET_NUMBERHB(lpSkill->m_index);
  3346: 
  3347: 	pMsg.skill[1] = SET_NUMBERLB(lpSkill->m_index);
  3348: 
  3349: 	pMsg.index = aIndex;
  3350: 
  3351: 	pMsg.count = 0;
  3352: 
  3353: 	PMSG_SKILL_CHAIN_SKILL info;
  3354: 
  3355: 	for(int n=0;n < _countof(target);n++)
  3356: 	{
  3357: 		gObjAddAttackProcMsgSendDelay(lpObj,52,target[n],(200+(n*200)),lpSkill->m_index,(n+1));
  3358: 
  3359: 		if(combo != 0)
  3360: 		{
  3361: 			this->GCSkillAttackSend(lpObj,SKILL_COMBO,target[n],1);
  3362: 		}
  3363: 
  3364: 		info.index = target[n];
  3365: 
  3366: 		memcpy(&send[size],&info,sizeof(info));
  3367: 		size += sizeof(info);
  3368: 
  3369: 		pMsg.count++;
  3370: 	}
  3371: 
  3372: 	pMsg.header.size = size;
  3373: 
  3374: 	memcpy(send,&pMsg,sizeof(pMsg));
  3375: 
  3376: 	DataSend(aIndex,send,size);
  3377: 
  3378: 	MsgSendV2(lpObj,send,size);
  3379: 
  3380: 	return 1;
  3381: }
  3382: 
  3383: bool CSkillManager::SkillDamageReflect(int aIndex,int bIndex,CSkill* lpSkill) // OK
  3384: {
  3385: 	LPOBJ lpTarget = &gObj[bIndex];
  3386: 
  3387: 	if(lpTarget->Type != OBJECT_USER && OBJECT_RANGE(lpTarget->SummonIndex) == 0)
  3388: 	{
  3389: 		return 0;
  3390: 	}
  3391: 
  3392: 	LPOBJ lpObj = &gObj[aIndex];
  3393: 
  3394: 	if(CA_MAP_RANGE(lpObj->Map) != 0 && aIndex != bIndex)
  3395: 	{
  3396: 		return 0;
  3397: 	}
  3398: 
  3399: 	if(CC_MAP_RANGE(lpObj->Map) != 0 && aIndex != bIndex)
  3400: 	{
  3401: 		return 0;
  3402: 	}
  3403: 
  3404: 	if(this->CheckSkillRange(lpSkill->m_index,lpObj->X,lpObj->Y,lpTarget->X,lpTarget->Y) == 0)
  3405: 	{
  3406: 		return 0;
  3407: 	}
  3408: 
  3409: 	int value = gServerInfo.m_ReflectDamageConstA+((lpObj->Energy+lpObj->AddEnergy)/gServerInfo.m_ReflectDamageConstB);
  3410: 
  3411: 	value = ((value>gServerInfo.m_ReflectDamageMaxRate)?gServerInfo.m_ReflectDamageMaxRate:value);
  3412: 
  3413: 	value = (value*((lpTarget->Type==OBJECT_USER)?gServerInfo.m_ReflectDamageRate[lpTarget->Class]:100))/100;
  3414: 
  3415: 	int count = gServerInfo.m_ReflectDamageTimeConstA+((lpObj->Energy+lpObj->AddEnergy)/gServerInfo.m_ReflectDamageTimeConstB);
  3416: 
  3417: 	gEffectManager.AddEffect(lpTarget,0,this->GetSkillEffect(lpSkill->m_index),count,value,0,0,0);
  3418: 
  3419: 	this->GCSkillAttackSend(lpObj,lpSkill->m_index,bIndex,1);
  3420: 	return 1;
  3421: }
  3422: 
  3423: bool CSkillManager::SkillSwordPower(int aIndex,int bIndex,CSkill* lpSkill,bool combo) // OK
  3424: {
  3425: 	LPOBJ lpObj = &gObj[aIndex];
  3426: 
  3427: 	if(lpObj->Type != OBJECT_USER || aIndex != bIndex)
  3428: 	{
  3429: 		return 0;
  3430: 	}
  3431: 
  3432: 	int value1 = (lpObj->Energy+lpObj->AddEnergy)/gServerInfo.m_SwordPowerConstA;
  3433: 
  3434: 	value1 = ((value1>gServerInfo.m_SwordPowerMaxRate)?gServerInfo.m_SwordPowerMaxRate:value1);
  3435: 
  3436: 	int value2 = (lpObj->Energy+lpObj->AddEnergy)/gServerInfo.m_SwordPowerConstA;
  3437: 
  3438: 	value2 = ((value2>gServerInfo.m_SwordPowerMaxRate)?gServerInfo.m_SwordPowerMaxRate:value2);
  3439: 
  3440: 	int value3 = (((40-((lpObj->Energy+lpObj->AddEnergy)/gServerInfo.m_SwordPowerConstB))<10)?10:(40-((lpObj->Energy+lpObj->AddEnergy)/gServerInfo.m_SwordPowerConstB)));
  3441: 
  3442: 	int value4 = (((40-((lpObj->Energy+lpObj->AddEnergy)/gServerInfo.m_SwordPowerConstB))<10)?10:(40-((lpObj->Energy+lpObj->AddEnergy)/gServerInfo.m_SwordPowerConstB)));
  3443: 
  3444: 	int count = gServerInfo.m_SwordPowerTimeConstA+((lpObj->Energy+lpObj->AddEnergy)/gServerInfo.m_SwordPowerTimeConstB);
  3445: 
  3446: 	gEffectManager.AddEffect(lpObj,0,this->GetSkillEffect(lpSkill->m_index),count,value1,value2,value3,value4);
  3447: 
  3448: 	this->GCSkillAttackSend(lpObj,lpSkill->m_index,bIndex,1);
  3449: 
  3450: 		if(combo != 0)
  3451: 		{
  3452: 			this->GCSkillAttackSend(lpObj,SKILL_COMBO,bIndex,1);
  3453: 		}
  3454: 
  3455: 	return 1;
  3456: }
  3457: 
  3458: bool CSkillManager::SkillSwordPowerGetDefense(int aIndex,int* defense) // OK
  3459: {
  3460: 	LPOBJ lpObj = &gObj[aIndex];
  3461: 
  3462: 	if(gEffectManager.CheckEffect(lpObj,EFFECT_SWORD_POWER) == 0 && gEffectManager.CheckEffect(lpObj,EFFECT_SWORD_POWER_IMPROVED) == 0 && gEffectManager.CheckEffect(lpObj,EFFECT_SWORD_POWER_ENHANCED) == 0 && gEffectManager.CheckEffect(lpObj,EFFECT_SWORD_POWER_MASTERED) == 0)
  3463: 	{
  3464: 		return 0;
  3465: 	}
  3466: 
  3467: 	int value = lpObj->EffectOption.AddSwordPowerDefenseRate;
  3468: 
  3469: 	(*defense) -= (((lpObj->Dexterity+lpObj->AddDexterity)/gServerInfo.m_SUDefenseConstA)*value)/100;
  3470: 
  3471: 	return 1;
  3472: }
  3473: 
  3474: bool CSkillManager::SkillSwordPowerGetPhysiDamage(int aIndex,int* DamageMin,int* DamageMax) // OK
  3475: {
  3476: 	LPOBJ lpObj = &gObj[aIndex];
  3477: 
  3478: 	if(gEffectManager.CheckEffect(lpObj,EFFECT_SWORD_POWER) == 0 && gEffectManager.CheckEffect(lpObj,EFFECT_SWORD_POWER_IMPROVED) == 0 && gEffectManager.CheckEffect(lpObj,EFFECT_SWORD_POWER_ENHANCED) == 0 && gEffectManager.CheckEffect(lpObj,EFFECT_SWORD_POWER_MASTERED) == 0)
  3479: 	{
  3480: 		return 0;
  3481: 	}
  3482: 
  3483: 	int value = lpObj->EffectOption.AddSwordPowerDamageRate;
  3484: 
  3485: 	value = ((value>gServerInfo.m_SwordPowerMaxRate)?gServerInfo.m_SwordPowerMaxRate:value);
  3486: 
  3487: 	(*DamageMin) += ((((lpObj->Strength+lpObj->AddStrength)+(lpObj->Dexterity+lpObj->AddDexterity))/gServerInfo.m_SUPhysiDamageMinConstA)*value)/100;
  3488: 
  3489: 	(*DamageMax) += ((((lpObj->Strength+lpObj->AddStrength)+(lpObj->Dexterity+lpObj->AddDexterity))/gServerInfo.m_SUPhysiDamageMaxConstA)*value)/100;
  3490: 
  3491: 	#if(GAMESERVER_UPDATE>=602)
  3492: 
  3493: 	(*DamageMin) += gMasterSkillTree.GetMasterSkillValue(lpObj,MASTER_SKILL_ADD_SWORD_POWER_MASTERED);
  3494: 
  3495: 	(*DamageMax) += gMasterSkillTree.GetMasterSkillValue(lpObj,MASTER_SKILL_ADD_SWORD_POWER_MASTERED);
  3496: 
  3497: 	#endif
  3498: 
  3499: 	return 1;
  3500: }
  3501: 
  3502: bool CSkillManager::SkillSwordPowerGetMagicDamage(int aIndex,int* DamageMin,int* DamageMax) // OK
  3503: {
  3504: 	LPOBJ lpObj = &gObj[aIndex];
  3505: 
  3506: 	if(gEffectManager.CheckEffect(lpObj,EFFECT_SWORD_POWER) == 0 && gEffectManager.CheckEffect(lpObj,EFFECT_SWORD_POWER_IMPROVED) == 0 && gEffectManager.CheckEffect(lpObj,EFFECT_SWORD_POWER_ENHANCED) == 0 && gEffectManager.CheckEffect(lpObj,EFFECT_SWORD_POWER_MASTERED) == 0)
  3507: 	{
  3508: 		return 0;
  3509: 	}
  3510: 
  3511: 	int value = lpObj->EffectOption.AddSwordPowerDamageRate;
  3512: 
  3513: 	#if(GAMESERVER_UPDATE>=602)
  3514: 
  3515: 	value += gMasterSkillTree.GetMasterSkillValue(lpObj,MASTER_SKILL_ADD_SWORD_POWER_ENHANCED);
  3516: 
  3517: 	#endif
  3518: 
  3519: 	value = ((value>gServerInfo.m_SwordPowerMaxRate)?gServerInfo.m_SwordPowerMaxRate:value);
  3520: 
  3521: 	(*DamageMin) += (((lpObj->Energy+lpObj->AddEnergy)/gServerInfo.m_SUMagicDamageMinConstA)*value)/100;
  3522: 
  3523: 	(*DamageMax) += (((lpObj->Energy+lpObj->AddEnergy)/gServerInfo.m_SUMagicDamageMaxConstA)*value)/100;
  3524: 
  3525: 	#if(GAMESERVER_UPDATE>=602)
  3526: 
  3527: 	(*DamageMin) += gMasterSkillTree.GetMasterSkillValue(lpObj,MASTER_SKILL_ADD_SWORD_POWER_MASTERED);
  3528: 
  3529: 	(*DamageMax) += gMasterSkillTree.GetMasterSkillValue(lpObj,MASTER_SKILL_ADD_SWORD_POWER_MASTERED);
  3530: 
  3531: 	#endif
  3532: 
  3533: 	return 1;
  3534: }
  3535: 
  3536: bool CSkillManager::SkillSwordPowerGetCurseDamage(int aIndex,int* DamageMin,int* DamageMax) // OK
  3537: {
  3538: 	LPOBJ lpObj = &gObj[aIndex];
  3539: 
  3540: 	if(gEffectManager.CheckEffect(lpObj,EFFECT_SWORD_POWER) == 0 && gEffectManager.CheckEffect(lpObj,EFFECT_SWORD_POWER_IMPROVED) == 0 && gEffectManager.CheckEffect(lpObj,EFFECT_SWORD_POWER_ENHANCED) == 0 && gEffectManager.CheckEffect(lpObj,EFFECT_SWORD_POWER_MASTERED) == 0)
  3541: 	{
  3542: 		return 0;
  3543: 	}
  3544: 
  3545: 	int value = lpObj->EffectOption.AddSwordPowerDamageRate;
  3546: 
  3547: 	#if(GAMESERVER_UPDATE>=602)
  3548: 
  3549: 	value += gMasterSkillTree.GetMasterSkillValue(lpObj,MASTER_SKILL_ADD_SWORD_POWER_IMPROVED);
  3550: 
  3551: 	#endif
  3552: 
  3553: 	value = ((value>gServerInfo.m_SwordPowerMaxRate)?gServerInfo.m_SwordPowerMaxRate:value);
  3554: 
  3555: 	(*DamageMin) += (((lpObj->Energy+lpObj->AddEnergy)/gServerInfo.m_SUMagicDamageMinConstA)*value)/100;
  3556: 
  3557: 	(*DamageMax) += (((lpObj->Energy+lpObj->AddEnergy)/gServerInfo.m_SUMagicDamageMaxConstA)*value)/100;
  3558: 
  3559: 	#if(GAMESERVER_UPDATE>=602)
  3560: 
  3561: 	(*DamageMin) += gMasterSkillTree.GetMasterSkillValue(lpObj,MASTER_SKILL_ADD_SWORD_POWER_MASTERED);
  3562: 
  3563: 	(*DamageMax) += gMasterSkillTree.GetMasterSkillValue(lpObj,MASTER_SKILL_ADD_SWORD_POWER_MASTERED);
  3564: 
  3565: 	#endif
  3566: 
  3567: 	return 1;
  3568: }
  3569: 
  3570: bool CSkillManager::SkillSleep(int aIndex,int bIndex,CSkill* lpSkill) // OK
  3571: {
  3572: 	LPOBJ lpObj = &gObj[aIndex];
  3573: 
  3574: 	if(OBJECT_RANGE(bIndex) == 0)
  3575: 	{
  3576: 		return 0;
  3577: 	}
  3578: 
  3579: 	LPOBJ lpTarget = &gObj[bIndex];
  3580: 
  3581: 	if(lpTarget->Type == OBJECT_USER && lpTarget->Authority == 32)
  3582: 	{
  3583: 		return 0;
  3584: 	}
  3585: 
  3586: 	if(this->CheckSkillRange(lpSkill->m_index,lpObj->X,lpObj->Y,lpTarget->X,lpTarget->Y) == 0)
  3587: 	{
  3588: 		return 0;
  3589: 	}
  3590: 
  3591: 	if(lpTarget->Type == OBJECT_USER && this->CheckSkillTarget(lpObj,bIndex,-1,lpTarget->Type) == 0)
  3592: 	{
  3593: 		return 0;
  3594: 	}
  3595: 
  3596: 	int rate = gServerInfo.m_SleepConstA+((lpObj->Energy+lpObj->AddEnergy)/gServerInfo.m_SleepConstB);
  3597: 
  3598: 	rate += lpObj->Inventory[1].GetBookSuccessRate();
  3599: 
  3600: 	#if(GAMESERVER_UPDATE>=602)
  3601: 
  3602: 	rate += gMasterSkillTree.GetMasterSkillValue(lpObj,MASTER_SKILL_ADD_SLEEP_IMPROVED);
  3603: 
  3604: 	#elif(GAMESERVER_UPDATE>=401)
  3605: 
  3606: 	rate += gMasterSkillTree.GetMasterSkillValue(lpObj,lpSkill->m_index);
  3607: 
  3608: 	#endif
  3609: 
  3610: 	int count = gServerInfo.m_SleepTimeConstA+((lpObj->Energy+lpObj->AddEnergy)/gServerInfo.m_SleepTimeConstB);
  3611: 
  3612: 	count = ((count>gServerInfo.m_SleepMaxTime)?gServerInfo.m_SleepMaxTime:count);
  3613: 
  3614: 	if((GetLargeRand()%100) >= rate)
  3615: 	{
  3616: 		gAttack.MissSend(lpObj,lpTarget,lpSkill,1,0);
```

### HIT: Number packing / SET_NUMBERH|SET_NUMBERL|MAKE_NUMBERW

- line 907: 	WORD index = MAKE_NUMBERW(((lpMsg[0]==0xFF&&lpMsg[2]==0x00)?0xFF:lpMsg[2]),lpMsg[0]);
- line 922: 	lpMsg[0] = SET_NUMBERLB(lpSkill->m_index);
- line 924: 	lpMsg[2] = SET_NUMBERHB(lpSkill->m_index);
- line 3345: 	pMsg.skill[0] = SET_NUMBERHB(lpSkill->m_index);
- line 3347: 	pMsg.skill[1] = SET_NUMBERLB(lpSkill->m_index);
- line 5080: 	pMsg.skill[0] = SET_NUMBERHB(lpSkill->m_index);
- line 5082: 	pMsg.skill[1] = SET_NUMBERLB(lpSkill->m_index);
- line 5458: 		gEffectManager.AddEffect(lpTarget,0,EFFECT_DEATH_STAB_ENHANCED,10,lpObj->Index,1,SET_NUMBERHW(damage),SET_NUMBERLW(damage));
- line 5552: 		if(damage > 0){gEffectManager.AddEffect(lpTarget,0,EFFECT_DRAIN_LIFE_ENHANCED,5,lpObj->Index,1,SET_NUMBERHW(damage),SET_NUMBERLW(damage));}
- line 5564: 	gEffectManager.AddEffect(lpTarget,0,gSkillManager.GetSkillEffect(lpSkill->m_index),5,lpObj->Index,1,SET_NUMBERHW(damage),SET_NUMBERLW(damage));
- line 5579: 	gEffectManager.AddEffect(lpTarget,0,gSkillManager.GetSkillEffect(lpSkill->m_index),5,lpObj->Index,1,SET_NUMBERHW(damage),SET_NUMBERLW(damage));
- line 5698: 		gEffectManager.AddEffect(lpTarget,0,EFFECT_CHAIN_DRIVER_ENHANCED,10,lpObj->Index,1,SET_NUMBERHW(damage),SET_NUMBERLW(damage));
- line 5721: 		gEffectManager.AddEffect(lpTarget,0,EFFECT_DRAGON_ROAR_ENHANCED,10,lpObj->Index,1,SET_NUMBERHW(damage),SET_NUMBERLW(damage));
- line 5825: 	int skill = MAKE_NUMBERW(lpMsg->skillH,lpMsg->skillL);

#### CONTEXT (first hit)

```
   837: 	for(int n=0;n < MAX_MASTER_SKILL_LIST;n++)
   838: 	{
   839: 		if(lpObj->MasterSkill[n].IsMasterSkill() == 0)
   840: 		{
   841: 			return ((lpObj->MasterSkill[n].Set(index,level)==0)?-1:n);
   842: 		}
   843: 	}
   844: 
   845: 	return -1;
   846: 
   847: 	#else
   848: 
   849: 	return -1;
   850: 
   851: 	#endif
   852: }
   853: 
   854: int CSkillManager::DelMasterSkill(LPOBJ lpObj,int index) // OK
   855: {
   856: 	#if(GAMESERVER_UPDATE>=401)
   857: 
   858: 	for(int n=0;n < MAX_MASTER_SKILL_LIST;n++)
   859: 	{
   860: 		if(lpObj->MasterSkill[n].IsMasterSkill() != 0)
   861: 		{
   862: 			if(lpObj->MasterSkill[n].m_index == index)
   863: 			{
   864: 				lpObj->MasterSkill[n].Clear();
   865: 				return n;
   866: 			}
   867: 		}
   868: 	}
   869: 
   870: 	return -1;
   871: 
   872: 	#else
   873: 
   874: 	return -1;
   875: 
   876: 	#endif
   877: }
   878: 
   879: CSkill* CSkillManager::GetMasterSkill(LPOBJ lpObj,int index) // OK
   880: {
   881: 	#if(GAMESERVER_UPDATE>=401)
   882: 
   883: 	index = ((index==SKILL_NOVA_START)?SKILL_NOVA:index);
   884: 
   885: 	for(int n=0;n < MAX_MASTER_SKILL_LIST;n++)
   886: 	{
   887: 		if(lpObj->MasterSkill[n].IsMasterSkill() != 0)
   888: 		{
   889: 			if(lpObj->MasterSkill[n].m_index == index || lpObj->MasterSkill[n].m_skill == index)
   890: 			{
   891: 				return &lpObj->MasterSkill[n];
   892: 			}
   893: 		}
   894: 	}
   895: 
   896: 	return 0;
   897: 
   898: 	#else
   899: 
   900: 	return 0;
   901: 
   902: 	#endif
   903: }
   904: 
   905: bool CSkillManager::ConvertSkillByte(CSkill* lpSkill,BYTE* lpMsg) // OK
   906: {
   907: 	WORD index = MAKE_NUMBERW(((lpMsg[0]==0xFF&&lpMsg[2]==0x00)?0xFF:lpMsg[2]),lpMsg[0]);
   908: 
   909: 	if(index == 0xFFFF)
   910: 	{
   911: 		return 0;
   912: 	}
   913: 
   914: 	lpSkill->m_level = lpMsg[1];
   915: 	lpSkill->m_skill = index;
   916: 	lpSkill->m_index = index;
   917: 	return 1;
   918: }
   919: 
   920: void CSkillManager::SkillByteConvert(BYTE* lpMsg,CSkill* lpSkill) // OK
   921: {
   922: 	lpMsg[0] = SET_NUMBERLB(lpSkill->m_index);
   923: 	lpMsg[1] = lpSkill->m_level;
   924: 	lpMsg[2] = SET_NUMBERHB(lpSkill->m_index);
   925: }
   926: 
   927: void CSkillManager::UseAttackSkill(int aIndex,int bIndex,CSkill* lpSkill) // OK
   928: {
   929: 	LPOBJ lpObj = &gObj[aIndex];
   930: 
   931: 	if(lpObj->Type == OBJECT_USER && this->CheckSkillRequireWeapon(lpObj,lpSkill->m_skill) == 0)
   932: 	{
   933: 		return;
   934: 	}
   935: 
   936: 	if(lpObj->Type == OBJECT_USER && lpSkill->m_skill == SKILL_TWISTING_SLASH && lpObj->Inventory[0].IsItem() == 0)
   937: 	{
   938: 		return;
   939: 	}
   940: 
   941: 	bool combo = 0;
   942: 
   943: 	if((gServerInfo.m_EnableComboToAllSwitch == 1 || gQuest.CheckQuestListState(lpObj,3,QUEST_FINISH) != 0) && lpObj->ComboSkill.CheckCombo(lpSkill->m_skill) != 0)
   944: 	{
   945: 		if(gServerInfo.m_CheckAutoComboHack == 0 || (GetTickCount()-lpObj->ComboTime) > ((DWORD)gServerInfo.m_CheckAutoComboHackTolerance))
   946: 		{
   947: 			combo = 1;
   948: 			lpObj->ComboTime = GetTickCount();
   949: 		}
   950: 
   951: 	}
   952: 	//LogAdd(LOG_ALERT,"%d",lpSkill->m_skill);
   953: 
   954: 	if(lpObj->SkillNovaState != 0 && lpSkill->m_skill != SKILL_NOVA && this->GetSkill(lpObj,SKILL_NOVA) != 0)
   955: 	{
   956: 		this->RunningSkill(aIndex,0,this->GetSkill(lpObj,SKILL_NOVA),(BYTE)lpObj->X,(BYTE)lpObj->Y,0,combo);
   957: 		return;
   958: 	}
   959: 
   960: 	#if(GAMESERVER_UPDATE>=602)
   961: 	if(lpSkill->m_skill == SKILL_NOVA && bIndex != SKILL_NOVA_START && bIndex != MASTER_SKILL_ADD_NOVA_START_IMPROVED)
   962: 	#else
   963: 	if(lpSkill->m_skill == SKILL_NOVA && bIndex != SKILL_NOVA_START)
   964: 	#endif
   965: 	{
   966: 		this->RunningSkill(aIndex,bIndex,lpSkill,(BYTE)lpObj->X,(BYTE)lpObj->Y,0,combo);
   967: 		return;
   968: 	}
   969: 
   970: 	if((lpSkill->m_skill == SKILL_UPPER_BEAST || lpSkill->m_skill == SKILL_CHAIN_DRIVER || lpSkill->m_skill == SKILL_DARK_SIDE) && lpObj->RageFighterSkillIndex == lpSkill->m_index && (lpObj->RageFighterSkillCount++) > 0)
   971: 	{
   972: 		this->RunningSkill(aIndex,bIndex,lpSkill,(BYTE)lpObj->X,(BYTE)lpObj->Y,0,combo);
   973: 		return;
   974: 	}
   975: 
   976: 	if(lpObj->Type != OBJECT_USER || (this->CheckSkillMana(lpObj,lpSkill->m_index) != 0 && this->CheckSkillBP(lpObj,lpSkill->m_index) != 0))
   977: 	{
   978: 		if(this->RunningSkill(aIndex,bIndex,lpSkill,(BYTE)lpObj->X,(BYTE)lpObj->Y,0,combo) != 0 && lpObj->Type == OBJECT_USER)
   979: 		{
   980: 			lpObj->Mana -= ((this->GetSkillMana(lpSkill->m_index)*lpObj->MPConsumptionRate)/100);
   981: 			lpObj->BP -= ((this->GetSkillBP(lpSkill->m_index)*lpObj->BPConsumptionRate)/100);
   982: 			GCManaSend(aIndex,0xFF,(int)lpObj->Mana,lpObj->BP);
   983: 		}
   984: 	}
   985: }
   986: 
   987: void CSkillManager::UseDurationSkillAttack(int aIndex,int bIndex,CSkill* lpSkill,BYTE x,BYTE y,BYTE dir,BYTE angle) // OK
   988: {
   989: 	LPOBJ lpObj = &gObj[aIndex];
   990: 
   991: 	if(lpObj->Type == OBJECT_USER && this->CheckSkillRequireWeapon(lpObj,lpSkill->m_skill) == 0)
   992: 	{
   993: 		return;
   994: 	}
   995: 
   996: 	if(lpObj->Type == OBJECT_USER && lpSkill->m_skill == SKILL_TWISTING_SLASH && lpObj->Inventory[0].IsItem() == 0)
   997: 	{
   998: 		return;
   999: 	}
  1000: 
  1001: 	bool combo = 0;
  1002: 
  1003: 
  1004: 	if(lpSkill->m_skill != SKILL_FLAME && lpSkill->m_skill != SKILL_TWISTER && lpSkill->m_skill != SKILL_EVIL_SPIRIT && lpSkill->m_skill != SKILL_HELL_FIRE && lpSkill->m_skill != SKILL_AQUA_BEAM && lpSkill->m_skill != SKILL_BLAST && lpSkill->m_skill != SKILL_INFERNO && lpSkill->m_skill != SKILL_TRIPLE_SHOT && lpSkill->m_skill != SKILL_IMPALE && lpSkill->m_skill != SKILL_MONSTER_AREA_ATTACK && lpSkill->m_skill != SKILL_PENETRATION && lpSkill->m_skill != SKILL_FIRE_SLASH && lpSkill->m_skill != SKILL_FIRE_SCREAM)
  1005: 	{
  1006: 		if((gServerInfo.m_EnableComboToAllSwitch == 1 || gQuest.CheckQuestListState(lpObj,3,QUEST_FINISH) != 0) && lpObj->ComboSkill.CheckCombo(lpSkill->m_skill) != 0)
  1007: 		{
  1008: 			if(gServerInfo.m_CheckAutoComboHack == 0 || (GetTickCount()-lpObj->ComboTime) > ((DWORD)gServerInfo.m_CheckAutoComboHackTolerance))
  1009: 			{
  1010: 				combo = 1;
  1011: 				lpObj->ComboTime = GetTickCount();
  1012: 			}
  1013: 		}
  1014: 	}
  1015: 
  1016: 	if(lpObj->SkillNovaState != 0 && lpSkill->m_skill != SKILL_NOVA && this->GetSkill(lpObj,SKILL_NOVA) != 0)
  1017: 	{
  1018: 		this->RunningSkill(aIndex,0,this->GetSkill(lpObj,SKILL_NOVA),x,y,0,combo);
  1019: 		return;
  1020: 	}
  1021: 
  1022: 	this->GCDurationSkillAttackSend(lpObj,lpSkill->m_index,x,y,dir);
  1023: 
  1024: 	if(lpObj->Type != OBJECT_USER || (this->CheckSkillMana(lpObj,lpSkill->m_index) != 0 && this->CheckSkillBP(lpObj,lpSkill->m_index) != 0))
  1025: 	{
  1026: 		if(this->RunningSkill(aIndex,bIndex,lpSkill,x,y,angle,combo) != 0 && lpObj->Type == OBJECT_USER)
  1027: 		{
  1028: 			lpObj->Mana -= ((this->GetSkillMana(lpSkill->m_index)*lpObj->MPConsumptionRate)/100);
  1029: 			lpObj->BP -= ((this->GetSkillBP(lpSkill->m_index)*lpObj->BPConsumptionRate)/100);
  1030: 			GCManaSend(aIndex,0xFF,(int)lpObj->Mana,lpObj->BP);
  1031: 		}
  1032: 	}
  1033: 
  1034: }
  1035: 
  1036: bool CSkillManager::RunningSkill(int aIndex,int bIndex,CSkill* lpSkill,BYTE x,BYTE y,BYTE angle,bool combo) // OK
  1037: {
  1038: 	LPOBJ lpObj = &gObj[aIndex];
  1039: 
  1040: 	lpObj->ShieldDamageReductionTime = 0;
  1041: 
  1042: 	gEffectManager.DelEffect(lpObj,EFFECT_INVISIBILITY);
  1043: 
  1044: 	switch(lpSkill->m_skill)
  1045: 	{
  1046: 		case SKILL_FLAME:
  1047: 			return this->MultiSkillAttack(aIndex,bIndex,lpSkill,combo);
  1048: 		case SKILL_TWISTER:
  1049: 			return this->MultiSkillAttack(aIndex,bIndex,lpSkill,combo);
  1050: 		case SKILL_EVIL_SPIRIT:
  1051: 			return this->MultiSkillAttack(aIndex,bIndex,lpSkill,combo);
  1052: 		case SKILL_HELL_FIRE:
  1053: 			return this->MultiSkillAttack(aIndex,bIndex,lpSkill,combo);
  1054: 		case SKILL_AQUA_BEAM:
  1055: 			return this->MultiSkillAttack(aIndex,bIndex,lpSkill,combo);
  1056: 		case SKILL_BLAST:
  1057: 			return this->MultiSkillAttack(aIndex,bIndex,lpSkill,combo);
  1058: 		case SKILL_INFERNO:
  1059: 			return this->MultiSkillAttack(aIndex,bIndex,lpSkill,combo);
  1060: 		case SKILL_MANA_SHIELD:
  1061: 			return this->SkillManaShield(aIndex,bIndex,lpSkill);
  1062: 		case SKILL_DEFENSE:
  1063: 			return this->SkillDefense(aIndex,bIndex,lpSkill);
  1064: 		case SKILL_TRIPLE_SHOT:
  1065: 			return this->MultiSkillAttack(aIndex,bIndex,lpSkill,combo);
  1066: 		case SKILL_HEAL:
  1067: 			return this->SkillHeal(aIndex,bIndex,lpSkill);
  1068: 		case SKILL_GREATER_DEFENSE:
  1069: 			return this->SkillGreaterDefense(aIndex,bIndex,lpSkill);
  1070: 		case SKILL_GREATER_DAMAGE:
  1071: 			return this->SkillGreaterDamage(aIndex,bIndex,lpSkill);
  1072: 		case SKILL_SUMMON1:
  1073: 			return this->SkillSummon(aIndex,bIndex,lpSkill);
  1074: 		case SKILL_SUMMON2:
  1075: 			return this->SkillSummon(aIndex,bIndex,lpSkill);
  1076: 		case SKILL_SUMMON3:
  1077: 			return this->SkillSummon(aIndex,bIndex,lpSkill);
  1078: 		case SKILL_SUMMON4:
  1079: 			return this->SkillSummon(aIndex,bIndex,lpSkill);
  1080: 		case SKILL_SUMMON5:
  1081: 			return this->SkillSummon(aIndex,bIndex,lpSkill);
  1082: 		case SKILL_SUMMON6:
  1083: 			return this->SkillSummon(aIndex,bIndex,lpSkill);
  1084: 		case SKILL_SUMMON7:
  1085: 			return this->SkillSummon(aIndex,bIndex,lpSkill);
  1086: 		case SKILL_DECAY:
  1087: 			return this->SkillDecay(aIndex,bIndex,lpSkill,x,y,combo);
  1088: 		case SKILL_ICE_STORM:
  1089: 			return this->SkillIceStorm(aIndex,bIndex,lpSkill,x,y,combo);
  1090: 		case SKILL_NOVA:
  1091: 			return this->SkillNova(aIndex,bIndex,lpSkill,combo);
  1092: 		case SKILL_TWISTING_SLASH:
  1093: 			return this->SkillTwistingSlash(aIndex,bIndex,lpSkill,combo);
  1094: 		case SKILL_RAGEFUL_BLOW:
  1095: 			return this->SkillRagefulBlow(aIndex,bIndex,lpSkill,combo);
  1096: 		case SKILL_DEATH_STAB:
  1097: 			return this->SkillDeathStab(aIndex,bIndex,lpSkill,combo);
  1098: 		case SKILL_CRESCENT_MOON_SLASH:
  1099: 			return this->SkillCastleSiege(aIndex,bIndex,lpSkill,combo);
  1100: 		case SKILL_MANA_GLAIVE:
  1101: 			return this->SkillCastleSiege(aIndex,bIndex,lpSkill,combo);
  1102: 		case SKILL_STAR_FALL:
  1103: 			return this->SkillCastleSiege(aIndex,bIndex,lpSkill,combo);
  1104: 		case SKILL_IMPALE:
  1105: 			return this->MultiSkillAttack(aIndex,bIndex,lpSkill,combo);
  1106: 		case SKILL_GREATER_LIFE:
  1107: 			return this->SkillGreaterLife(aIndex,bIndex,lpSkill);
  1108: 		case SKILL_MONSTER_AREA_ATTACK:
  1109: 			return this->SkillMonsterAreaAttack(aIndex,bIndex,lpSkill);
  1110: 		case SKILL_PENETRATION:
  1111: 			return this->MultiSkillAttack(aIndex,bIndex,lpSkill,combo);
  1112: 		case SKILL_FIRE_SLASH:
  1113: 			return this->MultiSkillAttack(aIndex,bIndex,lpSkill,combo);
  1114: 		case SKILL_POWER_SLASH:
  1115: 			return this->SkillPowerSlash(aIndex,bIndex,lpSkill,angle,combo);
  1116: 		case SKILL_SPIRAL_SLASH:
  1117: 			return this->SkillCastleSiege(aIndex,bIndex,lpSkill,combo);
  1118: 		case SKILL_FORCE:
  1119: 			return this->SkillForce(aIndex,bIndex,lpSkill,combo);
  1120: 		case SKILL_FIRE_BURST:
  1121: 			return this->SkillFireBurst(aIndex,bIndex,lpSkill,combo);
  1122: 		case SKILL_EARTHQUAKE:
  1123: 			return this->SkillEarthquake(aIndex,bIndex,lpSkill,combo);
  1124: 		case SKILL_SUMMON_PARTY:
  1125: 			return this->SkillSummonParty(aIndex,bIndex,lpSkill);
  1126: 		case SKILL_GREATER_CRITICAL_DAMAGE:
  1127: 			return this->SkillGreaterCriticalDamage(aIndex,bIndex,lpSkill);
  1128: 		case SKILL_ELECTRIC_SPARK:
  1129: 			return this->SkillElectricSpark(aIndex,bIndex,lpSkill,angle,combo);
  1130: 		case SKILL_STERN:
  1131: 			return this->SkillStern(aIndex,bIndex,lpSkill,angle);
  1132: 		case SKILL_REMOVE_STERN:
  1133: 			return this->SkillRemoveStern(aIndex,bIndex,lpSkill);
  1134: 		case SKILL_GREATER_MANA:
  1135: 			return this->SkillGreaterMana(aIndex,bIndex,lpSkill);
  1136: 		case SKILL_INVISIBILITY:
  1137: 			return this->SkillInvisibility(aIndex,bIndex,lpSkill);
  1138: 		case SKILL_REMOVE_INVISIBILITY:
  1139: 			return this->SkillRemoveInvisibility(aIndex,bIndex,lpSkill);
  1140: 		case SKILL_REMOVE_ALL_EFFECT:
  1141: 			return this->SkillRemoveAllEffect(aIndex,bIndex,lpSkill);
  1142: 		case SKILL_MANA_RAYS:
  1143: 			return this->SkillCastleSiege(aIndex,bIndex,lpSkill,combo);
  1144: 		case SKILL_FIRE_BLAST:
  1145: 			return this->SkillCastleSiege(aIndex,bIndex,lpSkill,combo);
  1146: 		case SKILL_BRAND:
  1147: 			return this->SkillBrand(aIndex,bIndex,lpSkill);
```

### HIT: GS dispatch anchor / case\s+0x19\s*:|CGSkillAttackRecv

- line 5956: void CSkillManager::CGSkillAttackRecv(PMSG_SKILL_ATTACK_RECV* lpMsg,int aIndex) // OK

#### CONTEXT (first hit)

```
  5886: 		if(lpObj->Inventory[0].m_Index < GET_ITEM(4,0) || lpObj->Inventory[0].m_Index >= GET_ITEM(5,0))
  5887: 		{
  5888: 			return;
  5889: 		}
  5890: 
  5891: 		if(lpObj->Inventory[1].m_Index < GET_ITEM(4,0) || lpObj->Inventory[1].m_Index >= GET_ITEM(5,0))
  5892: 		{
  5893: 			return;
  5894: 		}
  5895: 	}
  5896: 
  5897: 	lpMsg->count = ((lpMsg->count>5)?5:lpMsg->count);
  5898: 
  5899: 	int combo = 0;
  5900: 
  5901: 	if((gServerInfo.m_EnableComboToAllSwitch == 1 || gQuest.CheckQuestListState(lpObj,3,QUEST_FINISH) != 0) && lpObj->ComboSkill.CheckCombo(lpSkill->m_skill) != 0)
  5902: 	{
  5903: 		if(gServerInfo.m_CheckAutoComboHack == 0 || (GetTickCount()-lpObj->ComboTime) > ((DWORD)gServerInfo.m_CheckAutoComboHackTolerance))
  5904: 		{
  5905: 			combo = 1;
  5906: 			lpObj->ComboTime = GetTickCount();
  5907: 		}
  5908: 	}
  5909: 
  5910: 	for(int n=0;n < lpMsg->count;n++)
  5911: 	{
  5912: 		PMSG_MULTI_SKILL_ATTACK* lpInfo = (PMSG_MULTI_SKILL_ATTACK*)(((BYTE*)lpMsg)+sizeof(PMSG_MULTI_SKILL_ATTACK_RECV)+(sizeof(PMSG_MULTI_SKILL_ATTACK)*n));
  5913: 
  5914: 		#if(GAMESERVER_UPDATE>=701)
  5915: 
  5916: 		int bIndex = MAKE_NUMBERW(lpInfo->indexH,lpInfo->indexL);
  5917: 
  5918: 		#else
  5919: 
  5920: 		int bIndex = MAKE_NUMBERW(lpInfo->index[0],lpInfo->index[1]);
  5921: 
  5922: 		#endif
  5923: 
  5924: 		if(OBJECT_RANGE(bIndex) == 0)
  5925: 		{
  5926: 			continue;
  5927: 		}
  5928: 
  5929: 		LPOBJ lpTarget = &gObj[bIndex];
  5930: 
  5931: 		if(lpObj->Type == OBJECT_USER && this->CheckSkillRadio(lpSkill->m_index,lpObj->X,lpObj->Y,lpTarget->X,lpTarget->Y) == 0)
  5932: 		{
  5933: 			continue;
  5934: 		}
  5935: 
  5936: 		if(type != 0)
  5937: 		{
  5938: 			gAttack.Attack(lpObj,lpTarget,lpSkill,1,1,0,0,combo);
  5939: 		}
  5940: 		else if(lpSkill->m_skill == SKILL_FIRE_SLASH)
  5941: 		{
  5942: 			gAttack.Attack(lpObj,lpTarget,lpSkill,1,1,0,0,combo);
  5943: 		}
  5944: 		else
  5945: 		{
  5946: 			gAttack.Attack(lpObj,lpTarget,lpSkill,0,1,0,0,combo);
  5947: 		}
  5948: 
  5949: 		if(combo != 0)
  5950: 		{
  5951: 			this->GCSkillAttackSend(lpObj,SKILL_COMBO,lpTarget->Index,1);
  5952: 		}
  5953: 	}
  5954: }
  5955: 
  5956: void CSkillManager::CGSkillAttackRecv(PMSG_SKILL_ATTACK_RECV* lpMsg,int aIndex) // OK
  5957: {
  5958: 	LPOBJ lpObj = &gObj[aIndex];
  5959: 
  5960: 	if(gObjIsConnectedGS(aIndex) == 0)
  5961: 	{
  5962: 		return;
  5963: 	}
  5964: 
  5965: 	if(lpObj->Live == 0)
  5966: 	{
  5967: 		return;
  5968: 	}
  5969: 
  5970: 	#if(GAMESERVER_UPDATE>=701)
  5971: 
  5972: 	int bIndex = MAKE_NUMBERW(lpMsg->indexH,lpMsg->indexL);
  5973: 
  5974: 	#else
  5975: 
  5976: 	int bIndex = MAKE_NUMBERW(lpMsg->index[0],lpMsg->index[1]);
  5977: 
  5978: 	#endif
  5979: 
  5980: 	if(OBJECT_RANGE(bIndex) == 0)
  5981: 	{
  5982: 		return;
  5983: 	}
  5984: 
  5985: 	LPOBJ lpTarget = &gObj[bIndex];
  5986: 
  5987: 	if(gObjIsConnectedGS(bIndex) == 0)
  5988: 	{
  5989: 		return;
  5990: 	}
  5991: 
  5992: 	if(lpTarget->Live == 0)
  5993: 	{
  5994: 		return;
  5995: 	}
  5996: 
  5997: 	if(gMap[lpObj->Map].CheckAttr(lpObj->X,lpObj->Y,1) != 0 || gMap[lpTarget->Map].CheckAttr(lpTarget->X,lpTarget->Y,1) != 0)
  5998: 	{
  5999: 		return;
  6000: 	}
  6001: 
  6002: 	#if(GAMESERVER_UPDATE>=402)
  6003: 
  6004: 	if(gDuel.GetDuelArenaBySpectator(aIndex) != 0 || gDuel.GetDuelArenaBySpectator(bIndex) != 0)
  6005: 	{
  6006: 		return;
  6007: 	}
  6008: 
  6009: 	#endif
  6010: 
  6011: 	#if(GAMESERVER_UPDATE>=701)
  6012: 
  6013: 	int skill = MAKE_NUMBERW(lpMsg->skillH,lpMsg->skillL);
  6014: 
  6015: 	#else
  6016: 
  6017: 	int skill = MAKE_NUMBERW(lpMsg->skill[0],lpMsg->skill[1]);
  6018: 
  6019: 	#endif
  6020: 
  6021: 	CSkill* lpSkill = 0;
  6022: 
  6023: 	if(gHackSkillCheck.CheckSpeedHack(lpObj,skill) == 1)
  6024: 	{
  6025: 		return;
  6026: 	}
  6027: 
  6028: 	if(lpObj->Type == OBJECT_USER)
  6029: 	{
  6030: 		lpSkill = this->GetSkill(lpObj,skill);
  6031: 
  6032: 		if(lpSkill == 0)
  6033: 		{
  6034: 			return;
  6035: 		}
  6036: 
  6037: 		if(this->CheckSkillDelay(lpObj,lpSkill->m_index) == 0)
  6038: 		{
  6039: 			return;
  6040: 		}
  6041: 
  6042: 		if(this->CheckSkillRequireKillPoint(lpObj,lpSkill->m_index) == 0)
  6043: 		{
  6044: 			return;
  6045: 		}
  6046: 
  6047: 		if(this->CheckSkillRequireGuildStatus(lpObj,lpSkill->m_index) == 0)
  6048: 		{
  6049: 			return;
  6050: 		}
  6051: 
  6052: 		if(this->CheckSkillRequireClass(lpObj,lpSkill->m_index) == 0)
  6053: 		{
  6054: 			return;
  6055: 		}
  6056: 	}
  6057: 	else
  6058: 	{
  6059: 		lpSkill = &lpObj->Skill[skill];
  6060: 
  6061: 		if(lpSkill->IsSkill() == 0)
  6062: 		{
  6063: 			return;
  6064: 		}
  6065: 	}
  6066: 
  6067: 	lpObj->MultiSkillIndex = 0;
  6068: 
  6069: 	lpObj->MultiSkillCount = 0;
  6070: 
  6071: 	#if(GAMESERVER_UPDATE>=602)
  6072: 	this->UseAttackSkill(aIndex,((skill==SKILL_NOVA_START||skill==MASTER_SKILL_ADD_NOVA_START_IMPROVED)?skill:bIndex),lpSkill);
  6073: 	#else
  6074: 	this->UseAttackSkill(aIndex,((skill==SKILL_NOVA_START)?skill:bIndex),lpSkill);
  6075: 	#endif
  6076: }
  6077: 
  6078: void CSkillManager::CGDurationSkillAttackRecv(PMSG_DURATION_SKILL_ATTACK_RECV* lpMsg,int aIndex) // OK
  6079: {
  6080: 	LPOBJ lpObj = &gObj[aIndex];
  6081: 
  6082: 	if(gObjIsConnectedGS(aIndex) == 0)
  6083: 	{
  6084: 		return;
  6085: 	}
  6086: 
  6087: 	if(lpObj->Live == 0)
  6088: 	{
  6089: 		return;
  6090: 	}
  6091: 
  6092: 	if(gMap[lpObj->Map].CheckAttr(lpObj->X,lpObj->Y,1) != 0)
  6093: 	{
  6094: 		return;
  6095: 	}
  6096: 
  6097: 	#if(GAMESERVER_UPDATE>=402)
  6098: 
  6099: 	if(gDuel.GetDuelArenaBySpectator(aIndex) != 0)
  6100: 	{
  6101: 		return;
  6102: 	}
  6103: 
  6104: 	#endif
  6105: 
  6106: 	#if(GAMESERVER_UPDATE>=701)
  6107: 
  6108: 	int skill = MAKE_NUMBERW(lpMsg->skillH,lpMsg->skillL);
  6109: 
  6110: 	#else
  6111: 
  6112: 	int skill = MAKE_NUMBERW(lpMsg->skill[0],lpMsg->skill[1]);
  6113: 
  6114: 	#endif
  6115: 
  6116: 	CSkill* lpSkill = 0;
  6117: 
  6118: 	if(gHackSkillCheck.CheckSpeedHack(lpObj,skill) == 1)
  6119: 	{
  6120: 		return;
  6121: 	}
  6122: 
  6123: 	if(lpObj->Type == OBJECT_USER)
  6124: 	{
  6125: 		lpSkill = this->GetSkill(lpObj,skill);
  6126: 
  6127: 		if(lpSkill == 0)
  6128: 		{
  6129: 			return;
  6130: 		}
  6131: 
  6132: 		if(this->CheckSkillDelay(lpObj,lpSkill->m_index) == 0)
  6133: 		{
  6134: 			return;
  6135: 		}
  6136: 
  6137: 		if(this->CheckSkillRequireKillPoint(lpObj,lpSkill->m_index) == 0)
  6138: 		{
  6139: 			return;
  6140: 		}
  6141: 
  6142: 		if(this->CheckSkillRequireGuildStatus(lpObj,lpSkill->m_index) == 0)
  6143: 		{
  6144: 			return;
  6145: 		}
  6146: 
  6147: 		if(this->CheckSkillRequireClass(lpObj,lpSkill->m_index) == 0)
  6148: 		{
  6149: 			return;
  6150: 		}
  6151: 	}
  6152: 	else
  6153: 	{
  6154: 		lpSkill = &lpObj->Skill[skill];
  6155: 
  6156: 		if(lpSkill->IsSkill() == 0)
  6157: 		{
  6158: 			return;
  6159: 		}
  6160: 	}
  6161: 
  6162: 	lpObj->MultiSkillIndex = 0;
  6163: 
  6164: 	lpObj->MultiSkillCount = 0;
  6165: 
  6166: 	#if(GAMESERVER_UPDATE>=701)
  6167: 
  6168: 	int bIndex = MAKE_NUMBERW(lpMsg->indexH,lpMsg->indexL);
  6169: 
  6170: 	#else
  6171: 
  6172: 	int bIndex = MAKE_NUMBERW(lpMsg->index[0],lpMsg->index[1]);
  6173: 
  6174: 	#endif
  6175: 
  6176: 	this->UseDurationSkillAttack(lpObj->Index,bIndex,lpSkill,lpMsg->x,lpMsg->y,lpMsg->dir,lpMsg->angle);
  6177: 
  6178: }
  6179: 
  6180: void CSkillManager::CGSkillCancelRecv(PMSG_SKILL_CANCEL_RECV* lpMsg,int aIndex) // OK
  6181: {
  6182: 	LPOBJ lpObj = &gObj[aIndex];
  6183: 
  6184: 	if(gObjIsConnectedGP(aIndex) == 0)
  6185: 	{
  6186: 		return;
  6187: 	}
  6188: 
  6189: 	gEffectManager.DelEffect(lpObj,this->GetSkillEffect(MAKE_NUMBERW(lpMsg->skill[0],lpMsg->skill[1])));
  6190: }
  6191: 
  6192: void CSkillManager::CGRageFighterSkillAttackRecv(PMSG_RAGE_FIGHTER_SKILL_ATTACK_RECV* lpMsg,int aIndex) // OK
  6193: {
  6194: 	#if(GAMESERVER_UPDATE>=601)
  6195: 
  6196: 	LPOBJ lpObj = &gObj[aIndex];
```


---

## FILE: SkillManager.h

### HIT: GS dispatch anchor / case\s+0x19\s*:|CGSkillAttackRecv

- line 557: 	void CGSkillAttackRecv(PMSG_SKILL_ATTACK_RECV* lpMsg,int aIndex);

#### CONTEXT (first hit)

```
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


---

## FILE: Protocol.cpp

### HIT: Any 0x19 mention / \b0x19\b

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

### HIT: Client-visible magic struct tokens / \bPMSG_\w*MAGIC\w*\b|\bMAGIC\b|\bSkill\b

- line 628: 				//	//CGReqITLMagicUse((PMSG_ITL_USE_MAGIC *)aRecv, aIndex);

#### CONTEXT (first hit)

```
   558: 				case 0x05:
   559: 					gIllusionTemple.CGIllusionTempleButtonClickRecv(aIndex);
   560: 					break;
   561: 				case 0x0B:
   562: 					#if(GAMESERVER_UPDATE>=402)
   563: 					gLuckyCoin.CGLuckyCoinCountRecv((PMSG_LUCKY_COIN_COUNT_RECV*)lpMsg,aIndex);
   564: 					#endif
   565: 					break;
   566: 				case 0x0C:
   567: 					#if(GAMESERVER_UPDATE>=402)
   568: 					gLuckyCoin.CGLuckyCoinRegisterRecv((PMSG_LUCKY_COIN_REGISTER_RECV*)lpMsg,aIndex);
   569: 					#endif
   570: 					break;
   571: 				case 0x0D:
   572: 					#if(GAMESERVER_UPDATE>=402)
   573: 					gLuckyCoin.CGLuckyCoinExchangeRecv((PMSG_LUCKY_COIN_EXCHANGE_RECV*)lpMsg,aIndex);
   574: 					#endif
   575: 					break;
   576: 				case 0x0E:
   577: 					#if(GAMESERVER_UPDATE>=501)
   578: 					gDoubleGoer.CGDoubleGoerEnterRecv((PMSG_DOUBLE_GOER_ENTER_RECV*)lpMsg,aIndex);
   579: 					#endif
   580: 					break;
   581: 				case 0x17:
   582: 					#if(GAMESERVER_UPDATE>=501)
   583: 					gNpcTalk.CGNpcJuliaRecv(aIndex);
   584: 					#endif
   585: 					break;
   586: 				case 0x20:
   587: 					#if(GAMESERVER_UPDATE>=501)
   588: 					gInventoryEquipment.CGInventoryEquipmentRecv((PMSG_INVENTORY_EQUIPMENT_RECV*)lpMsg,aIndex);
   589: 					#endif
   590: 					break;
   591: 				case 0x51:
   592: 					#if(GAMESERVER_UPDATE>=603)
   593: 					gHelper.CGHelperStartRecv((PMSG_HELPER_START_RECV*)lpMsg,aIndex);
   594: 					#endif
   595: 					break;
   596: 				//case 0x5A:
   597: 				//	#if(GAMESERVER_UPDATE>=803)
   598: 				//	//CGEnterCheckITL((PMSG_REQ_ENTER_ITL *)aRecv, aIndex);
   599: 				//	#endif
   600: 				//	break;
   601: 				//case 0x5B:
   602: 				//	#if(GAMESERVER_UPDATE>=803)
   603: 				//	//CGReqAcceptEnterITL((PMSG_REQ_ACCEPTENTER *)aRecv, aIndex);
   604: 				//	#endif
   605: 				//	break;
   606: 				//case 0x5C:
   607: 				//	#if(GAMESERVER_UPDATE>=803)
   608: 				//	//CGReqITLSchedule(aIndex);
   609: 				//	#endif
   610: 				//	break;
   611: 				//case 0x5D:
   612: 				//	#if(GAMESERVER_UPDATE>=803)
   613: 				//	//CGReqITLGuild_LeagueRank(aIndex);
   614: 				//	#endif
   615: 				//	break;
   616: 				//case 0x5E:
   617: 				//	#if(GAMESERVER_UPDATE>=803)
   618: 				//	//CGReqITLTournamentRank(aIndex);
   619: 				//	#endif
   620: 				//	break;
   621: 				//case 0x5F:
   622: 				//	#if(GAMESERVER_UPDATE>=803)
   623: 				//	//CGReq_ITL_ReEnter(aIndex);
   624: 				//	#endif
   625: 				//	break;
   626: 				//case 0x67:
   627: 				//	#if(GAMESERVER_UPDATE>=803)
   628: 				//	//CGReqITLMagicUse((PMSG_ITL_USE_MAGIC *)aRecv, aIndex);
   629: 				//	#endif
   630: 				//	break;
   631: 				//case 0x68:
   632: 				//	#if(GAMESERVER_UPDATE>=803)
   633: 				//	//CGReq_RewardList(aIndex);
   634: 				//	#endif
   635: 				//	break;
   636: 				//case 0x69:
   637: 				//	#if(GAMESERVER_UPDATE>=803)
   638: 				//	//CGReq_ITLGetReward(aIndex);
   639: 				//	#endif
   640: 				//	break;
   641: 				//case 0x6A:
   642: 				//	#if(GAMESERVER_UPDATE>=803) //esse
   643: 				//	//CGReq_ITL_Relics((PMSG_REQ_ITL_RELATE_RELICS *)aRecv, aIndex);
   644: 				//	#endif
   645: 				//	break;
   646: 				//case 0x6B:
   647: 				//	#if(GAMESERVER_UPDATE>=803)
   648: 				//	//CGReq_ITL_Reward_Gem((PMSG_ITL_REQ_REWARD *)aRecv, aIndex);
   649: 				//	#endif
   650: 				//	break;
   651: 				case 0x70:
   652: 					#if(GAMESERVER_UPDATE >= 803) //esse
   653: 					gIllusionTemple.CGIllusionTempleEnterRecv((PMSG_ILLUSION_TEMPLE_ENTER_RECV*)lpMsg,aIndex); //Original
   654: 					#endif
   655: 					break;
   656: 				default:
   657: 					LogAdd(LOG_RED, "[0xBF] Unknown packet BF:%d",lpMsg[3]);
   658: 					break;
   659: 			}
   660: 			break;
   661: 		case 0xC0:
   662: 			FriendListRequest(aIndex);
   663: 			break;
   664: 		case 0xC1:
   665: 			FriendAddRequest((PMSG_FRIEND_ADD_REQ*)lpMsg,aIndex);
   666: 			break;
   667: 		case 0xC2:
   668: 			WaitFriendAddRequest((PMSG_FRIEND_ADD_SIN_RESULT*)lpMsg,aIndex);
   669: 			break;
   670: 		case 0xC3:
   671: 			FriendDelRequest((PMSG_FRIEND_DEL_REQ*)lpMsg,aIndex);
   672: 			break;
   673: 		case 0xC4:
   674: 			FriendStateClientRecv((PMSG_FRIEND_STATE_C*)lpMsg,aIndex);
   675: 			break;
   676: 		case 0xC5:
   677: 			FriendMemoSend((PMSG_FRIEND_MEMO*)lpMsg,aIndex);
   678: 			break;
   679: 		case 0xC7:
   680: 			FriendMemoReadReq((PMSG_FRIEND_READ_MEMO_REQ*)lpMsg,aIndex);
   681: 			break;
   682: 		case 0xC8:
   683: 			FriendMemoDelReq((PMSG_FRIEND_MEMO_DEL_REQ*)lpMsg,aIndex);
   684: 			break;
   685: 		case 0xC9:
   686: 			FriendMemoListReq(aIndex);
   687: 			break;
   688: 		case 0xCA:
   689: 			FriendChatRoomCreateReq((PMSG_FRIEND_ROOMCREATE_REQ*)lpMsg,aIndex);
   690: 			break;
   691: 		case 0xCB:
   692: 			FriendRoomInvitationReq((PMSG_ROOM_INVITATION*)lpMsg,aIndex);
   693: 			break;
   694: 		case 0xD0:
   695: 			switch(lpMsg[3])
   696: 			{
   697: 				case 0x05:
   698: 					#if(GAMESERVER_UPDATE<=402)
   699: 					gPcPoint.CGPcPointItemBuyRecv((PMSG_PC_POINT_ITEM_BUY_RECV*)lpMsg,aIndex);
   700: 					#endif
   701: 					break;
   702: 				case 0x06:
   703: 					#if(GAMESERVER_UPDATE<=402)
   704: 					gPcPoint.CGPcPointOpenRecv((PMSG_PC_POINT_OPEN_RECV*)lpMsg,aIndex);
   705: 					#endif
   706: 					break;
   707: 				case 0x07:
   708: 					#if(GAMESERVER_UPDATE>=301)
   709: 					gQuest.CGQuestNpcWarewolfRecv(aIndex);
   710: 					#endif
   711: 					break;
   712: 				case 0x08:
   713: 					#if(GAMESERVER_UPDATE>=301)
   714: 					gQuest.CGQuestNpcKeeperRecv(aIndex);
   715: 					#endif
   716: 					break;
   717: 				case 0x09:
   718: 					#if(GAMESERVER_UPDATE>=202)
   719: 					gNpcTalk.CGNpcLeoTheHelperRecv(aIndex);
   720: 					#endif
   721: 					break;
   722: 				case 0x0A:
   723: 					#if(GAMESERVER_UPDATE>=401)
   724: 					gNpcTalk.CGNpcSnowmanRecv(aIndex);
   725: 					#endif
   726: 					break;
   727: 				case 0x10:
   728: 					#if(GAMESERVER_UPDATE>=401)
   729: 					gNpcTalk.CGNpcSantaClausRecv(aIndex);
   730: 					#endif
   731: 					break;
   732: 			}
   733: 			break;
   734: 		case 0xD1:
   735: 			switch(lpMsg[3])
   736: 			{
   737: 				case 0x00:
   738: 					gKanturuEntranceNPC.CGKanturuEnterInfoRecv(aIndex);
   739: 					break;
   740: 				case 0x01:
   741: 					gKanturuEntranceNPC.CGKanturuEnterRecv(aIndex);
   742: 					break;
   743: 			}
   744: 			break;
   745: 		case 0xD2:
   746: 			switch(lpMsg[3])
   747: 			{
   748: 				case 0x01:
   749: 					gCashShop.CGCashShopPointRecv(aIndex);
   750: 					break;
   751: 				case 0x02:
   752: 					gCashShop.CGCashShopOpenRecv((PMSG_CASH_SHOP_OPEN_RECV*)lpMsg,aIndex);
   753: 					break;
   754: 				case 0x03:
   755: 					gCashShop.CGCashShopItemBuyRecv((PMSG_CASH_SHOP_ITEM_BUY_RECV*)lpMsg,aIndex);
   756: 					break;
   757: 				case 0x04:
   758: 					gCashShop.CGCashShopItemGifRecv((PMSG_CASH_SHOP_ITEM_GIF_RECV*)lpMsg,aIndex);
   759: 					break;
   760: 				case 0x05:
   761: 					gCashShop.CGCashShopItemNumRecv((PMSG_CASH_SHOP_ITEM_NUM_RECV*)lpMsg,aIndex);
   762: 					break;
   763: 				case 0x0B:
   764: 					gCashShop.CGCashShopItemUseRecv((PMSG_CASH_SHOP_ITEM_USE_RECV*)lpMsg,aIndex);
   765: 					break;
   766: 			}
   767: 			break;
   768: 		case PROTOCOL_CODE1:
   769: 			CGMoveRecv((PMSG_MOVE_RECV*)lpMsg,aIndex);
   770: 			break;
   771: 		case PROTOCOL_CODE4:
   772: 			gSkillManager.CGMultiSkillAttackRecv((PMSG_MULTI_SKILL_ATTACK_RECV*)lpMsg,aIndex,0);
   773: 			break;
   774: 		case 0xE1:
   775: 			gGuild.CGGuildAssignStatusRecv((PMSG_GUILD_ASSIGN_STATUS_RECV*)lpMsg,aIndex);
   776: 			break;
   777: 		case 0xE2:
   778: 			CGGuildAssignType((PMSG_GUILD_ASSIGN_TYPE_REQ*)lpMsg,aIndex);
   779: 			break;
   780: 		case 0xE5:
   781: 			CGRelationShipReqJoinBreakOff((PMSG_RELATIONSHIP_JOIN_BREAKOFF_REQ*)lpMsg,aIndex);
   782: 			break;
   783: 		case 0xE6:
   784: 			CGRelationShipAnsJoinBreakOff((PMSG_RELATIONSHIP_JOIN_BREAKOFF_ANS*)lpMsg,aIndex);
   785: 			break;
   786: 		case 0xE7:
   787: 			switch(lpMsg[3])
   788: 			{
   789: 				case 0x01:
   790: 					#if(GAMESERVER_UPDATE>=802)
   791: 					gMiniMap.CGMiniMapStartPartyInfoRecv(aIndex);
   792: 					#endif
   793: 					break;
   794: 				case 0x02:
   795: 					#if(GAMESERVER_UPDATE>=802)
   796: 					gMiniMap.CGMiniMapClosePartyInfoRecv(aIndex);
   797: 					#endif
   798: 					break;
   799: 				case 0x03:
   800: 					#if(GAMESERVER_UPDATE>=802)
   801: 					gMiniMap.CGMiniMapInfoRecv((PMSG_MINI_MAP_INFO_RECV*)lpMsg,aIndex);
   802: 					#endif
   803: 					break;
   804: 			}
   805: 			break;
   806: 		case 0xE9:
   807: 			CGUnionList((PMSG_UNIONLIST_REQ*)lpMsg,aIndex);
   808: 			break;
   809: 		case 0xEB:
   810: 			switch(lpMsg[3])
   811: 			{
   812: 				case 0x01:
   813: 					CGRelationShipReqKickOutUnionMember((PMSG_KICKOUT_UNIONMEMBER_REQ*)lpMsg,aIndex);
   814: 					break;
   815: 			}
   816: 			break;
   817: 		case 0xEC:
   818: 			switch(lpMsg[3])
   819: 			{
   820: 				case 0x00:
   821: 					#if(GAMESERVER_UPDATE>=701)
   822: 					gPentagramSystem.CGPentagramJewelInsertRecv((PMSG_PENTAGRAM_JEWEL_INSERT_RECV*)lpMsg,aIndex);
   823: 					#endif
   824: 					break;
   825: 				case 0x01:
   826: 					#if(GAMESERVER_UPDATE>=701)
   827: 					gPentagramSystem.CGPentagramJewelRemoveRecv((PMSG_PENTAGRAM_JEWEL_REMOVE_RECV*)lpMsg,aIndex);
   828: 					#endif
   829: 					break;
   830: 				case 0x02:
   831: 					#if(GAMESERVER_UPDATE>=701)
   832: 					gPentagramSystem.CGPentagramJewelRefineRecv((PMSG_PENTAGRAM_JEWEL_REFINE_RECV*)lpMsg,aIndex);
   833: 					#endif
   834: 					break;
   835: 				case 0x03:
   836: 					#if(GAMESERVER_UPDATE>=701)
   837: 					gPentagramSystem.CGPentagramJewelUpgradeRecv((PMSG_PENTAGRAM_JEWEL_UPGRADE_RECV*)lpMsg,aIndex);
   838: 					#endif
   839: 					break;
   840: 				case 0x31:
   841: 					#if(GAMESERVER_UPDATE>=802)
   842: 					gPersonalShop.CGPShopSearchRecv((PMSG_PSHOP_SEARCH_RECV*)lpMsg,aIndex);
   843: 					#endif
   844: 					break;
   845: 				case 0x33:
   846: 					#if(GAMESERVER_UPDATE>=802)
   847: 					gPersonalShop.CGPShopSearchLogRecv((PMSG_PSHOP_SEARCH_LOG_RECV*)lpMsg,aIndex);
   848: 					#endif
   849: 					break;
   850: 			}
   851: 			break;
   852: 		case 0xED:
   853: 			switch(lpMsg[3])
   854: 			{
   855: 				case 0x00:
   856: 					#if(GAMESERVER_UPDATE>=801)
   857: 					gGuildMatching.CGGuildMatchingListRecv((PMSG_GUILD_MATCHING_LIST_RECV*)lpMsg,aIndex);
   858: 					#endif
   859: 					break;
   860: 				case 0x01:
   861: 					#if(GAMESERVER_UPDATE>=801)
   862: 					gGuildMatching.CGGuildMatchingListSearchRecv((PMSG_GUILD_MATCHING_LIST_SEARCH_RECV*)lpMsg,aIndex);
   863: 					#endif
   864: 					break;
   865: 				case 0x02:
   866: 					#if(GAMESERVER_UPDATE>=801)
   867: 					gGuildMatching.CGGuildMatchingInsertRecv((PMSG_GUILD_MATCHING_INSERT_RECV*)lpMsg,aIndex);
   868: 					#endif
```

### HIT: Send APIs / \bDataSend\b|\bMsgSendV2\b|\bsend\(|\bGCSend\b

- line 1224: 					DataSend(gParty.m_PartyInfo[lpObj->PartyNumber].Index[n],(BYTE*)&pMsg,pMsg.header.size);
- line 1264: 					DataSend(n,(BYTE*)&pMsg,pMsg.header.size);
- line 1271: 		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
- line 1272: 		MsgSendV2(lpObj,(BYTE*)&pMsg,pMsg.header.size);
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

### HIT: Number packing / SET_NUMBERH|SET_NUMBERL|MAKE_NUMBERW

- line 1426: 	pMsg.index[0] = SET_NUMBERHB(aIndex);
- line 1428: 	pMsg.index[1] = SET_NUMBERLB(aIndex);
- line 1496: 	pMsg.index[0] = SET_NUMBERHB(aIndex);
- line 1498: 	pMsg.index[1] = SET_NUMBERLB(aIndex);
- line 1649: 	int bIndex = MAKE_NUMBERW(lpMsg->index[0],lpMsg->index[1]);
- line 1923: 	pMsg.index[0] = SET_NUMBERHB(aIndex);
- line 1925: 	pMsg.index[1] = SET_NUMBERLB(aIndex);
- line 2305: 	gLog.Output(LOG_HACK,"[ClientSecurityBreach][%s][%s] (Code: %d)",lpObj->Account,lpObj->Name,MAKE_NUMBERDW(MAKE_NUMBERW(lpMsg->code[0],lpMsg->code[1]),MAKE_NUMBERW(lpMsg->code[2],lpMsg->code[3])));
- line 2381: 	pMsg.index[0] = SET_NUMBERHB(aIndex);
- line 2383: 	pMsg.index[1] = SET_NUMBERLB(aIndex);
- line 2409: 	pMsg.index[0] = SET_NUMBERHB(aIndex);
- line 2411: 	pMsg.index[1] = SET_NUMBERLB(aIndex);
- line 2510: 	pMsg.index[0] = SET_NUMBERHB(bIndex) | (flag*0x80);
- line 2511: 	pMsg.index[1] = SET_NUMBERLB(bIndex);

#### CONTEXT (first hit)

```
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
  1465: 	LPOBJ lpObj = &gObj[aIndex];
  1466: 
  1467: 	if(gObjIsConnectedGP(aIndex) == 0)
  1468: 	{
  1469: 		return;
  1470: 	}
  1471: 
  1472: 	lpObj->Dir = lpMsg->dir;
  1473: 
  1474: 	lpObj->ActionNumber = lpMsg->action;
  1475: 
  1476: 	if(lpObj->ActionNumber == ACTION_SIT1)
  1477: 	{
  1478: 		lpObj->ViewState = 2;
  1479: 		lpObj->Rest = lpObj->ActionNumber;
  1480: 	}
  1481: 	else if(lpObj->ActionNumber == ACTION_POSE1)
  1482: 	{
  1483: 		lpObj->ViewState = 3;
  1484: 		lpObj->Rest = lpObj->ActionNumber;
  1485: 	}
  1486: 	else if(lpObj->ActionNumber == ACTION_HEALING1)
  1487: 	{
  1488: 		lpObj->ViewState = 4;
  1489: 		lpObj->Rest = lpObj->ActionNumber;
  1490: 	}
  1491: 
  1492: 	PMSG_ACTION_SEND pMsg;
  1493: 
  1494: 	pMsg.header.set(0x18,sizeof(pMsg));
  1495: 
  1496: 	pMsg.index[0] = SET_NUMBERHB(aIndex);
  1497: 
  1498: 	pMsg.index[1] = SET_NUMBERLB(aIndex);
  1499: 
  1500: 	pMsg.dir = lpMsg->dir;
  1501: 
  1502: 	pMsg.action = lpMsg->action;
  1503: 
  1504: 	pMsg.target[0] = lpMsg->index[0];
  1505: 
  1506: 	pMsg.target[1] = lpMsg->index[1];
  1507: 
  1508: 	for(int n=0;n < MAX_VIEWPORT;n++)
  1509: 	{
  1510: 		if(lpObj->VpPlayer2[n].type == OBJECT_USER)
  1511: 		{
  1512: 			if(lpObj->VpPlayer2[n].state != OBJECT_EMPTY && lpObj->VpPlayer2[n].state != OBJECT_DIECMD && lpObj->VpPlayer2[n].state != OBJECT_DIED)
  1513: 			{
  1514: 				DataSend(lpObj->VpPlayer2[n].index,(BYTE*)&pMsg,pMsg.header.size);
  1515: 			}
  1516: 		}
  1517: 	}
  1518: }
  1519: 
  1520: void CGEventRemainTimeRecv(PMSG_EVENT_REMAIN_TIME_RECV* lpMsg,int aIndex) // OK
  1521: {
  1522: 	LPOBJ lpObj = &gObj[aIndex];
  1523: 
  1524: 	if(gObjIsConnectedGP(aIndex) == 0)
  1525: 	{
  1526: 		return;
  1527: 	}
  1528: 
  1529: 	PMSG_EVENT_REMAIN_TIME_SEND pMsg;
  1530: 
  1531: 	pMsg.header.set(0x91,sizeof(pMsg));
  1532: 
  1533: 	pMsg.EventType = lpMsg->EventType;
  1534: 
  1535: 	pMsg.RemainTimeH = 0;
  1536: 
  1537: 	pMsg.EnteredUser = 0;
  1538: 
  1539: 	pMsg.RemainTimeL = 0;
  1540: 
  1541: 	if(lpMsg->EventType == 4)
  1542: 	{
  1543: 		lpMsg->ItemLevel = gChaosCastle.GetUserAbleLevel(&gObj[aIndex]);
  1544: 	}
  1545: 	else
  1546: 	{
  1547: 		lpMsg->ItemLevel = ((lpMsg->EventType==2)?lpMsg->ItemLevel:(lpMsg->ItemLevel-1));
  1548: 	}
  1549: 
  1550: 	switch(lpMsg->EventType)
  1551: 	{
  1552: 		case 1:
  1553: 			if(DS_LEVEL_RANGE(lpMsg->ItemLevel) != 0)
  1554: 			{
  1555: 				if(gDevilSquare.GetState(lpMsg->ItemLevel) == DS_STATE_EMPTY)
  1556: 				{
  1557: 					if(gDevilSquare.GetEnterEnabled(lpMsg->ItemLevel) == 0)
  1558: 					{
  1559: 						pMsg.RemainTimeH = gDevilSquare.GetRemainTime(lpMsg->ItemLevel);
  1560: 					}
  1561: 					else
  1562: 					{
  1563: 						pMsg.EnteredUser = gDevilSquare.GetEnteredUserCount(lpMsg->ItemLevel);
  1564: 					}
  1565: 				}
  1566: 				else
  1567: 				{
  1568: 					pMsg.RemainTimeH = gDevilSquare.GetRemainTime(lpMsg->ItemLevel);
  1569: 				}
  1570: 			}
  1571: 			break;
  1572: 		case 2:
  1573: 			if(BC_LEVEL_RANGE(lpMsg->ItemLevel) != 0)
  1574: 			{
  1575: 				if(gBloodCastle.GetState(lpMsg->ItemLevel) == BC_STATE_EMPTY)
  1576: 				{
  1577: 					if(gBloodCastle.GetEnterEnabled(lpMsg->ItemLevel) == 0)
  1578: 					{
  1579: 						pMsg.RemainTimeH = gBloodCastle.GetRemainTime(lpMsg->ItemLevel);
  1580: 					}
  1581: 					else
  1582: 					{
  1583: 						pMsg.EnteredUser = gBloodCastle.GetEnteredUserCount(lpMsg->ItemLevel);
  1584: 					}
  1585: 				}
  1586: 				else
  1587: 				{
  1588: 					pMsg.RemainTimeH = gBloodCastle.GetRemainTime(lpMsg->ItemLevel);
  1589: 				}
  1590: 			}
  1591: 			break;
  1592: 		case 4:
  1593: 			if(CC_LEVEL_RANGE(lpMsg->ItemLevel) != 0)
  1594: 			{
  1595: 				if(gChaosCastle.GetState(lpMsg->ItemLevel) == CC_STATE_EMPTY)
  1596: 				{
  1597: 					if(gChaosCastle.GetEnterEnabled(lpMsg->ItemLevel) == 0)
  1598: 					{
  1599: 						pMsg.RemainTimeL = gChaosCastle.GetRemainTime(lpMsg->ItemLevel);
  1600: 					}
  1601: 					else
  1602: 					{
  1603: 						pMsg.EnteredUser = gChaosCastle.GetEnteredUserCount(lpMsg->ItemLevel);
  1604: 					}
  1605: 				}
  1606: 				else
  1607: 				{
  1608: 					pMsg.RemainTimeL = gChaosCastle.GetRemainTime(lpMsg->ItemLevel);
  1609: 				}
  1610: 			}
  1611: 			break;
  1612: 		case 5:
  1613: 			if(IT_LEVEL_RANGE(lpMsg->ItemLevel) != 0)
  1614: 			{
  1615: 				if(gIllusionTemple.GetState(lpMsg->ItemLevel) == IT_STATE_EMPTY)
  1616: 				{
  1617: 					if(gIllusionTemple.GetEnterEnabled(lpMsg->ItemLevel) == 0)
  1618: 					{
  1619: 						pMsg.RemainTimeH = gIllusionTemple.GetRemainTime(lpMsg->ItemLevel);
  1620: 					}
  1621: 					else
  1622: 					{
  1623: 						pMsg.EnteredUser = gIllusionTemple.GetEnteredUserCount(lpMsg->ItemLevel);
  1624: 					}
  1625: 				}
  1626: 				else
  1627: 				{
  1628: 					pMsg.RemainTimeH = gIllusionTemple.GetRemainTime(lpMsg->ItemLevel);
  1629: 				}
  1630: 			}
  1631: 			break;
  1632: 	}
  1633: 
  1634: 	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
  1635: }
  1636: 
  1637: void CGPetItemCommandRecv(PMSG_PET_ITEM_COMMAND_RECV* lpMsg,int aIndex) // OK
  1638: {
  1639: 	if(gObjIsConnectedGP(aIndex) == 0)
  1640: 	{
  1641: 		return;
  1642: 	}
  1643: 
  1644: 	if(lpMsg->type != 0)
  1645: 	{
  1646: 		return;
  1647: 	}
  1648: 
  1649: 	int bIndex = MAKE_NUMBERW(lpMsg->index[0],lpMsg->index[1]);
  1650: 
  1651: 	gDarkSpirit[aIndex].ChangeCommand(lpMsg->command,bIndex);
  1652: }
  1653: 
  1654: void CGPetItemInfoRecv(PMSG_PET_ITEM_INFO_RECV* lpMsg,int aIndex) // OK
  1655: {
  1656: 	LPOBJ lpObj = &gObj[aIndex];
  1657: 
  1658: 	if(gObjIsConnectedGP(aIndex) == 0)
  1659: 	{
  1660: 		return;
  1661: 	}
  1662: 
  1663: 	if(lpMsg->type != 0 && lpMsg->type != 1)
  1664: 	{
  1665: 		return;
  1666: 	}
```

### HIT: GS dispatch anchor / case\s+0x19\s*:|CGSkillAttackRecv

- line 121: 		case 0x19:
- line 122: 			gSkillManager.CGSkillAttackRecv((PMSG_SKILL_ATTACK_RECV*)lpMsg,aIndex);

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


---

## QUESTIONS (responder como CONFIRMED/PROBABLE com âncora)

1) Onde o GS monta e envia o pacote 0x19 (magic)? (função + linha)

2) Qual struct on-wire é usada (campos/ordem/tamanho) e qual `header.set(0x19,sizeof)`?

3) Como SourceKey/TargetKey/MagicNumber são preenchidos (SET_NUMBERH/L etc)?

4) O envio é DataSend direto, MsgSendV2 (viewport) ou ambos?
