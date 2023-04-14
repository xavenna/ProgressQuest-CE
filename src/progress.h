#ifndef PROGRESS_H
#define PROGRESS_H


/* some utility macros */
#define xv_Pause() while(os_GetCSC() != sk_Enter)
#define xv_MoveCursorAndPrint(st, y, x) \
  os_SetCursorPos(y, x);		\
  os_PutStrFull(st)

#include <fileioc.h>

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

#include <keypadc.h>

#include <ti/screen.h>
#include <ti/getcsc.h>  /*  Included for the loading screen  */
#include <ti/real.h>
#include <ti/getkey.h>

#include <sys/timers.h>
#include <sys/util.h>
#include <sys/rtc.h>

#include "charset.h"

struct Player {
  uint24_t Str;
  uint24_t Dex;
  uint24_t Con;
  uint24_t Int;
  uint24_t Wis;
  uint24_t Cha;

  uint24_t HP_max;
  uint24_t MP_max;
  char name[80];

  uint24_t level;
  uint24_t exp;
  uint24_t target_exp;

  uint24_t kill;
  uint24_t kill_max;

}; 
void printNCharsOfInt(int24_t, size_t, char, char);
void initPl(struct Player*);
void dispUpd(struct Player*);

void updateKill(struct Player*);
uint8_t loadPl(struct Player*, const char*);
uint8_t writePl(struct Player*, const char*);

void levelUp(struct Player*);

int expToLevelUp(int);

void screenSetup(void);

void getstr(char*, int, bool);

void generateName(char*);

#endif
