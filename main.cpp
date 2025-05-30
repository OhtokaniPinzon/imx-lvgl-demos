/*
 * Copyright 2023 NXP 
 * SPDX-License-Identifier: BSD-3-Clause
*/

#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"
#include "lv_drivers/display/fbdev.h"
#include "lv_drivers/display/drm.h"
#include "lv_drivers/wayland/wayland.h"  
#include "lv_drivers/indev/evdev.h"
#include "lvgl/src/hal/lv_hal_indev.h"
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include "lvgl/examples/lv_examples.h"

#define H_RES (1280)
#define V_RES (720)

#define DISP_BUF_SIZE (128 * 1024)

void sig_handler(int signum)
{
	printf("\nInside Signal handler function\n");
	printf("------SIGINT signal catched------\n");
	printf("Program exit...\n");

    lv_deinit();
#if USE_FBDEV /*framebuffer*/
	fbdev_exit();
#endif
#if USE_DRM /*DRM*/
	drm_exit();
#endif
#if USE_WAYLAND /*wayland*/	
    lv_wayland_deinit();
#endif
	
	exit(0);
}


bool close_cb(lv_disp_t * disp)
{
	printf("\nInside close callback function!!\n");
	printf("------Close event catched------\n");
	printf("Program exit...\n");

    lv_deinit();
#if USE_FBDEV /*framebuffer*/
	fbdev_exit();
#endif
#if USE_DRM /*DRM*/
	drm_exit();
#endif
#if USE_WAYLAND /*wayland*/	
    lv_wayland_deinit();
#endif	

    exit(0);
}

static void hal_init(void)
{
	lv_disp_t * disp;

#if USE_FBDEV /*framebuffer*/
	/*Linux frame buffer device init*/
	fbdev_init();
	
	static lv_color_t buf1[DISP_BUF_SIZE];
	static lv_color_t buf2[DISP_BUF_SIZE];

    /*Initialize a descriptor for the buffer*/
    static lv_disp_draw_buf_t disp_buf;
    lv_disp_draw_buf_init(&disp_buf, buf1, buf2, DISP_BUF_SIZE);

    /*Initialize and register a display driver*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf   = &disp_buf;
    disp_drv.flush_cb   = fbdev_flush;
    disp_drv.hor_res    = H_RES;
    disp_drv.ver_res    = V_RES;
    disp = lv_disp_drv_register(&disp_drv);    
#endif

#if USE_DRM /*DRM*/
	/*Linux DRM device init*/
	drm_init();
	
	static lv_color_t buf1[DISP_BUF_SIZE];
	static lv_color_t buf2[DISP_BUF_SIZE];

    /*Initialize a descriptor for the buffer*/
    static lv_disp_draw_buf_t disp_buf;
    lv_disp_draw_buf_init(&disp_buf, buf1, buf2, DISP_BUF_SIZE);

    /*Initialize and register a display driver*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf   = &disp_buf;
    disp_drv.flush_cb   = drm_flush;
    disp_drv.hor_res    = H_RES;
    disp_drv.ver_res    = V_RES;
    disp = lv_disp_drv_register(&disp_drv);    
#endif

#if USE_WAYLAND /*wayland*/	
    lv_wayland_init();
    disp = lv_wayland_create_window(H_RES, V_RES, "Window Title", close_cb);
#endif

	printf("Disp %d \n", disp);
}

int main(int argc, char ** argv)
{
	signal(SIGINT, sig_handler); // Register signal handler

    /*LittlevGL init*/
    lv_init();
    /*Linux Wayland device init*/

	hal_init();
	
	if(strcmp(argv[1],"widgets") == 0)
	{
		printf("Running Demo Widgets \n");
		lv_demo_widgets();
	}

	if(strcmp(argv[1],"music") == 0)
	{
		printf("Running Demo Music \n");
		lv_demo_music();
	}

	if(strcmp(argv[1],"benchmark") == 0)
	{
		printf("Running Demo Benchmark \n");
		lv_demo_benchmark();
	}

	if(strcmp(argv[1],"keypad") == 0)
	{
		printf("Running Demo Keypad \n");
		lv_demo_keypad_encoder();
	}

	if(strcmp(argv[1],"stress") == 0)
	{
		printf("Running Demo Stress \n");
		lv_demo_stress();
	}


    /*Handle LitlevGL tasks (tickless mode)*/
    while(1) 
    {
        lv_timer_handler();
        usleep(5000);
    }

    return 0;
}

