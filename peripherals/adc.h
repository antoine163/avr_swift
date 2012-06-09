/*Fichier conçu pour la génération de la documentation automatique avec
* Doxygen.
*/
//! \file **********************************************************************
//!
//! \brief Fichier pour bénéficier de la conversation analogique numérique (Analog-to-digital Converter).
//!
//! - Compilateur		: AVR-GCC
//! - Composent supporter	: atmega/attiny
//!
//!
//! \author Maleyrie Antoine
//! \version 1.3
//! \date 16 Juin 2011
//!
//! ****************************************************************************

/*
*	Copyright © 2011 - Antoine Maleyrie.
*/

#ifndef ADC_H_INCLUDED
#define ADC_H_INCLUDED

//! \addtogroup Adc
//! \brief Driver pour bénéficier de la conversation analogique numérique (Analog-to-digital Converter).
//!
//! \code
//! #include "sw/drv/adc.h"
//! \endcode
//!
//! Voir un code d'exemple.
//! \include adc.c
//! @{


//_____ I N C L U D E S ________________________________________________________
#include <avr/io.h>
#include <stdbool.h>

//_____ M A C R O S ____________________________________________________________
#if defined(ADCSRB) && !defined(ADHSM) && !defined(__DOXYGEN__)
#define ADHSM 7
#endif

//_____ E N U M ________________________________________________________________
//! \brief Valeur pour sélectionner la valeur de la tension de référence (Vref) du convertisseur.
//!
//! A utiliser avec \ref adc_voltage_select().
//! \see adc_voltage_select()
typedef enum
{
	ADC_VOLTAGE_AREF 	= 0x00, //!< Tension de référence sur AREF (off).
	ADC_VOLTAGE_AVCC 	= 0x40, //!< Tension de référence sur AVCC, avec une capacité sur AREF.
	ADC_VOLTAGE_1_1 	= 0x80, //!< Tension de référence sur le 1.1V, avec une capacité sur AREF. \note Cette référence n'est pas présent pour tout les composants.
	ADC_VOLTAGE_2_56 	= 0xc0, //!< Tension de référence sur 2.56V, avec une capacité sur AREF.

	#if !defined(__DOXYGEN__)
	ADC_VOLTAGE_MASK 	= 0x3f	//!< Mask, ne pas l'utiliser.
	#endif
}adc_voltage_e;

