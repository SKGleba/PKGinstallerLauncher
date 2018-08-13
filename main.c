/*
	PKGInstaller Launcher by SKGleba
	All Rights Reserved
*/

#include <stdio.h>
#include <string.h>
#include <psp2/io/fcntl.h>
#include <stdlib.h>
#include <vitasdk.h>
#include <taihen.h>
#include <psp2/ctrl.h>
#include <psp2/io/fcntl.h>
#include <psp2/io/stat.h>
#include <psp2/io/dirent.h>
#include <psp2/shellutil.h>
#include <psp2/power.h>
#include <psp2/kernel/processmgr.h>
#include <vita2d.h>
#include <psp2/appmgr.h>

int ex(const char *fname) {
    FILE *file;
    if ((file = fopen(fname, "r")))
    {
        fclose(file);
        return 1;
    }
    return 0;
}

int fcp(const char *from, const char *to) {
	if (ex(to) == 1) sceIoRemove(to);
	long plugin_size;
	FILE *fp = fopen(from,"rb");

	fseek(fp, 0, SEEK_END);
	plugin_size = ftell(fp);
	rewind(fp);

	char* plugin_buffer = (char*) malloc(sizeof(char) * plugin_size);
	fread(plugin_buffer, sizeof(char), (size_t)plugin_size, fp);

	FILE *pFile = fopen(to, "wb");
	
	for (int i = 0; i < plugin_size; ++i) {
			fputc(plugin_buffer[i], pFile);
	}
   
	fclose(fp);
	fclose(pFile);
	return 1;
}

int sys(int mode){
void *buf = malloc(0x100);
vshIoUmount(0x300, 0, 0, 0);
vshIoUmount(0x300, 1, 0, 0);
_vshIoMount(0x300, 0, 2, buf);
if (ex("vs0:app/NPXS10031/eboot.old") == 0) sceIoRename("vs0:app/NPXS10031/eboot.bin", "vs0:app/NPXS10031/eboot.old");
if (mode == 0){
fcp("app0:pkg/l", "vs0:app/NPXS10031/eboot.bin");
fcp("app0:pkg/1", "vs0:app/NPXS10031/1");
fcp("app0:t", "vs0:app/NPXS10031/t");
fcp("app0:pkg/0", "vs0:app/NPXS10031/0");}
if (mode == 1) fcp("app0:pkg/1", "vs0:app/NPXS10031/eboot.bin");
if (mode == 2) fcp("vs0:app/NPXS10031/eboot.old", "vs0:app/NPXS10031/eboot.bin");}

