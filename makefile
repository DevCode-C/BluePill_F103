CC = arm-none-eabi
CORE = -mcpu=cortex-m3 -mthumb -mfloat-abi=soft
TARGET = temp
SYMBOLS = -DSTM32F103xB -DUSE_HAL_DRIVER
# SYMBOLS += -DSEMIHOSTING
# MOdification
VPATH = App/Source cmsis/startup HALF1/Src FreeRTOS/source 

INCLUDES = -I App/Include -I cmsis/core -I cmsis/registers -I HALF1/Inc
INCLUDES += -I FreeRTOS/include

F_SECTIONS = -ffunction-sections -fdata-sections
SPECS_V = -Wl,--gc-sections --specs=rdimon.specs --specs=nano.specs

CFLAGS = -g3 -c $(CORE) -std=gnu99 -Wall -O0 $(F_SECTIONS) $(INCLUDES) $(SYMBOLS)
LDFLAGS = $(CORE) $(SPECS_V) -T cmsis/linker/STM32F103XB_FLASH.ld -Wl,-Map=$(OUTPUT_F)/$(TARGET).map
OBJS_F = Build/Obj
OUTPUT_F = Build

#---Linter ccpcheck flags--------------------------------------------------------------------------
LNFLAGS  = --inline-suppr       # comments to suppress lint warnings
LNFLAGS += --quiet              # spit only useful information
LNFLAGS += --std=c99            # check against C11
LNFLAGS += --template=gcc       # display warning gcc style
LNFLAGS += --force              # evaluate all the #if sentences
LNFLAGS += --platform=unix32    # lint againt a unix32 platform, but we are using arm32
LNFLAGS += --cppcheck-build-dir=Build/cppcheck

SRCS  = main.c App_ints.c App_msps.c startup_stm32f103xb.c system_stm32f1xx.c 
SRCS += stm32f1xx_hal.c stm32f1xx_hal_cortex.c stm32f1xx_hal_rcc.c stm32f1xx_hal_flash.c
SRCS += stm32f1xx_hal_gpio.c stm32f1xx_hal_uart.c stm32f1xx_hal_dma.c stm32f1xx_hal_rtc.c

SRCS += tasks.c list.c queue.c port.c

OBJS = $(SRCS:%.c=$(OBJS_F)/%.o)

all:$(TARGET) | misra

$(TARGET) : $(addprefix $(OUTPUT_F)/,$(TARGET).elf)
	$(CC)-objcopy -Oihex $< Build/$(TARGET).hex
	$(CC)-objdump -S $< > Build/$(TARGET).lst
	$(CC)-size --format=berkeley $<

$(addprefix $(OUTPUT_F)/,$(TARGET).elf): $(OBJS)
	@$(CC)-gcc $(LDFLAGS) -o $@ $^	

$(addprefix $(OBJS_F)/,%.o) : %.c
	@mkdir -p $(OBJS_F)
	@$(CC)-gcc -MD $(CFLAGS) -o $@ $<

$(addprefix $(OBJS_F)/,%.o) : %.s
	@$(CC)-as -c $(CORE) -o $@ $<

-include $(OBJS_F)/*.d

.PHONY: clean flash open debug  misra

misra:
	@echo "Misra verification:"
	@mkdir -p Build/cppcheck
	@cppcheck --addon=Tools/cppcheck/misra.json --suppressions-list=Tools/cppcheck/.msupress $(LNFLAGS) App/Source App/Include -iApp/Source/App_ints.c 
clean:
	@rm -rf Build
flash:all
	@openocd -f interface/stlink.cfg -f target/stm32f_BluePill.cfg -c "program Build/$(TARGET).hex verify reset" -c shutdown
open:
	@openocd -f interface/stlink.cfg -f target/stm32f_BluePill.cfg -c "reset_config srst_only srst_nogate"
debug:
	@$(CC)-gdb Build/$(TARGET).elf -q -iex "set auto-load safe-path /"