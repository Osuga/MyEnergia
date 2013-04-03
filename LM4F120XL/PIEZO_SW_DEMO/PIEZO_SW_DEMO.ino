/*
  PIEZO_SW_DEMO - piezo sounder & push sw interrupt demo program.
  Copyright (c) 2013 Satoshi Osuga.  All right reserved.
*/
/*
  This DEMO code is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This DEMO code is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
//    PIEZO SOUNDER    PB_4
//
//    RED_LED        PF1
//    GREEN_LED    PF_3
//    BLUE_LED       PF_2
//
//    PUSH1            PF_4
//    PUSH2            PF_0


static int    proc_state=0;

void    setup( void )
{
    delay( 500 );
    Serial.begin(9600);

    pinMode(RED_LED,OUTPUT);
    pinMode(GREEN_LED,OUTPUT);
    pinMode(BLUE_LED,OUTPUT);
    pinMode(PB_4,OUTPUT);        // for Beep

    pinMode( PUSH1, INPUT_PULLUP);
    pinMode( PUSH2, INPUT_PULLUP);
    attachInterrupt(PUSH1,sw1_proc,FALLING);
    attachInterrupt(PUSH2,sw2_proc,FALLING);

    Beep();

    analogWrite(GREEN_LED,20);

}

void    sw1_proc( void )
{
    detachInterrupt(PUSH1);

   delay(5);        // delay 20ms
    if( digitalRead(PUSH1) == LOW ) {
        digitalWrite(BLUE_LED, !digitalRead(BLUE_LED));
        proc_state |= 0x01;
    }
    attachInterrupt(PUSH1,sw1_proc,FALLING); 

}

void    sw2_proc( void )
{
    detachInterrupt(PUSH2);

   delay(5);        // delay 20ms
    if( digitalRead(PUSH2) == LOW ) {
        digitalWrite(RED_LED, !digitalRead(RED_LED));
        proc_state |= 0x02;
    }
    attachInterrupt(PUSH2,sw2_proc,FALLING); 
}

void    Beep(void)
{
    tone( PB_4, 4000);
    delay( 50 );
    noTone( PB_4 );
    delay( 20 );
}
void    BeepH(void)
{
    tone( PB_4, 4300);
    delay( 40 );
    noTone( PB_4 );
    delay( 20 );
}


void    loop( void )
{

    if( proc_state &0x01 ) {
        proc_state &= ~0x01;
        BeepH();
        Serial.println("push 1");
    }
    if( proc_state &0x02 ) {
        proc_state &= ~0x02;
        Beep();
        Serial.println("push 2");
    }

}

