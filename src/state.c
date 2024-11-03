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

static int16_t addressesSize[] = {
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

    int16_t camX;
    int16_t camY;
    int16_t pastCamX;
    int16_t pastCamY;
    int16_t borderR;
    int16_t borderL;
    int16_t borderB;
    int16_t borderT;
    int16_t pastBorderR;
    int16_t pastBorderL;
    int16_t pastBorderB;
    int16_t pastBorderT;
    int16_t camX2;
    int16_t camY2;
    int16_t pastCamX2;
    int16_t pastCamY2;
    int16_t BG1_X;
    int16_t BG1_Y;
    int16_t BG2_X;
    int16_t BG2_Y;
    int16_t BG3_X;
    int16_t BG3_Y;
    int16_t textureDumpOffset;
    int16_t textureLength;
    int16_t BG2_baseX;
    int16_t BG2_baseY;
    int16_t tankAmmo;
    int16_t tankAmmo2;

    uint8_t scrollType;
    uint8_t scrollType2;
    uint8_t drawFlags;
    uint8_t decompressFlag;
    uint8_t effectId;
    uint8_t textureId;
    uint8_t disableMega;
    uint8_t disableWeapons;
    uint8_t disableShots;
    uint8_t disableEnemies;
    uint8_t disableItems;
    uint8_t disableMisc;
    uint8_t disableEffect;
    uint8_t doorFlag;
    uint8_t BG3_flag;
    uint8_t checkPoint;
    uint8_t pauseEnable;
    uint8_t collisionFlag;
    uint8_t transFlag;
    uint8_t lives;
    uint8_t contact;
    uint8_t refights;
    uint8_t upgrades;
    uint8_t armorParts;
    uint8_t hearts;
    uint8_t maxHP;
    uint8_t flags;
    uint8_t controls;
    uint8_t brightness;
    uint8_t gameMode3;
    uint8_t gameMode4;
    uint8_t ready;
    uint8_t zero;
    uint8_t zero2;
    uint8_t ammoAct;
    uint8_t cutscene;
};

