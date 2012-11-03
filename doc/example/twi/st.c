#include <avr/io.h>
#include <twi.h>

//L'adresse du composent.
#define TWI_ADD 0x32

//Une fonction pour les erreur provenant du bus TWI.
void TWI_ERREUR()
{
	//...
}

int main()
{
	//...
	
	//Un buffer pour envoyer les données.
	uint8_t buf[2] = {0xf8, 0x64};
	
	//Attribue l'adresse esclave au twi.
	twi_set_address(TWI_ADD);
	//Active le twi.
	twi_enable();
	
	//...
	
	twi_receive_byte_ack();					// Prépare le composent a recevoir un octet, puis générera le bit ack (ici l'octet est l'adresse).
	while(!twi_is_raising_falg());			// Attend que le drapeau ce lève. Attend qu'un maître adresse le composent.
	if(twi_get_statu() != TW_ST_SLA_ACK)	// Si le statu n'est pas TW_ST_SLA_ACK, s'est qu'il ses produit une erreur.
		TWI_ERREUR();
				
	twi_send_byte_ack(buf[0]);				// Envoi d'une données.Puis prépare le composent à recevoire le bit ack.
	while(!twi_is_raising_falg());				// Attend que le drapeau ce lève.
	if(twi_get_statu() != TW_ST_DATA_ACK)	// Si le statu n'est pas TW_ST_DATA_ACK, s'est qu'il ses produit une erreur.
		TWI_ERREUR();

	twi_send_byte_nack(buf[1]);				// Envoi d'une données.Puis prépare le composent à recevoir le bit nack.
	while(!twi_is_raising_falg());			// Attend que le drapeau ce lève.
	if(twi_get_statu() != TW_ST_DATA_NACK)	// Si le statu n'est pas TW_ST_DATA_NACK, s'est qu'il ses produit une erreur.
		TWI_ERREUR();
			
	//...


	while(1)
	{
		//...
	}

    return 0;
}
