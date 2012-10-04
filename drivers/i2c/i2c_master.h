/*Fichier conçue pour la génération de la documentation automatique avec
* Doxygen.
*/
//! \file **********************************************************************
//!
//! \brief Ce fichier contiens le module permettent de manipuler l'i2c en esclave avec le twi.
//!
//! - Compilateur			: GCC-AVR
//! - Composent supporter   : atmega
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


#ifndef I2C_MASTER_H_INCLUDED
#define I2C_MASTER_H_INCLUDED

//! \addtogroup i2cMaster
//! 
//! Rapidement :
//! Il faut définir la fréquence (en Hz) de fonctionnement de l'i2c via la define I2C_FREQUENCY, dans le ficher config.h.
//!	exemple ->
//! \code
//! //********************************* Config i2c *********************************
//! #define I2C_FREQUENCY 400000 //400 KHz
//! \endcode
//!
//! Puis initialiser la bibliothèque via la fonction \ref i2cInit(), activer l'i2c via \ref i2cEnable() et vous pouvez utiliser les fonctions \ref i2cWriteData() et \ref i2cReadData()
//!
//! \todo un/des exemple(s).
//! @{


//_____ I N C L U D E S ________________________________________________________
#include <stdbool.h>
#include <stddef.h>
#include <i2c_common.h>

//_____ M A C R O S ____________________________________________________________

//_____ E N U M ________________________________________________________________

//_____ P R O T O T Y P E S ____________________________________________________
//! \brief Permet d'initialiser l'i2c.
void i2cInit(void);

//! \brief Envoi des données à une adresse.
//!
//! \attention Si une erreur et retourner, le bus est libérer par un appelle à \ref twi_generate_stop(), ( \p stop n'est pas prit en compte).
//! \param address est l'adresse du composent cible.
//! \param data est le tableau de données à envoyés.
//! \param size le nombre de donner à envoyer.
//! \param stop permet de générai la condition de stop à la fin de l'envoi des données.
//! \return Une info qui aurais put survenir. Si l'info est  \ref I2C_INFO_NO_ERROR les données on bien été écrite.
//! \see i2cReadData()
//! \see i2c_info_e
i2c_info_e i2cWriteData(const uint8_t address, const uint8_t data[], const size_t size, const bool stop);


//! \brief Resoit des données d'une adresse.
//!
//! \attention Si une erreur et retourner, le bus est libérer par un appelle à \ref twi_generate_stop(), ( \p stop n'est pas prit en compte).
//! \param address est l'adresse du composent cible.
//! \param data est un tableau où seront stoker les données.
//! \param size le nombre de donner à recevoir.
//! \param stop permet de générai la condition de stop à la fin de l'envoi des données.
//! \return Une info qui aurais put survenir. Si l'info est  \ref I2C_INFO_NO_ERROR les données on bien été lue.
//! \see i2cWriteData()
//! \see i2c_info_e
i2c_info_e i2cReadData(const uint8_t address, uint8_t data[], const size_t size, const bool stop);

//! @}

#endif // I2C_MASTER_H_INCLUDED
