

#ifdef HW_SPIFLASH_DEF
	#define HW_SPIFLASH_GLOBE
#else
	#define HW_SPIFLASH_GLOBE		extern
#endif



#include <bsp_spi_hard.h>




#define		HW_SPIFLASH_SCK_PIN_PORT			GPIOB
#define		HW_SPIFLASH_SCK_PIN					GPIO_Pin_13

#define		HW_SPIFLASH_MOSI_PIN_PORT			GPIOB
#define		HW_SPIFLASH_MOSI_PIN					GPIO_Pin_15

#define		HW_SPIFLASH_MISO_PIN_PORT			GPIOB
#define		HW_SPIFLASH_MISO_PIN					GPIO_Pin_14

#define		HW_SPIFLASH_CS_PIN_PORT				GPIOB
#define		HW_SPIFLASH_CS_PIN					GPIO_Pin_12

#define		HW_SPIFLASH_SPI_PORT					SPI2




HW_SPIFLASH_GLOBE void HW_SPIFLASH_Init(void);

HW_SPIFLASH_GLOBE void HW_SPIFLASH_CsEnable(void);

HW_SPIFLASH_GLOBE void HW_SPIFLASH_CsDisable(void);

HW_SPIFLASH_GLOBE INT8U HW_SPIFLASH_WriteByte(INT8U data);

HW_SPIFLASH_GLOBE INT8U HW_SPIFLASH_ReadByte(void);



