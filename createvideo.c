#include <stdio.h>
#include <stdlib.h>

/* From fire.c */
unsigned int *init(int w, int h);
void render(double timestamp);
extern unsigned int *screen;
extern unsigned char *fire;
extern unsigned int *palette;

#define WIDTH 1280
#define HEIGHT 720

int main(void)
{
	int i;
	FILE *fp;
	unsigned int *scr;
	char filename[16];

	scr = init(WIDTH, HEIGHT);
	for (i = 0; i < 60 * 30; i++) {
		render(i);
		/* Save frame to disk */
		snprintf(filename, sizeof(filename), "frame%05d.raw", i);
		fp = fopen(filename, "wb");
		if (!fp) {
			fprintf(stderr, "Failed to open %s for writing", filename);
			goto out;
		}
		fwrite(scr, WIDTH * HEIGHT * 4, 1, fp);
		fclose(fp);
	}
	printf("for f in frame*.raw; do convert -size %dx%d -depth 8 RGBA:${f} ${f}.png; done\n", WIDTH, HEIGHT);
	puts("ffmpeg -framerate 30 -pattern_type glob -i 'frame*.png' -c:v libx264 -r 30 -pix_fmt yuv420p out.mp4");
out:
	if (palette)
		free(palette);
	if (fire)
		free(fire);
	if(screen)
		free(screen);

	return 0;
}
