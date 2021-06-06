#include <stdlib.h>

unsigned int *screen = NULL;
unsigned char *fire = NULL;
unsigned int *palette = NULL;

/* static means these variables and functions won't be exported */
static int width = 0;
static int height = 0;
static int pixels = 0;

/* sweep from (r1, g1, b1) -> (r2, g2, b2) in num colours */
static void palrange(unsigned int *pal, int num, int r1, int g1, int b1, int r2, int g2, int b2)
{
	int i;
	unsigned char r, g, b;
	float dr = (r2 - r1) / (float)num;
	float dg = (g2 - g1) / (float)num;
	float db = (b2 - b1) / (float)num;
	for (i = 0; i < num; i++) {
		r = r1 + (int)(i * dr);
		g = g1 + (int)(i * dg);
		b = b1 + (int)(i * db);
		pal[i] = (0xff << 24) + (b << 16) + (g << 8) + r;
	}
}

/* Copy from fire screen to ImageData screen
 * using palette as map */
static void fire2screen(void)
{
	int x, y;
	for (y = 0; y < pixels; y += width) {
		for (x = 0; x < width; x++) {
			/* Save colour to screen */
			screen[y + x] = palette[fire[y + x]];
		}
	}
}

/* --- init and render are exported --- */

/* Return pixel screen buffer */
unsigned int *init(int w, int h)
{
	int i;
	int r1, g1, b1, r2, g2, b2, r3, g3, b3, r4, g4, b4;
	width = w;
	height = h;
	pixels = width * height;
	screen = malloc((pixels) * 4);
	/* Alloc 4 extra rows for fire seeds
	 * below visible area */
	fire = malloc(pixels + 4 * width);

	/* Calculate the palette, 256 colors.
	 * Each color stored as a 4-byte int.
	 * Same representation as canvas
	 * ImageData.data, RGBA, little endian.
	 * (alpha << 24) | (blue << 16) | (green << 8) | red.
	 * We don't use alpha, always 0xff */
	palette = malloc(256 * 4);
	r1 = 0;   g1 = 0;   b1 = 32;
	r2 = 128; g2 = 16;  b2 = 0;
	r3 = 207; g3 = 76;  b3 = 16;
	r4 = 252; g4 = 217; b4 = 91;
	palrange(&palette[0], 16, 0, 0, 0, r1, g1, b1);
	palrange(&palette[16], 48, r1, g1, b1, r2, g2, b2);
	palrange(&palette[64], 64, r2, g2, b2, r3, g3, b3);
	palrange(&palette[128], 64, r3, g3, b3, r4, g4, b4);
	palrange(&palette[192], 64, r4, g4, b4, 255, 255, 255);

	/* Init screen to black */
	for (i = 0; i < pixels; i++) {
		fire[i] = 0;
	}

	return screen;
}

void render(double timestamp __attribute__((unused)))
{
	int x, y;
	int o1, o2, o3, o4;
	int pixel;
	int fireindex;

	/* Randomize off-screen rows */
	for (y = 0; y < 2; y++) {
		for (x = 0; x < width; x++) {
			//int r = ((rand() & 0xff) << 1) & 0xff;
			int r = rand() & 0xff;
			/* Randomize which row to put the new pixel on */
			int flameh = rand() & 3;
			fire[pixels + flameh * width + x] = r;
		}
	}

	/* Fire effect
	 *      c
	 * --- --- ---
	 * x-1| x |x+1
	 * y+1|y+1|y+1
	 * --- --- ---
	 *    | x |
	 *    |y+2|
	 *
	 * precalc the four offsets from c
	 * */
	o1 = width - 1;
	o2 = width;
	o3 = width + 1;
	o4 = width + width;
	/* Do fire effect on fire screen */
	for (y = 0; y < pixels + 2 * width; y += width) {
		for (x = 1; x < width - 1; x++) {
			pixel = y + x;
			/* Make the multiplicator slightly below 0.25 */
			fireindex =
				(fire[pixel + o1] +
				fire[pixel + o2] +
				fire[pixel + o3] +
				fire[pixel + o4]) * 0.249;
			fire[pixel] = fireindex;
		}
	}
	/* Copy to ImageData screen */
	fire2screen();
}

