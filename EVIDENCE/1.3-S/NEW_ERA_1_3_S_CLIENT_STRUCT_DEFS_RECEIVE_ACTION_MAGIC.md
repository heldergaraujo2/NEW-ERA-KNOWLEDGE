# NEW-ERA 1.3-S — CLIENT: defs de RECEIVE_ACTION / RECEIVE_MAGIC (legado) — EVIDENCE

- Legacy commit pin: `580472e0d5723f9709cbad594f233deb07f9f351`

- API index: `https://api.github.com/repos/wongddd/muonline/contents/Source%20Client/source?ref=580472e0d5723f9709cbad594f233deb07f9f351`

- Goal: localizar structs/typedefs usados por `ReceiveAction` (0x18) e `ReceiveMagic` (0x19) e evidenciar packing/ordem de campos.


---

## Files fetched (subset)

- `Source Client/source/WSclient.h` bytes=91012 sha256=`19d38a3b6506a745662c14dd7cdd6e41ca3ad29b28b17c76f860cd9e67799c10`
- `Source Client/source/wsclientinline.h` bytes=62684 sha256=`67a6d5359bb702b38f2a43e7d8377f3c8e37b8af4f2a49b15b4417d4e98a0854`
- `Source Client/source/StreamPacketEngine.h` bytes=2603 sha256=`797d26d9678852e2f586df967a090b451d1d5b07d97c6fba7ddbe21d39c2e684`
- `Source Client/source/ZzzAI.h` bytes=1999 sha256=`3d27d7c1073a7255bd9f89ce5651324151e3190a51047a8688382555de037378`
- `Source Client/source/ZzzBMD.h` bytes=10581 sha256=`44d0d6693ebb4a5fc652b1ca5b1f8d0c208d2785a457fc1ce47064dc4016a0d0`
- `Source Client/source/wsctlc.h` bytes=3162 sha256=`8c9520d4bc13f57e9d77c738e95323ec2756620f67d3df40872e8d88ef9c5c16`
- `Source Client/source/ZzzPath.h` bytes=9733 sha256=`981cc6fed3918de1d17f8e9c7a03c19a681a06b5dfe57a5ea3ff1c6dc1b85125`
- `Source Client/source/ZzzScene.h` bytes=968 sha256=`c2da5a5090908ae58fe4227bd03adf3cc43b8101409264389f3a594e230ad079`
- `Source Client/source/ZzzEffect.h` bytes=6504 sha256=`5b9b1adf1887d5ab40bd7f0bf90dd961a127871d4fe4eaf1e7127e11e6321c1f`
- `Source Client/source/ZzzObject.h` bytes=4108 sha256=`dc7ce4e8079d0c4e08eea75e8240c3c704d0a2d9485086e7aab9d74c6f2c35da`
- `Source Client/source/ZzzShader.h` bytes=1349 sha256=`332413c3ccce1ccaee3ed186093ef7de4bf2fc8d3993f1a33972681e08965571`
- `Source Client/source/ZzzTexture.h` bytes=961 sha256=`27c09ee032416345156681edff3a5d00bb68604fcca63f9519df6a6ecf476c34`
- `Source Client/source/ZzzOpenData.h` bytes=817 sha256=`776212556dbe45c1ce792710892cc450bb547c73e2df7a467a52efbb0a55a25c`
- `Source Client/source/ProtocolAsio.h` bytes=20884 sha256=`8489f782392d96d718aa9bf65c47d2de5798a770dc850ee7f32c78368052df8c`
- `Source Client/source/ProtocolSend.h` bytes=3321 sha256=`3ff13302150537b3ed7db40aef8c3dd6f5e5b81927e39584b0510d598d97d684`
- `Source Client/source/SocketSystem.h` bytes=4219 sha256=`6873108a65b187fb5308b6d0742ae81ad37c7f0ac0e87c563c86232b2f964e7a`
- `Source Client/source/ZzzCharacter.h` bytes=3963 sha256=`b0c2894404175ac771558820da3d827fb6158d6dd4122116bb2dcd71129ec234`
- `Source Client/source/ZzzInterface.h` bytes=4840 sha256=`ff2b7beff78b407974f5f9a3ca5f38ba66c232a5143fb5e3c6d785c983d854ed`
- `Source Client/source/ZzzInventory.h` bytes=6850 sha256=`0308783b44c5aa7e27d76df66dab35bf2627583b9ae6b48700929f8db0052871`
- `Source Client/source/wsctlc_addon.h` bytes=2182 sha256=`04cdbb9b659388b40d08959902e3b3524a1f05562be9fcb9f3d78b053d59ce35`
- `Source Client/source/ZzzInfomation.h` bytes=3778 sha256=`7533a5a98c69b4fff82ff9b1f6c1e10c056722e1ba6c18718aad51cf67cfa962`
- `Source Client/source/ZzzLodTerrain.h` bytes=5107 sha256=`7421c87f13d0522392b177e2edee2a252181b51fd6ef21f12ea6f492c50fc70b`
- `Source Client/source/ZzzOpenglUtil.h` bytes=5286 sha256=`1e3c0112e382759557ba8f6c4260efeecf75c45ceec1ff9d2234009119e1ed15`
- `Source Client/source/zzzMixInventory.h` bytes=95 sha256=`ebfe92d0ad780d154441928b179f05523c0c4babe51b1ffc6118a86b2bbc3e26`
- `Source Client/source/SMD.h` bytes=1743 sha256=`d0694809f1493fa5773c6d83367197bd9365944cc3e971e0266852c312f93b62`
- `Source Client/source/Win.h` bytes=2460 sha256=`ef55746c05bfab6833abd6751f1f837d51b71c7afdb9d1ac5febf3baf085c082`
- `Source Client/source/Event.h` bytes=1401 sha256=`22a2b26a4b1d6e16a81504b7276f71f0817268dee4f91b4ac35c2b6fff892607`
- `Source Client/source/Input.h` bytes=2441 sha256=`f14b85bbf86ce9c271360c14f5218eec7abc9c86dd3fd79f9736f93468009045`
- `Source Client/source/Local.h` bytes=163 sha256=`3674195d15bffabdf4f12e800dcfa0ac57a9dde61ef9d905f48079cd3c2bd407`
- `Source Client/source/PList.h` bytes=1412 sha256=`72408f85e10d00418af495b58cd6b11b5ce9480a206c7254bc6ae7eaba3b9dd2`
- `Source Client/source/UIMng.h` bytes=2394 sha256=`d7944dd432d2e9bec04715bbdafad4c795f6cc490767c2b222ad5c8de039c7b8`
- `Source Client/source/WinEx.h` bytes=1067 sha256=`6a5c698c87c5ca0b84cc40350e96f06afa3a6dd93bc6a8b7cffefcf491b4edbf`
- `Source Client/source/_enum.h` bytes=56491 sha256=`7a3eba988f02b19178aee240ad2730615f851def3e83f6c8e0bfb85d3e35ad90`
- `Source Client/source/nvapi.h` bytes=100622 sha256=`e605db63e2a7521962f015248f0fa7bd457c11cecdb44b810856644aca25c3fe`
- `Source Client/source/Button.h` bytes=1785 sha256=`039c24b3f041069c020f7acdfe4921aecac4c1c79fab8e3559289ace87ad28ea`
- `Source Client/source/CCRC32.h` bytes=915 sha256=`04ff5727d4477d9478745badf71833f628626323f33e303c337c73b08c13c370`
- `Source Client/source/Define.h` bytes=91 sha256=`5e582b9797d8fd6b20754cb0e00ca0657a88ee0c6b4f4c9f383f9ad057c5adec`
- `Source Client/source/GMAida.h` bytes=989 sha256=`b41e71ba6422b30ccf4e6bfef871cd3bdea0033347837a3c4208c6f66ba32973`
- `Source Client/source/GOBoid.h` bytes=1009 sha256=`70ee03016810c9c18832b8d7aa3738da78c804c6478a2f9e97692f58b4bb2188`
- `Source Client/source/MixMgr.h` bytes=12034 sha256=`321c7d57ddc4b91b3010b1efd25f8c41abc6076e673d99bf40bc55fe49e55a96`
- `Source Client/source/MsgWin.h` bytes=1183 sha256=`cfb8c8d3d350fe41c626e87c3c646c65450ce4a7d4ec5ecc6b0f7c84b5de8ea0`
- `Source Client/source/Slider.h` bytes=1530 sha256=`9e50f9831c5270137d6e234b2d237831de03ce48a28afd57decf7c45729dcfee`
- `Source Client/source/Sprite.h` bytes=3067 sha256=`76d533f8fc7da80d4a9bdb351d5fbc66b53839c23d4d1757afff8b99091f3d7f`
- `Source Client/source/_types.h` bytes=2222 sha256=`28369eb6108716aa2a33c30330eb2ffad4f4f64a892a4fa9eee6841ed2363f7d`
- `Source Client/source/stdafx.h` bytes=2139 sha256=`d004d4d1ce1e7ca0af511976ba7da9439072a0807e4bd98cfa70e17b25a2934f`
- `Source Client/source/w_Buff.h` bytes=1096 sha256=`93b5d34d78e6176d6f014cbbe3bc66298b9546553650c5d47a9a79b6b16d9150`
- `Source Client/source/BaseCls.h` bytes=18438 sha256=`9d87d26aeed5a5f132e3179f7fe275ce1a60c0f618ee58982405fc028ff40ef9`
- `Source Client/source/Builder.h` bytes=1068 sha256=`f128d9c56a4843eac56eedc252cfb5669dcb832736332c135bed1dd4644b9657`
- `Source Client/source/CComGem.h` bytes=1742 sha256=`797fea4fa2b11e61ac4a2e6a1286899ebbb26866621d9787914c3bbb9f813b28`
- `Source Client/source/CSParts.h` bytes=1480 sha256=`19ce441097c88b41dea396a3d677d05b536413d1549b3ab315d24ada7b410586`
- `Source Client/source/CSQuest.h` bytes=2386 sha256=`8a8a41999d1643f3b38cb2ad25b9c9a25c22d52ac9c5c59ca10319e4126090dc`
- `Source Client/source/DuelMgr.h` bytes=2562 sha256=`6cdcde6e6bdbcea592bce46deb83189a282fa97e60500566de2a76712a5bf199`
- `Source Client/source/Protect.h` bytes=409 sha256=`0131051b77e952a266785460654b60b86a0565c9cd9aa6bf60491d09dfdb7470`
- `Source Client/source/UIGuild.h` bytes=69 sha256=`3e17ed538f5cc1b7e74fe45c90ef9b9c502308fdd3d91b2fbd2f8fb1e39373d7`
- `Source Client/source/UIPopup.h` bytes=2061 sha256=`37e4548caa61e9cb48ddca0828726f0b81da6e88ac8930ad6804b6e534f43b14`
- `Source Client/source/Winmain.h` bytes=2517 sha256=`3ba0577b1aea96a3f03113a35491b786b9fd18ab85d40e016a04bb57984e8ccf`
- `Source Client/source/_define.h` bytes=18981 sha256=`c2fa25d74def49ff12b76b5918e3108bb14e99b04a8bf47c77eefb84a3f7204f`
- `Source Client/source/_struct.h` bytes=12146 sha256=`35fa9ac36b65702713df8613f32d800251a1332d91d1b273335428793710b620`
- `Source Client/source/BlackWin.h` bytes=380 sha256=`8a43ba8824a324e883481034cfebefad88433b9e1392470ac8a06ccd3d9c0849`
- `Source Client/source/DSwaveIO.h` bytes=1006 sha256=`c1aa647b1db9f6c8856e1c59ea9ae0cd04e675ea7d5fefc886bb0bf2ea5b5748`
- `Source Client/source/GMGmArea.h` bytes=127 sha256=`c074674531eb21e7590c52fb968a2687e47b8c38e879efcc27497ddde75e574a`
- `Source Client/source/GMHellas.h` bytes=2465 sha256=`bfc1df5ac443530fa764b5e8ba92e2138976bc95eadf55cd554a27a8930ac283`
- `Source Client/source/GaugeBar.h` bytes=1060 sha256=`c5db2c559b396bee0797cd3d90015c1187930dd8412489987e89f4653de737fe`
- `Source Client/source/ItemInfo.h` bytes=1218 sha256=`6663a631fa9f5e0c4de20cd5299e1cc1d9b8153ed87bd0a2aa7ba15e7ba996f3`
- `Source Client/source/LoadData.h` bytes=413 sha256=`e8bf2abcc5f4c00cdf0775bff6a798e25a500523f359e34f5f72d5d2cc89d888`
- `Source Client/source/LoginWin.h` bytes=955 sha256=`90c6f276cb4c266aacd06aa5d894d4eb8c55dc774a50674d8bee11db8e7ce821`
- `Source Client/source/Nprotect.h` bytes=743 sha256=`9c222c791ac570486eee2eb968d888de182a36fd2a81a6c8b58f526ec6941de1`
- `Source Client/source/Observer.h` bytes=830 sha256=`9faaf8002934acb5cd49a541c33bab2ab9f195c729129c7e5ff7677dfa2567d1`
- `Source Client/source/QuestMng.h` bytes=3663 sha256=`7c52cb8939a1508c0d93aa6967810fd17bb68e0d31bf63dc91ed575425645fee`
- `Source Client/source/SideHair.h` bytes=816 sha256=`a2cba12685c606806d69be978d531873deb3ded5ea35b5a100fb803febf5e750`
- `Source Client/source/resource.h` bytes=615 sha256=`bb22154038c7eb5844c1c9e47a3dde3c7d8fce96c1417d02fe230b970f1a21d7`
- `Source Client/source/CreditWin.h` bytes=1982 sha256=`52d3584a65b1dc10fd1c0131704501870bc37fb9b02e0e77ad801f60380accbc`
- `Source Client/source/DSWavRead.h` bytes=1166 sha256=`9991adbe301c09df61f0c2603c5faa9add41139717cd649b90325acba712e5f0`
- `Source Client/source/DebugSkip.h` bytes=1054 sha256=`6d4d62a6de15c9f5235f032a60adddc3850cc49cac98192abc4af562bf711dca`
- `Source Client/source/GMNewTown.h` bytes=1670 sha256=`5d4933b510c7c26b0a99c304d59b1700541f7e62b14a1761eb78633e3c0e219a`
- `Source Client/source/NewUIBase.h` bytes=1092 sha256=`5752eba2adae3c7b60ee49197f863643e2aa1c648704fadc196f5bc0f2800d06`
- `Source Client/source/OptionWin.h` bytes=896 sha256=`dfa123491ac0ccd690b416e35264f7fe04e5a0e8c5aaabf29e381efc7dc038bf`
- `Source Client/source/PortalMgr.h` bytes=709 sha256=`2bc7b8c5d8ce5c2389f89075d45a7d40e02f7ccf28ae203c09b896363efa5fea`
- `Source Client/source/QuestInfo.h` bytes=1204 sha256=`1e849d54424e367ed8a5feca04014e65b236777532707ffb917cd57d28275067`
- `Source Client/source/Singleton.h` bytes=1196 sha256=`1257d16d883e3b2896b967ca6adbeeb675d9ccf5528e02a04e7e4a3b83b8e265`

