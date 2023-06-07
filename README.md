# Mobile Checker

A Mobile Checker, you can check player device!

## Main features
* Изменить клиента игрока
* Включение отображения статуса игрока (Мобильный игрок/Клиент)
* Использование PawnRak.Net при наличии
## Installation

Include in your code and begin using the library:
```pawn
#include <mobile_checker.inc>
```

## Example

```pawn
CMD:checkdevice(playerid, params[])
{
    extract params -> new target_id; else return true;
    new string[144];
    GetPlayerDevice(target_id, string);
    
    return SendClientMessage(playerid, -1, string);
}
```

## Dependencies
* PAWNRAKNET
* a_samp

## Used Interceptions
 * Mobile_OnPlayerConnect
 * Mobile_OnPlayerSpawn
 * Mobile_OnPlayerDisconnect
 * Mobile_OnGameModeInit
 
## Stocks
 * GetDeviceInfo(playerid, string[], len = sizeof string)
 * GetClientID(playerid)
 * ChangeMobileType(playerid, type)
 * ChangePlayerHash(playerid, hash[])
