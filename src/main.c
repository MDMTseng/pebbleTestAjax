#include <pebble.h>
  #include <stdio.h>
#define SCRRES_H 144
#define SCRRES_W 168
  
#define SWP(percent) (percent*SCRRES_W/100)
#define SHP(percent) (percent*SCRRES_H/100)
  
  
AppTimer *timer;
  
Window *window;
TextLayer *text_layer;
TextLayer *text_layer2;
char strBuff[100];
int timer_callbackCounter=0;

static void sendData(char* str) {
  

	Tuplet tuple1 = TupletCString(0, str);
  
	DictionaryIterator *iter;
	app_message_outbox_begin(&iter);
	if (iter == NULL) {
		return;
	}

	dict_write_tuplet(iter, &tuple1);
	dict_write_end(iter);

	app_message_outbox_send();
}
static void timer_callback(void *data) {
    
    //Get current position
    timer = app_timer_register(2000, (AppTimerCallback) timer_callback, NULL);
  snprintf(strBuff, sizeof(strBuff),  "timer_callback::::%d",timer_callbackCounter);
  timer_callbackCounter++;
    text_layer_set_text(text_layer2,strBuff);
    sendData(strBuff);
    //text_layer_set_text(text_layer, recv_data1->value->cstring);
}
  
  
  
  
  
  
char strBuff2[100];
  
int droppedErrorC=0;
int out_failedC=0;


void in_received_handler(DictionaryIterator *received, void *context) {
  Tuple *recv_data1 = dict_find(received, 0);
  
	//APP_LOG(APP_LOG_LEVEL_DEBUG, recv_data1->value->cstring);
  
  snprintf(strBuff2, sizeof(strBuff2),  ">>%s  Err:%d:%d",recv_data1->value->cstring,droppedErrorC,out_failedC);
  text_layer_set_text(text_layer, strBuff2);

 // window_stack_push(window, true);
}

void in_dropped_handler(AppMessageResult reason, void *context) {
	APP_LOG(APP_LOG_LEVEL_DEBUG, "in dropped handler");
  droppedErrorC++;
  text_layer_set_text(text_layer, "XXXXXXXXXXX");
}


void out_sent_handler(DictionaryIterator *sent, void *context) {
	APP_LOG(APP_LOG_LEVEL_DEBUG, "out sent handler");
}

void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
	APP_LOG(APP_LOG_LEVEL_DEBUG, "out failed handler");
  out_failedC++;
}


void handle_init(void) {
	// Create a window and text layer
	window = window_create();
	text_layer = text_layer_create(GRect(SWP(0),SHP(0), SWP(100),SHP(50)));
	text_layer2 = text_layer_create(GRect(SWP(00),SHP(50), SWP(100),SHP(50)));
	
	// Set the text, font, and text alignment
	text_layer_set_text(text_layer, "Welcome start to interact with JS");
	text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	text_layer_set_text_alignment(text_layer, GTextAlignmentLeft);
  
	text_layer_set_font(text_layer2, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	text_layer_set_text_alignment(text_layer2, GTextAlignmentLeft);
	
	// Add the text layer to the window
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer2));

	// Push the window
	window_stack_push(window, true);
	
	// App Logging!
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Just pushed a window!");
  
	app_message_register_inbox_received(in_received_handler);
	app_message_register_inbox_dropped(in_dropped_handler);
  app_message_register_outbox_sent(out_sent_handler);
	app_message_register_outbox_failed(out_failed_handler);

	const uint32_t inbound_size  =app_message_inbox_size_maximum();;
	const uint32_t outbound_size =app_message_outbox_size_maximum();;
	app_message_open(inbound_size, outbound_size);
  
  timer_callback(NULL);
  
}

void handle_deinit(void) {
	// Destroy the text layer
	text_layer_destroy(text_layer);
	
	// Destroy the window
	window_destroy(window);
}


int main(void) {
	handle_init();
	app_event_loop();
	handle_deinit();
}
