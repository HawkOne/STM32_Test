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

#include "chprintf.h"
#include "mpu9250.h"

#define cls(chp) chprintf(chp, "\033[2J\033[1;1H")

/*===========================================================================*/
/* MPU9250 related.                                                          */
/*===========================================================================*/

/* MPU9250 Driver: This object represent an MPU9250 instance */
static MPU9250Driver MPU9250D1;

static int32_t accraw1[MPU9250_ACC_NUMBER_OF_AXES];
static int32_t gyroraw1[MPU9250_GYRO_NUMBER_OF_AXES];

static float acccooked1[MPU9250_ACC_NUMBER_OF_AXES];
static float gyrocooked1[MPU9250_GYRO_NUMBER_OF_AXES];

//	----	 End Variables IMU1		----		//


/* MPU9250 Driver: This object represent an MPU9250 instance */
static MPU9250Driver MPU9250D2;

static int32_t accraw2[MPU9250_ACC_NUMBER_OF_AXES];
static int32_t gyroraw2[MPU9250_GYRO_NUMBER_OF_AXES];

static float acccooked2[MPU9250_ACC_NUMBER_OF_AXES];
static float gyrocooked2[MPU9250_GYRO_NUMBER_OF_AXES];
//	----	 End Variables IMU2		----		//

static float accBias1[MPU9250_ACC_NUMBER_OF_AXES]={2565,2670,70};
static float accBias2[MPU9250_ACC_NUMBER_OF_AXES]={2850,3320,70};

//	----	 End Biases 			----		//


static char axisID[MPU9250_ACC_NUMBER_OF_AXES] = {'X', 'Y', 'Z'};
static uint32_t i;

static const SPIConfig spicfg1 = {
    FALSE,
    NULL,
    GPIOD,
    GPIOD_CS_MPU1,
    SPI_CR1_BR_2 | SPI_CR1_BR_1 | SPI_CR1_BR_0 | SPI_CR1_CPOL | SPI_CR1_CPHA,
    0};

static const SPIConfig spicfg2 = {
    FALSE,
    NULL,
    GPIOE,
    GPIOE_CS_MPU2,
    SPI_CR1_BR_2 | SPI_CR1_BR_1 | SPI_CR1_BR_0 | SPI_CR1_CPOL | SPI_CR1_CPHA,
    0};

static MPU9250Config mpu9250cfg1 = {&SPID1,
                                    &spicfg1,
                                    NULL,
                                    NULL,
                                    MPU9250_ACC_FS_16G,
                                    NULL,
                                    NULL,
                                    MPU9250_GYRO_FS_2000DPS};
static MPU9250Config mpu9250cfg2 = {&SPID2,
                                    &spicfg2,
                                    NULL,
                                    NULL,
                                    MPU9250_ACC_FS_16G,
                                    NULL,
                                    NULL,
                                    MPU9250_GYRO_FS_2000DPS};

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
    chSysInit();

    /*
     * Activates the serial driver 3 using the driver default configuration.
     */

    sdStart(&SD3, NULL);
    chprintf((BaseSequentialStream*)&SD3, "\n%s\n\r" BOARD_NAME);

    chThdSleepSeconds(2);
    
    /* MPU9250 Object Initialization.*/
    mpu9250ObjectInit(&MPU9250D1);
    mpu9250ObjectInit(&MPU9250D2);

    /* Activates the MPU9250 driver.*/
    mpu9250Start(&MPU9250D1, &mpu9250cfg1);
    mpu9250Start(&MPU9250D2, &mpu9250cfg2);

    mpu9250GyroscopeSampleBias(&MPU9250D1);
    mpu9250GyroscopeSampleBias(&MPU9250D2);
    mpu9250AccelerometerSetBias(&MPU9250D1,accBias1);
    mpu9250AccelerometerSetBias(&MPU9250D2,accBias2);
    