//! \brief Valeur pour sélectionner le signale d'entrée et le gain.
//!
//! A utiliser avec \ref adc_channel_select().
//! \note Les "channels" avec gain sont à utiliser avec la tension de référence de 2.56V.
//! \see adc_channel_select()
typedef enum
{
	ADC_CHANNEL_ADC0		= 0x00,	//!< ADC0 (Vin) comme entrée.
	ADC_CHANNEL_ADC1		= 0x01,	//!< ADC1 (Vin) comme entrée.
	ADC_CHANNEL_ADC2		= 0x02,	//!< ADC2 (Vin) comme entrée.
	ADC_CHANNEL_ADC3		= 0x03,	//!< ADC3 (Vin) comme entrée.
	ADC_CHANNEL_ADC4		= 0x04,	//!< ADC4 (Vin) comme entrée.
	ADC_CHANNEL_ADC5		= 0x05,	//!< ADC5 (Vin) comme entrée.
	ADC_CHANNEL_ADC6		= 0x06,	//!< ADC6 (Vin) comme entrée.
	ADC_CHANNEL_ADC7		= 0x07,	//!< ADC7 (Vin) comme entrée.

	ADC_CHANNEL_ADC0_COMP_ADC0_10X	= 0x08,	//!< ADC0 (Vpos) comparer avec ADC0 (Vneg) comme entrée avec un gain de 10.
	ADC_CHANNEL_ADC1_COMP_ADC0_10X	= 0x09,	//!< ADC1 (Vpos) comparer avec ADC0 (Vneg) comme entrée avec un gain de 10.
	ADC_CHANNEL_ADC0_COMP_ADC0_200X	= 0x0a,	//!< ADC0 (Vpos) comparer avec ADC0 (Vneg) comme entrée avec un gain de 200.
	ADC_CHANNEL_ADC1_COMP_ADC0_200X	= 0x0b,	//!< ADC1 (Vpos) comparer avec ADC0 (Vneg) comme entrée avec un gain de 200.
	ADC_CHANNEL_ADC2_COMP_ADC2_10X	= 0x0c,	//!< ADC2 (Vpos) comparer avec ADC2 (Vneg) comme entrée avec un gain de 10.
	ADC_CHANNEL_ADC3_COMP_ADC2_10X	= 0x0d,	//!< ADC3 (Vpos) comparer avec ADC2 (Vneg) comme entrée avec un gain de 10.
	ADC_CHANNEL_ADC2_COMP_ADC2_200X	= 0x0e,	//!< ADC2 (Vpos) comparer avec ADC2 (Vneg) comme entrée avec un gain de 200.
	ADC_CHANNEL_ADC3_COMP_ADC2_200X	= 0x0f,	//!< ADC3 (Vpos) comparer avec ADC2 (Vneg) comme entrée avec un gain de 200.
	ADC_CHANNEL_ADC0_COMP_ADC1_1X	= 0x10,	//!< ADC0 (Vpos) comparer avec ADC1 (Vneg) comme entrée avec un gain de 1.
	ADC_CHANNEL_ADC1_COMP_ADC1_1X	= 0x11,	//!< ADC1 (Vpos) comparer avec ADC1 (Vneg) comme entrée avec un gain de 1.
	ADC_CHANNEL_ADC2_COMP_ADC1_1X	= 0x12,	//!< ADC2 (Vpos) comparer avec ADC1 (Vneg) comme entrée avec un gain de 1.
	ADC_CHANNEL_ADC3_COMP_ADC1_1X	= 0x13,	//!< ADC3 (Vpos) comparer avec ADC1 (Vneg) comme entrée avec un gain de 1.
	ADC_CHANNEL_ADC4_COMP_ADC1_1X	= 0x14,	//!< ADC4 (Vpos) comparer avec ADC1 (Vneg) comme entrée avec un gain de 1.
	ADC_CHANNEL_ADC5_COMP_ADC1_1X	= 0x15,	//!< ADC5 (Vpos) comparer avec ADC1 (Vneg) comme entrée avec un gain de 1.
	ADC_CHANNEL_ADC6_COMP_ADC1_1X	= 0x16,	//!< ADC6 (Vpos) comparer avec ADC1 (Vneg) comme entrée avec un gain de 1.
	ADC_CHANNEL_ADC7_COMP_ADC1_1X	= 0x17,	//!< ADC7 (Vpos) comparer avec ADC1 (Vneg) comme entrée avec un gain de 1.
	ADC_CHANNEL_ADC0_COMP_ADC2_1X	= 0x18,	//!< ADC0 (Vpos) comparer avec ADC2 (Vneg) comme entrée avec un gain de 1
	ADC_CHANNEL_ADC1_COMP_ADC2_1X	= 0x19,	//!< ADC1 (Vpos) comparer avec ADC2 (Vneg) comme entrée avec un gain de 1.
	ADC_CHANNEL_ADC2_COMP_ADC2_1X	= 0x1a,	//!< ADC2 (Vpos) comparer avec ADC2 (Vneg) comme entrée avec un gain de 1.
	ADC_CHANNEL_ADC3_COMP_ADC2_1X	= 0x1b,	//!< ADC3 (Vpos) comparer avec ADC2 (Vneg) comme entrée avec un gain de 1.
	ADC_CHANNEL_ADC4_COMP_ADC2_1X	= 0x1c,	//!< ADC4 (Vpos) comparer avec ADC2 (Vneg) comme entrée avec un gain de 1.
	ADC_CHANNEL_ADC5_COMP_ADC2_1X	= 0x1d,	//!< ADC5 c(Vpos) omparer avec ADC2 (Vneg) comme entrée avec un gain de 1.

	ADC_CHANNEL_1_1			= 0x1e,	//!< 1.1V (Vin) comme entrée, (Selon composent).
	ADC_CHANNEL_1_22		= 0x1e,	//!< 1.22V (Vin) comme entrée, (Selon composent).
	ADC_CHANNEL_0			= 0x1f,	//!< 0V (GND) (Vin) comme entrée.

	#if !defined(__DOXYGEN__)
	ADC_CHANNEL_MASK		= 0xe0
	#endif
}adc_channel_e;

//! \brief Valeur possible poue le prediviseur.
//!
//! A utiliser avec \ref adc_prescaler_select().
//!
//! \see adc_prescaler_select()
typedef enum
{
	ADC_PRESCALER_2 	= 0x00, 	//!< Prediviseur à 2.
	ADC_PRESCALER_2_	= 0x01, 	//!< Prediviseur à 2.
	ADC_PRESCALER_4 	= 0x02, 	//!< Prediviseur à 4.
	ADC_PRESCALER_8 	= 0x03, 	//!< Prediviseur à 8.
	ADC_PRESCALER_16 	= 0x04, 	//!< Prediviseur à 16.
	ADC_PRESCALER_32	= 0x05, 	//!< Prediviseur à 32.
	ADC_PRESCALER_64 	= 0x06, 	//!< Prediviseur à 64.
	ADC_PRESCALER_128 	= 0x07,	 	//!< Prediviseur à 128.

	#if !defined(__DOXYGEN__)
	ADC_PRESCALER_MASK 	= 0xf8
	#endif
}adc_prescaler_e;

//! \brief Valeur possible pour (auto trigger).
//!
//! A utiliser avec \ref adc_trigger_select().
//!
//! \see adc_trigger_select()
typedef enum
{
	ADC_TRIGGER_FREE_RUNNING_MODE 		 = 0x00,				//!< Déclenche une conversion à la fin d'une conversion.
	ADC_TRIGGER_ANALOG_COMPARATOR		 = _BV(ADTS0), 				//!< \todo à expliquer.
	ADC_TRIGGER_EXTERNAL_INTERRUPT_REQUEST_0 = _BV(ADTS1), 				//!< Déclenche une conversion sur l'interruption externe (pin INT0).
	ADC_TRIGGER_TIMER0_COMP			 = _BV(ADTS1)|_BV(ADTS0), 		//!< Déclenche une conversion sur la comparaison du timer 0.
	ADC_TRIGGER_TIMER0_OVERFOW		 = _BV(ADTS2), 				//!< Déclenche une conversion à chaque débordement du timer 0.
	ADC_TRIGGER_TIMER1_COMP_B		 = _BV(ADTS2)|_BV(ADTS0), 		//!< Déclenche une conversion sur la comparaison B du timer 1.
	ADC_TRIGGER_TIMER1_OVERFOW		 = _BV(ADTS2)|_BV(ADTS1),		//!< Déclenche une conversion à chaque débordement du timer 1.
	ADC_TRIGGER_TIMER1_CAPTURE_EVENT	 = _BV(ADTS2)|_BV(ADTS1)|_BV(ADTS0),	//!< Déclenche une conversion à chaque capture du timer 1. \see tc_enable_interrupt_capture()

	#if !defined(__DOXYGEN__)
	ADC_TRIGGER_MASK 			 =  ~(_BV(ADTS2)|_BV(ADTS1)|_BV(ADTS0))
	#endif
}adc_trigger_e;

#if defined(DIDR0) || defined(__DOXYGEN__)

//! \brief Les différent bits rattacher au convertisseur.
//!
//! A utiliser avec \ref adc_disable_input().
//!
//! \see adc_disable_input()
//! \see adc_enable_input()
typedef enum
{
	ADC_INPUT_ADC0		= _BV(ADC0D), 	//!< Pin ADC0.
	ADC_INPUT_ADC1		= _BV(ADC1D), 	//!< Pin ADC1.
	ADC_INPUT_ADC2		= _BV(ADC2D), 	//!< Pin ADC2.
	ADC_INPUT_ADC3		= _BV(ADC3D), 	//!< Pin ADC3.
	ADC_INPUT_ADC4		= _BV(ADC4D), 	//!< Pin ADC4.
	ADC_INPUT_ADC5		= _BV(ADC5D), 	//!< Pin ADC5.
	ADC_INPUT_ADC6		= _BV(ADC6D), 	//!< Pin ADC6.
	ADC_INPUT_ADC7		= _BV(ADC7D), 	//!< Pin ADC7.
	ADC_INPUT_ADC_ALL	= 0xff  	//!< Tout les Pins ADC.
}adc_input_e;

#endif

//_____ P R O T O T Y P E S ____________________________________________________

//_____ F U N C T I O N ________________________________________________________

//! \brief Pour sélectionner la tension de référence (Vref).
//! \param voltage est la tension de référence.
static inline void adc_voltage_select(adc_voltage_e voltage)
{
	ADMUX = (ADMUX&ADC_VOLTAGE_MASK)|voltage;
}


//! \brief Pour ajuster le résulta de la conversion à gauche.
//!
//! Ceci veux dire, que les 8 bits du pois le plus fore se retrouve dans l'octet du pois le plus fore du convertisseur (ADCH),
//! les 2 bits de pois faible restent se retrouve dans les bits de de pois les plus fort de l'octet du pois le plus faible du convertisseur (ADCL). \n
//! Ceci est utiles pour les conversions sur 8 bits.
//! \see adc_right_adjust_result()
//! \see adc_get_conversion_8bit();
static inline void adc_left_adjust_result()
{
	ADMUX |= _BV(ADLAR);
}

