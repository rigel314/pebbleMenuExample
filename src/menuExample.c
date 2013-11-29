#include <pebble.h>

Window* window;
MenuLayer* mainMenu;
void showDetail(MenuIndex* index); // Defined in detailView.c

char hex[] = "0\0001\0002\0003\0004\0005\0006\0007\0008\0009\000A\000B\000C\000D\000E\000F";
// hex+2*0 is the string "0",
// hex+2*1 is the string "1",
// hex+2*3 is the string "3" and so on.  There is an implied \0 at the end of a constant C string.
// I used \000 here to escape a null-termination.  \0 followed by a 1 looks like \01 and confuses my syntax hilighter.  It may also confuse the compiler.

void mainMenu_select_click(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context)
{ // Show the detail view when select is pressed.
	showDetail(cell_index); // Defined in detailView.c
}
void mainMenu_draw_row(GContext *ctx, const Layer *cell_layer, MenuIndex *cell_index, void *callback_context)
{ // Adding the row number as text on the row cell.
	graphics_context_set_text_color(ctx, GColorBlack); // This is important.
	graphics_draw_text(ctx, hex+2*cell_index->row, fonts_get_system_font(FONT_KEY_GOTHIC_14), GRect(0,0,layer_get_frame(cell_layer).size.w,layer_get_frame(cell_layer).size.h), GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
	// Just saying layer_get_frame(cell_layer) for the 4th argument doesn't work.  Probably because the GContext is relative to the cell already, but the cell_layer.frame is relative to the menulayer or the screen or something.
}
void mainMenu_draw_header(GContext *ctx, const Layer *cell_layer, uint16_t section_index, void *callback_context)
{ // Adding the header number as text on the header cell.
	graphics_context_set_text_color(ctx, GColorBlack); // This is important.
	graphics_draw_text(ctx, hex+2*section_index, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD), GRect(0,0,layer_get_frame(cell_layer).size.w,layer_get_frame(cell_layer).size.h), GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
}
int16_t mainMenu_get_header_height(struct MenuLayer *menu_layer, uint16_t section_index, void *callback_context)
{ // Always 30px tall for a header cell
	return 30;
}
int16_t mainMenu_get_cell_height(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context)
{ // Always 20px tall for a normal cell
	return 20;
}
uint16_t mainMenu_get_num_rows_in_section(struct MenuLayer *menu_layer, uint16_t section_index, void *callback_context)
{ // 3, 6, and 9 rows per section
	return (section_index+1)*3;
}
uint16_t mainMenu_get_num_sections(struct MenuLayer *menu_layer, void *callback_context)
{ // Always 3 sections
	return 3;
}

MenuLayerCallbacks cbacks; // The Pebble API documentation says this should be long-lived
// Currently, however, MenuLayerCallbacks is a struct and the menu_layer_set_callbacks does not take a pointer to this struct as an argument.
// Instead, it takes the struct itself.  In C structs are passed by value and returned by value, so this would work fine if it were not long-lived.  The struct's data would be copied anyway.

void init(void)
{
	GRect frame;
	window = window_create();

	frame = layer_get_bounds(window_get_root_layer(window));

	mainMenu = menu_layer_create(GRect(0,0,frame.size.w,frame.size.h));
	menu_layer_set_click_config_onto_window(mainMenu, window); // Sets the Window's button callbacks to the MenuLayer's default button callbacks.
	// Set all of our callbacks.
	cbacks.get_num_sections = &mainMenu_get_num_sections; // Gets called at the beginning of drawing the table.
	cbacks.get_num_rows = &mainMenu_get_num_rows_in_section; // Gets called at the beginning of drawing each section.
	cbacks.get_cell_height = &mainMenu_get_cell_height; // Gets called at the beginning of drawing each normal cell.
	cbacks.get_header_height = &mainMenu_get_header_height; // Gets called at the beginning of drawing each header cell.
	cbacks.select_click = &mainMenu_select_click; // Gets called when select is pressed.
	cbacks.draw_row = &mainMenu_draw_row; // Gets called to set the content of a normal cell.
	cbacks.draw_header = &mainMenu_draw_header; // Gets called to set the content of a header cell.
	// cbacks.selection_changed = &func(struct MenuLayer *menu_layer, MenuIndex new_index, MenuIndex old_index, void *callback_context) // I assume this would be called whenever an up or down button was pressed.
	// cbacks.select_long_click = &func(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context) // I didn't use this.
	menu_layer_set_callbacks(mainMenu, NULL, cbacks); // I have no user data to supply to the callback functions, so 
	layer_add_child(window_get_root_layer(window), menu_layer_get_layer(mainMenu));

	window_stack_push(window, true);
}

void deinit(void)
{
	menu_layer_destroy(mainMenu);
	window_destroy(window);
}

int main(void)
{
	init();
	app_event_loop();
	deinit();
}
