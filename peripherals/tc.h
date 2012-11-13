/*Fichier conçu pour la génération de la documentation automatique avec
* Doxygen.
*/
//! \file **********************************************************************
//!
//! \brief Driveur pour les Timers/Counters.
//!
//! - Compilateur		: AVR-GCC
//! - Composant tester	: atmega8535, atmega644, at90usb1287 (Tout les fonctionnaliste n'on pas été tester)
//!
//!
//! \author Maleyrie Antoine
//! \version 1.3
//! \date 08 Juin 2011
//!
//! ****************************************************************************

/*
*	Copyright © 2011 - Antoine Maleyrie.
*/

#ifndef TC_H_INCLUDED
#define TC_H_INCLUDED

//! \addtogroup tc
//! \brief timers/counters.
//!
//! \code
//! #include <tc.h>
//! \endcode
//!
//! \todo Les composes supportent une PLL sur leur timer 1 ne sont pas encore supporter.
//!
//! \todo La sélection du quartz externe de 32KHz sur le timer 2 n'ais pas encore possible.
//!
//! Voici un code d'exemple sur le timer 1 (16bit).
//! \include tc/basic.c
//!
//! @{

//_____ I N C L U D E S ________________________________________________________
#include <stdbool.h>

//_____ M A C R O S ____________________________________________________________

//_____ E N U M ________________________________________________________________

//! \brief Valeur représentent le numéro du timer/counter.
//!
//! A utiliser avec \ref tc_init().
//! \note Si le composent ne dispose pas d'un ou plusieurs timer/counter la valeur de ce timer/counter ne sera pas défini.
//! \see tc_init()
typedef enum
{
	#if defined(TCNT0) || defined(__DOXYGEN__)
	TC_0,	//!< Valeur représentent le timer/counter 0.
	#endif
	#if defined(TCNT1) || defined(__DOXYGEN__)
	TC_1,	//!< Valeur représentent le timer/counter 1 (16bits).
	#endif
	#if defined(TCNT2) || defined(__DOXYGEN__)
	TC_2,	//!< Valeur représentent le timer/counter 2.
	#endif
	#if defined(TCNT3) || defined(__DOXYGEN__)
	TC_3,	//!< Valeur représentent le timer/counter 3 (16bits).
	#endif
	#if defined(TCNT4) || defined(__DOXYGEN__)
	TC_4,	//!< Valeur représentent le timer/counter 4 (16bits).
	#endif
	#if defined(TCNT5) || defined(__DOXYGEN__)
	TC_5,	//!< Valeur représentent le timer/counter 5 (16bits).
	#endif
}tc_s;

//! \brief Valeur représentent le nom du comparateur.
//!
//! \note Si le composent ne dispose pas d'un ou plusieurs comparateur la valeur de ce comparateur ne sera pas défini.
typedef enum
{
	TC_COMPARE_A				= 0x02,	//!< Valeur représentent le comparateur A.
	TC_COMPARE_B				= 0x04,	//!< Valeur représentent le comparateur B.
	#if defined(OCR1C) || defined(OCR3C) || defined(OCR4C) | defined(OCR5C) || defined(__DOXYGEN__)
	TC_COMPARE_C				= 0x08,	//!< Valeur représentent le comparateur C.
	#endif
	#if defined(OCR1D) || defined(__DOXYGEN__)
	TC_COMPARE_D				= 0x10,	//!< Valeur représentent le comparateur D. \todo pas encore supporter.
	#endif
}tc_compare_e;

//! \brief Valeur possible pour la sélection de l'horloge.
//!
//! A utiliser avec \ref tc_clock_select().
//!
//! \note Les valeurs finissent pas _RT2 sont \b uniquement réserver au timer 2.
//!
//! \see tc_clock_select()
typedef enum
{
	TC_CLOCK_NO 					= 0x00, //!< Pas d'horloge.
	TC_CLOCK_CLK_1 					= 0x01,	//!< Horloge du cpu diviser par 1.
	TC_CLOCK_CLK_8 					= 0x02,	//!< Horloge du cpu diviser par 8.
	TC_CLOCK_CLK_64 				= 0x03,	//!< Horloge du cpu diviser par 64.
	TC_CLOCK_CLK_256 				= 0x04,	//!< Horloge du cpu diviser par 256.
	TC_CLOCK_CLK_1024 				= 0x05,	//!< Horloge du cpu diviser par 1024.
	TC_CLOCK_EXTERNAL_FALLING_EDGE 	= 0x06,	//!< Horloge externe (pin Tn), sur front descendant, (n représentent le numéro du timer).
	TC_CLOCK_EXTERNAL_RISING_EDGE 	= 0x07,	//!< Horloge externe (pin Tn), sur front montent, (n représentent le numéro du timer).

	TC_CLOCK_NO_RT2 				= 0x00,	//!< Pas d'horloge, réserver au timer 2.
	TC_CLOCK_SLK_1_RT2 				= 0x01,	//!< Horloge du cpu diviser par 1, réserver au timer 2.
	TC_CLOCK_CLK_8_RT2 				= 0x02,	//!< Horloge du cpu diviser par 8, réserver au timer 2.
	TC_CLOCK_CLK_32_RT2 			= 0x03,	//!< Horloge du cpu diviser par 32, réserver au timer 2.
	TC_CLOCK_CLK_64_RT2 			= 0x04,	//!< Horloge du cpu diviser par 64, réserver au timer 2.
	TC_CLOCK_CLK_128_RT2 			= 0x05,	//!< Horloge du cpu diviser par 128, réserver au timer 2.
	TC_CLOCK_CLK_256_RT2 			= 0x06,	//!< Horloge du cpu diviser par 256, réserver au timer 2.
	TC_CLOCK_SLK_1024_RT2 			= 0x07,	//!< Horloge du cpu diviser par 1024, réserver au timer 2.

	#if !defined(__DOXYGEN__)
	TC_CLOCK_MASK 			= 0x07
	#endif
}tc_clock_e;

