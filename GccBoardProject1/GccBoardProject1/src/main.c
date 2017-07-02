/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include <asf.h>
#include <delay.h>
int main (void)
{      
       struct pwm_config mypwm[4]; //For your PWM configuration –CKH
       // Insert system clock initialization code here (sysclk_init()).
       sysclk_init();
       board_init();
       /* Set up a PWM channel with 500 Hz frequency. Here we will output 4 different
       pulse trains on 4 pins of the A3BU Xplained. –CKH June 2014 */
       pwm_init(&mypwm[0], PWM_TCC0, PWM_CH_A, 500);//this is SDA on J1 on the A3BU Xplained
       pwm_init(&mypwm[1], PWM_TCC0, PWM_CH_B, 500);//On A3BU Xplained board, this is SCL on J1
       pwm_init(&mypwm[2], PWM_TCC0, PWM_CH_C, 500);//On A3BU Xplained board, RXD on J1
       pwm_init(&mypwm[3], PWM_TCC0, PWM_CH_D, 500);//On A3BU Xplained board, TXD on J1
       // Set each of the 4 PWM channel duty cycles independently, and start them going
       pwm_start(&mypwm[0], 10);
       pwm_start(&mypwm[1],30);
       pwm_start(&mypwm[2],60);
       pwm_start(&mypwm[3],80);
       //Change around the last number--the DUTY CYCLE--, inspect the pulse train on the scope, see
       //how it changes.
       while(1) {
              
              int i=100;
              
              for(i=100;i>=0;i--)
                      {      
                      pwm_start(&mypwm[2],i);
                      delay_ms(3.5);
                      
                      }
                      
       
              
              for(i=0;i<=100;i++)
                      {      
                      pwm_start(&mypwm[2],i);
                      delay_ms(3.5);
                      }
              
              
              
       }
}
