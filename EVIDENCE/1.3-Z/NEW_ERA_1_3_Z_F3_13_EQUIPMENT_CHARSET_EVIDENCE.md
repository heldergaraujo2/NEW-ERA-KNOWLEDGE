# NEW-ERA 1.3-Z — F3:13 Equipment/CharSet (aparência) — EVIDENCE

- Goal: provar o layout on-wire e onde é emitido/aplicado (client+GS).


## Files fetched (sha256)

- `WSclient.cpp` sha256 `6538d944126181dfa77d4419bf992ac77151eec5411dc3ab243f2926fec868bf` bytes=415884
- `WSclient.h` sha256 `19d38a3b6506a745662c14dd7cdd6e41ca3ad29b28b17c76f860cd9e67799c10` bytes=91012
- `ItemManager.h` sha256 `2ad786682724c2b40fa15c6cd2bf2d25948cdf408173bcc4c8489d2b411e0b64` bytes=12056
- `ItemManager.cpp` sha256 `2ebe79ada6400e0a1d06c92ca76a2513260ea36590258f7ac0ccafdca2656030` bytes=105683
- `PG_Custom.cpp` sha256 `19e1cdb2f46a2de5e54447b60e3761412eb4aebd965ba0834362f52fdfc4e3e0` bytes=19165
- `GS_Protocol.h` sha256 `943c59b7a457eccea4953b7410fd2140c6299979782b58ca81a7b42a52d6c3b0` bytes=36305

---

## CLIENT — dispatch F3:13

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

