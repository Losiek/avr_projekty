#ifndef MAKRA_H_INCLUDED
#define MAKRA_H_INCLUDED

//Makra uprzaszczaj¹ce dosêpt do portów
//*** Port
#define PORT(x) XPORT(x)
#define XPORT(x) (PORT##x)
//*** Pin
#define PIN(x) XPIN(x)
#define XPIN(x) (PIN##x)
//*** DDR
#define DDR(x) XDDR(x)
#define XDDR(x) (DDR##x)

//NOPek
#define NOP() {asm voltile(,,nop,,::);}

#endif //MAKRA_H_INCLUDED
