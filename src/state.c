#include <common.h>
#include <gpu.h>
#include "practice.h"

#define VariableSetsCount 20
#define VRAM_DUMP_X 258
#define StateCoolDown 10

static void *readAddresses[] = {
    0x800d9658, // Ride Armor
    0x800d8a18, // MegaMan
    0x800d8ee8, // Weapon
    0x800d8ca8, // Weapon Effects
    0x800da148, // Misc
    0x800d92d8, // Shots
    0x800d9d48, // Items
    0x800d96b8, // Enemies
    0x800da8c8, // Effects
    0x800d8c18, // IDK (Object related)
    0x80134B5C, // Tile Data for Layers 1-2
    0x80108c5c, // Tile Data for Layer 3
    0x800d81c8, // Background & Sprite Clut
    0x800d7f08, // Thread Memory @ index 1
    0x800e8c98, // Decompressed Textures
    0x800f0c98, // Object Texture Locations
    0x800f0d98, // Object Clut Locations
    0x800e8218, // Enemy Memory
    0x800e8a48, // Config
    0x800dadd8 // Screen Data
};

static short addressesSize[] = {
    0x70,   // Ride Armor
    0x100,  // MegaMan
    0x3F0,  // Weapon
    0x240,  // Weapon Effects
    0x780,  // Misc
    0x380,  // Shots
    0x400,  // Items
    0x690,  // Enemies
    0x200,  // Effects
    0x90,   // IDK (Object related)
    0x2000, // Tile Data for Layers 1-2
    0x3000, // Tile Data for Layer 3
    0x300,  // Background & Sprite Clut
    0xC0,   // Thread Memory @ index 1
    0x800,  // Decompressed Textures
    220,    // Object Texture Locations
    220,    // Object Clut Locations
    0x5B0,  // Enemy Memory
    12,     // Config
    0x7E00 // Screen Data
};

static struct State
{
    int bossPointer;
    int lastTextureDumpOffset;

    short camX;
    short camY;
    short pastCamX;
    short pastCamY;
    short borderR;
    short borderL;
    short borderB;
    short borderT;
    short pastBorderR;
    short pastBorderL;
    short pastBorderB;
    short pastBorderT;
    short camX2;
    short camY2;
    short pastCamX2;
    short pastCamY2;
    short BG1_X;
    short BG1_Y;
    short BG2_X;
    short BG2_Y;
    short BG3_X;
    short BG3_Y;
    short textureDumpOffset;
    short textureLength;
    short BG2_baseX;
    short BG2_baseY;
    short tankAmmo;
    short tankAmmo2;

    u_char scrollType;
    u_char scrollType2;
    u_char drawFlags;
    u_char decompressFlag;
    u_char effectId;
    u_char textureId;
    u_char disableMega;
    u_char disableWeapons;
    u_char disableShots;
    u_char disableEnemies;
    u_char disableItems;
    u_char disableMisc;
    u_char disableEffect;
    u_char doorFlag;
    u_char BG3_flag;
    u_char checkPoint;
    u_char pauseEnable;
    u_char collisionFlag;
    u_char transFlag;
    u_char lives;
    u_char contact;
    u_char refights;
    u_char upgrades;
    u_char armorParts;
    u_char hearts;
    u_char maxHP;
    u_char flags;
    u_char controls;
    u_char brightness;
    u_char gameMode3;
    u_char gameMode4;
    u_char ready;
    u_char zero;
    u_char zero2;
    u_char ammoAct;
};

extern u_char weaponAmmo[9 * 2];
static u_char ammoBackup[9 * 2];

void StateSleep()
{
    DRAWENV *draw = 0x8013b7c4;
    draw->clip.w = 256;
    draw->clip.h = 240;
    draw->clip.x = 0;
    draw->isbg = 1;
    draw->clip.y = 0;
    PutDrawEnv(draw);
    DrawSync(0);
    draw->clip.y = 240;
    PutDrawEnv(draw);
    DrawSync(0);
    VSync(StateCoolDown);
}


