#define SCREEN_WIDTH 2560
#define SCREEN_HEIGHT 1664

/* this should be cleared up */
#define SCREEN_SIZE SCREEN_WIDTH * SCREEN_HEIGHT * 4

typedef unsigned char color_t[3];

int fb_close(unsigned char **, int *);
void fb_init(unsigned char **, int *);
void fb_set_pixel(unsigned char **, unsigned int, unsigned int, color_t);
void fb_draw_rect(unsigned char **, unsigned int, unsigned int, unsigned int, 
		unsigned int, color_t);
void fb_draw_line_horizontal(unsigned char **, unsigned int, unsigned int, 
		unsigned int, color_t);
void fb_draw_line_vertical(unsigned char **, unsigned int, unsigned int, 
		unsigned int, color_t);
void fb_draw_line(unsigned char **, unsigned int, unsigned int, unsigned int, 
		unsigned int, color_t);
