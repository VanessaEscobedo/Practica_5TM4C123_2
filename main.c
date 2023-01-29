#include "lib/include.h"

uint16_t Result[3]; //variables
uint16_t duty[3];

//Experimento 2
/* Usando el modulo 0 de PWM con una frecuencia de reloj del sistema de 20,000,000 Hz
 * junto con el generador 0,1,2  habilitar alguno de los pwm's asociados y obtener un PWM
 * cuya frecuencia sea de 50Hz. Con tres potenciometros variar el ciclo de trabajo
 * para controlar la posicion de tres servos sg90 o otros.
 *
 */

int main(void)
{
    Configurar_PLL(); //configuracion velocidad de reloj
    Configura_Reg_PWM0(8); //configuracion PWM
    Configura_Reg_ADC0(); //configuracion ADC

    while(1) 
    {

        ADC0_InSeq2(Result,duty); //leer los valores del ADC

        //ciclo de trabajo
        PWM0->_0_CMPB = duty[0]; //canal 8
        PWM0->_1_CMPA = duty[1]; //canal 2
        PWM0->_2_CMPA = duty[2]; //canal 1
    }
}
