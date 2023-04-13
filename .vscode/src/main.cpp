#include <Arduino.h>
#define LGFX_USE_V1
#include "ui.h"
#include <LovyanGFX.hpp>
#include <WiFi.h>
#include <HTTPClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>
#include <ctime>
#include <cctype>
#include <cstring>

lv_obj_t *ui_Home;
lv_obj_t *ui_smallClockLabel;
lv_obj_t *ui_wifi;
lv_obj_t *ui_Kalendar;
lv_obj_t *ui_Screen1_Image1;
lv_obj_t *ui_KalendarLabel;
lv_obj_t *ui_bigClockLabel;
lv_obj_t *ui_JadwalSholat;
lv_obj_t *ui_Screen1_Image2;
lv_obj_t *ui_Info;
lv_obj_t *ui_Screen1_Image3;
lv_obj_t *ui_Jam;
lv_obj_t *ui_Screen1_Image4;
lv_obj_t *ui_Pengaturan;
lv_obj_t *ui_Screen1_Image5;
lv_obj_t *ui_Weather;
lv_obj_t *ui_berawanBg;
lv_obj_t *ui_cerahBg;
lv_obj_t *ui_hujanBg;
lv_obj_t *ui_d01;
lv_obj_t *ui_d02;
lv_obj_t *ui_d03;
lv_obj_t *ui_d04;
lv_obj_t *ui_d09;
lv_obj_t *ui_d10;
lv_obj_t *ui_d11;
lv_obj_t *ui_Home_Label13;
lv_obj_t *ui_Home_Label14;
lv_obj_t *ui_Home_Label1;
lv_obj_t *ui_Kalender;
lv_obj_t *ui_calendar;
lv_obj_t *ui_kalenderBck;
lv_obj_t *ui_kalendarBckLbl;
lv_obj_t *ui_Jadwal;
lv_obj_t *ui_jadwalSholatBck;
lv_obj_t *ui_bckLblJadwal;
lv_obj_t *ui_Screen3_Label5;
lv_obj_t *ui_Seting;
lv_obj_t *ui_jam;
lv_obj_t *ui_jamBckBtn;
lv_obj_t *ui_jamBckLbl;
lv_obj_t *ui_panelClock;
lv_obj_t *ui_labelClock;
lv_obj_t *ui_panelMinute;
lv_obj_t *ui_labelMinute;
lv_obj_t *ui_panelSecond;
lv_obj_t *ui_labelSecond;
lv_obj_t *ui_jam_Image2;
lv_obj_t *ui_panelCalendar;
lv_obj_t *ui_labelSecond1;
lv_obj_t *ui_info;
lv_obj_t *ui_infoBckBtn;
lv_obj_t *ui_infoBckLbl;
lv_obj_t *ui_info_Label12;
lv_obj_t *root_page;
lv_obj_t *kb;
lv_obj_t *menu;
lv_disp_t *dispp;
class LGFX : public lgfx::LGFX_Device
{
  lgfx::Panel_ILI9488 _panel_instance;
  lgfx::Bus_SPI _bus_instance;
  lgfx::Light_PWM _light_instance;
  lgfx::Touch_XPT2046 _touch_instance;

public:
  LGFX(void)
  {
    {                                    // バス制御の設定を行います。
      auto cfg = _bus_instance.config(); // バス設定用の構造体を取得します。

      // SPIバスの設定
      // cfg.spi_host = VSPI_HOST;     // 使用するSPIを選択  ESP32-S2,C3 : SPI2_HOST or SPI3_HOST / ESP32 : VSPI_HOST or HSPI_HOST
      // ※ ESP-IDFバージョンアップに伴い、VSPI_HOST , HSPI_HOSTの記述は非推奨になるため、エラーが出る場合は代わりにSPI2_HOST , SPI3_HOSTを使用してください。
      cfg.spi_mode = 0;          // SPI通信モードを設定 (0 ~ 3)
      cfg.freq_write = 50000000; // 送信時のSPIクロック (最大80MHz, 80MHzを整数で割った値に丸められます)
      cfg.freq_read = 16000000;  // 受信時のSPIクロック
      cfg.spi_3wire = false;     // 受信をMOSIピンで行う場合はtrueを設定
      cfg.use_lock = true;       // トランザクションロックを使用する場合はtrueを設定
      cfg.dma_channel = 1;       // 使用するDMAチャンネルを設定 (0=DMA不使用 / 1=1ch / 2=ch / SPI_DMA_CH_AUTO=自動設定)
      // ※ ESP-IDFバージョンアップに伴い、DMAチャンネルはSPI_DMA_CH_AUTO(自動設定)が推奨になりました。1ch,2chの指定は非推奨になります。
      cfg.pin_sclk = 18;                      // SPIのSCLKピン番号を設定
      cfg.pin_mosi = 23;                      // SPIのMOSIピン番号を設定
      cfg.pin_miso = 19;                      // SPIのMISOピン番号を設定 (-1 = disable)
      cfg.pin_dc = 16;                        // SPIのD/Cピン番号を設定  (-1 = disable)
      _bus_instance.config(cfg);              // 設定値をバスに反映します。
      _panel_instance.setBus(&_bus_instance); // バスをパネルにセットします。
    }

    {                                      // 表示パネル制御の設定を行います。
      auto cfg = _panel_instance.config(); // 表示パネル設定用の構造体を取得します。

      cfg.pin_cs = 5;    // CSが接続されているピン番号   (-1 = disable)
      cfg.pin_rst = 17;  // RSTが接続されているピン番号  (-1 = disable)
      cfg.pin_busy = -1; // BUSYが接続されているピン番号 (-1 = disable)

      // ※ 以下の設定値はパネル毎に一般的な初期値が設定されていますので、不明な項目はコメントアウトして試してみてください。

      cfg.panel_width = 320;    // 実際に表示可能な幅
      cfg.panel_height = 480;   // 実際に表示可能な高さ
      cfg.offset_x = 0;         // パネルのX方向オフセット量
      cfg.offset_y = 0;         // パネルのY方向オフセット量
      cfg.offset_rotation = 0;  // 回転方向の値のオフセット 0~7 (4~7は上下反転)
      cfg.dummy_read_pixel = 8; // ピクセル読出し前のダミーリードのビット数
      cfg.dummy_read_bits = 1;  // ピクセル以外のデータ読出し前のダミーリードのビット数
      cfg.readable = true;      // データ読出しが可能な場合 trueに設定
      cfg.invert = false;       // パネルの明暗が反転してしまう場合 trueに設定
      cfg.rgb_order = false;    // パネルの赤と青が入れ替わってしまう場合 trueに設定
      cfg.dlen_16bit = false;   // 16bitパラレルやSPIでデータ長を16bit単位で送信するパネルの場合 trueに設定
      cfg.bus_shared = true;    // SDカードとバスを共有している場合 trueに設定(drawJpgFile等でバス制御を行います)

      // 以下はST7735やILI9163のようにピクセル数が可変のドライバで表示がずれる場合にのみ設定してください。
      //    cfg.memory_width     =   240;  // ドライバICがサポートしている最大の幅
      //    cfg.memory_height    =   320;  // ドライバICがサポートしている最大の高さ

      _panel_instance.config(cfg);
    }

    //*
    {                                      // バックライト制御の設定を行います。（必要なければ削除）
      auto cfg = _light_instance.config(); // バックライト設定用の構造体を取得します。

      cfg.pin_bl = 21;     // バックライトが接続されているピン番号
      cfg.invert = false;  // バックライトの輝度を反転させる場合 true
      cfg.freq = 44100;    // バックライトのPWM周波数
      cfg.pwm_channel = 7; // 使用するPWMのチャンネル番号

      _light_instance.config(cfg);
      _panel_instance.setLight(&_light_instance); // バックライトをパネルにセットします。
    }
    //*/

    //*
    { // タッチスクリーン制御の設定を行います。（必要なければ削除）
      auto cfg = _touch_instance.config();

      cfg.x_min = 0;           // タッチスクリーンから得られる最小のX値(生の値)
      cfg.x_max = 319;         // タッチスクリーンから得られる最大のX値(生の値)
      cfg.y_min = 0;           // タッチスクリーンから得られる最小のY値(生の値)
      cfg.y_max = 479;         // タッチスクリーンから得られる最大のY値(生の値)
      cfg.pin_int = -1;        // INTが接続されているピン番号
      cfg.bus_shared = true;   // 画面と共通のバスを使用している場合 trueを設定
      cfg.offset_rotation = 0; // 表示とタッチの向きのが一致しない場合の調整 0~7の値で設定

      // SPI接続の場合
      cfg.spi_host = VSPI_HOST; // 使用するSPIを選択 (HSPI_HOST or VSPI_HOST)
      cfg.freq = 1000000;       // SPIクロックを設定
      cfg.pin_sclk = 18;        // SCLKが接続されているピン番号
      cfg.pin_mosi = 23;        // MOSIが接続されているピン番号
      cfg.pin_miso = 19;        // MISOが接続されているピン番号
      cfg.pin_cs = 22;          //   CSが接続されているピン番号
      _touch_instance.config(cfg);
      _panel_instance.setTouch(&_touch_instance); // タッチスクリーンをパネルにセットします。
    }
    //*/

    setPanel(&_panel_instance); // 使用するパネルをセットします。
  }
};

#define LV_USE_LOG 0
#if LV_USE_LOG != 0
void my_print(const char *buf)
{
  Serial.printf(buf);
  Serial.flush();
}
#endif

LGFX tft;

/*Change to your screen resolution*/
static const uint16_t screenWidth = 480;
static const uint16_t screenHeight = 320;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * 10];
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  // tft.pushColors( ( uint16_t * )&color_p->full, w * h, true );
  tft.writePixels((lgfx::rgb565_t *)&color_p->full, w * h);
  tft.endWrite();

  lv_disp_flush_ready(disp);
}
void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
  uint16_t touchX, touchY;

  bool touched = tft.getTouch(&touchX, &touchY);

  if (!touched)
  {
    data->state = LV_INDEV_STATE_REL;
  }
  else
  {
    data->state = LV_INDEV_STATE_PR;

    /*Set the coordinates*/
    data->point.x = touchX;
    data->point.y = touchY;
  }
}

const char *ssid = "TP-Link_92D6";
const char *pass = "00000000";
time_t now;
String kota = "Purwakarta";
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", (long)(7 * 3600), 60000);
struct tm *localTime;
int jam;               // Jam (0-23)
int menit;             // Menit (0-59)
int detik;             // Detik (0-59)
int hariDalamSeminggu; // Hari dalam seminggu (0-6)
int tanggal;           // Tanggal (1-31)
int bulan;             // Bulan (1-12)
int tahun;             // Tahun
static const char *daysOfWeek[] = {"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu"};
float lat_cord_pray, lng_cord_pray, lat_cord_wthr, lng_cord_wthr;

typedef enum
{
  LV_MENU_ITEM_BUILDER_VARIANT_1,
  LV_MENU_ITEM_BUILDER_VARIANT_2,
  LV_MENU_ITEM_BUILDER_VARIANT_3
}lv_menu_builder_variant_t;
 
static void ta_event_cb(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *ta = lv_event_get_target(e);
  lv_obj_t *kb = (lv_obj_t *)lv_event_get_user_data(e);
  if (code == LV_EVENT_FOCUSED)
  {
    lv_keyboard_set_textarea(kb, ta);
    lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);
  }

  if (code == LV_EVENT_DEFOCUSED)
  {
    lv_keyboard_set_textarea(kb, NULL);
    lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
    if (std::strcmp(lv_textarea_get_placeholder_text(ta), "SSID") == 0)
    {
      ssid = lv_textarea_get_text(ta);
    }
    else if (std::strcmp(lv_textarea_get_placeholder_text(ta), "PASS") == 0)
    {
      pass = lv_textarea_get_text(ta);
    }
  }
}
static void ta_event_cb_cord_pray(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *ta = lv_event_get_target(e);
  lv_obj_t *kb = (lv_obj_t *)lv_event_get_user_data(e);
  if (code == LV_EVENT_FOCUSED)
  {
    lv_keyboard_set_textarea(kb, ta);
    lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);
  }

  if (code == LV_EVENT_DEFOCUSED)
  {
    lv_keyboard_set_textarea(kb, NULL);
    lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
    if (std::strcmp(lv_textarea_get_placeholder_text(ta), "LATITUDE") == 0)
    {
      lat_cord_pray = atof(lv_textarea_get_text(ta));
    }
    else if (std::strcmp(lv_textarea_get_placeholder_text(ta), "LONGITUDE") == 0)
    {
      lng_cord_pray = atof(lv_textarea_get_text(ta));
    }
  }
}
static void ta_event_cb_cord_wthr(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *ta = lv_event_get_target(e);
  lv_obj_t *kb = (lv_obj_t *)lv_event_get_user_data(e);
  if (code == LV_EVENT_FOCUSED)
  {
    lv_keyboard_set_textarea(kb, ta);
    lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);
  }

  if (code == LV_EVENT_DEFOCUSED)
  {
    lv_keyboard_set_textarea(kb, NULL);
    lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
    if (std::strcmp(lv_textarea_get_placeholder_text(ta), "LATITUDE") == 0)
    {
      lat_cord_wthr = atof(lv_textarea_get_text(ta));
    }
    else if (std::strcmp(lv_textarea_get_placeholder_text(ta), "LONGITUDE") == 0)
    {
      lng_cord_wthr = atof(lv_textarea_get_text(ta));
    }
  }
}
static void back_event_handler(lv_event_t *e)
{
  lv_obj_t *obj = lv_event_get_target(e);
  lv_obj_t *menu = (lv_obj_t *)lv_event_get_user_data(e);

  if (lv_menu_back_btn_is_root(menu, obj))
  {
    // lv_obj_t *mbox1 = lv_msgbox_create(NULL, "Hello", "Root back btn click.", NULL, true);
    // lv_obj_center(mbox1);
    lv_scr_load_anim(ui_Home, LV_SCR_LOAD_ANIM_FADE_ON, 0, 0, false);
  }
}
static void switch_handler(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *menu = (lv_obj_t *)lv_event_get_user_data(e);
  lv_obj_t *obj = lv_event_get_target(e);
  if (code == LV_EVENT_VALUE_CHANGED)
  {
    if (lv_obj_has_state(obj, LV_STATE_CHECKED))
    {
      lv_menu_set_page(menu, NULL);
      lv_menu_set_sidebar_page(menu, root_page);
      lv_event_send(lv_obj_get_child(lv_obj_get_child(lv_menu_get_cur_sidebar_page(menu), 0), 0), LV_EVENT_CLICKED, NULL);
    }
    else
    {
      lv_menu_set_sidebar_page(menu, NULL);
      lv_menu_clear_history(menu); /* Clear history because we will be showing the root page later */
      lv_menu_set_page(menu, root_page);
    }
  }
}
static void switch_connect_to_wifi(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *menu = (lv_obj_t *)lv_event_get_user_data(e);
  lv_obj_t *obj = lv_event_get_target(e);
  if (code == LV_EVENT_VALUE_CHANGED)
  {
    if (lv_obj_has_state(obj, LV_STATE_CHECKED))
    {
      WiFi.begin(ssid, pass);
      Serial.printf("SSID : %s\n", ssid);
      Serial.printf("PASS : %s\n", pass);
    }
    else
    {
      WiFi.disconnect();
    }
  }
}
static void slider_handler(lv_event_t *e)
{
  lv_obj_t *slider = lv_event_get_target(e);
  Serial.printf("%d\n", (int8_t)lv_slider_get_value(slider));
  tft.setBrightness((int8_t)lv_slider_get_value(slider));
}