### first case 0x13 @ 5969
- `	case 0x13:`
```
  5849: 		else if ( 15 == Data->SubCode || 16 == Data->SubCode )
  5850: 		{
  5851: 			g_pLuckyItemWnd->GetResult(1, Data->Index, Data->Item);
  5852: 		}
  5853: #endif // LEM_ADD_LUCKYITEM
  5854: 
  5855: 		PlayBuffer(SOUND_GET_ITEM01);
  5856: 	}
  5857: 	else
  5858: 	{
  5859: 		SEASON3B::CNewUIInventoryCtrl::BackupPickedItem();
  5860: 		g_pStorageInventory->ProcessStorageItemAutoMoveFailure();
  5861: 	}
  5862: 
  5863: 	if ( g_bPacketAfter_EquipmentItem)
  5864: 	{
  5865: 		ReceiveTradeExit( g_byPacketAfter_EquipmentItem);
  5866: 		g_bPacketAfter_EquipmentItem = FALSE;
  5867: 	}
  5868: 
  5869: 	g_ConsoleDebug->Write(MCD_RECEIVE, "0x24 [ReceiveEquipmentItem(%d %d)]", Data->SubCode, Data->Index);
  5870: 
  5871: 	return ( TRUE);
  5872: }
  5873: 
  5874: void ReceiveModifyItem( BYTE *ReceiveBuffer )
  5875: {
  5876: 	LPPHEADER_DEFAULT_SUBCODE_ITEM Data = (LPPHEADER_DEFAULT_SUBCODE_ITEM)ReceiveBuffer;
  5877: 
  5878: 	if(SEASON3B::CNewUIInventoryCtrl::GetPickedItem())
  5879: 		SEASON3B::CNewUIInventoryCtrl::DeletePickedItem();
  5880: 
  5881: 	int itemindex = Data->Index - MAX_EQUIPMENT_INDEX;
  5882: 	if(g_pMyInventory->FindItem(itemindex))
  5883: 		g_pMyInventory->DeleteItem(itemindex);
  5884: 
  5885: 	g_pMyInventory->InsertItem(itemindex, Data->Item);
  5886: 
  5887: 	int iType = ConvertItemType(Data->Item);
  5888: 	if(iType == ITEM_POTION+28 || iType == ITEM_POTION+111)
  5889: 	{
  5890: 		PlayBuffer(SOUND_KUNDUN_ITEM_SOUND);
  5891: 	}
  5892: 	else if(GambleSystem::Instance().IsGambleShop())
  5893: 	{
  5894: 
  5895: 	}
  5896: 	else
  5897: 	{
  5898: 		PlayBuffer(SOUND_JEWEL01);
  5899: 	}
  5900: }
  5901: 
  5902: BOOL ReceiveTalk(BYTE *ReceiveBuffer, BOOL bEncrypted)
  5903: {
  5904: 
  5905: 	LPPHEADER_DEFAULT Data = (LPPHEADER_DEFAULT)ReceiveBuffer;
  5906: 
  5907: 	g_pNewUISystem->HideAll();
  5908: 
  5909: 	switch(Data->Value)
  5910: 	{
  5911: 	case 2:
  5912: 		g_pNewUISystem->Show(SEASON3B::INTERFACE_STORAGE);
  5913: 		break;
  5914: 
  5915: 	case 3:
  5916: 		g_MixRecipeMgr.SetMixType(SEASON3A::MIXTYPE_GOBLIN_NORMAL);
  5917: 		g_pNewUISystem->Show(SEASON3B::INTERFACE_MIXINVENTORY);
  5918: 		//BYTE *pbyChaosRate = ( &Data->Value) + 1;
  5919: 		//int iDummyRate[6];	// ����ǥ Ȯ���� �������� ������ ������� �ʰ� ����
  5920: 		//for ( int i = 0; i < 6; ++i)
  5921: 		//	iDummyRate[i] = ( int)pbyChaosRate[i];	// ����ǥ Ȯ���� �������� ������ ������� �ʰ� ����(��ũ��Ʈ���)
  5922: 		break;
  5923: 
  5924: 	case 4:
  5925: 		g_pNewUISystem->Show( SEASON3B::INTERFACE_DEVILSQUARE );
  5926: 		break;
  5927: 
  5928:     case 5:
  5929: 		g_pUIManager->Open( ::INTERFACE_SERVERDIVISION );
  5930: 		break;
  5931: 
  5932: 	case 6:
  5933: 		g_pNewUISystem->Show( SEASON3B::INTERFACE_BLOODCASTLE );
  5934: 		break;
  5935: 
  5936:     case 7:
  5937: 		SEASON3B::CreateMessageBox(MSGBOX_LAYOUT_CLASS(SEASON3B::CTrainerMenuMsgBoxLayout));
  5938: 		break;
  5939: 
  5940: 	case INDEX_NPC_LAHAP:
  5941: 		{
  5942: 			if(COMGEM::isAble())
  5943: 			{
  5944: 				g_pNewUISystem->HideAll();
  5945: 				SEASON3B::CreateMessageBox(MSGBOX_LAYOUT_CLASS(SEASON3B::CGemIntegrationMsgBoxLayout));
  5946: 			}
  5947: 		}
  5948: 		break;
  5949: 
  5950:     case 0x0C:
  5951: 		g_pNewUISystem->Show(SEASON3B::INTERFACE_SENATUS);
  5952:         break;
  5953: 
  5954:     case 0x0D:
  5955: 		SendRequestBCStatus();
  5956:         break;
  5957: 	case 0x11:
  5958: 		{
  5959: 			SEASON3B::CreateMessageBox(MSGBOX_LAYOUT_CLASS(SEASON3B::CElpisMsgBoxLayout));
  5960: 		}
  5961: 		break;
  5962: 	case 0x12:
  5963: 		{
  5964: 			SEASON3B::CreateMessageBox(MSGBOX_LAYOUT_CLASS(SEASON3B::COsbourneMsgBoxLayout));
  5965: 			// 			BYTE *pbyChaosRate = ( &Data->Value) + 1;
  5966: 			// 			g_pUIJewelHarmony->SetMixSuccessRate(pbyChaosRate);
  5967: 		}
  5968: 		break;
  5969: 	case 0x13:
  5970: 		{
  5971: 			g_MixRecipeMgr.SetMixType(SEASON3A::MIXTYPE_JERRIDON);
  5972: 			g_pNewUISystem->Show(SEASON3B::INTERFACE_MIXINVENTORY);
  5973: 			// 			BYTE *pbyChaosRate = ( &Data->Value) + 1;
  5974: 			// 			g_pUIJewelHarmony->SetMixSuccessRate(pbyChaosRate);
  5975: 		}
  5976: 		break;
  5977: 	case 0x14:
  5978: 		{
  5979: 			g_pNewUISystem->Show(SEASON3B::INTERFACE_CURSEDTEMPLE_NPC);
  5980: 
  5981: 			BYTE *cursedtempleenterinfo = ( &Data->Value) + 1;
  5982: 			g_pCursedTempleEnterWindow->SetCursedTempleEnterInfo(cursedtempleenterinfo);
  5983: 		}
  5984: 		break;
  5985: 	case 0x15:
  5986: 		{
  5987: 			g_MixRecipeMgr.SetMixType(SEASON3A::MIXTYPE_CHAOS_CARD);
  5988: 			g_pNewUISystem->Show(SEASON3B::INTERFACE_MIXINVENTORY);
  5989: 		}
  5990: 		break;
  5991: 	case 0x16:
  5992: 		{
  5993: 			g_MixRecipeMgr.SetMixType(SEASON3A::MIXTYPE_CHERRYBLOSSOM);
  5994: 			g_pNewUISystem->Show(SEASON3B::INTERFACE_MIXINVENTORY);
  5995: 		}
  5996: 		break;
  5997: 	case 0x17:
  5998: 		{
  5999: 			SEASON3B::CreateMessageBox(MSGBOX_LAYOUT_CLASS(SEASON3B::CSeedMasterMenuMsgBoxLayout));
  6000: 		}
  6001: 		break;
  6002: 	case 0x18:
  6003: 		{
  6004: 			SEASON3B::CreateMessageBox(MSGBOX_LAYOUT_CLASS(SEASON3B::CSeedInvestigatorMenuMsgBoxLayout));
  6005: 		}
  6006: 		break;
  6007: 	case 0x19:
  6008: 		{
  6009: 			SEASON3B::CreateMessageBox(MSGBOX_LAYOUT_CLASS(SEASON3B::CResetCharacterPointMsgBoxLayout));
  6010: 		}
  6011: 		break;
  6012: 	case 0x20:
  6013: 		{
  6014: 			SEASON3B::CreateMessageBox(MSGBOX_LAYOUT_CLASS(SEASON3B::CDelgardoMainMenuMsgBoxLayout));
  6015: 		}
  6016: 		break;
  6017: 	case 0x21:
  6018: 		{
  6019: 			g_pNewUISystem->Show(SEASON3B::INTERFACE_DUELWATCH);
  6020: 		}
  6021: 		break;
  6022: 	case 0x22:
  6023: 		{
  6024: 			GambleSystem::Instance().SetGambleShop();
  6025: 			g_pNewUISystem->Show(SEASON3B::INTERFACE_NPCSHOP);
  6026: 		}
  6027: 		break;
  6028: 	case 0x23:
  6029: 		{
  6030: 			g_pNewUISystem->Show(SEASON3B::INTERFACE_DOPPELGANGER_NPC);
  6031: 			BYTE * pbtRemainTime = (&Data->Value) + 1;
  6032: 			g_pDoppelGangerWindow->SetRemainTime(*pbtRemainTime);
  6033: 		}
  6034: 		break;
  6035: 	case 0x24:
  6036: 		{
  6037: 			g_pNewUISystem->Show(SEASON3B::INTERFACE_EMPIREGUARDIAN_NPC);
  6038: 		}
  6039: 		break;
  6040: 	case 0x25:
  6041: 		{
  6042: 			g_pNewUISystem->Show(SEASON3B::INTERFACE_UNITEDMARKETPLACE_NPC_JULIA);
  6043: 		}
  6044: 		break;
  6045: #ifdef LEM_ADD_LUCKYITEM
  6046: 	case 0x26:
  6047: 		{
  6048: 			SEASON3B::CreateMessageBox(MSGBOX_LAYOUT_CLASS(SEASON3B::CLuckyTradeMenuMsgBoxLayout));
  6049: 		}
  6050: 		break;
  6051: #endif // LEM_ADD_LUCKYITEM
  6052: 	default:
  6053: 		{
  6054: 			// Data->Value
  6055: 			g_pNewUISystem->Show(SEASON3B::INTERFACE_NPCSHOP);
  6056: 		}
  6057: 		break;
  6058: 	}
  6059:     PlayBuffer(SOUND_CLICK01);
  6060:     PlayBuffer(SOUND_INTERFACE01);
  6061: #ifndef FOR_WORK
  6062: #ifdef WINDOWMODE
  6063: 	if (g_bUseWindowMode == FALSE)
  6064: 	{
  6065: #endif	// WINDOWMODE
  6066: 		int x = 260*MouseX/640;
  6067: 		SetCursorPos((x)*WindowWidth/640,(MouseY)*WindowHeight/480);
  6068: #ifdef WINDOWMODE
  6069: 	}
  6070: #endif	// WINDOWMODE
  6071: #endif	// FOR_WORK
  6072: 
  6073: 	return ( TRUE);
  6074: }
  6075: 
  6076: void ReceiveBuy( BYTE *ReceiveBuffer )
  6077: {
  6078: 	LPPHEADER_DEFAULT_ITEM Data = (LPPHEADER_DEFAULT_ITEM)ReceiveBuffer;
  6079: 	if(Data->Index != 255)
  6080: 	{
  6081: 		if(Data->Index >= MAX_EQUIPMENT_INDEX && Data->Index < MAX_MY_INVENTORY_INDEX)
  6082: 		{
  6083: 			g_pMyInventory->InsertItem(Data->Index-MAX_EQUIPMENT_INDEX, Data->Item);
  6084: 		}
  6085: 		else
  6086: 		{
  6087: #ifdef _DEBUG
  6088: 			__asm { int 3 };
  6089: #endif // _DEBUG
  6090: 		}
  6091: 
  6092: 		PlayBuffer(SOUND_GET_ITEM01);
  6093: 	}
  6094: 	if( Data->Index == 0xfe )
  6095: 	{
  6096: 		g_pNewUISystem->HideAll();
  6097: 
  6098: 		g_pChatListBox->AddText(Hero->ID,GlobalText[732], SEASON3B::TYPE_ERROR_MESSAGE);
  6099: 	}
  6100: 	BuyCost = 0;
  6101: 
  6102: 	g_ConsoleDebug->Write(MCD_RECEIVE, "0x32 [ReceiveBuy(%d)]", Data->Index);
  6103: }
  6104: 
  6105: void ReceiveSell( BYTE *ReceiveBuffer )
  6106: {
  6107: 	LPPRECEIVE_GOLD Data = (LPPRECEIVE_GOLD)ReceiveBuffer;
  6108: 	if(Data->Flag != 0)
  6109: 	{
  6110: 		if( Data->Flag == 0xff )
  6111: 		{
  6112: 			SEASON3B::CNewUIInventoryCtrl::BackupPickedItem();
  6113: 
  6114: 			g_pChatListBox->AddText(Hero->ID,GlobalText[733], SEASON3B::TYPE_ERROR_MESSAGE);
  6115: 		}
  6116: 		else if( Data->Flag == 0xfe )
  6117: 		{
  6118: 			g_pNewUISystem->HideAll();
  6119: 
  6120: 			g_pChatListBox->AddText(Hero->ID,GlobalText[733], SEASON3B::TYPE_ERROR_MESSAGE);
  6121: 		}
  6122: 		else
  6123: 		{
  6124: 			SEASON3B::CNewUIInventoryCtrl::DeletePickedItem();
  6125: 
  6126: 			CharacterMachine->Gold = Data->Gold;
  6127: 
  6128: 			PlayBuffer(SOUND_GET_ITEM01);
  6129: 
  6130: 			g_pNPCShop->SetSellingItem(false);
  6131: 		}
  6132: 	}
  6133: 	else
  6134: 	{
  6135: 		SEASON3B::CNewUIInventoryCtrl::BackupPickedItem();
  6136: 	}
  6137: }
  6138: 
  6139: void ReceiveRepair( BYTE *ReceiveBuffer )
  6140: {
  6141: 	LPPRECEIVE_REPAIR_GOLD Data = (LPPRECEIVE_REPAIR_GOLD)ReceiveBuffer;
  6142: 
  6143:     if(Data->Gold != 0)
  6144: 	{
  6145: 		CharacterMachine->Gold = Data->Gold;
  6146:         CharacterMachine->CalculateAll();
  6147:         PlayBuffer(SOUND_REPAIR);
  6148: 	}
  6149: 
  6150: 	g_ConsoleDebug->Write(MCD_RECEIVE, "0x34 [ReceiveRepair(%d)]", Data->Gold);
  6151: }
  6152: 
  6153: void ReceiveLevelUp( BYTE *ReceiveBuffer )
  6154: {
  6155: 	LPPRECEIVE_LEVEL_UP Data			= (LPPRECEIVE_LEVEL_UP)ReceiveBuffer;
  6156: 	CharacterAttribute->Level			= Data->Level;
  6157: 	CharacterAttribute->LevelUpPoint	= Data->LevelUpPoint;
  6158: 	CharacterAttribute->LifeMax			= Data->MaxLife;
  6159: 	CharacterAttribute->ManaMax			= Data->MaxMana;
  6160: 	CharacterAttribute->Life			= Data->MaxLife;
  6161: 	CharacterAttribute->Mana			= Data->MaxMana;
  6162: 	CharacterAttribute->ShieldMax		= Data->MaxShield;
  6163: 	CharacterAttribute->SkillManaMax	= Data->SkillManaMax;
  6164:     CharacterAttribute->AddPoint		= Data->AddPoint;
  6165:     CharacterAttribute->MaxAddPoint		= Data->MaxAddPoint;
  6166: 	CharacterAttribute->wMinusPoint		= Data->wMinusPoint;
  6167:     CharacterAttribute->wMaxMinusPoint	= Data->wMaxMinusPoint;
  6168: 
  6169: 	unicode::t_char szText[256] = {NULL, };
  6170: 	WORD iExp = CharacterAttribute->NextExperince - CharacterAttribute->Experience;
  6171: 	sprintf(szText,GlobalText[486], iExp);
  6172: 	g_pChatListBox->AddText("", szText, SEASON3B::TYPE_SYSTEM_MESSAGE);
  6173: 
  6174:     CharacterMachine->CalculateNextExperince();
  6175: 
  6176: 	OBJECT *o = &Hero->Object;
  6177: 
  6178: 	if(gCharacterManager.IsMasterLevel(Hero->Class) == true)
  6179: 	{
  6180: 
  6181: 		CreateJoint(BITMAP_FLARE,o->Position,o->Position,o->Angle,45,o,80,2);
  6182: 		for ( int i=0; i<19; ++i )
  6183: 		{
  6184: 			CreateJoint(BITMAP_FLARE,o->Position,o->Position,o->Angle,46,o,80,2);
  6185: 		}
  6186: 	}
  6187: 	else
  6188: 	{
  6189: 		for ( int i=0; i<15; ++i )
  6190: 		{
  6191: 			CreateJoint(BITMAP_FLARE,o->Position,o->Position,o->Angle,0,o,40,2);
  6192: 		}
  6193: 		CreateEffect(BITMAP_MAGIC+1,o->Position,o->Angle,o->Light,0,o);
  6194: 	}
  6195:     PlayBuffer(SOUND_LEVEL_UP);
  6196: 
  6197: 	g_ConsoleDebug->Write(MCD_RECEIVE, "0x05 [ReceiveLevelUp]");
  6198: }
  6199: 
  6200: void ReceiveAddPoint( BYTE *ReceiveBuffer )
  6201: {
  6202:     LPPRECEIVE_ADD_POINT Data = (LPPRECEIVE_ADD_POINT)ReceiveBuffer;
  6203: 	if(Data->Result>>4)
  6204: 	{
  6205: 		CharacterAttribute->LevelUpPoint --;
  6206: 		switch(Data->Result&0xf)
  6207: 		{
  6208: 		case 0:
  6209: 			CharacterAttribute->Strength ++;
  6210: 			break;
  6211: 		case 1:
  6212: 			CharacterAttribute->Dexterity ++;
  6213: 			break;
  6214: 		case 2:
  6215: 			CharacterAttribute->Vitality ++;
  6216: 			CharacterAttribute->LifeMax = Data->Max;
  6217: 			break;
  6218: 		case 3:
  6219: 			CharacterAttribute->Energy ++;
  6220: 			CharacterAttribute->ManaMax = Data->Max;
  6221: 			break;
  6222:         case 4:
  6223: 			CharacterAttribute->Charisma ++;
  6224:             break;
  6225: 		}
  6226: 		CharacterAttribute->SkillManaMax = Data->SkillManaMax;
  6227: 		CharacterAttribute->ShieldMax = Data->ShieldMax;
  6228: 	}
  6229:     CharacterMachine->CalculateAll();
```


