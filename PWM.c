#include "lib/include.h"

//modulo 0, generador 0, 1 y 2
//M0PWM1->PF1 (generador 0, comparador B') p. 1233
//M0PWM2->PF2 (generador 1, comparador A')
//M0PWM4->PG0 (generador 2, comparador A')

extern void Configura_Reg_PWM1(void)
{
    SYSCTL->RCGCPWM |= (1<<0); //habilitar reloj de modulo PWM0 (RCGCPWM) p. 354
    SYSCTL->RCGCGPIO |= (1<<6)|(1<<5); //habilitar reloj de GPIO Puerto F y Puerto G (RCGCGPIO) p. 340
                                                                                                                                          
    GPIOF_AHB->AFSEL |= (1<<1); //habilitar funciones alternativas (AFSEL) p. 672
    GPIOF_AHB->AFSEL |= (1<<2); 
    GPIOG_AHB->AFSEL |= (1<<0); 
    //1->función alternativa
    //0->GPIO (I/O)
    GPIOF_AHB->DIR = (1<<1); //configura el pin correspondiente como I/O p. 663
    GPIOF_AHB->DIR = (1<<2); 
    GPIOG_AHB->DIR = (1<<0); 
    //0->entrada
    //1->salida

    GPIOF_AHB->PCTL |= (GPIOB->PCTL&0x0FFFF00F) | 0x00000660; //indicar funcion alternativa (PWM) (PCTL) p. 689, 1351
    GPIOG_AHB->PCTL |= (GPIOE->PCTL&0xFFF0FFF0) | 0x00000006; 
    //el 6 indica que es pwm, la posición que es el pin PF1, PF2 y PG0
    GPIOF_AHB->DEN |= (1<<1)|(1<<2); //pin digital p. 682
    GPIOG_AHB->DEN |= (1<<0); 
    
    PWM0->CC = (1<<8) | (0<<2) | (1<<1) | (0<<0); //habilitar divisor (RCC) p. 254 
    //# cuentas = 20,000,000/50 = 400 000-> (20,000,000/8)/50 = 50 000
    //el divisor se habilita si el numero de cuentas es mayor a 65 mil (16 bits)

    PWM0->_0_CTL = (0<<0); //deshabilitar generador (CTL) p. 1266, 1233
    PWM0->_1_CTL = (0<<0); 
    PWM0->_2_CTL = (0<<0);
    //1->se habilitda todo el bloque PWM
    //0->se deshabilita todo el bloque PWM
 
    PWM0->_0_GENB = (0X2<<2)|(0X3<<10)|(0x0<<0); //registro de las acciones del pwm (modo de configuración) p. 1285
    PWM0->_1_GENA = (0X2<<2)|(0X3<<6)|(0x0<<0);
    PWM0->_2_GENA = (0X2<<2)|(0X3<<6)|(0x0<<0);
    //0x0000008C para modo countdown pmA' (en la p. 1248 dice con que comparador trabajan)
    //0x0000080C para modo countdown pmB'
                                                     
    PWM0->_0_LOAD = 50000; //cuentas (carga = fclk/fpwm)
    PWM0->_1_LOAD = 50000;  
    PWM0->_2_LOAD = 50000; 
    PWM0->_0_CMPB = 5000; // valor inicial del ciclo de trabajo (50%)
    PWM0->_1_CMPA = 5000; 
    PWM0->_2_CMPA = 5000; 

    PWM0->_0_CTL = (1<<0); //activar generadores 
    PWM0->_1_CTL = (1<<0);
    PWM0->_2_CTL = (1<<0);

    PWM0->ENABLE = (1<<1)|(1<<2)|(1<<4); //habilitar el bloque p. 1248
}
