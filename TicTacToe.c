	#include <stdint.h>
	#include <stdbool.h>
	#include "inc/hw_memmap.h"
	#include "driverlib/fpu.h"
	#include "driverlib/sysctl.h"
	#include "driverlib/rom.h"
	#include "driverlib/pin_map.h"
	#include "driverlib/uart.h"
    #include "grlib/grlib.h"
	#include "drivers/ili9341_240x320x262K.h"
	#include "utils/uartstdio.h"
	#include "driverlib/gpio.h"
    #define GPIO_PINS_ALL GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7
    //tab to store information what symbol is in exactly place in the board
    int tab[3][3]={3,3,3,3,3,3,3,3,3};
    //info who win, in my code it isnt used but with this u always could make winning scene in infinity loop after win
    int whoWin=0;

    tContext sContext;

void isWin(){

    int i = 0;
        //checking if there is win horizontaly
        for ( i = 0; i < 3; i++ ) {
            if ( tab[i][0] == tab[i][1] && tab[i][1] == tab[i][2] && tab[i][0] != 3 ) {
                //set red
                GrContextForegroundSet(&sContext, ClrRed);
                //drawing winning line
                GrLineDraw(&sContext, 40, 40+80*i, 280, 40+80*i);
                //updating info who won
                whoWin = tab[i][0];
                
                while(1){
                    //when winning program funcionality stopped in infinity loop, always could be replaced by winner/end screen
                }

               }
            //when its end of searching horizontal, reset i to use later
            if( i == 2 ){
                i = 0;
                break;
                }
        }
     //checking if there is win verticaly
     for ( i = 0; i < 3; i++ ) {
         if ( tab[0][i] == tab[1][i] && tab[1][i] == tab[2][i] && tab[0][i] != 3 ) {
             //set red
             GrContextForegroundSet(&sContext, ClrRed);
             //drawing winning line
             GrLineDraw(&sContext, 60+100*i, 20, 60+100*i, 220);
             //updating info who won
             whoWin = tab[0][i];
             while(1){
                //when winning program funcionality stopped in infinity loop, always could be replaced by winner/end screen
              }
         }
     }
    //checking if there is win diagonally
    if (tab[0][0] == tab[1][1] && tab[1][1] == tab[2][2] && tab[0][0] != 3) {
        //set red
        GrContextForegroundSet(&sContext, ClrRed);
        //drawing winning line
        GrLineDraw(&sContext, 40, 20, 280, 220);
        //updating info who won
        whoWin = tab[1][1];
        while(1){
            //when winning program funcionality stopped in infinity loop, always could be replaced by winner/end screen
        }
    }
    //checking if there is win diagonally but second possible win
    if (tab[2][0] == tab[1][1] && tab[1][1] == tab[0][2] && tab[2][0] != 3) {
        //set red
        GrContextForegroundSet(&sContext, ClrRed);
        //drawing winning line
        GrLineDraw(&sContext, 280, 20, 40, 220);
        //updating info who won
        whoWin = tab[1][1];
        while(1){
            //when winning program funcionality stopped in infinity loop, always could be replaced by winner/end screen
        }
    }

    return;

}

//using to draw circles player

void drawCircle(int x, int y){

    //set red
    GrContextForegroundSet(&sContext, ClrRed);
    GrCircleDraw(&sContext,60+100*x,40+80*y,20);
    //fill
    GrCircleFill(&sContext,60+100*x,40+80*y,20);
    //erasing inside circle to make only circumference
    GrContextForegroundSet(&sContext, ClrWhite);
    GrCircleDraw(&sContext,60+100*x,40+80*y,17);
    GrCircleFill(&sContext,60+100*x,40+80*y,17);

}

//using to draw crosses player

void drawCross(int x, int y){

    //set red
    GrContextForegroundSet(&sContext, ClrRed);
    //drawing cross
    GrLineDraw(&sContext,40+100*x,20+80*y,80+100*x,60+80*y);
    GrLineDraw(&sContext,80+100*x,20+80*y,40+100*x,60+80*y);

}

