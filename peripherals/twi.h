/*Fichier conçu pour la génération de la documentation automatique avec
* Doxygen.
*/
//! \file **********************************************************************
//!
//! \brief Fichier pour le twi "2-wire Serial Interface".
//!
//! - Compilateur		: AVR-GCC
//! - Composant tester	: atmega8535, atmega644, at90usb1287, at90can128
//!
//!
//! \see http://www.nongnu.org/avr-libc/user-manual/group__util__twi.html
//!
//!
//! \author Maleyrie Antoine
//! \version 1.1
//! \date 16 Juin 2011
//!
//! ****************************************************************************

/*
*	Copyright © 2011 - Antoine Maleyrie.
*/

#ifndef TWI_H_INCLUDED
#define TWI_H_INCLUDED

//! \addtogroup twi
//! \brief twi (2-wire Serial Interface).
//!
//! \code
//! #include <twi.h>
//! \endcode
//!
//! Toue les exemple si dessous, sont la à titre indicatifs. La gestion du twi peut se faire totalement différemment que ces exemple, notamment avec l'interruption.
//!
//! - Voici un code d'exemple pour le \b maître \b transmettent deux données.
//! \include twi/mt.c
//!
//! - Voici un code d'exemple pour l'\b esclave \b recevant deux données.
//! \include twi/sr.c
//!
//! - Voici un code d'exemple pour le \b maître \b recevant deux données.
//! \include twi/mr.c
//!
//! - Voici un code d'exemple pour l'\b esclave \b transmettent deux données.
//! \include twi/st.c
//!
//! @{

//_____ I N C L U D E S ________________________________________________________
#include <util/twi.h>
#include <stdbool.h>

//_____ M A C R O S ____________________________________________________________
#if !defined(__DOXYGEN__)
	#define TWI_TWCR_MASK	0x0f
#endif

//_____ E N U M ________________________________________________________________

//! \brief Pré-diviseur twps.
//!
//! A utiliser avec \ref twi_bit_rate_generator().
//! \see twi_bit_rate_generator()
typedef enum
{
	TWI_PRESCALER_1		= 0x00,	//!< pré-diviseur pas 1.
	TWI_PRESCALER_4		= 0x01,	//!< pré-diviseur pas 4.
	TWI_PRESCALER_16	= 0x02,	//!< pré-diviseur pas 16.
	TWI_PRESCALER_64	= 0x03,	//!< pré-diviseur pas 64.

	#if !defined(__DOXYGEN__)
	TWI_PRESCALER_MASK	= 0xfc
	#endif
}twi_prescaler_e;


//_____ P R O T O T Y P E S ____________________________________________________

//_____ F U N C T I O N ________________________________________________________

//! \brief Permet d'activer le twi.
//! \see twi_disable()
static inline void twi_enable()
{
	TWCR |= _BV(TWEN);
}

//! \brief Permet de désactiver le twi.
//! \see twi_enable()
static inline void twi_disable()
{
	TWCR &= ~_BV(TWEN);
}

//! \brief Permet d'activer l'intteroption.
//!
//! A utiliser avec le vecteur d'interruption TWI_vect.
//!
//! \code
//! ISR(TWI_vect)
//! {
//! 	//code
//! }
//! \endcode
//! \see twi_disable_interrupt()
static inline void twi_enable_interrupt()
{
	TWCR |= _BV(TWIE);
}

//! \brief Permet de désactiver l'interruption.
//! \see twi_enable_interrupt()
static inline void twi_disable_interrupt()
{
	TWCR &= ~_BV(TWIE);
}


//! \brief Pour savoir si le drapeau des jobs est lever.
//!
//! \return false si le drapeau n'est pas lever.
static inline bool twi_is_raising_falg()
{
	return bit_is_set(TWCR, TWINT);
}

//! \brief Pour obtenir le statut du bus.
//!
//! Pour avoirs la description de tout les status possible, consulter cette page http://www.nongnu.org/avr-libc/user-manual/group__util__twi.html . \n
//! Dans les fonctions présent ici, l'apparition des différent statuts son expliquer dans leur documentation, à l'exception des status \p TW_NO_INFO et \p TW_BUS_ERROR , qui peut survenir le reste du temps, aven une opération par exemple.
//! \see http://www.nongnu.org/avr-libc/user-manual/group__util__twi.html
static inline uint8_t twi_get_statu()
{
	return TW_STATUS;
}

