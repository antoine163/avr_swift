/*Fichier conçu pour la génération de la documentation automatique avec
* Doxygen.
*/
//! \file **********************************************************************
//!
//! \brief Fichier pour le spi (Serial Peripheral Interface).
//!
//! - Compilateur		: AVR-GCC
//! - Composant tester	: at90usb1287, atmega8535
//!
//!
//!
//! \author Maleyrie Antoine
//! \version 0.2
//! \date 04 Octobre 2012
//!
//! ****************************************************************************

/*
*	Copyright © 2012 - Antoine Maleyrie.
*/

#ifndef SPI_H_INCLUDED
#define SPI_H_INCLUDED

//! \addtogroup spi
//! \brief spi (Serial Peripheral Interface).
//!
//! \code
//! #include <spi.h>
//! \endcode
//!
//! - Voir un code d'exemple en mode maître :
//! \include master_scan.c
//! Pour attendre la fin d'une transaction dans l'exemple, on scrute le 
//! drapeau de transfert \ref spi_is_raising_transfer_flag().
//! Vous pouvez vous inspirer de l'exemple suivent pour mètre en place les interruptions.
//! - Voir un code d'exemple en mode esclave :
//! \include slave_interruption.c
//! De même ici pour pouvez faire de la scrutation à la place des interruptions
//! @{

//_____ I N C L U D E S ________________________________________________________
#include <avr/io.h>
#include <stdbool.h>

//_____ M A C R O S ____________________________________________________________

//_____ S T R U C T ____________________________________________________________

//_____ E N U M ________________________________________________________________

//! \brief L'ordre des données.
//!
//! A utiliser avec \ref spi_data_order().
//! \see spi_data_order()
typedef enum
{
	SPI_DATA_ORDER_LSB		= _BV(DORD),	//!< Les bits de pois faible en premier
	SPI_DATA_ORDER_MSB		= 0,			//!< Les bits de pois fort en premier
	
	#if !defined(__DOXYGEN__)
	SPI_DATA_ORDER_MASK 	= _BV(DORD)
	#endif
}spi_data_order_e;

//! \brief le mode maître ou esclave.
//!
//! A utiliser avec \ref spi_mode().
//! \see spi_mode()
typedef enum
{
	SPI_MODE_MASTER		= _BV(MSTR),		//!< spi en mode maître.
	SPI_MODE_SLAVE		= 0,				//!< spi en mode esclave.
	
	#if !defined(__DOXYGEN__)
	SPI_MODE_MASK 	= _BV(MSTR)
	#endif
}spi_mode_e;

//! \brief les différent modes d'horloges.
//! A utiliser avec \ref spi_clock_phase().
//! \see spi_clock_mode()
typedef enum
{
	SPI_CLOCK_MODE_0	= 0,					//!< Donnée valide sur front montent sans déphasage.\n
												//!< Voici un exemple ou le maître écrie 0x33 :\image html spi/clock_mode_0.bmp
	SPI_CLOCK_MODE_1	= _BV(CPHA),			//!< Donnée valide sur front descendant avec déphasage.\n
												//!< Voici un exemple ou le maître écrie 0x33 :\image html spi/clock_mode_1.bmp
	SPI_CLOCK_MODE_2	= _BV(CPOL),			//!< Donnée valide sur front descendant sans déphasage.\n
												//!< Voici un exemple ou le maître écrie 0x33 :\image html spi/clock_mode_2.bmp
	SPI_CLOCK_MODE_3	= _BV(CPHA)|_BV(CPOL),	//!< Donnée valide sur front montent avec déphasage.\n
												//!< Voici un exemple ou le maître écrie 0x33 :\image html spi/clock_mode_3.bmp
	
	#if !defined(__DOXYGEN__)
	SPI_CLOCK_MODE 	= _BV(CPOL)|_BV(CPHA)
	#endif
}spi_clock_mode_e;


