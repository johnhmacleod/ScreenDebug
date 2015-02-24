#include "pebble.h"

Window *s_main_window;
TextLayer *s_data_layer;
int boundsShift = 0;

void setLayerBounds(TextLayer *subject, int16_t new_bounds)
{
  APP_LOG(APP_LOG_LEVEL_INFO, "In setter with %d", new_bounds);
  GRect b;
  b = layer_get_bounds((Layer *)subject);
  b.origin.x = new_bounds;
  layer_set_bounds((Layer *)subject, b);
};

void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  ++boundsShift;
  setLayerBounds(s_data_layer, boundsShift);
} 

void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  --boundsShift;
  setLayerBounds(s_data_layer, boundsShift);
}
  
static void main_window_load(Window *window) {
  s_data_layer = text_layer_create(GRect(73, 91, 142, 51));
  layer_set_frame((Layer *) s_data_layer, GRect(73, 91, 71, 51));
  text_layer_set_background_color(s_data_layer, GColorClear);
  text_layer_set_text_color(s_data_layer, GColorWhite);
  layer_add_child(window_get_root_layer(s_main_window), text_layer_get_layer(s_data_layer));
  text_layer_set_font(s_data_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_MEDIUM_NUMBERS)); 
  text_layer_set_text(s_data_layer, "890");
}


static void main_window_unload(Window *window) {
  text_layer_destroy(s_data_layer);    
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void init() {
  s_main_window = window_create();
  window_set_background_color(s_main_window, GColorBlack);
  window_set_click_config_provider(s_main_window, click_config_provider);
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  window_stack_push(s_main_window, true);
}

static void deinit() {
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}