//! \brief Permet de récupérer l'octet qui viens d'être reçus.
//!
//! \return l'octet reçus.
static inline uint8_t twi_get_received_byte()
{
	return TWDR;
}

//! \brief Prépare le composent à resevoire un octet inci qu'a trensmétre le bit ack.
//!
//! \note En mode esclave, l'appelle a cette fonction permet aussi de préparer la réception du statu TW_SR_STOP.
//!
//! \par Si le mode \e maître est actifs.
//! Ceci signifie qu'un octet va être ressue venant d'un esclave, ensuit le mètre va écrira le bit ack, pour indiquer qu'il veut continuer à lire des données. \n \n
//! Le statut qui peut survenir après l'appelle de cette fonction sont :
//! - TW_MR_DATA_ACK \n
//! Les actions qui peuve être générai ensuit sont : \n
//! \ref twi_get_received_byte() suivi de \ref twi_receive_byte_ack() \n
//! \ref twi_get_received_byte() suivi de \ref twi_receive_byte_nack() \n \n
//!
//! \par Si le mode \e esclave est actifs. \n
//! Ceci signifie qu'un octet va être ressue venant du maître, ensuit, l'esclave écrira le bit ack. \n \n
//! Les status qui peuve survenir après l'appelle de cette fonction sont :
//! - TW_SR_SLA_ACK
//! - TW_SR_GCALL_ACK \n
//! - TW_SR_STOP (commute dans le mode non adresser) \n
//! Les actions qui peuve être générai ensuit sont : \n
//! \ref twi_receive_byte_ack() \n
//! \ref twi_receive_byte_nack() \n \n
//! - TW_SR_DATA_ACK
//! - TW_SR_GCALL_DATA_ACK \n
//! Les actions qui peuve être générai ensuit sont : \n
//! \ref twi_get_received_byte() suivi de \ref twi_receive_byte_ack() \n
//! \ref twi_get_received_byte() suivi de \ref twi_receive_byte_nack() \n \n
//! - TW_ST_SLA_ACK \n
//! Les actions qui peuve être générai ensuit sont : \n
//! \ref twi_send_byte_ack() \n
//! \ref twi_send_byte_nack()
static inline void twi_receive_byte_ack()
{
	TWCR = (TWCR&TWI_TWCR_MASK)|_BV(TWINT)|_BV(TWEA);
}

//! \brief Prépare le composent à resevoire un octet inci qu'a trensmétre le bit nack.
//!
//! \note En mode esclave, l'appelle a cette fonction permet aussi de préparer la réception du statu TW_SR_STOP.
//!
//! \par Si le mode \e maître est actifs.
//! Ceci signifie qu'un octet va être ressue venant d'un esclave, ensuit le mètre va écrira le bit nack, pour indiquer qu'il ne veut plu continuer à lire de données. \n \n
//! Le statut qui peut survenir après l'appelle de cette fonction sont :
//! - TW_MR_DATA_NACK \n
//! Les actions qui peuve être générai ensuit sont : \n
//! \ref twi_get_received_byte() suivi de \ref twi_generate_start() \n
//! \ref twi_get_received_byte() suivi de \ref twi_generate_stop() \n
//! \ref twi_get_received_byte() suivi de \ref twi_generate_stop_start() \n
//!
//! \par Si le mode \e esclave est actifs. \n
//! Ceci signifie qu'un octet va être ressue venant du maître, ensuit, l'esclave écrira le bit nack. \n \n
//! Les status qui peuve survenir après l'appelle de cette fonction sont :
//! - TW_SR_DATA_NACK
//! - TW_SR_GCALL_DATA_NACK \n
//! Les actions qui peuve être générai ensuit sont : \n
//! \ref twi_get_received_byte() suivi de \ref twi_receive_byte_ack() (commute dans le mode non adresser) \n
//! \ref twi_get_received_byte() suivi de \ref twi_receive_byte_nack() (commute dans le mode non adresser) \n \n
//! - TW_SR_STOP (commute dans le mode non adresser) \n
//! Les actions qui peuve être générai ensuit sont : \n
//! \ref twi_receive_byte_ack() \n
//! \ref twi_receive_byte_nack() \n \n
static inline void twi_receive_byte_nack()
{
	TWCR = (TWCR&TWI_TWCR_MASK)|_BV(TWINT);
}

//! @name Maître.
//! @{

