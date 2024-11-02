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
#define lives *(char*)0x800d80f7
#define zeroUnavailable *(char*)0x800d8124 // Whether Zero is available in the start menu

// upgrades
#define upgrade_vile_dead 0x30
#define upgrade_zero_dead 0x40
#define upgrade_saber 0x80

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
    lives = 2;
    zeroUnavailable = 0;
    if (practice.route == 1 || stageId == 0) // LOW%
    {
        return;
    }
    else if(practice.route == 0) //ANY% ALL STAGES
    {
        if (stageId == 8 || stageId == 5 && !practice.revist)
        {
            lives = 0;
        }
        switch (stageId)
        {
        case 5: //Volt CatFish
            if (practice.revist)
            {
                upgrades = upgrade_saber | upgrade_vile_dead | upgrade_zero_dead;
                zeroUnavailable = 1;
                bossFlags = 0x30;
            }
            armorParts = 8;
            break;
        case 0xA:
        case 0xB:
            SetWeapons(5);
            armorParts = 8;
            upgrades = upgrade_vile_dead; //Vile defeated in Volt Catfish level
            return;
        case 0xC:
        case 0xD:
            SetWeapons(-1);
            armorParts = 8;
            upgrades = upgrade_saber | upgrade_vile_dead | upgrade_zero_dead;
            zeroUnavailable = 1;
            return;
        
        default:
            if (stageId != 2)
            {
                armorParts = 8;
            }
            if (stageId != 2 && stageId != 8)
            {
                upgrades = upgrade_saber | upgrade_vile_dead | upgrade_zero_dead;
                zeroUnavailable = 1;
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