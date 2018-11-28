# Sources

SRCS = 
S_SRCS =  

# Project name

PROJ_NAME = swift
OUTPATH = build

OUTPATH := $(abspath $(OUTPATH))

BASEDIR := $(abspath ./)

###################################################

# Check for valid float argument
# NOTE that you have to run make clan after
# changing these as hardfloat and softfloat are not
# binary compatible
ifneq ($(FLOAT_TYPE), hard)
ifneq ($(FLOAT_TYPE), soft)
#override FLOAT_TYPE = hard
override FLOAT_TYPE = soft
endif
endif

###################################################

AS=$(BINPATH)arm-none-eabi-as
CC=$(BINPATH)arm-none-eabi-gcc
LD=$(BINPATH)arm-none-eabi-gcc
AR=$(BINPATH)arm-none-eabi-ar
#LD=$(BINPATH)arm-none-eabi-ld
OBJCOPY=$(BINPATH)arm-none-eabi-objcopy
OBJDUMP=$(BINPATH)arm-none-eabi-objdump
SIZE=$(BINPATH)arm-none-eabi-size

#LINKER_SCRIPT = stm32_flash.ld
LINKER_SCRIPT = stm32f207flash.ld

CPU = -mcpu=cortex-m3 -mthumb

CFLAGS  = $(CPU) -c -std=gnu99 -g -O2 -Wall
#LDFLAGS  = $(CPU) -mlittle-endian -mthumb-interwork -nostartfiles -Wl,--gc-sections,-Map=$(OUTPATH)/$(PROJ_NAME).map,--cref --specs=nano.specs
#LDFLAGS  = $(CPU) -mlittle-endian -mthumb-interwork -Wl,--gc-sections,-Map=$(OUTPATH)/$(PROJ_NAME).map,--cref --specs=nano.specs
LDFLAGS  = -Wl,--gc-sections,-Map=$(OUTPATH)/$(PROJ_NAME).map -specs=nano.specs -specs=nosys.specs $(CPU) -g3 -gdwarf-2

ifeq ($(FLOAT_TYPE), hard)
CFLAGS += -fsingle-precision-constant -Wdouble-promotion
CFLAGS += -mfpu=fpv4-sp-d16 -mfloat-abi=hard
else
CFLAGS += -msoft-float
endif

CFLAGS += -DUSE_STDPERIPH_DRIVER

# Default to STM32F40_41xxx if no device is passed
ifeq ($(DEVICE_DEF), )
DEVICE_DEF = STM32F2xx
endif

CFLAGS += -D$(DEVICE_DEF)

#vpath %.c src
vpath %.a lib


# Includes
CFLAGS += -Iinc -I$(BASEDIR)/Lib/CMSIS/ -I$(BASEDIR)/Lib/CMSIS/DeviceSupport/
CFLAGS += -I$(BASEDIR)/uCOS/uC-CPU/
CFLAGS += -I$(BASEDIR)/uCOS/uC-CPU/ARM-Cortex-M3/GNU/
CFLAGS += -I$(BASEDIR)/APP/inc/
CFLAGS += -I$(BASEDIR)/HW/
CFLAGS += -I$(BASEDIR)/uCOS/uC-LIB/
CFLAGS += -I$(BASEDIR)/Lib/STM32F2xx_StdPeriph_Driver/inc/
CFLAGS += -I$(BASEDIR)/Lib/STM32F2x7_ETH_Driver/inc/
CFLAGS += -I$(BASEDIR)/BSP/inc/
CFLAGS += -I$(BASEDIR)/BSP/bsp_stm32F2X7/inc/
CFLAGS += -I$(BASEDIR)/uCOS/uCOS-III/Source/
CFLAGS += -I$(BASEDIR)/uCOS/uCOS-III/Ports/ARM-Cortex-M3/Generic/GNU/
CFLAGS += -I$(BASEDIR)/uCOS/uCOS-SHELL/inc/
CFLAGS += -I$(BASEDIR)/lwip-1.4.1/port/STM32/include/
CFLAGS += -I$(BASEDIR)/lwip-1.4.1/src/include/lwip/
CFLAGS += -I$(BASEDIR)/lwip-1.4.1/src/include/ipv4/lwip/
CFLAGS += -I$(BASEDIR)/lwip-1.4.1/src/include/netif/

# Libraries to link
LD_SYS_LIBS = -lxxyyzz -L$(BASEDIR)/build/

# Extra includes
INCLUDE_PATHS += -I$(BASEDIR)/Lib/STM32F2xx_StdPeriph_Driver/inc
INCLUDE_PATHS += -I$(BASEDIR)/Lib/STM32F2x7_ETH_Driver/inc

#CFLAGS += -Map $(OUTPATH)/$(PROJ_NAME).map

OBJS = $(SRCS:.c=.o)
OBJS += $(S_SRCS:.s=.o)

LDOBJS = startup_stm32f2xx.o
LDOBJS += system_stm32f2xx.o
LDOBJS += main.o

###################################################

.PHONY: lib bsp hw app proj demo

all: lib ucos bsp hw lwip app demo proj
	$(SIZE) $(OUTPATH)/$(PROJ_NAME).elf

