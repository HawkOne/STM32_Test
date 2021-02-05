#include <cstdio>
#include <miosix.h>
#include <pthread.h>

using namespace std;
using namespace miosix;


typedef Gpio<GPIOD_BASE, 12> led;
typedef Gpio<GPIOB_BASE, 1> GPIO1;
typedef Gpio<GPIOB_BASE, 12> GPIO2;
typedef Gpio<GPIOA_BASE, 3> GPIO3;
typedef Gpio<GPIOA_BASE, 10> GPIO4;
typedef Gpio<GPIOB_BASE, 5> Watchdog;

typedef Gpio<GPIOC_BASE, 9> EXT_CS;





typedef Gpio<GPIOA_BASE, 7> SPI1_MOSI;
typedef Gpio<GPIOA_BASE, 6> SPI1_MISO;
typedef Gpio<GPIOA_BASE, 5> SPI1_SCL;

typedef Gpio<GPIOE_BASE, 10> CS_IMU1;



void SOS_Morse();
void* ledRapido(void*);
void* ledLento(void*);
void* BUZZ(void*);



void* Seriale3Test(void*);
void Watchdog_Init();

////
void spiSend(unsigned char byte);
unsigned char spiRecive();

void Mpu_init();

/*
 * Un developer risolse problemi con l'init di questo device
 * inviando 0x22 nel registro INIT_CFG
 * 
 * void IMU1_Init(){
 * 0x22 INT_CFG 
 * }
*/

int main() {

    
    led::mode(Mode::OUTPUT);
    GPIO1::mode(Mode::OUTPUT);
    GPIO2::mode(Mode::OUTPUT);
    GPIO3::mode(Mode::OUTPUT);
    GPIO4::mode(Mode::OUTPUT);
    EXT_CS::mode(Mode::OUTPUT);

    Watchdog::mode(Mode::OUTPUT);
    
    
    SPI1_MOSI::mode(Mode::ALTERNATE);
    SPI1_MOSI::alternateFunction(5);
    SPI1_MISO::mode(Mode::ALTERNATE);
    SPI1_MISO::alternateFunction(5);
    SPI1_SCL::mode(Mode::ALTERNATE);
    SPI1_SCL::alternateFunction(5);
    CS_IMU1::mode(Mode::OUTPUT);
    CS_IMU1::high();
    
    
    printf("\n FEES - StarSystem - Booting Up! \n \n");



    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
    SPI1->CR1=//SPI_CR1_DFF      | //16bit mode
                SPI_CR1_SSM      | //SS pin not connected to SPI
		SPI_CR1_SSI      | //Internal SS signal pulled high
                //SPI_CR1_LSBFIRST | //Send LSB first
                SPI_CR1_MSTR     | //Master mode
                SPI_CR1_SPE      | //SPI enabled, master mode
		SPI_CR1_BR_2;      //Speed
   
    

    pthread_t THREAD_BUZZ;
    pthread_create(&THREAD_BUZZ,NULL,BUZZ,NULL);

    pthread_t THREAD_TEST;
    pthread_create(&THREAD_TEST, NULL, Seriale3Test, NULL);


    Watchdog_Init();

    
    led::low();
    usleep(5000000);
    led::high();


    sleep(55);
    Watchdog_Init();




    while (1) {
        led::low();
        GPIO2::low();
        GPIO3::low();
        GPIO4::low();
        EXT_CS::low();

        
    usleep(500000);
    
        led::high();
        GPIO2::high();
        GPIO3::high();
        GPIO4::high();
        EXT_CS::high();

    usleep(500000);
    }
}


void IMU1_init(){
    CS_IMU1::low();
        
    CS_IMU1::high();

}




void spiSend(unsigned char byte){
    SPI1->DR=byte;
    while((SPI1->SR & SPI_SR_RXNE)==0); //Wait
    byte=SPI1->DR; // DummyREAD
}

unsigned char spiRecive(){
    SPI1->DR=0;
    while((SPI1->SR & SPI_SR_RXNE)==0); // Wait
    return SPI1->DR;
}



void* Seriale3Test(void*) {
    while (1) {
        printf("Acceso da: %llu ticks \n ", getTick());
        usleep(1000000); // wait 1 secondo.
    }
}





void SOS_Morse() {
    led::low();
    usleep(100000);
    led::high();
    usleep(100000);
    led::low();
    usleep(100000);
    led::high();
    usleep(100000);
    led::low();
    usleep(100000);
    led::high();

    usleep(100000);
    led::low();
    usleep(300000);
    led::high();
    usleep(100000);
    led::low();
    usleep(300000);
    led::high();
    usleep(100000);
    led::low();
    usleep(300000);
    led::high();
    usleep(100000);


    led::low();
    usleep(100000);
    led::high();
    usleep(100000);
    led::low();
    usleep(100000);
    led::high();
    usleep(100000);
    led::low();
    usleep(100000);
    led::high();
    usleep(3000000);

}


void* ledRapido(void*) {
    for (;;) {
        led::low();
        usleep(100000);
        led::high();
        usleep(100000);
    }
}

void* ledLento(void*){
    for(;;){
        led::low();
        usleep(500000);
        led::high();
        usleep(500000);
    }
}

 


void Watchdog_Init() {

    Watchdog::high();
    usleep(10000);
    Watchdog::low();
    usleep(10000);
    Watchdog::high();
    printf("\n  WatchDog Init - Fatto! \n \n");

}


void* BUZZ(void*) {
    for(int i=0; i<100; i++){
    GPIO1::high();
    usleep(5000);
    GPIO1::low();
    usleep(5000);
    }
}