---
## CLIENT — header tokens (CharSet / EQUIPMENT_LENGTH)

### `\bEQUIPMENT_LENGTH\b`

- hit @ 71: `#define EQUIPMENT_LENGTH    17`
```
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
   249: 
   250: //send trade
   251: typedef struct {
   252: 	PBMSG_HEADER  Header;
   253: 	BYTE          SrcFlag;
   254: 	BYTE          SrcIndex;
   255: 	BYTE          Item[PACKET_ITEM_LENGTH];
   256: 	BYTE          DstFlag;
   257: 	BYTE          DstIndex;
   258: } PSEND_TRADE, * LPSEND_TRADE;
   259: 
   260: ///////////////////////////////////////////////////////////////////////////////
   261: // log in
   262: ///////////////////////////////////////////////////////////////////////////////
   263: 
   264: typedef struct
   265: {
   266: 	WORD         Index;
   267: 	BYTE         Percent;
   268: } PRECEIVE_SERVER_LIST, * LPPRECEIVE_SERVER_LIST;
   269: 
   270: //receive join server
   271: typedef struct
   272: {
   273: 	PBMSG_HEADER Header;
   274: 	BYTE         SubCode;
   275: 	BYTE         Result;
   276: 	BYTE         NumberH;
   277: 	BYTE         NumberL;
   278: 	BYTE         Version[SIZE_PROTOCOLVERSION];
   279: } PRECEIVE_JOIN_SERVER, * LPPRECEIVE_JOIN_SERVER;
   280: 
   281: //request create acoount
   282: typedef struct
   283: {
   284: 	PBMSG_HEADER Header;
   285: 	BYTE         SubCode;
   286: 	BYTE         ID[MAX_ID_SIZE];
   287: 	BYTE         Password[MAX_ID_SIZE];
   288: 	BYTE         Name[10];
   289: 	BYTE         ResidentNumber[13];
   290: 	BYTE         Quiz[30];
   291: 	BYTE         Answer[30];
   292: 	BYTE         PhoneNumber[20];
   293: 	BYTE         Email[50];
   294: } PREQUEST_CREATE_ACCOUNT, * LPPREQUEST_CREATE_ACCOUNT;
   295: 
   296: //request log in
   297: typedef struct
   298: {
   299: 	PBMSG_HEADER Header;
   300: 	BYTE         SubCode;
   301: 	BYTE         ID[MAX_ID_SIZE];
   302: 	BYTE         Password[MAX_ID_SIZE];
   303: 	DWORD        Time;
   304: 	BYTE         Version[SIZE_PROTOCOLVERSION];
   305: 	BYTE         Serial[SIZE_PROTOCOLSERIAL];
   306: } PREQUEST_LOG_IN, * LPPREQUEST_LOG_IN;
   307: 
   308: //request confirm password
   309: typedef struct
   310: {
   311: 	PBMSG_HEADER Header;
```

