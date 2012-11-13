#include <avr/io.h>
#include <avr/interrupt.h>

#include <tc.h>

//...

//Interruption de débordement sur le timer 1.
ISR(TIMER1_OVF_vect)
{
	//...
}

//Interruption du comparateur A, sur le timer 1.
ISR(TIMER1_COMPA_vect)
{
	//...
}

//Interruption du comparateur B, sur le timer 1.
ISR(TIMER1_COMPB_vect)
{
	//...
}

//...

int main()
{
	//...

	tc_s tc;					//On déclare une structure pour manipuler un timer.
	tc_init(&tc, TC_NUM_1);		//On l'initialise sur le timer 1.

	//Active les interruptions.
	sei();	

	tc_enable_interrupt_overflow(&tc);					//Active l'interruption de débordement du timer 1.
	tc_enable_interrupt_compare(&tc, TC_COMPARE_A);		//Active l'interruption de comparaison sur le comparateur A, du timer 1.
	tc_enable_interrupt_compare(&tc, TC_COMPARE_B);		//Active l'interruption de comparaison sur le comparateur B, du timer 1.
	tc_clock_select(&tc, TC_CLOCK_CLK_1);				//Cadence du timer 1 à l'horloge du cpu diviser pas 1.

	tc_set_compare_16bit(&tc, TC_COMPARE_A, 0x8000);	//Pour avoirs environ un rapport cyclique de 50%, sur le comparateur A.
	tc_set_compare_16bit(&tc, TC_COMPARE_B, 0x3333);	//Pour avoirs environ un rapport cyclique de 80%, sur le comparateur B.

	//...

	while(1)
	{
		//...
	}

    return 0;
}
