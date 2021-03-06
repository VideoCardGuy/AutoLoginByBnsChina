#ifndef __AUTOLOGINBYBNSCHINA_BNSDLL_GAMESTRUCT_H__
#define __AUTOLOGINBYBNSCHINA_BNSDLL_GAMESTRUCT_H__

#include <memory>
#include <algorithm>
#include <vector>
#include <MyTools/CLLog.h>
#include <MyTools/ClassInstance.h>
#include <MyTools/Character.h>

#define SZFILE_NAME_SHAREDINFO	L"Bns_China_Share_Mem"

#define ReadDWORD(x)								CCharacter::ReadDWORD(x)
#define ReadBYTE(x)									CCharacter::ReadBYTE(x)
#define ReadFloat(x)								CCharacter::ReadFloat(x)
#define ReadDouble(x)								CCharacter::ReadDouble(x)

enum em_Base
{
	em_Base_Person,
	em_Base_PersonOffset1,
	em_Base_PersonOffset2,
	em_Base_PersonOffset3,
	em_Base_PlayerTraverseOffset,
	em_Base_Volume
};

enum em_Player_Classes
{
	// 力士
	em_Player_Classes_Destroyer = 0x5,
	// 刺客
	em_Player_Classes_Assassin = 0x7,
	// 剑士
	em_Player_Classes_BladeMaster = 0x1,
	// 召唤师
	em_Player_Classes_Summoner = 0x6,
	// 气功
	em_Player_Classes_ForceMaster = 0x3,
	// 拳师
	em_Player_Classes_KongFuMaster = 0x2,
	// 灵剑士
	em_Player_Classes_Danncer = 0x8,
	// 咒术师
	em_Player_Classes_Warlock = 0x9,
	// 气宗
	em_Player_Classes_KongFuForceMaster = 0xA

};

struct AccountConfig
{
	std::wstring		wsAccountName;
	std::wstring		wsAccountPass;

	VOID Clear()
	{
		wsAccountName = wsAccountPass = L"";
	}
};

struct AccountPlayerConfig
{
	UINT				uIndex;
	UINT				uLevel;
	em_Player_Classes	emPlayerClass;
	std::wstring		wsPlayerName;

	VOID Clear()
	{
		uIndex = uLevel = NULL;
		wsPlayerName = L"";
	}
};

struct TextAccountSchedule
{
	AccountConfig				AccountContent;
	BOOL						bFinish;
	UINT						uVolume;
	vector<AccountPlayerConfig> AccountPlayerConfigVec;

	VOID Clear()
	{
		bFinish = FALSE;
		uVolume = NULL;
		AccountContent.Clear();
		AccountPlayerConfigVec.clear();
	}
};

enum em_Console_Variable
{

};

///////////////帐号///////////////////////////////////////////////////////////
typedef struct _Account_Info
{
	WCHAR szUserName[64];									//	帐号
	WCHAR szPassWord[32];									//	密码
	UINT uVolumn;											//  点卷
}ACCOUNT_INFO, *PACCOUNT_INFO;


typedef struct _AccountRunStatus
{
	BOOL bDone;												// 是否刷完了这个号
	BOOL bExist;											// 该帐号已经被占用了
	BOOL bLogining;											// 是否登录中
	BOOL bClose;											// 是否强制关闭该帐号
	BOOL bPassInvalid;										// 密码错误
}AccountRunStatus;

typedef struct _AccountLog
{
	BOOL bOccupy;											// 是否被占用
	WCHAR szMsg[64];										// 日志
}AccountLog;

struct Account_Player_Info
{
	UINT uIndex;											// 角色索引
	UINT uLevel;											// 角色等级
	em_Player_Classes emPlayerClasses;						// 职业
	WCHAR wszPlayerName[64];								// 角色名字
};

#define MAX_PLAYER_COUNT 10
typedef struct _Account_Info_GAME
{
	ACCOUNT_INFO MyAcount_Info;								// 帐号信息
	AccountLog   AccountLog_;								// 帐号日志
	DWORD dwPid;											// 进程ID
	HWND hGameWnd;											// 窗口句柄
	HWND hLauncher;											// 
	AccountRunStatus AccountStatus;							// 帐号状态
	UINT uAccountPlayerCount;								// 
	Account_Player_Info AccountPlayerArr[MAX_PLAYER_COUNT];	// 
	UINT uLoginFaildCount;									// 登录失败次数

	LPCWSTR GetAccName()
	{
		return this->GetAccountInfo()->szUserName;
	}
	PACCOUNT_INFO GetAccountInfo()
	{
		return &this->MyAcount_Info;
	}
	VOID Clear()
	{
		ZeroMemory(AccountPlayerArr, sizeof(Account_Player_Info) * MAX_PLAYER_COUNT);
		uAccountPlayerCount = NULL;
		dwPid = NULL;
		hGameWnd = NULL;
		AccountStatus.bDone = FALSE;
		AccountStatus.bClose = FALSE;
		AccountStatus.bLogining = FALSE;
		AccountStatus.bPassInvalid = FALSE;
	}
}ACCOUNT_INFO_GAME, *PACCOUNT_INFO_GAME;


#define MAX_GAME_COUNT				1000
typedef struct _Shared_Info
{
	WCHAR szConsolePath[MAX_PATH];							// 控制台路径
	WCHAR wszGamePath[MAX_PATH];							// 游戏路径
	int nAccountCount;										// Size
	ACCOUNT_INFO_GAME arrGameInfo[MAX_GAME_COUNT];			// 数组

	PACCOUNT_INFO_GAME ExistEmptyArrGameInfo()
	{
		auto itr = std::find_if(std::begin(arrGameInfo), std::end(arrGameInfo), [](ACCOUNT_INFO_GAME& AccountGame) {
			return !AccountGame.AccountStatus.bExist;
		});

		return itr == std::end(arrGameInfo) ? nullptr : itr;
	}
	PACCOUNT_INFO_GAME GetCurrentAccountGame(_In_ DWORD dwPid)
	{
		auto itr = std::find_if(std::begin(arrGameInfo), std::end(arrGameInfo), [&dwPid](ACCOUNT_INFO_GAME& AccountGame) {
			return AccountGame.dwPid == dwPid;
		});
		return itr == std::end(arrGameInfo) ? nullptr : itr;
	}
	PACCOUNT_INFO_GAME GetUnLoginArrGameInfo()
	{
		auto itr = std::find_if(std::begin(arrGameInfo), std::end(arrGameInfo), [](ACCOUNT_INFO_GAME& AccountGame) {
			return !AccountGame.AccountStatus.bDone && AccountGame.AccountStatus.bExist;
		});
		return itr == std::end(arrGameInfo) ? nullptr : itr;
	}
	VOID Clear()
	{
		nAccountCount = NULL;
		ZeroMemory(arrGameInfo, sizeof(ACCOUNT_INFO_GAME) * MAX_GAME_COUNT);
	}
}SHARED_INFO, *PSHARED_INFO;

#endif