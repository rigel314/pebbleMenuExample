#include <pebble.h>

Window* detailW;
TextLayer* detailW_text;

char msg[10+1+6+1];
extern char hex[]; // Defined in menuExample.c

void detailUnload(Window* win)
{
	APP_LOG(APP_LOG_LEVEL_INFO, "Destroying Detail Window.");
	text_layer_destroy(detailW_text);
	window_destroy(detailW);
}

void showDetail(MenuIndex* index)
{
	WindowHandlers wh = { .unload = &detailUnload };
	detailW = window_create();
	window_set_window_handlers(detailW, wh);

	detailW_text = text_layer_create(GRect(0,52,144,40));
	text_layer_set_text_alignment(detailW_text, GTextAlignmentCenter); // Center the text.
	text_layer_set_font(detailW_text, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
	msg[0] = 0; // Ensure the message starts with a null so strcat will overwrite it.
	strcat(msg, "Section: ");
	strcat(msg, hex+index->section*2);
	strcat(msg, "\nRow: ");
	strcat(msg, hex+index->row*2);

	text_layer_set_text(detailW_text, msg);
	layer_add_child(window_get_root_layer(detailW), text_layer_get_layer(detailW_text));

	window_stack_push(detailW, true); // The back button will dismiss the current window, not close the app.  So just press back to go back to the master view.
}