---

## Token hits (headers que contêm tokens relevantes)

### `Source Client/source/WSclient.h`
- url: https://raw.githubusercontent.com/wongddd/muonline/580472e0d5723f9709cbad594f233deb07f9f351/Source%20Client/source/WSclient.h
- bytes: 91012
- sha256: `19d38a3b6506a745662c14dd7cdd6e41ca3ad29b28b17c76f860cd9e67799c10`
- tokens: RECEIVE_ACTION, LPPRECEIVE_ACTION, RECEIVE_MAGIC, LPPRECEIVE_MAGIC, #pragma pack

#### CONTEXT: LPPRECEIVE_ACTION (first hit @ line 736)
```
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
   693: 	BYTE          ExpL;
   694: 	BYTE          DamageH;
   695: 	BYTE          DamageL;
   696: } PRECEIVE_DIE, * LPPRECEIVE_DIE;
   697: typedef struct {
   698: 	PBMSG_HEADER  Header;
   699: 	BYTE          KeyH;
   700: 	BYTE          KeyL;
   701: 	WORD          ExpH;
   702: 	WORD          ExpL;
   703: 	BYTE          DamageH;
   704: 	BYTE          DamageL;
   705: } PRECEIVE_DIE2, * LPPRECEIVE_DIE2;
   706: 
   707: //receive default key
   708: typedef struct {
   709: 	PBMSG_HEADER Header;
   710: 	BYTE         KeyH;
   711: 	BYTE         KeyL;
   712: 	BYTE          MagicH;
   713: 	BYTE          MagicL;
   714: 	BYTE		 TKeyH;
   715: 	BYTE		 TKeyL;
   716: } PHEADER_DEFAULT_DIE, * LPPHEADER_DEFAULT_DIE;
   717: 
   718: //request action
   719: typedef struct {
   720: 	PBMSG_HEADER  Header;
   721: 	BYTE          Angle;
   722: 	BYTE          Action;
   723: 	BYTE		  TargetKeyH;
   724: 	BYTE		  TargetKeyL;
   725: } PREQUEST_ACTION, * LPPREQUEST_ACTION;
   726: 
   727: //receive action
   728: typedef struct {
   729: 	PBMSG_HEADER  Header;
   730: 	BYTE          KeyH;
   731: 	BYTE          KeyL;
   732: 	BYTE          Angle;
   733: 	BYTE          Action;
   734: 	BYTE		  TargetKeyH;
   735: 	BYTE		  TargetKeyL;
   736: } PRECEIVE_ACTION, * LPPRECEIVE_ACTION;
   737: 
   738: //send magic
   739: typedef struct {
   740: 	PBMSG_HEADER  Header;
   741: 	BYTE          MagicH;
   742: 	BYTE          MagicL;
   743: 	BYTE          KeyH;
   744: 	BYTE          KeyL;
   745: } PSEND_MAGIC, * LPPSEND_MAGIC;
   746: 
   747: //send magic continue
   748: typedef struct {
   749: 	PBMSG_HEADER  Header;
   750: 	BYTE          MagicH;
   751: 	BYTE          MagicL;
   752: 	BYTE          PositionX;
   753: 	BYTE          PositionY;
   754: 	BYTE          Angle;
   755: } PSEND_MAGIC_CONTINUE, * LPPSEND_MAGIC_CONTINUE;
   756: 
   757: //receive magic continue
   758: typedef struct {
   759: 	PBMSG_HEADER  Header;
   760: 	BYTE          MagicH;
   761: 	BYTE          MagicL;
   762: 	BYTE          KeyH;
   763: 	BYTE          KeyL;
   764: 	BYTE          PositionX;
   765: 	BYTE          PositionY;
   766: 	BYTE          Angle;
   767: } PRECEIVE_MAGIC_CONTINUE, * LPPRECEIVE_MAGIC_CONTINUE;
   768: 
   769: //receive magic
   770: typedef struct {
   771: 	PBMSG_HEADER  Header;
   772: 	BYTE          MagicH;
   773: 	BYTE          MagicL;
   774: 	BYTE          SourceKeyH;
   775: 	BYTE          SourceKeyL;
   776: 	BYTE          TargetKeyH;
   777: 	BYTE          TargetKeyL;
   778: } PRECEIVE_MAGIC, * LPPRECEIVE_MAGIC;
   779: 
   780: //receive MonsterSkill
   781: typedef struct {
   782: 	PBMSG_HEADER  Header;
   783: 	BYTE          MagicH;
   784: 	BYTE          MagicL;
   785: 	WORD		  SourceKey;
   786: 	WORD		  TargetKey;
   787: } PRECEIVE_MONSTERSKILL, * LPPRECEIVE_MONSTERSKILL;
   788: 
   789: //receive magic target
   790: typedef struct {
   791: 	BYTE          KeyH;
   792: 	BYTE          KeyL;
   793: } PRECEIVE_MAGIC_POSITION, * LPPRECEIVE_MAGIC_POSITION;
   794: 
   795: //send magic continue
   796: typedef struct {
   797: 	PBMSG_HEADER  Header;
   798: 	BYTE          MagicH;
   799: 	BYTE          MagicL;
   800: 	BYTE          PositionX;
   801: 	BYTE          PositionY;
   802: 	BYTE          Count;
   803: 	BYTE          Key[10];
   804: } PSEND_MAGIC_CONTINUES, * LPPSEND_MAGIC_CONTINUES;
   805: 
   806: //receive magic target
   807: typedef struct {
   808: 	PBMSG_HEADER  Header;
   809: 	BYTE          KeyH;
   810: 	BYTE          KeyL;
   811: 
   812: 	BYTE          MagicH;
   813: 	BYTE          MagicL;
   814: 
   815: 	BYTE          PositionX;
   816: 	BYTE          PositionY;
   817: 	BYTE          Count;
   818: } PRECEIVE_MAGIC_POSITIONS, * LPPRECEIVE_MAGIC_POSITIONS;
   819: 
   820: //receive magic list count
   821: typedef struct {
   822: 	PBMSG_HEADER Header;
   823: 	BYTE         SubCode;
   824: 	BYTE         Value;
   825:     BYTE         ListType;
   826: } PHEADER_MAGIC_LIST_COUNT, * LPPHEADER_MAGIC_LIST_COUNT;
   827: 
   828: #pragma pack(push, 1)
   829: //receive magic target
   830: typedef struct {
   831: 	BYTE          Index;
   832: 
   833: 	WORD		  Type;
   834: 	BYTE          Level;
   835: } PRECEIVE_MAGIC_LIST, * LPPRECEIVE_MAGIC_LIST;
   836: #pragma pack(pop)
   837: 
   838: //receive skill count.
   839: typedef	struct {
   840: 	PBMSG_HEADER  Header;
   841: 	BYTE          KeyH;
   842: 	BYTE          KeyL;
   843: 	BYTE		  m_byType;
   844: 	BYTE		  m_byCount;
   845: } PRECEIVE_EX_SKILL_COUNT, * LPPRECEIVE_EX_SKILL_COUNT;
   846: 
   847: //receive gold
   848: typedef struct {
   849: 	PBMSG_HEADER  Header;
   850: 	BYTE          Flag;
   851: 	DWORD         Gold;
   852: } PRECEIVE_GOLD, * LPPRECEIVE_GOLD;
   853: 
   854: //receive repair gold
   855: typedef struct {
   856: 	PBMSG_HEADER  Header;
   857: 	DWORD         Gold;
   858: } PRECEIVE_REPAIR_GOLD, * LPPRECEIVE_REPAIR_GOLD;
   859: 
   860: //receive level up
   861: typedef struct {
   862: 	PBMSG_HEADER Header;
   863: 	BYTE         SubCode;
   864: 	WORD         Level;
   865: 	WORD         LevelUpPoint;
   866: 	WORD         MaxLife;
   867: 	WORD         MaxMana;
   868: 	WORD		 MaxShield;
   869: 	WORD		 SkillManaMax;
   870:     short        AddPoint;
   871:     short        MaxAddPoint;
   872: 	WORD		 wMinusPoint;
   873: 	WORD		 wMaxMinusPoint;
   874: } PRECEIVE_LEVEL_UP, * LPPRECEIVE_LEVEL_UP;
   875: 
   876: typedef struct {
   877: 	PBMSG_HEADER  Header;
   878: 	BYTE          Index;
   879: 	BYTE		  Life[5];
   880: } PRECEIVE_LIFE, * LPPRECEIVE_LIFE;
   881: 
   882: //receive add point
   883: typedef struct {
   884: 	PBMSG_HEADER Header;
   885: 	BYTE         SubCode;
   886: 	BYTE         Result;
   887: 	WORD         Max;
   888: 	WORD		 ShieldMax;
   889: 	WORD		 SkillManaMax;
   890: } PRECEIVE_ADD_POINT, * LPPRECEIVE_ADD_POINT;
   891: 
   892: typedef struct {
   893: 	PBMSG_HEADER Header;
   894: 	BYTE         KeyH;
   895: 	BYTE         KeyL;
   896: 	BYTE         PositionX;
   897: 	BYTE         PositionY;
   898: } PRECEIVE_MOVE_POSITION, * LPPRECEIVE_MOVE_POSITION;
   899: 
   900: typedef struct {
   901: 	PBMSG_HEADER Header;
   902: 	WORD		 Flag;
   903: 	BYTE         Map;
   904: 	BYTE         PositionX;
   905: 	BYTE         PositionY;
   906: 	BYTE         Angle;
   907: } PRECEIVE_TELEPORT_POSITION, * LPPRECEIVE_TELEPORT_POSITION;
   908: 
   909: //receive damage
   910: typedef struct {
   911: 	PBMSG_HEADER Header;
   912: 	BYTE         SubCode;
   913: 	BYTE         DamageH;
   914: 	BYTE         DamageL;
   915: 	BYTE		 ShieldDamageH;
   916: 	BYTE		 ShieldDamageL;
   917: } PRECEIVE_DAMAGE, * LPPRECEIVE_DAMAGE;
   918: 
   919: //receive create guild master
   920: typedef struct {
   921: 	PBMSG_HEADER Header;
   922: 	BYTE         Name[8];
   923: 	BYTE         Mark[32];
   924: } PSEND_CREATE_GUILD, * LPPSEND_CREATE_GUILD;
   925: 
   926: //receive party info
   927: typedef struct {
   928:     BYTE         value;
   929: } PRECEIVE_PARTY_INFO, * LPPRECEIVE_PARTY_INFO;
   930: 
   931: //receive party infos
   932: typedef struct {
   933: 	PBMSG_HEADER Header;
   934:     BYTE         Count;
   935: } PRECEIVE_PARTY_INFOS, * LPPRECEIVE_PARTY_INFOS;
   936: 
   937: //receive party list
   938: typedef struct {
   939: 	BYTE         ID[MAX_ID_SIZE];
   940: 	BYTE         Number;
   941: 	BYTE         Map;
   942: 	BYTE         x;
   943: 	BYTE         y;
   944:     int          currHP;
   945:     int          maxHP;
   946: } PRECEIVE_PARTY_LIST, * LPPRECEIVE_PARTY_LIST;
   947: 
   948: //receive party list
   949: typedef struct {
   950: 	PBMSG_HEADER Header;
   951: 	BYTE         Result;
   952: 	BYTE         Count;
   953: } PRECEIVE_PARTY_LISTS, * LPPRECEIVE_PARTY_LISTS;
   954: 
   955: typedef struct {
   956:     PBMSG_HEADER Header;
```