//! \brief Les différents modes.
//!
//! On peut distinguer deux grande famille de valeur. Les timer 0 et 2 sur 8 bit et les timer 1, 3, 4, 5 sur 16 bit.
//! Pour plus de renseignement, veiller consulter la documentation de votre composent.
//! A utiliser avec \ref tc_mode_select().
//!
//! \note Les dénomination OCn (timer ayant qu'un seul comparateur) et OCnX, représente les comparateurs, avec n représentent le numéro du timer et X représentent le nom des comparateurs.
//!
//! 5 type de mode possible :
//! \par Normale. \n
//! 	C'est le mode qui est configurée par défaut et le plus basique. Le(s) registre(s) de comparaison (OCn/OCnX) sont mis à jour immédiatement.
//! 	Le débordement du conteur un ci que le drapeau de débordement s'effectu au maximums (soit 0xff pour un timer de 8 bit ou 0xffff pour un timer de 16 bit).
//!
//! \par CTC (Clear Timer on Compare). \n
//!	Le(s) registre(s) de comparaison (OCn/OCnX) sont mis à jour immédiatement.
//!	Le débordement du conteur un ci que le drapeau de débordement s'effectu sur le registre de comparaison (OCn/OCnX) ou sur le registre de capture (ICRn). \n
//!	\f$ fOCnX = \frac {fclk_I/O}{2*N*(1+OCRnA)}\f$
//!	- \b fOCnX Fréquence du signale générai sur OCnX.
//!	- \b fclk_I/O fréquence d'entrée du timer, (aven le pré diviseur).
//!	- \b N Valeur du pré diviseur, ( \ref tc_clock_e ).
//!	- \b OCRnA ou ICRn valeur du comparateur.
//!
//! \par Fast PWM. \n
//!	Le(s) registre(s) de comparaison (OCn/OCnX) sont mis à jour a la valeur minimale (0) du timer.
//!	Le débordement du conteur un ci que le drapeau de débordement s'effectu sur la valeur maximale du timer. \n
//!	\f$ RFPWM = \frac {log(TOP+1)}{log(2)}\f$
//!	- \b RFPWM Résolution de la pwm générer.
//!	- \b TOP Valeur maximale du timer \n
//!	\f$ fOCnXPWM = \frac {fclk_I/O}{N*(1+TOP)}\f$
//!	- \b fOCnXPWM Fréquence du signale générai sur OCnX.
//!	- \b fclk_I/O fréquence d'entrée du timer, (aven le pré diviseur).
//!	- \b N Valeur du pré diviseur, ( \ref tc_clock_e ).
//!	- \b TOP Valeur maximale du timer.
//!
//! \par PWM, Phase Correct. \n
//!	Le(s) registre(s) de comparaison (OCn/OCnX) sont mis à jour à la valeur minimale (0) du timer.
//!	Le débordement du conteur s'effectu sur la valeur maximale du timer, le drapeau de débordement est lever à la valeur minimale tu timer (0) \n
//! 	\f$ RPCPWM = \frac {log(TOP+1)}{log(2)}\f$
//! 	- \b RPCPWM Résolution de la pwm générer.
//! 	- \b TOP Valeur maximale du timer \n
//! 	\f$ fOCnXPCPWM = \frac {fclk_I/O}{2*N*TOP}\f$
//! 	- \b fOCnXPCPWM  Fréquence du signale générai sur OCnX.
//! 	- \b fclk_I/O fréquence d'entrée du timer, (aven le pré diviseur).
//! 	- \b N Valeur du pré diviseur, ( \ref tc_clock_e ).
//! 	- \b TOP Valeur maximale du timer.
//!
//! \par PWM, Phase and Frequency Correct. \n
//!	Le(s) registre(s) de comparaison (OCn/OCnX) sont mis à jour à la valeur minimale (0) du timer.
//!	Le débordement du conteur un ci que le drapeau de débordement s'effectu sur la valeur minimale (0) du timer. \n
//!	\f$ RPFCPWM = \frac {log(TOP+1)}{log(2)}\f$
//!	- \b RFPWM Résolution de la pwm générer.
//!	- \b TOP Valeur maximale du timer \n
//!	\f$ fOCnXPFCPWM = \frac {fclk_I/O}{2*N*TOP}\f$
//!	- \b fOCnXPFCPWM Fréquence du signale générai sur OCnX.
//!	- \b fclk_I/O fréquence d'entrée du timer, (aven le pré diviseur).
//!	- \b Valeur du pré diviseur, ( \ref tc_clock_e ).
//!	- \b TOP Valeur maximale du timer.
//!
//! \see tc_mode_select()
//! \see tc_output_select()
//! \see tc_output_e
//! \todo l'explication est à fignoler, pour le moment le plus simple est de regarder la doc du composent.
typedef enum
{
	#if (defined(TCCR0) || defined(TCCR2)) && !defined(__DOXYGEN__) //Timer 1 et 2 avec un comparateur.
	TC_MODE_NORMAL_RT02  				= 0x00,	//!< Mode normale. Avec comme valeur maximale 0xff.<b>Réserver au timer 0 et 2</b>.
	TC_MODE_PWM_PHASE_CORRECT_RT02		= 0x40,	//!< Mode PWM, Phase Correct. Avec comme valeur maximale, 0xff.<b>Réserver au timer 0 et 2</b>.
	TC_MODE_CTC_OCRA_RT02				= 0x08,	//!< Mode CTC. Avec comme valeur maximale, le registre de compareson A, (OCRnA/OCRn).<b>Réserver au timer 0 et 2</b>.
	TC_MODE_FAST_PWM_RT02				= 0x48,	//!< Mode Fast PWM. Avec comme valeur maximale, 0xff.<b>Réserver au timer 0 et 2</b>.
	#else
	TC_MODE_NORMAL_RT02  				= 0x00,	//!< Mode normale. Avec comme valeur maximale 0xff.<b>Réserver au timer 0 et 2</b>.
	TC_MODE_PWM_PHASE_CORRECT_RT02		= 0x01,	//!< Mode PWM, Phase Correct. Avec comme valeur maximale, 0xff.<b>Réserver au timer 0 et 2</b>.
	TC_MODE_CTC_OCRA_RT02				= 0x02,	//!< Mode CTC. Avec comme valeur maximale, le registre de compareson A, (OCRnA/OCRn).<b>Réserver au timer 0 et 2</b>.
	TC_MODE_FAST_PWM_RT02				= 0x03,	//!< Mode Fast PWM. Avec comme valeur maximale, 0xff.<b>Réserver au timer 0 et 2</b>.
	TC_MODE_PWM_PHASE_CORRECT_OCRA_RT02	= 0x09,	//!< Mode PWM, Phase Correct. Avec comme valeur maximale, le registre de compareson A, (OCRnA).<b>Réserver au timer 0 et 2</b>. \note Valeur possible, si il y a au moins 2 comparateur sur le timer.
	TC_MODE_FAST_PWM_OCRA_RT02			= 0x0b,	//!< Mode Fast PWM. Avec comme valeur maximale, le registre de compareson A, (OCRnA).<b>Réserver au timer 0 et 2</b>. \note Valeur possible, si il y a au moins 2 comparateur sur le timer.
	#endif


	TC_MODE_NORMAL  					= 0x00, //!< Mode normale. Avec comme valeur maximale 0xffff.
	TC_MODE_CTC_OCRA					= 0x08, //!< Mode CTC. Avec comme valeur maximale, le registre de compareson A, (OCRnA).
	TC_MODE_CTC_ICR						= 0x18, //!< Mode CTC. Avec comme valeur maximale, le registre de capture, (ICRn).

	TC_MODE_PWM_PHASE_CORRECT_8BIT		= 0x01, //!< Mode PWM, Phase Correct. Timer sur 8 bit, avec comme valeur maximale, 0x00ff.
	TC_MODE_PWM_PHASE_CORRECT_9BIT 		= 0x02, //!< Mode PWM, Phase Correct. Timer sur 9 bit, avec comme valeur maximale, 0x01ff.
	TC_MODE_PWM_PHASE_CORRECT_10BIT		= 0x03, //!< Mode PWM, Phase Correct. Timer sur 10 bit, avec comme valeur maximale, 0x03ff.
	TC_MODE_PWM_PHASE_CORRECT_OCRA		= 0x13, //!< Mode PWM, Phase Correct. Avec comme valeur maximale, le registre de compareson A, (OCRnA).
	TC_MODE_PWM_PHASE_CORRECT_ICR		= 0x12, //!< Mode PWM, Phase Correct. Avec comme valeur maximale, le registre de capture, (ICRn).


	TC_MODE_FAST_PWM_8BIT 				= 0x09, //!< Mode Fast PWM. Timer sur 8 bit, avec comme valeur maximale, 0x00ff.
	TC_MODE_FAST_PWM_9BIT				= 0x0a, //!< Mode Fast PWM. Timer sur 9 bit, avec comme valeur maximale, 0x01ff.
	TC_MODE_FAST_PWM_10BIT				= 0x0b, //!< Mode Fast PWMt. Timer sur 10 bit, avec comme valeur maximale, 0x03ff.
	TC_MODE_FAST_PWM_OCRA				= 0x1b, //!< Mode Fast PWM. Avec comme valeur maximale, le registre de compareson A, (OCRnA).
	TC_MODE_FAST_PWM_ICR				= 0x1a, //!< Mode Fast PWM. Avec comme valeur maximale, le registre de capture, (ICRn).

	TC_MODE_PWM_PHASE_AND_FREQUENCY_CORRECT_OCRA 	= 0x10, //!< Mode PWM, Phase and Frequency Correct. Avec comme valeur maximale, le registre de compareson A, (OCRnA).
	TC_MODE_PWM_PHASE_AND_FREQUENCY_CORRECT_ICR		= 0x11, //!< Mode PWM, Phase and Frequency Correct, le registre de capture, (ICRn).

	#if !defined(__DOXYGEN__)
	#if defined(TCCR0) || defined(TCCR2) //Timer 1 et 2 avec un comparateur.
	TC_MODE_MASK_TCCR					= 0xb7,
	#else //Timer 1 et 2 avec deux comparateurs.
	TC_MODE_MASK_TCCR12_B				= 0xf7,
	#endif
	TC_MODE_MASK_TCCR_A					= 0xfc,
	TC_MODE_MASK_TCCR_B					= 0xe7
	#endif
}tc_mode_e;

//! \brief Les différents mode de sortie.
//!
//! Ceci va pouvoirs, définir le comportement des sortirs, OCn (timer ayant qu'un seul comparateur), OCnX (n représentent le numéro du timer et X, ce se sont les nom des comparateurs).
//!
//! Selon le mode choisi avec \ref tc_mode_select() les différents valeurs énumérer ici vont avoir une légère différence (sauf pour la valeur TC_OUTPUT_NORMAL).
//! - Si vous avez choisie le mode \b Normale ou \b CTC (non-PWM).
//! 	- TC_OUTPUT_TOGGLE : A chaque comparaison égale, les sortis change d'état.
//! 	- TC_OUTPUT_CLEAR : Mise à 0 de la sorti a chaque comparaison égale.
//! 	- TC_OUTPUT_SET	: Mise à 1 de la sorti a chaque comparaison égale.
//! - Si vous avez choisie un mode <b>PWM Fast</b>.
//! 	- TC_OUTPUT_TOGGLE : A chaque comparaison égale, la sorti OCnA/OCn, change d'état. Les autres sorti, si il en existe, sont déconnecter du timer/counter.
//! 	- TC_OUTPUT_CLEAR : Mise à 0 de la sorti a chaque comparaison égale. Mise à 1 de la sorti a chaque débordement du compteur.
//! 	- TC_OUTPUT_SET : Mise à 1 de la sorti a chaque comparaison égale. Mise à 0 de la sorti a chaque débordement du compteur.
//! - Si vous avez choisie un mode <b>PWM, Phase Correct</b> ou <b>PWM, Phase and Frequency Correct</b>.
//! 	- TC_OUTPUT_TOGGLE : A chaque comparaison égale, la sorti OCnA/OCn, change d'état. Les autres sorti, si il en existe, sont déconnecter du timer/counter.
//! 	- TC_OUTPUT_CLEAR : Mise à 0 de la sorti a chaque comparaison égale quand le compteur compte. Mise à 1 de la sorti a chaque comparaison égale quand le compteur décompte.
//! 	- TC_OUTPUT_SET : Mise à 1 de la sorti a chaque comparaison égale quand le compteur compte. Mise à 0 de la sorti a chaque comparaison égale quand le compteur décompte.
//!
//! \see tc_output_select()
//! \see tc_mode_select()
//! \see tc_mode_e
typedef enum
{
	TC_OUTPUT_NORMAL  				= 0x00,	//!< Fonctionnement normale de la sorti, la sorti est déconnecter du timer/counter.
	TC_OUTPUT_TOGGLE  				= 0x54,	//!< A chaque comparaison égale, la sorti change d'état.
	TC_OUTPUT_CLEAR  				= 0xa8,	//!< Mise à 0 de la sorti a chaque comparaison égale.
	TC_OUTPUT_SET	  				= 0xfc,	//!< Mise à 1 de la sorti a chaque comparaison égale.

	#if !defined(__DOXYGEN__)
	TC_OUTPUT_A_MASK				= 0x3f,
	TC_OUTPUT_B_MASK				= 0xcf,
	#if defined(OCR1C) || defined(OCR3C) || defined(OCR4C) || defined(OCR5C)
	TC_OUTPUT_C_MASK				= 0xf3
	#endif
	#endif
}tc_output_e;

