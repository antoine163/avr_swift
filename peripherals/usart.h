/*Fichier conçu pour la génération de la documentation automatique avec
* Doxygen.
*/
//! \file **********************************************************************
//!
//! \brief Fichier pour l'usart (Universal Synchronous/Asynchronous Receiver/Transmitter).
//!
//! - Compilateur		: AVR-GCC
//! - Composent supporter	: atmega
//!
//!
//!
//! \author Maleyrie Antoine
//! \version 0.1
//! \date 28 Août 2011
//!
//! ****************************************************************************

/*
*	Copyright © 2011 - Antoine Maleyrie.
*/

#ifndef USART_H_INCLUDED
#define USART_H_INCLUDED

//! \addtogroup usart
//! \brief usart (Universal Synchronous/Asynchronous Receiver/Transmitter)
//! \todo A finir/tester
//! \code
//! #include <usart.h>
//! \endcode
//!
//!
//! @{

//_____ I N C L U D E S ________________________________________________________

//_____ M A C R O S ____________________________________________________________

//_____ S T R U C T ____________________________________________________________

//_____ E N U M ________________________________________________________________
typedef enum
{
	#if defined(UCSR0A) || defined(__DOXYGEN__)
	USART0,
	#endif
	#if defined(UCSR1A) || defined(__DOXYGEN__)
	USART1,
	#endif
}usart_e;

typedef enum
{
	USART_MODE_ASYNCHRONOUS		= 0x00,
	USART_MODE_SYNCHRONOUS		= 0x40,
	USART_MODE_MASTER_SPI		= 0xc0,
	
	#if !defined(__DOXYGEN__)
	USART_MODE_MASK 			= 0x3f
	#endif
}usart_mode_e;

typedef enum
{
	USART_PARITY_DISABLED				= 0x00,
	USART_PARITY_ENABLED_EVEN_PARITY	= 0x20,
	USART_PARITY_ENABLED_ODD_PARITY		= 0x30,
	
	#if !defined(__DOXYGEN__)
	USART_PARITY_MASK 					= 0xcf
	#endif
}usart_parity_e;

typedef enum
{
	USART_STOP_BIT_ONE				= 0x00,
	USART_STOP_BIT_TWO				= 0x08,
	
	#if !defined(__DOXYGEN__)
	USART_STOP_BIT_MASK 			= 0xf7
	#endif
}usart_stop_bit_e;


typedef enum
{
	USART_CHARACTER_SIZE_5BIT		= 0x00,
	USART_CHARACTER_SIZE_6BIT		= 0x02,
	USART_CHARACTER_SIZE_7BIT		= 0x04,
	USART_CHARACTER_SIZE_8BIT		= 0x06,
	USART_CHARACTER_SIZE_9BIT		= 0x0e,
	
	#if !defined(__DOXYGEN__)
	USART_CHARACTER_SIZE_MASK 				= 0xf9
	#endif
}usart_character_size_e;

typedef enum
{
	USART_CLOCK_POLARITY_RISING_EDGE	= 0x00,
	USART_CLOCK_POLARITY_FALLING_EDGE	= 0x01,
	
	#if !defined(__DOXYGEN__)
	USART_CLOCK_POLARITY_MASK 				= 0xfe
	#endif

}usart_clock_polarity_e;

//_____ P R O T O T Y P E S ____________________________________________________

//_____ F U N C T I O N ________________________________________________________

static inline void usart_baud_rate(usart_e usart, uint16_t val)
{
	switch(usart)
	{
		#if defined(UBRR0L)
		case USART0:
		*((volatile uint16_t*)(&UBRR0L)) = val;
		break;
		#endif
		
		#if defined(UBRR1L)
		case USART1:
		*((volatile uint16_t*)(&UBRR1L)) = val;
		break;
		#endif
	}
}

static inline void usart_mode_select(usart_e usart, usart_mode_e val)
{
	switch(usart)
	{
		#if defined(UCSR0C)
		case USART0:
		UCSR0C = (UCSR0C&USART_MODE_MASK)|val;
		break;
		#endif
		
		#if defined(UCSR1C)
		case USART1:
		UCSR1C = (UCSR1C&USART_MODE_MASK)|val;
		break;
		#endif
	}
}

static inline void usart_parity_mode(usart_e usart, usart_parity_e val)
{
	switch(usart)
	{
		#if defined(UCSR0C)
		case USART0:
		UCSR0C = (UCSR0C&USART_PARITY_MASK)|val;
		break;
		#endif
		
		#if defined(UCSR1C)
		case USART1:
		UCSR1C = (UCSR1C&USART_PARITY_MASK)|val;
		break;
		#endif
	}
}

static inline void usart_stop_bit_select(usart_e usart, usart_stop_bit_e val)
{
	switch(usart)
	{
		#if defined(UCSR0C)
		case USART0:
		UCSR0C = (UCSR0C&USART_STOP_BIT_MASK)|val;
		break;
		#endif
		
		#if defined(UCSR1C)
		case USART1:
		UCSR1C = (UCSR1C&USART_STOP_BIT_MASK)|val;
		break;
		#endif
	}
}

static inline void usart_character_size(usart_e usart, usart_character_size_e val)
{
	switch(usart)
	{
		#if defined(UCSR0C)
		case USART0:
		UCSR0C = (UCSR0C&USART_CHARACTER_SIZE_MASK)|val;
		break;
		#endif
		
		#if defined(UCSR1C)
		case USART1:
		UCSR1C = (UCSR1C&USART_CHARACTER_SIZE_MASK)|val;
		break;
		#endif
	}
}

static inline void usart_clock_polarity(usart_e usart, usart_clock_polarity_e val)
{
	switch(usart)
	{
		#if defined(UCSR0C)
		case USART0:
		UCSR0C = (UCSR0C&USART_CLOCK_POLARITY_MASK)|val;
		break;
		#endif
		
		#if defined(UCSR1C)
		case USART1:
		UCSR1C = (UCSR1C&USART_CLOCK_POLARITY_MASK)|val;
		break;
		#endif
	}
}

static inline void usart_config(usart_e usart,  uint8_t config)
{
	switch(usart)
	{
		#if defined(UCSR0C)
		case USART0:
		UCSR0C = config;
		break;
		#endif
		
		#if defined(UCSR1C)
		case USART1:
		UCSR1C = config;
		break;
		#endif
	}
}

//! @} //usart



#endif // USART_H_INCLUDED



