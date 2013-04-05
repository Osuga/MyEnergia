//
//   OLED display module ALO-095BWNN-J9
//   software SPI 
//   96x64 pixcel / SSD1332 or SSD1331
//    Copyright (c) s.osuga 2013
//
#ifndef __OLED_TEST_H_
#define __OLED_TEST_H_

#include <inttypes.h>

#include    "COLOR_DEF.h"
/*
#define    OLED_RES  PA_7
#define    OLED_DC    PA_6
#define    OLED_CS    PA_3
*/


#define    pinRES      PA_7
#define    pinDC        PA_6

#define    pinCLK      PA_2
#define    pinMOSI    PA_5
#define    pinMISO    PA_4
#define    pinCS        PA_3


#define  DSP_MAX_X    (96-1)
#define  DSP_MAX_Y    (64-1)
#define  DSP_TABLE_NUM 50


//
//
#define        FONT_SMALL    0
#define        FONT_LARGE    1
#define        FONT_INV        2
#define        FONT_SEG7S    3
#define        FONT_SEG7M    4
#define        FONT_SEG7L    5


class    OLED 
{
    //
private:
    static uint16_t    __CharColor;
    static uint16_t    __ForeColor;
    static uint16_t    __BackColor;
    static uint8_t    __pos_x;
    static uint8_t    __pos_y;
    static uint8_t    __x_witch;
    static uint8_t    __y_witch;
    static uint8_t    __font_size;

    static uint8_t   buff[12];
public:
    OLED();
    static void    begin( void );
    static void    InitOLED(void);

    static void    dspON();
    static void    dspOFF();

    static void    drawDot(uint8_t x, uint8_t y, uint16_t color);
    static void    drawLine(uint8_t x1, uint8_t x2, uint8_t y1, uint8_t y2, uint16_t color);
    static void    drawRect(uint8_t x1, uint8_t x2, uint8_t y1, uint8_t y2, uint16_t color);
    static void    clearScreen( uint16_t color );

    static void	dspLocate(uint8_t x, uint8_t y);
    static void	dspPutChar(uint8_t x,uint8_t y,uint8_t a);
    static void    dspPutChar( uint8_t a );
    static void    dspPutChar( uint8_t *msg );
    // 
    inline    void    dspSetForeColor( uint16_t color ) {    __ForeColor = color; }
    inline    void    dspSetBackColor( uint16_t color ) {    __BackColor = color; }
    inline    void    dspSetCharColor( uint16_t color ) {    __CharColor = color; }
    inline    uint16_t    dspGetForeColor( void ) {    return(__ForeColor); }
    inline    uint16_t    dspGetBackColor( void ) {    return(__BackColor); }
    inline    uint16_t    dspGetCharColor( void ) {    return(__CharColor); }
    static    uint16_t    dspGetColor( uint8_t num );
    static    void    dspSetFontSize( uint8_t num );


    static void    drawLineTest( void );
    static void	dspDrawTest1( void );

    static void    dspPutChar_24x32s(uint8_t x,uint8_t y,uint8_t dat);
    static void    dspPutChar_32x48s(uint8_t x,uint8_t y,uint8_t dat);

private:
    static void    dspDelay( long count );
    static void    send_cmd( uint8_t data );
    static void    send_data( uint8_t data );
    static void    send_multi( uint8_t *data , uint8_t size);
    static void    spi_transfer( uint8_t data );
    
};

const uint8_t DSP_TABLE[DSP_TABLE_NUM] = {
	0xAE,			//display OFF
	0x15,			//Set Column address
	0x00, 0x5f,		//  start = 0	,end   = 95
	0x75,			//Set Row address
	0x00, 0x3f,		//  start = 0	,end   = 63
	0xA0,0x70,		//Set remap & data format  00xx xxxx:256color/ 01xx xxxx:65k color [70 or 74]
	0xA1, 0x00,		//Set display strat line
	0xA2, 0x00,		//Set display offset
	0xA4,			//Set display mode 0xA4:normal,0xA5:,0xA6:,0xA7:Inverse		
	0xA8, 0x3f,		//Set Multiplex Ratio 	0-63,16MUX-64MUX
	0xAD,0x8f,		//Set Master Configuration External VCC Supply selected
	0xB0, 0x12,		//Set Power saving mode  [00 or 12 or 1a]
	0xB1, 0x74,		//Set Phase 1&2 Period adjustment 
	0xB3, 0xd0,		//Set Display Clock Divide Ratio / Oscillator Frequency
	0x8A, 0x81,		//Set Second pre-charge of color A
	0x8B, 0x82,		//Set Second pre-charge of color B
	0x8C, 0x83,		//Set Second pre-charge of color C
	0xBB, 0x80,		//Set Pre-charge Level
	0xBC, 0x80,		//Set Pre-charge Level
	0xBD, 0x80,		//Set Pre-charge Level
	0xBE, 0x3F,		// Set VCOMH
	0x81, 0x59,		//Set Contrast Control for Color A  [0x5A:100cd/m2	0x59:80cd/m2]
	0x82, 0x59,		//Set Contrast Control for Color B  [0x2C:100cd/m2	0x2E:80cd/m2]
	0x83, 0x5A,		//Set Contrast Control for Color C  [0x54:100cd/m2	0x5A:80cd/m2]
	0x87, 0x0F,		//Set Master Current control		//.0x03:
	0x26, 0x01,		//Set Enable Fill Rectangle 
	0xe3,
	0xe3                        // display ON [0xAF] or display OFF [0xAE]
};


// SCK, MOSI, MISO, CS
const    uint8_t pinSPI[][4] = {
    {PA_2,PA_5,PA_4,PA_3},        //set 0
    {PF_2,PF_1,PF_0,PF_3},        //set 1
    {PB_4,PB_7,PB_6,PB_5},        //set 2
    {PD_0,PD_3,PD_2,PD_1}        //set 3
};

const    uint16_t colorTable[] = {
    COLOR_BLUE,
    COLOR_GREEN,
    COLOR_RED,
    COLOR_DGREEN,
    COLOR_BLACK,
    COLOR_ORANGE,
    COLOR_WHITE,
    COLOR_DGRAY,
    COLOR_GRAY,
    COLOR_PINK,
    COLOR_CYAN,
    COLOR_YELLOW
};

#endif    //__OLED_TEST_H_

