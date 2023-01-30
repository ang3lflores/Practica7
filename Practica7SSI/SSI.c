#include "lib/include.h"
#define Bdata 0xF
extern void Configurar_SSI3(void)
{

    SYSCTL->RCGCSSI |= (1<<3); // Se activa el Modulo 3 SSI2
    SYSCTL->RCGCGPIO |= (1<<3);// Puerto D
    GPIOD->DIR |= (0<<3) | (1<<2) | (1<<1) | (0<<0); //selector es salida = 1 (0<<3) | (1<<2) | (1<<1) | (0<<0)
    GPIOD->AFSEL = (1<<3)|(1<<2)|(1<<1)|(1<<0); // salidas
    GPIOD->PCTL = (GPIOD->PCTL&0xFFFF0000) | 0x00001111; // tabla p.1351 limpiar/escribir
    //GPIOD->PCTL &= 0xFFFF0000;
    //GPIOD->PCTL |= 0x00001111; //
    GPIOD->DEN |= (1<<0)|(1<<1)|(1<<2)|(1<<3); // salida digital
    //             SCLK   CS   MISO(Rx)  MOSI(Tx)

   // GPIOD_AHB->DATA |= (1<<5); //registrar CS
    //GPIOB->PUR |= (0<<7)|(0<<6)|(0<<5)|(0<<4);
    //GPIOB->PDR |= (0<<7)|(0<<6)|(0<<5)|(0<<4);
    //GPIOB->AMSEL |= (0<<7)|(0<<6)|(0<<5)|(0<<4);

    // Pág. 971, 972
    SSI3->CR1 = (0<<1); // SSE=0 deshabilitar módulo
    SSI3->CR1 = (0<<2); // MS = 0 modo maestro
    SSI3->CC = (0<<0); // System clock
    //SSInClk = SysClk / (CPSDVSR * (1 + SCR))  --> SCR = SysClk / SSInClk*CPSDVSR) - 1
    // SSInClk = 2.5
    // SysClk = 4.3 MHz
    // CPSDVSR (2-254) = 2
    // SCR = (50 000 000/4 300 000 * 2) - 1 = 5

    // Pág. 969, 970
    SSI3->CPSR =2; // 4.3 MHZ (0x2; 0b010, 2, 1<<2)
    //SSI3->CR0 = (0x9<<8) | 0x07; // datos de 8 bits
    SSI3->CR0 = (5<<8); // SCR (a partir del bit 8)
    SSI3->CR0 |= (1<<7); // SPH
    SSI3->CR0 |= (1<<6); // SPO
    SSI3->CR0 |= (0<<4); // FRF
    SSI3->CR0 |= (Bdata<<0); // DSD 16-bit data
    SSI3->CR1 |= (1<<1); //SSE=1 habilitar modoulo p.961 (0x02)
}


extern void SPI_write(uint16_t data)
{
    while (SSI3->SR & 0x2)
    {
        SSI3->DR = (uint16_t)data;
    }
}

//extern void SPI_write_data(uint8_t reg, uint8_t data)
//{
//    GPIOD_AHB->DATA &= ~(1<<3); // CS = 0 se niega
//    SPI_write(reg & ~0x80); //escribir registro + MSB igualado a cero
//    SPI_write(data);
//    GPIOD_AHB->DATA |= (1<<3); //CS = 1
//}

extern void SPI_write_data(void)
{
    GPIOD->DATA |= (0<<1); // en CS = 0
    SPI_write(0b11101010); //escribir registro + MSB igualado a cero
    GPIOD->DATA |= (1<<1); //CS = 1
}


//
extern uint16_t SPI_readMAX(void)
{
    int tem = 0;
    uint16_t data = 0;
    GPIOD->DATA |= (1<<1); // en CS = 0
    SPI_write(0x0);
    while ((SSI3->SR & 0x10) == 0x10){
        // espera por busy bit
    }
    data = SSI3->DR;
    GPIOD->DATA |= (0<<1); // en CS = 0

    data  = (data>>3);
    data |= (0<<12)|(0<<13);
    tem=(float)(0.25 * data);
//    tem = data*0.25; //
    return tem;
}

//extern uint8_t SPI_read_data(uint8_t reg)
//{
//    uint8_t data = 0;
//    GPIOD_AHB->DATA &= ~(1<<3); // CS = 0
//    SPI_write(reg | 0x80); // escribe registro + MSB
//    SPI_write(0x00); //escribir dato para generar seÃ±al de reloj
//    data = SPI_read(); //leer dato
//    GPIOD_AHB->DATA |= (1<<3); //CS = 1
//    return data;
//}
//