extern uint8_t weaponAmmo[9 * 2];
static uint8_t ammoBackup[9 * 2];

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

    state->camX = *(int16_t *)0x800dabae;
    state->camY = *(int16_t *)0x800dabb2;
    state->pastCamX = *(int16_t *)0x800dabd0;
    state->pastCamY = *(int16_t *)0x800dabd2;
    state->borderR = *(int16_t *)0x800dabc4;
    state->borderL = *(int16_t *)0x800dabc2;
    state->borderB = *(int16_t *)0x800dabd4;
    state->borderT = *(int16_t *)0x800dabcc;
    state->pastBorderR = *(int16_t *)0x800dabba;
    state->pastBorderL = *(int16_t *)0x800dabb8;
    state->pastBorderB = *(int16_t *)0x800dabbe;
    state->pastBorderT = *(int16_t *)0x800dabbc;
    state->camX2 = *(int16_t *)0x800dac0e;
    state->camY2 = *(int16_t *)0x800dac12;
    state->pastCamX2 = *(int16_t *)0x800dac30;
    state->pastCamY2 = *(int16_t *)0x800dac32;
    state->BG1_X = *(int16_t *)0x800d7fe8;
    state->BG1_Y = *(int16_t *)0x800d7fea;
    state->BG2_X = *(int16_t *)0x800d7fec;
    state->BG2_Y = *(int16_t *)0x800d7fee;
    state->BG3_X = *(int16_t *)0x800d7ff0;
    state->BG3_Y = *(int16_t *)0x800d7ff2;
    state->textureDumpOffset = *(int16_t *)0x800d7ede;
    state->textureLength = *(int16_t *)0x800d8040;
    state->BG2_baseX = *(int16_t *)0x800dac14;
    state->BG2_baseY = *(int16_t *)0x800dac16;
    state->tankAmmo = *(int16_t *)0x800d80fa;
    state->tankAmmo2 = *(int16_t *)0x800d80fc;

    state->scrollType = *(uint8_t *)0x800daba9;
    state->scrollType2 = *(uint8_t *)0x800dac09;
    state->drawFlags = *(uint8_t *)0x800d7ff5;
    state->decompressFlag = *(uint8_t *)0x800d8044;
    state->effectId = *(uint8_t *)0x800d8082;
    state->textureId = *(uint8_t *)0x800d8045;
    state->disableMega = *(uint8_t *)0x800d8093;
    state->disableWeapons = *(uint8_t *)0x800d8094;
    state->disableShots = *(uint8_t *)0x800d8095;
    state->disableEnemies = *(uint8_t *)0x800d8096;
    state->disableItems = *(uint8_t *)0x800d8097;
    state->disableMisc = *(uint8_t *)0x800d8098;
    state->disableEffect = *(uint8_t *)0x800d8099;
    state->doorFlag = *(uint8_t *)0x800d80d1;
    state->BG3_flag = *(uint8_t *)0x800d80ad;
    state->checkPoint = *(uint8_t *)0x800d80f8;
    state->pauseEnable = *(uint8_t *)0x800d80c1;
    state->collisionFlag = *(uint8_t *)0x800d809a;
    state->transFlag = *(uint8_t *)0x800fe7cb;
    state->lives = *(uint8_t *)0x800d80f7;
    state->contact = *(uint8_t *)0x800d8087;
    state->refights = *(uint8_t *)0x800d811d;
    state->upgrades = *(uint8_t *)0x800d80f5;
    state->armorParts = *(uint8_t *)0x800d8114;
    state->hearts = *(uint8_t *)0x800d8117;
    state->maxHP = *(uint8_t *)0x800d8115;
    state->flags = *(uint8_t *)0x800d8116;
    state->controls = *((uint8_t *)0x800d80d6);
    state->brightness = *((uint8_t *)0x800d7ff4);
    state->gameMode3 = gameMode3;
    state->gameMode4 = gameMode4;
    state->ready = *((uint8_t *)0x800da898);
    state->zero = *((uint8_t*)0x800d80cf);
    state->zero2 = *((uint8_t*)0x800d8124);
    state->ammoAct = *((uint8_t*)0x800d8092);
    state->cutscene = *((uint8_t*)0x800d80b7);

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
    *(uint8_t *)0x800d7fcc = 1;

    // restoring smaller variables
    struct State *state = 0x1f800000;

    *(int *)0x800d808c = state->bossPointer;
    *(int *)0x800d803c = state->lastTextureDumpOffset;

    *(int16_t *)0x800dabae = state->camX;
    *(int16_t *)0x800dabb2 = state->camY;
    *(int16_t *)0x800dabd0 = state->pastCamX;
    *(int16_t *)0x800dabd2 = state->pastCamY;
    *(int16_t *)0x800dabc4 = state->borderR;
    *(int16_t *)0x800dabc2 = state->borderL;
    *(int16_t *)0x800dabd4 = state->borderB;
    *(int16_t *)0x800dabcc = state->borderT;
    *(int16_t *)0x800dabba = state->pastBorderR;
    *(int16_t *)0x800dabb8 = state->pastBorderL;
    *(int16_t *)0x800dabbe = state->pastBorderB;
    *(int16_t *)0x800dabbc = state->pastBorderT;
    *(int16_t *)0x800dac0e = state->camX2;
    *(int16_t *)0x800dac12 = state->camY2;
    *(int16_t *)0x800dac30 = state->pastCamX2;
    *(int16_t *)0x800dac32 = state->pastCamY2;
    *(int16_t *)0x800d7fe8 = state->BG1_X;
    *(int16_t *)0x800d7fea = state->BG1_Y;
    *(int16_t *)0x800d7fec = state->BG2_X;
    *(int16_t *)0x800d7fee = state->BG2_Y;
    *(int16_t *)0x800d7ff0 = state->BG3_X;
    *(int16_t *)0x800d7ff2 = state->BG3_Y;
    *(int16_t *)0x800d7ede = state->textureDumpOffset;
    *(int16_t *)0x800d8040 = state->textureLength;
    *(int16_t *)0x800dac14 = state->BG2_baseX;
    *(int16_t *)0x800dac16 = state->BG2_baseY;
    *(int16_t *)0x800d80fa = state->tankAmmo;
    *(int16_t *)0x800d80fc = state->tankAmmo2;
 
    *(uint8_t *)0x800daba9 = state->scrollType;
    *(uint8_t *)0x800dac09 = state->scrollType2;
    *(uint8_t *)0x800d7ff5 = state->drawFlags;
    *(uint8_t *)0x800d8044 = state->decompressFlag;
    *(uint8_t *)0x800d8082 = state->effectId;
    *(uint8_t *)0x800d8045 = state->textureId;
    *(uint8_t *)0x800d8093 = state->disableMega;
    *(uint8_t *)0x800d8094 = state->disableWeapons;
    *(uint8_t *)0x800d8095 = state->disableShots;
    *(uint8_t *)0x800d8096 = state->disableEnemies;
    *(uint8_t *)0x800d8097 = state->disableItems;
    *(uint8_t *)0x800d8098 = state->disableMisc;
    *(uint8_t *)0x800d8099 = state->disableEffect;
    *(uint8_t *)0x800d80d1 = state->doorFlag;
    *(uint8_t *)0x800d80ad = state->BG3_flag;
    *(uint8_t *)0x800d80f8 = state->checkPoint;
    *(uint8_t *)0x800d80c1 = state->pauseEnable;
    *(uint8_t *)0x800d809a = state->collisionFlag;
    *(uint8_t *)0x800fe7cb = state->transFlag;
    *(uint8_t *)0x800d80f7 = state->lives;
    *(uint8_t *)0x800d8087 = state->contact;
    *(uint8_t *)0x800d811d = state->refights;
    *(uint8_t *)0x800d80f5 = state->upgrades;
    *(uint8_t *)0x800d8114 = state->armorParts;
    *(uint8_t *)0x800d8117 = state->hearts;
    *(uint8_t *)0x800d8115 = state->maxHP;
    *(uint8_t *)0x800d8116 = state->flags;
    *(uint8_t *)0x800d80d6 = state->controls;
    *(uint8_t *)0x800d7ff4 = state->brightness;
    gameMode3 = state->gameMode3;
    gameMode4 = state->gameMode4;
    *(uint8_t *)0x800da898 = state->ready;
    *(uint8_t *)0x800d80cf = state->zero;
    *(uint8_t *)0x800d8124 = state->zero2;
    *(uint8_t *)0x800d8092 = state->ammoAct;
    *(uint8_t *)0x800d80b7 = state->cutscene;

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