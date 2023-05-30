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
    
    new fmt_str[] = "Player device type: %s";
    new string[sizeof(fmt_str) + 7];
    format(string, sizeof(string), fmt_str, GetPlayerDevice(target_id));
    
    return SendClientMessage(playerid, -1, string);
}
```