#### CONTEXT: LPPRECEIVE_MAGIC (first hit @ line 778)
```
   718: //request action
   719: typedef struct {
   720: 	PBMSG_HEADER  Header;
   721: 	BYTE          Angle;
   722: 	BYTE          Action;
   723: 	BYTE		  TargetKeyH;
   724: 	BYTE		  TargetKeyL;
   725: } PREQUEST_ACTION, * LPPREQUEST_ACTION;
   726: 
   727: //receive action
   728: typedef struct {
   729: 	PBMSG_HEADER  Header;
   730: 	BYTE          KeyH;
   731: 	BYTE          KeyL;
   732: 	BYTE          Angle;
   733: 	BYTE          Action;
   734: 	BYTE		  TargetKeyH;
   735: 	BYTE		  TargetKeyL;
   736: } PRECEIVE_ACTION, * LPPRECEIVE_ACTION;
   737: 
   738: //send magic
   739: typedef struct {
   740: 	PBMSG_HEADER  Header;
   741: 	BYTE          MagicH;
   742: 	BYTE          MagicL;
   743: 	BYTE          KeyH;
   744: 	BYTE          KeyL;
   745: } PSEND_MAGIC, * LPPSEND_MAGIC;
   746: 
   747: //send magic continue
   748: typedef struct {
   749: 	PBMSG_HEADER  Header;
   750: 	BYTE          MagicH;
   751: 	BYTE          MagicL;
   752: 	BYTE          PositionX;
   753: 	BYTE          PositionY;
   754: 	BYTE          Angle;
   755: } PSEND_MAGIC_CONTINUE, * LPPSEND_MAGIC_CONTINUE;
   756: 
   757: //receive magic continue
   758: typedef struct {
   759: 	PBMSG_HEADER  Header;
   760: 	BYTE          MagicH;
   761: 	BYTE          MagicL;
   762: 	BYTE          KeyH;
   763: 	BYTE          KeyL;
   764: 	BYTE          PositionX;
   765: 	BYTE          PositionY;
   766: 	BYTE          Angle;
   767: } PRECEIVE_MAGIC_CONTINUE, * LPPRECEIVE_MAGIC_CONTINUE;
   768: 
   769: //receive magic
   770: typedef struct {
   771: 	PBMSG_HEADER  Header;
   772: 	BYTE          MagicH;
   773: 	BYTE          MagicL;
   774: 	BYTE          SourceKeyH;
   775: 	BYTE          SourceKeyL;
   776: 	BYTE          TargetKeyH;
   777: 	BYTE          TargetKeyL;
   778: } PRECEIVE_MAGIC, * LPPRECEIVE_MAGIC;
   779: 
   780: //receive MonsterSkill
   781: typedef struct {
   782: 	PBMSG_HEADER  Header;
   783: 	BYTE          MagicH;
   784: 	BYTE          MagicL;
   785: 	WORD		  SourceKey;
   786: 	WORD		  TargetKey;
   787: } PRECEIVE_MONSTERSKILL, * LPPRECEIVE_MONSTERSKILL;
   788: 
   789: //receive magic target
   790: typedef struct {
   791: 	BYTE          KeyH;
   792: 	BYTE          KeyL;
   793: } PRECEIVE_MAGIC_POSITION, * LPPRECEIVE_MAGIC_POSITION;
   794: 
   795: //send magic continue
   796: typedef struct {
   797: 	PBMSG_HEADER  Header;
   798: 	BYTE          MagicH;
   799: 	BYTE          MagicL;
   800: 	BYTE          PositionX;
   801: 	BYTE          PositionY;
   802: 	BYTE          Count;
   803: 	BYTE          Key[10];
   804: } PSEND_MAGIC_CONTINUES, * LPPSEND_MAGIC_CONTINUES;
   805: 
   806: //receive magic target
   807: typedef struct {
   808: 	PBMSG_HEADER  Header;
   809: 	BYTE          KeyH;
   810: 	BYTE          KeyL;
   811: 
   812: 	BYTE          MagicH;
   813: 	BYTE          MagicL;
   814: 
   815: 	BYTE          PositionX;
   816: 	BYTE          PositionY;
   817: 	BYTE          Count;
   818: } PRECEIVE_MAGIC_POSITIONS, * LPPRECEIVE_MAGIC_POSITIONS;
   819: 
   820: //receive magic list count
   821: typedef struct {
   822: 	PBMSG_HEADER Header;
   823: 	BYTE         SubCode;
   824: 	BYTE         Value;
   825:     BYTE         ListType;
   826: } PHEADER_MAGIC_LIST_COUNT, * LPPHEADER_MAGIC_LIST_COUNT;
   827: 
   828: #pragma pack(push, 1)
   829: //receive magic target
   830: typedef struct {
   831: 	BYTE          Index;
   832: 
   833: 	WORD		  Type;
   834: 	BYTE          Level;
   835: } PRECEIVE_MAGIC_LIST, * LPPRECEIVE_MAGIC_LIST;
   836: #pragma pack(pop)
   837: 
   838: //receive skill count.
   839: typedef	struct {
   840: 	PBMSG_HEADER  Header;
   841: 	BYTE          KeyH;
   842: 	BYTE          KeyL;
   843: 	BYTE		  m_byType;
   844: 	BYTE		  m_byCount;
   845: } PRECEIVE_EX_SKILL_COUNT, * LPPRECEIVE_EX_SKILL_COUNT;
   846: 
   847: //receive gold
   848: typedef struct {
   849: 	PBMSG_HEADER  Header;
   850: 	BYTE          Flag;
   851: 	DWORD         Gold;
   852: } PRECEIVE_GOLD, * LPPRECEIVE_GOLD;
   853: 
   854: //receive repair gold
   855: typedef struct {
   856: 	PBMSG_HEADER  Header;
   857: 	DWORD         Gold;
   858: } PRECEIVE_REPAIR_GOLD, * LPPRECEIVE_REPAIR_GOLD;
   859: 
   860: //receive level up
   861: typedef struct {
   862: 	PBMSG_HEADER Header;
   863: 	BYTE         SubCode;
   864: 	WORD         Level;
   865: 	WORD         LevelUpPoint;
   866: 	WORD         MaxLife;
   867: 	WORD         MaxMana;
   868: 	WORD		 MaxShield;
   869: 	WORD		 SkillManaMax;
   870:     short        AddPoint;
   871:     short        MaxAddPoint;
   872: 	WORD		 wMinusPoint;
   873: 	WORD		 wMaxMinusPoint;
   874: } PRECEIVE_LEVEL_UP, * LPPRECEIVE_LEVEL_UP;
   875: 
   876: typedef struct {
   877: 	PBMSG_HEADER  Header;
   878: 	BYTE          Index;
   879: 	BYTE		  Life[5];
   880: } PRECEIVE_LIFE, * LPPRECEIVE_LIFE;
   881: 
   882: //receive add point
   883: typedef struct {
   884: 	PBMSG_HEADER Header;
   885: 	BYTE         SubCode;
   886: 	BYTE         Result;
   887: 	WORD         Max;
   888: 	WORD		 ShieldMax;
   889: 	WORD		 SkillManaMax;
   890: } PRECEIVE_ADD_POINT, * LPPRECEIVE_ADD_POINT;
   891: 
   892: typedef struct {
   893: 	PBMSG_HEADER Header;
   894: 	BYTE         KeyH;
   895: 	BYTE         KeyL;
   896: 	BYTE         PositionX;
   897: 	BYTE         PositionY;
   898: } PRECEIVE_MOVE_POSITION, * LPPRECEIVE_MOVE_POSITION;
   899: 
   900: typedef struct {
   901: 	PBMSG_HEADER Header;
   902: 	WORD		 Flag;
   903: 	BYTE         Map;
   904: 	BYTE         PositionX;
   905: 	BYTE         PositionY;
   906: 	BYTE         Angle;
   907: } PRECEIVE_TELEPORT_POSITION, * LPPRECEIVE_TELEPORT_POSITION;
   908: 
   909: //receive damage
   910: typedef struct {
   911: 	PBMSG_HEADER Header;
   912: 	BYTE         SubCode;
   913: 	BYTE         DamageH;
   914: 	BYTE         DamageL;
   915: 	BYTE		 ShieldDamageH;
   916: 	BYTE		 ShieldDamageL;
   917: } PRECEIVE_DAMAGE, * LPPRECEIVE_DAMAGE;
   918: 
   919: //receive create guild master
   920: typedef struct {
   921: 	PBMSG_HEADER Header;
   922: 	BYTE         Name[8];
   923: 	BYTE         Mark[32];
   924: } PSEND_CREATE_GUILD, * LPPSEND_CREATE_GUILD;
   925: 
   926: //receive party info
   927: typedef struct {
   928:     BYTE         value;
   929: } PRECEIVE_PARTY_INFO, * LPPRECEIVE_PARTY_INFO;
   930: 
   931: //receive party infos
   932: typedef struct {
   933: 	PBMSG_HEADER Header;
   934:     BYTE         Count;
   935: } PRECEIVE_PARTY_INFOS, * LPPRECEIVE_PARTY_INFOS;
   936: 
   937: //receive party list
   938: typedef struct {
   939: 	BYTE         ID[MAX_ID_SIZE];
   940: 	BYTE         Number;
   941: 	BYTE         Map;
   942: 	BYTE         x;
   943: 	BYTE         y;
   944:     int          currHP;
   945:     int          maxHP;
   946: } PRECEIVE_PARTY_LIST, * LPPRECEIVE_PARTY_LIST;
   947: 
   948: //receive party list
   949: typedef struct {
   950: 	PBMSG_HEADER Header;
   951: 	BYTE         Result;
   952: 	BYTE         Count;
   953: } PRECEIVE_PARTY_LISTS, * LPPRECEIVE_PARTY_LISTS;
   954: 
   955: typedef struct {
   956:     PBMSG_HEADER Header;
   957:     BYTE         KeyH;
   958:     BYTE         KeyL;
   959: 	int			 ItemInfo;
   960: 	BYTE		 ItemLevel;
   961: }PRECEIVE_GETITEMINFO_FOR_PARTY, * LPPRECEIVE_GETITEMINFO_FOR_PARTY;
   962: 
   963: //receive pk
   964: typedef struct {
   965: 	PBMSG_HEADER Header;
   966: 	BYTE         SubCode;
   967: 	BYTE         KeyH;
   968: 	BYTE         KeyL;
   969: 	BYTE         PK;
   970: } PRECEIVE_PK, * LPPRECEIVE_PK;
   971: 
   972: typedef struct {
   973: 	PBMSG_HEADER Header;
   974: 	BYTE         Index;
   975: 	WORD         Time;
   976: } PRECEIVE_HELPER_ITEM, * LPPRECEIVE_HELPER_ITEM;
   977: 
   978: typedef struct {
   979: 	PBMSG_HEADER Header;
   980: 	BYTE         SubCode;
   981: 	BYTE         IP[15];
   982: 	WORD         Port;
   983: } PRECEIVE_SERVER_ADDRESS, * LPPRECEIVE_SERVER_ADDRESS;
   984: 
   985: typedef struct {
   986: 	PBMSG_HEADER Header;
   987: 	BYTE         SubCode;
   988: 	BYTE		 Server;
   989: } PRECEIVE_SERVER_BUSY, * LPPRECEIVE_SERVER_BUSY;
   990: 
   991: typedef struct {
   992: 	BYTE         KeyH;
   993: 	BYTE         KeyL;
   994: 	BYTE         Name[8];
   995: 	BYTE         Mark[32];
   996: } PRECEIVE_GUILD, * LPPRECEIVE_GUILD;
   997: 
   998: typedef struct {
```


