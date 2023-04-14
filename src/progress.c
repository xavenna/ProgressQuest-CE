#include "progress.h"

void initPl(struct Player* p) {
  /* initialize player */
  p->Str = randInt(1, 6) + randInt(1, 6) + randInt(1, 6);
  p->Dex = randInt(1, 6) + randInt(1, 6) + randInt(1, 6);
  p->Con = randInt(1, 6) + randInt(1, 6) + randInt(1, 6);
  p->Int = randInt(1, 6) + randInt(1, 6) + randInt(1, 6);
  p->Wis = randInt(1, 6) + randInt(1, 6) + randInt(1, 6);
  p->Cha = randInt(1, 6) + randInt(1, 6) + randInt(1, 6);
  p->HP_max = 2 + p->Con / 3;
  p->MP_max = 2 + p->Int / 3; //I'm not sure how accurate this is to the real game
  p->kill = 0;
  p->kill_max = 17; //this may change
  p->level = 1;
  p->exp = 0;
  p->target_exp = expToLevelUp(1);
  xv_MoveCursorAndPrint("Enter your Name: ", 0, 0);
  char buf[80];
  getstr(buf, sizeof(buf), true);
  strncpy(p->name, buf, sizeof(buf));
}

uint8_t loadPl(struct Player* p, const char* name) {
  const uint8_t handle = ti_Open(name, "r");
  if(handle == 0) {
    ti_Close(handle);
    return 2;
  }

  const size_t read = ti_Read(p, sizeof(struct Player), 1, handle);

  if(read != 1) {
    return 1;
  }
  return 0;
}

uint8_t writePl(struct Player* p, const char* name) {
  const uint8_t handle = ti_Open(name, "w");
  if(handle == 0) {
    ti_Close(handle);
    return 2;
  }

  const size_t write = ti_Write(p, sizeof(struct Player), 1, handle);

  if(write != 1) {
    return 1;
  }
  return 0;
}

void screenSetup() {
  os_ClrHome();
  xv_MoveCursorAndPrint("LEVEL:     TO NEXT:      ", 1, 0);
  xv_MoveCursorAndPrint("STR:       INT:", 2, 0);
  xv_MoveCursorAndPrint("DEX:       WIS:", 3, 0);
  xv_MoveCursorAndPrint("CON:       CHA:", 4, 0);
  xv_MoveCursorAndPrint("KILLING:", 6, 0);
  xv_MoveCursorAndPrint("KILL  \xc1                ]", 8, 0);
  xv_MoveCursorAndPrint("LEVEL \xc1                ]", 9, 0);
  // I have to use \xc1 instead of [ because TI's character set is strange
  // the character corresponding to [ in ascii is theta in TI's set

  //  Generate a name for the first monster
  char name[50];
  generateName(name);
  xv_MoveCursorAndPrint(name, 7, 1);
}

void dispUpd(struct Player* p) {
  int level_completion = (int)(17 * ((float)p->exp / (float) p->target_exp)) ;
  int kill = (int)(17 * ((float)p->kill / (float) p->kill_max)) ;
  char level_bar[17]; //it needs to be 17 characters b/c of the null character
  char kill_bar[17];
  xv_MoveCursorAndPrint(p->name, 0, 0);
  printNCharsOfInt(p->Str, 3, 2, 5);
  printNCharsOfInt(p->Dex, 3, 3, 5);
  printNCharsOfInt(p->Con, 3, 4, 5);
  printNCharsOfInt(p->Int, 3, 2, 16);
  printNCharsOfInt(p->Wis, 3, 3, 16);
  printNCharsOfInt(p->Cha, 3, 4, 16);

  printNCharsOfInt(p->level, 4, 1, 7);
  printNCharsOfInt(p->target_exp - p->exp, 4, 1, 20); 


  /* draw killing progressbar */
  for(int i=0;i<kill;i++) {
    kill_bar[i] = '=';
  }
  for(int i=kill;i<16;i++) {
    kill_bar[i] = ' ';
  }
  kill_bar[16] = '\0';
  xv_MoveCursorAndPrint(kill_bar, 8, 7);
  
  /* draw level-up bar */
  for(int i=0;i<level_completion;i++) {
    level_bar[i] = '=';
  }
  for(int i=level_completion;i<16;i++) {
    level_bar[i] = ' ';
  }
  level_bar[16] = '\0';
  xv_MoveCursorAndPrint(level_bar, 9, 7);
}
/*  screen layout
+01234567890123456789012345+
0name                      0
1level: xx  to next: xxxxx 1
2str: xx    int: xx        2
3dex: xx    wis: xx        3
4con: xx    cha: xx        4
5                          5
6KILLING:                  6
7 monster-name             7
8 kill   [                ]8
9 level  [                ]9
+01234567890123456789012345+
*/

