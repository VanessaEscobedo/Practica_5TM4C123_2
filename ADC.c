#include "lib/include.h"

//modulo 0 (3 canales para los 3 potenciometros )
//sec0->canal 8 (PE5)
//sec1->canal 2 (PE1)
//sec2->canal 1 (PE2)

extern void Configura_Reg_ADC0(void)
{
    SYSCTL->RCGCADC = (1<<0); //inicializar el modulo 0 de reloj del adc (RCGCADC) p. 352 
    //                     F     E      D       C      B     A      
    SYSCTL->RCGCGPIO |= (1<<5)|(1<<4)|(1<<3)|(0<<2)|(0<<1)|(1<<0); //habilitar reloj puertos A, D, E y F p. 340, 801
    //ADC->puerto E
    
    //habilita los pines como I/O (GPIODIR) p. 663
    GPIOE_AHB->DIR = (0<<2) | (0<<1) | (0<<5); //PE2, PE5 y PE1
    //1->salida
    //0->entrada
    
    GPIOE_AHB->AFSEL = (1<<2) | (1<<1) | (1<<5); //habilitar funciones alternativas, para que el modulo analógico tenga control de esos pines (GPIOAFSEL) p. 672 
    
    GPIOE_AHB->DEN = (0<<2) | (0<<1) | (0<<5);  //deshabilita el modo digital (GPIODEN) p. 683
    
    GPIOE_AHB->PCTL = GPIOE_AHB->PCTL & (0xFF0FF00F);  //registro combinado con el GPIOAFSEL y la tabla p. 1351, p. 688 (GPIOPCTL) 
    //la posición indica el pin, el 0 que es ADC
    
    GPIOE_AHB->AMSEL = (1<<2) | (1<<1) | (1<<5); //habilitar analogico (GPIOAMSEL) p. 687 
    
    //establecer la velocidad de conversión por segundo p. 891
    ADC0->PC = (0<<2)|(1<<1)|(1<<0); //para config. a 250 ksps (0x3)
    
    //configurar la prioridad de los secuenciadores (SSPRI) p. 841
    ADC0->SSPRI = 0x3210; //mayor prioridad sec0
    
    //este registro controla la activación de los secuenciadores p. 821
    ADC0->ACTSS  =   (0<<3) | (0<<2) | (0<<1) | (0<<0); //primero se desactivan para config.
    
    //este registro (ADCEMUX) selecciona el evento que activa la conversión (trigger) p. 834
    ADC0->EMUX  = (0x0000); //se configura por procesador
    
    //este registro (ADCSSMUX2) define las entradas analógicas con el canal y secuenciador seleccionado p. 867
    ADC0->SSMUX0 = 0x0008; //canal 8
    ADC0->SSMUX1 = 0x0002; //canal 2
    ADC0->SSMUX2 = 0x0001; //canal 1
    
    //este registro (ADCSSCTL2), configura el bit de control de muestreo y la interrupción p. 868
    ADC0->SSCTL0 = (1<<2) | (1<<1);
    ADC0->SSCTL1 = (1<<2) | (1<<1);
    ADC0->SSCTL2 = (1<<2) | (1<<1);
    
    //*enable ADC Interrupt interrumpe la conversión p. 825
    ADC0->IM |= (1<<0) | (1<<1) |(1<<2); //para indicar que manda la señal
    //NVIC_PRI4_R = (NVIC_PRI4_R & 0xFFFFFF00) | 0x00000020;
    //NVIC_EN0_R = 0x00010000;
    
    //p. 821 (ADCACTSS) Este registro controla la activación de los secuenciadores
    ADC0->ACTSS = (0<<3) | (1<<2) | (1<<1) | (1<<0); 

    ADC0->PSSI |= (1<<0) | (1<<1) | (1<<2); //se habilita el registro cuando se configura por procesador 
}

extern void ADC0_InSeq2(uint16_t *Result,uint16_t *duty)
{
       //ADC Processor Sample Sequence Initiate (ADCPSSI)
       ADC0->PSSI = 0x00000007; //por procesador
       
       while((ADC0->RIS&0x04)==0){}; 
      
       Result[0] = ADC0->SSFIFO0&0xFFF; //se almacena el valor en el FIFO
       duty[0] = (Result[0]*50000)/4096; 

       Result[1] = ADC0->SSFIFO1&0xFFF;
       duty[1] = (Result[1]*50000)/4096;
       
       Result[2] = ADC0->SSFIFO2&0xFFF;
       duty[2] = (Result[2]*50000)/4096;
       
       ADC0->ISC = 0x0007;  //conversion finalizada
}