//! \brief Valeur possible pour la sélection de l'horloge.
//!
//! A utiliser avec \ref spi_clock_select().
//! \see spi_clock_select()
typedef enum
{
	SPI_CLOCK_CLK_2 		= (_BV(SPI2X)<<8),				//!< Horloge du cpu diviser par 2.
	SPI_CLOCK_CLK_4 		= 0,							//!< Horloge du cpu diviser par 4.
	SPI_CLOCK_CLK_8 		= (_BV(SPI2X)<<8)|_BV(SPR0),	//!< Horloge du cpu diviser par 8.
	SPI_CLOCK_CLK_16 		= _BV(SPR0),					//!< Horloge du cpu diviser par 16.
	SPI_CLOCK_CLK_32 		= (_BV(SPI2X)<<8)|_BV(SPR1),	//!< Horloge du cpu diviser par 32.
	SPI_CLOCK_CLK_64 		= _BV(SPR1),					//!< Horloge du cpu diviser par 64.
	SPI_CLOCK_CLK_128 		= _BV(SPR1)|_BV(SPR0),			//!< Horloge du cpu diviser par 128.

	#if !defined(__DOXYGEN__)
	SPI_CLOCK_MASK 			= (_BV(SPI2X)<<8)|_BV(SPR1)|_BV(SPR0)
	#endif
}spi_clock_e;


//_____ F U N C T I O N ________________________________________________________

//! \brief Permet d'activer le spi.
//! \see spi_disable()
static inline void spi_enable()
{
	SPCR |= _BV(SPE);
}

//! \brief Permet de désactiver le twi.
//! \see twi_enable()
static inline void spi_disable()
{
	SPCR &= ~_BV(SPE);
}

//! \brief Permet d'activer l'interruption.
//!
//! A utiliser avec le vecteur d'interruption SPI_STC_vect.
//! 
//! \code
//! ISR(SPI_STC_vect)
//! {
//! 	//code
//! }
//! \endcode
//! \note Le drapeau de transfert est automatiquement 
//! nettoyer à l'exécution de cette interruption (SPI_STC_vect). La fonction
//! \ref spi_is_raising_transfer_flag() n'est donc pas à appeler pour le nettoyage.
//! \see spi_disable_interrupt()
static inline void spi_enable_interrupt()
{
	SPCR |= _BV(SPIE);
}

//! \brief Permet de désactiver l'interruption.
//! \see spi_enable_interrupt()
static inline void spi_disable_interrupt()
{
	SPCR &= ~_BV(SPIE);
}

//! \brief Permet de définir l'ordre des données.
//! \param data_order permet de choisir le sens de transmission des données.\ref SPI_DATA_ORDER_MSB est actifs par défaut.
static inline void spi_data_order(spi_data_order_e data_order)
{
	SPCR = (SPCR&~SPI_DATA_ORDER_MASK)|data_order;
}

//! \brief Définir le mode du composent, en maître ou en esclave.
//! \param mode permet de choisir le mode du composent, maître ou esclave.\ref SPI_MODE_SLAVE est actifs par défaut.
static inline void spi_mode(spi_mode_e mode) 
{
	SPCR = (SPCR&~SPI_MODE_MASK)|mode;
}

//! \brief Définir le mode de l'horloge.
//! \param spi_clock_mode_e est le mode choisi,.\ref SPI_CLOCK_MODE_0 est actifs par défaut.
static inline void spi_clock_mode(spi_clock_mode_e clock_mode)
{
	SPCR = (SPCR&~SPI_CLOCK_MODE)|clock_mode;
}

//! \brief Pour sélectionner l'horloge.
//!
//!Cette fonction est utilise seulement en mode maître.
//! \note L'appelle de cette fonction nettoie automatiquement les drapeaux.
//! \param clock est la sélection de la source de l'horloge pour le SPI. \ref SPI_CLOCK_CLK_4 est actifs par défaut.
static inline void spi_clock_select(spi_clock_e clock)
{
	SPCR = (SPCR&~SPI_CLOCK_MASK)|clock;
	SPSR = (SPSR&~(SPI_CLOCK_MASK>>8))|(clock>>8);
}

