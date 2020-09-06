# samp-auto-objects

[![Mergevos](https://img.shields.io/badge/mergevos-samp--automatic--objects-2f2f2f.svg?style=for-the-badge)](https://github.com/Mergevos/samp-automatic-objects)

Automatic Objects is a library allowing you to use and create automatic movable objects.


## Installation

Simply install to your project:

```bash
sampctl package install Mergevos/samp-automatic-objects
```

Include in your code and begin using the library:

```pawn
#include < samp-auto-objects >
```

## Dependencies 

Dependencies are installed automatically. It's using YSI Library v5, together with streamer plugin.

## Usage

There're a few functions and a callback.

### Creating

Following function creates a movable object with its area. When a player enters area, objects start to move.

```pawn
AutoObject_Create(modelid, Float: X, Float: Y, Float: Z, Float: minX, Float: minY, Float: maxX, Float: maxY, Float: rX, Float: rY, Float: rZ, Float: torX, Float: torY, Float: torZ, Float: toX, Float: toY, Float: toZ, Float: move_speed, virtualworld, interior)
```
So, further we will look onto params: 
```
    modelid: Modelid id to create
    Float: X: Object's X Coordinate
    Float: Y: Object's Y Coordinate
    Float: Z: Object's Z Coordinate
    Float: minX: Object's minX Coordinate
    Float: minY: Object's minY Coordinate
    Float: maxX: Object's maxZ Coordinate
    Float: maxY: Object's maxZ Coordinate
    Float: rX: Object's rotation X Coordinate
    Float: rY: Object's rotation Y Coordinate
    Float: rZ: Object's rotation Z Coordinate
    Float: torX: Object's final rotation X Coordinate
    Float: torY: Object's final rotation Y Coordinate
    Float: torZ: Object's final rotation Z Coordinate
    Float: toX: Object's final X Coordinate
    Float: toY: Object's final Y Coordinate
    Float: toZ: Object's final Z Coordinate
    Float: move_speed: Object's moving speed
    virtualworld: Object's virtualworld to be created in
    interior: Object's interiorid to be created in
```
### Position checking

In destroying part we've theese 2 functions:
```pawn
AutoObj_IsPlayerInAnyAutoArea(playerid)
```
This function, of course, check if player's inside any area, and:
```pawn
AutoObject_IsPlayerInAutoArea(playerid, areaid) 
```
This function checks if player's inside a particular area.

### Alias 

The following code allows you to alias function names, thus by adding `#define AUTO_OBJECT_ALIAS_ON`
```pawn
#if defined AUTO_OBJECT_ALIAS_ON // used as alias
    #define AutoObject_Create       CreateAutomaticObject
    #define AutoObject_Destroy      DestroyAutomaticObject
    #define AutoObject_DestroyAll   DestroyAllAutomaticObject
    #define AutoObj_IsPlayerInAnyAutoArea    IsPlayerInAnyAutomaticArea
    #define AutoObject_IsPlayerInAutoArea    IsPlayerInAutomaticArea 
#endif
```
To adjust number of max possible automatic created objects use:
`#define MAX_AUTO_OBJECTS` by default is 32.

### Enums and callbacks

This is used to check changed state of an object, imagining it like a gate.

```pawn
forward OnAutomaticObjectStateChange(objectid, oldstate, newstate);
enum {
    AUTOMATIC_OBJECT_STATE_UNDEF = -1,
    AUTOMATIC_OBJECT_STATE_CLOSED = 0, // gate closed
    AUTOMATIC_OBJECT_STATE_OPENED, // gate opened (moved to final positions and rotations)
    AUTOMATIC_OBJECT_STATE_CREATED, // gate just created (moved to start positions and rotations)
    AUTOMATIC_OBJECT_STATE_DESTROY // gate just destroyed
};
```

## Testing

To test, simply config pawn.json and run the package:

```bash
sampctl package run
```
