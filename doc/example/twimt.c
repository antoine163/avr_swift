#include <avr/io.h>
#include <sw/drv/twi.h>

//Une adresse esclave.
#define TWI_ADD 0x32

//Une fonction pour les erreur provenant du bus TWI.
void TWI_ERREUR()
{
	//...
}

int main(void)
{
	//...
	
	//Un buffer pour envoyer les données.
	uint8_t buf[2] = {0xf8, 0x64};
	
	//Sélectionne la fréquence du bus à 400KHz (cpu à 16MHz).
	twi_bit_rate_generator(12, TWI_PRESCALER_1);
	//Active le twi.
	twi_enable();
	
	//...
	
	twi_generate_start();			// Pour générai la condition de start.
	while(!twi_is_raising_falg());		// Attend que le drapeau ce lève.
	if(twi_get_statu() != TW_START)  	// Si le statu n'est pas TW_START, s'est qu'il ses produit une erreur.
		TWI_ERREUR();

	//Envoi l'adresse.
	twi_send_byte((TWI_ADD<<1)|TW_WRITE);	// Envoi du premier octet, qui contient l'adresse et le mode écriture.
	while(!twi_is_raising_falg());		// Attend que le drapeau ce lève.
	if(twi_get_statu() != TW_MT_SLA_ACK)  	// Si le statu n'est pas TW_MT_SLA_ACK, s'est qu'il ses produit une erreur.
		TWI_ERREUR();

	//Envoi les données.
	twi_send_byte(buf[0]);			// Envoi d'une données.
	while(!twi_is_raising_falg());		// Attend que le drapeau ce lève.
	if(twi_get_statu() != TW_MT_DATA_ACK)  // Si le statu n'est pas TW_MT_DATA_ACK, s'est qu'il ses produit une erreur.
		TWI_ERREUR();

	twi_send_byte(buf[1]);			// Envoi d'une données.
	while(!twi_is_raising_falg());		// Attend que le drapeau ce lève.
	if(twi_get_statu() != TW_MT_DATA_ACK)  // Si le statu n'est pas TW_MT_DATA_ACK, s'est qu'il ses produit une erreur.
		TWI_ERREUR();

	twi_generate_stop();			// Pour générai la condition de stop.
		
	//...
	
	while(1)
	{
		//...
	}
   	 
	return 0;

}
