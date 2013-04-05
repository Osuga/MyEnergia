#include    <Energia.h>

#include    "OLED_TEST.h"

#include    "COLOR_DEF.h"

#include    "font6x7.h"
#include    "font10x12.h"
#include    "font_inv12.h"

#define    __HAS_7SEG__
#ifdef        __HAS_7SEG__
#include    "SEVEN_SEG.h"
#endif        //__HAS_7SEG__

uint16_t    OLED::__CharColor = 0xFFFF;
uint16_t    OLED::__ForeColor = 0xFFF0;
uint16_t    OLED::__BackColor = 0x0000;


uint8_t    OLED::__pos_x = 0;
uint8_t    OLED::__pos_y = 0;
uint8_t    OLED::__x_witch = 5;
uint8_t    OLED::__y_witch = 8;

uint8_t    OLED::__font_size = FONT_SMALL;

uint8_t    OLED::buff[12] = { 0 };


OLED::OLED()
{
}

void    OLED::begin( void )
{
    InitOLED();
}


void    OLED::InitOLED(void)
{

    pinMode( pinRES,OUTPUT);
    pinMode( pinDC,OUTPUT);

    pinMode( pinCLK,OUTPUT);
    pinMode( pinMOSI,OUTPUT);
    pinMode( pinMISO,INPUT);
    pinMode( pinCS,OUTPUT);




    digitalWrite(pinRES, HIGH );
    digitalWrite(pinDC, LOW );
    digitalWrite(pinCLK, HIGH );
    digitalWrite(pinMOSI, LOW );
    delay( 2 );

    //
    //

    digitalWrite( pinRES, LOW );
    delay( 10 );
    digitalWrite( pinRES, HIGH );
    delay( 2 );

    for( int i=0; i<DSP_TABLE_NUM;i++) {
        send_cmd( DSP_TABLE[i] );
    }

    clearScreen( 0x0000 );
}


void    OLED::dspDelay( long count )
{
    while( (count--)>0 ) {
        asm("nop");
    }
}
void    OLED::dspON()
{
    send_cmd( 0xAF );
}

void    OLED:: dspOFF()
{
    send_cmd( 0xAE );
}

void    OLED::send_cmd( uint8_t data )
{
    digitalWrite( pinDC, LOW );
    digitalWrite( pinCS, LOW );
    dspDelay( 20 );

    spi_transfer( data );
    digitalWrite( pinCS, HIGH );
    digitalWrite( pinDC, LOW );

    dspDelay( 20 );

}

void    OLED::send_data( uint8_t data )
{
     digitalWrite( pinDC, HIGH );
    digitalWrite( pinCS, LOW );
    dspDelay( 20 );

    spi_transfer( data );
    digitalWrite( pinCS, HIGH );
    digitalWrite( pinDC, LOW );

    dspDelay( 20 );

}

void    OLED::send_multi( uint8_t *data , uint8_t size)
{
    digitalWrite( pinDC, LOW );
    digitalWrite( pinCS, LOW );
    dspDelay( 20 );

    for(int i=0; i<size; i++)
        spi_transfer( *(data++) );
    digitalWrite( pinCS, HIGH );
    digitalWrite( pinDC, LOW );
    dspDelay( 20 );


}
void    OLED::spi_transfer( uint8_t data )
{
    for( int i=0; i<8; i++) {
        if(data & 0x80 ) digitalWrite(pinMOSI,HIGH );
        else             digitalWrite(pinMOSI,LOW );
        digitalWrite(pinCLK, LOW );
        dspDelay( 10 );
        digitalWrite(pinCLK, HIGH );
        dspDelay( 10 );
        data = data << 1;
    }
}


void    OLED::drawDot(uint8_t x, uint8_t y, uint16_t color)
{
    drawLine(x,y,x,y,color);
}

