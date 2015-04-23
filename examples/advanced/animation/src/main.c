//-----------------------------LICENSE NOTICE------------------------------------
//  This file is part of CPCtelera: An Amstrad CPC Game Engine 
//  Copyright (C) 2015 Dardalorth / Fremos / Carlio
//  Copyright (C) 2015 ronaldo / Fremos / Cheesetea / ByteRealms (@FranGallegoBR)
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//------------------------------------------------------------------------------

#include <cpctelera.h>
#include "entities.h"
#include "sprites.h"

//
// Initialization of the Amstrad CPC at the start of the game
//   Sets Palette and Mode, and disables firmware
//
void initializeCPC() {
   // Disable firmware: we dont want it to interfere with our code
   cpct_disableFirmware();

   // Set the hardware palette (convert firmware colour values to hardware ones and set the palette)
   cpct_fw2hw(gc_palette, 16);
   cpct_setPalette(gc_palette, 16);

   // Change to Mode 0 (160x200, 16 colours)
   cpct_setVideoMode(0);

   // Draw Sky and Fremos Logo
   cpct_drawSolidBox((void*)0xC000, 0xCC, 40, 60);
   cpct_drawSolidBox((void*)0xC028, 0xCC, 40, 60);
   cpct_drawSprite(gc_LogoFremos, (void*)0xC0FC, 55, 20);

   // Draw Floor
   cpct_drawSolidBox((void*)0xC3C0, 0xFF, 40, 8);
   cpct_drawSolidBox((void*)0xC3E8, 0xFF, 40, 8);

   // Draw Underfloor
   cpct_drawSolidBox((void*)0xC410, 0xCC, 40, 96);
   cpct_drawSolidBox((void*)0xC438, 0xCC, 40, 96);
}

//
// Scan Keyboard and do user actions as requested
//
void updateUser(TEntity* user) {
   // Animation Request (new entity status to promote)
   TEntityStatus animrequest = es_stop;

   // Scan Keyboard
   cpct_scanKeyboardFast();

   // Check possible keys to press, and do actions
   if      ( cpct_isKeyPressed(Key_Space)       ) animrequest = es_hit;
   else if ( cpct_isKeyPressed(Key_CursorUp)    ) animrequest = es_kick;
   else if ( cpct_isKeyPressed(Key_CursorDown)  ) animrequest = es_fist;
   else if ( cpct_isKeyPressed(Key_CursorRight) ) animrequest = es_walk_right;
   else if ( cpct_isKeyPressed(Key_CursorLeft)  ) animrequest = es_walk_left;
   else if ( cpct_isKeyPressed(Key_1)           ) animrequest = es_dead;
   else if ( cpct_isKeyPressed(Key_2)           ) animrequest = es_win;

   // Set new animation, based on action requested
   if (animrequest != es_stop)
      setAnimation(user, animrequest);
}

//
// MAIN EXAMPLE CODE
//
void main(void) {
   TEntity* persea;

   // Initialize game
   initializeCPC();
   persea = getPersea();

   // Main Game Loop
   while(1) {
      updateUser(persea);
      cpct_waitVSYNC();
      updateEntity(persea);
      drawEntity(persea);
   }
}
