#if defined _mobile_checker__included_
	#endinput
#endif
#define _mobile_checker__included_

#if !defined PAWNRAKNET_INC_
	#define _mobile_checker_status_noraknet
#endif

#if defined PAWNRAKNET_INC_
	#define _mobile_checker_status_raknet
#endif

#if !defined gpci
	native gpci(playerid, buffer[], size = sizeof(buffer));
#endif

#if !defined GetPlayerClientID
	native GetPlayerClientID(playerid, buffer[], size = sizeof(buffer)) = gpci;
#endif 

#if !defined HTTP 
	#include <a_http>
#endif

#define DEVICE_TYPE_MOBILE      		1
#define DEVICE_TYPE_PC      			0

#define HASH_LEN						44
#define MAX_DEVICE_NAME					7
#define MAX_CLIENT_NAME					22

#define MAX_PERCENT_MEANING				80
#define MIN_PACKET_SIZE     			3

#define MOBILE_CHECKER_VERSION 			"0.5"

const ID_CUSTOM_SYNC = 221;
const RPC_INIT_MOBILE = 0x10;

static g_player_client[MAX_PLAYERS];

enum E_CLIENT_STRUCTURE {
	e_szAuthKey[HASH_LEN],
	e_szClientName[MAX_CLIENT_NAME],
	e_szDevice[MAX_DEVICE_NAME]
};

enum E_PLAYERS_STRUCTURE {
	e_szPercent = 0,
	bool: isHaveAutoaim = false
};
static g_mobile_data[MAX_PLAYERS][E_PLAYERS_STRUCTURE];

public OnGameModeInit()
{
	#if defined MOBILE_CHECKER_VERSION
		HTTP(1002, HTTP_GET, "text-host.ru/raw/mobile-checker", "", "OnMobileVersionChecked");	
	#endif
	#if defined Mobile_OnGameModeInit
		return Mobile_OnGameModeInit();
	#else
		return true;
	#endif
}
#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif

#define OnGameModeInit Mobile_OnGameModeInit
#if defined Mobile_OnGameModeInit
	forward Mobile_OnGameModeInit();
#endif

static m_structure_authkeys[][E_CLIENT_STRUCTURE] = {
	{"UNDEFINED", "Default", DEVICE_TYPE_PC},

    {"5638413348335738345A4536524D4A524539334B", "Old Jekmant", DEVICE_TYPE_MOBILE},
    {"ED40ED0E8089CC44C08EE9580F4C8C44EE8EE990", "Jekmant", DEVICE_TYPE_MOBILE},
    {"FF2BE5E6F5D9392F57C4E66F7AD78767277C6E4F6B", "Online RP", DEVICE_TYPE_MOBILE},
    {"E02262CF28BC542486C558D4BE9EFB716592AFAF8B", "Codessar/FLIN BY SGMP", DEVICE_TYPE_MOBILE},
    {"10B3D2B1317ADD02CC1F680BC500A8BC0FD7AD42CE7", "grishin", DEVICE_TYPE_MOBILE},
    {"15121F6F18550C00AC4B4F8A167D0379BB0ACA99043", "Monser/Tushoonka", DEVICE_TYPE_MOBILE}
};

public OnPlayerConnect(playerid)
{
	g_player_client[playerid] = 0;
	
    static clientID[HASH_LEN];
    GetPlayerClientID(playerid, clientID, sizeof(clientID));

    for(new i; i < sizeof(m_structure_authkeys); i++) 
	{
        if(!strcmp(clientID, m_structure_authkeys[i][e_szAuthKey]))
		{
			g_player_client[playerid] = i;
			g_mobile_data[playerid][e_szPercent] = g_mobile_data[playerid][e_szPercent] + (MAX_PERCENT_MEANING * 20/100);
		}
    }
    if( (GetPlayerCameraAspectRatio(playerid) >= 1.2470589 ) || (GetPlayerCameraAspectRatio(playerid) <= 1.3333334 ))
    {
    	g_mobile_data[playerid][e_szPercent] = g_mobile_data[playerid][e_szPercent] + (MAX_PERCENT_MEANING * 20/100);
    }

	#if defined Mobile_OnPlayerConnect
		return Mobile_OnPlayerConnect(playerid);
	#else
		return true;
	#endif
}
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif

#define OnPlayerConnect Mobile_OnPlayerConnect
#if defined Mobile_OnPlayerConnect
	forward Mobile_OnPlayerConnect(playerid);
#endif

public OnPlayerDisconnect(playerid, reason)
{
	g_player_client[playerid] = -1;

	#if defined Mobile_OnPlayerDisconnect
		return Mobile_OnPlayerDisconnect(playerid, reason);
	#else
		return true;
	#endif
}
#if defined _ALS_OnPlayerDisconnect
	#undef OnPlayerDisconnect
#else
	#define _ALS_OnPlayerDisconnect
#endif

#define OnPlayerDisconnect Mobile_OnPlayerDisconnect
#if defined Mobile_OnPlayerDisconnect
	forward Mobile_OnPlayerDisconnect(playerid, reason);
#endif

stock GetClientID(const playerid)
{
	if(!IsPlayerConnected(playerid)) return INVALID_PLAYER_ID;

	return g_player_client[playerid];
}

stock GetAllDeviceInfo(const playerid, string[], const len = sizeof string)
{
	format(string, len, "ID: %i - Hash: %s - Device: %s - Chance: %i - Client: %s",
		playerid,
		m_structure_authkeys [g_player_client[playerid]] [e_szAuthKey],
		m_structure_authkeys [g_player_client[playerid]] [e_szDevice] ? ("Mobile") : ("Client"),
		g_mobile_data[playerid][e_szPercent],
		m_structure_authkeys [g_player_client[playerid]] [e_szClientName]
	);
}

stock ChangeMobileType(const playerid, const type)
{
	if(!IsPlayerConnected(playerid)) return INVALID_PLAYER_ID;

    return m_structure_authkeys [g_player_client[playerid]] [e_szDevice] = type;
}

stock ChangePlayerHash(playerid, const hash[])
{
	if(!IsPlayerConnected(playerid)) return INVALID_PLAYER_ID;
	if(strlen(hash) > HASH_LEN) return false;

	m_structure_authkeys [g_player_client[playerid]] [e_szAuthKey] [0] = EOS;

	return
		strins(m_structure_authkeys [g_player_client[playerid]] [e_szAuthKey], hash, 0);
}



#if defined _mobile_checker_status_raknet
	IRawPacket:ID_CUSTOM_SYNC(playerid, BitStream:bs)
	{
		new bytes, rpcid;
		BS_GetNumberOfBytesUsed(bs, bytes);
		if(bytes < MIN_PACKET_SIZE) return false;

		BS_ReadValue(bs, 
			PR_IGNORE_BITS, 8,
			PR_UINT8, rpcid);

		if(rpcid == RPC_INIT_MOBILE)
		{
			new autoaim;
			BS_ReadValue(bs, PR_UINT8, autoaim);

	        g_mobile_data[playerid][e_szPercent] + 25;
			g_mobile_data[playerid][isHaveAutoaim] = bool:autoaim;
		}
		return true;
	}
#endif

forward OnMobileVersionChecked(index, response_code, data[]);
public OnMobileVersionChecked(index, response_code, data[])
{
	if(response_code == 200)
	{
		if(strcmp(data, MOBILE_CHECKER_VERSION))
		{
			return printf("A new version of 'Mobile Checker' is available: %s", data);
		}
	}
	else
	{
		printf("Failed check 'Mobile Checker' to update!");
	}
	return false;
}