void    OLED::drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint16_t color)
{
//    if  ( x1 > DSP_MAX_X ) x1 = DSP_MAX_X;
//    if  ( y1 > DSP_MAX_Y ) y1 = DSP_MAX_Y;
//    if  ( x2 > DSP_MAX_X ) x2 = DSP_MAX_X;
//    if  ( y2 > DSP_MAX_Y ) y2 = DSP_MAX_Y;

    buff[0] = 0x21;
    buff[1] = x1;
    buff[2] = y1;
    buff[3] = x2;
    buff[4] = y2;
    buff[5] = (uint8_t)(((color>>11)&0x1f)<<1);   // Blue
    buff[6] = (uint8_t) ((color>>5 )&0x3f);   // Green
    buff[7] = (uint8_t)(( color     &0x1f)<<1);   // Red

    send_multi( &buff[0],8);
}
void    OLED::drawRect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint16_t color)
{
 //       if  ( x1 > DSP_MAX_X ) x1 = DSP_MAX_X;
 //       if  ( y1 > DSP_MAX_Y ) y1 = DSP_MAX_Y;
 //       if  ( x2 > DSP_MAX_X ) x2 = DSP_MAX_X;
 //       if  (  y2 > DSP_MAX_Y ) y2 = DSP_MAX_Y;
    
    
        buff[0] = 0x22;      // 
        buff[1] = x1; 
        buff[2] = y1; 
        buff[3] = x2; 
        buff[4] = y2; 
        buff[5] = (uint8_t)(((color>>11)&0x1f)<<1);   // Blue
        buff[6] = (uint8_t) ((color>>5 )&0x3f);   // Green
        buff[7] = (uint8_t)(( color     &0x1f)<<1);   // Red
        buff[8] = buff[5];    //Blue
        buff[9] = buff[6];   // Green
        buff[10]= buff[7];   // Red
        
        send_multi(buff, 11);
}
void    OLED::clearScreen( uint16_t color )
{
    drawRect( 0,0, DSP_MAX_X,DSP_MAX_Y,color);
}

//void    OLED::drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint16_t color)
void    OLED::drawLineTest( void )
{
    int    _r;
    int    _g;
    int    _b;
    int    color;

    for( int i=0; i<=DSP_MAX_Y/2;i++) {
        _b =(i&0x1F)<<11 ;
        _g =(i&0x1F)<<6;
        _r =(i&0x1F);
        color = _b+_g+_r;
        drawLine(0,DSP_MAX_Y-i,DSP_MAX_X,DSP_MAX_Y-i,color);
        drawLine(0,i,DSP_MAX_X,i,color);
        delay(16);
    }
/*
    for( int i=DSP_MAX_Y/2; i>0;i--) {
        _b =(i&0x1F)<<11 ;
        _g =(i&0x1F)<<6;
        _r =(i&0x1F);
        color = _b+_g+_r;
        drawLine(0,DSP_MAX_Y-i,DSP_MAX_X,DSP_MAX_Y-i,__ForeColor);
        drawLine(0,i,DSP_MAX_X,i,__ForeColor);
        delay(16);
    }
*/

}
void  OLED::dspPutChar( uint8_t *msg )
{
    while( *msg !=0x00 ) {
        dspPutChar( *(msg++) );
    }
}
void  OLED::dspPutChar( uint8_t a )
{
    if( (__pos_x+__x_witch)> DSP_MAX_X  ) {
        __pos_x=0;
        __pos_y += (__y_witch+1);
    }
    if( (__pos_y)> DSP_MAX_Y  ) {
        __pos_x = 0;
        __pos_y = 0;
    }
    dspPutChar(__pos_x, __pos_y, a );
    __pos_x += (__x_witch);

}
void	OLED::dspPutChar(uint8_t x,uint8_t y,uint8_t a)
{
	int    i,j;
	int    temp;
	

	if(a < 32)a=32;
        if(a > 128 ) a=128;

#ifdef    __HAS_7SEG__
    if( __font_size == FONT_SEG7M )  {
        dspPutChar_24x32s( x, y, a);
        return;
    } else if( __font_size == FONT_SEG7L )  {
        dspPutChar_32x48s( x, y, a);
        return;
    }
#endif //__HAS_7SEG__

    drawRect(x,y,x+__x_witch,y+__y_witch,__BackColor);
    if( __font_size == FONT_SMALL )  {
	for(i=0; i< __x_witch ; i++) {	
            temp = font6x7[a-32][i];
            for(j=__y_witch; j>=0; j--) {
                if(temp & 0x80) drawDot(x+i,y+j,__CharColor);
                temp <<= 1;
           }
        }
    } else if( __font_size == FONT_LARGE )  {
	for(i=0; i< __x_witch ; i++) {	
            temp = font10x12[a-32][i];
            for(j=__y_witch+1; j>0; j--) {
//                if(temp & 0x8000) drawDot(x+i,y+j,__CharColor);        //16bit Hight
                if(temp & 0x1000) drawDot(x+i,y+j,__CharColor);        //12bit Hight
                temp <<= 1;
           }
        }
    } else if( __font_size == FONT_INV )  {
	for(i=0; i< __x_witch ; i++) {	
            temp = font_inv12[a-32][i];
            for(j=__y_witch; j>0; j--) {
                if(temp & 0x8000) drawDot(x+i,y+j,__CharColor);
                temp <<= 1;
           }
        }
    }else {
    }
}

