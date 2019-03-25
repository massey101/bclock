#include "fonts.h"
#include <avr/pgmspace.h>

const uint8_t Courier_New16_Table [] PROGMEM = 
{
    // @0 ' ' (8 pixels wide)
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
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @16 '!' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x08, //     #   
    0x08, //     #   
    0x08, //     #   
    0x08, //     #   
    0x08, //     #   
    0x08, //     #   
    0x00, //         
    0x00, //         
    0x08, //     #   
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @32 '"' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x36, //   ## ## 
    0x36, //   ## ## 
    0x24, //   #  #  
    0x24, //   #  #  
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @48 '#' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x12, //    #  # 
    0x12, //    #  # 
    0x24, //   #  #  
    0x7e, //  ###### 
    0x24, //   #  #  
    0x24, //   #  #  
    0x7e, //  ###### 
    0x24, //   #  #  
    0x48, //  #  #   
    0x48, //  #  #   
    0x00, //         
    0x00, //         
    0x00, //         

    // @64 '$' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x08, //     #   
    0x1c, //    ###  
    0x24, //   #  #  
    0x20, //   #     
    0x18, //    ##   
    0x04, //      #  
    0x24, //   #  #  
    0x38, //   ###   
    0x08, //     #   
    0x08, //     #   
    0x00, //         
    0x00, //         
    0x00, //         

    // @80 '%' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x18, //    ##   
    0x24, //   #  #  
    0x24, //   #  #  
    0x18, //    ##   
    0x3e, //   ##### 
    0x0c, //     ##  
    0x12, //    #  # 
    0x12, //    #  # 
    0x0c, //     ##  
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @96 '&' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x1c, //    ###  
    0x20, //   #     
    0x20, //   #     
    0x30, //   ##    
    0x4c, //  #  ##  
    0x48, //  #  #   
    0x36, //   ## ## 
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @112 ''' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x08, //     #   
    0x08, //     #   
    0x08, //     #   
    0x08, //     #   
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @128 '(' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x04, //      #  
    0x04, //      #  
    0x08, //     #   
    0x08, //     #   
    0x08, //     #   
    0x08, //     #   
    0x08, //     #   
    0x08, //     #   
    0x08, //     #   
    0x04, //      #  
    0x04, //      #  
    0x00, //         
    0x00, //         

    // @144 ')' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x20, //   #     
    0x20, //   #     
    0x20, //   #     
    0x10, //    #    
    0x10, //    #    
    0x10, //    #    
    0x10, //    #    
    0x10, //    #    
    0x20, //   #     
    0x20, //   #     
    0x20, //   #     
    0x00, //         
    0x00, //         

    // @160 '*' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x10, //    #    
    0x10, //    #    
    0x7c, //  #####  
    0x10, //    #    
    0x28, //   # #   
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @176 '+' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x08, //     #   
    0x08, //     #   
    0x08, //     #   
    0x7f, //  #######
    0x08, //     #   
    0x08, //     #   
    0x08, //     #   
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @192 ',' (8 pixels wide)
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
    0x18, //    ##   
    0x10, //    #    
    0x30, //   ##    
    0x20, //   #     
    0x00, //         
    0x00, //         

    // @208 '-' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x7e, //  ###### 
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @224 '.' (8 pixels wide)
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
    0x18, //    ##   
    0x18, //    ##   
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @240 '/' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x02, //       # 
    0x02, //       # 
    0x04, //      #  
    0x04, //      #  
    0x08, //     #   
    0x08, //     #   
    0x10, //    #    
    0x10, //    #    
    0x20, //   #     
    0x20, //   #     
    0x40, //  #      
    0x00, //         
    0x00, //         
    0x00, //         

    // @256 '0' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x3c, //   ####  
    0x42, //  #    # 
    0x42, //  #    # 
    0x42, //  #    # 
    0x42, //  #    # 
    0x42, //  #    # 
    0x42, //  #    # 
    0x42, //  #    # 
    0x3c, //   ####  
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @272 '1' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x08, //     #   
    0x38, //   ###   
    0x08, //     #   
    0x08, //     #   
    0x08, //     #   
    0x08, //     #   
    0x08, //     #   
    0x08, //     #   
    0x3e, //   ##### 
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @288 '2' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x38, //   ###   
    0x44, //  #   #  
    0x04, //      #  
    0x08, //     #   
    0x08, //     #   
    0x10, //    #    
    0x20, //   #     
    0x44, //  #   #  
    0x7c, //  #####  
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @304 '3' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x38, //   ###   
    0x44, //  #   #  
    0x04, //      #  
    0x04, //      #  
    0x18, //    ##   
    0x04, //      #  
    0x04, //      #  
    0x44, //  #   #  
    0x38, //   ###   
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @320 '4' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x0c, //     ##  
    0x14, //    # #  
    0x14, //    # #  
    0x24, //   #  #  
    0x24, //   #  #  
    0x7e, //  ###### 
    0x04, //      #  
    0x04, //      #  
    0x0e, //     ### 
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @336 '5' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x7e, //  ###### 
    0x40, //  #      
    0x40, //  #      
    0x7c, //  #####  
    0x02, //       # 
    0x02, //       # 
    0x02, //       # 
    0x42, //  #    # 
    0x3c, //   ####  
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @352 '6' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x0e, //     ### 
    0x10, //    #    
    0x20, //   #     
    0x20, //   #     
    0x3c, //   ####  
    0x22, //   #   # 
    0x22, //   #   # 
    0x22, //   #   # 
    0x1c, //    ###  
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @368 '7' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x7e, //  ###### 
    0x42, //  #    # 
    0x02, //       # 
    0x04, //      #  
    0x04, //      #  
    0x04, //      #  
    0x08, //     #   
    0x08, //     #   
    0x08, //     #   
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @384 '8' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x38, //   ###   
    0x44, //  #   #  
    0x44, //  #   #  
    0x44, //  #   #  
    0x38, //   ###   
    0x44, //  #   #  
    0x44, //  #   #  
    0x44, //  #   #  
    0x38, //   ###   
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @400 '9' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x3c, //   ####  
    0x42, //  #    # 
    0x42, //  #    # 
    0x42, //  #    # 
    0x42, //  #    # 
    0x3e, //   ##### 
    0x02, //       # 
    0x04, //      #  
    0x78, //  ####   
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @416 ':' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x18, //    ##   
    0x18, //    ##   
    0x00, //         
    0x00, //         
    0x18, //    ##   
    0x18, //    ##   
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @432 ';' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x18, //    ##   
    0x18, //    ##   
    0x00, //         
    0x00, //         
    0x18, //    ##   
    0x10, //    #    
    0x30, //   ##    
    0x20, //   #     
    0x00, //         
    0x00, //         

    // @448 '<' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x03, //       ##
    0x0c, //     ##  
    0x10, //    #    
    0x60, //  ##     
    0x10, //    #    
    0x0c, //     ##  
    0x03, //       ##
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @464 '=' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0xfe, // ####### 
    0x00, //         
    0x00, //         
    0xfe, // ####### 
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @480 '>' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x60, //  ##     
    0x18, //    ##   
    0x04, //      #  
    0x03, //       ##
    0x04, //      #  
    0x18, //    ##   
    0x60, //  ##     
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @496 '?' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x3c, //   ####  
    0x42, //  #    # 
    0x02, //       # 
    0x02, //       # 
    0x0c, //     ##  
    0x10, //    #    
    0x00, //         
    0x18, //    ##   
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @512 '@' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x1c, //    ###  
    0x22, //   #   # 
    0x42, //  #    # 
    0x4e, //  #  ### 
    0x52, //  # #  # 
    0x52, //  # #  # 
    0x4e, //  #  ### 
    0x40, //  #      
    0x22, //   #   # 
    0x1c, //    ###  
    0x00, //         
    0x00, //         
    0x00, //         

    // @528 'A' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x38, //   ###   
    0x18, //    ##   
    0x18, //    ##   
    0x24, //   #  #  
    0x24, //   #  #  
    0x3c, //   ####  
    0x42, //  #    # 
    0xe7, // ###  ###
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @544 'B' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x7c, //  #####  
    0x22, //   #   # 
    0x22, //   #   # 
    0x3c, //   ####  
    0x22, //   #   # 
    0x22, //   #   # 
    0x22, //   #   # 
    0x7c, //  #####  
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @560 'C' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x1a, //    ## # 
    0x26, //   #  ## 
    0x40, //  #      
    0x40, //  #      
    0x40, //  #      
    0x40, //  #      
    0x22, //   #   # 
    0x1c, //    ###  
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @576 'D' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x78, //  ####   
    0x24, //   #  #  
    0x22, //   #   # 
    0x22, //   #   # 
    0x22, //   #   # 
    0x22, //   #   # 
    0x24, //   #  #  
    0x78, //  ####   
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @592 'E' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x7e, //  ###### 
    0x22, //   #   # 
    0x24, //   #  #  
    0x3c, //   ####  
    0x24, //   #  #  
    0x20, //   #     
    0x22, //   #   # 
    0x7e, //  ###### 
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @608 'F' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x7e, //  ###### 
    0x22, //   #   # 
    0x2a, //   # # # 
    0x38, //   ###   
    0x28, //   # #   
    0x20, //   #     
    0x20, //   #     
    0x78, //  ####   
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @624 'G' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x1a, //    ## # 
    0x26, //   #  ## 
    0x40, //  #      
    0x40, //  #      
    0x4f, //  #  ####
    0x42, //  #    # 
    0x22, //   #   # 
    0x1c, //    ###  
    0x00, //         
    0x00, //         
    0x00, //         

    // @640 'H' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x77, //  ### ###
    0x22, //   #   # 
    0x22, //   #   # 
    0x3e, //   ##### 
    0x22, //   #   # 
    0x22, //   #   # 
    0x22, //   #   # 
    0x77, //  ### ###
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @656 'I' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x3e, //   ##### 
    0x08, //     #   
    0x08, //     #   
    0x08, //     #   
    0x08, //     #   
    0x08, //     #   
    0x08, //     #   
    0x3e, //   ##### 
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @672 'J' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x3f, //   ######
    0x04, //      #  
    0x04, //      #  
    0x04, //      #  
    0x44, //  #   #  
    0x44, //  #   #  
    0x44, //  #   #  
    0x38, //   ###   
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @688 'K' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x77, //  ### ###
    0x24, //   #  #  
    0x28, //   # #   
    0x28, //   # #   
    0x38, //   ###   
    0x24, //   #  #  
    0x22, //   #   # 
    0x73, //  ###  ##
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @704 'L' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0xf8, // #####   
    0x20, //   #     
    0x20, //   #     
    0x20, //   #     
    0x22, //   #   # 
    0x22, //   #   # 
    0x22, //   #   # 
    0xfe, // ####### 
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @720 'M' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0xe3, // ###   ##
    0x63, //  ##   ##
    0x55, //  # # # #
    0x55, //  # # # #
    0x55, //  # # # #
    0x49, //  #  #  #
    0x41, //  #     #
    0xe3, // ###   ##
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @736 'N' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x77, //  ### ###
    0x32, //   ##  # 
    0x32, //   ##  # 
    0x2a, //   # # # 
    0x2a, //   # # # 
    0x2a, //   # # # 
    0x26, //   #  ## 
    0x76, //  ### ## 
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @752 'O' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x1c, //    ###  
    0x22, //   #   # 
    0x41, //  #     #
    0x41, //  #     #
    0x41, //  #     #
    0x41, //  #     #
    0x22, //   #   # 
    0x1c, //    ###  
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @768 'P' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x7c, //  #####  
    0x22, //   #   # 
    0x22, //   #   # 
    0x22, //   #   # 
    0x3c, //   ####  
    0x20, //   #     
    0x20, //   #     
    0x78, //  ####   
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @784 'Q' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x1c, //    ###  
    0x22, //   #   # 
    0x41, //  #     #
    0x41, //  #     #
    0x41, //  #     #
    0x41, //  #     #
    0x22, //   #   # 
    0x1c, //    ###  
    0x1f, //    #####
    0x00, //         
    0x00, //         
    0x00, //         

    // @800 'R' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x7c, //  #####  
    0x22, //   #   # 
    0x22, //   #   # 
    0x22, //   #   # 
    0x3c, //   ####  
    0x24, //   #  #  
    0x22, //   #   # 
    0x71, //  ###   #
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @816 'S' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x3a, //   ### # 
    0x46, //  #   ## 
    0x40, //  #      
    0x3c, //   ####  
    0x02, //       # 
    0x02, //       # 
    0x62, //  ##   # 
    0x5c, //  # ###  
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @832 'T' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x7f, //  #######
    0x49, //  #  #  #
    0x49, //  #  #  #
    0x08, //     #   
    0x08, //     #   
    0x08, //     #   
    0x08, //     #   
    0x3e, //   ##### 
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @848 'U' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x77, //  ### ###
    0x22, //   #   # 
    0x22, //   #   # 
    0x22, //   #   # 
    0x22, //   #   # 
    0x22, //   #   # 
    0x22, //   #   # 
    0x1c, //    ###  
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @864 'V' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0xe7, // ###  ###
    0x42, //  #    # 
    0x24, //   #  #  
    0x24, //   #  #  
    0x24, //   #  #  
    0x18, //    ##   
    0x18, //    ##   
    0x18, //    ##   
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @880 'W' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0xe3, // ###   ##
    0x41, //  #     #
    0x49, //  #  #  #
    0x49, //  #  #  #
    0x55, //  # # # #
    0x55, //  # # # #
    0x55, //  # # # #
    0x22, //   #   # 
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @896 'X' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x77, //  ### ###
    0x22, //   #   # 
    0x14, //    # #  
    0x08, //     #   
    0x08, //     #   
    0x14, //    # #  
    0x22, //   #   # 
    0x77, //  ### ###
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @912 'Y' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x77, //  ### ###
    0x22, //   #   # 
    0x14, //    # #  
    0x14, //    # #  
    0x08, //     #   
    0x08, //     #   
    0x08, //     #   
    0x3e, //   ##### 
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @928 'Z' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x7e, //  ###### 
    0x42, //  #    # 
    0x04, //      #  
    0x08, //     #   
    0x10, //    #    
    0x22, //   #   # 
    0x42, //  #    # 
    0x7e, //  ###### 
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @944 '[' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x1c, //    ###  
    0x10, //    #    
    0x10, //    #    
    0x10, //    #    
    0x10, //    #    
    0x10, //    #    
    0x10, //    #    
    0x10, //    #    
    0x10, //    #    
    0x10, //    #    
    0x1c, //    ###  
    0x00, //         
    0x00, //         

    // @960 '\' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x40, //  #      
    0x40, //  #      
    0x20, //   #     
    0x20, //   #     
    0x10, //    #    
    0x10, //    #    
    0x08, //     #   
    0x08, //     #   
    0x04, //      #  
    0x04, //      #  
    0x02, //       # 
    0x02, //       # 
    0x00, //         
    0x00, //         

    // @976 ']' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x38, //   ###   
    0x08, //     #   
    0x08, //     #   
    0x08, //     #   
    0x08, //     #   
    0x08, //     #   
    0x08, //     #   
    0x08, //     #   
    0x08, //     #   
    0x08, //     #   
    0x38, //   ###   
    0x00, //         
    0x00, //         

    // @992 '^' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x10, //    #    
    0x10, //    #    
    0x28, //   # #   
    0x44, //  #   #  
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @1008 '_' (8 pixels wide)
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
    0x00, //         
    0x00, //         
    0x00, //         
    0xff, // ########

    // @1024 '`' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x10, //    #    
    0x08, //     #   
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

    // @1040 'a' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x3c, //   ####  
    0x02, //       # 
    0x3e, //   ##### 
    0x42, //  #    # 
    0x46, //  #   ## 
    0x3b, //   ### ##
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @1056 'b' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0xc0, // ##      
    0x40, //  #      
    0x40, //  #      
    0x5c, //  # ###  
    0x62, //  ##   # 
    0x42, //  #    # 
    0x42, //  #    # 
    0x62, //  ##   # 
    0xdc, // ## ###  
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @1072 'c' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x3a, //   ### # 
    0x46, //  #   ## 
    0x40, //  #      
    0x40, //  #      
    0x42, //  #    # 
    0x3c, //   ####  
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @1088 'd' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x06, //      ## 
    0x02, //       # 
    0x02, //       # 
    0x3a, //   ### # 
    0x46, //  #   ## 
    0x42, //  #    # 
    0x42, //  #    # 
    0x42, //  #    # 
    0x3f, //   ######
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @1104 'e' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x3c, //   ####  
    0x42, //  #    # 
    0x7e, //  ###### 
    0x40, //  #      
    0x40, //  #      
    0x3e, //   ##### 
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @1120 'f' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x0e, //     ### 
    0x10, //    #    
    0x10, //    #    
    0x7e, //  ###### 
    0x10, //    #    
    0x10, //    #    
    0x10, //    #    
    0x10, //    #    
    0x3c, //   ####  
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @1136 'g' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x3b, //   ### ##
    0x46, //  #   ## 
    0x42, //  #    # 
    0x42, //  #    # 
    0x46, //  #   ## 
    0x3a, //   ### # 
    0x02, //       # 
    0x02, //       # 
    0x3c, //   ####  
    0x00, //         

    // @1152 'h' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x60, //  ##     
    0x20, //   #     
    0x20, //   #     
    0x2c, //   # ##  
    0x32, //   ##  # 
    0x22, //   #   # 
    0x22, //   #   # 
    0x22, //   #   # 
    0x77, //  ### ###
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @1168 'i' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x08, //     #   
    0x00, //         
    0x38, //   ###   
    0x08, //     #   
    0x08, //     #   
    0x08, //     #   
    0x08, //     #   
    0x7f, //  #######
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @1184 'j' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x08, //     #   
    0x00, //         
    0x7c, //  #####  
    0x04, //      #  
    0x04, //      #  
    0x04, //      #  
    0x04, //      #  
    0x04, //      #  
    0x04, //      #  
    0x04, //      #  
    0x78, //  ####   
    0x00, //         

    // @1200 'k' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x60, //  ##     
    0x20, //   #     
    0x20, //   #     
    0x2e, //   # ### 
    0x28, //   # #   
    0x30, //   ##    
    0x28, //   # #   
    0x24, //   #  #  
    0x67, //  ##  ###
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @1216 'l' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x18, //    ##   
    0x08, //     #   
    0x08, //     #   
    0x08, //     #   
    0x08, //     #   
    0x08, //     #   
    0x08, //     #   
    0x08, //     #   
    0x7f, //  #######
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @1232 'm' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0xd2, // ## #  # 
    0x6d, //  ## ## #
    0x49, //  #  #  #
    0x49, //  #  #  #
    0x49, //  #  #  #
    0xed, // ### ## #
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @1248 'n' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x6c, //  ## ##  
    0x32, //   ##  # 
    0x22, //   #   # 
    0x22, //   #   # 
    0x22, //   #   # 
    0x77, //  ### ###
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @1264 'o' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x3c, //   ####  
    0x42, //  #    # 
    0x42, //  #    # 
    0x42, //  #    # 
    0x42, //  #    # 
    0x3c, //   ####  
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @1280 'p' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x6c, //  ## ##  
    0x32, //   ##  # 
    0x22, //   #   # 
    0x22, //   #   # 
    0x22, //   #   # 
    0x3c, //   ####  
    0x20, //   #     
    0x20, //   #     
    0x70, //  ###    
    0x00, //         

    // @1296 'q' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x3b, //   ### ##
    0x46, //  #   ## 
    0x42, //  #    # 
    0x42, //  #    # 
    0x46, //  #   ## 
    0x3a, //   ### # 
    0x02, //       # 
    0x02, //       # 
    0x07, //      ###
    0x00, //         

    // @1312 'r' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x6e, //  ## ### 
    0x30, //   ##    
    0x20, //   #     
    0x20, //   #     
    0x20, //   #     
    0x7c, //  #####  
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @1328 's' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x3e, //   ##### 
    0x42, //  #    # 
    0x3c, //   ####  
    0x02, //       # 
    0x42, //  #    # 
    0x7c, //  #####  
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @1344 't' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x20, //   #     
    0x20, //   #     
    0x7e, //  ###### 
    0x20, //   #     
    0x20, //   #     
    0x20, //   #     
    0x22, //   #   # 
    0x1c, //    ###  
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @1360 'u' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x66, //  ##  ## 
    0x22, //   #   # 
    0x22, //   #   # 
    0x22, //   #   # 
    0x26, //   #  ## 
    0x1b, //    ## ##
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @1376 'v' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0xe7, // ###  ###
    0x42, //  #    # 
    0x24, //   #  #  
    0x24, //   #  #  
    0x18, //    ##   
    0x18, //    ##   
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @1392 'w' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0xe3, // ###   ##
    0x41, //  #     #
    0x49, //  #  #  #
    0x55, //  # # # #
    0x55, //  # # # #
    0x22, //   #   # 
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @1408 'x' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x66, //  ##  ## 
    0x24, //   #  #  
    0x18, //    ##   
    0x18, //    ##   
    0x24, //   #  #  
    0x66, //  ##  ## 
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @1424 'y' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x77, //  ### ###
    0x22, //   #   # 
    0x22, //   #   # 
    0x14, //    # #  
    0x14, //    # #  
    0x08, //     #   
    0x08, //     #   
    0x10, //    #    
    0x38, //   ###   
    0x00, //         

    // @1440 'z' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         
    0x3e, //   ##### 
    0x24, //   #  #  
    0x08, //     #   
    0x10, //    #    
    0x22, //   #   # 
    0x3e, //   ##### 
    0x00, //         
    0x00, //         
    0x00, //         
    0x00, //         

    // @1456 '{' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x04, //      #  
    0x08, //     #   
    0x08, //     #   
    0x08, //     #   
    0x08, //     #   
    0x10, //    #    
    0x08, //     #   
    0x08, //     #   
    0x08, //     #   
    0x08, //     #   
    0x04, //      #  
    0x00, //         
    0x00, //         

    // @1472 '|' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x08, //     #   
    0x08, //     #   
    0x08, //     #   
    0x08, //     #   
    0x08, //     #   
    0x08, //     #   
    0x08, //     #   
    0x08, //     #   
    0x08, //     #   
    0x08, //     #   
    0x00, //         
    0x00, //         
    0x00, //         

    // @1488 '}' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x10, //    #    
    0x08, //     #   
    0x08, //     #   
    0x08, //     #   
    0x08, //     #   
    0x04, //      #  
    0x08, //     #   
    0x08, //     #   
    0x08, //     #   
    0x08, //     #   
    0x10, //    #    
    0x00, //         
    0x00, //         

};

sFONT Courier_New16 = {
    Courier_New16_Table,
    8, /* Width */
    16, /* Height*/
};