///////////////////  MY IMU BIASING //////////////////
//
//    int a=1;
//
//	static int32_t accraw1old[MPU9250_ACC_NUMBER_OF_AXES];
//
//	static int32_t accraw2old[MPU9250_ACC_NUMBER_OF_AXES];






    while (true)
    {
        palToggleLine(LINE_LED_BLUE);
        mpu9250AccelerometerReadRaw(&MPU9250D1, accraw1);
        mpu9250AccelerometerReadRaw(&MPU9250D2, accraw2);

        mpu9250AccelerometerReadCooked(&MPU9250D1, acccooked1);
        mpu9250AccelerometerReadCooked(&MPU9250D2, acccooked2);


        chprintf((BaseSequentialStream*)&SD3,
                "MPU9250 Accelerometer Cooked data...[milli-g]\r\nSENSOR 1 "
                "\t\t\t SENSOR 2\n\r");

                for (i = 0; i < MPU9250_ACC_NUMBER_OF_AXES; i++)
                {
                	chprintf((BaseSequentialStream*)&SD3,
                            "%c-axis: %8.2f \t\t %8.2f\r\n", axisID[i], acccooked1[i],
                            acccooked2[i]);
                }



//        mpu9250GyroscopeReadRaw(&MPU9250D1, gyroraw1);
//        mpu9250GyroscopeReadRaw(&MPU9250D2, gyroraw2);

        mpu9250GyroscopeReadCooked(&MPU9250D1, gyrocooked1);
        mpu9250GyroscopeReadCooked(&MPU9250D2, gyrocooked2);


        chprintf((BaseSequentialStream*)&SD3,
                 "MPU9250 Gyroscope Cooked data...[DPS]\r\nSENSOR 1 \t\t\t "
                 "SENSOR 2\n\n\r");

                for (i = 0; i < MPU9250_GYRO_NUMBER_OF_AXES; i++)
                {
                    chprintf((BaseSequentialStream*)&SD3,
                             "%c-axis: %8.2f \t\t %8.2f\r\n", axisID[i], gyrocooked1[i],
                             gyrocooked2[i]);
                }



//        // (Ogni 10 loop)
//        if ( (a%10) == 0 ){
//        	 		chprintf((BaseSequentialStream*)&SD3, " NOW CALCULATING NEW BIASES \n");
//
//        	 		accBias1[0]= (accraw1old[0] + accraw1[0]) /2;
//        	 		accBias1[1]= (accraw1old[1] + accraw1[1]) /2;
//        	 		accBias1[2]= (accraw1old[2] + accraw1[2]) /2;
//
//					accBias2[0]= (accraw2old[0] + accraw2[0]) /2;
//        	 		accBias2[1]= (accraw2old[1] + accraw2[1]) /2;
//        	 		accBias2[2]= (accraw2old[2] + accraw2[2]) /2;
//
//        	 	mpu9250AccelerometerSetBias(&MPU9250D1,accBias1);
//    			mpu9250AccelerometerSetBias(&MPU9250D2,accBias2);
//
//				for (i = 0; i < MPU9250_ACC_NUMBER_OF_AXES; i++)
//                {
//                			chprintf((BaseSequentialStream*)&SD3,
//                            "%c-axis: %8.2f \t\t %8.2f\r\n", axisID[i], accBias1[i],
//                            accBias2[i]);
//                }
//
//         		chprintf((BaseSequentialStream*)&SD3, " ///////////////////////////// \n");
//
//        }
//
//
// 		a++;
// 		accraw1old[0] = accraw1[0];
// 		accraw1old[1] = accraw1[1];
// 		accraw1old[2] = accraw1[2];
//
// 		accraw2old[0] = accraw2[0];
// 		accraw2old[1] = accraw2[1];
// 		accraw2old[2] = accraw2[2];


        chThdSleepMilliseconds(100);

        cls((BaseSequentialStream*)&SD3);
    }
}
