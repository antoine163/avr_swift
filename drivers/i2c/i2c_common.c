/*Fichier conçu pour la génération de la documentation automatique avec
* Doxygen.
*/
//! \file **********************************************************************
//!
//! \brief Code commun pour l'i2c en maître ou esclave.
//!
//! - Compilateur		    : GCC-AVR
//! - Composant tester	: atmega8535, atmega644, at90usb1287, at90can128
//!
//!
//! \author Maleyrie Antoine
//! \version 0.1
//! \date 16 Juin 2012
//!
//! ****************************************************************************

/*
*	Copyright © 2012 - Antoine Maleyrie.
*/

//_____ I N C L U D E S ________________________________________________________
#include <i2c_common.h>

//_____ M A C R O S ____________________________________________________________

//_____ P R O T O T Y P E S ____________________________________________________

//_____ I M P L E M E N T A T I O N S __________________________________________
void i2cEnable(void)
{
	twi_enable();
}

void i2cDisable(void)
{
	twi_disable();
}
