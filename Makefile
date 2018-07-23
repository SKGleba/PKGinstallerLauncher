TITLE_ID = SKGPKG2CH
TARGET   = PKGInstallerL
OBJS     = main.o

LIBS = -lvita2d -lSceDisplay_stub -lSceLibKernel_stub -lSceGxm_stub -lSceIofilemgr_stub \
	-lSceSysmodule_stub -lSceCtrl_stub -lScePgf_stub -lScePvf_stub -lSceShellSvc_stub -lSceAppMgr_stub_weak \
	-lSceCommonDialog_stub -lScePower_stub -ltaihen_stub -lSceVshBridge_stub -lfreetype -lpng -ljpeg -lz -lm -lc

PREFIX  = arm-vita-eabi
CC      = $(PREFIX)-gcc
CFLAGS  = -Wl,-q -Wall -fno-lto
ASFLAGS = $(CFLAGS)

all: $(TARGET).vpk

%.vpk: eboot.bin
	vita-mksfoex -s TITLE_ID=$(TITLE_ID) "PKGInstaller Launcher" param.sfo
	vita-pack-vpk -s param.sfo -b eboot.bin \
    -a sce_sys/icon0.png=sce_sys/icon0.png \
    -a sce_sys/livearea/contents/startup.png=sce_sys/livearea/contents/startup.png \
    -a sce_sys/livearea/contents/bg.png=sce_sys/livearea/contents/bg.png \
    -a sce_sys/livearea/contents/template.xml=sce_sys/livearea/contents/template.xml \
    -a pkg/2=2 \
	-a pkg/0=0 \
	-a pkg/3=3 \
	-a pkg/1=1 \
    -a pkg/4=4 \
    -a pkg/sample_plugin.rco=sample_plugin.rco \
    -a pkg/h=h \
    -a pkg/t=t \$@

eboot.bin: $(TARGET).velf
	vita-make-fself -c $< $@

%.velf: %.elf
	vita-elf-create $< $@

$(TARGET).elf: $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

%.o: %.png
	$(PREFIX)-ld -r -b binary -o $@ $^

clean:
	@rm -rf $(TARGET).vpk $(TARGET).velf $(TARGET).elf $(OBJS) \
		eboot.bin param.sfo

vpksend: $(TARGET).vpk
	curl -T $(TARGET).vpk ftp://$(PSVITAIP):1337/ux0:/
	@echo "Sent."

send: eboot.bin
	curl -T eboot.bin ftp://$(PSVITAIP):1337/ux0:/app/$(TITLE_ID)/
	@echo "Sent."
