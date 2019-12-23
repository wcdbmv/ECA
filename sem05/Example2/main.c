#include <LPC23xx.H>

#define STB 26 //Port1.26
#define CLK 27 //Port1.27
#define DIO	28 //Port1.28


void delay(unsigned int count) {
	unsigned int i;
	for (i=0;i<count;i++){}
}

void tm1638_sendbyte(unsigned int x) {
		unsigned int i;
		IODIR1 |= (1<<DIO);//Устанавливаем пин DIO на вывод
		for(i = 0; i < 8; i++) {
			IOCLR1=(1<<CLK);//Сигнал CLK устанавливаем в 0
			delay(0xfff);//Задержка 
			if (x&1) 	{IOSET1=(1<<DIO);} //Устанавливаем значение на выходе DIO
			else 			{IOCLR1=(1<<DIO);}
			delay(0xfff);//Задержка
      x  >>= 1;
      IOSET1=(1<<CLK);//Сигнал CLK устанавливаем в 1
      delay(0x1fff);			
    }

}

unsigned int tm1638_receivebyte() {
		unsigned int i;
		unsigned int x=0;
		IODIR1 &= ~(1<<DIO);//Устанавливаем пин DIO на ввод
		for(i = 0; i < 32; i++) {
			IOCLR1=(1<<CLK);//Сигнал CLK устанавливаем в 0
			delay(0xfff);//Задержка 
			if (IOPIN1&(1<<DIO)) {
				x |= (1<<i);
			}
			delay(0xfff);//Задержка
      IOSET1=(1<<CLK);//Сигнал CLK устанавливаем в 1
      delay(0x1fff);			
    }
	return x;
}

void tm1638_sendcmd(unsigned int x) {
			//Устанавливаем пассивный высокий уровень сигнала STB
			IOSET1=(1<<STB);
			//Устанавливаем пины CLK,DIO,STB на вывод
			IODIR1 = (1<<CLK)|(1<<DIO)|(1<<STB);
			//Устанавливаем активный низкий уровень сигнала STB
			IOCLR1=(1<<STB);
			tm1638_sendbyte(x);
}


void tm1638_setaddr(unsigned int adr) {
	  //Установить адрес регистра LED инидикации
		tm1638_sendcmd(0xC0|adr);	
}

void tm1638_init() {
		unsigned int i;
		//Разрешить работу индикации
		tm1638_sendcmd(0x88);	
		//Установить режим адресации: автоинкремент
		tm1638_sendcmd(0x40);
   	//Установить адрес регистра LED инидикации
		tm1638_setaddr(0);
		//Сбросить все 
		for (i=0;i<=0xf;i++)
			tm1638_sendbyte(0);
		//Установить режим адресации: фиксированный
		tm1638_sendcmd(0x44);
}

void setbyte(unsigned int addr, unsigned int byte) {
	tm1638_setaddr(addr);
	tm1638_sendbyte(byte);
}

#define DYNAMIC 1
#define STRONG  3
#define WEAK    5

int main (void) {
	unsigned n, i;
	tm1638_init();

	n = 1;
	for (;;) {	
		tm1638_sendcmd(0x46);
		i = tm1638_receivebyte();
		
		if (i == 0) {
			setbyte(DYNAMIC, 1);
			if (n++ % 3) {
				setbyte(STRONG, 0);
				setbyte(WEAK, 1);
			} else {
				setbyte(WEAK, 0);
				setbyte(STRONG, 1);
			}
		}
		else {
			setbyte(DYNAMIC, 0);
			setbyte(STRONG, 0);
			setbyte(WEAK, 0);
		}

		delay(0xfff);
	}
}