void SaveState()
{
    /*
    Save OAM-1 Textures @ X:258, Y:264 & OAM-2 @ X:322, Y:264
    */
    RECT rect = {
        258, 264, 32, 128};
    MoveImage(&rect, 258 + 32, 264);
    DrawSync(0);
    rect.x = 322;

    MoveImage(&rect, 322 + 32, 264);
    DrawSync(0);

    /*
    FREE VRAM for CPU RAM starts @ X:VRAM_DUMP_X , Y:392.
    */
    rect.x = VRAM_DUMP_X;
    rect.y = 392;
    rect.h = 1;

    int freeWidth = 1024 - VRAM_DUMP_X;

    for (size_t i = 0; i < VariableSetsCount; i++)
    {
        int dumpSize = addressesSize[i];
        uint srcAddr = readAddresses[i];

    SizeCheck:
        if ((dumpSize / 2) > freeWidth)
        {

            if (rect.y >= 511)
            {
                printf("ERROR: went past MAX reserved VRAM space on Set: %d\n", i);
                return;
            }

            int width = 1024 - rect.x;
            rect.w = width;
            LoadImage(&rect, srcAddr);
            DrawSync(0);
            srcAddr += width * 2;
            dumpSize -= width * 2;

            rect.y++;
            rect.x = VRAM_DUMP_X;
            freeWidth = 1024 - VRAM_DUMP_X;
            goto SizeCheck;
        }
        else
        {
            int width = dumpSize / 2;
            rect.w = width;
            LoadImage(&rect, srcAddr);
            DrawSync(0);
            rect.x += width;
            freeWidth -= width;
        }
    }
    // backup smaller variables
    struct State *state = 0x1f800000;

    state->bossPointer = *(int *)0x800d808c;
    state->lastTextureDumpOffset = *(int *)0x800d803c;

    state->camX = *(short *)0x800dabae;
    state->camY = *(short *)0x800dabb2;
    state->pastCamX = *(short *)0x800dabd0;
    state->pastCamY = *(short *)0x800dabd2;
    state->borderR = *(short *)0x800dabc4;
    state->borderL = *(short *)0x800dabc2;
    state->borderB = *(short *)0x800dabd4;
    state->borderT = *(short *)0x800dabcc;
    state->pastBorderR = *(short *)0x800dabba;
    state->pastBorderL = *(short *)0x800dabb8;
    state->pastBorderB = *(short *)0x800dabbe;
    state->pastBorderT = *(short *)0x800dabbc;
    state->camX2 = *(short *)0x800dac0e;
    state->camY2 = *(short *)0x800dac12;
    state->pastCamX2 = *(short *)0x800dac30;
    state->pastCamY2 = *(short *)0x800dac32;
    state->BG1_X = *(short *)0x800d7fe8;
    state->BG1_Y = *(short *)0x800d7fea;
    state->BG2_X = *(short *)0x800d7fec;
    state->BG2_Y = *(short *)0x800d7fee;
    state->BG3_X = *(short *)0x800d7ff0;
    state->BG3_Y = *(short *)0x800d7ff2;
    state->textureDumpOffset = *(short *)0x800d7ede;
    state->textureLength = *(short *)0x800d8040;
    state->BG2_baseX = *(short *)0x800dac14;
    state->BG2_baseY = *(short *)0x800dac16;
    state->tankAmmo = *(short *)0x800d80fa;
    state->tankAmmo2 = *(short *)0x800d80fc;

    state->scrollType = *(u_char *)0x800daba9;
    state->scrollType2 = *(u_char *)0x800dac09;
    state->drawFlags = *(u_char *)0x800d7ff5;
    state->decompressFlag = *(u_char *)0x800d8044;
    state->effectId = *(u_char *)0x800d8082;
    state->textureId = *(u_char *)0x800d8045;
    state->disableMega = *(u_char *)0x800d8093;
    state->disableWeapons = *(u_char *)0x800d8094;
    state->disableShots = *(u_char *)0x800d8095;
    state->disableEnemies = *(u_char *)0x800d8096;
    state->disableItems = *(u_char *)0x800d8097;
    state->disableMisc = *(u_char *)0x800d8098;
    state->disableEffect = *(u_char *)0x800d8099;
    state->doorFlag = *(u_char *)0x800d80d1;
    state->BG3_flag = *(u_char *)0x800d80ad;
    state->checkPoint = *(u_char *)0x800d80f8;
    state->pauseEnable = *(u_char *)0x800d80c1;
    state->collisionFlag = *(u_char *)0x800d809a;
    state->transFlag = *(u_char *)0x800fe7cb;
    state->lives = *(u_char *)0x800d80f7;
    state->contact = *(u_char *)0x800d8087;
    state->refights = *(u_char *)0x800d811d;
    state->upgrades = *(u_char *)0x800d80f5;
    state->armorParts = *(u_char *)0x800d8114;
    state->hearts = *(u_char *)0x800d8117;
    state->maxHP = *(u_char *)0x800d8115;
    state->flags = *(u_char *)0x800d8116;
    state->controls = *((u_char *)0x800d80d6);
    state->brightness = *((u_char *)0x800d7ff4);
    state->gameMode3 = gameMode3;
    state->gameMode4 = gameMode4;
    state->ready = *((u_char *)0x800da898);
    state->zero = *((u_char*)0x800d80cf);
    state->zero2 = *((u_char*)0x800d8124);
    state->ammoAct = *((u_char*)0x800d8092);

    //Weapon Ammo
    for (size_t i = 0; i < 18; i++)
    {
        ammoBackup[i] = weaponAmmo[i];
    }

    // done
    practice.stateMade = true;
    practice.stateStageId = stageId;

    StateSleep();
}

