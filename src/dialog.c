#include "../include/dialog.h"
#include "../include/tile.h"
#include "../include/time.h"

#include <math.h>

#include <qmap.h>

typedef struct {
	char *text, *next;
} dialog_t;

unsigned ui_box_hd;

ui_box_t dialog_box = {
	.scale = 3,
	.idx = {
		23 + 1, 23 + 2, 23 + 3,
		46 + 1, 46 + 2, 46 + 3,
		69 + 1, 69 + 2, 69 + 3,
		92 + 1, 92 + 2, 92 + 3,
		115 + 1,
	},
	.pad = 0.2,
};

dialog_t cdialog;

dialog_settings_t dialog = {
	.ui_box = &dialog_box,
	.font_scale = 2,
};

static inline void
box_row(unsigned start_idx, ui_box_t *ui_box,
		unsigned ix, unsigned iy,
		unsigned w, unsigned h,
		unsigned nx, unsigned mx)
{
	unsigned idx = ui_box->idx[start_idx];

	tm_render(ui_box->ui_tm, ix, iy,
			idx % nx,
			idx / nx,
			w, h, 1, 1);
	ix += w;

	while (ix + w < mx) {
		idx = ui_box->idx[1 + start_idx];

		tm_render(ui_box->ui_tm, ix, iy,
				idx % nx,
				idx / nx,
				w, h, 1, 1);
		ix += w;
	};

	idx = ui_box->idx[2 + start_idx];

	tm_render(ui_box->ui_tm, ix, iy,
			idx % nx,
			idx / nx,
			w, h, 1, 1);

	iy += h;
}

void
box_render(ui_box_t *ui_box,
		uint32_t x, uint32_t y,
		uint32_t mx, uint32_t my)
{
	const tm_t *tm = tm_get(ui_box->ui_tm);
	uint32_t start_idx = 0, idx;
	uint32_t h = ui_box->scale * tm->h,
		 w = ui_box->scale * tm->w,
		 iy;

	long dx = mx - x;
	long dy = my - y;
	unsigned ax = dx / w;
	unsigned ay = dy / w;

	dx = dx - ax * w;
	dy = dy - ay * h;

	x += dx / 2;
	mx -= dx / 2;

	y += dy / 2;
	my -= dy / 2;

	iy = y;

	if (iy + h >= my)
		goto small_height;

	box_row(0, ui_box, x, y, w, h, tm->nx, mx);
	iy += h;

	for (; iy + h < my; iy += h) {
		box_row(3, ui_box, x, iy,
				w, h, tm->nx, mx);
	}

	box_row(6, ui_box, x, iy,
			w, h, tm->nx, mx);

	return;

small_height:

	if (mx > x + w) {
		box_row(9, ui_box, x, iy,
				w, h, tm->nx, mx);
		return;
	}

	idx = ui_box->idx[12];
	tm_render(ui_box->ui_tm, x, y,
			idx % tm->nx,
			idx / tm->nx,
			w, h, 1, 1);
}

static inline void
box_d(
		long *dx_r, long *dy_r,
		ui_box_t *box,
		unsigned x, unsigned y,
		unsigned mx, unsigned my)
{
	const tm_t *tm = tm_get(dialog.ui_box->ui_tm);
	uint32_t h = box->scale * tm->h,
		 w = box->scale * tm->w;

	long dx = mx - x;
	long dy = my - y;
	unsigned ax = dx / w;
	unsigned ay = dy / w;

	*dx_r = dx - ax * w;
	*dy_r = dy - ay * h;
}

void
dialog_start(char *text)
{
	cdialog.text = text;
	cdialog.next = NULL;
}

void
dialog_render(void)
{
	const tm_t *tm = tm_get(dialog.ui_box->ui_tm);
	unsigned x = 20;
	unsigned y = be_height - 250;
	unsigned mx = be_width - 20;
	unsigned my = be_height - 10;
	long dx, dy;

	if (!cdialog.text)
		return;

	uint32_t h = dialog.ui_box->scale * tm->h,
		 w = dialog.ui_box->scale * tm->w;

	box_d(&dx, &dy, dialog.ui_box, x, y, mx, my);

	box_render(dialog.ui_box,
			x, y,
			mx,
			my);

	x += dx / 2 + (long) (w * dialog.ui_box->pad);
	mx -= dx / 2 + (long) (w * dialog.ui_box->pad);

	y += dy / 2 + (long) (h * dialog.ui_box->pad);
	my -= dy / 2 + (long) (h * dialog.ui_box->pad);

	img_tint(0x77000000);

	cdialog.next = font_render(
			dialog.font_tm,
			cdialog.text,
			x, y,
			mx, my,
			dialog.font_scale);

	if (cdialog.next && (unsigned) round(time_tick * 2) & 1)
		font_render_char(
				dialog.font_tm,
				158,
				mx - tm->w, my - tm->h,
				dialog.font_scale);


	img_tint(default_tint);
}

int
dialog_action(void) {
	if (!cdialog.text)
		return 0;

	cdialog.text = cdialog.next;
	return 1;
}
