#include "pebble.h"
#include "startline.h"





Window *s_main_window;
TextLayer *s_data_layer;

void setLayerBounds(void *subject, int16_t new_bounds)
{
  APP_LOG(APP_LOG_LEVEL_INFO, "In setter with %d", new_bounds);
  GRect b;
  b = layer_get_bounds(subject);
  b.origin.x = new_bounds;
  layer_set_bounds((Layer *)subject, b);
};


static GFont s_4_font;

static int ib = 0;

void up_click_handler(ClickRecognizerRef recognizer, void *context) {

  ++ib;
  setLayerBounds(s_data_layer, ib);
} 


//
// Down Click Handler - just like the up click handler
//
void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  
  --ib;
  setLayerBounds(s_data_layer, ib);
}
  




static void main_window_load(Window *window) {
  
  s_4_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_PTN_50));
     
  s_data_layer = text_layer_create(GRect(73, 91, 142, 51));
  layer_set_frame((Layer *) s_data_layer, GRect(73, 91, 71, 51));

  text_layer_set_background_color(s_data_layer, GColorClear);
  text_layer_set_text_color(s_data_layer, GColorWhite);
  text_layer_set_text_alignment(s_data_layer, GTextAlignmentLeft);
  text_layer_set_overflow_mode(s_data_layer, GTextOverflowModeWordWrap);
  layer_add_child(window_get_root_layer(s_main_window), text_layer_get_layer(s_data_layer));
  text_layer_set_font(s_data_layer, s_4_font); 
  text_layer_set_text(s_data_layer, "MOO");
}


static void main_window_unload(Window *window) {
  text_layer_destroy(s_data_layer);    
  fonts_unload_custom_font(s_4_font);
}


static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();
  window_set_background_color(s_main_window, GColorBlack);
  // Set click handlers
  window_set_click_config_provider(s_main_window, click_config_provider);
  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
}

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
}

//
// Main
//
int main(void) {
  
  init();

  app_event_loop();

  deinit();
  
}

