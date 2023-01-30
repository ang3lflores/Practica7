
#ifndef SSI_H_
#define SSI_H_

#include <inttypes.h>
//#define Bdata 0xF

extern void Configurar_SSI3(void);
extern void SPI_write(uint16_t data);
extern void SPI_write_data(void);
extern uint16_t SPI_readMAX(void);



#endif /* SSI_H_ */