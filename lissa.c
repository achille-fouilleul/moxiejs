#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#ifndef M_PI
# define M_PI 0x1.921fb54442d18p+1
#endif

#define WIDTH 640
#define HEIGHT 480

#define FBADDR ((void *)0x1000000)

int fbwrite(void *addr, int width, int height);
int msleep(int value);

static void clear(uint16_t value)
{
	uint16_t *p = (uint16_t *)FBADDR;
	for (int y = 0; y < HEIGHT; ++y) {
		for (int x = 0; x < WIDTH; ++x)
			*p++ = value;
	}
}

static void putpixel(int x, int y, uint16_t value)
{
	*((uint16_t *)FBADDR + y * WIDTH + x) = value;
}

static int blendchannel(int srcRGB, int srcA, int dstRGB)
{
	return (srcRGB * srcA + dstRGB * (255 - srcA)) / 255;
}

static void blendpixel(int x, int y, uint16_t value, uint8_t alpha)
{
	if (alpha == 0)
		return;
	if (alpha == 0xff) {
		putpixel(x, y, value);
		return;
	}
	uint16_t *p = (uint16_t *)FBADDR + y * WIDTH + x;
	int r0 = *p >> 11;
	int g0 = (*p >> 5) & 0x3f;
	int b0 = *p & 0x1f;
	int r1 = value >> 11;
	int g1 = (value >> 5) & 0x3f;
	int b1 = value & 0x1f;
	int r2 = blendchannel(r1, alpha, r0);
	int g2 = blendchannel(g1, alpha, g0);
	int b2 = blendchannel(b1, alpha, b0);
	*p = (r2 << 11) | (g2 << 5) | b2;
}

static void hline(int x0, int y, int x1, uint16_t value)
{
	if (x0 > x1) {
		int t = x0; x0 = x1; x1 = t;
	}
	for (int x = x0; x <= x1; ++x)
		putpixel(x, y, value);
}

static void vline(int x, int y0, int y1, uint16_t value)
{
	if (y0 > y1) {
		int t = y0; y0 = y1; y1 = t;
	}
	for (int y = y0; y <= y1; ++y)
		putpixel(x, y, value);
}

static void line(int x0, int y0, int x1, int y1, uint16_t value)
{
	int dx = x1 - x0;
	if (dx == 0) {
		vline(x0, y0, y1, value);
		return;
	}
	int dy = y1 - y0;
	if (dy == 0) {
		hline(x0, y0, x1, value);
		return;
	}
	int abs_dx = dx >= 0 ? dx : -dx;
	int abs_dy = dy >= 0 ? dy : -dy;
	if (abs_dy >= abs_dx) {
		if (dy < 0) {
			int t;
			t = y0; y0 = y1; y1 = t;
			t = x0; x0 = x1; x1 = t;
		}
		for (int y = y0; y <= y1; ++y) {
			int u = (x1 - x0) * (y - y0);
			int v = y1 - y0;
			int x = x0 + u / v;
			int w = u % v;
			if (w == 0)
				putpixel(x, y, value);
			else {
				int alpha = 255 * (w >= 0 ? w : -w) / v;
				blendpixel(x, y, value, 255 - alpha);
				if (w >= 0)
					++x;
				else
					--x;
				blendpixel(x, y, value, alpha);
			}
		}
	} else {
		if (dx < 0) {
			int t;
			t = y0; y0 = y1; y1 = t;
			t = x0; x0 = x1; x1 = t;
		}
		for (int x = x0; x <= x1; ++x) {
			int u = (y1 - y0) * (x - x0);
			int v = x1 - x0;
			int y = y0 + u / v;
			int w = u % v;
			if (w == 0)
				putpixel(x, y, value);
			else {
				int alpha = 255 * (w >= 0 ? w : -w) / v;
				blendpixel(x, y, value, 255 - alpha);
				if (w >= 0)
					++y;
				else
					--y;
				blendpixel(x, y, value, alpha);
			}
		}
	}
}

static int gcd(int a, int b)
{
	if (b == 0)
		return a;
	return gcd(b, a % b);
}

int main(int argc, char *argv[])
{
	int a = 1, b = 1;
	if (argc == 3) {
		a = strtol(argv[1], NULL, 0);
		b = strtol(argv[2], NULL, 0);
	}

	const int xc = WIDTH / 2;
	const int yc = HEIGHT / 2;
	const int R = 200;

	int n = a * b / gcd(a, b);

	for (int c = 0; c < 360; c += 15) {
		clear(0xffff);

		line(0, 0, WIDTH - 1, HEIGHT - 1, 0xf800);
		line(0, HEIGHT - 1, WIDTH - 1, 0, 0x001f);
		hline(0, 0, WIDTH - 1, 0x07e0);
		hline(0, HEIGHT - 1, WIDTH - 1, 0x07e0);
		vline(0, 0, HEIGHT - 1, 0x07e0);
		vline(WIDTH - 1, 0, HEIGHT - 1, 0x07e0);

		double ph = 2 * M_PI * c / 360;

		int x0 = xc + R * cos(ph);
		int y0 = yc + R * sin(0.0);
		for (int i = 1; i <= 90 * n; ++i) {
			double th = M_PI * i / (45 * n);
			int x1 = xc + R * cos(a * th + ph);
			int y1 = yc + R * sin(b * th);
			line(x0, y0, x1, y1, 0);
			x0 = x1;
			y0 = y1;
		}

		fbwrite(FBADDR, WIDTH, HEIGHT);
		msleep(50);
	}
	return 0;
}
