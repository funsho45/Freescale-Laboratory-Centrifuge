/*
 Laboratory Centrifuge system

 This program simulates and controls a Laboratory Centrifuge system.


Created by Funsho Adebari

*/

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include <stdio.h>   /* all C inbuilt functions are declared in this header file*/


int CurrrntSpeed_value;   /*This is the name given to the integer variable that will store
                            the analogue value the potentiometer. */

int RequiredSpeed_value;  /*This is the name given to the integer variable that will store
                            the analogue value of the second potentiometer. */

int LightSensor_value;    /*This is the name given to the integer variable that will store
                            the Photosensor value */

int CurrrntSpeed;         /*This is the name given to the integer variable that will store
                            the Converted current speed value (Which will be in RPM) */

int RequiredSpeed;      /*This is the name given to the integer variable that will store
                            the Converted required speed value (Which will be in RPM) */

int LightSensor;         /*This is the name given to the integer variable that will store
                            the inverted photosensor value reading. */




int activate = 0;


/*This "delay"function pauses the program for the amount of time (in milliseconds) specified
as parameter.*/

void delay(int ms){
  int i,j;
  for (i=0;i<ms;i++)
  {
    for (j=0;j<329;j++)
       {}
  }
}
//LCD Functions /////////////////////////////////////////////////////////////////////////

 /* The following functions give us a basic functionality of the 20 x 4 LCD module.*/


#define LCD PORTA  //Data port for the LCD screen

/*These unsigned char variables will be used throughout the code to specify the PORTA pin numbers that
are connected to the control pins of the LCD. */

unsigned char RS_Pin= 0x20;   //The RS pin of the LCD is connected to the sixth pin of PORTA
unsigned char RW_Pin= 0x40;  // The RW pin of the LCD is connected to the seventh pin of PORTA
unsigned char E_Pin= 0x80;   // The E pin of the LCD is connected to the eighth pin of PORTA


 /* The "LCDcmd" function is used to send initialising commands to the
 LCD display which is why the register select pin is set to LOW. */

  void LCDcmd(char value)
{
  LCD = value;  // The arguments for this functions will be going to the LCD dataport


  PORTB=PORTB & (~(RS_Pin)); // RS_Pin = 0 (Sending command)
  PORTB=PORTB & (~(RW_Pin)); //  RW_Pin = 0 (Writing data)
  PORTB=PORTB | (E_Pin);  //  E_Pin = 1 (set E to 1 and enable LCD)
  delay (10);
  PORTB=PORTB & (~(E_Pin)); // E_Pin = 0 (For high to low pulse)


}

/* The "LCData" function is used to send data to the to data register of
 LCD display which is why the register select pin is set to HIGH.  */

void LCData(char value){
  LCD = value;



 PORTB=PORTB | (RS_Pin); //RS_Pin = 1 (Sending data)
 PORTB=PORTB & (~(RW_Pin));
 PORTB=PORTB | (E_Pin);
 delay (10);
 PORTB=PORTB & (~(E_Pin));
}

 /*  The "display" function write a string of characters */

void display(char *c){
    while (*c != 0)
       LCData(*c++);
  }

/*The "lcdnumber" function converts a convert a an integer number into a string.*/

void lcdnumber (int intergerToBeDisplayed)
{

  char stringNumber[5];
  (void)far_sprintf (stringNumber, "%d ",intergerToBeDisplayed );

  display(stringNumber);
}

/*The "LCDclear" function clears the LCD display*/

void LCDclear(void)
{
delay(2);
LCDcmd (0x01);   //Clear the display
LCDcmd (0x02);   //returns the display to its original status if it was shifted.
}



//////////////////////////////////////////////////////////////////////////////




void DELAY20uS (void){
  int x = 40;
  while(x--);
}

void wait(ulong iteration){
	while( iteration > 0){
		--iteration;
	}
}

void ATD_Init(void)
{
	ATDCTL2 = 0x80;
	/* 1 - ATD power down*/
	/* 0 - ATD fast flag clear*/
	/* 0 - ATD power down in wait mode*/
	/* 0 - External trigger level/edge control*/
	/* 0 - External trigger polarity*/
	/* 0 - External trigger mode enable*/
	/* 0 - ATD sequence complete interrupt enable*/
	/* 0 - ATD sequence complete interrupt flag*/

	DELAY20uS(); // Waiting at least 20us for the analog circuitry to stabilize and the ATD0 to get ready

	ATDCTL3 = 0x18;
	/* 0 - reserved*/
	/* 0011 - Conversion sequence length = 1*/
	/* 0 - Result register FIFO mode*/
	/* 00 - Background Debug Freeze Enable*/

	ATDCTL4 = 0x80;
	/* 1 - A/D resolution select - 8bit*/
	/* 00 - Sample time select - 2 A/D conversion clock periods*/
	/* 00000 - ATD clock prescaler*/
	/* bus clock 8MHz, PRS = 0*/
	/* ATD clock = BusClk/(PRS + 1)*0.5 = 2MHz*/

}