//! \brief Les différents front possible pour déclencher le comptage.
//!
//! A utiliser avec \ref tc_edge_select().
//! \see tc_edge_select()
typedef enum
{
	TC_EDGE_FALLING	= 0x00, 	//!< Sur front descendant.
	TC_EDGE_RISING	= 0x40, 	//!< Sur front montent.
	#if !defined(__DOXYGEN__)
	TC_EDGE_MASK 	= 0x40
	#endif
}tc_edge_e;

//_____ S T R U C T ____________________________________________________________

//_____ P R O T O T Y P E S ____________________________________________________

//_____ F U N C T I O N ________________________________________________________

//! \brief Pour de sélectionner l'horloge.
//! \param tc est le timer/counter que vous voulez modifier.
//! \param clock est la sélection de la source de l'horloge pour le compteur.
static inline void tc_clock_select(tc_s tc, tc_clock_e clock)
{
	switch(tc)
	{
		#if defined(TCNT0)
		case TC_0:	//timer/counter 0.
			#if defined(TCCR0)
			TCCR0 |= clock;
			#elif defined(TCCR0B)
			TCCR0B |= clock;
			#elif defined(TCCR0A) != defined(TCCR0B)
			TCCR0A |= clock;
			#endif
		break;
		#endif
		
		#if defined(TCNT1)
		case TC_1:	//timer/counter 1 (16bits).
			TCCR1B |= clock;
		break;
		#endif
		
		#if defined(TCNT2)
		case TC_2:	//timer/counter 2.
			#if defined(TCCR2)
			TCCR2 |= clock;
			#elif defined(TCCR2B)
			TCCR2B |= clock;
			#elif defined(TCCR2A) != defined(TCCR2B)
			TCCR2A |= clock;
			#endif
		break;
		#endif
		
		#if defined(TCNT3)
		case TC_3:	//timer/counter 3 (16bits).
			TCCR3B |= clock;
		break;
		#endif
		
		#if defined(TCNT4)
		case TC_4:	//timer/counter 4 (16bits).
			TCCR4B |= clock;
		break;
		#endif
		
		#if defined(TCNT5)
		case TC_5:	//timer/counter 5 (16bits).
			TCCR5B |= clock;
		break;
		#endif
	}
}

//! \brief Pour sélectionner le mode de fonctionement.
//! \param tc est le timer/counter que vous voulez modifier.
//! \param mode est le mode souhaiter.
static inline void tc_mode_select(tc_s tc, tc_mode_e mode)
{
	//if(tc->num == TC_NUM_0 || tc->num == TC_NUM_2)
	//{
		//#if defined(TCCR0) || defined(TCCR2)//Timer 1 et 2 avec un comparateur.
		//*tc->tccrb = (*tc->tccrb&TC_MODE_MASK_TCCR)|(~TC_MODE_MASK_TCCR&mode);
		//#else //Timer 1 et 2 avec deux comparateurs.
		//*tc->tccra = (*tc->tccra&TC_MODE_MASK_TCCR_A)|(~TC_MODE_MASK_TCCR_A&mode);
		//*tc->tccrb = (*tc->tccrb&TC_MODE_MASK_TCCR12_B)|(~TC_MODE_MASK_TCCR12_B&mode);
		//#endif
	//}
	//else
	//{
		//*tc->tccra = (*tc->tccra&TC_MODE_MASK_TCCR_A)|(~TC_MODE_MASK_TCCR_A&mode);
		//*tc->tccrb = (*tc->tccrb&TC_MODE_MASK_TCCR_B)|(~TC_MODE_MASK_TCCR_B&mode);
	//}
}

//! \brief Permet de sélectionner le mode de sorti (OCRnX).
//! \note Pour les timer disposent que d'un seul comparateur la valeur passer à \p compare n'est pas pris en compte.
//! \param tc est le timer/counter que vous voulez modifier.
//! \param compare est le comparateur affecter par l'opération de cette fonction.
//! \param output est la nouvelle valeur du mode de sortie.
static inline void tc_output_select(tc_s tc, tc_compare_e compare, tc_output_e output)
{
	//#if defined(TCCR0) || defined(TCCR2)
	//if(tc->num == TC_NUM_0 || tc->num == TC_NUM_2)
	//{
		//*tc->tccrb = (*tc->tccrb&TC_OUTPUT_B_MASK)|(~TC_OUTPUT_B_MASK&output);
	//}
	//else
	//{
	//#endif
		//switch(compare)
		//{
			//case TC_COMPARE_A:
				//*tc->tccra = (*tc->tccra&TC_OUTPUT_A_MASK)|(~TC_OUTPUT_A_MASK&output);
			//break;
			//case TC_COMPARE_B:
				//*tc->tccra = (*tc->tccra&TC_OUTPUT_B_MASK)|(~TC_OUTPUT_B_MASK&output);
			//break;
			//#if defined(OCR1C) || defined(OCR3C) || defined(OCR4C) | defined(OCR5C)
			//case TC_COMPARE_C:
				//*tc->tccra = (*tc->tccra&TC_OUTPUT_C_MASK)|(~TC_OUTPUT_C_MASK&output);
			//break;
			//#endif
		//}
	//#if defined(TCCR0) || defined(TCCR2)
	//}
	//#endif
}


//! @name Overflow
//!
//! Fonction lier au débordement des timers (overflow).
//! @{

//! \brief Active l'interruption de débordement.
//!
//! A utiliser avec le vecteur d'interruption TIMERn_OVF_vect, (n représentent le numéro du timer).
//!
//! \code
//! //Exemple pour le timer 0.
//! ISR(TIMER0_OVF_vect)
//! {
//! 	//code
//! }
//! \endcode
//!
//! \note Certain composent ne dispose pas des vecteurs d'interruption décri si dessus. Les autre vecteur possible sont décri si dessous.
//!
//! \par TIM0_OVF_vect
//! Le vecteur d'interruption TIM0_OVF_vect, est utiliser par les composants suivent : \n
//! ATtiny13, ATtiny43U, ATtiny24, ATtiny44, ATtiny84, ATtiny45, ATtiny25, ATtiny85
//!
//! \par TIM1_OVF_vect
//! Le vecteur d'interruption TIM1_OVF_vect, est utiliser par les composants suivent : \n
//! ATtiny24, ATtiny44, ATtiny84, ATtiny45, ATtiny25, ATtiny85
//!
//! \par TIMER0_OVF0_vect
//! Le vecteur d'interruption TIMER0_OVF0_vect, est utiliser par les composants suivent : \n
//! AT90S2313, AT90S2323, AT90S2343, ATtiny22, ATtiny26
//!
//! \par TIMER1_OVF1_vect
//! Le vecteur d'interruption TIMER1_OVF1_vect, est utiliser par les composants suivent : \n
//! AT90S2313, ATtiny26
//!
//! \param tc est le timer/counter que vous voulez modifier.
//! \see tc_disable_interrupt_overflow()
static inline void tc_enable_interrupt_overflow(tc_s tc)
{	
	switch(tc)
	{
		#if defined(TCNT0)
		case TC_0:	//timer/counter 0.
			#if defined(TIMSK)
			TIMSK |= _BV(TOIE0);
			#elif defined(TIMSK0)
			TIMSK0 |= _BV(TOIE0);
			#endif
		break;
		#endif
		
		#if defined(TCNT1)
		case TC_1:	//timer/counter 1 (16bits).
			#if defined(TIMSK)
			TIMSK |= _BV(TOIE1);
			#elif defined(TIMSK1)
			TIMSK1 |= _BV(TOIE1);
			#endif
		break;
		#endif
		
		#if defined(TCNT2)
		case TC_2:	//timer/counter 2.
			#if defined(TIMSK)
			TIMSK |= _BV(TOIE2);
			#elif defined(TIMSK2)
			TIMSK2 |= _BV(TOIE2);
			#endif
		break;
		#endif
		
		#if defined(TCNT3)
		case TC_3:	//timer/counter 3 (16bits).
			TIMSK3 |= _BV(TOIE3);
		break;
		#endif
		
		#if defined(TCNT4)
		case TC_4:	//timer/counter 4 (16bits).
			TIMSK4 |= _BV(TOIE4);
		break;
		#endif
		
		#if defined(TCNT5)
		case TC_5:	//timer/counter 5 (16bits).
			TIMSK5 |= _BV(TOIE5);
		break;
		#endif
	}
}

