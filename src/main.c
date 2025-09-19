#include "../include/game.h"
#include "../include/shader.h"
#include "../include/img.h"
#include "../include/tile.h"
#include "../include/dialog.h"

unsigned cont = 1;
unsigned lamb;

extern double hw, hh;

void key_quit(unsigned short code,
		unsigned short type, int value)
{
	cont = 0;
}

void key_cont(unsigned short code,
		unsigned short type, int value)
{
	if (!type)
		vdialog_action();
}

void ensure_move(enum dir dir) {
	if (char_animation(lamb) == AN_WALK)
		return;
	char_face(lamb, dir);
	char_animate(lamb, AN_WALK);
}

void
my_update(void) {
	if (key_value(KEY_J) || key_value(KEY_DOWN))
		ensure_move(DIR_DOWN);
	if (key_value(KEY_K) || key_value(KEY_UP))
		ensure_move(DIR_UP);
	if (key_value(KEY_H) || key_value(KEY_LEFT))
		ensure_move(DIR_LEFT);
	if (key_value(KEY_L) || key_value(KEY_RIGHT))
		ensure_move(DIR_RIGHT);
}

int main() {
	double lamb_speed = 2;
	unsigned font;

	game_init();

	input_reg(KEY_Q, key_quit);
	input_reg(KEY_ESC, key_quit);
	input_reg(KEY_SPACE, key_cont);

	view_load("./map.txt");

	unsigned font_img = img_load("./resources/font.png");
	dialog.font_tm = tm_load(font_img, 9, 21); 

	unsigned ui_img = img_load("./resources/Complete_UI_Essential_Pack_Free/01_Flat_Theme/Spritesheets/Spritesheet_UI_Flat.png");
	dialog.ui_box->ui_tm
		= tm_load(ui_img, 32, 32);

	lamb = 0;

	char_dialog(1, "Choose from the palette in the ground by pressing 'x' and draw by pressing 'i'. Do you understand?");

	game_start();

	while (cont) {
		shader_render();
		view_render();
		dialog_render();
		game_update();
		my_update();

		char_pos(&cam.x, &cam.y, lamb);
	}

	game_deinit();
	return 0;
}