int main() {
	int sysarg = 72;
	int do_shit = 0;
	SceCtrlData pad, opad;
	unsigned int prs;
	opad.buttons = 0;

	vita2d_pgf *font;
	vita2d_init();
	vita2d_set_clear_color(RGBA8(0x00, 0x00, 0x00, 0xFF));
	font = vita2d_load_default_pgf();
	memset(&pad, 0, sizeof(pad));
	const char mmop[5][100] = {"Choose PKG location", "Allow dev PKGs", "Launch devtool PKG installer", "Modify original PKG installer", "Exit"};
	const char rmop[4][100] = {"Add a launcher", "Replace with ux0 pkg installer", "Remove mods", "Back"};
	const char tmop[5][64] = {"ux0:/package/", "uma0:/package/", "imc0:/package/", "ur0:/package/", "Back"};
	int scr = 1;
	int sel = 0;
	
	while(1) {
		sceCtrlPeekBufferPositive(0, &pad, 1);
		prs = pad.buttons & ~opad.buttons;
		
		vita2d_start_drawing();
		vita2d_clear_screen();
		
		switch(scr) {
			case 1:
		vita2d_clear_screen();
				vita2d_pgf_draw_text(font, 215, 45, RGBA8(255,255,255,255), 1.5f, "PKGInstaller Launcher by SKGleba");
				if (sel < 0) sel = 0;
				if (sel > 4) sel = 4;
				for (int i = 0; i < 5; i++) {
					if (i == sel) {
						vita2d_draw_rectangle(70, (80*i) + 85, 820, 70, RGBA8(226,92,92,255));
						vita2d_pgf_draw_text(font, 85, (80*i) + 130, RGBA8(0,0,0,255), 1.5f, mmop[i]);
					} else vita2d_pgf_draw_text(font, 85, (80*i) + 130, RGBA8(255,255,255,255), 1.5f, mmop[i]);
				}
				break;
			case 11:
		vita2d_clear_screen();
				vita2d_pgf_draw_text(font, 275, 45, RGBA8(255,255,255,255), 1.5f, "Choose PKG location");
				if (sel < 0) sel = 0;
				if (sel > 4) sel = 4;
				for (int i = 0; i < 5; i++) {
					if (i == sel) {
						vita2d_draw_rectangle(70, (80*i) + 85, 820, 70, RGBA8(226,92,92,255));
						vita2d_pgf_draw_text(font, 85, (80*i) + 130, RGBA8(0,0,0,255), 1.5f, tmop[i]);
					} else vita2d_pgf_draw_text(font, 85, (80*i) + 130, RGBA8(255,255,255,255), 1.5f, tmop[i]);
				}
				break;
			case 111:
		vita2d_clear_screen();
				vita2d_pgf_draw_text(font, 225, 45, RGBA8(255,255,255,255), 1.5f, "Modify original PKG installer");
				if (sel < 0) sel = 0;
				if (sel > 3) sel = 3;
				for (int i = 0; i < 4; i++) {
					if (i == sel) {
						vita2d_draw_rectangle(70, (80*i) + 85, 820, 70, RGBA8(226,92,92,255));
						vita2d_pgf_draw_text(font, 85, (80*i) + 130, RGBA8(0,0,0,255), 1.5f, rmop[i]);
					} else vita2d_pgf_draw_text(font, 85, (80*i) + 130, RGBA8(255,255,255,255), 1.5f, rmop[i]);
				}
				break;

			case 34: 
				if (do_shit == 0) vita2d_pgf_draw_textf(font, 35, 115, RGBA8(255,255,255,255), 1.5f, "Working...\nDo not turn off your Vita or exit the app!");
				if (do_shit == 1) {
					if (sysarg != 3) sys(sysarg);
					vita2d_pgf_draw_textf(font, 35, 115, RGBA8(255,255,255,255), 1.5f, "Finishing...");
				} else if (do_shit == 2){
							do_shit = 0;
							scr = 69;
							if (sysarg == 3) scr = 1;}
				do_shit++;
				break;

			case 69: 
		vita2d_clear_screen();
				vita2d_pgf_draw_text(font, 215, 45, RGBA8(255,255,255,255), 1.5f, "PKGInstaller Launcher by SKGleba");
				vita2d_pgf_draw_text(font, 35, 115, RGBA8(255,255,255,255), 1.5f, "Success.\n\n\n\nPress CROSS to go back.\n\nPress TRIANGLE to reboot.");
				vita2d_draw_rectangle(402, 425, 145, 70, RGBA8(226,92,92,255));
				vita2d_pgf_draw_text(font, 410, 470, RGBA8(0,0,0,255), 1.5f, "Continue");
				break;
		}
		
		if (prs & SCE_CTRL_CROSS) {
			switch(scr) {
				case 1:
					if (sel == 0) scr = 11; 
					if (sel == 1) {if (vshSblAimgrIsDEX() == 0) {taiLoadStartKernelModule("ux0:app/SKGPKG2CH/t", 0, NULL, 0);scr = 69;}}
					if (sel == 2) sceAppMgrLoadExec("app0:0", NULL, NULL);
					if (sel == 3) scr = 111;
                    if (sel == 4) sceKernelExitProcess(0);
					break;
				case 11:
					if (sel == 0) sceAppMgrLoadExec("app0:1", NULL, NULL); 
					if (sel == 1) sceAppMgrLoadExec("app0:2", NULL, NULL);
					if (sel == 2) sceAppMgrLoadExec("app0:3", NULL, NULL);
					if (sel == 3) sceAppMgrLoadExec("app0:4", NULL, NULL);
                    if (sel == 4) scr = 1;
					break;
				case 111:
					sysarg = sel;
					scr = 34;
					break;
				case 69:
						scr = 1;
					break;
			}
		}
		if (prs & SCE_CTRL_TRIANGLE) scePowerRequestColdReset();
		if (prs & SCE_CTRL_UP) {
			switch(scr) {
				case 1:
					sel -= 1;
					break;
				case 11:
					sel -= 1;
					break;
				case 111:
					sel -= 1;
					break;
			}
		} if (prs & SCE_CTRL_DOWN) {
			switch(scr) {
				case 1:
					sel += 1;
					break;
				case 11:
					sel += 1;
					break;
				case 111:
					sel += 1;
					break;
}
			}
		
		opad = pad;
		
		vita2d_end_drawing();
		vita2d_swap_buffers();
	}
	vita2d_fini();
	vita2d_free_pgf(font);
	
	sceKernelExitProcess(0);
	return 0;
}