//! \brief Désactive l'interruption de débordement.
//! \param tc est le timer/counter que vous voulez modifier.
//! \see tc_enable_interrupt_overflow()
static inline void tc_disable_interrupt_overflow(tc_s tc)
{
	switch(tc)
	{
		#if defined(TCNT0)
		case TC_0:	//timer/counter 0.
			#if defined(TIMSK)
			TIMSK &= ~_BV(TOIE0);
			#elif defined(TIMSK0)
			TIMSK0 &= ~_BV(TOIE0);
			#endif
		break;
		#endif
		
		#if defined(TCNT1)
		case TC_1:	//timer/counter 1 (16bits).
			#if defined(TIMSK)
			TIMSK &= ~_BV(TOIE1);
			#elif defined(TIMSK1)
			TIMSK1 &= ~_BV(TOIE1);
			#endif
		break;
		#endif
		
		#if defined(TCNT2)
		case TC_2:	//timer/counter 2.
			#if defined(TIMSK)
			TIMSK &= ~_BV(TOIE2);
			#elif defined(TIMSK2)
			TIMSK2 &= ~_BV(TOIE2);
			#endif
		break;
		#endif
		
		#if defined(TCNT3)
		case TC_3:	//timer/counter 3 (16bits).
			TIMSK3 &= ~_BV(TOIE3);
		break;
		#endif
		
		#if defined(TCNT4)
		case TC_4:	//timer/counter 4 (16bits).
			TIMSK4 &= ~_BV(TOIE4);
		break;
		#endif
		
		#if defined(TCNT5)
		case TC_5:	//timer/counter 5 (16bits).
			TIMSK5 &= ~_BV(TOIE5);
		break;
		#endif
	}
}

//! \brief Pour savoir si le drapeau de débordement est lever.
//! \param tc est le timer/counter que vous voulez modifier.
//! \return false si le drapeau n'est pas lever.
//! \see tc_cleared_overflow_flag()
static inline bool tc_is_raising_overflow_flag(tc_s tc)
{
	switch(tc)
	{
		#if defined(TCNT0)
		case TC_0:	//timer/counter 0.
			#if defined(TIFR)
			return TIFR&_BV(TOV0);
			#elif defined(TIFR0)
			return TIFR0&_BV(TOV0);
			#endif
		break;
		#endif
		
		#if defined(TCNT1)
		case TC_1:	//timer/counter 1 (16bits).
			#if defined(TIFR)
			return TIFR&_BV(TOV1);
			#elif defined(TIFR1)
			return TIFR1&_BV(TOV1);
			#endif
		break;
		#endif
		
		#if defined(TCNT2)
		case TC_2:	//timer/counter 2.
			#if defined(TIFR)
			return TIFR&_BV(TOV2);
			#elif defined(TIFR2)
			return TIFR2&_BV(TOV2);
			#endif
		break;
		#endif
		
		#if defined(TCNT3)
		case TC_3:	//timer/counter 3 (16bits).
		return TIFR3&_BV(TOV3);
		break;
		#endif
		
		#if defined(TCNT4)
		case TC_4:	//timer/counter 4 (16bits).
		return TIFR4&_BV(TOV4);
		break;
		#endif
		
		#if defined(TCNT5)
		case TC_5:	//timer/counter 5 (16bits).
		return TIFR5&_BV(TOV5);
		break;
		#endif
	}
}

//! \brief Pour effacer le drapeau de débordement.
//! \param tc est le timer/counter que vous voulez modifier.
//! \see tc_is_raising_overflow_flag()
static inline void tc_cleared_overflow_flag(tc_s tc)
{
	switch(tc)
	{
		#if defined(TCNT0)
		case TC_0:	//timer/counter 0.
			#if defined(TIFR)
			TIFR |= _BV(TOV0);
			#elif defined(TIFR0)
			TIFR0 |= _BV(TOV0);
			#endif
		break;
		#endif
		
		#if defined(TCNT1)
		case TC_1:	//timer/counter 1 (16bits).
			#if defined(TIFR)
			TIFR |= _BV(TOV1);
			#elif defined(TIFR1)
			TIFR1 |= _BV(TOV1);
			#endif
		break;
		#endif
		
		#if defined(TCNT2)
		case TC_2:	//timer/counter 2.
			#if defined(TIFR)
			TIFR |= _BV(TOV2);
			#elif defined(TIFR2)
			TIFR2 |= _BV(TOV2);
			#endif
		break;
		#endif
		
		#if defined(TCNT3)
		case TC_3:	//timer/counter 3 (16bits).
		TIFR3 |= _BV(TOV3);
		break;
		#endif
		
		#if defined(TCNT4)
		case TC_4:	//timer/counter 4 (16bits).
		TIFR4 |= _BV(TOV4);
		break;
		#endif
		
		#if defined(TCNT5)
		case TC_5:	//timer/counter 5 (16bits).
		TIFR5 |= _BV(TOV5);
		break;
		#endif
	}
}

//! @} //overflow

//! @name Counter
//!
//! Fonction lier aux compteurs des timers (counter).
//! @{

//! \brief Permet de modifier la valeur des compteurs de 8 bits, (timer/counter 0, 2).
//! \param tc est le timer/counter que vous voulez modifier.
//! \param val est la nouvelle valeur du compteur.
//! \see tc_get_counter_8bit()
//! \see tc_set_counter_16bit()
//! \see tc_get_counter_16bit()
static inline void tc_set_counter_8bit(tc_s tc, uint8_t val)
{
	switch(tc)
	{
		#if defined(TCNT0)
		case TC_0:	//timer/counter 0.
			TCNT0 = val;
		break;
		#endif
		
		#if defined(TCNT2)
		case TC_2:	//timer/counter 2.
			TCNT2 = val;
		break;
		#endif
	}
}

//! \brief Permet d'obtenir la valeur des compteurs de 8 bits, (timer/counter 0, 2).
//! \param tc est le timer/counter que vous voulez modifier.
//! \return La valeur du compteur.
//! \see tc_set_counter_8bit()
//! \see tc_set_counter_16bit()
//! \see tc_get_counter_16bit()
static inline uint8_t tc_get_counter_8bit(tc_s tc)
{
	//Sauvegarde du drapeau d'interruption globale.
	uint8_t sreg = SREG;
	//Désactive les interruptions
	cli();
	
	switch(tc)
	{
		#if defined(TCNT0)
		case TC_0:	//timer/counter 0.
			return TCNT0;
		break;
		#endif
		
		#if defined(TCNT2)
		case TC_2:	//timer/counter 2.
			return TCNT2;
		break;
		#endif
	}
	
	//Restaure du drapeau d'interruption globale.
	SREG = sreg;
}

//! \brief Permet de modifier la valeur des compteurs de 16 bits, (timer/counter 1, 3, 4 et 5).
//! \param tc est la structure du timer/counter.
//! \param tc est le timer/counter que vous voulez modifier.
//! \see tc_set_counter_8bit()
//! \see tc_get_counter_8bit()
//! \see tc_get_counter_16bit()
static inline void tc_set_counter_16bit(tc_s tc, uint16_t val)
{
	//Sauvegarde du drapeau d'interruption globale.
	uint8_t sreg = SREG;
	//Désactive les interruptions
	cli();
	
	switch(tc)
	{		
		#if defined(TCNT1)
		case TC_1:	//timer/counter 1 (16bits).
			TCNT1 = val;
		break;
		#endif
		
		#if defined(TCNT3)
		case TC_3:	//timer/counter 3 (16bits).
			TCNT3 = val;
		break;
		#endif
		
		#if defined(TCNT4)
		case TC_4:	//timer/counter 4 (16bits).
			TCNT4 = val;
		break;
		#endif
		
		#if defined(TCNT5)
		case TC_5:	//timer/counter 5 (16bits).
			TCNT5 = val;
		break;
		#endif
	}
	
	//Restaure du drapeau d'interruption globale.
	SREG = sreg;
}

