#include "progress.h"

/* This is a clone of ProgressQuest, an idle game, for the TI-84+CE
 * This version was written by xavenna.
 *
 * This Project is Released under the MIT License. See the LICENSE File for more details.
 * This Program comes with ABSOLUTELY NO WARRANTY.
 *
 * ProgressQuest can be found here:
 * http://progressquest.com/
 *
 * Inspired by this project: https://github.com/TheMostOGName/TI-PQ
 * The original version was written in TI-BASIC, but I decided to rewrite it in c to improve it
 *
 */

int main(void) {
  
  /*  DEFINE VARIABLES  */
  
  bool run = true;
  uint8_t programCounter = 0;
  uint8_t maxPC = 10;

  os_ClrHome(); 
  struct Player player;

  if(loadPl(&player, "PQPLAYER") != 0) {
    /* load failure. Create a new player */
    initPl(&player);
  }



  srandom(rtc_Time());
  /* SET UP SCREEN */
  
  os_SetCursorPos(0, 0);
  os_PutStrFull("ProgressQuest CE          Created by xavenna        Inspired by TI-PQ, by     TheMostOGName             See readme for details");
  os_SetCursorPos(5,0);
  xv_Pause()
  
  kb_EnableOnLatch();
  kb_ClearOnLatch();
  
  
  /*  Setup Screen  */
  screenSetup();


  do {  /* Main program loop */
    
    /* loop initialization */
    
    kb_Scan();  /* get keys pressed */
    
    /*  only check key presses every 5 frames to avoid multiple presses */
    if(kb_On) {  /*  the on check can happen every frame  */
      /* break */
      run = false;
    }
    
    /* Intermediate processing */

    programCounter++;
    if(programCounter == maxPC) {
      updateKill(&player);
      programCounter = 0;
    }
    
    /*  refresh display */
    dispUpd(&player);
  } while(run);
  /*  draw the end screen  */
  os_ClrHome();
  xv_MoveCursorAndPrint("Goodbye...",  0,  0);

  uint8_t status = writePl(&player, "PQPLAYER");
  if(status == 2) {
    xv_MoveCursorAndPrint("Appvar not found", 2, 0);
  }
  else if(status == 1) {
    xv_MoveCursorAndPrint("Write failure", 2, 0);
  }
  else {
    xv_MoveCursorAndPrint("Write successful", 2, 0);
  }
  // eventually save player to appvar here
  xv_Pause();
  kb_DisableOnLatch();
  return 0;
}