### `\bCharSet\b`

- no hits


### `\bCHARSET\b`

- no hits


### `\bEQUIPMENT\b`

- no hits


### `\bF3\b.*\b13\b`

- no hits



---
## SERVER — ItemManager.h (struct do F3:13)

### hit `\bPMSG_ITEM_EQUIPMENT_SEND\b` @ 179
- `struct PMSG_ITEM_EQUIPMENT_SEND`
```
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
## SERVER — ItemManager.cpp (GCItemEquipmentSend)

- hit @ 4682: `void CItemManager::GCItemEquipmentSend(int aIndex) // OK`
```
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


---
## SERVER — PG_Custom.cpp (evidência paralela)

- hit @ 244: `struct PMSG_EQUIPMENTLIST`
```
   164: 	{
   165: 		if ( gObj[iIndex].Inventory[ItemPos].m_Index == itemId )
   166: 		{
   167: 			if (  gObj[iIndex].Inventory[ItemPos].m_Level == ItemLevel )
   168: 			{
   169: 				gItemManager.InventoryDelItem(iIndex,ItemPos);
   170: 				gItemManager.GCItemDeleteSend(iIndex, ItemPos, 1);
   171: 				return 1;
   172: 			}
   173: 		}
   174: 	}
   175: 	return 0;
   176: }
   177: 
   178: int gObjMonsterAdd(WORD Type, BYTE Map, BYTE X, BYTE Y)	//SkyEvent
   179: {
   180: 	if ( gMonsterSetBase.m_count >= MAX_OBJECT )
   181: 	{
   182: //		ErrorMessageBox("Monster attribute max over %s %d", __FILE__, __LINE__);
   183: 		return -1;
   184: 	}
   185: 
   186: 	if (gMapServerManager.CheckMapServer(Map) == FALSE )
   187: 	{
   188: 		LogAdd(LOG_BLACK,"[MonsterAdd] No need to load monster for map %d [%d,%d,%d]",Map,Type,X,Y);
   189: 		return -1;
   190: 	}
   191: 
   192: 	gMonsterSetBase.m_count++;
   193: 	gMonsterSetBase.m_Mp[gMonsterSetBase.m_count].Dis			= 30;
   194: 	gMonsterSetBase.m_Mp[gMonsterSetBase.m_count].Type			= Type;
   195: 	gMonsterSetBase.m_Mp[gMonsterSetBase.m_count].Map			= Map;
   196: 	gMonsterSetBase.m_Mp[gMonsterSetBase.m_count].TY			= X;
   197: 	gMonsterSetBase.m_Mp[gMonsterSetBase.m_count].TX			= Y;
   198: 	gMonsterSetBase.m_Mp[gMonsterSetBase.m_count].X				= X;
   199: 	gMonsterSetBase.m_Mp[gMonsterSetBase.m_count].Y				= Y;
   200: 	gMonsterSetBase.m_Mp[gMonsterSetBase.m_count].Value			= 1;	//TEST!!!
   201: 	gMonsterSetBase.m_Mp[gMonsterSetBase.m_count].Dir			= 3;
   202: 
   203: 	return gMonsterSetBase.m_count;
   204: }
   205: 
   206: int ItemGetNumberMake(int type, int index)	//TEST
   207: {
   208: 	int make;
   209: 
   210: 	make = type*MAX_ITEM_TYPE + index;
   211: 
   212: 	if ( make < 0 || make >= MAX_ITEM )
   213: 	{
   214: 		return -1;
   215: 	}
   216: 
   217: 	if (ItemAttribute[make].Width < 1 ||  ItemAttribute[make].Height < 1)
   218: 	{
   219: 		return -1;
   220: 	}
   221: 	return make;
   222: }
   223: 
   224: int gObjGetIndex(char* szId)	//OK BUSCAR SU FUNCION CORRESPONDIENTE
   225: {
   226: 	for (int n=OBJECT_START_USER;n<MAX_OBJECT; n++)
   227: 	{
   228: 		if ( gObj[n].Connected >= OBJECT_ONLINE)
   229: 		{
   230: 			if ( gObj[n].Name[0] == *szId )
   231: 			{
   232: 				if ( strcmp(&gObj[n].Name[0] , szId) == 0)
   233: 				{
   234: 					return n;
   235: 				}
   236: 			}
   237: 		}
   238: 	}
   239: 	return -1;
   240: }
   241: /****************************************************************************************/
   242: /****************************************************************************************/
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
   504: 	int S6E2 = 1;
   505: 
   506: 	int InventoryRows = 12;
   507: 	if(S6E2 == 1)
   508: 		InventoryRows = 28;
   509: 
   510: 	if(sx + width > 8)
   511: 	{
   512: 		return -1;
   513: 	}
   514: 
   515: 	if(sy + height > InventoryRows)
   516: 	{
   517: 		return -2;
   518: 	}
   519: 
   520: 	if(S6E2 == 1 && height > 1)
   521: 	{
   522: 		if(sy < 8 && sy+height > 8 )
   523: 		{
   524: 			return -1;
   525: 		}
   526: 
   527: 		if(sy > 7 && sy < 12 && sy+height > 12)
   528: 		{
   529: 			return -1;
   530: 		}
   531: 
   532: 		if(sy > 11 && sy < 16 && sy+height > 16)
   533: 		{
   534: 			return -1;
   535: 		}
   536: 
   537: 		if(sy > 16 && sy < 20 && sy+height > 20)
   538: 		{
   539: 			return -1;
   540: 		}
   541: 
   542: 		if(sy > 20 && sy < 24 && sy+height > 24)
   543: 		{
   544: 			return -1;
   545: 		}
   546: 
   547: 		if(sy > 24 && sy < 28 && sy+height > 28)
   548: 		{
   549: 			return -1;
   550: 		}
   551: 	}
   552: 
   553: 	int xx,yy;
   554: 
   555: 	for(y = 0; y < height; y ++)
   556: 	{
   557: 		yy = sy+y;
   558: 
   559: 		for(x = 0; x < width; x++)
   560: 		{
   561: 			xx = sx + x;
   562: 
   563: 			if(ExtentCheck(xx,yy,8,InventoryRows)==1)
   564: 			{
```


---
## SERVER — Protocol.h (PSBMSG_HEAD)

- hit @ 100: `struct PSBMSG_HEAD`
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
```


---
## QUESTIONS

1) Qual é o layout exato do F3:13 (campos + tamanho)?

2) No client, qual função aplica o CharSet recebido?

3) No GS, é DataSend direto ou MsgSendV2 (broadcast)?
