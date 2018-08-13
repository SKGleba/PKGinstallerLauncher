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

int main() {
	SceCtrlData pad, opad;
	unsigned int prs;
	opad.buttons = 0;

	vita2d_pgf *font;
	vita2d_init();
	vita2d_set_clear_color(RGBA8(0x00, 0x00, 0x00, 0xFF));
	font = vita2d_load_default_pgf();
	memset(&pad, 0, sizeof(pad));
	const char mmop[5][100] = {"Install package from ux0:/package/", "Launch devtool PKG installer", "Launch original PKG installer", "Allow dev PKGs", "Exit"};
	int scr = 1;
	int sel = 0;
	
	while(1) {
		sceCtrlPeekBufferPositive(0, &pad, 1);
		prs = pad.buttons & ~opad.buttons;
		
		vita2d_start_drawing();
		vita2d_clear_screen();
		
		vita2d_pgf_draw_text(font, 215, 45, RGBA8(255,255,255,255), 1.5f, "PKGInstaller Launcher (sys) by SKGleba");
		
		switch(scr) {
			case 1: // Main Menu
				if (sel < 0) sel = 0;
				if (sel > 4) sel = 4;
				for (int i = 0; i < 5; i++) {
					if (i == sel) {
						vita2d_draw_rectangle(70, (80*i) + 85, 820, 70, RGBA8(226,92,92,255));
						vita2d_pgf_draw_text(font, 85, (80*i) + 130, RGBA8(0,0,0,255), 1.5f, mmop[i]);
					} else vita2d_pgf_draw_text(font, 85, (80*i) + 130, RGBA8(255,255,255,255), 1.5f, mmop[i]);
				}
				break;
			case 69: 
				vita2d_pgf_draw_text(font, 35, 115, RGBA8(255,255,255,255), 1.5f, "Success.\n\n\n\nPress CROSS to go back.");
				vita2d_draw_rectangle(402, 425, 145, 70, RGBA8(226,92,92,255));
				vita2d_pgf_draw_text(font, 410, 470, RGBA8(0,0,0,255), 1.5f, "Continue");
				break;
		}
		
		if (prs & SCE_CTRL_CROSS) {
			switch(scr) {
				case 1:
					if (sel == 0) sceAppMgrLoadExec("app0:1", NULL, NULL);
					if (sel == 1) sceAppMgrLoadExec("app0:0", NULL, NULL);
					if (sel == 2) sceAppMgrLoadExec("app0:eboot.old", NULL, NULL);
					if (sel == 3) {if (vshSblAimgrIsDEX() == 0) {taiLoadStartKernelModule("vs0:app/NPXS10031/t", 0, NULL, 0);scr = 69;}}
                    if (sel == 4) sceKernelExitProcess(0);
					break;
				case 69:
						scr = 1;
					break;
			}
		}
		
		if (prs & SCE_CTRL_UP) {
			switch(scr) {
				case 1:
					sel -= 1;
					break;
				case 11:
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