void	OLED::dspLocate(uint8_t x, uint8_t y)
{
        __pos_x = x;
        __pos_y = y;
}

void	OLED::dspDrawTest1( void )
{
    uint8_t ch=' ';
    uint8_t y=0;
    uint8_t i;


//     for(y=0;y<DSP_MAX_Y;y+=(__y_witch+1)) {
     for(y=0;y<4;y++) {
        for(i=0;i<9; i++ ,ch++) {
            dspPutChar( ch );
         }
    }
}

uint16_t    OLED::dspGetColor( uint8_t num )
{

    return(colorTable[num%12]);
}

void        OLED::dspSetFontSize( uint8_t num )
{
        switch( num ) {
        case FONT_SMALL:
             __x_witch = 6;  __y_witch = 8;
             __font_size = FONT_SMALL;
            break;
        case FONT_LARGE:
             __x_witch = 10;  __y_witch = 12;
             __font_size = FONT_LARGE;
            break;
        case FONT_INV:
             __x_witch = 12;  __y_witch = 16;
             __font_size = FONT_INV;
            break;
        case FONT_SEG7M:
             __x_witch = 24;  __y_witch = 32;
             __font_size = FONT_SEG7M;
            break;
        case FONT_SEG7L:
             __x_witch = (32-2);  __y_witch = 48;
             __font_size = FONT_SEG7L;
            break;
        default :
           __x_witch = 6;  __y_witch = 8;
             __font_size = FONT_SMALL;
            break;

    }
}


void    OLED::dspPutChar_24x32s(uint8_t x,uint8_t y,uint8_t dat)
{
	uint8_t seg,seg1,idx;	
	uint8_t  i,j;
	uint8_t  temp[4];
	uint8_t  temp1[4];
	uint8_t  dot_flag = 0x00;

#ifdef    __HAS_7SEG__	
	if( dat&0x80)	dot_flag = 0x80;

        if( dat < 32 )  {
            dat = 32;
        } else{
            dat -= 32;
        }
//	X_Witch = 24;
//	Y_Witch = 32;

	dat=(0x7F&dat);
	
	//DrawRect(x,y,x+X_Witch,y+Y_Witch,__BackColor);
	
	
	for(i=0; i<__x_witch; i++) {
		for( idx=0; idx<4; idx++) temp[idx] = 0;
		seg  = seg7bit_table[dat];
		seg |= dot_flag;
		seg1 = seg7bit_table[0x5F];
		if( (seg | seg1) == 0)		continue;
		for( idx=0; idx<8; idx++) {
			if( seg & 0x01) {
				temp[0] |= (uint8_t)(seg7Font_2432a[idx][i]);
				temp[1] |= (uint8_t)(seg7Font_2432a[idx][i] >> 8);
				temp[2] |= (uint8_t)(seg7Font_2432b[idx][i]);
				temp[3] |= (uint8_t)(seg7Font_2432b[idx][i] >> 8);
			} else if( seg1 & 0x01) {
				temp1[0] |= (uint8_t)(seg7Font_2432a[idx][i]);
				temp1[1] |= (uint8_t)(seg7Font_2432a[idx][i] >> 8);
				temp1[2] |= (uint8_t)(seg7Font_2432b[idx][i]);
				temp1[3] |= (uint8_t)(seg7Font_2432b[idx][i] >> 8);
			}
			seg >>= 1;
			seg1 >>= 1;
		}
		for(j=8; j>0; j--) {
			if((temp[0] & 0x80))  {		drawDot(x+i,y+j,  __CharColor);    }
                        else if((temp1[0] & 0x80))  {	drawDot(x+i,y+j,  __BackColor);    }
			if((temp[1] & 0x80)) {		drawDot(x+i,y+j+8,__CharColor);  }
			else if((temp1[1] & 0x80)) {	drawDot(x+i,y+j+8,__BackColor);  }
			if((temp[2] & 0x80)) {		drawDot(x+i,y+j+16,__CharColor); }
                        else if((temp1[2] & 0x80)) {	drawDot(x+i,y+j+16,__BackColor); }
			if((temp[3] & 0x80)) {		drawDot(x+i,y+j+24,__CharColor); }
			else if((temp1[3] & 0x80)) {	drawDot(x+i,y+j+24,__BackColor); }

			for( idx=0; idx<4; idx++)  {
				temp[idx] <<= 1;
				temp1[idx] <<= 1;
			}
		}
	}
#endif    //__HAS_7SEG__
}

