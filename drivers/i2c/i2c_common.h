/*Fichier conçu pour la génération de la documentation automatique avec
* Doxygen.
*/
//! \file **********************************************************************
//!
//! \brief Code commun pour l'i2c en maître ou esclave.
//!
//! - Compilateur		: GCC-AVR
//! - Composant tester	: atmega8535, atmega644, at90usb1287, at90can128
//!
//!
//! \author Maleyrie Antoine : antoine.maleyrie@gmail.com
//! \version 2.1
//! \date 17 Juin 2011
//!
//! ****************************************************************************

/*
*	Copyright © 2011-2012 - Antoine Maleyrie.
*/

#ifndef I2C_COMMON_H_INCLUDED
#define I2C_COMMON_H_INCLUDED

//! \addtogroup i2cCommun
//! @{

//_____ I N C L U D E S ________________________________________________________
#include <twi.h>

//_____ M A C R O S ____________________________________________________________

//_____ E N U M ________________________________________________________________
//! \brief Les info a propos de l'êta du bus qui peuve survenir au cour d'un transfère de données.
typedef enum
{
	// Erreur
	I2C_INFO_NO_ERROR,			//!< Indique qu'il ne c'est pas produit d'erreur.
	I2C_INFO_DATA_NO_WRITE,		//!< Une parti ou tout les données nom pas pue être écrite.
	I2C_INFO_DATA_NO_READ,		//!< Une parti ou tout les données nom pas pue être lue.

	// Possible si l'i2c en maître.
	#if defined(I2C_MASTER_H_INCLUDED) || defined(__DOXYGEN__)
	I2C_INFO_NO_START,			//!< La condition de start ne s'est pas produit. \note Définie seulement si l'i2c est en maître.
	I2C_INFO_NO_ADDRESSED,		//!< L'esclave n'a pas put être adresser. \note Définie seulement si l'i2c est en maître.
	#endif
}i2c_info_e;

//_____ S T R U C T ____________________________________________________________

//_____ P R O T O T Y P E S ____________________________________________________

//! \brief Active l'i2c.
void i2cEnable(void);

//! \brief Désactive l'i2c.
void i2cDisable(void);

//! @}


#endif // I2C_COMMON_H_INCLUDED