//! \brief Permet d'obtenir la valeur des compteurs de 16 bits, (timer/counter 1, 3, 4 et 5).
//! \param tc est le timer/counter que vous voulez modifier.
//! \return La valeur du compteur.
//! \see tc_set_counter_8bit()
//! \see tc_get_counter_8bit()
//! \see tc_get_counter_16bit()
static inline uint16_t tc_get_counter_16bit(tc_s tc)
{
	switch(tc)
	{		
		#if defined(TCNT1)
		case TC_1:	//timer/counter 1 (16bits).
			return TCNT1;
		break;
		#endif
		
		#if defined(TCNT3)
		case TC_3:	//timer/counter 3 (16bits).
			return TCNT3;
		break;
		#endif
		
		#if defined(TCNT4)
		case TC_4:	//timer/counter 4 (16bits).
			return TCNT4;
		break;
		#endif
		
		#if defined(TCNT5)
		case TC_5:	//timer/counter 5 (16bits).
			return TCNT5;
		break;
		#endif
	}
}

//! @} //Counter


//! @name Compare
//!
//! Fonction lier aux comparateurs des timers (compare).
//! @{


//! \brief Active l'interruption de comparaison.
//!
//! A utiliser avec le vecteur d'interruption TIMERn_COMPX_vect, (n représentent le numéro du timer et X le nom du comparateur).
//!
//! \note Pour les timers contenant qu'un seul registre de comparaison, la valeur donnée à \p compare n'est pas prise en compte.
//!
//! \code
//! //Exemple pour le comparateur C du timer 4.
//! ISR(TIMER4_COMPC_vect)
//! {
//! 	//code
//! }
//! \endcode
//!
//! \note Certain composent ne dispose pas des vecteurs d'interruption décri si dessus. Les autre vecteur possible sont décri si dessous.
//!
//! \par TIM0_COMPA_vect
//! Le vecteur d'interruption TIM0_COMPA_vect, est utiliser par les composants suivent : \n
//! ATtiny13, ATtiny43U, ATtiny24, ATtiny44, ATtiny84, ATtiny45, ATtiny25, ATtiny85
//!
//! \par TIM0_COMPB_vect
//! Le vecteur d'interruption TIM0_COMPB_vect, est utiliser par les composants suivent : \n
//! ATtiny13, ATtiny43U, ATtiny24, ATtiny44, ATtiny84, ATtiny45, ATtiny25, ATtiny85
//!
//! \par TIM1_COMPA_vect
//! Le vecteur d'interruption TIM1_COMPA_vect, est utiliser par les composants suivent : \n
//! ATtiny24, ATtiny44, ATtiny84, ATtiny45, ATtiny25, ATtiny85
//!
//! \par TIM1_COMPB_vect
//! Le vecteur d'interruption TIM1_COMPB_vect, est utiliser par les composants suivent : \n
//! ATtiny24, ATtiny44, ATtiny84, ATtiny45, ATtiny25, ATtiny85
//!
//! \par TIMER1_CMPA_vect
//! Le vecteur d'interruption TIMER1_CMPA_vect, est utiliser par les composants suivent : \n
//! ATtiny26
//!
//! \par TIMER1_CMPB_vect
//! Le vecteur d'interruption TIMER1_CMPB_vect, est utiliser par les composants suivent : \n
//! ATtiny26
//!
//! \par TIMER1_COMP1_vect
//! Le vecteur d'interruption TIMER1_COMP1_vect, est utiliser par les composants suivent : \n
//! AT90S2313
//!
//! \par TIMER0_COMP_vect
//! Le vecteur d'interruption TIMER0_COMP_vect, est utiliser par les composants suivent : \n
//! AT90CAN128, AT90CAN32, AT90CAN64, ATmega103, ATmega128, ATmega16, ATmega161, ATmega162, ATmega165, ATmega165P, ATmega169, ATmega169P, ATmega32, ATmega323, ATmega325, ATmega3250, ATmega3250P, ATmega329, ATmega3290, ATmega3290P, ATmega64, ATmega645, ATmega6450, ATmega649, ATmega6490, ATmega8515, ATmega8535.
//!
//! \par TIMER1_COMP_vect
//! Le vecteur d'interruption TIMER1_COMP_vect, est utiliser par les composants suivent : \n
//! AT90S2333, AT90S4433, ATtiny15.
//!
//! \par TIMER2_COMP_vect
//! Le vecteur d'interruption TIMER2_COMP_vect, est utiliser par les composants suivent : \n
//! AT90S4434, AT90S8535, AT90CAN128, AT90CAN32, AT90CAN64, ATmega103, ATmega128, ATmega16, ATmega161, ATmega162, ATmega163, ATmega165, ATmega165P, ATmega169, ATmega169P, ATmega32, ATmega323, ATmega325, ATmega3250, ATmega3250P, ATmega329, ATmega3290, ATmega3290P, ATmega64, ATmega645, ATmega6450, ATmega649, ATmega6490, ATmega8, ATmega8535.
//!
//! \par TIMER0_COMP_A_vect
//! Le vecteur d'interruption TIMER0_COMP_A_vect, est utiliser par les composants suivent : \n
//! AT90PWM3, AT90PWM2, AT90PWM1.
//!
//! \param tc est le timer/counter que vous voulez modifier.
//! \param compare est le comparateur affecter par l'opération de cette fonction.
//! \see tc_disable_interrupt_compare()
static inline void tc_enable_interrupt_compare(tc_s tc, tc_compare_e compare)
{
	switch(tc)
	{
		#if defined(TCNT0)
		case TC_0:	//timer/counter 0.
			#if defined(TIMSK)
			TIMSK |= _BV(OCIE0);
			#elif defined(TIMSK0)
			TIMSK0 |= compare;
			#endif
		break;
		#endif
		
		#if defined(TCNT1)
		case TC_1:	//timer/counter 1 (16bits).
			#if defined(TIMSK)
			switch(compare)
			{
				case TC_COMPARE_A:
					TIMSK |= _BV(OCIE1A);
				break;
				case TC_COMPARE_B:
					TIMSK |= _BV(OCIE1B);
				break;
			}
			#elif defined(TIMSK1)
			TIMSK1 |= compare;
			#endif
		break;
		#endif
		
		#if defined(TCNT2)
		case TC_2:	//timer/counter 2.
			#if defined(TIMSK)
			TIMSK |= _BV(OCIE2);
			#elif defined(TIMSK2)
			TIMSK2 |= compare;
			#endif
		break;
		#endif
		
		#if defined(TCNT3)
		case TC_3:	//timer/counter 3 (16bits).
			TIMSK3 |= compare;
		break;
		#endif
		
		#if defined(TCNT4)
		case TC_4:	//timer/counter 4 (16bits).
			TIMSK4 |= compare;
		break;
		#endif
		
		#if defined(TCNT5)
		case TC_5:	//timer/counter 5 (16bits).
			TIMSK5 |= compare;
		break;
		#endif
	}
}

//! \brief Désactive l'interruption de comparaison.
//! \note Pour les timers contenant qu'un seul registre de comparaison, la valeur donnée à \p compare n'est pas prise en compte.
//! \param tc est le timer/counter que vous voulez modifier.
//! \param compare est le comparateur affecter par l'opération de cette fonction.
//! \see tc_enable_interrupt_compare()
static inline void tc_disable_interrupt_compare(tc_s tc, tc_compare_e compare)
{
	switch(tc)
	{
		#if defined(TCNT0)
		case TC_0:	//timer/counter 0.
			#if defined(TIMSK)
			TIMSK &= ~_BV(OCIE0);
			#elif defined(TIMSK0)
			TIMSK0 &= ~compare;
			#endif
		break;
		#endif
		
		#if defined(TCNT1)
		case TC_1:	//timer/counter 1 (16bits).
			#if defined(TIMSK)
			switch(compare)
			{
				case TC_COMPARE_A:
					TIMSK &= ~_BV(OCIE1A);
				break;
				case TC_COMPARE_B:
					TIMSK &= ~_BV(OCIE1B);
				break;
			}
			#elif defined(TIMSK1)
			TIMSK1 &= ~compare;
			#endif
		break;
		#endif
		
		#if defined(TCNT2)
		case TC_2:	//timer/counter 2.
			#if defined(TIMSK)
			TIMSK &= ~BV(OCIE2);
			#elif defined(TIMSK2)
			TIMSK2 &= ~compare;
			#endif
		break;
		#endif
		
		#if defined(TCNT3)
		case TC_3:	//timer/counter 3 (16bits).
			TIMSK3 &= ~compare;
		break;
		#endif
		
		#if defined(TCNT4)
		case TC_4:	//timer/counter 4 (16bits).
			TIMSK4 &= ~compare;
		break;
		#endif
		
		#if defined(TCNT5)
		case TC_5:	//timer/counter 5 (16bits).
			TIMSK5 &= ~compare;
		break;
		#endif
	}
}

