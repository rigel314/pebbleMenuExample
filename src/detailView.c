#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

typedef int (*sprintfref)(char *restrict s, const char *restrict format, ...);

Window detailW;
TextLayer detailW_text;

char msg[10+1+6+1];
extern char hex[]; // Defined in menuExample.c

void showDetail(MenuIndex* index)
{
	window_init(&detailW, "Menu Exapmle");

	text_layer_init(&detailW_text, GRect(0,52,144,40));
	text_layer_set_text_alignment(&detailW_text, GTextAlignmentCenter); // Center the text.
	text_layer_set_font(&detailW_text, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
	msg[0] = 0; // Ensure the message starts with a null so strcat will overwrite it.
	strcat(msg, "Section: ");
	strcat(msg, hex+index->section*2);
	strcat(msg, "\nRow: ");
	strcat(msg, hex+index->row*2);

	text_layer_set_text(&detailW_text, msg);
	layer_add_child(&detailW.layer, &detailW_text.layer);

	window_stack_push(&detailW, true); // The back button will dismiss the current window, not close the app.  So just press back to go back to the master view.
}
