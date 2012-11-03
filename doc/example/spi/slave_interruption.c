#include <avr/io.h>
#include <avr/interrupt.h>
#include <spi.h>

// ...

//Ici exemple avec un atmega8535.
//Le spi et sur le port B.
#define SPI_DD		DDRB
//Le bit MISO est sur le pin 6 du port B.
#define SPI_MISO	PB6

// ...

ISR(SPI_STC_vect)
{	
	//Lecture de la valeur qui vient d'être reçus (MOSI) est sauvegarde de la valeur lu dans twiRead.
	uint8_t twiRead = twi_read_data();
	
	// ...
	
	//Préparation pour écrire 0x54 sur le bus (MISO).
	twi_write_data(0x54);
	
	// ...
}

// ...

int main()
{
	// ...

	//Configuration des sorties.
	SPI_DD = _BV(SPI_MISO);
	//Configuration du bus spi.
	spi_configure(	SPI_MODE_SLAVE	 	|	//Mode esclave
					SPI_DATA_ORDER_MSB 	|	//Bit de pois fort en premier.
					SPI_CLOCK_MODE_0;		//Communique avec le mode 0.
	//Activation du spi.
	spi_enable();
	//Active les interruptions spi
	spi_enable_interrupt();
	
	//Active les interruption globale.
	sei();
	
	// ...

	while(1)
	{
		// ...
	}
}