void	OLED::dspPutChar_32x48s(uint8_t x,uint8_t y,uint8_t dat)
{
	uint8_t seg,seg1,idx;	
	uint8_t i,j;
	uint8_t temp[6];
	uint8_t temp1[6];
	uint8_t dot_flag = 0x00;

#ifdef    __HAS_7SEG__
	if(dat&0x80)	dot_flag = 0x80;

        if( dat < 32 )  {
            dat = 32;
        } else{
            dat -= 32;
        }

	dat=(dat&0x7F);

//	X_Witch = 32-2;
//	Y_Witch = 48;

	
	//dspDrawRect(x,y,x+X_Witch,y+Y_Witch,__BackColor);
	
	
	for(i=0; i<__x_witch; i++) {
		for( idx=0; idx<6; idx++)  {
                            temp[idx] = 0;
                            temp1[idx] = 0;
                }
		seg = seg7bit_table[dat];
		seg |= dot_flag;
		seg1 = seg7bit_table[0x5F];
		if( (seg | seg1) == 0)		continue;
		for( idx=0; idx<8; idx++) {
			if( seg & 0x01) {
				temp[0] |= (uint8_t)(seg7Font_32x48a[idx][i]);
				temp[1] |= (uint8_t)(seg7Font_32x48a[idx][i] >> 8);
				temp[2] |= (uint8_t)(seg7Font_32x48b[idx][i]);
				temp[3] |= (uint8_t)(seg7Font_32x48b[idx][i] >> 8);
				temp[4] |= (uint8_t)(seg7Font_32x48c[idx][i]);
				temp[5] |= (uint8_t)(seg7Font_32x48c[idx][i] >> 8);
			}else  if( seg1 & 0x01) {
				temp1[0] |= (uint8_t)(seg7Font_32x48a[idx][i]);
				temp1[1] |= (uint8_t)(seg7Font_32x48a[idx][i] >> 8);
				temp1[2] |= (uint8_t)(seg7Font_32x48b[idx][i]);
				temp1[3] |= (uint8_t)(seg7Font_32x48b[idx][i] >> 8);
				temp1[4] |= (uint8_t)(seg7Font_32x48c[idx][i]);
				temp1[5] |= (uint8_t)(seg7Font_32x48c[idx][i] >> 8);
			}
			seg >>= 1;
			seg1 >>= 1;
		}
		for(j=8; j>0; j--) {
			if((temp[0] & 0x80)) { 		drawDot(x+i,y+j,   __CharColor); } 
			else if((temp1[0] & 0x80)) {	drawDot(x+i,y+j,   __BackColor); }
			if((temp[1] & 0x80)) {		drawDot(x+i,y+j+8, __CharColor); }
			else if((temp1[1] & 0x80)) {	drawDot(x+i,y+j+8,   __BackColor); }
			if((temp[2] & 0x80)) {		drawDot(x+i,y+j+16,__CharColor); }
			else if((temp1[2] & 0x80)) {	drawDot(x+i,y+j+16,   __BackColor); }
			if((temp[3] & 0x80)) {		drawDot(x+i,y+j+24,__CharColor); }
			else if((temp1[3] & 0x80)) {	drawDot(x+i,y+j+24,   __BackColor); }
			if((temp[4] & 0x80)) {		drawDot(x+i,y+j+32,__CharColor); }
			else if((temp1[4] & 0x80)) {	drawDot(x+i,y+j+32,   __BackColor); }
			if((temp[5] & 0x80)) {		drawDot(x+i,y+j+40,__CharColor); }
			else if((temp1[5] & 0x80)) {	drawDot(x+i,y+j+40,   __BackColor); }


			for( idx=0; idx<6; idx++) {
				temp[idx] <<= 1;
				temp1[idx] <<= 1;
			}
		}
	}
#endif    //__HAS_7SEG__
}

