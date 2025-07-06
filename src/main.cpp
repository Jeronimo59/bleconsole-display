#include <Arduino.h>
#include <esp32_smartdisplay.h>

#include "ui.h"
#include <core/lv_global.h>
#include <misc/lv_log.h>

void lv_log_register_print_cb(lv_log_print_g_cb_t print_cb);

void my_log_cb(lv_log_level_t level, const char *buf)
{
   Serial.println(buf);
}

void setup()
{
   Serial.begin(115200);
   Serial.setDebugOutput(true);

   smartdisplay_init();

   /** initialize display */
   __attribute__((unused)) auto disp = lv_disp_get_default();
   // lv_disp_set_rotation(disp, LV_DISPLAY_ROTATION_90);

   ui_init();

   /** setup logs */
   lv_log_register_print_cb(my_log_cb);

   lv_log("\n*************** bleconsole 0.1 ****************");
}

auto lv_last_tick = millis();

void loop()
{
   auto const now = millis();
   // Update the ticker
   lv_tick_inc(now - lv_last_tick);
   lv_last_tick = now;
   // Update the UI
   lv_timer_handler();

   delay(100);
}