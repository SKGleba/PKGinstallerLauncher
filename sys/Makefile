TARGET   = pkih
OBJS     = main.o
LIBS = -lvita2d -lSceDisplay_stub -lSceLibKernel_stub -lSceGxm_stub -lSceIofilemgr_stub \
	-lSceSysmodule_stub -lSceCtrl_stub -lScePgf_stub -lScePvf_stub -lSceShellSvc_stub -lSceAppMgr_stub_weak \
	-lSceCommonDialog_stub -lScePower_stub -ltaihen_stub -lSceVshBridge_stub -lfreetype -lpng -ljpeg -lz -lm -lc
PREFIX  = arm-vita-eabi
CC      = $(PREFIX)-gcc
CFLAGS  = -Wl,-q -Wall -fno-lto
ASFLAGS = $(CFLAGS)
all: eboot.bin
	@rm -rf $(TARGET).velf $(TARGET).elf $(OBJS) \ $@
eboot.bin: $(TARGET).velf
	vita-make-fself -c $< $@
%.velf: %.elf
	vita-elf-create $< $@
$(TARGET).elf: $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@
