#include <LPC23xx.H>

#define BUTTON  (1 << 29)
#define DYNAMIC (1 << 28)
#define STRONG  (1 << 27)
#define WEAK    (1 << 26)

void delay(void) {
	unsigned i;
	for (i = 0; i < 0x0ffff; ++i)
		;
}

int main (void) {
	unsigned n = 1;

	PINSEL3 = 0x00000000;
	IODIR1  = DYNAMIC | STRONG | WEAK;
	IOSET1  = 0x00000000;

	for (;;) {
		IOCLR1 = DYNAMIC | STRONG | WEAK;
		if (IOPIN1 & BUTTON)
			IOSET1 = DYNAMIC | (n++ % 3 ? WEAK : STRONG);
		delay();
	}
}

