/* 
 * File:   Post-Lab07.c
 * Author: sando
 *
 * Created on 8 de abril de 2022, 06:54 PM
 */

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

// --------------- CONSTANTES --------------- //
 
#define BOTON1 PORTBbits.RB0    // Asignamos un alias a RB0
#define BOTON2 PORTBbits.RB1    // Asignamos un alias a RB1
#define _tmr0_value 217         // 5 ms tiempo de retardo
#define _XTAL_FREQ 8000000      // Oscilacion de 8MHz

//--------------- VARIABLES --------------- //
char TABLA[10]={0X3F, 0X06, 0X5B, 0X4F, 0X66, 0X6D, 0X7D, 0X07, 0X7F, 0X67};    // Se define una tabla para mostrar los valores en los Display 7 segmentos.

int banderas;
int unidad;
int decena;
int centena;
int variable;       // se llama contador
int residuo;

// --------------- PROTOTIPO DE FUNCIONES --------------- //
 
void setup(void);
int division(void);                 

//--------------- INTERRUPCIONES --------------- //
void __interrupt() isr (void){      // Interrupcion del TMR0 o Multiplexado
    
    if  (T0IF == 1){                // Multiplexado
        
        PORTBbits.RB2 = 0;           // limpiamos PORTB2
        PORTBbits.RB3 = 0;           // limpiamos PORTB3
        PORTBbits.RB4 = 0;           // limpiamos PORTB4
        
        INTCONbits.T0IF = 0;        // Reinicio de TRM0;
        TMR0 = _tmr0_value;         
        
        if (banderas == 0){         // Se usan las banderas para indicar el display a encender y luego de la sentencia se cambia el valor en la bandera
            PORTBbits.RB4 = 0;
            PORTBbits.RB2 = 1;
            PORTD = TABLA[centena];
            banderas = 1;
        }
        
        else if (banderas == 1){    // Se usan las banderas para indicar el display a encender y luego de la sentencia se cambia el valor en la bandera
            PORTBbits.RB2 = 0;
            PORTBbits.RB3 = 1;
            PORTD = TABLA[decena];
            banderas = 2;
        }
        
        else if (banderas == 2){    // Se usan las banderas para indicar el display a encender y luego de la sentencia se cambia el valor en la bandera
            PORTBbits.RB3 = 0;
            PORTBbits.RB4 = 1;
            PORTD = TABLA[unidad];
            banderas = 0;
        }
                    
    }
    
// --------------- INTERRUPCION IOCB --------------- //    
    if(RBIF == 1){                       // Si fue interrupción del PORTB
        if(!BOTON1){                // Verificamos si RB0 esta presionado 
            PORTC++;                // Incrementar PORTC 
        }
        if (!BOTON2){               // Si fue RB1 
            PORTC--;                // Decrementar PORTC
        }
        INTCONbits.RBIF = 0;        // Limpiamos bandera de interrupcion
    }    
        
    return;
}

// --------------- CICLO PRINCIPAL  --------------- //
void main(void) {
    setup();                            // Llamamos a la función de configuraciones
// ---------- LOOP PRINCIPAL ---------- //     
    while(1){
        division();                     // Llamamos a la funcion de divicion para dividir en unidades, decenas y centenas
        variable = PORTC; //            // Convertimos variable en PORTC para dividirlo 
    }
    return;
}

// --------------- CONFIGURACION --------------- //    
void setup(void){               // CONFIGURACION DE PUERTOS    

    ANSEL = 0;
    ANSELH = 0;                 // I/O digitales
    
    TRISB = 0x03;               // PORTB como salida, excepto RB0 y RB1
    TRISC = 0;                  // PORTC como salida
    TRISD = 0;                  // PORTD como salida
    
    PORTB = 0;                  // Limpiamos PORTB
    PORTC = 0;                  // Limpiamos PORTC
    PORTD = 0;                  // Limpiamos PORTD
    
// --------------- CONFIGURACION DE OSCILADOR --------------- //    
    OSCCONbits.SCS = 1;         // Oscilador interno
    OSCCONbits.IRCF = 0b0111;   // 8MHz
    
// --------------- CONFIGURACION DE INTERRUPCIONES --------------- //    
    INTCONbits.GIE = 1;         // Habilitamos interrupciones globales
    INTCONbits.T0IE = 1;        // Habilitamos interrupciones del TMR0
    INTCONbits.RBIE = 1;        // Habilitamos interrupciones del PORTB
    INTCONbits.RBIF = 0;        // Limpiamos bandera de interrupción del PORTB
    INTCONbits.T0IF = 0;        // Limpiamos bandera de interrupción del TMR0
 
// --------------- CONFIGURACION DE INTERRUPCIONES POR CAMBIO DE ESTADO --------------- //    
    IOCBbits.IOCB0 = 1;         // Habilitamos IOCB en RB0
    IOCBbits.IOCB1 = 1;         // Habilitamos IOCB en RB1    
    
// --------------- CONFIGURACION DE TIMER 0 --------------- //    
    OPTION_REGbits.T0CS = 0;    // Uso de reloj interno
    OPTION_REGbits.PSA = 0;     // Uso de Prescaler con TMR0
    OPTION_REGbits.PS = 0b0111; // Prescaler de 1:256 
       
    TMR0= _tmr0_value;          // Reiniciamos el TMR0 a 217 para tener un retardo de 5ms
    banderas = 0;               // Limpiamos la variable de banderas
}

// --------------- FUNCIONES --------------- //
int division(void){           
    centena = variable/100;     // Dividir PORTC entre 100 para obtener las centenas
    residuo = variable%100;     // Utilizar division para obtener el residuo
    decena  = residuo/10;       // El residuo se divide entre 10 para obtener decenas
    unidad = residuo%10;       // El residuo restante se coloca en las unidades
}   
 