//! \brief Permet de configurer le spi.
//! Évite d'appeler tout les fonctions pour la configuration.
//! \param config est la configuration souhaité. Les valeur possible son d'écrite dans les enum \ref spi_data_order_e, 
//!	\ref spi_mode_e, \ref spi_clock_mode_e,  \ref spi_clock_e.
//! \code
//! //un exemple :
//! spi_configure(SPI_MODE_MASTER|SPI_DATA_ORDER_MSB|SPI_CLOCK_POLARITY_RISING|SPI_CLOCK_PHASE_SAMPLE|SPI_CLOCK_CLK_8);
//! \endcode
//! \see spi_data_order()
//! \see spi_mode()
//! \see spi_clock_polarity()
//! \see spi_clock_phase()
//! \see spi_clock_phase()
//! \see spi_data_order_e
//! \see spi_mode_e
//! \see spi_clock_mode_e
//! \see spi_clock_e
static inline void spi_configure(uint16_t config)
{
	SPCR = config;
	SPSR = config>>8;
}

//! \brief Pour savoir si le drapeau de transfert est lever.
//! \note L'appelle de cette fonction nettoie automatiquement le drapeau de transfert.
//! \attention Nettoie aussi le drapeau de collision lier à la fonction \ref spi_is_raising_collision_flag().
//! \return false si le drapeau n'est pas lever.
static inline bool spi_is_raising_transfer_flag()
{
	return bit_is_set(SPSR, SPIF);
}

//! \brief Pour savoir si le drapeau de collision est lever.
//! \note L'appelle de cette fonction nettoie automatiquement le drapeau de collision.
//! \attention Nettoie aussi le drapeau de transfert lier à la fonction \ref spi_is_raising_transfer_flag().
//! \return false si le drapeau n'est pas lever.
static inline bool spi_is_raising_collision_flag()
{
	return bit_is_set(SPSR, SPIF);
}

//! \brief Pour écrire une donnée.
//! \note L'appelle de cette fonction écrira votre donnée juste dans le registre SPDR.
//! \brief 
//! - Si vous êtes en mode esclave la donner sera transmise lorsque le maître l'auras décider. 
//! - Si vous êtes en mode maître pour envoyer la donnée sur le bus spi il vous faudra en plus appeler la fonction \ref twi_read_data().
//! \code
//! //Exemple, pour écrire 0x32 sur le bus isp en mode maître.
//! twi_write_data(0x32)	//Écriture dans le registre SPDR.
//! twi_read_data()			//Lecture de SPDR (qui contiens 0x32, on ignorer la lecture).
//!							//Mais surtout provoque la rotation des registres sur le bus spi.
//!							//La donnée et transmis sur le bus à ce moment,
//!							//(ainsi on récupère dans SPDR la donnée qui se trouvent dans l'esclave,
//!							//un deuxième appelle a \ref twi_read_data() pour récupérée la donnée provenant de l'esclave).
//! \endcode
//! \param data la donnée à écrire.
//! \see twi_write_data()
static inline void twi_write_data(uint8_t data)
{
	SPDR = data;
}

//! \brief Pour lire une donnée.
//! - Si vous êtes en mode esclave, rien de particulier vous lisez juste la donnée se trouvent dans SPDR.
//! - Si vous êtes en mode maître, de même vous lisez la donnée se trouvent dans SPDR, mais ensuit cette fonction 
//! provoque aussi la rotation des registres sur le bus spi.
//! \return la donnée se trouvent dans SPDR.
//! \see twi_write_data()
static inline uint8_t twi_read_data()
{
	return SPDR;
}

//! @} //spi

#endif // SPI_H_INCLUDED



