/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/


#include "ch.h"
#include "hal.h"

#include "halconf.h"
#include "hal_i2c_lld.h"
#include "mcuconf.h"


#include "chprintf.h"
#include "mpu9250.h"
#include "LM75B.h"

#define cls(chp) chprintf(chp, "\033[2J\033[1;1H")


/////////////////////////////////////////////////////
/*
 * This is a periodic thread that flashes the LED.
 */
static THD_WORKING_AREA(blinker_wa, 128);
static THD_FUNCTION(blinker, arg)
{
  (void)arg;
  chRegSetThreadName("blinker");
  while (1)
  	  {
	  palToggleLine(LINE_LED_BLUE);
	  chThdSleepMilliseconds(500);
      palToggleLine(LINE_LED_BLUE);
	  chThdSleepMilliseconds(500);
  	  }
}
//////////////////



//// I2C RELATED THINGS ///

//#if 0
//static const I2CConfig i2cconfig = {
//  STM32_TIMINGR_PRESC(8U)  |            /* 72MHz/9 = 8MHz I2CCLK.           */
//  STM32_TIMINGR_SCLDEL(3U) | STM32_TIMINGR_SDADEL(3U) |
//  STM32_TIMINGR_SCLH(3U)   | STM32_TIMINGR_SCLL(9U),
//  0,
//  0
//};
//#endif


//////  I2C SETTINGS
//
//static const I2CConfig i2cconfig = {
//  STM32_TIMINGR_PRESC(15U) |
//  STM32_TIMINGR_SCLDEL(4U) | STM32_TIMINGR_SDADEL(2U) |
//  STM32_TIMINGR_SCLH(15U)  | STM32_TIMINGR_SCLL(21U),
//  0,
//  0
//};


//////  I2C SETTINGS

//typedef struct {
//  /* End of the mandatory fields.*/
//  i2copmode_t     op_mode;       /**< @brief Specifies the I2C mode.        */
//  uint32_t        clock_speed;   /**< @brief Specifies the clock frequency.
//                                      @note Must be set to a value lower
//                                      than 400kHz.                          */
// i2cdutycycle_t  duty_cycle;    /**< @brief Specifies the I2C fast mode
//                                     duty cycle.                           */
//} I2CConfig;


// ACTUAL CONFIGURATION //

static const I2CConfig i2cconfig = {
  OPMODE_I2C,
  400000,
  FAST_DUTY_CYCLE_2,
};





/*
 * Application entry point.
 */
int main(void)
{

    /*
     * System initializations.
     * - HAL initialization, this also initializes the configured device drivers
     *   and performs the board-specific initializations.
     * - Kernel initialization, the main() function becomes a thread and the
     *   RTOS is active.
     */
    halInit();
    // halInit(); implicitly invokes also i2cInit();
    chSysInit();

    /*
     * Activates the serial driver 3 using the driver default configuration.
     */
    sdStart(&SD3, NULL);
    chprintf((BaseSequentialStream*)&SD3, "\n %s \n\r" BOARD_NAME);
    chThdSleepSeconds(2);
    
    ///// I2C RELATED THINGS ////

    /*
     * Starting the I2C driver 1.
     */
    // Need to modify halconf.h to HAL_use_I2C TRUE
    // Need to modify also mcuconfig.h to #define STM32_I2C_USE_I2C1 TRUE
    //i2cStart(&I2CD1, &i2cconfig);


//    /* Configuring I2C SCK and I2C SDA related GPIOs .*/
//      palSetLineMode(LINE_ARD_D15, PAL_MODE_ALTERNATE(4) |
//                     PAL_STM32_OSPEED_HIGHEST | PAL_STM32_OTYPE_OPENDRAIN);
//      palSetLineMode(LINE_ARD_D14, PAL_MODE_ALTERNATE(4) |
//                     PAL_STM32_OSPEED_HIGHEST | PAL_STM32_OTYPE_OPENDRAIN);

    /*
     * Starting the I2C driver 2.
     */
    i2cStart(&I2CD2, &i2cconfig);


    /*
     * Starting the blinker thread.
     */
     chThdCreateStatic(blinker_wa, sizeof(blinker_wa), NORMALPRIO-1, blinker, NULL);




 	int a=0;

    while (true)
     {
        chprintf((BaseSequentialStream*)&SD3, "\n Loop numero: %d \n\r", a );

        msg_t msg;

        char cmd[1];
        cmd[0]= 0x00;
        char rec[2];
        rec[0]= 0x00;
        rec[1]= 0x00;
        float temperature=0;

        i2cAcquireBus(&I2CD2);

        /////// ASK FOR TEMPERATURE /////

        i2cMasterTransmitTimeout(&I2CD2, 0b1001000 , cmd, 1, NULL, 0, TIME_INFINITE);

        ////// READ  ////////

        i2cMasterReceiveTimeout(&I2CD2, 0b1001000 ,rec ,2 ,TIME_INFINITE);

        temperature = ((float)((rec[0]<<8)|rec[1]) / 256.0) ;

        chprintf((BaseSequentialStream*)&SD3, "\n TEMP RECEIVED: %f \n\r" , temperature );


        i2cReleaseBus(&I2CD2);

        a++;

        chThdSleepMilliseconds(300);
        cls((BaseSequentialStream*)&SD3);
    }
    return 0;
}
