#include <avr/io.h>
#include <spi.h>

// ...

//Ici exemple avec un at90usb.
//Le spi et sur le port B.
#define SPI_DD		DDRB
//Le bit SCK est sur le pin 1 du port B.
#define SPI_SCK		PB1
//Le bit MOSI est sur le pin 2 du port B.
#define SPI_MOSI	PB2

// ...

int main()
{
	// ...

	//Configuration des sorties.
	SPI_DD = _BV(SPI_SCK)|_BV(SPI_MOSI);
	//Configuration du bus spi.
	spi_configure(	SPI_MODE_MASTER 	|	//Mode maître
					SPI_DATA_ORDER_MSB 	|	//Bit de pois fort en premier.
					SPI_CLOCK_MODE_0	|	//Communique avec le mode 0.
					SPI_CLOCK_CLK_16);		//Horloge avec un diviseur de 16, soit une horloge spi à 1Mhz pour une horloge cpu à 16Mhz
	//Activation du spi.
	spi_enable();
	
	//Préparation pour écrire 0x33 sur le bus (MOSI).
	twi_write_data(0x33);
	//Écriture de 0x33 sur le bus. (La lecture (MISO) est ici ignorer)
	twi_read_data();
	//Attend la fin de la transaction.
	while(!spi_is_raising_transfer_flag())
	//lecture du bus (MISO) est sauvegarde de la valeur lu dans twiRead.
	uint8_t twiRead = twi_read_data();
	
	// ...

	while(1)
	{
		// ...
	}
}
