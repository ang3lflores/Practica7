
#include "lib/include.h"
volatile float temperatura1 = 0;
int main(void)
{
//    int temperatura = 0;
    Configurar_PLL(50000000);  // Configuración a 50 MHz
    Configurar_SSI3();

    while (1){
//        SPI_write_data();
    temperatura1 = SPI_readMAX();
    }
}
