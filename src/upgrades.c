#include <common.h>
#include "practice.h"

extern uint8_t weaponAmmo[9 * 2];

static uint8_t aquiredWeapons[][8] = {
    /*Acid | P.Bomb | T.Thunder | S.Blade | Ray.S | B.Hole | Frost.S | Tornado.F*/

    // Intro
    {0, 0, 0, 0, 0, 0, 0, 0},
    // Blast Hornet
    {0x5C, 0, 0x5C, 0x5C, 0x5C, 0x5C, 0x5C, 0x5C},
    // Blizzard Buffalo
    {0, 0, 0, 0, 0, 0, 0, 0},
    // Gravity Beetle
    {0, 0, 0x5C, 0x5C, 0x5C, 0, 0x5C, 0x5C},
    // Toxic Seahorse
    {0, 0, 0x5C, 0x5C, 0x5C, 0x5C, 0x5C, 0x5C},
    // Volt Catfish
    {0, 0, 0, 0, 0x5C, 0, 0x5C, 0},
    // Crush Clawfish
    {0, 0, 0x5C, 0, 0x5C, 0, 0x5C, 0},
    // Tunnel Rhino
    {0, 0, 0x5C, 0x5C, 0x5C, 0, 0x5C, 0},
    // Neon Tiger
    {0, 0, 0, 0, 0, 0, 0x5C, 0},

    /*100%*/
    // Blast Hornet
    {0x5C, 0, 0, 0, 0x5C, 0x5C, 0x5C, 0x5C},
    // Blizzard Buffalo
    {0, 0, 0, 0, 0, 0, 0, 0x5C},
    // Gravity Beetle
    {0, 0, 0, 0, 0x5C, 0, 0x5C, 0x5C},
    // Toxic Seahorse
    {0, 0, 0, 0, 0x5C, 0x5C, 0x5C, 0x5C},
    // Volt Catfish
    {0x5C, 0x5C, 0, 0, 0x5C, 0x5C, 0x5C, 0x5C},
    // Crush Clawfish
    {0x5C, 0x5C, 0x5C, 0, 0x5C, 0x5C, 0x5C, 0x5C},
    // Tunnel Rhino
    {0, 0, 0, 0, 0, 0, 0, 0},
    // Neon Tiger
    {0, 0, 0, 0, 0, 0, 0x5C, 0x5C}
};

void ApplyHearts();

static void SetWeapons(int set)
{
    for (size_t i = 0; i < 8; i++)
    {
        if (set == -1)
        {
            weaponAmmo[i * 2] = 0x5C;
        }
        else
        {
            weaponAmmo[i * 2] = aquiredWeapons[set][i];
        }
    }
}
#define tankAmmo ((char *)0x800d80fa)
#define upgrades *(char *)0x800d80f5
#define maxHP *(char *)0x800d8115
#define rideArmors *(char *)0x800d811a
#define hearts *(char *)0x800d8117
#define armorParts *(char *)0x800d8114
#define bossFlags *(char *)0x800d811b
#define lives *(char *)0x800d80f7
#define zeroUnavailable *(char *)0x800d8124 // Whether Zero is available in the start menu

// armor
#define amror_helmet 1
#define armor_arm 2
#define armor_body 4
#define armor_boots 8

// upgrades
#define upgrade_vile_dead 0x30
#define upgrade_zero_dead 0x40
#define upgrade_saber 0x80