static lv_obj_t *create_text(lv_obj_t *parent, const char *icon, const char *txt, lv_menu_builder_variant_t builder_variant)
{
  lv_obj_t *obj = lv_menu_cont_create(parent);

  lv_obj_t *img = NULL;
  lv_obj_t *label = NULL;

  if (icon)
  {
    img = lv_img_create(obj);
    lv_img_set_src(img, icon);
  }

  if (txt)
  {
    label = lv_label_create(obj);
    lv_label_set_text(label, txt);
    lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_obj_set_flex_grow(label, 1);
  }

  if (builder_variant == LV_MENU_ITEM_BUILDER_VARIANT_2 && icon && txt)
  {
    lv_obj_add_flag(img, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK);
    lv_obj_swap(img, label);
  }

  return obj;
}
static lv_obj_t *create_slider(lv_obj_t *parent, const char *icon, const char *txt, int32_t min, int32_t max, int32_t val)
{
  lv_obj_t *obj = create_text(parent, icon, txt, LV_MENU_ITEM_BUILDER_VARIANT_2);

  lv_obj_t *slider = lv_slider_create(obj);
  lv_obj_set_flex_grow(slider, 1);
  lv_slider_set_range(slider, min, max);
  lv_slider_set_value(slider, val, LV_ANIM_OFF);

  if (icon == NULL)
  {
    lv_obj_add_flag(slider, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK);
  }

  return obj;
}
static lv_obj_t *create_switch(lv_obj_t *parent, const char *icon, const char *txt, bool chk)
{
  lv_obj_t *obj = create_text(parent, icon, txt, LV_MENU_ITEM_BUILDER_VARIANT_1);

  lv_obj_t *sw = lv_switch_create(obj);
  lv_obj_add_state(sw, chk ? LV_STATE_CHECKED : 0);

  return obj;
}
static lv_obj_t *create_btn(lv_obj_t *parent, const char *icon, const char *txt)
{
  lv_obj_t *obj = lv_menu_cont_create(parent);

  lv_obj_t *btn = lv_btn_create(obj);
  lv_obj_t *label = lv_label_create(btn);
  lv_obj_set_width(btn, 100);
  lv_obj_set_height(btn, 38);
  // lv_obj_set_x(btn, lv_pct(50));
  lv_obj_set_style_radius(btn, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_label_set_text(label, txt);
  lv_obj_set_align(label, LV_ALIGN_CENTER);
  lv_obj_set_align(btn, LV_ALIGN_CENTER);

  return obj;
}
static lv_obj_t *create_txt_area(lv_obj_t *parent, const char *txt, bool pass_mode)
{
  lv_obj_t *obj = lv_menu_cont_create(parent);

  lv_obj_t *ta = lv_textarea_create(obj);
  lv_obj_set_height(ta, 40);
  lv_obj_set_width(ta, lv_pct(100));
  lv_textarea_set_placeholder_text(ta, txt);
  lv_textarea_set_password_mode(ta, pass_mode);
  lv_textarea_set_one_line(ta, true);
  return obj;
}

void ui_event_JadwalSholat(lv_event_t *e)
{
  lv_event_code_t event_code = lv_event_get_code(e);
  lv_obj_t *target = lv_event_get_target(e);
  if (event_code == LV_EVENT_CLICKED)
  {
    // _ui_screen_change(ui_Jadwal, LV_SCR_LOAD_ANIM_FADE_ON, 0, 0);
    lv_scr_load_anim(ui_Jadwal, LV_SCR_LOAD_ANIM_FADE_ON, 0, 0, false);
  }
}
void ui_event_Info(lv_event_t *e)
{
  lv_event_code_t event_code = lv_event_get_code(e);
  lv_obj_t *target = lv_event_get_target(e);
  if (event_code == LV_EVENT_CLICKED)
  {
    // _ui_screen_change(ui_info, LV_SCR_LOAD_ANIM_FADE_ON, 0, 0);
    lv_scr_load_anim(ui_info, LV_SCR_LOAD_ANIM_FADE_ON, 0, 0, false);
  }
}
void ui_event_Jam(lv_event_t *e)
{
  lv_event_code_t event_code = lv_event_get_code(e);
  lv_obj_t *target = lv_event_get_target(e);
  if (event_code == LV_EVENT_CLICKED)
  {
    // _ui_screen_change(ui_jam, LV_SCR_LOAD_ANIM_FADE_ON, 0, 0);
    lv_scr_load_anim(ui_jam, LV_SCR_LOAD_ANIM_FADE_ON, 0, 0, false);
  }
}
void ui_event_Pengaturan(lv_event_t *e)
{
  lv_event_code_t event_code = lv_event_get_code(e);
  lv_obj_t *target = lv_event_get_target(e);
  if (event_code == LV_EVENT_CLICKED)
  {
    // _ui_screen_change(ui_Seting, LV_SCR_LOAD_ANIM_FADE_ON, 0, 0);
    lv_scr_load_anim(ui_Seting, LV_SCR_LOAD_ANIM_FADE_ON, 0, 0, false);
  }
}
void backToHome(lv_event_t *e)
{
  lv_event_code_t event_code = lv_event_get_code(e);
  lv_obj_t *target = lv_event_get_target(e);
  if (event_code == LV_EVENT_CLICKED)
  {
    // _ui_screen_change(ui_Home, LV_SCR_LOAD_ANIM_OVER_RIGHT, 0, 0);
    lv_scr_load_anim(ui_Home, LV_SCR_LOAD_ANIM_FADE_ON, 0, 0, false);
  }
}
void ui_event_Kalendar(lv_event_t *e)
{
  lv_event_code_t event_code = lv_event_get_code(e);
  lv_obj_t *target = lv_event_get_target(e);
  static lv_calendar_date_t highlighted_days[1];
  highlighted_days[0].year = tahun;
  highlighted_days[0].month = bulan;
  highlighted_days[0].day = tanggal;
  if (event_code == LV_EVENT_CLICKED)
  {
    lv_scr_load_anim(ui_Kalender, LV_SCR_LOAD_ANIM_OVER_LEFT, 0, 0, false);
    lv_calendar_set_today_date(ui_calendar, tahun, bulan, tanggal);
    lv_calendar_set_showed_date(ui_calendar, tahun, bulan);
    lv_calendar_set_highlighted_dates(ui_calendar, highlighted_days, 1);
  }
}
void calendar_event(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *obj = lv_event_get_current_target(e);

  if (code == LV_EVENT_VALUE_CHANGED)
  {
    lv_calendar_date_t date;
    if (lv_calendar_get_pressed_date(obj, &date))
    {
      LV_LOG_USER("Clicked date: %02d.%02d.%d", date.day, date.month, date.year);
      lv_calendar_set_today_date(ui_calendar, date.year, date.month, date.day);
    }
  }
}

void ui_Home_screen_init(void)
{
  ui_Home = lv_obj_create(NULL);
  lv_obj_clear_flag(ui_Home, LV_OBJ_FLAG_SCROLLABLE); /// Flags

  ui_smallClockLabel = lv_label_create(ui_Home);
  lv_obj_set_width(ui_smallClockLabel, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(ui_smallClockLabel, LV_SIZE_CONTENT); /// 1
  lv_obj_set_x(ui_smallClockLabel, 33);
  lv_obj_set_y(ui_smallClockLabel, 13);
  lv_label_set_text(ui_smallClockLabel, "20:21");
  lv_obj_set_style_text_font(ui_smallClockLabel, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);

  ui_wifi = lv_img_create(ui_Home);
  lv_img_set_src(ui_wifi, &ui_img_1671879197);
  lv_obj_set_width(ui_wifi, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(ui_wifi, LV_SIZE_CONTENT); /// 1
  lv_obj_set_x(ui_wifi, 429);
  lv_obj_set_y(ui_wifi, 15);
  lv_obj_add_flag(ui_wifi, LV_OBJ_FLAG_ADV_HITTEST);  /// Flags
  lv_obj_clear_flag(ui_wifi, LV_OBJ_FLAG_SCROLLABLE); /// Flags
  lv_obj_set_style_img_recolor(ui_wifi, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_img_recolor_opa(ui_wifi, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

  ui_Kalendar = lv_obj_create(ui_Home);
  lv_obj_set_width(ui_Kalendar, 149);
  lv_obj_set_height(ui_Kalendar, 71);
  lv_obj_set_x(ui_Kalendar, 30);
  lv_obj_set_y(ui_Kalendar, 46);
  lv_obj_clear_flag(ui_Kalendar, LV_OBJ_FLAG_SCROLLABLE); /// Flags
  lv_obj_set_style_radius(ui_Kalendar, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(ui_Kalendar, lv_color_hex(0x04BE0C), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(ui_Kalendar, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(ui_Kalendar, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_left(ui_Kalendar, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_right(ui_Kalendar, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_top(ui_Kalendar, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_bottom(ui_Kalendar, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

  ui_Screen1_Image1 = lv_img_create(ui_Kalendar);
  lv_img_set_src(ui_Screen1_Image1, &ui_img_calendar_png);
  lv_obj_set_width(ui_Screen1_Image1, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(ui_Screen1_Image1, LV_SIZE_CONTENT); /// 1
  lv_obj_set_x(ui_Screen1_Image1, -39);
  lv_obj_set_y(ui_Screen1_Image1, 0);
  lv_obj_set_align(ui_Screen1_Image1, LV_ALIGN_CENTER);
  lv_obj_clear_flag(ui_Screen1_Image1,
                    LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SNAPPABLE |
                        LV_OBJ_FLAG_SCROLLABLE); /// Flags

  ui_KalendarLabel = lv_label_create(ui_Kalendar);
  lv_obj_set_width(ui_KalendarLabel, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(ui_KalendarLabel, LV_SIZE_CONTENT); /// 1
  lv_obj_set_x(ui_KalendarLabel, 61);
  lv_obj_set_y(ui_KalendarLabel, 11);
  lv_label_set_text(ui_KalendarLabel, "Sabtu");
  lv_obj_clear_flag(ui_KalendarLabel, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE |
                                          LV_OBJ_FLAG_SNAPPABLE); /// Flags
  lv_obj_set_style_text_font(ui_KalendarLabel, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);

  ui_bigClockLabel = lv_label_create(ui_Kalendar);
  lv_obj_set_width(ui_bigClockLabel, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(ui_bigClockLabel, LV_SIZE_CONTENT); /// 1
  lv_obj_set_x(ui_bigClockLabel, 61);
  lv_obj_set_y(ui_bigClockLabel, 31);
  lv_label_set_text(ui_bigClockLabel, "08/04");
  lv_obj_clear_flag(ui_bigClockLabel, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE |
                                          LV_OBJ_FLAG_SNAPPABLE); /// Flags
  lv_obj_set_style_text_font(ui_bigClockLabel, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

  ui_JadwalSholat = lv_obj_create(ui_Home);
  lv_obj_set_width(ui_JadwalSholat, 71);
  lv_obj_set_height(ui_JadwalSholat, 71);
  lv_obj_set_x(ui_JadwalSholat, 31);
  lv_obj_set_y(ui_JadwalSholat, 125);
  lv_obj_clear_flag(ui_JadwalSholat, LV_OBJ_FLAG_SCROLLABLE); /// Flags
  lv_obj_set_style_radius(ui_JadwalSholat, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(ui_JadwalSholat, lv_color_hex(0x359CDB), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(ui_JadwalSholat, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(ui_JadwalSholat, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

  ui_Screen1_Image2 = lv_img_create(ui_JadwalSholat);
  lv_img_set_src(ui_Screen1_Image2, &ui_img_masjid_png);
  lv_obj_set_width(ui_Screen1_Image2, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(ui_Screen1_Image2, LV_SIZE_CONTENT); /// 1
  lv_obj_set_align(ui_Screen1_Image2, LV_ALIGN_CENTER);
  lv_obj_clear_flag(ui_Screen1_Image2,
                    LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SNAPPABLE |
                        LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                        LV_OBJ_FLAG_SCROLL_CHAIN); /// Flags

  ui_Info = lv_obj_create(ui_Home);
  lv_obj_set_width(ui_Info, 71);
  lv_obj_set_height(ui_Info, 71);
  lv_obj_set_x(ui_Info, 109);
  lv_obj_set_y(ui_Info, 125);
  lv_obj_clear_flag(ui_Info, LV_OBJ_FLAG_SCROLLABLE); /// Flags
  lv_obj_set_style_radius(ui_Info, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(ui_Info, lv_color_hex(0xEDAF09), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(ui_Info, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(ui_Info, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

  ui_Screen1_Image3 = lv_img_create(ui_Info);
  lv_img_set_src(ui_Screen1_Image3, &ui_img_info_png);
  lv_obj_set_width(ui_Screen1_Image3, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(ui_Screen1_Image3, LV_SIZE_CONTENT); /// 1
  lv_obj_set_align(ui_Screen1_Image3, LV_ALIGN_CENTER);
  lv_obj_clear_flag(ui_Screen1_Image3,
                    LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SNAPPABLE |
                        LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                        LV_OBJ_FLAG_SCROLL_CHAIN); /// Flags

  ui_Jam = lv_obj_create(ui_Home);
  lv_obj_set_width(ui_Jam, 71);
  lv_obj_set_height(ui_Jam, 71);
  lv_obj_set_x(ui_Jam, 31);
  lv_obj_set_y(ui_Jam, 204);
  lv_obj_clear_flag(ui_Jam, LV_OBJ_FLAG_SCROLLABLE); /// Flags
  lv_obj_set_style_radius(ui_Jam, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(ui_Jam, lv_color_hex(0x4F4477), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(ui_Jam, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(ui_Jam, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

  ui_Screen1_Image4 = lv_img_create(ui_Jam);
  lv_img_set_src(ui_Screen1_Image4, &ui_img_clock_png);
  lv_obj_set_width(ui_Screen1_Image4, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(ui_Screen1_Image4, LV_SIZE_CONTENT); /// 1
  lv_obj_set_align(ui_Screen1_Image4, LV_ALIGN_CENTER);
  lv_obj_clear_flag(ui_Screen1_Image4,
                    LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SNAPPABLE |
                        LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                        LV_OBJ_FLAG_SCROLL_CHAIN); /// Flags

  ui_Pengaturan = lv_obj_create(ui_Home);
  lv_obj_set_width(ui_Pengaturan, 71);
  lv_obj_set_height(ui_Pengaturan, 71);
  lv_obj_set_x(ui_Pengaturan, 109);
  lv_obj_set_y(ui_Pengaturan, 204);
  lv_obj_clear_flag(ui_Pengaturan, LV_OBJ_FLAG_SCROLLABLE); /// Flags
  lv_obj_set_style_radius(ui_Pengaturan, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(ui_Pengaturan, lv_color_hex(0x20B57F), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(ui_Pengaturan, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(ui_Pengaturan, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

  ui_Screen1_Image5 = lv_img_create(ui_Pengaturan);
  lv_img_set_src(ui_Screen1_Image5, &ui_img_seting_png);
  lv_obj_set_width(ui_Screen1_Image5, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(ui_Screen1_Image5, LV_SIZE_CONTENT); /// 1
  lv_obj_set_align(ui_Screen1_Image5, LV_ALIGN_CENTER);
  lv_obj_clear_flag(ui_Screen1_Image5,
                    LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SNAPPABLE |
                        LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                        LV_OBJ_FLAG_SCROLL_CHAIN); /// Flags

  ui_Weather = lv_obj_create(ui_Home);
  lv_obj_set_width(ui_Weather, 261);
  lv_obj_set_height(ui_Weather, 229);
  lv_obj_set_x(ui_Weather, 188);
  lv_obj_set_y(ui_Weather, 46);
  lv_obj_clear_flag(ui_Weather, LV_OBJ_FLAG_SCROLLABLE); /// Flags
  lv_obj_set_style_radius(ui_Weather, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(ui_Weather, lv_color_hex(0x359CFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(ui_Weather, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_img_tiled(ui_Weather, true, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(ui_Weather, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_left(ui_Weather, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_right(ui_Weather, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_top(ui_Weather, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_bottom(ui_Weather, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

  ui_berawanBg = lv_img_create(ui_Weather);
  lv_img_set_src(ui_berawanBg, &ui_img_berawan_png);
  lv_obj_set_width(ui_berawanBg, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(ui_berawanBg, LV_SIZE_CONTENT); /// 1
  lv_obj_set_align(ui_berawanBg, LV_ALIGN_CENTER);
  lv_obj_add_flag(ui_berawanBg, LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST); /// Flags
  lv_obj_clear_flag(ui_berawanBg, LV_OBJ_FLAG_SCROLLABLE);                     /// Flags

  ui_cerahBg = lv_img_create(ui_Weather);
  lv_img_set_src(ui_cerahBg, &ui_img_cerah_png);
  lv_obj_set_width(ui_cerahBg, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(ui_cerahBg, LV_SIZE_CONTENT); /// 1
  lv_obj_set_align(ui_cerahBg, LV_ALIGN_CENTER);
  lv_obj_add_flag(ui_cerahBg, LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST); /// Flags
  lv_obj_clear_flag(ui_cerahBg, LV_OBJ_FLAG_SCROLLABLE);                     /// Flags

  ui_hujanBg = lv_img_create(ui_Weather);
  lv_img_set_src(ui_hujanBg, &ui_img_hujan_png);
  lv_obj_set_width(ui_hujanBg, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(ui_hujanBg, LV_SIZE_CONTENT); /// 1
  lv_obj_set_align(ui_hujanBg, LV_ALIGN_CENTER);
  lv_obj_add_flag(ui_hujanBg, LV_OBJ_FLAG_ADV_HITTEST);  /// Flags
  lv_obj_clear_flag(ui_hujanBg, LV_OBJ_FLAG_SCROLLABLE); /// Flags

  ui_d01 = lv_img_create(ui_Weather);
  lv_img_set_src(ui_d01, &ui_img_1348540801);
  lv_obj_set_width(ui_d01, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(ui_d01, LV_SIZE_CONTENT); /// 1
  lv_obj_set_x(ui_d01, 0);
  lv_obj_set_y(ui_d01, 45);
  lv_obj_add_flag(ui_d01, LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST); /// Flags
  lv_obj_clear_flag(ui_d01, LV_OBJ_FLAG_SCROLLABLE);                     /// Flags

  ui_d02 = lv_img_create(ui_Weather);
  lv_img_set_src(ui_d02, &ui_img_1354468288);
  lv_obj_set_width(ui_d02, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(ui_d02, LV_SIZE_CONTENT); /// 1
  lv_obj_set_x(ui_d02, 0);
  lv_obj_set_y(ui_d02, 45);
  lv_obj_add_flag(ui_d02, LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST); /// Flags
  lv_obj_clear_flag(ui_d02, LV_OBJ_FLAG_SCROLLABLE);                     /// Flags

  ui_d03 = lv_img_create(ui_Weather);
  lv_img_set_src(ui_d03, &ui_img_1355516799);
  lv_obj_set_width(ui_d03, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(ui_d03, LV_SIZE_CONTENT); /// 1
  lv_obj_set_x(ui_d03, 0);
  lv_obj_set_y(ui_d03, 45);
  lv_obj_add_flag(ui_d03, LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST); /// Flags
  lv_obj_clear_flag(ui_d03, LV_OBJ_FLAG_SCROLLABLE);                     /// Flags

  ui_d04 = lv_img_create(ui_Weather);
  lv_img_set_src(ui_d04, &ui_img_1352235966);
  lv_obj_set_width(ui_d04, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(ui_d04, LV_SIZE_CONTENT); /// 1
  lv_obj_set_x(ui_d04, 0);
  lv_obj_set_y(ui_d04, 45);
  lv_obj_add_flag(ui_d04, LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST); /// Flags
  lv_obj_clear_flag(ui_d04, LV_OBJ_FLAG_SCROLLABLE);                     /// Flags

  ui_d09 = lv_img_create(ui_Weather);
  lv_img_set_src(ui_d09, &ui_img_1358010745);
  lv_obj_set_width(ui_d09, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(ui_d09, LV_SIZE_CONTENT); /// 1
  lv_obj_set_x(ui_d09, 0);
  lv_obj_set_y(ui_d09, 45);
  lv_obj_add_flag(ui_d09, LV_OBJ_FLAG_ADV_HITTEST);  /// Flags
  lv_obj_clear_flag(ui_d09, LV_OBJ_FLAG_SCROLLABLE); /// Flags

  ui_d10 = lv_img_create(ui_Weather);
  lv_img_set_src(ui_d10, &ui_img_116869539);
  lv_obj_set_width(ui_d10, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(ui_d10, LV_SIZE_CONTENT); /// 1
  lv_obj_set_x(ui_d10, 0);
  lv_obj_set_y(ui_d10, 45);
  lv_obj_add_flag(ui_d10, LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST); /// Flags
  lv_obj_clear_flag(ui_d10, LV_OBJ_FLAG_SCROLLABLE);                     /// Flags

  ui_d11 = lv_img_create(ui_Weather);
  lv_img_set_src(ui_d11, &ui_img_115821028);
  lv_obj_set_width(ui_d11, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(ui_d11, LV_SIZE_CONTENT); /// 1
  lv_obj_set_x(ui_d11, 0);
  lv_obj_set_y(ui_d11, 45);
  lv_obj_add_flag(ui_d11, LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST); /// Flags
  lv_obj_clear_flag(ui_d11, LV_OBJ_FLAG_SCROLLABLE);                     /// Flags

  ui_Home_Label13 = lv_label_create(ui_Weather);
  lv_obj_set_width(ui_Home_Label13, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(ui_Home_Label13, LV_SIZE_CONTENT); /// 1
  lv_obj_set_x(ui_Home_Label13, 20);
  lv_obj_set_y(ui_Home_Label13, 131);
  lv_label_set_text(ui_Home_Label13, "Purwakarta | 30C");
  lv_obj_set_style_text_font(ui_Home_Label13, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);

  ui_Home_Label14 = lv_label_create(ui_Weather);
  lv_obj_set_width(ui_Home_Label14, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(ui_Home_Label14, LV_SIZE_CONTENT); /// 1
  lv_obj_set_x(ui_Home_Label14, 20);
  lv_obj_set_y(ui_Home_Label14, 151);
  lv_label_set_text(ui_Home_Label14, "Hujan");
  lv_obj_set_style_text_font(ui_Home_Label14, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);

  ui_Home_Label1 = lv_label_create(ui_Weather);
  lv_obj_set_width(ui_Home_Label1, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(ui_Home_Label1, LV_SIZE_CONTENT); /// 1
  lv_obj_set_x(ui_Home_Label1, 20);
  lv_obj_set_y(ui_Home_Label1, 165);
  lv_label_set_text(ui_Home_Label1, "Terasa Seperti | 29C");
  lv_obj_set_style_text_font(ui_Home_Label1, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);

  // lv_obj_set_event_cb(list_btn, event_handler);
  lv_obj_add_event_cb(ui_Kalendar, ui_event_Kalendar, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(ui_JadwalSholat, ui_event_JadwalSholat, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(ui_Info, ui_event_Info, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(ui_Jam, ui_event_Jam, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(ui_Pengaturan, ui_event_Pengaturan, LV_EVENT_ALL, NULL);
}
void ui_Kalender_screen_init(void)
{
  ui_Kalender = lv_obj_create(NULL);
  lv_obj_clear_flag(ui_Kalender, LV_OBJ_FLAG_SCROLLABLE); /// Flags

  ui_calendar = lv_calendar_create(ui_Kalender);
  lv_obj_t *ui_calendar_header = lv_calendar_header_arrow_create(ui_calendar);
  lv_obj_set_width(ui_calendar, 337);
  lv_obj_set_height(ui_calendar, 301);
  lv_obj_set_x(ui_calendar, -16);
  lv_obj_set_y(ui_calendar, 0);
  lv_obj_set_align(ui_calendar, LV_ALIGN_RIGHT_MID);

  ui_kalenderBck = lv_btn_create(ui_Kalender);
  lv_obj_set_width(ui_kalenderBck, 50);
  lv_obj_set_height(ui_kalenderBck, 50);
  lv_obj_set_x(ui_kalenderBck, 10);
  lv_obj_set_y(ui_kalenderBck, -10);
  lv_obj_set_align(ui_kalenderBck, LV_ALIGN_BOTTOM_LEFT);
  lv_obj_add_flag(ui_kalenderBck, LV_OBJ_FLAG_FLOATING | LV_OBJ_FLAG_SCROLL_ON_FOCUS); /// Flags
  lv_obj_clear_flag(ui_kalenderBck, LV_OBJ_FLAG_SCROLLABLE);                           /// Flags
  lv_obj_set_style_radius(ui_kalenderBck, 50, LV_PART_MAIN | LV_STATE_DEFAULT);

  ui_kalendarBckLbl = lv_label_create(ui_kalenderBck);
  lv_obj_set_width(ui_kalendarBckLbl, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(ui_kalendarBckLbl, LV_SIZE_CONTENT); /// 1
  lv_obj_set_align(ui_kalendarBckLbl, LV_ALIGN_CENTER);
  lv_label_set_text(ui_kalendarBckLbl, "<");

  lv_obj_add_event_cb(ui_kalenderBck, backToHome, LV_EVENT_ALL, NULL);
}
void ui_Jadwal_screen_init(void)
{
  ui_Jadwal = lv_obj_create(NULL);
  lv_obj_clear_flag(ui_Jadwal, LV_OBJ_FLAG_SCROLLABLE); /// Flags

  ui_jadwalSholatBck = lv_btn_create(ui_Jadwal);
  lv_obj_set_width(ui_jadwalSholatBck, 50);
  lv_obj_set_height(ui_jadwalSholatBck, 50);
  lv_obj_set_x(ui_jadwalSholatBck, 10);
  lv_obj_set_y(ui_jadwalSholatBck, -10);
  lv_obj_set_align(ui_jadwalSholatBck, LV_ALIGN_BOTTOM_LEFT);
  lv_obj_add_flag(ui_jadwalSholatBck, LV_OBJ_FLAG_SCROLL_ON_FOCUS); /// Flags
  lv_obj_clear_flag(ui_jadwalSholatBck, LV_OBJ_FLAG_SCROLLABLE);    /// Flags
  lv_obj_set_style_radius(ui_jadwalSholatBck, 50, LV_PART_MAIN | LV_STATE_DEFAULT);

  ui_bckLblJadwal = lv_label_create(ui_jadwalSholatBck);
  lv_obj_set_width(ui_bckLblJadwal, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(ui_bckLblJadwal, LV_SIZE_CONTENT); /// 1
  lv_obj_set_align(ui_bckLblJadwal, LV_ALIGN_CENTER);
  lv_label_set_text(ui_bckLblJadwal, "<");

  ui_Screen3_Label5 = lv_label_create(ui_Jadwal);
  lv_obj_set_width(ui_Screen3_Label5, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(ui_Screen3_Label5, LV_SIZE_CONTENT); /// 1
  lv_obj_set_align(ui_Screen3_Label5, LV_ALIGN_CENTER);
  lv_label_set_text(ui_Screen3_Label5, "Jadwal Sholat");

  lv_obj_add_event_cb(ui_jadwalSholatBck, backToHome, LV_EVENT_ALL, NULL);
}
void ui_jam_screen_init(void)
{
  ui_jam = lv_obj_create(NULL);
  lv_obj_clear_flag(ui_jam, LV_OBJ_FLAG_SCROLLABLE); /// Flags

  ui_jamBckBtn = lv_btn_create(ui_jam);
  lv_obj_set_width(ui_jamBckBtn, 50);
  lv_obj_set_height(ui_jamBckBtn, 50);
  lv_obj_set_x(ui_jamBckBtn, 10);
  lv_obj_set_y(ui_jamBckBtn, -10);
  lv_obj_set_align(ui_jamBckBtn, LV_ALIGN_BOTTOM_LEFT);
  lv_obj_add_flag(ui_jamBckBtn, LV_OBJ_FLAG_SCROLL_ON_FOCUS); /// Flags
  lv_obj_clear_flag(ui_jamBckBtn, LV_OBJ_FLAG_SCROLLABLE);    /// Flags
  lv_obj_set_style_radius(ui_jamBckBtn, 50, LV_PART_MAIN | LV_STATE_DEFAULT);

  ui_jamBckLbl = lv_label_create(ui_jamBckBtn);
  lv_obj_set_width(ui_jamBckLbl, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(ui_jamBckLbl, LV_SIZE_CONTENT); /// 1
  lv_obj_set_align(ui_jamBckLbl, LV_ALIGN_CENTER);
  lv_label_set_text(ui_jamBckLbl, "<");

  ui_panelClock = lv_obj_create(ui_jam);
  lv_obj_set_width(ui_panelClock, 90);
  lv_obj_set_height(ui_panelClock, 90);
  lv_obj_set_x(ui_panelClock, 79);
  lv_obj_set_y(ui_panelClock, 38);
  lv_obj_clear_flag(ui_panelClock, LV_OBJ_FLAG_SCROLLABLE); /// Flags
  lv_obj_set_style_bg_color(ui_panelClock, lv_color_hex(0x04BE0C), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(ui_panelClock, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(ui_panelClock, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

  ui_labelClock = lv_label_create(ui_panelClock);
  lv_obj_set_width(ui_labelClock, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(ui_labelClock, LV_SIZE_CONTENT); /// 1
  lv_obj_set_align(ui_labelClock, LV_ALIGN_CENTER);
  lv_label_set_text(ui_labelClock, "20");
  lv_obj_set_style_text_font(ui_labelClock, &lv_font_montserrat_48, LV_PART_MAIN | LV_STATE_DEFAULT);

  ui_panelMinute = lv_obj_create(ui_jam);
  lv_obj_set_width(ui_panelMinute, 90);
  lv_obj_set_height(ui_panelMinute, 90);
  lv_obj_set_x(ui_panelMinute, 190);
  lv_obj_set_y(ui_panelMinute, 38);
  lv_obj_clear_flag(ui_panelMinute, LV_OBJ_FLAG_SCROLLABLE); /// Flags
  lv_obj_set_style_bg_color(ui_panelMinute, lv_color_hex(0x04BE0C), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(ui_panelMinute, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(ui_panelMinute, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

  ui_labelMinute = lv_label_create(ui_panelMinute);
  lv_obj_set_width(ui_labelMinute, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(ui_labelMinute, LV_SIZE_CONTENT); /// 1
  lv_obj_set_align(ui_labelMinute, LV_ALIGN_CENTER);
  lv_label_set_text(ui_labelMinute, "20");
  lv_obj_set_style_text_font(ui_labelMinute, &lv_font_montserrat_48, LV_PART_MAIN | LV_STATE_DEFAULT);

  ui_panelSecond = lv_obj_create(ui_jam);
  lv_obj_set_width(ui_panelSecond, 90);
  lv_obj_set_height(ui_panelSecond, 90);
  lv_obj_set_x(ui_panelSecond, 301);
  lv_obj_set_y(ui_panelSecond, 38);
  lv_obj_clear_flag(ui_panelSecond, LV_OBJ_FLAG_SCROLLABLE); /// Flags
  lv_obj_set_style_bg_color(ui_panelSecond, lv_color_hex(0x04BE0C), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(ui_panelSecond, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(ui_panelSecond, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

  ui_labelSecond = lv_label_create(ui_panelSecond);
  lv_obj_set_width(ui_labelSecond, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(ui_labelSecond, LV_SIZE_CONTENT); /// 1
  lv_obj_set_align(ui_labelSecond, LV_ALIGN_CENTER);
  lv_label_set_text(ui_labelSecond, "20");
  lv_obj_set_style_text_font(ui_labelSecond, &lv_font_montserrat_48, LV_PART_MAIN | LV_STATE_DEFAULT);

  ui_jam_Image2 = lv_img_create(ui_panelSecond);
  lv_obj_set_width(ui_jam_Image2, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(ui_jam_Image2, LV_SIZE_CONTENT); /// 1
  lv_obj_set_align(ui_jam_Image2, LV_ALIGN_CENTER);
  lv_obj_add_flag(ui_jam_Image2, LV_OBJ_FLAG_ADV_HITTEST);  /// Flags
  lv_obj_clear_flag(ui_jam_Image2, LV_OBJ_FLAG_SCROLLABLE); /// Flags

  ui_panelCalendar = lv_obj_create(ui_jam);
  lv_obj_set_width(ui_panelCalendar, 381);
  lv_obj_set_height(ui_panelCalendar, 80);
  lv_obj_set_x(ui_panelCalendar, 49);
  lv_obj_set_y(ui_panelCalendar, 160);
  lv_obj_clear_flag(ui_panelCalendar, LV_OBJ_FLAG_SCROLLABLE); /// Flags
  lv_obj_set_style_bg_color(ui_panelCalendar, lv_color_hex(0xEDBE0C), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(ui_panelCalendar, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(ui_panelCalendar, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

  ui_labelSecond1 = lv_label_create(ui_panelCalendar);
  lv_obj_set_width(ui_labelSecond1, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(ui_labelSecond1, LV_SIZE_CONTENT); /// 1
  lv_obj_set_align(ui_labelSecond1, LV_ALIGN_CENTER);
  lv_label_set_text(ui_labelSecond1, "Minggu 20/08/2023");
  lv_obj_set_style_text_font(ui_labelSecond1, &lv_font_montserrat_36, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_event_cb(ui_jamBckBtn, backToHome, LV_EVENT_ALL, NULL);
}
void ui_info_screen_init(void)
{
  ui_info = lv_obj_create(NULL);
  lv_obj_clear_flag(ui_info, LV_OBJ_FLAG_SCROLLABLE); /// Flags

  ui_infoBckBtn = lv_btn_create(ui_info);
  lv_obj_set_width(ui_infoBckBtn, 50);
  lv_obj_set_height(ui_infoBckBtn, 50);
  lv_obj_set_x(ui_infoBckBtn, 10);
  lv_obj_set_y(ui_infoBckBtn, -10);
  lv_obj_set_align(ui_infoBckBtn, LV_ALIGN_BOTTOM_LEFT);
  lv_obj_add_flag(ui_infoBckBtn, LV_OBJ_FLAG_SCROLL_ON_FOCUS); /// Flags
  lv_obj_clear_flag(ui_infoBckBtn, LV_OBJ_FLAG_SCROLLABLE);    /// Flags
  lv_obj_set_style_radius(ui_infoBckBtn, 50, LV_PART_MAIN | LV_STATE_DEFAULT);

  ui_infoBckLbl = lv_label_create(ui_infoBckBtn);
  lv_obj_set_width(ui_infoBckLbl, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(ui_infoBckLbl, LV_SIZE_CONTENT); /// 1
  lv_obj_set_align(ui_infoBckLbl, LV_ALIGN_CENTER);
  lv_label_set_text(ui_infoBckLbl, "<");

  ui_info_Label12 = lv_label_create(ui_info);
  lv_obj_set_width(ui_info_Label12, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(ui_info_Label12, LV_SIZE_CONTENT); /// 1
  lv_obj_set_align(ui_info_Label12, LV_ALIGN_CENTER);
  lv_label_set_text(ui_info_Label12, "Info");

  lv_obj_add_event_cb(ui_infoBckBtn, backToHome, LV_EVENT_ALL, NULL);
}
void ui_Seting_screen_init(void)
{
  // lv_group_t *group = lv_group_create();
  ui_Seting = lv_obj_create(NULL);
  menu = lv_menu_create(ui_Seting);

  lv_color_t bg_color = lv_obj_get_style_bg_color(menu, 0);
  if (lv_color_brightness(bg_color) > 127)
  {
    lv_obj_set_style_bg_color(menu, lv_color_darken(lv_obj_get_style_bg_color(menu, 0), 10), 0);
  }
  else
  {
    lv_obj_set_style_bg_color(menu, lv_color_darken(lv_obj_get_style_bg_color(menu, 0), 50), 0);
  }
  lv_menu_set_mode_root_back_btn(menu, LV_MENU_ROOT_BACK_BTN_ENABLED);
  lv_obj_add_event_cb(menu, back_event_handler, LV_EVENT_CLICKED, menu);
  lv_obj_set_size(menu, lv_disp_get_hor_res(NULL), lv_disp_get_ver_res(NULL));
  lv_obj_center(menu);

  lv_obj_t *cont;
  lv_obj_t *section;

  /*Create sub pages*/
  lv_obj_t *sub_cord_praying = lv_menu_page_create(menu, NULL);
  lv_obj_set_style_pad_hor(sub_cord_praying, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
  lv_menu_separator_create(sub_cord_praying);
  section = lv_menu_section_create(sub_cord_praying);
  kb = lv_keyboard_create(sub_cord_praying);
  lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(kb, LV_OBJ_FLAG_FLOATING);
  lv_keyboard_set_mode(kb, LV_KEYBOARD_MODE_NUMBER);
  cont = create_txt_area(section, "LATITUDE", false);
  lv_obj_add_event_cb(lv_obj_get_child(cont, NULL), ta_event_cb_cord_pray, LV_EVENT_ALL, kb);
  cont = create_txt_area(section, "LONGITUDE", false);
  lv_obj_add_event_cb(lv_obj_get_child(cont, NULL), ta_event_cb_cord_pray, LV_EVENT_ALL, kb);

  lv_obj_t *sub_cord_weather = lv_menu_page_create(menu, NULL);
  lv_obj_set_style_pad_hor(sub_cord_weather, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
  lv_menu_separator_create(sub_cord_weather);
  section = lv_menu_section_create(sub_cord_weather);
  kb = lv_keyboard_create(sub_cord_weather);
  lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(kb, LV_OBJ_FLAG_FLOATING);
  lv_keyboard_set_mode(kb, LV_KEYBOARD_MODE_NUMBER);
  cont = create_txt_area(section, "LATITUDE", false);
  lv_obj_add_event_cb(lv_obj_get_child(cont, NULL), ta_event_cb_cord_wthr, LV_EVENT_ALL, kb);
  cont = create_txt_area(section, "LONGITUDE", false);
  lv_obj_add_event_cb(lv_obj_get_child(cont, NULL), ta_event_cb_cord_wthr, LV_EVENT_ALL, kb);

  lv_obj_t *sub_display_page = lv_menu_page_create(menu, NULL);
  lv_obj_set_style_pad_hor(sub_display_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
  lv_menu_separator_create(sub_display_page);
  section = lv_menu_section_create(sub_display_page);
  cont = create_slider(section, LV_SYMBOL_SETTINGS, "Brightness", 10, 255, 255);
  lv_obj_add_event_cb(lv_obj_get_child(cont, 2), slider_handler, LV_EVENT_VALUE_CHANGED, NULL);

  lv_obj_t *sub_software_info_page = lv_menu_page_create(menu, NULL);
  lv_obj_set_style_pad_hor(sub_software_info_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
  section = lv_menu_section_create(sub_software_info_page);
  create_text(section, NULL, "Version:\n  1.0", LV_MENU_ITEM_BUILDER_VARIANT_1);
  create_text(section, NULL, "Author :\n  Andri Setianabrata", LV_MENU_ITEM_BUILDER_VARIANT_1);

  lv_obj_t *sub_legal_info_page = lv_menu_page_create(menu, NULL);
  lv_obj_set_style_pad_hor(sub_legal_info_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
  section = lv_menu_section_create(sub_legal_info_page);
  for (uint32_t i = 0; i < 1; i++)
  {
    create_text(section, NULL, "NULL", LV_MENU_ITEM_BUILDER_VARIANT_1);
  }

  lv_obj_t *sub_about_page = lv_menu_page_create(menu, NULL);
  lv_obj_set_style_pad_hor(sub_about_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
  lv_menu_separator_create(sub_about_page);
  section = lv_menu_section_create(sub_about_page);
  cont = create_text(section, NULL, "Software information", LV_MENU_ITEM_BUILDER_VARIANT_1);
  lv_menu_set_load_page_event(menu, cont, sub_software_info_page);
  cont = create_text(section, NULL, "Legal information", LV_MENU_ITEM_BUILDER_VARIANT_1);
  lv_menu_set_load_page_event(menu, cont, sub_legal_info_page);

  lv_obj_t *sub_menu_mode_page = lv_menu_page_create(menu, NULL);
  lv_obj_set_style_pad_hor(sub_menu_mode_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
  lv_menu_separator_create(sub_menu_mode_page);
  section = lv_menu_section_create(sub_menu_mode_page);
  cont = create_switch(section, LV_SYMBOL_LIST, "Sidebar enable", false);
  lv_obj_add_event_cb(lv_obj_get_child(cont, 2), switch_handler, LV_EVENT_VALUE_CHANGED, menu);

  lv_obj_t *sub_wifi = lv_menu_page_create(menu, NULL);
  lv_obj_set_style_pad_hor(sub_wifi, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
  lv_menu_separator_create(sub_wifi);
  section = lv_menu_section_create(sub_wifi);
  cont = create_switch(section, LV_SYMBOL_SETTINGS, "WiFi Enable", false);
  lv_obj_add_event_cb(lv_obj_get_child(cont, 2), switch_connect_to_wifi, LV_EVENT_ALL, NULL);

  lv_obj_t *sub_wifi_page = lv_menu_page_create(menu, NULL);
  lv_obj_set_style_pad_hor(sub_wifi_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
  lv_menu_separator_create(sub_wifi_page);
  section = lv_menu_section_create(sub_wifi_page);
  kb = lv_keyboard_create(sub_wifi_page);
  lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(kb, LV_OBJ_FLAG_FLOATING);
  cont = create_txt_area(section, "SSID", false);
  lv_obj_add_event_cb(lv_obj_get_child(cont, NULL), ta_event_cb, LV_EVENT_ALL, kb);
  cont = create_txt_area(section, "PASS", true);
  lv_obj_add_event_cb(lv_obj_get_child(cont, NULL), ta_event_cb, LV_EVENT_ALL, kb);
  create_btn(section, NULL, "SAVE");

  lv_obj_t *sub_wifi_seting = lv_menu_page_create(menu, NULL);
  lv_obj_set_style_pad_hor(sub_wifi_seting, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
  lv_menu_separator_create(sub_wifi_seting);
  section = lv_menu_section_create(sub_wifi_seting);
  cont = create_text(section, NULL, "Set WiFi Connection", LV_MENU_ITEM_BUILDER_VARIANT_1);
  lv_menu_set_load_page_event(menu, cont, sub_wifi_page);
  cont = create_text(section, NULL, "Turn WiFi On or Off", LV_MENU_ITEM_BUILDER_VARIANT_1);
  lv_menu_set_load_page_event(menu, cont, sub_wifi);

  lv_obj_t *sub_location = lv_menu_page_create(menu, NULL);
  lv_obj_set_style_pad_hor(sub_location, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
  lv_menu_separator_create(sub_location);
  section = lv_menu_section_create(sub_location);
  cont = create_text(section, NULL, "Koordinat jadwal sholat", LV_MENU_ITEM_BUILDER_VARIANT_1);
  lv_menu_set_load_page_event(menu, cont, sub_cord_praying);
  cont = create_text(section, NULL, "Koordinat cuaca", LV_MENU_ITEM_BUILDER_VARIANT_1);
  lv_menu_set_load_page_event(menu, cont, sub_cord_weather);

  /*Create a root page*/
  root_page = lv_menu_page_create(menu, "Settings");
  lv_obj_set_style_pad_hor(root_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
  section = lv_menu_section_create(root_page);
  cont = create_text(section, LV_SYMBOL_GPS, "Lokasi", LV_MENU_ITEM_BUILDER_VARIANT_1);
  lv_menu_set_load_page_event(menu, cont, sub_location);
  cont = create_text(section, LV_SYMBOL_SETTINGS, "Layar", LV_MENU_ITEM_BUILDER_VARIANT_1);
  lv_menu_set_load_page_event(menu, cont, sub_display_page);
  cont = create_text(section, LV_SYMBOL_WIFI, "WiFi", LV_MENU_ITEM_BUILDER_VARIANT_1);
  lv_menu_set_load_page_event(menu, cont, sub_wifi_seting);

  create_text(root_page, NULL, "Others", LV_MENU_ITEM_BUILDER_VARIANT_1);
  section = lv_menu_section_create(root_page);
  cont = create_text(section, LV_SYMBOL_DUMMY, "About", LV_MENU_ITEM_BUILDER_VARIANT_1);
  lv_menu_set_load_page_event(menu, cont, sub_about_page);
  cont = create_text(section, LV_SYMBOL_SETTINGS, "Menu mode", LV_MENU_ITEM_BUILDER_VARIANT_1);
  lv_menu_set_load_page_event(menu, cont, sub_menu_mode_page);

  // lv_menu_set_sidebar_page(menu, root_page);
  // lv_event_send(lv_obj_get_child(lv_obj_get_child(lv_menu_get_cur_sidebar_page(menu), 0), 0), LV_EVENT_CLICKED, NULL);

  lv_menu_set_sidebar_page(menu, NULL);
  lv_menu_clear_history(menu);
  lv_menu_set_page(menu, root_page);
  // lv_group_add_obj(group, ui_Seting);
}

void lvglTask(void *parameter)
{
  tft.begin();        /* TFT init */
  tft.setRotation(1); /* Landscape orientation, flipped */
  tft.setBrightness(255);
  uint16_t calData[] = {303, 3902, 249, 304, 3782, 3872, 3825, 287};
  tft.setTouchCalibrate(calData);

  lv_init();
  lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * 10);

  /*Initialize the display*/
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  /*Change the following line to your display resolution*/
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  /*Initialize the (dummy) input device driver*/
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register(&indev_drv);
  dispp = lv_disp_get_default();
  lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), true, LV_FONT_DEFAULT);
  ui_Home_screen_init();
  ui_Kalender_screen_init();
  ui_Jadwal_screen_init();
  ui_Seting_screen_init();
  ui_jam_screen_init();
  ui_info_screen_init();
  lv_disp_load_scr(ui_Home);

  lv_label_set_text(ui_kalendarBckLbl, LV_SYMBOL_LEFT);
  lv_label_set_text(ui_bckLblJadwal, LV_SYMBOL_LEFT);
  lv_label_set_text(ui_jamBckLbl, LV_SYMBOL_LEFT);
  lv_label_set_text(ui_infoBckLbl, LV_SYMBOL_LEFT);

  lv_obj_add_event_cb(ui_Kalendar, ui_event_Kalendar, LV_EVENT_ALL, NULL);
  const char *day_names[] = {"Mg", "Sn", "Sl", "Rb", "Km", "Jm", "Sb"};
  lv_calendar_set_day_names(ui_calendar, day_names);
  lv_obj_add_event_cb(ui_calendar, calendar_event, LV_EVENT_ALL, NULL);

  while (1)
  {
    lv_timer_handler();
    vTaskDelay(5 / portTICK_PERIOD_MS);
  }
}
void httpTask(void *parameter)
{
  while (1)
  {
    HTTPClient http;

    // kirim permintaan HTTP GET ke URL https://api.myquran.com/v1/sholat/jadwal/1220/2023/04/7
    http.begin("https://api.openweathermap.org/data/2.5/weather?q=" + kota + "&lang=id&units=metric&appid=2eb5d5fd2d079473a2c0897d2bd41dd2");

    int httpCode = http.GET();

    // jika permintaan HTTP berhasil
    if (httpCode == HTTP_CODE_OK)
    {
      String payload = http.getString();
      Serial.println(payload);
      DynamicJsonDocument doc(3200);
      DeserializationError error = deserializeJson(doc, payload);

      if (error)
      {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return;
      }

      JsonObject weather = doc["weather"][0];
      int weather_id = weather["id"];                   // 804
      const char *main = weather["main"];               // "Clouds"
      const char *description = weather["description"]; // "awan mendung"
      const char *icon = weather["icon"];               // "04n"
      const char *name = doc["name"];                   // "Banjar"

      JsonObject main_ = doc["main"];
      float temp = main_["temp"];             // 26.07
      float feels_like = main_["feels_like"]; // 26.07

      if (std::strcmp(icon, "01d") == 0 || std::strcmp(icon, "01n") == 0)
      {
        lv_obj_clear_flag(ui_d01, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui_cerahBg, LV_OBJ_FLAG_HIDDEN);
      }
      else
      {
        lv_obj_add_flag(ui_d01, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_cerahBg, LV_OBJ_FLAG_HIDDEN);
      }

      if (std::strcmp(icon, "02d") == 0 || std::strcmp(icon, "02n") == 0)
      {
        lv_obj_clear_flag(ui_d02, LV_OBJ_FLAG_HIDDEN);
      }
      else
      {
        lv_obj_add_flag(ui_d02, LV_OBJ_FLAG_HIDDEN);
      }

      if (std::strcmp(icon, "03d") == 0 || std::strcmp(icon, "03n") == 0)
      {
        lv_obj_clear_flag(ui_d03, LV_OBJ_FLAG_HIDDEN);
      }
      else
      {
        lv_obj_add_flag(ui_d03, LV_OBJ_FLAG_HIDDEN);
      }

      if (std::strcmp(icon, "04d") == 0 || std::strcmp(icon, "04n") == 0)
      {
        lv_obj_clear_flag(ui_d04, LV_OBJ_FLAG_HIDDEN);
      }
      else
      {
        lv_obj_add_flag(ui_d04, LV_OBJ_FLAG_HIDDEN);
      }

      if (std::strcmp(icon, "09d") == 0 || std::strcmp(icon, "09n") == 0)
      {
        lv_obj_clear_flag(ui_d09, LV_OBJ_FLAG_HIDDEN);
      }
      else
      {
        lv_obj_add_flag(ui_d09, LV_OBJ_FLAG_HIDDEN);
      }

      if (std::strcmp(icon, "10d") == 0 || std::strcmp(icon, "10n") == 0)
      {
        lv_obj_clear_flag(ui_d10, LV_OBJ_FLAG_HIDDEN);
      }
      else
      {
        lv_obj_add_flag(ui_d10, LV_OBJ_FLAG_HIDDEN);
      }

      if (std::strcmp(icon, "11d") == 0 || std::strcmp(icon, "11n") == 0)
      {
        lv_obj_clear_flag(ui_d11, LV_OBJ_FLAG_HIDDEN);
      }
      else
      {
        lv_obj_add_flag(ui_d11, LV_OBJ_FLAG_HIDDEN);
      }

      if ((std::strcmp(icon, "11d") == 0 || std::strcmp(icon, "11n") == 0) ||
          (std::strcmp(icon, "10d") == 0 || std::strcmp(icon, "10n") == 0) ||
          (std::strcmp(icon, "09d") == 0 || std::strcmp(icon, "09n") == 0))
      {
        lv_obj_clear_flag(ui_hujanBg, LV_OBJ_FLAG_HIDDEN);
      }
      else
      {
        lv_obj_add_flag(ui_hujanBg, LV_OBJ_FLAG_HIDDEN);
      }

      if ((std::strcmp(icon, "02d") == 0 || std::strcmp(icon, "02n") == 0) ||
          (std::strcmp(icon, "03d") == 0 || std::strcmp(icon, "03n") == 0) ||
          (std::strcmp(icon, "04d") == 0 || std::strcmp(icon, "04n") == 0))
      {
        lv_obj_clear_flag(ui_berawanBg, LV_OBJ_FLAG_HIDDEN);
      }
      else
      {
        lv_obj_add_flag(ui_berawanBg, LV_OBJ_FLAG_HIDDEN);
      }

      char teks[40];
      sprintf(teks, "Terasa seperti | %.01f \u00B0C", feels_like);
      lv_label_set_text(ui_Home_Label1, teks);
      sprintf(teks, "%s", description);
      for (int i = 0; teks[i]; i++)
      {
        teks[i] = std::toupper(teks[i]);
      }
      lv_label_set_text(ui_Home_Label14, teks);
      sprintf(teks, "%s | %.01f\u00B0C", name, temp);
      lv_label_set_text(ui_Home_Label13, teks);
    }
    else
    {
      Serial.println("HTTP request failed");
    }

    http.end();
    vTaskDelay(10000 / portTICK_PERIOD_MS); // delay 10 detik
  }
}
void ntpTask(void *parameter)
{
  WiFi.begin(ssid, pass);
  timeClient.begin();
  timeClient.update();
  WiFi.disconnect();
  while (1)
  {
    timeClient.update();
    now = timeClient.getEpochTime();
    localTime = localtime(&now);
    jam = localTime->tm_hour % 12;          // Jam (0-12)
    menit = localTime->tm_min;              // Menit (0-59)
    detik = localTime->tm_sec;              // Detik (0-59)
    hariDalamSeminggu = localTime->tm_wday; // Hari dalam seminggu (0-6)
    tanggal = localTime->tm_mday;           // Tanggal (1-31)
    bulan = localTime->tm_mon + 1;          // Bulan (1-12)
    tahun = localTime->tm_year + 1900;      // Tahun (dalam format tahun sejak 1900)

    if (jam == 0)
    {
      jam = 12; // Jam 0 dalam format 12 jam adalah 12 AM
    }
    bool isAM = localTime->tm_hour < 12;

    char waktu[5];
    sprintf(waktu, "%02d:%02d %s", jam, menit, isAM ? "AM" : "PM");
    lv_label_set_text(ui_smallClockLabel, waktu);
    char tanggalKecil[5];
    sprintf(tanggalKecil, "%02d/%02d", tanggal, bulan);
    lv_label_set_text(ui_bigClockLabel, tanggalKecil);
    lv_label_set_text(ui_KalendarLabel, daysOfWeek[hariDalamSeminggu]);
    char waktu_[2];
    sprintf(waktu_, "%02d", jam);
    lv_label_set_text(ui_labelClock, waktu_);
    sprintf(waktu_, "%02d", menit);
    lv_label_set_text(ui_labelMinute, waktu_);
    sprintf(waktu_, "%02d", detik);
    lv_label_set_text(ui_labelSecond, waktu_);
    char tanggalBesar[30];
    sprintf(tanggalBesar, "%s %02d/%02d/%d", daysOfWeek[hariDalamSeminggu], tanggal, bulan, tahun);
    lv_label_set_text(ui_labelSecond1, tanggalBesar);
    vTaskDelay(1000 / portTICK_PERIOD_MS); // delay 1 detik
  }
}

void setup()
{
#if LV_USE_LOG != 0
  lv_log_register_print_cb(my_print); /* register print function for debugging */
#endif
  Serial.begin(115200);
  Serial.println("Setup done");
  xTaskCreate(lvglTask, "LVGL", 10000, NULL, 1, NULL);
  xTaskCreate(httpTask, "http", 9000, NULL, 2, NULL);
  xTaskCreate(ntpTask, "ntp", 9000, NULL, 3, NULL);
}

void loop()
{
}
