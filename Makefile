LDSCRIPT = lib/linker_scripts/samd21g18a_flash.ld
BOOTUP = lib/startup_samd21.o
MCUTYPE=__SAMD21G18A__

OBJS=$(BOOTUP) blink.o

# Tools
CC=arm-none-eabi-gcc
LD=arm-none-eabi-gcc
SZ=arm-none-eabi-size -d

ELF=blink.elf

LDFLAGS+= -T$(LDSCRIPT) -mcpu=cortex-m0 -Wl,--gc-sections
CFLAGS+= -mcpu=cortex-m0 -g
CFLAGS+= -Ilib/ -I.
CFLAGS+= -D$(MCUTYPE)

$(ELF):		$(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

# compile
%.o:	%.c
	$(CC) -c $(CFLAGS) $< -o $@

%.o:	%.s
	$(AS) $< -o $@
size: $(ELF)
	$(SZ) $(ELF)

clean:
	rm -f $(OBJS) $(OBJS:.o=.d) $(ELF)