//! \brief Pour savoir si le drapeau de comparaison et lever.
//! \note Pour les timers contenant qu'un seul registre de comparaison, la valeur donnée à \p compare n'est pas prise en compte.
//! \param tc est le timer/counter que vous voulez modifier.
//! \param compare est le comparateur affecter par l'opération de cette fonction.
//! \return false si le drapeau n'est pas lever.
//! \see tc_cleared_compare_flag()
static inline bool tc_is_raising_compare_flag(tc_s tc, tc_compare_e compare)
{
	switch(tc)
	{
		#if defined(TCNT0)
		case TC_0:	//timer/counter 0.
			#if defined(TIFR)
			return TIFR&_BV(OCF0);
			#elif defined(TIFR0)
			return TIFR0&compare;
			#endif
		break;
		#endif
		
		#if defined(TCNT1)
		case TC_1:	//timer/counter 1 (16bits).
			#if defined(TIFR)
			switch(compare)
			{
				case TC_COMPARE_A:
					return TIFR&_BV(OCF1A);
				break;
				case TC_COMPARE_B:
					return TIFR&_BV(OCF1B);
				break;
			}
			#elif defined(TIFR1)
			return TIFR1&compare;
			#endif
		break;
		#endif
		
		#if defined(TCNT2)
		case TC_2:	//timer/counter 2.
			#if defined(TIFR)
			return TIFR&_BV(OCF2);
			#elif defined(TIFR2)
			return TIFR2&compare;
			#endif
		break;
		#endif
		
		#if defined(TCNT3)
		case TC_3:	//timer/counter 3 (16bits).
		return TIFR3&compare;
		break;
		#endif
		
		#if defined(TCNT4)
		case TC_4:	//timer/counter 4 (16bits).
		return TIFR4&compare;
		break;
		#endif
		
		#if defined(TCNT5)
		case TC_5:	//timer/counter 5 (16bits).
		return TIFR5&compare;
		break;
		#endif
	}
}

//! \brief Pour effacer le drapeau de comparaison.
//! \note Pour les timers contenant qu'un seul registre de comparaison, la valeur donnée à \p compare n'est pas prise en compte.
//! \param tc est le timer/counter que vous voulez modifier.
//! \param compare est le comparateur affecter par l'opération de cette fonction.
//! \see tc_is_raising_compare_flag()
static inline void tc_cleared_compare_flag(tc_s tc, tc_compare_e compare)
{
	switch(tc)
	{
		#if defined(TCNT0)
		case TC_0:	//timer/counter 0.
			#if defined(TIFR)
			TIFR |= _BV(OCF0);
			#elif defined(TIFR0)
			TIFR0 |= compare;
			#endif
		break;
		#endif
		
		#if defined(TCNT1)
		case TC_1:	//timer/counter 1 (16bits).
			#if defined(TIFR)
			switch(compare)
			{
				case TC_COMPARE_A:
					TIFR |= _BV(OCF1A);
				break;
				case TC_COMPARE_B:
					TIFR |= _BV(OCF1B);
				break;
			}
			#elif defined(TIFR1)
			TIFR1 |= compare;
			#endif
		break;
		#endif
		
		#if defined(TCNT2)
		case TC_2:	//timer/counter 2.
			#if defined(TIFR)
			TIFR |= _BV(OCF2);
			#elif defined(TIFR2)
			TIFR2 |= compare;
			#endif
		break;
		#endif
		
		#if defined(TCNT3)
		case TC_3:	//timer/counter 3 (16bits).
		TIFR3 |= compare;
		break;
		#endif
		
		#if defined(TCNT4)
		case TC_4:	//timer/counter 4 (16bits).
		TIFR4 |= compare;
		break;
		#endif
		
		#if defined(TCNT5)
		case TC_5:	//timer/counter 5 (16bits).
		TIFR5 |= compare;
		break;
		#endif
	}
}

//! \brief Permet de forcer la comparaison.
//! \note A l'appel de cette fonction, la comparaison sera considérer comme égale, mais l'interruption ne sera pas lever.
//! Ceci est active si vous n'avais pas choisi un mode en PWM.
//! \note Pour les timers contenant qu'un seul registre de comparaison, la valeur donnée à \p compare n'est pas prise en compte.
//! \param tc est le timer/counter que vous voulez modifier.
//! \param compare est le comparateur affecter par l'opération de cette fonction.
//! \see tc_mode_select()
//! \see tc_mode_e
static inline void tc_force_compare(tc_s tc, tc_compare_e compare)
{	
	switch(tc)
	{
		#if defined(TCNT0)
		case TC_0:	//timer/counter 0.
			#if defined(TCCR0)
			TCCR0 |= _BV(FOC0);
			#elif defined(TCCR0B)
			switch(compare)
			{
				case TC_COMPARE_A:
				TCCR0B |=  _BV(FOC0A);
				break;
				
				case TC_COMPARE_B:
				TCCR0B |=  _BV(FOC0B);
				break;
			}
			#elif defined(TCCR0A) != defined(TCCR0B)
			TCCR0A |= _BV(FOC0A);
			#endif
		break;
		#endif
		
		#if defined(TCNT1)
		case TC_1:	//timer/counter 1 (16bits).
			#if defined(TCCR1C)
			switch(compare)
			{
				case TC_COMPARE_A:
				TCCR1C |=  _BV(FOC1A);
				break;
				
				case TC_COMPARE_B:
				TCCR1C |=  _BV(FOC1B);
				break;
				
				#if defined(OCR1C)
				case TC_COMPARE_C:
				TCCR1C |= _BV(FOC1C);
				break;
				#endif
				
				#if defined(OCR1D)
				case TC_COMPARE_D:
				TCCR1C |= _BV(FOC1D);
				break;
				#endif
			}
			#elif defined(TCCR1A) != defined(TCCR1C)
			switch(compare)
			{
				case TC_COMPARE_A:
				TCCR1A |=  _BV(FOC1A);
				break;
				
				case TC_COMPARE_B:
				TCCR1A |=  _BV(FOC1B);
				break;
			}
			#endif
		break;
		#endif
		
		#if defined(TCNT2)
		case TC_2:	//timer/counter 2.
			#if defined(TCCR2)
			TCCR2 |= _BV(FOC2);
			#elif defined(TCCR2B)
			switch(compare)
			{
				case TC_COMPARE_A:
				TCCR2B |=  _BV(FOC2A);
				break;
				
				case TC_COMPARE_B:
				TCCR2B |=  _BV(FOC2B);
				break;
			}
			#elif defined(TCCR2A) != defined(TCCR2B)
			TCCR2A |= _BV(FOC2A);
			#endif
		break;
		#endif
		
		#if defined(TCNT3)
		case TC_3:	//timer/counter 3 (16bits).
			switch(compare)
			{
				case TC_COMPARE_A:
				TCCR3C |=  _BV(FOC3A);
				break;
				
				case TC_COMPARE_B:
				TCCR3C |=  _BV(FOC3B);
				break;
				
				#if defined(OCR3C)
				case TC_COMPARE_C:
				TCCR3C |= _BV(FOC3C);
				break;
				#endif
			}
		break;
		#endif
		
		#if defined(TCNT4)
		case TC_4:	//timer/counter 4 (16bits).
			switch(compare)
			{
				case TC_COMPARE_A:
				TCCR4C |=  _BV(FOC4A);
				break;
				
				case TC_COMPARE_B:
				TCCR4C |=  _BV(FOC4B);
				break;
				
				#if defined(OCR4C)
				case TC_COMPARE_C:
				TCCR4C |= _BV(FOC4C);
				break;
				#endif
			}
		break;
		#endif
		
		#if defined(TCNT5)
		case TC_5:	//timer/counter 5 (16bits).
			switch(compare)
			{
				case TC_COMPARE_A:
				TCCR5C |=  _BV(FOC5A);
				break;
				
				case TC_COMPARE_B:
				TCCR5C |=  _BV(FOC5B);
				break;
				
				#if defined(OCR5C)
				case TC_COMPARE_C:
				TCCR5C |= _BV(FOC5C);
				break;
				#endif
			}
		break;
		#endif
	}
}

//! \brief Permet de modifier la valeur de comparaison, des comparateurs de 8 bits (timer/counter 0 et 2).
//! \note Pour les timers contenant qu'un seul registre de comparaison, la valeur donnée à \p compare doit être à TC_COMPARE_A.
//! \param tc est le timer/counter que vous voulez modifier.
//! \param compare est le comparateur affecter par l'opération de cette fonction.
//! \param val est la nouvelle valeur de comparaison.
//! \see tc_get_compare_8bit()
//! \see tc_set_compare_16bit()
//! \see tc_get_compare_16bit()
static inline void tc_set_compare_8bit(tc_s tc, tc_compare_e compare, uint8_t val)
{
	switch(tc)
	{		
		#if defined(TCNT0)
		case TC_1:	//timer/counter 0.
			#if defined(OCR0)
			OCR0 = val;
			#elif defined(OCR0B)
			switch(compare)
			{
				case TC_COMPARE_A:
					OCR0A = val;
				break;
				case TC_COMPARE_B:
					OCR0B = val;
				break;
			}
			#elif defined(OCR0A) && !defined(OCR0B)
			OCR0A = val;
			#endif
		break;
		#endif
		
		#if defined(TCNT2)
		case TC_3:	//timer/counter 2.
			#if defined(OCR2)
			OCR2 = val;
			#elif defined(OCR2B)
			switch(compare)
			{
				case TC_COMPARE_A:
					OCR2A = val;
				break;
				case TC_COMPARE_B:
					OCR2B = val;
				break;
			}
			#elif defined(OCR2A) && !defined(OCR2B)
			OCR2A = val;
			#endif
		break;
		#endif
	}
}

