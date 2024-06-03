#include <stdbool.h>
#include <stm8s.h>
#include <stdio.h>
#include "main.h"
#include "milis.h"
//#include "delay.h"
#include "uart1.h"
#include "adc_helper.h"



void init(void)
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);      // taktovani MCU na 16MHz
    ADC2_SchmittTriggerConfig(ADC2_SCHMITTTRIG_CHANNEL14, DISABLE);
    ADC2_SchmittTriggerConfig(ADC2_SCHMITTTRIG_CHANNEL15, DISABLE);
    //nastavíme clock pro ADC2 (16MHz/4=4MHz)
    ADC2_PrescalerConfig(ADC2_PRESSEL_FCPU_D4);
    //volíme zarovnání výsledku - typicky doprava
    ADC2_AlignConfig(ADC2_ALIGN_RIGHT);
    //volíme zarovnání výsledku
    ADC2_AlignConfig(ADC2_ALIGN_RIGHT);
    //nastavíme multiplexer na některý kanál
    ADC2_Select_Channel(ADC2_CHANNEL_14);
    ADC2_Select_Channel(ADC2_CHANNEL_14);
    //Rozběhneme ADC
    ADC2_Cmd(ENABLE);
    //Počkáme až se rozběhne ADC
    ADC2_Startup_Wait();
    ADC2_Startup_Wait();

    GPIO_Init(LED_PORT, LED_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
#if defined (BTN_PORT) || defined (BTN_PIN)
    GPIO_Init(BTN_PORT, BTN_PIN, GPIO_MODE_IN_FL_NO_IT);
#endif

    init_milis();
    //init_uart1();
}


int main(void)
{

    uint32_t time = 0;
    uint16_t vref, vtemp, temp;
    //uint32_t CHANNEL_VREF, CHANNEL_VTEMP;
    init();

    while (1) {
        if (milis() - time > 1111) {
            time = milis();

            vref = ((uint32_t)ADC_get(CHANNEL_VREF) * 5000L +512) /1023;     //L = Long, //dělení děláme až poslední, protože je to ztrátová operace
            ADC_get(CHANNEL_VTEMP);
            vtemp = ((uint32_t)ADC_get(CHANNEL_VTEMP) * 5000L + 512) /1023;   
            vtemp = vtemp * 2495L / vref;
            temp = (100L*vtemp - 40000) / 195;
            printf("%u mV, %u mV, %u, %u C\n", vref, vtemp, temp/10, temp%10);//temp jako des. č.

        }

    }
//}
}

/*-------------------------------  Assert -----------------------------------*/
#include "__assert__.h"
