#general Makefile

include Makefile.common

LDFLAGS=$(COMMONFLAGS) -fno-exceptions -ffunction-sections -fdata-sections -L$(LIBDIR) -nostartfiles -Wl,--gc-sections,-Tlinker.ld

LDLIBS+=-lm

LDLIBS+=-lstm32

STARTUP=startup.c

all: libs src
	$(CC) -o $(PROGRAM).elf $(LDFLAGS) \
					-Wl,--whole-archive \
								src/app.a \
					-Wl,--no-whole-archive \
							$(LDLIBS)
	$(OBJCOPY) -O ihex $(PROGRAM).elf $(PROGRAM).hex
	$(OBJCOPY) -O binary $(PROGRAM).elf $(PROGRAM).bin
#Extract info contained in ELF to readable text-files:
	arm-none-eabi-readelf -a $(PROGRAM).elf > $(PROGRAM).info_elf
	arm-none-eabi-size -d -B -t $(PROGRAM).elf > $(PROGRAM).info_size
	arm-none-eabi-objdump -S $(PROGRAM).elf > $(PROGRAM).info_code
	arm-none-eabi-nm -t d -S --size-sort -s $(PROGRAM).elf > $(PROGRAM).info_symbol
.PHONY: libs src clean tshow flash
libs:
	$(MAKE) -C libs $@
src:
	$(MAKE) -C src $@
clean:
	$(MAKE) -C src $@
	$(MAKE) -C libs $@
	rm -f $(PROGRAM).elf $(PROGRAM).hex $(PROGRAM).bin $(PROGRAM).info_elf $(PROGRAM).info_size
	rm -f $(PROGRAM).info_code
	rm -f $(PROGRAM).info_symbol
tshow:
	@echo "######################################################################################################"
	@echo "################# optimize settings: $(InfoTextLib), $(InfoTextSrc)"
	@echo "######################################################################################################"
flash:
	@echo "######################################################################################################"
	@echo "################# killing all of st-util instances, flashing firmware..."
	@echo "######################################################################################################"
	@killall st-util||:
	st-flash --reset write $(PROGRAM).bin 0x8000000 
debug:
	@{ st-util > /dev/null 2>&1 & PID=$$!; arm-none-eabi-gdb --tui -x gdb-commands ${PROGRAM}.elf; kill $$PID; } ||:
