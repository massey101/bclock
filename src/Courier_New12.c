#include "fonts.h"
#include <avr/pgmspace.h>

const uint8_t Courier_New12_Table [] PROGMEM = 
{
    // @0 ' ' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       

    // @12 '!' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x10, //    #  
    0x10, //    #  
    0x10, //    #  
    0x10, //    #  
    0x00, //       
    0x10, //    #  
    0x00, //       
    0x00, //       
    0x00, //       

    // @24 '"' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x28, //   # # 
    0x28, //   # # 
    0x28, //   # # 
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       

    // @36 '#' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x28, //   # # 
    0x50, //  # #  
    0xf8, // ##### 
    0x50, //  # #  
    0x50, //  # #  
    0xf8, // ##### 
    0x50, //  # #  
    0xa0, // # #   
    0x00, //       
    0x00, //       

    // @48 '$' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x10, //    #  
    0x38, //   ### 
    0x40, //  #    
    0x20, //   #   
    0x18, //    ## 
    0x70, //  ###  
    0x20, //   #   
    0x20, //   #   
    0x00, //       
    0x00, //       

    // @60 '%' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x20, //   #   
    0x50, //  # #  
    0x38, //   ### 
    0x70, //  ###  
    0x28, //   # # 
    0x10, //    #  
    0x00, //       
    0x00, //       
    0x00, //       

    // @72 '&' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x38, //   ### 
    0x20, //   #   
    0x20, //   #   
    0x58, //  # ## 
    0x78, //  #### 
    0x00, //       
    0x00, //       
    0x00, //       

    // @84 ''' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x10, //    #  
    0x10, //    #  
    0x10, //    #  
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       

    // @96 '(' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x10, //    #  
    0x20, //   #   
    0x20, //   #   
    0x20, //   #   
    0x20, //   #   
    0x20, //   #   
    0x10, //    #  
    0x00, //       
    0x00, //       

    // @108 ')' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x20, //   #   
    0x10, //    #  
    0x10, //    #  
    0x10, //    #  
    0x10, //    #  
    0x10, //    #  
    0x20, //   #   
    0x00, //       
    0x00, //       

    // @120 '*' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x10, //    #  
    0x7c, //  #####
    0x30, //   ##  
    0x28, //   # # 
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       

    // @132 '+' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x20, //   #   
    0x20, //   #   
    0xf8, // ##### 
    0x20, //   #   
    0x20, //   #   
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       

    // @144 ',' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x10, //    #  
    0x20, //   #   
    0x20, //   #   
    0x00, //       
    0x00, //       

    // @156 '-' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x78, //  #### 
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       

    // @168 '.' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x30, //   ##  
    0x00, //       
    0x00, //       
    0x00, //       

    // @180 '/' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x10, //    #  
    0x10, //    #  
    0x20, //   #   
    0x20, //   #   
    0x40, //  #    
    0x40, //  #    
    0x80, // #     
    0x80, // #     
    0x00, //       
    0x00, //       

    // @192 '0' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x30, //   ##  
    0x48, //  #  # 
    0x48, //  #  # 
    0x48, //  #  # 
    0x48, //  #  # 
    0x30, //   ##  
    0x00, //       
    0x00, //       
    0x00, //       

    // @204 '1' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x30, //   ##  
    0x10, //    #  
    0x10, //    #  
    0x10, //    #  
    0x10, //    #  
    0x7c, //  #####
    0x00, //       
    0x00, //       
    0x00, //       

    // @216 '2' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x30, //   ##  
    0x48, //  #  # 
    0x10, //    #  
    0x20, //   #   
    0x40, //  #    
    0x78, //  #### 
    0x00, //       
    0x00, //       
    0x00, //       

    // @228 '3' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x30, //   ##  
    0x48, //  #  # 
    0x10, //    #  
    0x08, //     # 
    0x48, //  #  # 
    0x30, //   ##  
    0x00, //       
    0x00, //       
    0x00, //       

    // @240 '4' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x18, //    ## 
    0x28, //   # # 
    0x48, //  #  # 
    0x7c, //  #####
    0x08, //     # 
    0x1c, //    ###
    0x00, //       
    0x00, //       
    0x00, //       

    // @252 '5' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x78, //  #### 
    0x40, //  #    
    0x70, //  ###  
    0x08, //     # 
    0x48, //  #  # 
    0x30, //   ##  
    0x00, //       
    0x00, //       
    0x00, //       

    // @264 '6' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x38, //   ### 
    0x40, //  #    
    0x70, //  ###  
    0x48, //  #  # 
    0x48, //  #  # 
    0x30, //   ##  
    0x00, //       
    0x00, //       
    0x00, //       

    // @276 '7' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x78, //  #### 
    0x48, //  #  # 
    0x08, //     # 
    0x10, //    #  
    0x10, //    #  
    0x10, //    #  
    0x00, //       
    0x00, //       
    0x00, //       

    // @288 '8' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x30, //   ##  
    0x48, //  #  # 
    0x30, //   ##  
    0x48, //  #  # 
    0x48, //  #  # 
    0x30, //   ##  
    0x00, //       
    0x00, //       
    0x00, //       

    // @300 '9' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x30, //   ##  
    0x48, //  #  # 
    0x48, //  #  # 
    0x38, //   ### 
    0x08, //     # 
    0x70, //  ###  
    0x00, //       
    0x00, //       
    0x00, //       

    // @312 ':' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x20, //   #   
    0x00, //       
    0x00, //       
    0x20, //   #   
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       

    // @324 ';' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x10, //    #  
    0x00, //       
    0x00, //       
    0x10, //    #  
    0x20, //   #   
    0x20, //   #   
    0x00, //       

    // @336 '<' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x18, //    ## 
    0x20, //   #   
    0xc0, // ##    
    0x20, //   #   
    0x18, //    ## 
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       

    // @348 '=' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x7c, //  #####
    0x00, //       
    0x7c, //  #####
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       

    // @360 '>' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x60, //  ##   
    0x10, //    #  
    0x0c, //     ##
    0x10, //    #  
    0x60, //  ##   
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       

    // @372 '?' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x30, //   ##  
    0x48, //  #  # 
    0x08, //     # 
    0x10, //    #  
    0x00, //       
    0x10, //    #  
    0x00, //       
    0x00, //       
    0x00, //       

    // @384 '@' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x38, //   ### 
    0x44, //  #   #
    0x4c, //  #  ##
    0x54, //  # # #
    0x4c, //  #  ##
    0x40, //  #    
    0x3c, //   ####
    0x00, //       
    0x00, //       

    // @396 'A' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x30, //   ##  
    0x10, //    #  
    0x28, //   # # 
    0x38, //   ### 
    0x44, //  #   #
    0x6c, //  ## ##
    0x00, //       
    0x00, //       
    0x00, //       

    // @408 'B' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0xf0, // ####  
    0x48, //  #  # 
    0x70, //  ###  
    0x48, //  #  # 
    0x48, //  #  # 
    0xf0, // ####  
    0x00, //       
    0x00, //       
    0x00, //       

    // @420 'C' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x38, //   ### 
    0x48, //  #  # 
    0x40, //  #    
    0x40, //  #    
    0x40, //  #    
    0x38, //   ### 
    0x00, //       
    0x00, //       
    0x00, //       

    // @432 'D' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0xf0, // ####  
    0x48, //  #  # 
    0x48, //  #  # 
    0x48, //  #  # 
    0x48, //  #  # 
    0xf0, // ####  
    0x00, //       
    0x00, //       
    0x00, //       

    // @444 'E' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0xf8, // ##### 
    0x48, //  #  # 
    0x60, //  ##   
    0x40, //  #    
    0x48, //  #  # 
    0xf8, // ##### 
    0x00, //       
    0x00, //       
    0x00, //       

    // @456 'F' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x7c, //  #####
    0x24, //   #  #
    0x30, //   ##  
    0x20, //   #   
    0x20, //   #   
    0x70, //  ###  
    0x00, //       
    0x00, //       
    0x00, //       

    // @468 'G' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x38, //   ### 
    0x40, //  #    
    0x40, //  #    
    0x5c, //  # ###
    0x48, //  #  # 
    0x30, //   ##  
    0x00, //       
    0x00, //       
    0x00, //       

    // @480 'H' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0xdc, // ## ###
    0x48, //  #  # 
    0x78, //  #### 
    0x48, //  #  # 
    0x48, //  #  # 
    0xec, // ### ##
    0x00, //       
    0x00, //       
    0x00, //       

    // @492 'I' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x38, //   ### 
    0x10, //    #  
    0x10, //    #  
    0x10, //    #  
    0x10, //    #  
    0x38, //   ### 
    0x00, //       
    0x00, //       
    0x00, //       

    // @504 'J' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x1c, //    ###
    0x08, //     # 
    0x08, //     # 
    0x48, //  #  # 
    0x48, //  #  # 
    0x30, //   ##  
    0x00, //       
    0x00, //       
    0x00, //       

    // @516 'K' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0xec, // ### ##
    0x48, //  #  # 
    0x50, //  # #  
    0x70, //  ###  
    0x48, //  #  # 
    0xec, // ### ##
    0x00, //       
    0x00, //       
    0x00, //       

    // @528 'L' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0xe0, // ###   
    0x40, //  #    
    0x40, //  #    
    0x40, //  #    
    0x48, //  #  # 
    0xf8, // ##### 
    0x00, //       
    0x00, //       
    0x00, //       

    // @540 'M' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0xdc, // ## ###
    0xd8, // ## ## 
    0xd8, // ## ## 
    0xa8, // # # # 
    0x88, // #   # 
    0xdc, // ## ###
    0x00, //       
    0x00, //       
    0x00, //       

    // @552 'N' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0xdc, // ## ###
    0x68, //  ## # 
    0x68, //  ## # 
    0x58, //  # ## 
    0x58, //  # ## 
    0xe8, // ### # 
    0x00, //       
    0x00, //       
    0x00, //       

    // @564 'O' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x30, //   ##  
    0x48, //  #  # 
    0x48, //  #  # 
    0x48, //  #  # 
    0x48, //  #  # 
    0x30, //   ##  
    0x00, //       
    0x00, //       
    0x00, //       

    // @576 'P' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0xf0, // ####  
    0x48, //  #  # 
    0x48, //  #  # 
    0x70, //  ###  
    0x40, //  #    
    0xe0, // ###   
    0x00, //       
    0x00, //       
    0x00, //       

    // @588 'Q' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x30, //   ##  
    0x48, //  #  # 
    0x48, //  #  # 
    0x48, //  #  # 
    0x48, //  #  # 
    0x30, //   ##  
    0x18, //    ## 
    0x00, //       
    0x00, //       

    // @600 'R' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0xf0, // ####  
    0x48, //  #  # 
    0x48, //  #  # 
    0x70, //  ###  
    0x48, //  #  # 
    0xe4, // ###  #
    0x00, //       
    0x00, //       
    0x00, //       

    // @612 'S' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x38, //   ### 
    0x40, //  #    
    0x30, //   ##  
    0x08, //     # 
    0x48, //  #  # 
    0x70, //  ###  
    0x00, //       
    0x00, //       
    0x00, //       

    // @624 'T' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0xf8, // ##### 
    0xa8, // # # # 
    0x20, //   #   
    0x20, //   #   
    0x20, //   #   
    0x70, //  ###  
    0x00, //       
    0x00, //       
    0x00, //       

    // @636 'U' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0xdc, // ## ###
    0x48, //  #  # 
    0x48, //  #  # 
    0x48, //  #  # 
    0x48, //  #  # 
    0x30, //   ##  
    0x00, //       
    0x00, //       
    0x00, //       

    // @648 'V' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0xcc, // ##  ##
    0x84, // #    #
    0x48, //  #  # 
    0x48, //  #  # 
    0x48, //  #  # 
    0x30, //   ##  
    0x00, //       
    0x00, //       
    0x00, //       

    // @660 'W' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0xec, // ### ##
    0x44, //  #   #
    0x54, //  # # #
    0x54, //  # # #
    0x54, //  # # #
    0x28, //   # # 
    0x00, //       
    0x00, //       
    0x00, //       

    // @672 'X' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0xd8, // ## ## 
    0x50, //  # #  
    0x20, //   #   
    0x20, //   #   
    0x50, //  # #  
    0xd8, // ## ## 
    0x00, //       
    0x00, //       
    0x00, //       

    // @684 'Y' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0xec, // ### ##
    0x44, //  #   #
    0x28, //   # # 
    0x10, //    #  
    0x10, //    #  
    0x38, //   ### 
    0x00, //       
    0x00, //       
    0x00, //       

    // @696 'Z' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x78, //  #### 
    0x48, //  #  # 
    0x10, //    #  
    0x20, //   #   
    0x48, //  #  # 
    0x78, //  #### 
    0x00, //       
    0x00, //       
    0x00, //       

    // @708 '[' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x30, //   ##  
    0x20, //   #   
    0x20, //   #   
    0x20, //   #   
    0x20, //   #   
    0x20, //   #   
    0x30, //   ##  
    0x00, //       
    0x00, //       

    // @720 '\' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x80, // #     
    0x80, // #     
    0x40, //  #    
    0x40, //  #    
    0x20, //   #   
    0x20, //   #   
    0x10, //    #  
    0x10, //    #  
    0x00, //       
    0x00, //       

    // @732 ']' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x30, //   ##  
    0x10, //    #  
    0x10, //    #  
    0x10, //    #  
    0x10, //    #  
    0x10, //    #  
    0x30, //   ##  
    0x00, //       
    0x00, //       

    // @744 '^' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x30, //   ##  
    0x48, //  #  # 
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       

    // @756 '_' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0xfc, // ######

    // @768 '`' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x20, //   #   
    0x10, //    #  
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       

    // @780 'a' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x38, //   ### 
    0x08, //     # 
    0x78, //  #### 
    0x7c, //  #####
    0x00, //       
    0x00, //       
    0x00, //       

    // @792 'b' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0xc0, // ##    
    0x40, //  #    
    0x70, //  ###  
    0x48, //  #  # 
    0x48, //  #  # 
    0xf0, // ####  
    0x00, //       
    0x00, //       
    0x00, //       

    // @804 'c' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x38, //   ### 
    0x40, //  #    
    0x40, //  #    
    0x38, //   ### 
    0x00, //       
    0x00, //       
    0x00, //       

    // @816 'd' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x18, //    ## 
    0x08, //     # 
    0x38, //   ### 
    0x48, //  #  # 
    0x48, //  #  # 
    0x3c, //   ####
    0x00, //       
    0x00, //       
    0x00, //       

    // @828 'e' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x30, //   ##  
    0x78, //  #### 
    0x40, //  #    
    0x38, //   ### 
    0x00, //       
    0x00, //       
    0x00, //       

    // @840 'f' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x18, //    ## 
    0x20, //   #   
    0x78, //  #### 
    0x20, //   #   
    0x20, //   #   
    0x78, //  #### 
    0x00, //       
    0x00, //       
    0x00, //       

    // @852 'g' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x3c, //   ####
    0x48, //  #  # 
    0x48, //  #  # 
    0x38, //   ### 
    0x08, //     # 
    0x70, //  ###  
    0x00, //       

    // @864 'h' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0xc0, // ##    
    0x40, //  #    
    0x70, //  ###  
    0x48, //  #  # 
    0x48, //  #  # 
    0xec, // ### ##
    0x00, //       
    0x00, //       
    0x00, //       

    // @876 'i' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x10, //    #  
    0x00, //       
    0x30, //   ##  
    0x10, //    #  
    0x10, //    #  
    0x38, //   ### 
    0x00, //       
    0x00, //       
    0x00, //       

    // @888 'j' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x10, //    #  
    0x00, //       
    0x78, //  #### 
    0x08, //     # 
    0x08, //     # 
    0x08, //     # 
    0x08, //     # 
    0x78, //  #### 
    0x00, //       

    // @900 'k' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0xc0, // ##    
    0x40, //  #    
    0x58, //  # ## 
    0x60, //  ##   
    0x50, //  # #  
    0xd8, // ## ## 
    0x00, //       
    0x00, //       
    0x00, //       

    // @912 'l' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x60, //  ##   
    0x20, //   #   
    0x20, //   #   
    0x20, //   #   
    0x20, //   #   
    0x70, //  ###  
    0x00, //       
    0x00, //       
    0x00, //       

    // @924 'm' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0xe8, // ### # 
    0x54, //  # # #
    0x54, //  # # #
    0xd4, // ## # #
    0x00, //       
    0x00, //       
    0x00, //       

    // @936 'n' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0xf0, // ####  
    0x48, //  #  # 
    0x48, //  #  # 
    0xcc, // ##  ##
    0x00, //       
    0x00, //       
    0x00, //       

    // @948 'o' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x30, //   ##  
    0x48, //  #  # 
    0x48, //  #  # 
    0x30, //   ##  
    0x00, //       
    0x00, //       
    0x00, //       

    // @960 'p' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0xf0, // ####  
    0x48, //  #  # 
    0x48, //  #  # 
    0x70, //  ###  
    0x40, //  #    
    0xe0, // ###   
    0x00, //       

    // @972 'q' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x3c, //   ####
    0x48, //  #  # 
    0x48, //  #  # 
    0x38, //   ### 
    0x08, //     # 
    0x1c, //    ###
    0x00, //       

    // @984 'r' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x78, //  #### 
    0x20, //   #   
    0x20, //   #   
    0x78, //  #### 
    0x00, //       
    0x00, //       
    0x00, //       

    // @996 's' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x78, //  #### 
    0x70, //  ###  
    0x08, //     # 
    0x70, //  ###  
    0x00, //       
    0x00, //       
    0x00, //       

    // @1008 't' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x40, //  #    
    0xf8, // ##### 
    0x40, //  #    
    0x48, //  #  # 
    0x30, //   ##  
    0x00, //       
    0x00, //       
    0x00, //       

    // @1020 'u' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0xd8, // ## ## 
    0x48, //  #  # 
    0x48, //  #  # 
    0x3c, //   ####
    0x00, //       
    0x00, //       
    0x00, //       

    // @1032 'v' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0xcc, // ##  ##
    0x48, //  #  # 
    0x30, //   ##  
    0x30, //   ##  
    0x00, //       
    0x00, //       
    0x00, //       

    // @1044 'w' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0xec, // ### ##
    0x54, //  # # #
    0x54, //  # # #
    0x28, //   # # 
    0x00, //       
    0x00, //       
    0x00, //       

    // @1056 'x' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0xd8, // ## ## 
    0x20, //   #   
    0x50, //  # #  
    0xd8, // ## ## 
    0x00, //       
    0x00, //       
    0x00, //       

    // @1068 'y' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0xec, // ### ##
    0x48, //  #  # 
    0x28, //   # # 
    0x10, //    #  
    0x20, //   #   
    0x60, //  ##   
    0x00, //       

    // @1080 'z' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x00, //       
    0x78, //  #### 
    0x50, //  # #  
    0x28, //   # # 
    0x78, //  #### 
    0x00, //       
    0x00, //       
    0x00, //       

    // @1092 '{' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x10, //    #  
    0x20, //   #   
    0x20, //   #   
    0x60, //  ##   
    0x20, //   #   
    0x20, //   #   
    0x10, //    #  
    0x00, //       
    0x00, //       

    // @1104 '|' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x10, //    #  
    0x10, //    #  
    0x10, //    #  
    0x10, //    #  
    0x10, //    #  
    0x10, //    #  
    0x10, //    #  
    0x00, //       
    0x00, //       

    // @1116 '}' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x00, //       
    0x20, //   #   
    0x10, //    #  
    0x10, //    #  
    0x18, //    ## 
    0x10, //    #  
    0x10, //    #  
    0x20, //   #   
    0x00, //       
    0x00, //       

};

sFONT Courier_New12 = {
    Courier_New12_Table,
    7, /* Width */
    12, /* Height*/
};