//! \brief Permet d'obtenir la valeur de comparaison, des comparateurs de 8 bits (timer/counter 0 et 2).
//! \note Pour les timers contenant qu'un seul registre de comparaison, la valeur donnée à \p compare doit être à TC_COMPARE_A.
//! \param tc est le timer/counter que vous voulez modifier.
//! \param compare est le comparateur affecter par l'opération de cette fonction.
//! \return La valeur de comparaison.
//! \see tc_set_compare_8bit()
//! \see tc_set_compare_16bit()
//! \see tc_get_compare_16bit()
static inline uint8_t tc_get_compare_8bit(tc_s tc, tc_compare_e compare)
{
	switch(tc)
	{		
		#if defined(TCNT0)
		case TC_1:	//timer/counter 0.
			#if defined(OCR0)
			return OCR0;
			#elif defined(OCR0B)
			switch(compare)
			{
				case TC_COMPARE_A:
					return OCR0A;
				break;
				case TC_COMPARE_B:
					return OCR0B;
				break;
			}
			#elif defined(OCR0A) && !defined(OCR0B)
			return OCR0A = val;
			#endif
		break;
		#endif
		
		#if defined(TCNT2)
		case TC_3:	//timer/counter 2.
			#if defined(OCR2)
			return OCR2;
			#elif defined(OCR2B)
			switch(compare)
			{
				case TC_COMPARE_A:
					return OCR2A;
				break;
				case TC_COMPARE_B:
					return OCR2B;
				break;
			}
			#elif defined(OCR2A) && !defined(OCR2B)
			return OCR2A;
			#endif
		break;
		#endif
	}
}

//! \brief Permet de modifier la valeur de comparaison, des comparateurs de 16 bits (timer/counter 1, 3, 4 et 5).
//! \param tc est le timer/counter que vous voulez modifier.
//! \param compare est le comparateur affecter par l'opération de cette fonction.
//! \param val est la nouvelle valeur de comparaison.
//! \see tc_set_compare_8bit()
//! \see tc_get_compare_8bit()
//! \see tc_get_compare_16bit()
static inline void tc_set_compare_16bit(tc_s tc, tc_compare_e compare, uint16_t val)
{
	//Sauvegarde du drapeau d'interruption globale.
	uint8_t sreg = SREG;
	//Désactive les interruptions
	cli();
	
	switch(tc)
	{		
		#if defined(TCNT1)
		case TC_1:	//timer/counter 1 (16bits).
			switch(compare)
			{
				case TC_COMPARE_A:
					OCR1A = val;
				break;
				case TC_COMPARE_B:
					OCR1B = val;
				break;
				#if defined(OCR1C)
				case TC_COMPARE_C:
					OCR1C = val;
				break;
				#endif
				#if defined(OCR1D)
				case TC_COMPARE_D:
					OCR1D = val;
				break;
				#endif
			}
		break;
		#endif
		
		#if defined(TCNT3)
		case TC_3:	//timer/counter 3 (16bits).
			switch(compare)
			{
				case TC_COMPARE_A:
					OCR2A = val;
				break;
				case TC_COMPARE_B:
					OCR3B = val;
				break;
				#if defined(OCR3C)
				case TC_COMPARE_C:
					OCR3C = val;
				break;
				#endif
			}
		break;
		#endif
		
		#if defined(TCNT4)
		case TC_4:	//timer/counter 4 (16bits).
			switch(compare)
			{
				case TC_COMPARE_A:
					OCR4A = val;
				break;
				case TC_COMPARE_B:
					OCR4B = val;
				break;
				#if defined(OCR4C)
				case TC_COMPARE_C:
					OCR4C = val;
				break;
				#endif
			}
		break;
		#endif
		
		#if defined(TCNT5)
		case TC_5:	//timer/counter 5 (16bits).
			switch(compare)
			{
				case TC_COMPARE_A:
					OCR5A = val;
				break;
				case TC_COMPARE_B:
					OCR5B = val;
				break;
				#if defined(OCR5C)
				case TC_COMPARE_C:
					OCR5C = val;
				break;
				#endif
			}
		break;
		#endif
	}
	
	//Restaure du drapeau d'interruption globale.
	SREG = sreg;
}

//! \brief Permet d'obtenir la valeur de comparaison, des comparateurs de 16 bits (timer/counter 1, 3, 4 et 5).
//! \param tc est le timer/counter que vous voulez modifier.
//! \param compare est le comparateur affecter par l'opération de cette fonction.
//! \return La valeur de comparaison.
//! \see tc_set_compare_8bit()
//! \see tc_get_compare_8bit()
//! \see tc_set_compare_16bit()
static inline uint16_t tc_get_compare_16bit(tc_s tc, tc_compare_e compare)
{	
	//Sauvegarde du drapeau d'interruption globale.
	uint8_t sreg = SREG;
	//Désactive les interruptions
	cli();
	
	switch(tc)
	{		
		#if defined(TCNT1)
		case TC_1:	//timer/counter 1 (16bits).
			switch(compare)
			{
				case TC_COMPARE_A:
					return OCR1A;
				break;
				case TC_COMPARE_B:
					return OCR1B;
				break;
				#if defined(OCR1C)
				case TC_COMPARE_C:
					return OCR1C;
				break;
				#endif
				#if defined(OCR1D)
				case TC_COMPARE_D:
					return OCR1D;
				break;
				#endif
			}
		break;
		#endif
		
		#if defined(TCNT3)
		case TC_3:	//timer/counter 3 (16bits).
			switch(compare)
			{
				case TC_COMPARE_A:
					return OCR2A;
				break;
				case TC_COMPARE_B:
					return OCR3B;
				break;
				#if defined(OCR3C)
				case TC_COMPARE_C:
					return OCR3C;
				break;
				#endif
			}
		break;
		#endif
		
		#if defined(TCNT4)
		case TC_4:	//timer/counter 4 (16bits).
			switch(compare)
			{
				case TC_COMPARE_A:
					return OCR4A;
				break;
				case TC_COMPARE_B:
					return OCR4B;
				break;
				#if defined(OCR4C)
				case TC_COMPARE_C:
					return OCR4C;
				break;
				#endif
			}
		break;
		#endif
		
		#if defined(TCNT5)
		case TC_5:	//timer/counter 5 (16bits).
			switch(compare)
			{
				case TC_COMPARE_A:
					return OCR5A;
				break;
				case TC_COMPARE_B:
					return OCR5B;
				break;
				#if defined(OCR5C)
				case TC_COMPARE_C:
					return OCR5C;
				break;
				#endif
			}
		break;
		#endif
	}
	
	//Restaure du drapeau d'interruption globale.
	SREG = sreg;
}

//! @} //Compare


//! @name Capture
//!
//! \todo Prendre en compte l'entre de comparaison analogique.
//! Fonction lier au capture des timers (capture). Ces fonctionnalités sont présent pour les timer 1, 3, 4, 5.
//! @{


//! \brief Active l'interruption de capture.
//!
//! A utiliser avec le vecteur d'interruption TIMERn_CAPT_vect, (n représentent le numéro du timer).
//!
//! \code
//! //Exemple pour le timer 4.
//! ISR(TIMER4_CAPT_vect)
//! {
//! 	//code
//! }
//! \endcode
//!
//! \note Certain composent ne dispose pas des vecteurs d'interruption décri si dessus. Les autre vecteur possible sont décri si dessous.
//!
//! \par TIMER1_CAPT1_vect
//! Le vecteur d'interruption TIMER1_CAPT1_vect, est utiliser par les composants suivent : \n
//! AT90S2313
//!
//! \par TIM1_CAPT_vect
//! Le vecteur d'interruption TIM1_CAPT_vect, est utiliser par les composants suivent : \n
//! ATtiny24, ATtiny44, ATtiny84
//!
//! \param tc est le timer/counter que vous voulez modifier.
//! \see tc_disable_interrupt_capture()
static inline void tc_enable_interrupt_capture(tc_s tc)
{
	switch(tc)
	{
		#if defined(TCNT1)
		case TC_1:	//timer/counter 1 (16bits).
			#if defined(TIMSK)
			TIMSK |= _BV(TICIE1);
			#elif defined(TIMSK1)
			TIMSK1 |= _BV(ICIE1);
			#endif
		break;
		#endif
		
		#if defined(TCNT3)
		case TC_3:	//timer/counter 3 (16bits).
			TIMSK3 |= _BV(ICIE3);
		break;
		#endif
		
		#if defined(TCNT4)
		case TC_4:	//timer/counter 4 (16bits).
			TIMSK4 |= _BV(ICIE4);
		break;
		#endif
		
		#if defined(TCNT5)
		case TC_5:	//timer/counter 5 (16bits).
			TIMSK5 |= _BV(ICIE5);
		break;OCK_POLARITY_RISING|SPI_CLOCK_PHASE_SAMPL
		#endif
	}
}