void LoadState()
{
    RECT rect = {
        258 + 32, 264, 32, 128};
    MoveImage(&rect, 258, 264);
    DrawSync(0);
    rect.x = 322 + 32;

    MoveImage(&rect, 322, 264);
    DrawSync(0);

    rect.x = VRAM_DUMP_X;
    rect.y = 392;
    rect.h = 1;

    int freeWidth = 1024 - VRAM_DUMP_X;

    for (size_t i = 0; i < VariableSetsCount; i++)
    {
        int dumpSize = addressesSize[i];
        uint srcAddr = readAddresses[i];

    SizeCheck:
        if ((dumpSize / 2) > freeWidth)
        {
            int width = 1024 - rect.x;
            rect.w = width;
            StoreImage(&rect, srcAddr);
            DrawSync(0);
            srcAddr += width * 2;
            dumpSize -= width * 2;

            rect.y++;
            rect.x = VRAM_DUMP_X;
            freeWidth = 1024 - VRAM_DUMP_X;
            goto SizeCheck;
        }
        else
        {
            int width = dumpSize / 2;
            rect.w = width;
            StoreImage(&rect, srcAddr);
            DrawSync(0);
            rect.x += width;
            freeWidth -= width;
        }
    }
    *(u_char *)0x800d7fcc = 1;

    // restoring smaller variables
    struct State *state = 0x1f800000;

    *(int *)0x800d808c = state->bossPointer;
    *(int *)0x800d803c = state->lastTextureDumpOffset;

    *(short *)0x800dabae = state->camX;
    *(short *)0x800dabb2 = state->camY;
    *(short *)0x800dabd0 = state->pastCamX;
    *(short *)0x800dabd2 = state->pastCamY;
    *(short *)0x800dabc4 = state->borderR;
    *(short *)0x800dabc2 = state->borderL;
    *(short *)0x800dabd4 = state->borderB;
    *(short *)0x800dabcc = state->borderT;
    *(short *)0x800dabba = state->pastBorderR;
    *(short *)0x800dabb8 = state->pastBorderL;
    *(short *)0x800dabbe = state->pastBorderB;
    *(short *)0x800dabbc = state->pastBorderT;
    *(short *)0x800dac0e = state->camX2;
    *(short *)0x800dac12 = state->camY2;
    *(short *)0x800dac30 = state->pastCamX2;
    *(short *)0x800dac32 = state->pastCamY2;
    *(short *)0x800d7fe8 = state->BG1_X;
    *(short *)0x800d7fea = state->BG1_Y;
    *(short *)0x800d7fec = state->BG2_X;
    *(short *)0x800d7fee = state->BG2_Y;
    *(short *)0x800d7ff0 = state->BG3_X;
    *(short *)0x800d7ff2 = state->BG3_Y;
    *(short *)0x800d7ede = state->textureDumpOffset;
    *(short *)0x800d8040 = state->textureLength;
    *(short *)0x800dac14 = state->BG2_baseX;
    *(short *)0x800dac16 = state->BG2_baseY;
    *(short *)0x800d80fa = state->tankAmmo;
    *(short *)0x800d80fc = state->tankAmmo2;
 
    *(u_char *)0x800daba9 = state->scrollType;
    *(u_char *)0x800dac09 = state->scrollType2;
    *(u_char *)0x800d7ff5 = state->drawFlags;
    *(u_char *)0x800d8044 = state->decompressFlag;
    *(u_char *)0x800d8082 = state->effectId;
    *(u_char *)0x800d8045 = state->textureId;
    *(u_char *)0x800d8093 = state->disableMega;
    *(u_char *)0x800d8094 = state->disableWeapons;
    *(u_char *)0x800d8095 = state->disableShots;
    *(u_char *)0x800d8096 = state->disableEnemies;
    *(u_char *)0x800d8097 = state->disableItems;
    *(u_char *)0x800d8098 = state->disableMisc;
    *(u_char *)0x800d8099 = state->disableEffect;
    *(u_char *)0x800d80d1 = state->doorFlag;
    *(u_char *)0x800d80ad = state->BG3_flag;
    *(u_char *)0x800d80f8 = state->checkPoint;
    *(u_char *)0x800d80c1 = state->pauseEnable;
    *(u_char *)0x800d809a = state->collisionFlag;
    *(u_char *)0x800fe7cb = state->transFlag;
    *(u_char *)0x800d80f7 = state->lives;
    *(u_char *)0x800d8087 = state->contact;
    *(u_char *)0x800d811d = state->refights;
    *(u_char *)0x800d80f5 = state->upgrades;
    *(u_char *)0x800d8114 = state->armorParts;
    *(u_char *)0x800d8117 = state->hearts;
    *(u_char *)0x800d8115 = state->maxHP;
    *(u_char *)0x800d8116 = state->flags;
    *(u_char *)0x800d80d6 = state->controls;
    *(u_char *)0x800d7ff4 = state->brightness;
    gameMode3 = state->gameMode3;
    gameMode4 = state->gameMode4;
    *(u_char *)0x800da898 = state->ready;
    *(u_char *)0x800d80cf = state->zero;
    *(u_char *)0x800d8124 = state->zero2;
    *(u_char *)0x800d8092 = state->ammoAct;

    //Weapon Ammo
    for (size_t i = 0; i < 18; i++)
    {
        weaponAmmo[i] = ammoBackup[i];
    }

    StateSleep();
}

void GameMode1_2_1(Thread * thread); //'READY'
void GameMode1_2_2(Thread * thread); //'Normal'
void GameMode1_2_3(Thread * thread); //'Dead'

static void (*gameModeTable[4])(Thread * t) = {
    0,
    GameMode1_2_1,
    GameMode1_2_2,
    GameMode1_2_3
};

void StateCheck(Thread *thread)
{
    if (*(char*)0x800d80a9 != 0)
    {
        return;
    }
    
    if (gameMode3 == 2)
    {
        GetRng();
    }
    

    if ((buttonsPressed & (PAD_L2 | PAD_R2 | PAD_SELECT)) != 0)
    {
        if ((buttonsHeld & (PAD_R2 + PAD_SELECT)) == (PAD_R2 + PAD_SELECT))
        {
            SaveState(thread);
        }
        else if ((buttonsHeld & (PAD_L2 + PAD_SELECT)) == (PAD_L2 + PAD_SELECT) && practice.stateMade)
        {
            LoadState(thread);
        }
    }
}

void StateHook(Thread *thread)
{
    if (gameMode3 == 1 && gameMode4 == 0)
    {
        goto SkipCheck;
    }
    StateCheck(thread);

SkipCheck:
    gameModeTable[gameMode3](thread);
}