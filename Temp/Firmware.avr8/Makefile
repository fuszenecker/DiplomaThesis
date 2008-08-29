# ----------------------------------------------------------------------------
# This is the Makefile for ST's STM32 (ARM-Cortex based) Microcontollers
#
# Change CROSS parameter if you want to use a different C/C++ compiler or 
# the path to the C/C++ compiler is different.
# ----------------------------------------------------------------------------

CROSS		= avr-
AS			= $(CROSS)as
MCU			= -mmcu=atmega16
ASFLAGS		= $(MCU) -g -gdwarf-2
LD			= $(CROSS)ld
LDFLAGS		= -T /usr/lib/ldscripts/avr5.x

OBJDUMP		= $(CROSS)objdump
ODFLAGS		= -h -j .text -j .data -j .bss -dS 
OBJCOPY		= $(CROSS)objcopy
OCFLAGS		= -j .text -j .eeprom -O
NM			= $(CROSS)nm

PROG		= firmware

# ----------------------------------------------------------------------------
# Core modules of the formware application
# ----------------------------------------------------------------------------

OBJS		= sysinit.o

# ----------------------------------------------------------------------------
# Hardver drivers (Hardver abstration layer and Software library)
# ----------------------------------------------------------------------------

# ------------
# HAL modules:
# ------------

# OBJS		+=

# -------------------------
# Software library modules:
# -------------------------

# OBJS		+=

# ----------------------------------------------------------------------------
# Compile the firmware
# ----------------------------------------------------------------------------

all: clean $(OBJS)
	$(LD) $(LDFLAGS) -o $(PROG) $(OBJS) $(EXT_LIBS)
	$(OBJDUMP) $(ODFLAGS) $(PROG) > $(PROG).list 
	$(OBJCOPY) $(OCFLAGS) binary $(PROG) $(PROG).bin
	$(OBJCOPY) $(OCFLAGS) ihex $(PROG) $(PROG).hex
	$(NM) $(PROG) | sort > $(PROG).nm

# ----------------------------------------------------------------------------
# Clean unnecessary files
# ----------------------------------------------------------------------------

clean:
	rm -rf $(OBJS) $(PROG) $(PROG).list $(PROG).hex $(PROG).nm $(PROG).bin

burn:
	avrdude -p atmega16 -c ponyser -E reset -P /dev/ttyS0 -U flash:w:firmware.hex:i

.S.o:
	$(AS) $(ASFLAGS) -o $@ $<

