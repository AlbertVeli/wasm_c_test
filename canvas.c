#include <stdlib.h>

int *data = NULL;

int width = 0;
int height = 0;
int pixels = 0;

/* Return pixel data buffer */
int *init(int w, int h)
{
	width = w;
	height = h;
	pixels = width * height;
	data = malloc(pixels * 4);
	return data;
}

void render(double timestamp)
{
	int x, y;
	int gray = (int)timestamp & 0xff;
	/*           alpha          blue           green         red     */
	int pixel = (0xff << 24) + (gray << 16) + (gray << 8) + (gray << 0);

	/* Just draw some gray pixels. When pixels show up here
	 * we know it works. Then anything is possible. */
	for (y = 0; y < pixels; y += height) {
		for (x = 0; x < width; x++) {
			data[y + x] = pixel;
		}
	}
}

