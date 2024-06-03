#include <stdbool.h>
#include <stm8s.h>
//#include <stdio.h>
#include "main.h"
#include "milis.h"
//#include "delay.h"
//#include "uart1.h"
#include "daughterboard.h"

void init(void)
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);      // taktovani MCU na 16MHz

///    GPIO_Init(LED_PORT, LED_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(PWM_R_PORT, PWM_R_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(PWM_G_PORT, PWM_G_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(PWM_B_PORT, PWM_B_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(S1_PORT, S1_PIN, GPIO_MODE_IN_PU_NO_IT);
    GPIO_Init(S2_PORT, S2_PIN, GPIO_MODE_IN_PU_NO_IT);
    GPIO_Init(S3_PORT, S3_PIN, GPIO_MODE_IN_PU_NO_IT);

    GPIO_Init(LED5_PORT, LED5_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(LED7_PORT, LED7_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(LED8_PORT, LED8_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);


    TIM2_TimeBaseInit(TIM2_PRESCALER_16, 10000 - 1);
    TIM2_OC1Init(
        TIM2_OCMODE_PWM1,
        TIM2_OUTPUTSTATE_ENABLE,
        5000,
        TIM2_OCPOLARITY_HIGH
    );
    init_milis();
    //init_uart1();

    TIM2_OC3Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE, 2000, TIM2_OCPOLARITY_HIGH);
    TIM2_OC2Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE, 2000, TIM2_OCPOLARITY_HIGH);

    TIM2_OC1PreloadConfig(ENABLE);
    TIM2_OC2PreloadConfig(ENABLE);
    TIM2_OC3PreloadConfig(ENABLE);

    TIM2_Cmd(ENABLE);
}


int main(void)
{
    init();
    uint32_t time = 0;
    uint16_t r=1, g=1, b=1, lower, higher;
    uint16_t barva = 0;

    bool S1memory = false;
    bool S2memory = false;
    bool S3memory = false;

    TIM2_SetCompare1(r-1);
    TIM2_SetCompare2(g-1);
    TIM2_SetCompare3(b-1);

    while (1) {
        if (milis() - time > 10 ) {
            time = milis();
            if (!PUSH(S1)&&S1memory){
                ++barva;
                if (barva>3){
                    barva = 1;
                }
            }
        S1memory=PUSH(S1);

            if(barva==1){
                    HIGH(LED8);
                    LOW(LED5);
                    LOW(LED7);
            }else if (barva==2) {
                    LOW(LED8);
                    HIGH(LED5);
                    LOW(LED7);
            }else if (barva==3) {
                    LOW(LED8);
                    LOW(LED5);
                    HIGH(LED7);};


            if (!PUSH(S2)&&S2memory){
                if(barva==1){
                    r+=1000;
                    if (r>9000)
                        r = 9000;
                TIM2_SetCompare2(r-1);
                }else if (barva==2) {
                    g+=1000;
                    if (g>9000)
                        g = 9000;
                TIM2_SetCompare1(g-1);
                }else if (barva==3) {
                    b+=1000;
                    if (b>9000)
                        b = 9000;
                TIM2_SetCompare3(b-1);
                }
            }
            S2memory=PUSH(S2);
                        
            
            if (!PUSH(S3)&&S3memory){
                        if(barva==1){
                            r=r-1000;
                            if (r<1000)
                                r = 1000;
                        TIM2_SetCompare2(r-1);
                        }else if (barva==2) {
                            g=g-1000;
                            if (g<1000)
                                g = 1000;
                        TIM2_SetCompare1(g-1);
                        }else if (barva==3) {
                            b=b-1000;
                            if (b<1000)
                                b = 1000;
                        TIM2_SetCompare3(b-1);
                        }
                    }
                
                S3memory=PUSH(S3);
        }    
    }
}