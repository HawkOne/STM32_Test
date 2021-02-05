/***********************************************************************************************
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                                             *
 *    FFFFFFFFFFFFFFFFFFFFFEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE   SSSSSSSSSSSSSSS      *
 *    F:::::::::::::::::::FE::::::::::::::::::::EE::::::::::::::::::::E SS:::::::::::::::S     *
 *    F:::::::::::::::::::FE::::::::::::::::::::EE::::::::::::::::::::ES:::::SSSSSS::::::S     *
 *    FF:::::FFFFFFFFF::::FEE::::::EEEEEEEEE::::EEE::::::EEEEEEEEE::::ES:::::S     SSSSSSS     *
 *      F::::F       FFFFFF  E:::::E       EEEEEE  E:::::E       EEEEEES:::::S                 *
 *      F::::F               E:::::E               E:::::E             S:::::S                 *
 *      F:::::FFFFFFFFFF     E::::::EEEEEEEEEE     E::::::EEEEEEEEEE    S::::SSSS              *
 * ---- F::::::::::::::F     E:::::::::::::::E     E:::::::::::::::E     SS::::::SSSSS    ---- *
 * -::- F::::::::::::::F     E:::::::::::::::E     E:::::::::::::::E       SSS::::::::SS  -::- *
 * ---- F:::::FFFFFFFFFF     E::::::EEEEEEEEEE     E::::::EEEEEEEEEE          SSSSSS::::S ---- *
 *      F::::F               E:::::E               E:::::E                         S:::::S     *
 *      F::::F               E:::::E       EEEEEE  E:::::E       EEEEEE            S:::::S     *
 *    FF::::::FF           EE::::::EEEEEEEE:::::EEE::::::EEEEEEEE:::::ESSSSSSS     S:::::S     *
 *    F:::::::FF           E::::::::::::::::::::EE::::::::::::::::::::ES::::::SSSSSS:::::S     *
 *    F:::::::FF           E::::::::::::::::::::EE::::::::::::::::::::ES:::::::::::::::SS      *
 *    FFFFFFFFFF           EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE SSSSSSSSSSSSSSS        *
 *                                                                                             *
 *             ____    ___     __      ___    ___    __  ___ __       ___   ___                *
 *             |__ |   |__ \_/||__)|   |__    |__ \_/|__)|__ |__)||\/||__ |\ || /\ |           *
 *             |   |___|___/ \||__)|___|___   |___/ \|   |___|  \||  ||___| \||/~~\|___        *
 *                      ___     __  ___ __  __  ___ __     __     ________                     *
 *                      |__ |\/||__)|__ |  \|  \|__ |  \   /__`\ //__`||__ |\/|                *
 *                      |___|  ||__)|___|__/|__/|___|__/   .__/ | .__/||___|  |                *
 *                                                                                             *
 *                                                                                             *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                              *                                              *
 *                                        FEES Project                                         *
 *                      URL: https://github.com/ferrandi/DEIB-DAER-project                     *
 *                               Politecnico di Milano - DEIB                                  *
 *                          				Stefano Ampolo                             		   *
 *         ************************************************************************            *
 *                  Copyright (c) 2018-2019 Politecnico di Milano                              *
 *                                                                                             *
 *          This file is part of the FEES framework.                                           *
 *                                                                                             *
 *          The FEES framework is free software; you can redistribute it and/or modify         *
 *          it under the terms of the GNU General Public License as published by               *
 *          the Free Software Foundation; either version 3 of the License, or                  *
 *          (at your option) any later version.                                                *
 *                                                                                             *
 *          This program is distributed in the hope that it will be useful,                    *
 *          but WITHOUT ANY WARRANTY; without even the implied warranty of                     *
 *          MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                      *
 *          GNU General Public License for more details.                                       *
 *                                                                                             *
 *          You should have received a copy of the GNU General Public License                  *
 *          along with this program.  If not, see <http://www.gnu.org/licenses/>.              *
 *                                                                                             *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

#include "ch.h"
#include "hal.h"

#include "halconf.h"
#include "hal_i2c_lld.h"
#include "mcuconf.h"


#include "chprintf.h"
#include "mpu9250.h"

#include "LM75B.h"
#include "LM75B.cpp"

#define cls(chp) chprintf(chp, "\033[2J\033[1;1H")

using namespace std;


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
//////////////////////////////////////////////////////



//// I2C CONFIGURATION ///

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
    
    /*
     * Configuration of the the I2C driver
     */
    // Need to modify halconf.h to HAL_use_I2C TRUE
    // Need to modify also mcuconfig.h to #define STM32_I2C_USE_I2C1 TRUE

    static const I2CConfig i2cconfig = {
    				OPMODE_I2C,
    				LM75B_I2C_FREQ,
    				FAST_DUTY_CYCLE_2,
    		};

    // * Starting the I2C driver 2. * // (Connesso al BUS I2C1 sul FEES)
    i2cStart(&I2CD2, &i2cconfig);


    //** Starting the blinker thread.**//
     chThdCreateStatic(blinker_wa, sizeof(blinker_wa), NORMALPRIO-1, blinker, NULL);


     // Instanzio l'oggetto LM75B
     LM75B lm75b;


    while (1)
     {
        chprintf((BaseSequentialStream*)&SD3, "\n La Temperatura è : %.2f °\n\r" , lm75b.read());

        chThdSleepMilliseconds(300);
        cls((BaseSequentialStream*)&SD3);
    }
    return 0;
}
