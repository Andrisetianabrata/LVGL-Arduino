#ifndef _MY_UI_UI_H
#define _MY_UI_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined __has_include
#if __has_include("lvgl.h")
#include "lvgl.h"
#elif __has_include("lvgl/lvgl.h")
#include "lvgl/lvgl.h"
#else
#include "lvgl.h"
#endif
#else
#include "lvgl.h"
#endif

LV_IMG_DECLARE(ui_img_1671879197);    // assets\signal_wifi_4_bar@0.8x.png
LV_IMG_DECLARE(ui_img_calendar_png);    // assets\calendar.png
LV_IMG_DECLARE(ui_img_masjid_png);    // assets\masjid.png
LV_IMG_DECLARE(ui_img_info_png);    // assets\info.png
LV_IMG_DECLARE(ui_img_clock_png);    // assets\clock.png
LV_IMG_DECLARE(ui_img_seting_png);    // assets\seting.png
LV_IMG_DECLARE(ui_img_hujan_png);    // assets\hujan.png
LV_IMG_DECLARE(ui_img_berawan_png);    // assets\berawan.png
LV_IMG_DECLARE(ui_img_cerah_png);    // assets\cerah.png
LV_IMG_DECLARE(ui_img_1348540801);    // assets\01d@2x.png
LV_IMG_DECLARE(ui_img_1354468288);    // assets\02d@2x.png
LV_IMG_DECLARE(ui_img_1355516799);    // assets\03d@2x.png
LV_IMG_DECLARE(ui_img_1352235966);    // assets\04d@2x.png
LV_IMG_DECLARE(ui_img_1358010745);    // assets\09d@2x.png
LV_IMG_DECLARE(ui_img_116869539);    // assets\10d@2x.png
LV_IMG_DECLARE(ui_img_115821028);    // assets\11d@2x.png


LV_FONT_DECLARE(ui_font_Font1);
LV_FONT_DECLARE(ui_font_Font2);

void ui_Seting_screen_init(void);
void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