lib:
	$(MAKE) -C Lib FLOAT_TYPE=$(FLOAT_TYPE) BINPATH=$(BINPATH) DEVICE_DEF=$(DEVICE_DEF) BASEDIR=$(BASEDIR) OUTPATH=$(OUTPATH)

ucos:
	$(MAKE) -C uCOS FLOAT_TYPE=$(FLOAT_TYPE) BINPATH=$(BINPATH) DEVICE_DEF=$(DEVICE_DEF) BASEDIR=$(BASEDIR) OUTPATH=$(OUTPATH)

bsp:
	$(MAKE) -C BSP FLOAT_TYPE=$(FLOAT_TYPE) BINPATH=$(BINPATH) DEVICE_DEF=$(DEVICE_DEF) BASEDIR=$(BASEDIR) OUTPATH=$(OUTPATH)

hw:
	$(MAKE) -C HW FLOAT_TYPE=$(FLOAT_TYPE) BINPATH=$(BINPATH) DEVICE_DEF=$(DEVICE_DEF) BASEDIR=$(BASEDIR) OUTPATH=$(OUTPATH)

lwip:
	$(MAKE) -C lwip-1.4.1 FLOAT_TYPE=$(FLOAT_TYPE) BINPATH=$(BINPATH) DEVICE_DEF=$(DEVICE_DEF) BASEDIR=$(BASEDIR) OUTPATH=$(OUTPATH)

app:
	$(MAKE) -C APP FLOAT_TYPE=$(FLOAT_TYPE) BINPATH=$(BINPATH) DEVICE_DEF=$(DEVICE_DEF) BASEDIR=$(BASEDIR) OUTPATH=$(OUTPATH)

demo:
	cp -rp build/ build-test/
	rm -f build-test/main.o
	rm -f $(OUTPATH)/libxxyyzz.a
	rm -f build-test/startup_stm32f2xx.o
	rm -f build-test/system_stm32f2xx.o
	$(AR) rc $(OUTPATH)/libxxyyzz.a build-test/*.o

proj: $(OUTPATH)/$(PROJ_NAME).elf

.s.o:
	$(AS) $(CPU) -o $(addprefix $(OUTPATH)/, $@) $<

.c.o:
	$(CC) $(CFLAGS) -std=gnu99 $(INCLUDE_PATHS) -o $(addprefix  $(OUTPATH)/, $(notdir $@)) $<

$(OUTPATH)/$(PROJ_NAME).elf: $(OBJS)
	$(LD) -T$(LINKER_SCRIPT) $(LDFLAGS) -o $@ $(addprefix $(OUTPATH)/, $(LDOBJS)) $(LD_SYS_LIBS)
	$(OBJCOPY) -O ihex $(OUTPATH)/$(PROJ_NAME).elf $(OUTPATH)/$(PROJ_NAME).hex
	$(OBJCOPY) -O binary $(OUTPATH)/$(PROJ_NAME).elf $(OUTPATH)/$(PROJ_NAME).bin
	$(OBJDUMP) -S --disassemble $(OUTPATH)/$(PROJ_NAME).elf > $(OUTPATH)/$(PROJ_NAME).dis
	rm -rf build-test/	

clean:
	rm -f $(OUTPATH)/*.o
	rm -f $(OUTPATH)/*.a
	rm -f $(OUTPATH)/$(PROJ_NAME).elf
	rm -f $(OUTPATH)/$(PROJ_NAME).hex
	rm -f $(OUTPATH)/$(PROJ_NAME).bin
	rm -f $(OUTPATH)/$(PROJ_NAME).dis
	rm -f $(OUTPATH)/$(PROJ_NAME).map
	rm -rf build-test/	
	# Remove this line if you don't want to clean the libs as well
	$(MAKE) clean -C Lib FLOAT_TYPE=$(FLOAT_TYPE) BINPATH=$(BINPATH) DEVICE_DEF=$(DEVICE_DEF) BASEDIR=$(BASEDIR) OUTPATH=$(OUTPATH)
	$(MAKE) clean -C uCOS FLOAT_TYPE=$(FLOAT_TYPE) BINPATH=$(BINPATH) DEVICE_DEF=$(DEVICE_DEF) BASEDIR=$(BASEDIR) OUTPATH=$(OUTPATH)
	$(MAKE) clean -C BSP FLOAT_TYPE=$(FLOAT_TYPE) BINPATH=$(BINPATH) DEVICE_DEF=$(DEVICE_DEF) BASEDIR=$(BASEDIR) OUTPATH=$(OUTPATH)
	$(MAKE) clean -C HW FLOAT_TYPE=$(FLOAT_TYPE) BINPATH=$(BINPATH) DEVICE_DEF=$(DEVICE_DEF) BASEDIR=$(BASEDIR) OUTPATH=$(OUTPATH)
	$(MAKE) clean -C lwip-1.4.1 FLOAT_TYPE=$(FLOAT_TYPE) BINPATH=$(BINPATH) DEVICE_DEF=$(DEVICE_DEF) BASEDIR=$(BASEDIR) OUTPATH=$(OUTPATH)
	$(MAKE) clean -C APP FLOAT_TYPE=$(FLOAT_TYPE) BINPATH=$(BINPATH) DEVICE_DEF=$(DEVICE_DEF) BASEDIR=$(BASEDIR) OUTPATH=$(OUTPATH)
	
