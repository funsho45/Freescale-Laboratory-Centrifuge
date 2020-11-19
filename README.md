# Table of Contents
* [What is this?](#what-is-this) 	
* [Methodology](#methodology)
* [Problem Analysis](problem-analysis)
   * [Summary of Freescale semiconductor module connections](#summary-of-freescale-semiconductor-module-connections)
* [Flowchart](#flowchart)	
* [Debugging strategy](#debugging-strategy)	
* [Video demonstration of the Laboratory Centrifuge program (YouTube)](#video-demonstration-of-the-laboratory-centrifuge-program-(youtube))



# Freescale Laboratory Centrifuge
The control and implementation of an embedded system simulationg a laboratory centrifuge using the 4MHz Freescale HSC12 microcontroller board 

# What is this?
For this project, the task is to design a laboratory centrifuge, medical equipment driven by a motor that spins liquid samples and high speeds, for a manufacturer that develop medical equipment. A 4MHz Freescale HSC12 microcontroller board shown in Figure 1, is used to design, program and implement an embedded system. The program is written in the C programming language.

The Freescale Codewarrior integrated development environment (IDE) and its simulation tools for the MC9S12C128 module will be used to write and debug the program. After debugging the program, the laboratory centrifuge will be simulated with two potentiometers (POTs), a photosensor and four different coloured light-emitting diodes (LED). This will allow the program to simulate, and for the user to control, the current and desired speeds of the centrifuge as well as the vibrations and give indications through the LEDs.

![Freescale PBMCUSLK Axiom MCU Microcontroller Project Board](https://user-images.githubusercontent.com/73448401/99710254-3a832180-2a98-11eb-9de6-10a0aa40eb72.jpg)
   
Figure 1: Freescale PBMCUSLK Axiom MCU Microcontroller Project Board

# Methodology

Before beginning the program, it is important to declare the control registers for the analogue to digital converter (ATD) for the two POTs, and the photosensor used to simulate the laboratory centrifuge. Declaring the ports on the Freescale board to be an input or output through the DDR registers will also then need to be made. Another benefit of utilising the C programming language is allowing the use of simpler functions written for the of the centrifuge to be called throughout the program, allowing for a more complex program to be broken into smaller chunks. Compiling and debugging of the programme will also be essential for testing for any problems or errors that may arise.

The laboratory centrifuge will first be initialised through a pushbutton switch with a green LED turning on indicating that the device is on. Once on, there will be two POTs that the user can manipulate. One POT will control and simulate the current motor speed (RPM) of the centrifuge, while the second POT will simulate the desired speed the user wants the centrifuge to reach, for this program that will be 9750 RPM with a tolerance of ± 250. The maximum motor speed the centrifuge can reach is 10,000 RPM, and once the current speed of the current speed reaches the desired speed a blue LED will turn on, indicating that the desired speed has been reached. If the vibrations, simulated through a photosensor, in the centrifuge are out of balance and reach a value higher than 128, then a red LED will turn on representing that the motor has stopped and yellow LED will begin to blink representing that the centrifuge is out of balance due to the high vibrations.

The values of the current speed, the desired speed and the photosensor values will be displayed on a 20x4 Liquid Crystal Display (LCD) screen with a blue LED backlight and will be set as a digital output. 

As mentioned previously, the Freescale CodeWarrior 5.9.0 IDE will be used for programming the Freescale HSC12 microcontroller. The circuit diagram used to connect the LEDs to the microcontroller are shown in Figure 2, and the physical connections of the LED indicators, as well as other components, are shown in Figure 3. The four LEDs being used will be set as digital output signals; the two POTs are being used, as well as the photosensor, will have their analogue signals is converted to digital through an ATD. 

|     Description                                                                  |     Signal            |
|----------------------------------------------------------------------------------|-----------------------|
|     Pushbutton switch                                                            |     DIGITAL INPUT     |
|     Start light indicator (GREEN LED)                                            |     DIGITAL OUTPUT    |
|     Reached Speed light indicator (BLUE LED)                                     |     DIGITAL OUTPUT    |
|     Brake light indicator (RED LED)                                              |     DIGITAL OUTPUT    |
|     Out of balance light indicator (YELLOW LED)                                  |     DIGITAL OUTPUT    |
|     Required Speed simulator (POT)                                               |     ANALOGUE INPUT    |
|     Current speed simulator (POT)                                                |     ANALOGUE INPUT    |
|     Out of balance vibrations simulator (Photosensor)                            |     ANALOGUE INPUT    |
|     Display for Current and desired speed and for vibration values (20x4 LCD)    |     DIGITAL OUTPUT    |

Table 1: A description of the functions for the laboratory centrifuge simulation and its appropriate signals

![Circuit schematic for the four LEDs](https://user-images.githubusercontent.com/73448401/99714684-cba8c700-2a9d-11eb-94e2-c563dbd1dc88.jpg)

Figure 2: Circuit schematic for the four LEDs

![image](https://user-images.githubusercontent.com/73448401/99714832-f98e0b80-2a9d-11eb-8da0-dc8a42a5279a.png)

Figure 3: Freescale semiconductor connections of the Laboratory Centrifuge

# Problem Analysis

The registers for the ATD need to be declared before initiating and running the program. ATDCTL2 is the first register that is declared and is used to turn on the actual ATD. One of the most important factors of the ATDCTL2 is the ADPU, which was used in the program to turn on the ADC section of the HCS12. Other aspects of the ATDCTL2 register used in the program was setting the AFFC bit = 0 for normal flag clearing, which is the default option, and having the AWAI bit =0 so that the ATD continues to run in wait mode. The external trigger level and polarity bits were set to a falling edge (ETRIGLE= 0: ETRIGP = 0). The bits that were disabled were the external trigger mode (ETRIGE=0), and the ATD sequence complete interrupt (ASCIE=0), with no ATD, interrupt occurring with the ASCIF bit = 0 (Mazidi & Causey, 2009). The full declaration for ATDCTL2 is shown in Table 2.

|     ADPU    |     AFFC    |     AWAI    |     ETRIGLE    |     ETRIGP    |     ETRIGE    |     ASCIE    |     ASCIF    |
|-------------|-------------|-------------|----------------|---------------|---------------|--------------|--------------|
|     1       |     0       |     0       |     0          |     0         |     0         |     0        |     0        |

Table 2: ATDCTL2 (A/D Control Register 2)

Another significant control register of the HCS12 is the ATDCTL3, which is utilised to select the number of conversions per sequence. For this program, the desired number of conversions per sequence was three (S1C =1 S2C=1), two for both POTs and one for the photosensor, which results were obtained from the ATDDDR registers and inputs converted by the ATD. The conversion still happens during the breakpoint (FRZ1=0, FRZ0=0), with the conversion result placed in the corresponding result register (FIFO=0).The full declaration for ATDCTL3 is shown in Table 3.

|     0    |     S8C    |     S4C    |     S2C    |     S1C    |     FIFO    |     FRZ1    |     FRZ0    |
|----------|------------|------------|------------|------------|-------------|-------------|-------------|
|     0    |     0      |     0      |     1      |     1      |     0       |     0       |     0       |

Table 3: ATDCTL3 (A/D Control Register 3)

For the HCS12 ATD the resolution, sampling time, and conversion time are programmable using the ATDCTL4. For this program, an eight-bit resolution was chosen by setting the SRES bit = 1. To have the length of the A/D clock period to be two, the sample time bits were SMP1 = 0 and the SMP0 = 0. Moreover, the A/D clock pre-scaler bits were set as PRES4=0, PRES3=0, PRES2=0, PRES1=0, PRES0=0 to divide the bus clock by two. The full declaration for ATDCTL4 is shown in Table 4.

|     SRES8    |     SMP1    |     SMP0    |     PRS4    |     PRS3    |     PRS2    |     PRS1    |     PRS0    |
|--------------|-------------|-------------|-------------|-------------|-------------|-------------|-------------|
|     1        |     0       |     0       |     0       |     0       |     0       |     0       |     0       |

Table 4: ATDCTL4 (A/D Control Register 4)

For the program, one input will be utilised via the push switch button, and four outputs for the LEDs will be used. In the program, the various inputs and outputs have been declared. Bits 0 to 3 for PORTB are set up as outputs, with bits 4 to 7 as inputs by setting its data direction register (DDR) as DDRB=0x0F. PORTA will be used for the LCD screen with its DDR being set as an output (DDRA=0xFF). The initiation of the ATD converter starts and then, the initial state the program begins with the start (PORTB_BIT0) and break (PORTB_BIT2) LEDs turning on and off.

In the endless for loop, the ATDCTL5 register is declared and is utilised to select the input analogue and the A/D conversion is started by writing to this register. The result register data justification bit controls the justification of the conversion data in the result register and was set to be right-justified (DJM=1). The result placed in the register was represented in an unsigned data format (DSGN=0). The conversion sequence was set to be continuous in SCAN mode (SCAN=1). The multichannel bit was set to MULT=1, as three channels were being converted for this program and the address of the channels were given by the channel selection bits, CC=1, CB=0, CA=0. The full declaration for ATDCTL5 is shown in Table 5.

|     DJM    |     DSGN    |     SCAN    |     MULT    |     0    |     CC    |     CB    |     CA    |
|------------|-------------|-------------|-------------|----------|-----------|-----------|-----------|
|     1      |     0       |     0       |     1       |     0    |     1     |     0     |     0     |

Table 5: ATDCTL5 (A/D Control Register 5)

The conversion results for the first and second POTs on the MCU and the Freescale board were stored in the ATDDR0L and ATDDR1L variables respectively, while the results the photosensor was stored in the variable ATDDR2L.

The results stored in ATDDR0L and ATDDR1L registers were then placed in integer data types called “CurrrntSpeed_value” and “RequiredSpeed_value” to help with readability the program. The values were then converted to a rotation per minute (RPM) range between 0 to 10,000 by multiplying the value by 10000/255. Once the conversions were done, they were then stored in variables named “CurrrntSpeed” and “RequiredSpeed” again to help with readability for the program.

Before the main process of the program begins an if conditional statement was written so that the programme would only initiate once the push button switch has been pressed (PORTB_BIT4) and the green LED (PORTB_BIT0) is on. The global variable “activate”, declared before the main function, was assigned the value of one, with the if statement condition being that if “activate” is equal to one carry out the code in the code block and begin the program. Once the push button is pressed and the green LED is on the current speed, and the required speed will be able to be manipulated with POTs one and two. Another if conditional statement was written to allow the blue LED light (PORTB_BIT1), representing that the required speed had been reached, to turn on if the CurrrntSpeed value is greater than or equal to and less than or equal to 250 below or above the required speed value. Moreover, If the condition was false, the blue LED would remain in an off state.

The photosensor results stored in ATDDR2L were placed in the integer variable named “LightSensor” to help with readability for the program. Moreover, an if conditional statement was set that if the light sensor value was greater than the hex number 0x80 (or the decimal value of 128), then the current speed will be assigned a zero value with the red LED(PORTB_BIT2) representing the brake signal turning on and the yellow LED(PORTB_BIT3) representing the out of balance indication signal blinking. If the condition was not true, the red LED, and the yellow LED would remain off. 

After the pushbutton switch is pressed and the green LED is on, the already initialised LCD will display the string characters and sensor values of the two POTs and the photosensor. The LCD command function “LCDcmd” displays the address of the cursor positions for the first, second, third and fourth rows on the 20x4 LCD. The “display” function was used to a write a string of characters to the LCD for the first, second, third and the fourth rows, displaying the appropriate labels for the current speed, required speed, photosensor and an appropriate title. To display the analogue values of the two POTs and the photosensor, the integer values were converted into string format in order for them to be displayed on the LCDs. The readings were then displayed in the appropriate cursor positions on the LCD screen in conjunction with their labels, for example, the “lcdnumber (CurrrntSpeed);”   was displayed in the cursor position 9F. Figure 4 shows the 20x4 LCD and Table 6 shows its cursor address.

![image](https://user-images.githubusercontent.com/73448401/99716434-027fdc80-2aa0-11eb-8aab-8f29f6a697e3.png)

Figure 4: 20x4 Character LCD Module Display

|     20x4 LCD    |           |           |           |           |           |           |           |           |           |           |           |           |           |           |           |           |           |           |           |
|-----------------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|
|     80          |     81    |     82    |     83    |     84    |     85    |     86    |     87    |     88    |     89    |     8A    |     8B    |     8C    |     8D    |     8E    |     8F    |     90    |     91    |     92    |     93    |
|     CO          |     C1    |     C2    |     C3    |     C4    |     C5    |     C6    |     C7    |     C8    |     C9    |     CA    |     CB    |     CC    |     CD    |     CE    |     CF    |     D0    |     D1    |     D2    |     D3    |
|     94          |     95    |     96    |     97    |     98    |     99    |     9A    |     9B    |     9C    |     9D    |     9E    |     9F    |     A0    |     A1    |     A2    |     A3    |     A4    |     A5    |     A6    |     A7    |
|     D4          |     D5    |     D6    |     D7    |     D8    |     D9    |     DA    |     DB    |     DC    |     DD    |     DE    |     DF    |     E0    |     E1    |     E2    |     E3    |     E4    |     E5    |     E6    |     E7    |

Table 6: Cursor address for a 20x4 LCD

# Summary of Freescale semiconductor module connections

|     Reference   Description                                  |     Signal and   Description                                                            |
|--------------------------------------------------------------|-----------------------------------------------------------------------------------------|
|     Pushbutton switch                                        |     PB4 was set up as a DIGITAL INPUT                                                   |
|     Start light indicator                                    |     PB0 was set up as a DIGITAL OUTPUT                                                  |
|     Reached Speed light indicator                            |     PB1 was set up as a DIGITAL OUTPUT                                                  |
|     Brake light indicator                                    |     PB2 was set up as a DIGITAL OUTPUT                                                  |
|     Out of balance light indicator                           |     PB3 was set up as a DIGITAL OUTPUT                                                  |
|     Required Speed simulator                                 |     Was assigned   to PAD06 as an ANALOGUE INPUT                                        |
|     Current speed simulator                                  |     Was assigned   to PAD05 as an ANALOGUE INPUT                                        |
|     Out of balance vibrations simulator                      |     Was assigned   to PAD04 as an ANALOGUE INPUT                                        |
|     20x4 LCD to monitor   speeds and the vibration values    |     Data pin D0   on the 20x4 LCD was assigned to PA0 and was   set up as an output     |
|                                                              |     Data pin D0   on the 20x4 LCD was assigned to PA0 and was   set up as an output.    |
|                                                              |     Data pin D1   on the 20x4 LCD was assigned to PA1 and was   set up as an output.    |
|                                                              |     Data pin D2   on the 20x4 LCD was assigned to PA2 and was   set up as an output.    |
|                                                              |     Data pin D3   on the 20x4 LCD was assigned to PA3 and was   set up as an output.    |
|                                                              |     Data pin D4   on the 20x4 LCD was assigned to PA4 and was   set up as an output.    |
|                                                              |     Data pin D5   on the 20x4 LCD was assigned to PA5 and was   set up as an output.    |
|                                                              |     Data pin D6   on the 20x4 LCD was assigned to PA6 and was   set up as an output.    |
|                                                              |     Data pin D7 on the 20x4 LCD was assigned to PA7 and was set up as an output.        |

Table 7: Summary of Freescale semiconductor module connections

# Flowchartfor the program
![image](https://user-images.githubusercontent.com/73448401/99717110-dc0e7100-2aa0-11eb-863e-fd2e4c17ab4a.png)

# Debugging strategy

The program needed to begin once the push button was pressed and the green LED was on. An if statement was created with the condition that unless the push button was pressed, the LED would turn on. However, there was no condition for the program to begin if the pushbutton was pressed, letting the program function regardless of the pushbutton. So, the integer called “activate” was created as a global variable storing the value of zero, and was placed in an if statement with the condition being that if the “activate” integer variable was equal to one, then the program would begin. So, the previous if statement before this was written as the following:

   
    if (!PORTB_BIT4){
      PORTB_BIT0 = 0xFF;  //Green light
      activate = 1; 
    }
   
This made the activate variable equal to one, which in turn allowed the condition of the if statement after this be true, but only when the pushbutton had been pressed which was what had been desired.
To display the analogue values of the POTs and of the photosensor a function was created to convert the integer values of the analogue readings into a string format. The way the function would work is that whatever number value placed into the function it will return a string equivalent of that value. The “sprint()” function utilised to convert the number value to the string and the “LCData()” function created before was used to display them on the LCD screen as shown below:
```
void lcdnumber (int intergerToBeDisplayed)
{  
  char stringNumber[5];
  (void)far_sprintf (stringNumber, "%d ",intergerToBeDisplayed );
  LCData(stringNumber);
}
```
However, the problem with this function was the inclusion of the “LCData()” function, as when displaying a string which was more than one character it will only display the first letter of that string. The more appropriate function to include was the “display” function, as that was able to display more than one character from the string inputted and resolved the problem, giving the function the new form shown below:
```
void lcdnumber (int intergerToBeDisplayed)
{  
  char stringNumber[5];
  (void)far_sprintf (stringNumber, "%d ",intergerToBeDisplayed );
  display(stringNumber);
}
```
It is important that each initiation of the LCD that it clears the previous data written to the LCD. The “LCDclear” function does just that and is inputted into the main function so that it clears to display only once so that new data can be written to the LCD. However, when it comes to the analogue values converted to string format from the two POTs and the photosensor, which are dynamically changing, the characters stay there until the LCD is overwritten again. An attempt to solve this issue was to put the “LCDclear()” function after the analogue value, as shown below:
```
LCDcmd(0xC0);   // 2nd row
display("Req.Speed:");//POINTER METHOD
LCDcmd(0xCA);
lcdnumber(RequiredSpeed);
LCDclear();
LCDcmd(0xCF);
display("RPM");
```
The problem with this, however, is that running this code on the LCD screen causes the it to flicker, as it is constantly clearing the written data on the LCD since its in a forever for loop. To solve this, the characters of strings representing the analogue values was overwritten with a blank space (“ ”) to give it the impression that the characters were erased. This solution was placed in an if conditional statement saying that if the analogue value was less than 1000, 100 or 10 the previous value will be overwritten for bank space, allowing for new data or a new analogue value to be written and displayed as shown below.
```
LCDcmd(0xC0);   // 2nd row
        display("Req.Speed:");//POINTER METHOD
        LCDcmd(0xCA);
        lcdnumber(RequiredSpeed);
        ////////
        if (RequiredSpeed < 1000) display(" ");
        if (RequiredSpeed < 100) display(" ");
        if (RequiredSpeed < 10) display(" ");
        ////////
        LCDcmd(0xCF);
        display("RPM");
```
# Video demonstration of the Laboratory Centrifuge program (YouTube)

This is a video clip demonstration uploaded to my YouTube channel of the program’s performance. Click the image below :)
[![](http://img.youtube.com/vi/aTAbPgXhVFk/0.jpg)](http://www.youtube.com/watch?v=aTAbPgXhVFk "")
