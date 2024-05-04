/************************************************************************************************************
 *                                                                                                          *
 * File Name: stopwatch.c                                                                             	    *
 *                                                                                                          *
 * Description: Stopwatch System using an ATmega32 and multiplexed 7-segment displays.                      *
 *              It offers functionalities such as starting, stopping, resuming, and resetting,              *
 *              controlled through external push buttons.						    *
 *              The system utilizes Timer1 in ATmega32 with CTC mode for accurate timekeeping,              *
 *              and employs a multiplexing technique to efficiently manage multiple 7-segment displays.     *
 * Author: Omar Rizk                                                                                        *
 *                                                                                                          *
 ************************************************************************************************************/

/* Including AVR Libraries */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
/***********************************************************************************************************/

/* Functions Declaration */

/* Interrupt Functions */
void INT0_Init(void);                          /* Initialize External Interrupt 0 */
void INT1_Init(void);                          /* Initialize External Interrupt 1 */
void INT2_Init(void);                          /* Initialize External Interrupt 2 */

/* Timer Functions */
void Timer1_CompareMode_Init(void);            /* Initialize Timer 1 in Compare Mode */

/* Seven Segment Display Functions */
void Seven_Segment_Display();                  /* Display Function for Multiplexed 7-Segment Display */
void Seven_Segment_Stopwatch_Update();         /* Update Stopwatch Time on Multiplexed 7-Segment Display */
/***********************************************************************************************************/

/* Global Variables Declaration*/
unsigned char Compare_Interrupt_Flag ;

unsigned char Seconds_Units;
unsigned char Seconds_Tens;
unsigned char Minutes_Units;
unsigned char Minutes_Tens;
unsigned char Hours_Units;
unsigned char Hours_Tens;

unsigned char ticks_arr[6];


/* Timer 1 ISR */
ISR(TIMER1_COMPA_vect)
{
    /* Enable Global Interrupt Bit */
    SREG |= (1<<7);
    /* Set Interrupt Flag To Take Action In Superloop */
    Compare_Interrupt_Flag = 1 ;
}

/* Interrupt 0 to Reset Stop Watch */
ISR(INT0_vect)
{
    /* Enable Global Interrupt Bit */
    SREG |= (1<<7);

    /* Reset Seconds Digits */
    Seconds_Units = 0;
    Seconds_Tens = 0;
    /* Reset Minutes Digits */
    Minutes_Units = 0;
    Minutes_Tens = 0;
    /* Reset Minutes Digits */
    Hours_Units = 0;
    Hours_Tens = 0;
}

/* Interrupt 1 to Stop the Stop Watch */
ISR(INT1_vect)
{
    /* Enable Global Interrupt Bit */
    SREG |= (1<<7);
    /* Disable Timer 1 Clock Source */
    TCCR1B &=~(1<<CS10) &~(1<<CS11) &~(1<<CS12);
}

/* Interrupt 2 to Continue Stop Watch */
ISR(INT2_vect)
{
    /* Enable Global Interrupt Bit */
    SREG |= (1<<7);
    /* Enable Timer 1 With Selected Clock Source */
    TCCR1B |= (1<<CS10) |(1<<CS12);
}


int main()
{
	/* Enable Global Interrupt Bit */
	SREG |=(1<<7);

	/* Decoder Initialization */
	DDRC |= 0x0F;   /* Set Decoder Pins as Output */
	PORTC &= 0xF0;  /* Clear All Decoder Pins */

	/* Seven Segment Initialization */
	DDRA |= 0x3F;   /* Set Seven Segment Pins as Output */
	PORTA &= 0xC0;  /* Clear All Decoder Pins */

	/* Initialize Timer 1 */
	Timer1_CompareMode_Init();

	/* Initialize INT 0 */
	INT0_Init();
	/* Initialize INT 1 */
	INT1_Init();
	/* Initialize INT 2 */
	INT2_Init();

	while (1)
	{
		/* Display Stopwatch Value On Connected Seven Segment Display */
		Seven_Segment_Display();

		/* Check If 1 Second Has Passed */
		if( 1 == Compare_Interrupt_Flag )
		{
			/* Update Stopwatch Value */
			Seven_Segment_Stopwatch_Update();

			/* Clear Flag To Re-Enter in Next Second */
			Compare_Interrupt_Flag = 0 ;
		}
	}
}


