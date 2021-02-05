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


typedef Gpio<GPIOD_BASE, 8> UART3_TX;
typedef Gpio<GPIOD_BASE, 9> UART3_RX;




//void SOS_Morse();
//void* ledRapido(void*);
//void* ledLento(void*);

void* Seriale3Test(void*);


void Watchdog_Init();

int main() {

    led::mode(Mode::OUTPUT);
    GPIO1::mode(Mode::OUTPUT);
    GPIO2::mode(Mode::OUTPUT);
    GPIO3::mode(Mode::OUTPUT);
    GPIO4::mode(Mode::OUTPUT);
    Watchdog::mode(Mode::OUTPUT);
    
    UART3_TX::mode(Mode::ALTERNATE);
    UART3_RX::mode(Mode::ALTERNATE);
    UART3_TX::alternateFunction(7);
    UART3_RX::alternateFunction(7);
    
     printf("FEES - StarSystem - Booting Up! \n");
    
    

    
  // pthread_t THREAD1;
  // pthread_create(&THREAD1,NULL,ledRapido,NULL);

     pthread_t THREAD_TEST;
     pthread_create(&THREAD_TEST,NULL,Seriale3Test,NULL);

     
    Watchdog_Init();


    led::low();
    usleep(5000000);
    led::high();


	sleep(55);
    Watchdog_Init();




    while (1) { }

}







/*

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

*/


   void Watchdog_Init(){

    	Watchdog::high();
    	usleep(10000);
    	Watchdog::low();
    	usleep(10000);
    	Watchdog::high();

    }



    void* Seriale3Test(void*){

        while(1){
                printf("SONO VIVO! Sono Acceso da: %llu \n " , getTick());
                usleep(500000);

        }



    }

