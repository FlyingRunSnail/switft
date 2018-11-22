BINPATH=
OUTPATH=

CC=$(BINPATH)arm-none-eabi-gcc
AR=$(BINPATH)arm-none-eabi-ar

# Check for valid float argument
ifneq ($(FLOAT_TYPE), hard)
ifneq ($(FLOAT_TYPE), soft)
#override FLOAT_TYPE = hard
override FLOAT_TYPE = soft
endif
endif

vpath %.c src

CFLAGS  = -g -O2 -Wall
CFLAGS += -mlittle-endian -mthumb -mthumb-interwork -mcpu=cortex-m3

ifeq ($(FLOAT_TYPE), hard)
CFLAGS += -fsingle-precision-constant -Wdouble-promotion
CFLAGS += -mfpu=fpv4-sp-d16 -mfloat-abi=hard
else
CFLAGS += -msoft-float
endif

CFLAGS += -ffreestanding -nostdlib

# Includes
CFLAGS += -Iinc -I$(BASEDIR)/Lib/CMSIS/ -I$(BASEDIR)/Lib/CMSIS/DeviceSupport/

# Default to STM32F40_41xxx if no device is passed
ifeq ($(DEVICE_DEF), )
DEVICE_DEF = STM32F2xx
endif

CFLAGS += -D$(DEVICE_DEF)

# Sources
SRCS = misc.c stm32f2xx_dma.c stm32f2xx_rcc.c stm32f2xx_adc.c \
	stm32f2xx_exti.c stm32f2xx_rng.c stm32f2xx_can.c stm32f2xx_flash.c \
	stm32f2xx_rtc.c stm32f2xx_crc.c stm32f2xx_fsmc.c stm32f2xx_sdio.c \
	stm32f2xx_cryp_aes.c stm32f2xx_gpio.c stm32f2xx_spi.c \
	stm32f2xx_cryp.c stm32f2xx_hash.c stm32f2xx_syscfg.c \
	stm32f2xx_cryp_des.c stm32f2xx_hash_md5.c stm32f2xx_tim.c \
	stm32f2xx_cryp_tdes.c stm32f2xx_hash_sha1.c  stm32f2xx_usart.c \
	stm32f2xx_dac.c stm32f2xx_i2c.c stm32f2xx_wwdg.c \
	stm32f2xx_dbgmcu.c stm32f2xx_iwdg.c \
	stm32f2xx_dcmi.c stm32f2xx_pwr.c


OBJS = $(SRCS:.c=.o)
LIBNAME = $(OUTPATH)/libstdperiph.a

all: $(LIBNAME)

%.o : %.c
	$(CC) $(CFLAGS) -c -o $(addprefix $(OUTPATH)/,$@) $^

$(LIBNAME): $(OBJS)
	$(AR) -r $@ $(addprefix $(OUTPATH)/, $(OBJS))

clean:
	rm -f $(addprefix $(OUTPATH)/,$(OBJS)) $(LIBNAME)
	