//drawing rectangle in your choices places (could also draw current red stay or erasing previous ones)

void drawChoice(int x, int y){

    GrLineDraw(&sContext,20+100*x,10+80*y,100+100*x,10+80*y);
    GrLineDraw(&sContext,20+100*x,70+80*y,100+100*x,70+80*y);
    GrLineDraw(&sContext,20+100*x,10+80*y,20+100*x,70+80*y);
    GrLineDraw(&sContext,100+100*x,10+80*y,100+100*x,70+80*y);

    return;
}

main(void)
{

	SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOK);
	SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOE);
	GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_0);
	GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_5); //pressing key
	GPIOPinTypeGPIOInput(GPIO_PORTK_BASE, GPIO_PIN_7);
	GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_4);
	GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_5);
	ROM_FPULazyStackingEnable();
	ROM_SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
	ILI9341_240x320x262K_Init();
	GrContextInit(&sContext, &g_sILI9341_240x320x262K);


	GrContextBackgroundSet(&sContext, ClrWhite);
	GrContextForegroundSet(&sContext, ClrBlack);
	//setting starting configurations
    int ruch = 1;
    int x = 1;
    int y = 1;

    //drawing board

    GrLineDraw(&sContext,110,0,110,240);
    GrLineDraw(&sContext,210,0,210,240);
    GrLineDraw(&sContext,0,80,320,80);
    GrLineDraw(&sContext,0,160,320,160);

    //drawing your current choice
    GrContextForegroundSet(&sContext, ClrRed);
    drawChoice( x, y );

    //infinity loop for program to work

	while(1)
	{
        //picking your choice to place
	    if(GPIOPinRead(GPIO_PORTB_BASE,GPIO_PIN_5) == GPIO_PIN_5){
	        if( tab[x][y] == 3 ){

	            if( ruch <= 9 ){

	                        if( ruch %2 == 1){
	                            drawCross(x,y);
	                            tab[x][y] = ruch%2;
	                        }

	                        else{
	                            drawCircle(x,y);
	                            tab[x][y] = ruch%2;
	                        }

	                        if( ruch >= 5 ){
	                        isWin();
	                        }

	                        ruch++;
	            }
	        }
	    }

	    //going up over the board

        if(!(GPIOPinRead(GPIO_PORTB_BASE,GPIO_PIN_0) == GPIO_PIN_0)){

            if( y >= 1 ){
                            //setbackground color
                            GrContextForegroundSet(&sContext, ClrWhite);
                            drawChoice(x,y);
                            y--;
                            //set red
                            GrContextForegroundSet(&sContext, ClrRed);
                            drawChoice(x,y);
                        }

        }
        //going down over the board
        if(!(GPIOPinRead(GPIO_PORTE_BASE,GPIO_PIN_5) == GPIO_PIN_5)){

            if( y <= 1 ){
                            //setbackground color
                            GrContextForegroundSet(&sContext, ClrWhite);
                            drawChoice(x,y);
                            y++;
                            //set red
                            GrContextForegroundSet(&sContext, ClrRed);
                            drawChoice(x,y);
                        }
        }
        //going right over the board
        if(!(GPIOPinRead(GPIO_PORTE_BASE,GPIO_PIN_4) == GPIO_PIN_4)){

            if( x <= 1 ){
                            //setbackground color
                            GrContextForegroundSet(&sContext, ClrWhite);
                            drawChoice(x,y);
                            x++;
                            GrContextForegroundSet(&sContext, ClrRed);
                            drawChoice(x,y);
                        }
        }
        //going left over the board
        if(!(GPIOPinRead(GPIO_PORTK_BASE,GPIO_PIN_7) == GPIO_PIN_7)){

            if( x >= 1 ){
                            GrContextForegroundSet(&sContext, ClrWhite);
                            drawChoice(x,y);
                            x--;
                            GrContextForegroundSet(&sContext, ClrRed);
                            drawChoice(x,y);
                        }
        }

        //seting speed
         ROM_SysCtlDelay(ROM_SysCtlClockGet()/100);

	}
}