//! \brief Pour ajuster le résulta de la conversion à droit.
//!
//! Ceci veux dire, que les 8 bits du pois le plus faible se retrouve dans l'octet du pois le plus faible du convertisseur (ADCL),
//! les 2 bits de pois fort restent se retrouve dans les bits de de pois les plus faible de l'octet du pois le plus fort du convertisseur (ADCH). \n
//! Ceci est utiles pour les conversions sur 10 bits.
//! \note active par défaut.
//! \see  adc_left_adjust_result()
//! \see adc_get_conversion_10bit()
static inline void adc_right_adjust_result()
{
	ADMUX &= ~_BV(ADLAR);
}

//! \brief Pour sélectionner la tension à convertir.
//! \param channel est la tension de conversion.
static inline void adc_channel_select(adc_channel_e channel)
{
	ADMUX = (ADMUX&ADC_CHANNEL_MASK)|channel;
}

//! \brief Permet d'activer l'adc.
//! \see adc_disable()
static inline void adc_enable()
{
	ADCSRA |= _BV(ADEN);
}

//! \brief Permet de désactiver l'adc.
//! \see adc_enable()
static inline void adc_disable()
{
	ADCSRA &= ~_BV(ADEN);
}

//! \brief Pour lancer une conversion.
//! \note La première conversion est inviter a être jeter.
static inline void adc_start()
{
	ADCSRA |= _BV(ADSC);
}

//! \brief Permet d'activer une conversion automatique.
//!
//! \note La sélection de la source de déclenchement ce fait avec adc_trigger_select().
//! \see adc_disable_trigger()
//! \see adc_trigger_select()
static inline void adc_enable_trigger()
{
	ADCSRA |= _BV(ADATE);
}

//! \brief Permet de désactiver la conversion automatique (auto trigger).
//! \see adc_enable_trigger()
static inline void adc_disable_trigger()
{
	ADCSRA &= ~_BV(ADATE);
}

//! \brief Pour savoir si le drapeau de conversion est lever.
//!
//! Ceci indique que la conversion est finie.
//! \return false si le drapeau n'est pas lever.
//! \see adc_cleared_flag()
static inline bool adc_is_raising_flag()
{
	return bit_is_set(ADCSRA, ADIF);
}

//! \brief Pour effacer le drapeau de conversion.
//! \see adc_is_raising_flag()
static inline void adc_cleared_flag()
{
	ADCSRA |= _BV(ADIF);
}

//! \brief Permet de générer l'interruption, l'or ce que, une conversion est finie.
//!
//! A utiliser avec le vecteur d'interruption ADC_vect.
//! \code
//! ISR(ADC_vect)
//! {
//! 	//code
//! }
//! \endcode
//! \see adc_disable_interrupt()
static inline void adc_enable_interrupt()
{
	ADCSRA |= _BV(ADIE);
}

//! \brief Permet de désactiver l'interruption.
//! \see adc_enable_interrupt()
static inline void adc_disable_interrupt()
{
	ADCSRA &= ~_BV(ADIE);
}

//! \brief Pour sélectionner le pré-diviseur du convertisseur.
//! \note Une fréquence entre 50KHz et 200KHz peut être utiliser pour un conversion sur 10bits.
//! La fréquence peut être plus élever si la convection ne nécessite pas une résolution de 10bits.
//! \param prescaler est le pré-diviseur.
static inline void adc_prescaler_select(adc_prescaler_e prescaler)
{
	ADCSRA = (ADCSRA&ADC_PRESCALER_MASK)|prescaler;
}

//! \brief Permet d'obtenir la valeur de conversion sur 10 bits.
//!
//! Pour une conversion simple :
//! \f$ ADC =  \frac {Vin*1024}{Vref}\f$
//! - \b ADC est la valeur de conversion, (le résulta de cette fonction).
//! - \b Vin tentions de conversion, sélectionnable par adc_channel_select().
//! - \b Vref tension de référence, sélectionner par adc_voltage_select().
//!
//! Pour une conversion avec daux entrées :
//! \f$ ADC =  \frac {(Vpos-Vneg)*gain*512}{Vref}\f$
//! - \b ADC est la valeur de conversion, (le résulta de cette fonction).
//! - \b Vpos tentions positive de conversion, sélectionnable par adc_channel_select().
//! - \b Vneg tentions négative de conversion, sélectionnable par adc_channel_select().
//! - \b gain amplification de la comparaison (Vpos-Vneg), sélectionnable par adc_channel_select().
//! - \b Vref tension de référence, sélectionner par adc_voltage_select().
//!
//! \return La valeur de conversion.
//! \see adc_get_conversion_8bit()
//! \see adc_channel_select()
//! \see adc_voltage_select()
//! \see adc_right_adjust_result()
//! \see adc_channel_e
static inline uint16_t adc_get_conversion_10bit()
{
	return ADC;
}

