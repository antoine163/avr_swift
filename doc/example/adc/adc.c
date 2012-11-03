#include <avr/io.h>
#include <avr/interrupt.h>

#include <tc.h>
#include <adc.h>

// ...

tc_s tc0; 	//Un timer.

//Interruption de fin de conversion.
ISR(ADC_vect)
{
	//Récupération la valeur de convection sur 10 bits.
	unsigned int tmpadc = adc_get_conversion_10bit(); 		

	// ...

	//On affecte le drapeau de débordement du timer pour pouvoir refaire d'autre conversion.
	tc_cleared_overflow_flag(&tc0);				
}

int main()
{
	// ...

	//Active les interruptions.
	sei(); 


	adc_voltage_select(ADC_VOLTAGE_AVCC);			//Sélectionne la tension de référence.
	adc_channel_select(ADC_CHANNEL_ADC0);			//Sélectionne le bit ou se trouve la tension à convertir.
	adc_disable_input(ADC_INPUT_ADC0);				//Désactiver l'entrée numérique de la pin ADC0. (Pas obligatoire).
	adc_prescaler_select(ADC_PRESCALER_128);		//Sélectionne le pré-diviser à 128, pour par exemple avoir un convertisseur fonctionnent à 125KHz sur 10 bits pour un cpu à 16MHz.

	adc_enable();									//Active la conversion analogique numérique.
	adc_start();									//Lance une première conversion (qui est inutilisable).
	while(!adc_is_raising_flag());					//Attend que la conversion soie finie.
	adc_cleared_flag();								//Efface le bit du drapeau de fin de convection.


	tc_init(&tc0, TC_NUM_0);						//Initialise le timer 0.
	tc_clock_select(&tc0, TC_CLOCK_CLK_1024);		//Sélectionne un pré-diviseur de 1024. Pour effectuer une conversion tout les 16ms environ, (cpu à 16MHZ).


	adc_enable_interrupt();							//Active l'interruption lier au la conversion.
	adc_trigger_select(ADC_TRIGGER_TIMER0_OVERFOW);	//Sélectionne le débordement du timer 0 pour lancer des conversion.
	adc_enable_trigger();							//Active le déclenchement automatique des conversions.

	// ...

	while(1)
	{
		// ...
	}
}
