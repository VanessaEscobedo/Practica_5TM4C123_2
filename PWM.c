#include "lib/include.h"

//modulo 0, generador 0, 1 y 2
//M0PWM1->PB7 (generador 0) p. 1233
//M0PWM2->PB4 (generador 1)
//M0PWM4->PE4 (generador 2)

extern void Configura_Reg_PWM1(void)
{
    SYSCTL->RCGCPWM |= (1<<0); //habilitar reloj de modulo PWM0 (RCGCPWM) p. 354
    SYSCTL->RCGCGPIO |= (1<<1)|(1<<4); //habilitar reloj de GPIO Puerto B y Puerto E (RCGCGPIO) p. 340
                                                                                          //  frec, tarjeta / frec PWM = cuentas 
    SYSCTL->RCC |= (1<<20)|(0<<19)|(1<<18)|(0<<17); //habilitar divisor (RCC) p. 254 
    //# cuentas = 20,000,000/50 = 400 000-> (20,000,000/8)/50 = 50 000
    //el divisor se habilita si el numero de cuentas es mayor a 65 mil (16 bits)
                                                    
    GPIOB->AFSEL |= (1<<4); //habilitar funciones alternativas (AFSEL) p. 672
    GPIOB->AFSEL |= (1<<7); 
    GPIOE->AFSEL |= (1<<4); 
    //1->función alternativa
    //0->GPIO (I/O)
    GPIOB->DIR = (1<<4); //configura el pin correspondiente como I/O p. 663
    GPIOB->DIR = (1<<7); 
    GPIOE->DIR = (1<<4); 
    //0->entrada
    //1->salida

    GPIOB->PCTL |= (GPIOB->PCTL&0x0FF0FFFF) | 0x40040000; //indicar funcion alternativa (PWM) (PCTL) p. 689, 1351
    GPIOE->PCTL |= (GPIOE->PCTL&0xFFF0FFFF) | 0x00040000; 
    //el 4 indica que es pwm, la posición que es el pin PB4, PB7 y PE4
    GPIOB->DEN |= (1<<4)|(1<<7); //pin digital p. 682
    GPIOE->DEN |= (1<<4); 

    PWM0->_0_CTL = (0<<0); //deshabilitar generador (CTL) p. 1266, 1233
    PWM0->_1_CTL = (0<<0); 
    PWM0->_2_CTL = (0<<0);
    //1->se habilitda todo el bloque PWM
    //0->se deshabilita todo el bloque PWM
 
    PWM0->_0_GENB = 0x0000080C; //registro de las acciones del pwm (modo de configuración) p. 1285
    PWM0->_1_GENA = 0x0000008C; 
    PWM0->_2_GENA = 0x0000008C; 
    //0x0000008C para modo countdown pmA' (en la p. 1248 dice con que comparador trabajan)
    //0x0000080C para modo countdown pmB'
                                                     
    PWM0->_0_LOAD = 50000; //cuentas (carga = fclk/fpwm)
    PWM0->_1_LOAD = 50000;  
    PWM0->_2_LOAD = 50000; 
    PWM0->_0_CMPB = 25000; // valor inicial del ciclo de trabajo (50%)
    PWM0->_1_CMPA = 25000; 
    PWM0->_2_CMPA = 25000; 

    PWM0->_0_CTL = (1<<0); //activar generadores 
    PWM0->_1_CTL = (1<<0);
    PWM0->_2_CTL = (1<<0);

    PWM0->ENABLE = (1<<1)|(1<<2)|(1<<4); //habilitar el bloque p. 1248
}

