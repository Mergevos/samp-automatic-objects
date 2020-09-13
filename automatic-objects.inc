/*
	* Library: Samp Automatic Kick (advanced_kicks.inc)
	* Author: Mergevos
	* Date: 7. Septembre 2020.
	* Copyright: (C) 2020
	* Credits: 
        omcho - include guards
        samp-incognito - Streamer
        Y_Less - y_iterate * y_hooks
*/

// built-in include guard removal
// just in case the user has a local dependency with the same file name
#if defined _inc_automatic_objects
    #undef _inc_automatic_objects
#endif
// custom include-guard to ensure we don't duplicate
#if defined _automatic_objects_included
    #endinput
#endif
#define _automatic_objects_included

#include < a_samp >
#include < YSI_Data\y_iterate >
#include < YSI_Coding\y_hooks >
#include < streamer >


#if defined AUTO_OBJECT_ALIAS_ON // used as alias
    #define AutoObject_Create       CreateAutomaticObject
    #define AutoObject_Destroy      DestroyAutomaticObject
    #define AutoObject_DestroyAll   DestroyAllAutomaticObject
    #define AutoObj_IsPlayerInAnyAutoArea    IsPlayerInAnyAutomaticArea
    #define AutoObject_IsPlayerInAutoArea    IsPlayerInAutomaticArea 
#endif 

#if !defined MAX_AUTOMATIC_OBJECTS
    #define MAX_AUTOMATIC_OBJECTS 32
#endif

enum {
    AUTOMATIC_OBJECT_STATE_UNDEF = -1,
    AUTOMATIC_OBJECT_STATE_CLOSED = 0, // gate closed
    AUTOMATIC_OBJECT_STATE_OPENED, // gate opened
    AUTOMATIC_OBJECT_STATE_CREATED, // gate just created
    AUTOMATIC_OBJECT_STATE_DESTROY // gate just destroyed
};

static 
    autoobject_Handle[MAX_AUTOMATIC_OBJECTS],
    autoobject_Area[MAX_AUTOMATIC_OBJECTS],
    autoobject_CurrentState[MAX_AUTOMATIC_OBJECTS],
    Iterator: autoobject_Iterator<MAX_AUTOMATIC_OBJECTS>, 
    Float: autoobject_StartPosition[MAX_AUTOMATIC_OBJECTS][3],
    Float: autoobject_StartRotation[MAX_AUTOMATIC_OBJECTS][3],
    Float: autoobject_FinalPosition[MAX_AUTOMATIC_OBJECTS][3],
    Float: autoobject_FinalRotation[MAX_AUTOMATIC_OBJECTS][3],
    Float: autoobject_MinimalPos[MAX_AUTOMATIC_OBJECTS][2],
    Float: autoobject_MaximalPos[MAX_AUTOMATIC_OBJECTS][2],
    Float: autoobject_move_speed[MAX_AUTOMATIC_OBJECTS];

// --
// / Forwards /
// --

forward OnAutomaticObjectStateChange(objectid, oldstate, newstate);

// --
// / Hooks /
// --