//! \brief Configuration de l'horloge SCL du bus.
//!
//! La formule suivent permet de connaître les valeurs à envoyer.
//!
//! \f$ fSCL =  \frac {fCPU}{16+2*rate*prescaler} \f$
//! - \b fSCL est la fréquence du bus (<=400KHz).
//! - \b fCPU est le fréquence de votre cpu.
//! - \b rate est un facteur de division.
//! - \b prescaler est le pré-diviseur, cette valeur peut prendre une des valeurs suivent 1, 4, 16 ou 64. \n
//!
//! \param rate est un facteur de division sur 1 octet. \n
//! Prend directement la valeur trouver par la formule.
//! \param prescaler est le pré-diviseur de la fréquence cpu pour la génération de la fréquence du bus. \n
//!	Prend une des valeurs définie par l'énumération \ref twi_prescaler_e.
//!
//! Par exemple, pour un cpu cadencé à 16 MHz et une fréquence sur le bus de 400kHz \n
//! Ainsi on peut fixer le pré-diviseur à 1. On obtiens donc, \p rate = 12 et \p prescaler = 1. \n
//! \code
//!	twi_bit_rate_generator(12, TWI_PRESCALER_1);
//!\endcode
//!
//! \see twi_prescaler_e
static inline void twi_bit_rate_generator(uint8_t rate, twi_prescaler_e prescaler)
{
	TWBR = rate;
	TWSR = (TWSR&TWI_PRESCALER_MASK)|prescaler;
}

//! \brief Permet de généré la condition de start.
//!
//! Les status qui peuve survenir après l'appelle de cette fonction sont :
//! - TW_START
//! - TW_REP_START
//!
//! L'action qui peut être générai ensuit est : \n
//! \ref twi_send_byte()
static inline void twi_generate_start()
{
	TWCR = (TWCR&TWI_TWCR_MASK)|_BV(TWINT)|_BV(TWSTA);
}

//! \brief Permet de généré la condition de stop.
//!
//! L'action qui peut être générai ensuit est : \n
//! \ref twi_generate_start()
static inline void twi_generate_stop()
{
	TWCR = (TWCR&TWI_TWCR_MASK)|_BV(TWINT)|_BV(TWSTO);
}

//! \brief Permet de généré la condition de stop puis la condition de start.
//!
//! Le statut qui peut survenir après l'appelle de cette fonction est :
//! - TW_START
//!
//! L'action qui peut être générai ensuit est : \n
//! \ref twi_send_byte()
static inline void twi_generate_stop_start()
{
	TWCR = (TWCR&TWI_TWCR_MASK)|_BV(TWINT)|_BV(TWSTO)|_BV(TWSTA);
}

//! \brief Permet d'envoyer un octet sur le bus.
//!
//! Après la génération de la condition de start \ref twi_generate_start(), le premier octet à avoyer est particulier.
//! Cette octet, contiens l'adresse esclave sur 7 bit (MSB) et le premier bit (LSB) permet d'indiquer si le maître veut lire (\b TW_READ) ou écrie (\b TW_WRITE), le composent se métra en mode lecture ou écriture automatiquement.
//!
//! \code
//! //Une adresse esclave.
//! #define TWI_ADD 0x32
//!
//! //Exemple pour envoyer l'adresse, plus le mode écriture.
//! twi_send_byte((TWI_ADD<<1)|TW_WRITE);
//!
//! //Exemple pour envoyer l'adresse, plus le mode lecture.
//! twi_send_byte((TWI_ADD<<1)|TW_READ);
//! \endcode
//!
//! \par Si le composen et en mode \e écriture, les status qui peuve survenir après l'appelle de cette fonction sont :
//! - TW_MT_SLA_ACK
//! - TW_MT_SLA_NACK
//! - TW_MT_DATA_ACK
//! - TW_MT_DATA_NACK \n
//! Les actions qui peuve être générai ensuit sont : \n
//! \ref twi_send_byte() \n
//! \ref twi_generate_start() \n
//! \ref twi_generate_stop() \n
//! \ref twi_generate_stop_start() \n \n
//! - TW_MT_ARB_LOST \n
//! Les actions qui peuve être générai ensuit sont : \n
//! \ref twi_switched_slave_mode() \n
//! \ref twi_generate_start() (générai seulement quand le bus sera libre)
//!
//! \par Si le composent et en mode \e lecture, les status qui peuve survenir après l'appelle de cette fonction sont :
//! - TW_MR_SLA_ACK \n
//! Les actions qui peuve être générai ensuit sont : \n
//! \ref twi_receive_byte_ack() \n
//! \ref twi_receive_byte_nack() \n \n
//! - TW_MR_SLA_NACK \n
//! Les actions qui peuve être générai ensuit sont : \n
//! \ref twi_generate_start() \n
//! \ref twi_generate_stop() \n
//! \ref twi_generate_stop_start() \n \n
//! - TW_MR_ARB_LOST \n
//! Les actions qui peuve être générai ensuit sont : \n
//! \ref twi_switched_slave_mode() \n
//! \ref twi_generate_start() (générai seulement quand le bus sera libre)
//!
//! \param byte est l'octet à envoyer.
static inline void twi_send_byte(uint8_t byte)
{
	TWDR = byte;
	TWCR = (TWCR&TWI_TWCR_MASK)|_BV(TWINT);
}

//! \brief Permet de commuter en mode esclave, si un transfère à été perdu.
//!
//! Les status qui peuve survenir après l'appelle de cette fonction sont :
//! - TW_SR_ARB_LOST_SLA_ACK \n
//! - TW_SR_ARB_LOST_GCALL_ACK \n
//! Les actions qui peuve être générai ensuit sont : \n
//! \ref twi_receive_byte_ack() \n
//! \ref twi_receive_byte_nack() \n \n
//! - TW_ST_ARB_LOST_SLA_ACK \n
//! Les actions qui peuve être générai ensuit sont : \n
//! \ref twi_send_byte_ack() \n
//! \ref twi_send_byte_nack()
//!
//! \todo à tester
static inline void twi_switched_slave_mode()
{
	TWCR = (TWCR&TWI_TWCR_MASK)|_BV(TWINT);
}


//! @} //Maître.


//! @name Esclave.
//! @{

//! \brief Permet d'affecter une adresse.
//! \note L'adresse est coder sur 7 bit.
//! \param address est la nouvelle adresse à affecter.
static inline void twi_set_address(uint8_t address)
{
	TWAR = address<<1;
}

//! \brief Pour envoyer une octet et se préparer à recevoir le bit ack.
//!
//! Les status qui peuve survenir après l'appelle de cette fonction sont :
//! - TW_ST_DATA_ACK \n
//! Les actions qui peuve être générai ensuit sont : \n
//! \ref twi_send_byte_ack() \n
//! \ref twi_send_byte_nack() \n \n
//! - TW_ST_LAST_DATA \n
//! Les actions qui peuve être générai ensuit sont : \n
//! \ref twi_receive_byte_ack() (commute dans le mode non adresser) \n
//! \ref twi_receive_byte_nack() (commute dans le mode non adresser)
static inline void twi_send_byte_ack(uint8_t byte)
{
	TWDR = byte;
	TWCR = (TWCR&TWI_TWCR_MASK)|_BV(TWINT)|_BV(TWEA);
}

//! \brief Pour envoyer une octet et se préparer à recevoir le bit nack.
//!
//! Les status qui peuve survenir après l'appelle de cette fonction sont :
//! - TW_ST_DATA_NACK \n
//! - TW_ST_LAST_DATA \n
//! Les actions qui peuve être générai ensuit sont : \n
//! \ref twi_receive_byte_ack() (commute dans le mode non adresser) \n
//! \ref twi_receive_byte_nack() (commute dans le mode non adresser)
static inline void twi_send_byte_nack(uint8_t byte)
{
	TWDR = byte;
	TWCR = (TWCR&TWI_TWCR_MASK)|_BV(TWINT);
}

//! \brief Pour activer l'appelle générale
//!
//! Ceci va permettre de répondre au mètre si il fais un appelle générale.
static inline void twi_enable_general_call()
{
	TWAR |= _BV(TWGCE);
}

//! \brief Pour désactiver l'appelle générale.
static inline void twi_disable_general_call()
{
	TWAR &= ~_BV(TWGCE);
}

//! \brief Pour libéré le bus si un erreur est survenue.
//!
//! Si le statut \b TW_BUS_ERROR  est survenue, vous pouvais faire appelle à cette fonction.
static inline void twi_freed_bus()
{
	TWCR = (TWCR&TWI_TWCR_MASK)|_BV(TWINT)|_BV(TWSTO);
}

//! @} //Slave

//! @} //Twi



#endif // TWI_H_INCLUDED