// stages
#define stage_intro 0
#define stage_hornet 1
#define stage_buffalo 2
#define stage_beetle 3
#define stage_seahorse 4
#define stage_catfish 5
#define stage_crawfish 6
#define stage_rhino 7
#define stage_tiger 8
#define stage_doppler_1 0xA
#define stage_doppler_2 0xB
#define stage_doppler_3 0xC
#define stage_doppler_4 0xD

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
    tankAmmo[0] = 0;
    tankAmmo[1] = 0;
    tankAmmo[2] = 0;
    tankAmmo[3] = 0;
    if (practice.route == LOW_PERCENT || stageId == stage_intro)
    {
        return;
    }
    else if (practice.route == ALL_STAGES)
    {
        if (stageId == stage_catfish && !practice.revist)
        {
            lives = 0;
        }
        switch (stageId)
        {
        case stage_catfish:
            if (practice.revist)
            {
                upgrades = upgrade_saber | upgrade_vile_dead | upgrade_zero_dead;
                zeroUnavailable = 1;
                bossFlags = 0x30;
            }
            armorParts = armor_boots;
            break;
        case stage_doppler_1:
        case stage_doppler_2:
            SetWeapons(stage_catfish);
            armorParts = armor_boots;
            upgrades = upgrade_vile_dead; // Vile defeated in Volt Catfish level
            return;
        case stage_doppler_3:
        case stage_doppler_4:
            SetWeapons(-1);
            armorParts = armor_boots;
            upgrades = upgrade_saber | upgrade_vile_dead | upgrade_zero_dead;
            zeroUnavailable = 1;
            return;

        default:
            if (stageId != stage_buffalo)
            {
                armorParts = armor_boots;
            }
            if (stageId != stage_buffalo && stageId != stage_tiger)
            {
                upgrades = upgrade_saber | upgrade_vile_dead | upgrade_zero_dead;
                zeroUnavailable = 1;
                bossFlags = 0x30;
            }
            break;
        }
        if (stageId < stage_doppler_1)
        {
            SetWeapons(stageId);
        }
        else
        {
            SetWeapons(-1);
        }
    }
    else if (practice.route == ANY_PERCENT)
    {
        if (stageId != stage_buffalo)
        {
            armorParts = armor_boots;
        }
        switch (stageId)
        {
        case stage_crawfish:
            lives = 0;
            weaponAmmo[6 * 2] = 0x5C;
            break;
        case stage_catfish:
            lives = 0;
            weaponAmmo[3 * 2] = 0x5C;
            weaponAmmo[6 * 2] = 0x5C;
            break;
        case stage_doppler_1:
        case stage_doppler_2:
            upgrades = upgrade_vile_dead; // Vile defeated in Volt Catfish level
            weaponAmmo[3 * 2] = 0x5C;
            weaponAmmo[6 * 2] = 0x5C;
            break;
        case stage_doppler_3:
        case stage_doppler_4:
            weaponAmmo[3 * 2] = 0x5C;
            weaponAmmo[6 * 2] = 0x5C;
            upgrades = upgrade_saber | upgrade_vile_dead | upgrade_zero_dead;
            zeroUnavailable = 1;
            bossFlags = 0x30;
            break;
        default:
            break;
        }
    }
    else //100%
    {
        if (stageId < stage_doppler_1)
        {
            if (practice.revist)
            {
                rideArmors = 0xF;
                tankAmmo[0] = 0x80;
                tankAmmo[1] = 0x80;
                tankAmmo[2] = 0x80;
                upgrades = upgrade_vile_dead;
                if (stageId == stage_rhino)
                {
                    hearts = 0xBF;
                    armorParts = armor_arm + armor_body + armor_boots + 0xB0;
                }
                else
                {
                    hearts = 0xBB;
                    armorParts = armor_arm + armor_body + armor_boots + 0xB0;
                }
                SetWeapons(-1);
            }
            else
            {
                switch (stageId)
                {
                case stage_tiger:
                    hearts = 2;
                    armorParts = armor_boots + 0x10;
                    tankAmmo[0] = 0x80;
                    break;
                case stage_beetle:
                    hearts = 0x82;
                    armorParts = armor_boots + armor_arm  + 0x10 + 0x80;
                    tankAmmo[0] = 0x80;
                    tankAmmo[1] = 0x80;
                    break;
                case stage_seahorse:
                    hearts = 0x82;
                    armorParts = armor_boots + armor_arm  + 0x10 + 0x80;
                    tankAmmo[0] = 0x80;
                    tankAmmo[1] = 0x80;
                    rideArmors = 8;
                    break;
                case stage_hornet:
                    hearts = 0x8A;
                    armorParts = armor_boots + armor_arm  + 0x10 + 0x80;
                    tankAmmo[0] = 0x80;
                    tankAmmo[1] = 0x80;
                    rideArmors = 8 +  2;
                    break;
                case stage_catfish:
                    hearts = 0x8B;
                    armorParts = armor_boots + armor_arm  + 0x10 + 0x80;
                    tankAmmo[0] = 0x80;
                    tankAmmo[1] = 0x80;
                    rideArmors = 8 +  2 + 1;
                    break;
                case stage_crawfish:
                    hearts = 0x9B;
                    armorParts = armor_boots + armor_arm + armor_body  + 0x10 + 0x80 + 0x20;
                    tankAmmo[0] = 0x80;
                    tankAmmo[1] = 0x80;
                    tankAmmo[2] = 0x80;
                    rideArmors = 8 +  2 + 1;
                    break;
                default:
                    break;
                }
                SetWeapons(stageId + 8);
                ApplyHearts();
            }
        }
        else
        {
            hearts = 0xFF;
            rideArmors = 0xF;
            armorParts = 0xFF;
            tankAmmo[0] = 0x80;
            tankAmmo[1] = 0x80;
            tankAmmo[2] = 0x80;
            tankAmmo[3] = 0x80;
            bossFlags = 0x30;
            switch (stageId)
            {
            case stage_doppler_1:
                upgrades = upgrade_vile_dead; // Vile defeated in Volt Catfish level
                break;
            case stage_doppler_2:
                weaponAmmo[8 * 2] = 0x5C;
                upgrades = upgrade_vile_dead;
                rideArmors = 0xFF;
                break;
            case stage_doppler_3:
            case stage_doppler_4:
                weaponAmmo[8 * 2] = 0x5C;
                upgrades = upgrade_saber | upgrade_vile_dead | upgrade_zero_dead;
                rideArmors = 0xFF;
                zeroUnavailable = 1;
                break;
            default:
                break;
            }
            ApplyHearts();
            SetWeapons(-1);
        }
    }
}

void ResetSaveState()
{
    if (practice.stateStageId != stageId || stageId == stage_doppler_3)
    {
        practice.stateMade = 0;
    }

    *((char *)0x800d7fcf) = 0;
    *((char *)0x800d7ff4) = 0x1F; // brightness
}

#undef tankAmmo
#undef upgrades
#undef maxHP
#undef rideArmors
#undef hearts
#undef armorParts
#undef bossFlags
#undef lives
#undef zeroUnavailable
#undef amror_helmet
#undef armor_arm
#undef armor_body
#undef armor_boots
#undef upgrade_vile_dead
#undef upgrade_zero_dead
#undef upgrade_saber
#undef stage_intro
#undef stage_hornet
#undef stage_buffalo
#undef stage_beetle
#undef stage_seahorse
#undef stage_catfish
#undef stage_crawfish
#undef stage_rhino
#undef stage_tiger
#undef stage_doppler_1
#undef stage_doppler_2
#undef stage_doppler_3
#undef stage_doppler_4