//! \brief Permet d'obtenir la valeur de conversion sur 8 bit.
//!
//! Pour une conversion simple :
//! \f$ ADC =  \frac {Vin*256}{Vref}\f$
//! - \b ADC est la valeur de conversion, (le résulta de cette fonction).
//! - \b Vin tentions de conversion, sélectionnable par adc_channel_select().
//! - \b Vref tension de référence, sélectionner par adc_voltage_select().
//!
//! Pour une conversion avec daux entrées :
//! \f$ ADC =  \frac {(Vpos-Vneg)*gain*256}{Vref}\f$
//! - \b ADC est la valeur de conversion, (le résulta de cette fonction).
//! - \b Vpos tentions positive de conversion, sélectionnable par adc_channel_select().
//! - \b Vneg tentions négative de conversion, sélectionnable par adc_channel_select().
//! - \b gain amplification de la comparaison (Vpos-Vneg), sélectionnable par adc_channel_select().
//! - \b Vref tension de référence, sélectionner par adc_voltage_select().
//!
//! \note Pour une valeur correcte retourner, le résulta de la conversion doit être ajuster à gauche.
//!
//! \return La valeur de conversion.
//! \see adc_get_conversion_10bit()
//! \see adc_channel_select()
//! \see adc_voltage_select()
//! \see adc_left_adjust_result()
//! \see adc_channel_e
static inline uint8_t adc_get_conversion_8bit()
{
	return ADCH;
}

#if defined(ADCSRB) || defined(__DOXYGEN__)

//! \brief Permet d'activer le mode haut vitesse.
//! \attention Ce mode et valide que pour certain composent, veiller vérifier dans la datasheet de vautre composent si il existe (registre ADCSRB, bit ADHSM).
//! Si votre composent, ne dispose pas de se mode, il se peut que la compilation vous indique aucune erreur, mais vous ne disposerai pas de se mode pour autant.
//! \see adc_disable_high_speed()
static inline void adc_enable_high_speed()
{
	ADCSRB |= _BV(ADHSM);
}

//! \brief Permet de désactiver le mode haut vitesse.
//! \attention Ce mode et valide que pour certain composent, veiller vérifier dans la datasheet de vautre composent si il existe (registre ADCSRB, bit ADHSM).
//! Si votre composent, ne dispose pas de se mode, il se peut que la compilation vous indique aucune erreur, mais vous ne disposerai pas de se mode pour autant.
//! \see adc_enable_high_speed()
static inline void adc_disable_high_speed()
{
	ADCSRB &= ~_BV(ADHSM);
}

#endif

//! \brief Permet de sélectionner la source de la conversion automatique.
//! \param trigger est la source de déclenchement.
static inline void adc_trigger_select(adc_trigger_e trigger)
{
	#ifdef SFIOR
	SFIOR = (SFIOR&ADC_TRIGGER_MASK)|trigger;
	#endif
	#ifdef ADCSRB
	ADCSRB = (ADCSRB&ADC_TRIGGER_MASK)|trigger;
	#endif
}

#if defined(DIDR0) || defined(__DOXYGEN__)

//! \brief Permet, de désactiver les pins en entrée logique qui serve comme entrée analogique. Pour une diminution en consommation.
//! \note Peut être utiliser avec des ou logique.
//! \code
//! adc_disable_input(ADC_INPUT_ADC0|ADC_INPUT_ADC3|ADC_INPUT_ADC5);
//! \endcode
//! Tous sec bits sont activer comme entrer numérique par défaut.
//! \attention Cette fonction existe seulement si la fonctionnalité et supporter pare le composent.
//! \param input est le(s) pin(s) à désactiver.
//! \see adc_enable_input()
static inline void adc_disable_input(adc_input_e input)
{
	DIDR0 |= input;
}

//! \brief Permet, d'activer les pins en entrée logique qui ne serve pas comme entrée analogique.
//! \note Peut être utiliser avec des ou logique.
//! \code
//! adc_enable_input(ADC_INPUT_ADC0|ADC_INPUT_ADC3|ADC_INPUT_ADC5);
//! \endcode
//! Tous sec bits sont activer comme entrer numérique par défaut.
//! \attention Cette fonction existe seulement si la fonctionnalité et supporter pare le composent.
//! \param input est le(s) pin(s) à désactiver.
//! \see adc_disable_input()
static inline void adc_enable_input(adc_input_e input)
{
	DIDR0 &= ~input;
}

#endif

//! @} //Adc

#endif // ADC_H_INCLUDED