//! \brief Désactive l'interruption de capture.
//! \param tc est le timer/counter que vous voulez modifier.
//! \see tc_enable_interrupt_capture()
static inline void tc_disable_interrupt_capture(tc_s tc)
{
	switch(tc)
	{
		#if defined(TCNT1)
		case TC_1:	//timer/counter 1 (16bits).
			#if defined(TIMSK)
			TIMSK &= ~_BV(TICIE1);
			#elif defined(TIMSK1)
			TIMSK1 &= ~_BV(ICIE1);
			#endif
		break;
		#endif
		
		#if defined(TCNT3)
		case TC_3:	//timer/counter 3 (16bits).
			TIMSK3 &= ~_BV(ICIE3);
		break;
		#endif
		
		#if defined(TCNT4)
		case TC_4:	//timer/counter 4 (16bits).
			TIMSK4 &= ~_BV(ICIE4);
		break;
		#endif
		
		#if defined(TCNT5)
		case TC_5:	//timer/counter 5 (16bits).
			TIMSK5 &= ~_BV(ICIE5);
		break;
		#endif
	}
}

//! \brief Active le réducteur de bruit de l'entrée de capture (ICPn).
//! \param tc est le timer/counter que vous voulez modifier.
//! \see tc_disable_noise_canceler_capture()
static inline void tc_enable_noise_canceler_capture(tc_s tc)
{
	switch(tc)
	{
		#if defined(TCNT1)
		case TC_1:	//timer/counter 1 (16bits).
			TCCR1B |= _BV(ICNC1);
		break;
		#endif
		
		#if defined(TCNT3)
		case TC_3:	//timer/counter 3 (16bits).
			TCCR3B |= _BV(ICNC3);
		break;
		#endif
		
		#if defined(TCNT4)
		case TC_4:	//timer/counter 4 (16bits).
			TCCR4B |= _BV(ICNC4);
		break;
		#endif
		
		#if defined(TCNT5)
		case TC_5:	//timer/counter 5 (16bits).
			TCCR5B |= _BV(ICNC5);
		break;
		#endif
	}
}

//! \brief Désactive le réducteur de bruit de l'entrée de capture (ICPn).
//! \param tc est le timer/counter que vous voulez modifier.
//! \see tc_enable_noise_canceler_capture()
static inline void tc_disable_noise_canceler_capture(tc_s tc)
{
	switch(tc)
	{
		#if defined(TCNT1)
		case TC_1:	//timer/counter 1 (16bits).
			TCCR1B &= ~_BV(ICNC1);
		break;
		#endif
		
		#if defined(TCNT3)
		case TC_3:	//timer/counter 3 (16bits).
			TCCR3B &= ~_BV(ICNC3);
		break;
		#endif
		
		#if defined(TCNT4)
		case TC_4:	//timer/counter 4 (16bits).
			TCCR4B &= ~_BV(ICNC4);
		break;
		#endif
		
		#if defined(TCNT5)
		case TC_5:	//timer/counter 5 (16bits).
			TCCR5B &= ~_BV(ICNC5);
		break;
		#endif
	}
}

//! \brief Permet de sélectionner le front de déclenchement des captures.
//! \note Ceci est actifs que si le registre de capteur n'est pas utiliser dans les modes.
//! \param tc est le timer/counter que vous voulez modifier.
//! \param edge est la valeur représentant le front.
//! \see tc_mode_select()
//! \see tc_mode_e
static inline void tc_edge_select(tc_s tc, tc_edge_e edge)
{
	//*tc->tccrb = (*tc->tccrb&TC_EDGE_MASK)|edge;
	
	switch(tc)
	{
		#if defined(TCNT1)
		case TC_1:	//timer/counter 1 (16bits).
			TCCR1B = (TCCR1B&~TC_EDGE_MASK)|edge;
		break;
		#endif
		
		#if defined(TCNT3)
		case TC_3:	//timer/counter 3 (16bits).
			TCCR3B = (TCCR3B&~TC_EDGE_MASK)|edge;
		break;
		#endif
		
		#if defined(TCNT4)
		case TC_4:	//timer/counter 4 (16bits).
			TCCR4B = (TCCR4B&~TC_EDGE_MASK)|edge;
		break;
		#endif
		
		#if defined(TCNT5)
		case TC_5:	//timer/counter 5 (16bits).
			TCCR5B = (TCCR5B&~TC_EDGE_MASK)|edge;
		break;
		#endif
	}
}

//! \brief Pour savoir si le drapeau de capture et lever.
//! \param tc est le timer/counter que vous voulez modifier.
//! \return false si le drapeau n'est pas lever.
//! \see tc_cleared_capture_flag()
static inline bool tc_is_raising_capture_flag(tc_s tc)
{	
	switch(tc)
	{
		#if defined(TCNT1)
		case TC_1:	//timer/counter 1 (16bits).
			#if defined(TIFR)
			return TIFR&_BV(ICF1);
			#elif defined(TIFR1)
			return TIFR1&_BV(ICF1);
			#endif
		break;
		#endif
		
		#if defined(TCNT3)
		case TC_3:	//timer/counter 3 (16bits).
			return TIFR3&_BV(ICF3);
		break;
		#endif
		
		#if defined(TCNT4)
		case TC_4:	//timer/counter 4 (16bits).
			return TIFR4&_BV(ICF4);
		break;
		#endif
		
		#if defined(TCNT5)
		case TC_5:	//timer/counter 5 (16bits).
			return TIFR5&_BV(ICF5);
		break;
		#endif
	}
}

//! \brief Pour effacer le drapeau de capture.
//! \param tc est le timer/counter que vous voulez modifier.
//! \see tc_is_raising_capture_flag()
static inline void tc_cleared_capture_flag(tc_s tc)
{
	switch(tc)
	{
		#if defined(TCNT1)
		case TC_1:	//timer/counter 1 (16bits).
			#if defined(TIFR)
			TIFR |= _BV(ICF1);
			#elif defined(TIFR1)
			TIFR1 |= _BV(ICF1);
			#endif
		break;
		#endif
		
		#if defined(TCNT3)
		case TC_3:	//timer/counter 3 (16bits).
			TIFR3 |= _BV(ICF3);
		break;
		#endif
		
		#if defined(TCNT4)
		case TC_4:	//timer/counter 4 (16bits).
			TIFR4 |= _BV(ICF4);
		break;
		#endif
		
		#if defined(TCNT5)
		case TC_5:	//timer/counter 5 (16bits).
			TIFR5 |= _BV(ICF5);
		break;
		#endif
	}
}

//! \brief Permet de modifier la valeur de capture.
//! \param tc est le timer/counter que vous voulez modifier.
//! \param val est la nouvelle valeur de capture.
//! \see tc_get_capture()
static inline void tc_set_capture(tc_s tc, uint16_t val)
{	
	//Sauvegarde du drapeau d'interruption globale.
	uint8_t sreg = SREG;
	//Désactive les interruptions
	cli();
	
	switch(tc)
	{		
		#if defined(TCNT1)
		case TC_1:	//timer/counter 1 (16bits).
			ICR1 = val;
		break;
		#endif
		
		#if defined(TCNT3)
		case TC_3:	//timer/counter 3 (16bits).
			ICR3 = val;
		break;
		#endif
		
		#if defined(TCNT4)
		case TC_4:	//timer/counter 4 (16bits).
			ICR4 = val;
		break;
		#endif
		
		#if defined(TCNT5)
		case TC_5:	//timer/counter 5 (16bits).
			ICR5 = val;
		break;
		#endif
	}
	
	//Restaure du drapeau d'interruption globale.
	SREG = sreg;
}

//! \brief Permet d'obtenir la valeur de capture.
//! \param tc est le timer/counter que vous voulez modifier.
//! \return La valeur de capture.
//! \see tc_set_capture()
static inline uint16_t tc_get_capture(tc_s tc)
{
	//Sauvegarde du drapeau d'interruption globale.
	uint8_t sreg = SREG;
	//Désactive les interruptions
	cli();
	
	switch(tc)
	{		
		#if defined(TCNT1)
		case TC_1:	//timer/counter 1 (16bits).
			return ICR1;
		break;
		#endif
		
		#if defined(TCNT3)
		case TC_3:	//timer/counter 3 (16bits).
			return ICR3;
		break;
		#endif
		
		#if defined(TCNT4)
		case TC_4:	//timer/counter 4 (16bits).
			return ICR4;
		break;
		#endif
		
		#if defined(TCNT5)
		case TC_5:	//timer/counter 5 (16bits).
			return ICR5;
		break;
		#endif
	}
	
	//Restaure du drapeau d'interruption globale.
	SREG = sreg;
}


//! @} //Capture

//! @} //Tc

#endif // TC_H_INCLUDED
