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
#include <string.h>
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


    void (*event[4])();
    void (*anim[4])();
    void (*style[15])();
    void (*other[16])();
    void (*scroll[6])();
    void (*layout[12])();
    void (*widget[79])();
    void (*get_started[3])();

	//event;
	event[0] = &lv_example_event_1;
	event[1] = &lv_example_event_2;
	event[2] = &lv_example_event_3;
	event[3] = &lv_example_event_4;
	
	//anim;
	anim[0] = &lv_example_anim_timeline_1;
	anim[1] = &lv_example_anim_2;
	anim[2] = &lv_example_anim_1;
	anim[3] = &lv_example_anim_3;
	
	//style;
	style[0] = &lv_example_style_15;
	style[1] = &lv_example_style_9;
	style[2] = &lv_example_style_12;
	style[3] = &lv_example_style_3;
	style[4] = &lv_example_style_11;
	style[5] = &lv_example_style_2;
	style[6] = &lv_example_style_13;
	style[7] = &lv_example_style_8;
	style[8] = &lv_example_style_10;
	style[9] = &lv_example_style_1;
	style[10] = &lv_example_style_14;
	style[11] = &lv_example_style_5;
	style[12] = &lv_example_style_7;
	style[13] = &lv_example_style_4;
	style[14] = &lv_example_style_6;
	
	//others;
	other[0] = &lv_example_msg_2;
	other[1] = &lv_example_msg_3;
	other[2] = &lv_example_msg_1;
	other[3] = &lv_example_gridnav_2;
	other[4] = &lv_example_gridnav_4;
	other[5] = &lv_example_gridnav_1;
	other[6] = &lv_example_gridnav_3;
	other[7] = &lv_example_fragment_1;
	other[8] = &lv_example_fragment_2;
	other[9] = &lv_example_monkey_1;
	other[10] = &lv_example_monkey_3;
	other[11] = &lv_example_monkey_2;
	other[12] = &lv_example_imgfont_1;
	other[13] = &lv_example_ime_pinyin_1;
	other[14] = &lv_example_ime_pinyin_2;
	other[15] = &lv_example_snapshot_1;
	
	//scroll;
	scroll[0] = &lv_example_scroll_4;
	scroll[1] = &lv_example_scroll_3;
	scroll[2] = &lv_example_scroll_2;
	scroll[3] = &lv_example_scroll_5;
	scroll[4] = &lv_example_scroll_1;
	scroll[5] = &lv_example_scroll_6;
	
	//layouts;
	layout[0] = &lv_example_flex_5;
	layout[1] = &lv_example_flex_2;
	layout[2] = &lv_example_flex_4;
	layout[3] = &lv_example_flex_1;
	layout[4] = &lv_example_flex_6;
	layout[5] = &lv_example_flex_3;
	layout[6] = &lv_example_grid_1;
	layout[7] = &lv_example_grid_6;
	layout[8] = &lv_example_grid_4;
	layout[9] = &lv_example_grid_3;
	layout[10] = &lv_example_grid_2;
	layout[11] = &lv_example_grid_5;
	
	//widgets;
	widget[0] = &lv_example_switch_1;
	widget[1] = &lv_example_msgbox_1;
	widget[2] = &lv_example_tileview_1;
	widget[3] = &lv_example_arc_1;
	widget[4] = &lv_example_arc_2;
	widget[5] = &lv_example_calendar_1;
	widget[6] = &lv_example_meter_4;
	widget[7] = &lv_example_meter_1;
	widget[8] = &lv_example_meter_3;
	widget[9] = &lv_example_meter_2;
	widget[10] = &lv_example_list_2;
	widget[11] = &lv_example_list_1;
	widget[12] = &lv_example_textarea_3;
	widget[13] = &lv_example_textarea_2;
	widget[14] = &lv_example_textarea_1;
	widget[15] = &lv_example_span_1;
	widget[16] = &lv_example_led_1;
	widget[17] = &lv_example_label_2;
	widget[18] = &lv_example_label_1;
	widget[19] = &lv_example_label_4;
	widget[20] = &lv_example_label_5;
	widget[21] = &lv_example_label_3;
	widget[22] = &lv_example_btn_1;
	widget[23] = &lv_example_btn_3;
	widget[24] = &lv_example_btn_2;
	widget[25] = &lv_example_slider_1;
	widget[26] = &lv_example_slider_2;
	widget[27] = &lv_example_slider_3;
	widget[28] = &lv_example_roller_1;
	widget[29] = &lv_example_roller_2;
	widget[30] = &lv_example_roller_3;
	widget[31] = &lv_example_colorwheel_1;
	widget[32] = &lv_example_tabview_2;
	widget[33] = &lv_example_tabview_1;
	widget[34] = &lv_example_checkbox_2;
	widget[35] = &lv_example_checkbox_1;
	widget[36] = &lv_example_spinner_1;
	widget[37] = &lv_example_table_2;
	widget[38] = &lv_example_table_1;
	widget[39] = &lv_example_chart_6;
	widget[40] = &lv_example_chart_5;
	widget[41] = &lv_example_chart_9;
	widget[42] = &lv_example_chart_4;
	widget[43] = &lv_example_chart_8;
	widget[44] = &lv_example_chart_7;
	widget[45] = &lv_example_chart_1;
	widget[46] = &lv_example_chart_3;
	widget[47] = &lv_example_chart_2;
	widget[48] = &lv_example_spinbox_1;
	widget[49] = &lv_example_bar_3;
	widget[50] = &lv_example_bar_2;
	widget[51] = &lv_example_bar_6;
	widget[52] = &lv_example_bar_5;
	widget[53] = &lv_example_bar_1;
	widget[54] = &lv_example_bar_4;
	widget[55] = &lv_example_menu_4;
	widget[56] = &lv_example_menu_1;
	widget[57] = &lv_example_menu_3;
	widget[58] = &lv_example_menu_5;
	widget[59] = &lv_example_menu_2;
	widget[60] = &lv_example_img_3;
	widget[61] = &lv_example_img_2;
	widget[62] = &lv_example_img_4;
	widget[63] = &lv_example_img_1;
	widget[64] = &lv_example_btnmatrix_1;
	widget[65] = &lv_example_btnmatrix_2;
	widget[66] = &lv_example_btnmatrix_3;
	widget[67] = &lv_example_keyboard_1;
	widget[68] = &lv_example_dropdown_3;
	widget[69] = &lv_example_dropdown_2;
	widget[70] = &lv_example_dropdown_1;
	widget[71] = &lv_example_win_1;
	widget[72] = &lv_example_canvas_1;
	widget[73] = &lv_example_canvas_2;
	widget[74] = &lv_example_imgbtn_1;
	widget[75] = &lv_example_obj_2;
	widget[76] = &lv_example_obj_1;
	widget[77] = &lv_example_line_1;
	widget[78] = &lv_example_animimg_1;
	
	//get started;
	get_started[0] = &lv_example_get_started_1;
	get_started[1] = &lv_example_get_started_3;
	get_started[2] = &lv_example_get_started_2;

	
	int i=atoi(argv[2]);

	if(strcmp(argv[1],"event") == 0)
	{
		i=i%4;
		printf("Running Event Example %d \n", i);
		event[i]();
	}

	if(strcmp(argv[1],"anim") == 0)
	{
		i=i%4;
		printf("Running Anim Example %d \n", i);
		anim[i]();
	}

	if(strcmp(argv[1],"style") == 0)
	{
		i=i%15;
		printf("Running Style Example %d \n", i);
		style[i]();
	}

	if(strcmp(argv[1],"other") == 0)
	{
		i=i%16;
		printf("Running \"Other\" Example %d \n", i);
		other[i]();
	}

	if(strcmp(argv[1],"scroll") == 0)
	{
		i=i%6;
		printf("Running Scroll Example %d \n", i);
		scroll[i]();
	}

	if(strcmp(argv[1],"layout") == 0)
	{
		i=i%12;
		printf("Running Layout Example %d \n", i);
		layout[i]();
	}

	if(strcmp(argv[1],"widget") == 0)
	{
		i=i%79;
		printf("Running Widget Example %d \n", i);
		widget[i]();
	}

	if(strcmp(argv[1],"get-started") == 0)
	{
		i=i%3;
		printf("Running Get Started Example %d \n", i);
		get_started[i]();
	}

    /*Handle LitlevGL tasks (tickless mode)*/
    while(1) 
    {
        lv_timer_handler();
        usleep(5000);
    }

    return 0;
}

