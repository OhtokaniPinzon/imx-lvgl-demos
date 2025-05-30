/*
 * Copyright 2023 NXP 
 * SPDX-License-Identifier: BSD-3-Clause
*/

#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"
#include "lvgl/src/drivers/wayland/lv_wayland.h"  
#include "lvgl/src/drivers/display/fb/lv_linux_fbdev.h"  
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

#define H_RES 1280
#define V_RES 720


bool close_cb(lv_disp_t * disp)
{
    printf("\nInside close callback function!!\n");
    printf("Program exit...\n");
    lv_deinit();

    exit(0);
}

static const char *getenv_default(const char *name, const char *dflt)
{
    return getenv(name) ? : dflt;
}

static  lv_display_t * hal_init(void)
{
    lv_display_t * disp = NULL;

#if LV_USE_LINUX_FBDEV
    const char *device = getenv_default("LV_LINUX_FBDEV_DEVICE", "/dev/fb0");
    disp = lv_linux_fbdev_create();
    lv_linux_fbdev_set_file(disp, device);

#elif LV_USE_LINUX_DRM
    const char *device = getenv_default("LV_LINUX_DRM_CARD", "/dev/dri/card0");
    disp = lv_linux_drm_create();
    lv_linux_drm_set_file(disp, device, -1);

#elif LV_USE_WAYLAND
    /*Linux Wayland device init*/
    disp = lv_wayland_window_create(H_RES, V_RES, "Window Title", NULL);
#endif

	return disp;
}


int main(int argc, char ** argv)
{
	lv_display_t * disp;

    /*LittlevGL init*/
    lv_init();

	disp = hal_init();

    printf("Disp = %d \n", (int)disp);
	if (disp == NULL) {
		printf("display initialization failure!\r\n");
		return 1;
	}

	if (!lv_demos_create(&argv[1], argc - 1)) {
		printf("lv_demos initialization failure!\r\n");
		lv_demos_show_help();
		goto demo_end;
	}

    /* Handle LVGL tasks */
#if LV_USE_WAYLAND
	bool completed;
    while (1) {

        completed = lv_wayland_timer_handler();

        if (completed) {
            /* wait only if the cycle was completed */
            usleep(LV_DEF_REFR_PERIOD * 1000);
        }

        /* Run until the last window closes */
        if (!lv_wayland_window_is_open(NULL)) {
            break;
        }
    }
#else
	while (1) {
	uint32_t delay = lv_timer_handler();
		if (delay < 1) delay = 1;
		usleep(delay * 1000);
		lv_tick_inc(50);
	}
#endif
demo_end:
	lv_deinit();
	return 0;
}