/********************************* Interrupt Initialization Functions *********************************/

/* Initialize External Interrupt 0 */
void INT0_Init(void)
{
    /* Detect Falling Edge */
    MCUCR |= (1<<ISC01) &~(1<<ISC00);
    /* Enable External Interrupt 0 Request */
    GICR =(1<<INT0);

    /* Set Pin as Input For Push Button */
    DDRD &= ~(1<<PD2);
    /* Enable Internal Pull-up */
    PORTD |=(1<<PD2);
}

/* Initialize External Interrupt 1 */
void INT1_Init(void)
{
    /* Detect Rising Edge */
    MCUCR |=(1<<ISC11) |(1<<ISC10);
    /* Enable External Interrupt 1 Request  */
    GICR |= (1<<INT1);

    /* Set Pin as Input For Push Button */
    DDRD &= ~(1<<PD3);
}

/* Initialize External Interrupt 2 */
void INT2_Init(void)
{
    /* Detect Falling Edge */
    MCUCSR &= ~(1<<ISC2);
    /* Enable External Interrupt 2 Request */
    GICR|=(1<<INT2);

    /* Set Pin as Input For Push Button */
    DDRB &= ~(1<<PB2);
    /* Enable Internal Pull-up */
    PORTB |=(1<<PB2);
}

/* Initialize Timer1 in Compare Mode */
void Timer1_CompareMode_Init(void)
{
    /* Force Output Compare For Compare Unit A */
    TCCR1A = (1<<FOC1A);
    /* Use CTC Mode and Adjust Prescaler to 1024 */
    TCCR1B=(1<<WGM12) |(1<<CS12)|(1<<CS10);

    /* Timer 1 Start Value */
    TCNT1 = 0;
    /* Compare value : 977 -> To Count A Second */
    OCR1A = 977;

    /* Enable Timer 1 Interrupt */
    TIMSK|=(1<<OCIE1A);
}

/* Display Function for Multiplexed 7-Segment Display */
void Seven_Segment_Display()
{
    unsigned char loop_iterator=0;

    for (loop_iterator=0; loop_iterator<6 ;loop_iterator++)
    {
        /* Enable Selected Seven Segment */
        PORTA =(PORTA & 0xC0) | (1<<loop_iterator);

        /* Send Selected Value To Decoder */
        PORTC =(PORTC & 0x00) | (0x0F & ticks_arr[loop_iterator]);

        /* Delay for Persistence of Vision */
        _delay_ms(2);
    }
}

/* Update Stopwatch Time on Multiplexed 7-Segment Display */
void Seven_Segment_Stopwatch_Update()
{
    /* Increment Seconds By 1 */
    ticks_arr[0]++ ;

    /* Seconds Value Update */
    if(ticks_arr[0] == 10)
    {
        ticks_arr[0] = 0 ;
        ticks_arr[1]++;
    }
    if(ticks_arr[1] == 6 && ticks_arr[0] == 0)
    {
        ticks_arr[1] = 0;
        ticks_arr[2]++;
    }

    /* Minutes Value Update */
    if(ticks_arr[2] == 10)
    {
        ticks_arr[2] = 0 ;
        ticks_arr[3]++;
    }
    if(ticks_arr[3] == 6 && ticks_arr[2] == 0)
    {
        ticks_arr[3] = 0;
        ticks_arr[4]++;
    }

    /* Hours Value Update */
    if(ticks_arr[4]==10)
    {
        ticks_arr[4]= 0;
        ticks_arr[5]++;
    }
}
