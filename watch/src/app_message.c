#include <pebble.h>

static Window *window;
static TextLayer *desc_text;
static TextLayer *alphabet_text;
	
// Write message to buffer & send
void send_message(void){
	DictionaryIterator *iter;
	
	app_message_outbox_begin(&iter);
	dict_write_cstring(iter, 0, "A");
	
	dict_write_end(iter);
  	app_message_outbox_send();
}

// Called when a message is received from phone-app
static void in_received_handler(DictionaryIterator *received, void *context) {
	Tuple *tuple;
	
	tuple = dict_find(received, 1);
	if(tuple) {
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Received Message: %s", tuple->value->cstring);
		text_layer_set_text(alphabet_text, tuple->value->cstring);
	}}

// Called when an incoming message from phone-app is dropped
static void in_dropped_handler(AppMessageResult reason, void *context) {	
}

// Called when phone-app does not acknowledge receipt of a message
static void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
}

void init(void) {
	window = window_create();
	window_stack_push(window, true);
	
	// Register AppMessage handlers
	app_message_register_inbox_received(in_received_handler); 
	app_message_register_inbox_dropped(in_dropped_handler); 
	app_message_register_outbox_failed(out_failed_handler);
		
	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());

	desc_text = text_layer_create(GRect(0, 0, 144, 16));
	text_layer_set_text_alignment(desc_text, GTextAlignmentCenter);
  	layer_add_child(window_get_root_layer(window), text_layer_get_layer(desc_text));
  	text_layer_set_text(desc_text, "current letter");

  	alphabet_text = text_layer_create(GRect(0, 18, 144, 55));
  	text_layer_set_text_alignment(alphabet_text, GTextAlignmentCenter);
  	text_layer_set_font(alphabet_text, fonts_get_system_font(FONT_KEY_GOTHIC_28));
  	layer_add_child(window_get_root_layer(window), text_layer_get_layer(alphabet_text));
  	text_layer_set_text(alphabet_text, "*");

	send_message();
}

void deinit(void) {
	app_message_deregister_callbacks();
	window_destroy(window);
}

int main( void ) {
	init();
	app_event_loop();
	deinit();
}