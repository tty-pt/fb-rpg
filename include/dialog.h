#ifndef DIALOG_H
#define DIALOG_H

#include "font.h"

typedef uint32_t box_idx_t[13];

typedef struct {
	unsigned ui_tm;
	box_idx_t idx;
	uint32_t scale;
	double pad;
} ui_box_t;

typedef struct {
	unsigned font_tm;
	uint32_t font_scale;
	ui_box_t *ui_box;
} dialog_settings_t;

extern dialog_settings_t dialog;

void dialog_init(unsigned font_tm);

void dialog_start(char *text);
void dialog_render(void);
int dialog_action(void);

void
box_render(ui_box_t *ui_box,
		uint32_t x, uint32_t y,
		uint32_t mx, uint32_t my);

#endif
