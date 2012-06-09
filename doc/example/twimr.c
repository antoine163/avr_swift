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
	
	//Un buffer pour récupérer les données.
	uint8_t buf[2];
	
	//Sélectionne la fréquence du bus à 400KHz (cpu à 16MHz).
	twi_bit_rate_generator(12, TWI_PRESCALER_1);
	//Active le twi.
	twi_enable();
	
	//...
	
	twi_generate_start();			// Pour générai la condition de start.
	while(!twi_is_raising_falg());		// Attend que le drapeau ce lève.
	if(twi_get_statu() != TW_START)		// Si le statu n'est pas TW_START, s'est qu'il ses produit une erreur.
		TWI_ERREUR();

	//Envoi l'adresse.
	twi_send_byte((TWI_ADD<<1)|TW_READ);	// Envoi du premier octet, qui contient l'adresse et le mode lecture.
	while(!twi_is_raising_falg());		// Attend que le drapeau ce lève.
	if(twi_get_statu() != TW_MR_SLA_ACK)	// Si le statu n'est pas TW_MR_SLA_ACK, s'est que le composent de cette adresse n'a pas répondu pressent.
		TWI_ERREUR();

	//Reçoit les données.
	twi_receive_byte_ack();			// Permet de préparer la réception d'une donnée et de générai le bit ack à la fin de la résection, pour indiquer que l'on veut accérire d'autre donnée.
	while(!twi_is_raising_falg());		// Attend que le drapeau ce lève.
	if(twi_get_statu() != TW_MR_DATA_ACK)	// Si le statu n'est pas TW_MR_DATA_ACK, s'est qu'il ses produit une erreur.
		TWI_ERREUR();

	buf[0] = twi_get_received_byte();	// Récupère de la donnée ressue.

	twi_receive_byte_nack();		// Permet de préparer la réception d'une donnée et de générai le bit nack à la fin de la résection, pour indiquer que l'on ne veut plus accérire d'autre donnée.
	while(!twi_is_raising_falg());		// Attend que le drapeau ce lève.
	if(twi_get_statu() != TW_MR_DATA_NACK)	// Si le statu n'est pas TW_MR_DATA_NACK, s'est qu'il ses produit une erreur.
		TWI_ERREUR();

	buf[1] = twi_get_received_byte();		// Récupère de la donnée ressue.

	twi_generate_stop();			// Pour générai la condition de stop.
		
	//..
	
	while(1)
	{
		//...
	}
   	 
	return 0;

}
