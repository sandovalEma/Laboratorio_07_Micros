/* 
 * File:   Lab_07.c
 * Author: sando
 *
 * Created on 6 de abril de 2022, 03:08 PM
 */
/*
// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define _XTAL_FREQ 4000000

//---------- CONSTANTES ----------//

#define BOTON1 PORTBbits.RB0     // Asignamos un alias a RB0
#define BOTON2 PORTBbits.RB1     // Asignamos un alias a RB1 

//---------- CONFIGURACION DE TMR0 ---------//

void resetTMR0(void){
    TMR0 = 67;                  // TMR0 en 100 ms
    INTCONbits.T0IF = 0;         // Limpiamos la bandera de TMR0
    
}

// ---------- INTERRUPCIONES ---------- //

void __interrupt() isr (void){
    if(INTCONbits.RBIF){
        if(!BOTON1){                // verificar si ha sido el puerto RB0
            PORTA++;                // incremento del PORTA
        }
        if(!BOTON2){                // verificar si ha sido el puerto RB0
            PORTA--;                // decremento del PORTA 
        }
        INTCONbits.RBIF = 0;        // se limpia la bandera de interrupcion
    }
    if(INTCONbits.T0IF){            // si fue la bandera de interrupcion del TMR0
        resetTMR0();
        PORTC++;
    }
    return; 
}

//---------- CONFIGURACION ----------//

void setup(void){
    
    OSCCONbits.IRCF = 0b0110;   // 4MHz
    OSCCONbits.SCS = 1;         // Oscilador interno
    
    ANSEL = 0;
    ANSELH = 0;                 // I/O digitales
        
    TRISA = 0;                  // PORTA como salida
    TRISB = 0b00000011;         // PORTB como salida, excepto los primeros bits
    TRISC = 0;                  // PORTC como salida
    PORTA = 0;                  // Limpiamos PORTA  
    PORTB = 0;                  // Limpiamos PORTB
    PORTC = 0;                  // Limpiamos PORTC
    
    INTCONbits.RBIE = 1;        // Habilitamos interrupciones del PORTB
    IOCBbits.IOCB0 = 1;         // Habilitamos interrupcion por cambio de estado en RB0
    IOCBbits.IOCB1 = 1;         // Habilitamos interrupcion por cambio de estado en RB1
    INTCONbits.RBIF = 0;        // Limpiamos la bandera de interrupcion
    
}


// ---------- CONFIGURACION DE INTERRUPCIONES --------- //

void configINT(void){
    INTCONbits.T0IE = 1;        // Habilitamos interrupcion por TMR0
    INTCONbits.T0IF = 0;        // Limpiamos la bandera de TMR0
    INTCONbits.GIE = 1;         // Habilitamos interrupciones globales
    INTCONbits.PEIE = 1;        // Habilitamos interrupciones perifericas
    
}

//---------- SET TMR0 ----------//

void setuptmr0(void){
    OPTION_REGbits.T0CS = 0;    // Habilitamos fuente interna de TMR0
    OPTION_REGbits.PSA = 0;     // PRESCALER ASSIGNED TO TIMER0
    OPTION_REGbits.PS2 = 1;     // PRESCALER ASSIGNED TO TIMER0
    OPTION_REGbits.PS1 = 1;     // PRESCALER ASSIGNED TO TIMER0
    OPTION_REGbits.PS0 = 1;     // PRESCALER ASSIGNED TO TIMER0 256
    
    resetTMR0();
}

void main(void) {                   
    setup();                        // Sentencia de condicional IF
    setuptmr0(); 
    configINT();
    while(1){
        
    }
    return; 
}
*/ 