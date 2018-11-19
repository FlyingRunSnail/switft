

#ifdef HW_EEPROM_DEF
	#define HW_EEPROM_GLOBE
#else
	#define HW_EEPROM_GLOBE		extern
#endif

#define		HW_EEPROM_I2C_HARD_MODE

#ifndef	HW_EEPROM_I2C_HARD_MODE
	#include <bsp_i2c_soft.h>

	#define		I2C_PIN_SCK_INDEX			I2C_SOFT_PIN_SCK_INDEX
	#define		I2C_PIN_DATA_INDEX		I2C_SOFT_PIN_DATA_INDEX
#else
	#include <bsp_i2c_hard.h>

	#define		I2C_PIN_SCK_INDEX			I2C_HARD_PIN_SCK_INDEX
	#define		I2C_PIN_DATA_INDEX		I2C_HARD_PIN_DATA_INDEX

	#define		HW_EEPROM_I2C_DMA_STREAM_TX            DMA1_Stream6
	#define		HW_EEPROM_I2C_DMA_STREAM_RX            DMA1_Stream0   

	#define		HW_EEPROM_TXDMA_INT_ID	BSP_INT_ID_DMA1_CH7
	#define		HW_EEPROM_RXDMA_INT_ID	BSP_INT_ID_DMA1_CH1

#endif



#define		HW_EEPROM_SCK_PIN_PORT			GPIOB
#define		HW_EEPROM_SCK_PIN				GPIO_Pin_6

#define		HW_EEPROM_DATA_PIN_PORT			GPIOB
#define		HW_EEPROM_DATA_PIN				GPIO_Pin_7





HW_EEPROM_GLOBE void HW_EEPROM_Init(void);

#if 0
HW_EEPROM_GLOBE int HW_EEPROM_WriteByte(INT8U dev_addr, INT32U son_addr, INT8U son_addr_size,INT8U data);

HW_EEPROM_GLOBE int HW_EEPROM_ReadByte(INT8U dev_addr, INT32U son_addr, INT8U son_addr_size,INT8U *data);
#endif
HW_EEPROM_GLOBE int HW_EEPROM_ReadBuffer(INT32U ReadAddr, INT8U* pBuffer, INT32U *NumByteToRead);

HW_EEPROM_GLOBE int HW_EEPROM_WritePage(INT32U WriteAddr, INT8U* pBuffer, INT8U* NumByteToWrite);

HW_EEPROM_GLOBE int HW_EEPROM_WriteBuffer(INT32U WriteAddr, INT8U* pBuffer, INT32U NumByteToWrite);

HW_EEPROM_GLOBE void HW_EEPROM_I2C_DMA_TX_IRQHandler(void);

HW_EEPROM_GLOBE void HW_EEPROM_I2C_DMA_RX_IRQHandler(void);



HW_EEPROM_GLOBE  int HW_EEPROM_Test(void);





