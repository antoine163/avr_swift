/*Fichier conçue pour la génération de la documentation automatique avec
* Doxygen.
*/
//! \file **********************************************************************
//!
//! \brief Fichier référence de la bibliothèque avr_swift.
//! \attention Ce fichier est présent juste pour la description de la bibliothèque avr_swift. \n
//! Ce fichier ne devra donc pas être inclue à la compilation.
//!
//! - Compilateur		: GCC-AVR
//! - Composent supporter	: AVR
//!
//!
//! \author Maleyrie Antoine
//! \version  1.3
//! \date 19 Juin 2011
//!
//! ****************************************************************************

/*
*	Copyright © 2011 - Antoine Maleyrie.
*/

#ifndef AVR_SWIFT_H_INCLUDED
#define AVR_SWIFT_H_INCLUDED

//! \mainpage avr_swift
//! avr_swift est une bibliothèque réaliser dans le but de développer plus rapidement le software des microcontrollers de type AVR de chez ATMEL. \n
//! Elle contiens, les fichiers pour la gestion des coupleurs ainsi que des driver s'appuient sur ces coupleurs. \n
//!
//! \author Maleyrie Antoine : antoine.maleyrie@gmail.com
//! \version  1.1
//! \see http://www.atmel.com/products/avr/default.asp?source=cms&category_id=163&family_id=607&source=global_nav


//! \defgroup peripherials
//! @{
//! 	\defgroup twi
//! 	\defgroup tc
//! 	\defgroup adc
//! 	\defgroup spi
//! 	\defgroup usart
//! @}

/*
*
*/

//! \defgroup driver
//! @{
//!	 	\defgroup i2c
//! 	@{
//!			\defgroup i2cMaster
//! 		\brief Driver i2c en maître.
//!			\defgroup i2cSlave
//! 		\brief Driver i2c en esclave.
//!			\defgroup i2cCommun
//! 		\brief Fonction commune pour l'i2c en maître et esclave.
//! 	@}

//! @}

#endif // AVR_SWIFT_H_INCLUDED

