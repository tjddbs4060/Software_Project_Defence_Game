#ifndef __ORDER_H__
#define __ORDER_H__

#include "SimpleAudioEngine.h"

//GameScene
#define MAX_MONSTER 50

#define ZORDER_BACKGROUND 0
#define ZORDER_UNIT 1
#define ZORDER_HERO 1
#define ZORDER_INFORBOARD 2
#define ZORDER_INTERFACE 3
#define ZORDER_MENU 4
#define ZORDER_LABEL 5

#define TAG_BACKGROUND 1000
#define TAG_UNIT 1001
#define TAG_HERO 1002
#define TAG_INFORBOARD 1003
#define TAG_INTERFACE 1004
#define TAG_INTERFACE_SOUL 1005
#define TAG_INTERFACE_GAMBLE 1006
#define TAG_INTERFACE_UPGRADE 1007
#define TAG_INTERFACE_MIX 1008
#define TAG_INTERFACE_CAPSULE 1009
#define TAG_INTERFACE_HERO 1010
#define TAG_INTERFACE_BOSS 1011
#define TAG_INTERFACE_FRIEND 1012
#define TAG_INTERFACE_FRIEND_ESC 1013
#define TAG_INTERFACE_FRIEND_NAME_1 1014
#define TAG_INTERFACE_FRIEND_NAME_2 1015
#define TAG_INTERFACE_FRIEND_NAME_3 1016
#define TAG_INTERFACE_FRIEND_MONSTER_1 1017
#define TAG_INTERFACE_FRIEND_MONSTER_2 1018
#define TAG_INTERFACE_FRIEND_MONSTER_3 1019
#define TAG_INTERFACE_FRIEND_HELP_1 1020
#define TAG_INTERFACE_FRIEND_HELP_2 1021
#define TAG_INTERFACE_FRIEND_HELP_3 1022
#define TAG_LABEL 1023

#define ZORDER_RANGE 1
#define ZORDER_MONSTER 2
#define ZORDER_CHARACTER 3
#define ZORDER_MONSTER_DEATH 4
#define ZORDER_BULLET 10

#define TAG_RANGE 2001
#define TAG_MONSTER 2002
#define TAG_CHARACTER 2003

#define TAG_MENU 3000
#define TAG_MENU_SOUL 3001
#define TAG_MENU_SKIP 3002
#define TAG_MENU_SKIP_BACK 3003
#define TAG_MENU_GAMBLE 3004
#define TAG_MENU_UPGRADE 3005
#define TAG_MENU_MIX 3006
#define TAG_MENU_MIX_SCROLL 3007
#define TAG_MENU_CAPSULE 3008
#define TAG_MENU_HERO 3009
#define TAG_MENU_BOSS 3010
#define TAG_MENU_FRIEND 3011

#define TAG_SOUL_NUMBER 900


//Title Scene
#define TAG_TITLE_MENU 4000
#define TAG_TITLE_MENU_SINGLE 4001
#define TAG_TITLE_MENU_MULTI 4002
#define TAG_TITLE_MENU_EXIT 4003

//Login & Create Scene
#define TAG_ID 5000
#define TAG_PW 5001
#define TAG_LOGIN 5002
#define TAG_MEMBER 5003

#endif