---

### `Source Client/source/ProtocolSend.h`
- url: https://raw.githubusercontent.com/wongddd/muonline/580472e0d5723f9709cbad594f233deb07f9f351/Source%20Client/source/ProtocolSend.h
- bytes: 3321
- sha256: `3ff13302150537b3ed7db40aef8c3dd6f5e5b81927e39584b0510d598d97d684`
- tokens: #pragma pack


---

### `Source Client/source/nvapi.h`
- url: https://raw.githubusercontent.com/wongddd/muonline/580472e0d5723f9709cbad594f233deb07f9f351/Source%20Client/source/nvapi.h
- bytes: 100622
- sha256: `e605db63e2a7521962f015248f0fa7bd457c11cecdb44b810856644aca25c3fe`
- tokens: #pragma pack


---

### `Source Client/source/_struct.h`
- url: https://raw.githubusercontent.com/wongddd/muonline/580472e0d5723f9709cbad594f233deb07f9f351/Source%20Client/source/_struct.h
- bytes: 12146
- sha256: `35fa9ac36b65702713df8613f32d800251a1332d91d1b273335428793710b620`
- tokens: #pragma pack


---

### `Source Client/source/QuestMng.h`
- url: https://raw.githubusercontent.com/wongddd/muonline/580472e0d5723f9709cbad594f233deb07f9f351/Source%20Client/source/QuestMng.h
- bytes: 3663
- sha256: `7c52cb8939a1508c0d93aa6967810fd17bb68e0d31bf63dc91ed575425645fee`
- tokens: #pragma pack


---

## QUESTIONS (preencher como CONFIRMED/PROBABLE com âncora)

1) Em qual header estão `RECEIVE_ACTION` e `RECEIVE_MAGIC`?

2) Ordem exata de campos + tipos (u8/u16/etc) e se há `#pragma pack` (qual).

3) Tamanho on-wire esperado (C1?) e se existe subheader/packing adicional.
