/*Fichier conçue pour la génération de la documentation automatique avec
* Doxygen.
*/
//! \file **********************************************************************
//!
//! \brief Ce fichier contiens le module permettent de manipuler l'i2c en esclave avec le twi.
//!
//! - Compilateur		    : GCC-AVR
//! - Composant tester	: atmega8535, atmega644, at90usb1287, at90can128
//!
//!
//! \author Maleyrie Antoine
//! \version 0.2
//! \date 10 Joint 2012
//!
//! ****************************************************************************

/*
*	Copyright © 2012 - Antoine Maleyrie.
*/

//_____ I N C L U D E S ________________________________________________________
#include <i2c_master.h>
#include "config.h"

//_____ M A C R O S ____________________________________________________________
//Définie les valeurs de I2C_RATE et I2C_PRESCALER.
#if(((F_CPU/I2C_FREQUENCY)-16)/2 > 255)
	#if(((F_CPU/I2C_FREQUENCY)-16)/8 > 255)
		#if(((F_CPU/I2C_FREQUENCY)-16)/32 > 255)
			#if(((F_CPU/I2C_FREQUENCY)-16)/128 > 255)
			#else
			#define I2C_RATE ((F_CPU/I2C_FREQUENCY)-16)/128
			#define I2C_PRESCALER TWI_PRESCALER_64
			#endif
		#else
		#define I2C_RATE ((F_CPU/I2C_FREQUENCY)-16)/32
		#define I2C_PRESCALER TWI_PRESCALER_16
		#endif
	#else
	#define I2C_RATE ((F_CPU/I2C_FREQUENCY)-16)/8
	#define I2C_PRESCALER TWI_PRESCALER_4
	#endif
#else
#define I2C_RATE ((F_CPU/I2C_FREQUENCY)-16)/2
#define I2C_PRESCALER TWI_PRESCALER_1
#endif

//_____ P R O T O T Y P E S ____________________________________________________

//_____ I M P L E M E N T A T I O N S __________________________________________
void i2cInit()
{
	twi_bit_rate_generator(I2C_RATE, I2C_PRESCALER);
}

i2c_info_e i2cWriteData(const uint8_t address, const uint8_t data[], const size_t size, const bool stop)
{
	// Si pas de données a envoyer.
	if(size == 0)
		return I2C_INFO_NO_ERROR;

	twi_generate_start();												// Pour générai la condition de start.
	while(!twi_is_raising_falg());      								// Attend que le drapeau ce lève.
	if(twi_get_statu() != TW_START && twi_get_statu() != TW_REP_START)	// Si le statu n'est pas TW_START ou TW_REP_START, s'est qu'il ses produit une erreur.
	{
		twi_generate_stop();
		return I2C_INFO_NO_START;
	}

	//Envois de l'adresse et du mode écriture.
	twi_send_byte((address<<1)|TW_WRITE);					// Envoi du premier octet, qui contient l'adresse et le mode écriture.
	while(!twi_is_raising_falg());      					// Attend que le drapeau ce lève.
	if(twi_get_statu() != TW_MT_SLA_ACK)					// Si le statu n'est pas TW_MT_SLA_ACK, s'est qu'il ses produit une erreur.
	{
		twi_generate_stop();
		return I2C_INFO_NO_ADDRESSED;
	}

	//Envoi autan de données que demander.
	for(size_t i=0; i < size; i++)
	{
		twi_send_byte(data[i]);						// Envoi d'une données.
		while(!twi_is_raising_falg());      		// Attend que le drapeau ce lève.

		if(twi_get_statu() != TW_MT_DATA_ACK)		// Si le statu n'est pas TW_MT_DATA_ACK, s'est qu'il ses produit une erreur.
		{
			twi_generate_stop();
			return I2C_INFO_DATA_NO_WRITE;
		}
	}

	// Pour générai la condition de stop, si souhaiter.
	if(stop)
		twi_generate_stop();

	return I2C_INFO_NO_ERROR;
}

i2c_info_e i2cReadData(const uint8_t address, uint8_t data[], const size_t size, const bool stop)
{
	// Si pas de données a envoyer.
	if(size == 0)
		return I2C_INFO_NO_ERROR;

	twi_generate_start();												// Pour générai la condition de start.
	while(!twi_is_raising_falg());      								// Attend que le drapeau ce lève.
	if(twi_get_statu() != TW_START && twi_get_statu() != TW_REP_START)	// Si le statu n'est pas TW_START ou TW_REP_START, s'est qu'il ses produit une erreur.
	{
		twi_generate_stop();
		return I2C_INFO_NO_START;
	}

	//Envois de l'adresse et du mode lecture.
	twi_send_byte((address<<1)|TW_READ);			// Envoi du premier octet, qui contient l'adresse et le mode écriture.
	while(!twi_is_raising_falg());      			// Attend que le drapeau ce lève.
	if(twi_get_statu() != TW_MR_SLA_ACK)			// Si le statu n'est pas TW_MR_SLA_ACK, s'est qu'il ses produit une erreur.
	{
		twi_generate_stop();
		return I2C_INFO_NO_ADDRESSED;
	}

	const size_t iteratorTop = size-1;				// Itération maximum du tableau des données.

	//Reçoit autan de données -1 que demander.
	for(size_t i=0; i < iteratorTop; i++)
	{
		twi_receive_byte_ack();						// Prépare le composent à resevoire un octet inci qu'a transmettre le bit ack.
		while(!twi_is_raising_falg());      		// Attend que le drapeau ce lève.
		if(twi_get_statu() != TW_MR_DATA_ACK)		// Si le statu n'est pas TW_MR_DATA_ACK, s'est qu'il ses produit une erreur.
		{
			twi_generate_stop();
			return I2C_INFO_DATA_NO_READ;
		}
		data[i] = twi_get_received_byte();			// Récupère l'octet qui viens d'être reçus.
	}

	// Resoi le dernier octet
	twi_receive_byte_nack();						// Prépare le composent à resevoire un octet inci qu'a transmettre le bit nack.
	while(!twi_is_raising_falg());      			// Attend que le drapeau ce lève.
	if(twi_get_statu() != TW_MR_DATA_NACK)			// Si le statu n'est pas TW_MR_DATA_NACK, s'est qu'il ses produit une erreur.
	{
		twi_generate_stop();
		return I2C_INFO_DATA_NO_READ;
	}
	data[iteratorTop] = twi_get_received_byte();	// Récupère l'octet qui viens d'être reçus.

	// Pour générai la condition de stop, si souhaiter.
	if(stop)
		twi_generate_stop();

	return I2C_INFO_NO_ERROR;
}