void main(void) {

  /*The functions here run only once when the
  microcontroller is turned on and the program begins.*/

DDRB = 0xEF;  //Make PORTB 0b11101111

DDRA = 0xFF;  //Make PORTA 0b11111111 (An output)


LCDcmd(0x38);   //5x7 matric LCD
LCDcmd(0x0C);   // display on cursor off
LCDclear();

 ATD_Init();    /*Initialise the A/D*/


    PORTB_BIT0 = 0xFF;  //Turn the green LED (or Start Light) on
    PORTB_BIT2 = 0xFF;  //Turn the red LED (or Break Light) on
    wait(200000);
    PORTB_BIT0 = 0x00;   //Turn the green LED (or Start Light) off
    PORTB_BIT2 = 0x00;   //Turn the red LED (or Break Light) off




  for(;;){    /*Endless loop*/

   ATDCTL5 = 0x94; /*Start of conversion; channel AN5*/

   while (ATDSTAT0_SCF == 0){} /*Wait until the A/D produce result*/

   LightSensor_value = ATDDR0L; /*Read the result and store it in LightSensor_value */

   LightSensor = 255 - LightSensor_value;  /*The value gets lower the more intense the light received to the sensor is,
                                             so we invert the reading from 0-255 to 255-0 (Note: LightSensor = LightSensor_value )*/


   CurrrntSpeed_value = ATDDR1L;  /*Read the result and store it in CurrrntSpeed_value */

   CurrrntSpeed = CurrrntSpeed_value  * (10000/255);/*This is to convert the POT values 0-255 to an  RPM between 0 to 10000.
                                                       We then store the value in the int variable "CurrrntSpeed".*/

   RequiredSpeed_value = ATDDR2L;   /*Read the result and store it in RequiredSpeed_value */
   RequiredSpeed = RequiredSpeed_value * (10000/255);




   /*If the pushbutton (PORTB_BIT4)is pressed, turn the green LED (or Start Light) on
   And assign the value 1 to the "activate" variable*/

    if (!PORTB_BIT4){

      PORTB_BIT0 = 0xFF;  //Green light

      activate = 1;

    }

    /* If the activate variable is equal to 1, Carry out the code and the code block and begin the program. */
    if (activate == 1 ){

       /* If the Light sensor is greater than 128, make the current speed equal 0 turn off
       the blue LED or Speed Reached Light (PORTB_BIT2) and make the yellow LED or Out Of Balance Light (PORTB_BIT3) blink  */

       if (LightSensor>0x80){
        CurrrntSpeed=0;
        PORTB_BIT2 = 0xFF;
        PORTB_BIT3 = PORTB_BIT3 ^ 0xFF; //Turn the yellow LED or Out Of Balance Light
       // wait(50);
      }  else  PORTB_BIT2 = 0x00, PORTB_BIT3 = 0x00; /*In any other case, both LEDs should be off*/


     /* if the CurrrntSpeed value is greater than or equal to and less than or equal to ±250 the required speed value  */

     if ((CurrrntSpeed<=(RequiredSpeed +250)) && (CurrrntSpeed >=(RequiredSpeed -250))){


      PORTB_BIT1 = 0xFF;   // //Turn the Blue LED (or Speed Reached Light)
     } else PORTB_BIT1 = 0x00;  /*In any other case, the LED should be off*/



         /* To display the strings and converted analogue values on the 20 x 4 LCD display being used for this program,
           the functions declared earlier for the LCD display will be utilised.  */

        LCDcmd(0x80);   //Sends the command to the LCD to force the cursor to the 1st row
        display("*****LCD SCREEN*****");

        LCDcmd(0xC0);   // Sends the command to the LCD to force the cursor to the 2nd row

        display("Req.Speed:"); // Used the "display" function to write "Req.Speed" string on the second row on the LCD display

        LCDcmd(0xCA); // Sends the command to the LCD to force the cursor to the "CA" on the 20x4 LCD

        lcdnumber(RequiredSpeed); /* Used the "lcdnumber" function to write Current required speed analogue value from
                                     the potentiometer and writes the data as a string on the "CA" position the 20x4 LCD display */


        /* To combat the problem of the LCD remembering the previous character written before, the if conditional statements written below
            essentially "erases" the characters and overwrites them with a space (" ")   */

        if (RequiredSpeed < 1000) display(" ");  // 0-999 add one space
        if (RequiredSpeed < 100) display(" ");  // 0-99 add one space
        if (RequiredSpeed < 10) display(" ");  // 0-9 add one space
        ////////
        LCDcmd(0xCF);
        display("RPM");



        LCDcmd(0x94);   //Sends the command to the LCD to force the cursor to the 3rd row
        display("Curr.Speed:");
        LCDcmd(0x9F);
        lcdnumber(CurrrntSpeed);
        ////////
        //if (CurrrntSpeed < 1000) display(" ");
        if (CurrrntSpeed < 100) display(" ");
        if (CurrrntSpeed < 10) display(" ");
        ////////
        LCDcmd(0xA4);
        display("RPM");




        LCDcmd(0xD4);   //Sends the command to the LCD to force the cursor to the 4th row
        display("Balance Signal:");
        LCDcmd(0xE3);
        lcdnumber(LightSensor);
        ////////
        if (LightSensor < 1000) display(" ");
        if (LightSensor < 100) display(" ");
        if (LightSensor < 10) display(" ");

    }


  }



  }/* loop forever */
  /* please make sure that you never leave main */


