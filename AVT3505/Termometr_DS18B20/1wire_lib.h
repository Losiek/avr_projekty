#include <avr/io.h>
#include <inttypes.h>

#define pin_1w      PD5
#define dir_1w      DDRD
#define out_1w      PORTD
#define in_1w       PIND


#define write_1w	dir_1w |= _BV(pin_1w)  
#define read_1w		dir_1w &= ~_BV(pin_1w) 

#define write_one	out_1w |= _BV(pin_1w)  
#define write_zero	out_1w &= ~_BV(pin_1w) 

#define set_1w      bit_is_set(in_1w,pin_1w)
#define clear_1w    bit_is_clear(in_1w,pin_1w)
