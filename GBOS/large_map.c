#include <gb/gb.h>
#include <stdint.h>
#include <stdio.h>

#include "map_map.h"
#include "map_tiles.h"

#define DATASIZE 1000

uint8_t sprite_data[] = {
    0xff,0x00,0xff,0x7e,0xff,0x7c,0xfe,0x78,0xff,0x7c,0xff,0x6e,0xff,0x46,0xef,0x00
};
UINT8 joypadPrevious;

uint8_t br=0;
char brfuck[1000];
uint16_t brlen=0;

joypads_t joypads;

int16_t mox=64;
int16_t moy=64;

void brainfuck(char command_pointer[]) {
  int bracket_flag;
  char command;
  int data[DATASIZE] = {0};
  uint16_t i=0;
  int *dp;   /* data pointer */ 
  uint8_t input=1;
  /* Move dp to middle of the data array */
  dp = &data[DATASIZE/2];
  while (command=command_pointer[i++]){
    switch (command) {
    case '>':   /* Move data pointer to next address */
      dp++;
      break;
    case '<':   /* Move data pointer to previous address */
      dp--;
      break;
    case '+':   /* Increase value at current data cell by one */
      (*dp)++;
      break;
    case '-':   /* Decrease value at current data cell by one */
      (*dp)--;
      break;
    case '.':   /* Output character at current data cell */
      printf("\n%d", *dp);
      break;
    case ',':   /* Accept one character from input pointer ip and
                   advance to next one */
      printf("_");
      joypadPrevious=joypad();
      while(input) {
          if ((joypad() & J_LEFT) && !(joypadPrevious & J_LEFT)) (*dp)--, printf("%d\n", *dp);
          if ((joypad() & J_RIGHT) && !(joypadPrevious & J_RIGHT)) (*dp)++, printf("%d\n", *dp);
          if ((joypad() & J_UP) && !(joypadPrevious & J_UP)) (*dp)+=10, printf("%d\n", *dp);
          if ((joypad() & J_DOWN) && !(joypadPrevious & J_DOWN)) (*dp)-=10, printf("%d\n", *dp);
          if ((joypad() & J_START) && !(joypadPrevious & J_START)) input=0;
          joypadPrevious=joypad();
      }
      input=1;
      break;
    case '[':   /* When the value at current data cell is 0,
                   advance to next matching ] */
      if (!*dp) { 
        for (bracket_flag=1; bracket_flag; i++){
          if (command_pointer[i] == '[') bracket_flag++;
          else if (command_pointer[i] == ']') bracket_flag--;
        }
      } 
      break;
    case ']':    /* Moves the command pointer back to matching
                    opening [ if the value of current data cell is
                    non-zero */
      if (*dp) {
        /* Move command pointer just before ] */   
        i -= 2;  
        for (bracket_flag=1; bracket_flag; i--){
          if (command_pointer[i] == ']') bracket_flag++;
          else if (command_pointer[i] == '[') bracket_flag--;
        /* Advance pointer after loop to match with opening [ */  
        }
        i++;   
      }
      break;  
    }
  }
  /* Adding new line after end of the program */
  printf("\n");
}

void main(void) {
    DISPLAY_ON;
    SHOW_BKG;
    set_bkg_data(0, 24u, map_tiles);
    set_bkg_tiles(0,0,20,18,map_map);
    BGP_REG = OBP0_REG = OBP1_REG = 0xE4;
    set_sprite_data(0, 1, sprite_data);
    set_sprite_tile(0, 0);
    move_sprite(0, (0 << 3) + 64, 64);
    SHOW_SPRITES;

    NR52_REG = 0x80; // is 1000 0000 in binary and turns on sound
    NR50_REG = 0x77; // sets the volume for both left and right channel just set to max 0x77
    NR51_REG = 0xFF; // is 1111 1111 in binary, select which chanels we want to use in this case all of them. One bit for the L one bit for the R of all four channels
    
    while(1) {
        // iterate joypads, move sprites
            if (br==0){
            if (joypad() & J_LEFT) scroll_sprite(0, -2, 0), mox-=2;
            if (joypad() & J_RIGHT) scroll_sprite(0, 2, 0), mox+=2;
            if (joypad() & J_UP) scroll_sprite(0, 0, -2), moy-=2;
            if (joypad() & J_DOWN) scroll_sprite(0, 0, 2), moy+=2;
            if ((joypad() & J_A)  && !(joypadPrevious & J_A)){
                NR10_REG=0X00;
                NR11_REG=0X81;
                NR12_REG=0X43;
                NR13_REG=0X73;
                NR14_REG=0X86;
                if ((mox>110) && (moy>136)){
                    br=1;
                }
            }
            }
            // up-a=+  down-a=-  left-a=<  right-a=>  up-b=,(input, upload)  down-b=.(output,download)  left-b=[  right-b=]  start=run  select=edit
            if (br==1) {
            if ((joypad() & J_UP) && (joypad() & J_A) && !(joypadPrevious & J_UP)) brfuck[brlen]='+', printf("%c", brfuck[brlen]), brlen++;
            if ((joypad() & J_DOWN) && (joypad() & J_A) && !(joypadPrevious & J_DOWN)) brfuck[brlen]='-', printf("%c", brfuck[brlen]), brlen++;
            if ((joypad() & J_LEFT) && (joypad() & J_A) && !(joypadPrevious & J_LEFT)) brfuck[brlen]='<', printf("%c", brfuck[brlen]), brlen++;
            if ((joypad() & J_RIGHT) && (joypad() & J_A) && !(joypadPrevious & J_RIGHT)) brfuck[brlen]='>', printf("%c", brfuck[brlen]), brlen++;

            if ((joypad() & J_UP) && (joypad() & J_B) && !(joypadPrevious & J_UP)) brfuck[brlen]=',', printf("%c", brfuck[brlen]), brlen++;
            if ((joypad() & J_DOWN) && (joypad() & J_B) && !(joypadPrevious & J_DOWN)) brfuck[brlen]='.', printf("%c", brfuck[brlen]), brlen++;
            if ((joypad() & J_LEFT) && (joypad() & J_B) && !(joypadPrevious & J_LEFT)) brfuck[brlen]='[', printf("%c", brfuck[brlen]), brlen++;
            if ((joypad() & J_RIGHT) && (joypad() & J_B) && !(joypadPrevious & J_RIGHT)) brfuck[brlen]=']', printf("%c", brfuck[brlen]), brlen++;
            
            if ((joypad() & J_START) && !(joypadPrevious & J_START)) brainfuck(brfuck);
            }
        joypadPrevious=joypad();
        wait_vbl_done();
    }
}