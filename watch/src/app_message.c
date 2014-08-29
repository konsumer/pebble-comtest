#include <pebble.h>

static Window *window;
static TextLayer *desc_text;
static TextLayer *alphabet_text;
	
// Write message to buffer & send
void send_message(const char *letter){
	DictionaryIterator *iter;
	
	app_message_outbox_begin(&iter);
	dict_write_cstring(iter, 0, letter);
	
	dict_write_end(iter);
  	app_message_outbox_send();
}

// turn an AppMessageResult into text
char *translate_error(AppMessageResult result) {
  switch (result) {
    case APP_MSG_OK: return "APP_MSG_OK: All good, operation was successful.";
    case APP_MSG_SEND_TIMEOUT: return "APP_MSG_SEND_TIMEOUT: The other end did not confirm receiving the sent data with an (n)ack in time.";
    case APP_MSG_SEND_REJECTED: return "APP_MSG_SEND_REJECTED: The other end rejected the sent data, with a \"nack\" reply.";
    case APP_MSG_NOT_CONNECTED: return "APP_MSG_NOT_CONNECTED: The other end was not connected.";
    case APP_MSG_APP_NOT_RUNNING: return "APP_MSG_APP_NOT_RUNNING: The local application was not running.";
    case APP_MSG_INVALID_ARGS: return "APP_MSG_INVALID_ARGS: The function was called with invalid arguments.";
    case APP_MSG_BUSY: return "APP_MSG_BUSY: There are pending (in or outbound) messages that need to be processed first before new ones can be received or sent.";
    case APP_MSG_BUFFER_OVERFLOW: return "APP_MSG_BUFFER_OVERFLOW: The buffer was too small to contain the incoming message.";
    case APP_MSG_ALREADY_RELEASED: return "APP_MSG_ALREADY_RELEASED: The resource had already been released.";
    case APP_MSG_CALLBACK_ALREADY_REGISTERED: return "APP_MSG_CALLBACK_ALREADY_REGISTERED: The callback node was already registered, or its ListNode has not been initialized.";
    case APP_MSG_CALLBACK_NOT_REGISTERED: return "APP_MSG_CALLBACK_NOT_REGISTERED: The callback could not be deregistered, because it had not been registered before.";
    case APP_MSG_OUT_OF_MEMORY: return "APP_MSG_OUT_OF_MEMORY: The support library did not have sufficient application memory to perform the requested operation.";
    case APP_MSG_CLOSED: return "APP_MSG_CLOSED: App message was closed.";
    case APP_MSG_INTERNAL_ERROR: return "APP_MSG_INTERNAL_ERROR: An internal OS error prevented APP_MSG from completing an operation.";
    default: return "An unknown error occurred.";
  }
}

// Called when a message is received from phone-app
// when I recieve a message, send the same letter back
static void in_received_handler(DictionaryIterator *received, void *context) {
	Tuple *tuple;
	
	tuple = dict_find(received, 0);
	if(tuple) {
		APP_LOG(APP_LOG_LEVEL_DEBUG, "received letter: %s", tuple->value->cstring);
		text_layer_set_text(alphabet_text, tuple->value->cstring);
		send_message(tuple->value->cstring);
	}
}

// Called when an incoming message from phone-app is dropped
static void in_dropped_handler(AppMessageResult reason, void *context) {
	char *reasonText = translate_error(reason);
	APP_LOG(APP_LOG_LEVEL_DEBUG, "dropped: %s", reasonText);
}

// Called when phone-app does not acknowledge receipt of a message
static void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
	char *reasonText = translate_error(reason);
	APP_LOG(APP_LOG_LEVEL_DEBUG, "failed: %s", reasonText);
}


int main( void ) {
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
  	text_layer_set_text(desc_text, "current letter (phone)");

  	alphabet_text = text_layer_create(GRect(0, 18, 144, 50));
	text_layer_set_text_alignment(alphabet_text, GTextAlignmentCenter);
	text_layer_set_font(alphabet_text, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(alphabet_text));
	text_layer_set_text(alphabet_text, "*");

	app_event_loop();
	
	app_message_deregister_callbacks();
	window_destroy(window);
}