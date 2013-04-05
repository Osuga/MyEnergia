//        VCC        **
//        RES        PA_7
//        DC          PA_6
//        GND
//        CS          PA_3
//        CLK        PA_2
//        MISO      PA_4
//        MOSI      PA_5
//
//        Beep      PB_4
//
#include    "OLED_TEST.h"
#include    "COLOR_DEF.h"

const int analogPin = A0;


OLED myOLED;

static    int    proc_mode;
static    int    font_size;



void    Beep( void )
{
    tone( PB_4, 4000);
    delay( 50 );
    noTone( PB_4 );
    delay( 50 );
}

void    setup()
{
    delay( 500 );
    Serial.begin(9600);

    pinMode( GREEN_LED,OUTPUT );
    pinMode( RED_LED,OUTPUT );
    pinMode( BLUE_LED,OUTPUT );

    pinMode( PB_4,OUTPUT );
    pinMode( PUSH1,INPUT_PULLUP);
    pinMode( PUSH2,INPUT_PULLUP);

    attachInterrupt(PUSH1,trigger1,FALLING);
    attachInterrupt(PUSH2,trigger2,FALLING);

    Serial.println(" LM4F120XL OLED demo start.");

    myOLED.begin();

    myOLED.dspON();

    myOLED.drawLineTest();

    Beep();
    delay( 50 );
    Beep();
    delay( 100 );

    delay( 2000 );
    myOLED.clearScreen(COLOR_BLACK);
//    myOLED.dspSetFontSize( 1  );
    myOLED.dspSetFontSize( FONT_SEG7L );

    myOLED.dspSetCharColor(COLOR_BLUE);
//    myOLED.dspSetBackColor(COLOR_DGRAY);
}

void    trigger1(void)
{
    detachInterrupt(PUSH1);

   delay(5);        // delay 20ms
    if( digitalRead(PUSH1) == LOW ) {
        digitalWrite(BLUE_LED, !digitalRead(BLUE_LED));
        proc_mode |= 0x01;
    }
    attachInterrupt(PUSH1,trigger1,FALLING); 

}

void    trigger2(void)
{
    detachInterrupt(PUSH2);

   delay(5);        // delay 20ms
    if( digitalRead(PUSH2) == LOW ) {
        digitalWrite(RED_LED, !digitalRead(RED_LED));
        proc_mode |= 0x02;
    }
    attachInterrupt(PUSH2,trigger2,FALLING); 

}

double    aveTemp;
int        value;
int        msCount;


uint8_t        msg[10] = { "888888" };

void    loop()
{
    String    msgTemp;

    msCount++;
    value += analogRead( analogPin );
    value /= 2;

    if( msCount > 20000 ) {
        aveTemp = (double)((value*0.8 -600)/10);

        Serial.println( aveTemp,1);
        if( aveTemp >= 100.0 ) {
            msgTemp = String((int)(aveTemp),DEC);        // 888
            msgTemp.toCharArray((char *)msg,10);
        } else if(aveTemp >= 10.0 ) {
            msgTemp = String(" ");
            msgTemp += String((int)(aveTemp),DEC);        // 888
            msgTemp.toCharArray((char *)msg,10);
        }else if(aveTemp >= 0.0 ) {
            msgTemp = String("  ");
            msgTemp += String((int)(aveTemp),DEC);        // 888
            msgTemp.toCharArray((char *)msg,10);
        }else if(aveTemp < 0.0 ) {
            msgTemp = String((int)(aveTemp),DEC);        // 888
            msgTemp.toCharArray((char *)msg,10);
        }
//        msg[1] |= 0x80;
        msg[4] = 0x00;
//        msgTemp += String(".");
//        msgTemp += String(((int)(aveTemp*10)%10),DEC);

        myOLED.dspLocate(0,0);
        myOLED.dspPutChar( msg );

//        myOLED.dspPutChar( ' ' );

        msCount = 0;
 //        digitalWrite(RED_LED, !digitalRead(RED_LED));

//        myOLED.dspDrawTest1();
    }
    if( proc_mode &0x01 ) {
        proc_mode&=~0x01;
        if( ++font_size >FONT_SEG7L ) font_size=0;
        myOLED.dspSetFontSize( font_size );
        Beep();
        myOLED.clearScreen(0x0000);
    }
    if( proc_mode &0x02 ) {
        proc_mode&=~0x02;
        Beep();
        delay(50);
        Beep();
    }
 //   myOLED.drawLineTest();
 //   delay(500);
//    myOLED.dspDrawTest1();
//    delay(1000);
//   myOLED.clearScreen(0x0000);
//    delay(100);
 //   Beep();
//    delay(2000);
//    myOLED.dspON();
//    delay(2000);
}
