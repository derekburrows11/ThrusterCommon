

// Instead of serial.println(F("Some Text));
#define fprintln(x) println(F(x))
#define fprint(x)   print  (F(x))

#define pmprintln(x) println((__FlashStringHelper*)pgm_get_far_address(x))
#define pmprint(x)   print  ((__FlashStringHelper*)pgm_get_far_address(x))

// F(x) defined in ??? file
// PROGMEM is in #include <avr/pgmspace.h>

