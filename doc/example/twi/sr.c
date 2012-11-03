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
	
	//Un buffer pour récupérer les données.
	uint8_t buf[2];
	
	//Attribue l'adresse esclave au twi.
	twi_set_address(TWI_ADD);
	//Active le twi.
	twi_enable();
	
	//...

	twi_receive_byte_ack();					// Prépare le composent a recevoir un octet, puis générera le bit ack (ici l'octet est l'adresse).
	while(!twi_is_raising_falg());			// Attend que le drapeau ce lève. Attend qu'un maître adresse le composent.
	if(twi_get_statu() != TW_SR_SLA_ACK)	// Si le statu n'est pas TW_SR_SLA_ACK, s'est qu'il ses produit une erreur.
		TWI_ERREUR();

	twi_receive_byte_ack();					// Prépare le composent a recevoir un octet, puis générera le bit ack.
	while(!twi_is_raising_falg());			// Attend que le drapeau ce lève.
	if(twi_get_statu() != TW_SR_DATA_ACK)	// Si le statu n'est pas TW_SR_DATA_ACK, s'est qu'il ses produit une erreur.
		TWI_ERREUR();

	buf[0] = twi_get_received_byte();		// Récupère de la donnée ressue.
		
	twi_receive_byte_ack();					// Prépare le composent a recevoir un octet, puis générera le bit ack.
	while(!twi_is_raising_falg());			// Attend que le drapeau ce lève.
	if(twi_get_statu() != TW_SR_DATA_ACK)	// Si le statu n'est pas TW_SR_DATA_ACK, s'est qu'il ses produit une erreur.
		TWI_ERREUR();

	buf[1] = twi_get_received_byte();		// Récupère de la donnée ressue.
		
	twi_receive_byte_ack();					// Prépare le composent a recevoir la condition de stop.
	while(!twi_is_raising_falg());			// Attend que le drapeau ce lève.
	if(twi_get_statu() != TW_SR_STOP)		// Si le statu n'est pas W_SR_STOP, s'est qu'il ses produit une erreur.
		TWI_ERREUR();
		
	//...

	while(1)
	{
		//...
	}

    return 0;
}
