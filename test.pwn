#include < a_samp >
#define AUTO_OBJECT_ALIAS_ON
#include < YSI_Data\y_iterate >
#include < automatic-objects>
#include < YSI_Visual\y_commands >

main() 
{
    return;
}

new idd;
public OnGameModeInit()
{
    idd = CreateAutomaticObject(19870, 35, -7, 10, 15, -30, 55, 14, 0, 0, 0, 0, 0, 0, 35, -7, 29, 1, 0, 0);
    idd = CreateAutomaticObject(19870, 35, -7, 10, 15, -30, 55, 14, 0, 0, 0, 0, 0, 0, 35, -7, 29, 1, 0, 0);
    return 1;
}

public OnAutomaticObjectStateChange(objectid, oldstate, newstate) 
{
    if(newstate == AUTOMATIC_OBJECT_STATE_CREATED) 
    {
        return print("Working");
    }
    return 1;
}

CMD:test(playerid, params[])
{
    if(IsPlayerInAnyAutomaticArea(playerid)) {
        SendClientMessage(playerid, -1, "Test true");
    }
    else {
        SendClientMessage(playerid, -1, "Test false");
    }
    return 1;
}

CMD:testany(playerid, params[])
{
    if(IsPlayerInAutomaticArea(playerid, idd)) {
        SendClientMessage(playerid, -1, "Test true");
    }
    else {
        SendClientMessage(playerid, -1, "Test false");
    }
    return 1;
}