/*	Author: Shubham Batra
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab # 11 
 *	Exercise Description: This is my version of Space Invaders
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Notes: May be bugs
 *
 * 	Google Drive DEMO: https://drive.google.com/file/d/1hZLa837R6xkVCP9Qad95OklAzUy_J6HD/view?usp=sharing
 *
 * Current States: Game, Joystick, Button Input 1 (Shoot), Button Input 2 (Reset), Score, Player Movement, Alien Movement
 *
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
//#include <PWM.h>
//#include <scheduler.h>
#endif


// Default implementations below

// Joystick Implementation
void A2D_init() {
      ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: Enables analog-to-digital conversion
	// ADSC: Starts analog-to-digital conversion
	// ADATE: Enables auto-triggering, allowing for constant
	//	    analog to digital conversions.
}

//------------------------------Task scheduler data structure---------------------------
// Struct for Tasks represent a running process in our simple real-time operating system
typedef struct task{
	/* Tasks should have members that include: state, period,
	a measurement of elapsed time, and a function pointer. */
	signed 	 char state; 		// Task's current state
	unsigned long period; 		// Task period
	unsigned long elapsedTime; 	// Time elapsed since last task tick
	int (*TickFct)(int); 		// Task tick function
} task;

//----------------------------End task scheduler data structure-------------------------


//--------------------------------------
// LED Matrix Demo SynchSM (Becomes Alien Representation)
// Period: 100 ms
//--------------------------------------
	enum Demo_States {shift};
	int Demo_Tick(int state) {

	// Local Variables
	static unsigned char pattern = 0x80;	// LED pattern - 0: LED off; 1: LED on
	static unsigned char row = 0xFE;  	// Row(s) displaying pattern. 
							// 0: display pattern on row
							// 1: do NOT display pattern on row
	// Transitions
	switch (state) {
		case shift:	
			break;
		default:	
			state = shift;
			break;
	}	
	// Actions
	switch (state) {
		case shift:
			pattern = 0xFF;
			row = 0x00;
				
			/*if (row == 0xEF && pattern == 0x01) { // Reset demo 
				pattern = 0x80;		    
				row = 0xFE;
			} else if (pattern == 0x01) { // Move LED to start of next row
				pattern = 0x80;
				row = (row << 1) | 0x01;
			} else {  // Shift LED one spot to the right on current row
				pattern >>= 1;
			}*/
			break;
		default:
			break;
	}
	PORTC = pattern;	// Pattern to display
	PORTD = row;		// Row(s) displaying pattern	
	return state;
}



int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF; // Inputs
	DDRB = 0x7F; PORTB = 0x80; // Inputs
	DDRC = 0xFF; PORTC = 0x00; // Outputs
	DDRD = 0xFF; PORTD = 0x00; // Outputs

        //Task Setup
        task tasks[1];
	const unsigned short tasks_size = 1;
	
	tasks[0].state = shift;
	tasks[0].period = 100;
	tasks[0].elapsedTime = 100;
	tasks[0].TickFct = &Demo_Tick;

	//Timer
        TimerSet(1);
        TimerOn();

	unsigned short i;

    /* Insert your solution below */
    while (1) {

       	for(i=0; i<tasks_size; ++i) { // Scheduler code
		if(tasks[i].elapsedTime >= tasks[i].period) { // Task is ready to tick
			tasks[i].state = tasks[i].TickFct(tasks[i].state); // Set next state
			tasks[i].elapsedTime = 0; // Reset the elapsed time for next tick
		    }
			tasks[i].elapsedTime += 1;
		}
	

	  while(!TimerFlag);
	  TimerFlag = 0;
    }
    return 0; // Program should not exit
}