hook OnScriptInit()
{
    Iter_Init(autoobject_Iterator); 
    Iter_Add(autoobject_Iterator, 0);

    #if !defined AUTO_OBJECT_NO_CREDITS_MSG
        print("| Automatic Objects loaded |\n\
            | Version 1.0.0 by Mergevos |");
    #endif

    return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerEnterDynArea(playerid, STREAMER_TAG_AREA: areaid)
{
    if(autoobject_Handle[areaid] == areaid && autoobject_CurrentState[areaid] == AUTOMATIC_OBJECT_STATE_CLOSED) // if object matches areaid and is closed, open it
    {
        autoobject_CurrentState[areaid] = AUTOMATIC_OBJECT_STATE_OPENED;
        MoveDynamicObject(autoobject_Handle[areaid], autoobject_FinalPosition[areaid][0], autoobject_FinalPosition[areaid][1], autoobject_FinalPosition[areaid][2], autoobject_move_speed[areaid], autoobject_FinalRotation[areaid][0], autoobject_FinalRotation[areaid][1], autoobject_FinalRotation[areaid][2]);
        SetPVarInt(playerid, "AreaID", areaid); // player is in area
    }
    
    return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerLeaveDynArea(playerid, STREAMER_TAG_AREA: areaid)
{
    if(autoobject_Handle[areaid] == areaid && autoobject_CurrentState[areaid] == AUTOMATIC_OBJECT_STATE_OPENED) // if object matches areaid and is opened, close it
    {
        autoobject_CurrentState[areaid] = AUTOMATIC_OBJECT_STATE_CLOSED;
        MoveDynamicObject(autoobject_Handle[areaid], autoobject_StartPosition[areaid][0], autoobject_StartPosition[areaid][1], autoobject_StartPosition[areaid][2], autoobject_move_speed[areaid], autoobject_StartRotation[areaid][0], autoobject_StartRotation[areaid][1], autoobject_StartRotation[areaid][2]);
        SetPVarInt(playerid, "AreaID", 0); // player is in area
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}

// --
// / APi /
// --

// --
// <summary> Destroys automatic object </summary>
    // <param name="automatic_objectid"> Object id to destroy</param>
// --

stock AutoObject_Destroy(automatic_objectid)
{
    if(IsValidDynamicObject(autoobject_Handle[automatic_objectid]) && IsValidDynamicArea(autoobject_Area[automatic_objectid]))
    {
        if(Iter_Contains(autoobject_Iterator, automatic_objectid))
        {
            DestroyDynamicObject(autoobject_Handle[automatic_objectid]), DestroyDynamicArea(autoobject_Area[automatic_objectid]);
            Iter_Remove(autoobject_Iterator, automatic_objectid);
            CallLocalFunction("OnAutomaticObjectStateChange", "iii", automatic_objectid, autoobject_CurrentState[automatic_objectid], AUTOMATIC_OBJECT_STATE_DESTROY);
        }
    }
}

// --
// <summary> Destroys all automatic objects </summary>
// --

stock AutoObject_DestroyAll()
{
    foreach(new automatic_objectid : autoobject_Iterator) 
    {
        if(IsValidDynamicObject(autoobject_Handle[automatic_objectid]) && IsValidDynamicArea(autoobject_Area[automatic_objectid]))
        {
            DestroyDynamicObject(autoobject_Handle[automatic_objectid]), DestroyDynamicArea(autoobject_Area[automatic_objectid]);
            Iter_SafeRemove(autoobject_Iterator, automatic_objectid, automatic_objectid);
        }
    }
}

// --
// <summary> Creates automatic object.</summary>
    // <param name="modelid"> Modelid id to create</param>
    // <param name="Float: X"> Object's X Coordinate</param>
    // <param name="Float: Y"> Object's Y Coordinate</param>
    // <param name="Float: Z"> Object's Z Coordinate</param>
    // <param name="Float: minX"> Object's minX Coordinate</param>
    // <param name="Float: minY"> Object's minY Coordinate</param>
    // <param name="Float: maxX"> Object's maxZ Coordinate</param>
    // <param name="Float: maxY"> Object's maxZ Coordinate</param>
    // <param name="Float: rX"> Object's rotation X Coordinate</param>
    // <param name="Float: rY"> Object's rotation Y Coordinate</param>
    // <param name="Float: rZ"> Object's rotation Z Coordinate</param>
    // <param name="Float: torX"> Object's final rotation X Coordinate</param>
    // <param name="Float: torY"> Object's final rotation Y Coordinate</param>
    // <param name="Float: torZ"> Object's final rotation Z Coordinate</param>
    // <param name="Float: toX"> Object's final X Coordinate</param>
    // <param name="Float: toY"> Object's final Y Coordinate</param>
    // <param name="Float: toZ"> Object's final Z Coordinate</param>
    // <param name="Float: move_speed"> Object's moving speed</param>
        // <seealso name="MoveObject"/>
    // <param name="virtualworld"> Object's virtualworld to be created in</param>
    // <param name="interior"> Object's interiorid to be created in</param>
// --


stock AutoObject_Create(modelid, Float: X, Float: Y, Float: Z, Float: minX, Float: minY, Float: maxX, Float: maxY, Float: rX, Float: rY, Float: rZ, Float: torX, Float: torY, Float: torZ, Float: toX, Float: toY, Float: toZ, Float: move_speed, virtualworld, interior)
{
    new id = Iter_Alloc(autoobject_Iterator);
    autoobject_Handle[id] = CreateDynamicObject(modelid, X, Y, Z, rX, rY, rZ, virtualworld, interior);
    autoobject_Area[id] = CreateDynamicRectangle(minX, minY, maxX, maxY, virtualworld, interior);

    autoobject_StartPosition[id][0] = X,
    autoobject_StartPosition[id][1] = Y, 
    autoobject_StartPosition[id][2] = Z;

    autoobject_StartRotation[id][0] = rX, 
    autoobject_StartRotation[id][1] = rY,
    autoobject_StartRotation[id][2] = rZ;

    autoobject_FinalPosition[id][0] = toX,
    autoobject_FinalPosition[id][1] = toY,
    autoobject_FinalPosition[id][2] = toZ;

    autoobject_MinimalPos[id][0] = minX,
    autoobject_MinimalPos[id][1] = minY;

    autoobject_MaximalPos[id][0] = maxX,
    autoobject_MaximalPos[id][1] = maxY;

    autoobject_FinalRotation[id][0] = torX,
    autoobject_FinalRotation[id][1] = torY,
    autoobject_FinalRotation[id][2] = torZ;
    autoobject_move_speed[id] = move_speed;
    CallLocalFunction("OnAutomaticObjectStateChange", "iii", id, AUTOMATIC_OBJECT_STATE_UNDEF, AUTOMATIC_OBJECT_STATE_CREATED);
    autoobject_CurrentState[id] = AUTOMATIC_OBJECT_STATE_CLOSED;
    return id;
    
}

// --
// <summary> Checks if player's in any area </summary>
    // <param name="playerid"> Player's id to check</param>
// --

stock AutoObj_IsPlayerInAnyAutoArea(playerid)
{
    return GetPVarInt(playerid, "AreaID") > 0 ? true : false;
}

// --
// <summary> Checks if player's in particular area </summary>
    // <param name="playerid"> Player's id to check</param>
    // <param name="areaid"> Area's id to check</param>
// --

stock AutoObject_IsPlayerInAutoArea(playerid, areaid) 
{
    return GetPVarInt(playerid, "AreaID") == areaid ? true : false;
}