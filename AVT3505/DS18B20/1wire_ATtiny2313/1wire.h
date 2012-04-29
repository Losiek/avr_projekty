//#ifndef 1WIRE_H_INCLUDE
//#define 1WIRE_H_INCLUDE

#define xwirePORT PORTD //Port interfejsu 1-Wire.
#define xwireDDR DDRD   //DDRD interfejsu 1-Wire.
#define xwirePIN PIND   //PIN interfejsu 1-Wire.
#define wire 4          //Bit Portu interfejsu 1-Wire.

#define wire_out0 xwireDDR |= 1<<wire;    //Zaspis stanu 0 na lini 1-Wire.
#define wire_out1 xwireDDR &= ~(1<<wire); //Zaspis stanu 1 na lini 1-Wire.

//#endif //1WIRE_H_INCLUDE