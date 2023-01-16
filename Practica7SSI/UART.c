#include "lib/include.h"

extern void Configurar_UART0(void)
{
    SYSCTL->RCGCUART  = (1<<5);   // Uart5Paso 1 (RCGCUART) pag.344 UART/modulo0 0->Disable 1->Enable
    SYSCTL->RCGCGPIO |= (1<<4);     // Puerto E Paso 2 (RCGCGPIO) pag.340 Enable clock port A
    //(GPIOAFSEL) pag.671 Enable alternate function
    
    GPIOE->AFSEL |= (1<<5) | (1<<4);
    GPIOE->DIR |= (1<<5) | (0<<4);
    //GPIO Port Control (GPIOPCTL) PA0-> U0Rx PA1-> U0Tx pag.688
    GPIOE->PCTL |= (GPIOE->PCTL&0xFF00FFFF) | 0x00110000;  // (1<<0) | (1<<4);//0x00000011
    // GPIO Digital Enable (GPIODEN) pag.682
    GPIOE->DEN |= (1<<5) | (1<<4);//PA1 PA0
    //UART0 UART Control (UARTCTL) pag.918 DISABLE!!
    UART5->CTL |= (0<<9) | (0<<8) | (0<<0);


    // UART Integer Baud-Rate Divisor (UARTIBRD) pag.914
    /*
    En mi caso 
    BRD = 40,000,000/ (16 * 28800) =  86.80
    UARTFBRD[DIVFRAC] = integer(0.8 * 64 + 0.5) = 52
    */
    UART5->IBRD = 87;
    // UART Fractional Baud-Rate Divisor (UARTFBRD) pag.915
    UART5->FBRD = 52;
    //  UART Line Control (UARTLCRH) pag.916
    UART5->LCRH = (0x3<<5)|(1<<4);
    //  UART Clock Configuration(UARTCC) pag.939
    UART5->CC =(0<<0);
    //Disable UART0 UART Control (UARTCTL) pag.918
    UART5->CTL = (1<<0) | (1<<8) | (1<<9);



}

extern char readChar(void)
{
    //UART FR flag pag 911
    //UART DR data 906
    int v;
    char c;
    while((UART5->FR & (1<<4)) != 0 );
    v = UART5->DR & 0xFF;
    c = v;
    return c;
}
extern void printChar(char c)
{
    while((UART5->FR & (1<<5)) != 0 );
    UART5->DR = c;
}
extern void printString(char* string)
{
    while(*string)
    {
        printChar(*(string++));
    }
}

extern void readString(char UART5_Sring[], char delimitador)
{

   int i=0;
   char string [20]; //= (char *)calloc(10,sizeof(char));
   char c = readChar();
   while(c != delimitador)
   {
       (UART5_Sring[i]) = c;
       i++;
       /*if(i%10==0)
       {
           string = realloc(string,(i+10)*sizeof(char));
       }
       */

       c = readChar();
   }

   //UART7_Sring = string;
     UART5_Sring[i]= '\0';

}


//El envio es su nombre  (rave) 

// invertirlo y regresarlo con numeros consecutivos
// entre letras (e1v2a3r) 

