#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"


#define MY_UUID { 0x2A, 0xC7, 0xAC, 0xEB, 0xB6, 0xE2, 0x45, 0x39, 0x9E, 0xC2, 0xB9, 0x8A, 0xA6, 0x88, 0x32, 0xC9 }
PBL_APP_INFO(MY_UUID,
			 "Menu Example", "Computing Eureka",
			 1, 0, /* App version */
			 DEFAULT_MENU_ICON,
			 APP_INFO_STANDARD_APP);

Window window;
MenuLayer mainMenu;
void showDetail(MenuIndex* index); // Defined in detailView.c

char hex[] = "0\0001\0002\0003\0004\0005\0006\0007\0008\0009\000A\000B\000C\000D\000E\000F";


void mainMenu_select_click(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context)
{
	showDetail(cell_index); // Defined in detailView.c
}
void mainMenu_draw_row(GContext *ctx, Layer *cell_layer, MenuIndex *cell_index, void *callback_context)
{
	graphics_context_set_text_color(ctx, GColorBlack); // This is important.
	graphics_text_draw(ctx, hex+2*cell_index->row, fonts_get_system_font(FONT_KEY_GOTHIC_14), GRect(0,0,cell_layer->frame.size.w,cell_layer->frame.size.h), GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
}
void mainMenu_draw_header(GContext *ctx, Layer *cell_layer, uint16_t section_index, void *callback_context)
{
	graphics_context_set_text_color(ctx, GColorBlack); // This is important.
	graphics_text_draw(ctx, hex+2*section_index, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD), GRect(0,0,cell_layer->frame.size.w,cell_layer->frame.size.h), GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
}
int16_t mainMenu_get_header_height(struct MenuLayer *menu_layer, uint16_t section_index, void *callback_context)
{
	return 30;
}
int16_t mainMenu_get_cell_height(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context)
{
	return 20;
}
uint16_t mainMenu_get_num_rows_in_section(struct MenuLayer *menu_layer, uint16_t section_index, void *callback_context)
{
	return (section_index+1)*3;
}
uint16_t mainMenu_get_num_sections(struct MenuLayer *menu_layer, void *callback_context)
{
	return 3;
}

MenuLayerCallbacks cbacks;

void handle_init(AppContextRef ctx) {
	(void)ctx;

	window_init(&window, "Menu Exapmle");

	menu_layer_init(&mainMenu, GRect(0,0,window.layer.frame.size.w,window.layer.frame.size.h-15));
	menu_layer_set_click_config_onto_window(&mainMenu, &window);
	cbacks.get_num_sections = &mainMenu_get_num_sections;
	cbacks.get_num_rows = &mainMenu_get_num_rows_in_section;
	cbacks.get_cell_height = &mainMenu_get_cell_height;
	cbacks.get_header_height = &mainMenu_get_header_height;
	cbacks.select_click = &mainMenu_select_click;
	cbacks.draw_row = &mainMenu_draw_row;
	cbacks.draw_header = &mainMenu_draw_header;
	// cbacks.selection_changed = &func(struct MenuLayer *menu_layer, MenuIndex new_index, MenuIndex old_index, void *callback_context) // I assume this would be called whenever an up or down button was pressed.
	// cbacks.select_long_click = &func(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context) // I didn't use this.
	menu_layer_set_callbacks(&mainMenu, NULL, cbacks);
	layer_add_child(&window.layer, menu_layer_get_layer(&mainMenu));

	window_stack_push(&window, true);
}


void pbl_main(void *params) {
	PebbleAppHandlers handlers = {
		.init_handler = &handle_init
	};
	app_event_loop(params, &handlers);
}