int expToLevelUp(int level) {
  return(50 + 20 *  pow(1.15, level-1)); // this will need to be balanced
}

void levelUp(struct Player* p) {
  /* update thing */
  p->level++;
  p->Str += randInt(1,3);
  p->Dex += randInt(1,3);
  p->Con += randInt(1,3);
  p->Int += randInt(1,3);
  p->Wis += randInt(1,3);
  p->Cha += randInt(1,3);
  p->HP_max += p->Con / 3 + 1 + randInt(0,4);
  p->MP_max += p->Int / 3 + 1 + randInt(0,4);
  p->target_exp = expToLevelUp(p->level);
  p->exp = 0;
}


void updateKill(struct Player* p) {
  p->kill++;
  if(p->kill == p->kill_max) {

    // kill complete:
    p->exp += randInt(3, 9);
    p->kill = 0;

    char name[50];
    generateName(name);
    xv_MoveCursorAndPrint(name, 7, 1);
    xv_MoveCursorAndPrint("     ", 1, 20); //fixes garbage data being left over on screen
  }

  if(p->exp >= p->target_exp) {
    levelUp(p);
  }
}

void getstr(char* dest, int maxlen, bool echo) {
  // this function used example code from https://ce-programming.github.io/toolchain/headers/ti/getcsc.html
  const char *chars = "\0\0\0\0\0\0\0\0\0\0\"WRMH\0\0?[VQLG\0\0:ZUPKFC\0 YTOJEB\0\0XSNIDA\0\0\0\0\0\0\0\0";
  uint8_t key, i = 0;

  while((key = os_GetCSC()) != sk_Enter) {
    if(i == maxlen) {
      break;
    }
    if(chars[key]) {
      dest[i++] = chars[key];
      if(echo) {
        char t[2];
        t[0] = chars[key];
        t[1] = 0;
        os_PutStrFull(t);
      }
    }
  }
  dest[i] = 0; /* add a null character to terminate string */

}

void printNCharsOfInt(int24_t num, size_t len, char y, char x) {  /*  Only prints up to 12 chars  */
  char temp[12];
  char temp2[12];
  const real_t t = os_Int24ToReal(num);
  os_RealToStr(temp, &(t), 6, 1, -1);
  strncpy(temp2, temp, len+1);
  temp2[len] = 0;
  os_SetCursorPos(y, x);
  os_PutStrLine(temp2);
}

void generateName(char* dest) {
  /* create a new monster name 
   * for now, just have a list and choose a random element
   * if there's space on the screen, add modifiers
   * */
  const char* names[] = {
    "Beer Golem            ",
    "Shambling Mound       ",
    "Sand Elemental        ",
    "Ice Devil             ",
    "Demogorgon            ",
    "Hair Elemental        ",
    "Cheese Elemental      ",
    "Storm Giant           ",
    "Brontosaurus          ",
    "Quartz Giant          ",
    "Remorhaz              ",
    "Beige Dragon          ",
    "Will-o-the-wisp       ",
    "Naga                  ",
    "Ghoul                 ",
    "jellyrock             "
  };
  unsigned index = randInt(0, 15);
  strcpy(dest, names[index]);
}
