#include <common.h>
#include "practice.h"

extern uint8_t weaponAmmo[9 * 2];

static uint8_t aquiredWeapons[][8] = {
    /*Acid | P.Bomb | T.Thunder | S.Blade | Ray.S | B.Hole | Frost.S | Tornado.F*/

    //Intro
    {0,0,0,0,0,0,0,0},
    //Blast Hornet
    {0x5C,0,0x5C,0x5C,0x5C,0x5C,0x5C,0x5C},
    //Blizzard Buffalo
    {0,0,0,0,0,0,0,0},
    //Gravity Beetle
    {0,0,0x5C,0x5C,0x5C,0,0x5C,0x5C},
    //Toxic Seahorse
    {0,0,0x5C,0x5C,0x5C,0x5C,0x5C,0x5C},
    //Volt Catfish
    {0,0,0,0,0x5C,0,0x5C,0},
    //Crush Clawfish
    {0,0,0x5C,0,0x5C,0,0x5C,0},
    //Tunnel Rhino
    {0,0,0x5C,0x5C,0x5C,0,0x5C,0},
    //Neon Tiger
    {0,0,0,0,0,0,0x5C,0}
};

static void SetWeapons(int set)
{
    for (size_t i = 0; i < 8; i++)
    {
        if (set == -1)
        {
            weaponAmmo[i * 2] = 0x5C;
        }else{
            weaponAmmo[i * 2] = aquiredWeapons[set][i];
        }
    }
}

#define upgrades *(char*)0x800d80f5
#define maxHP *(char*)0x800d8115
#define rideArmors *(char*)0x800d811a
#define hearts *(char*)0x800d8117
#define armorParts *(char*)0x800d8114
#define bossFlags *(char*)0x800d811b

void SetupUpgrades()
{
    SetWeapons(0);
    practice.stateMade = 0;
    maxHP = 0x10;
    hearts = 0;
    upgrades = 0;
    rideArmors = 0;
    armorParts = 0;
    bossFlags = 0;
    *(char*)0x800d80f7 = 2; //lives
    if (practice.route == 1 || stageId == 0) // LOW%
    {
        if (stageId == 0xB)
        {
            stageId = 0xB;
        }
        return;
    }
    else if(practice.route == 0) //ANY% ALL STAGES
    {
        if (stageId == 8)
        {
            *(char*)0x800d80f7 = 0; //lives
        }else if (stageId == 5 && !practice.revist)
        {
            *(char*)0x800d80f7 = 0; //lives
        }
           
        switch (stageId)
        {
        case 5: //Volt CatFish
            if (practice.revist)
            {
                upgrades = 0x80 | 0x30;
                bossFlags = 0x30;
            }
            armorParts = 8;
            break;
        case 0xA:
        case 0xB:
            SetWeapons(5);
            armorParts = 8;
            upgrades = 0x30; //Vile defeted in Volt Catfish level
            return;
        case 0xC:
        case 0xD:
            SetWeapons(-1);
            armorParts = 8;
            upgrades = 0x80 | 0x30;
            return;
        
        default:
            if (stageId != 2)
            {
                armorParts = 8;
            }
            if (stageId != 2 && stageId != 8)
            {
                upgrades = 0x80 | 0x30;
                bossFlags = 0x30;
            }
            
            break;
        }
        if (stageId < 0xA)
        {
            SetWeapons(stageId);
        }else{
            SetWeapons(-1);
        }
        
    }
}

void ResetSaveState()
{
    if (practice.stateStageId != stageId)
    {
        practice.stateMade = 0;
    }

    *((char *)0x800d7fcf) = 0;
    *((char *)0x800d7ff4) = 0x1F; // brightness
}

#undef upgrades
#undef maxHP
#undef rideArmors
#undef hearts
#undef armorParts
#undef bossFlags