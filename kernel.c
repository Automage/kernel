#include <stddef.h>
#include <stdint.h>

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

/* VGA text mode definitions */
#define VGA_BUF_ADDR 0xB8000
#define VGA_HEIGHT 80
#define VGA_WIDTH 25


/* Hardware text mode color constants. */
enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

size_t strlen(const char *str) {
  size_t len = 0;
  while (str[len])
    len++;
  return len;
}

/*
 * VGA Entry
 *
 * Return a 16 bit value representing an entry to the VGA buffer.
 */
static inline uint16_t vga_entry(uint8_t uc, enum vga_color fg, enum vga_color bg) {
  uint16_t color = ((uint16_t) bg << 4) | (uint16_t) fg;
  return (uint16_t) uc | (color << 8);
}

static size_t term_row;
static size_t term_col;
static uint16_t* term_buf;

/*
 * Terminal putc
 *
 * Places a character on the screen given the specified arguments.
 */
void term_putc(uint8_t uc, size_t col, size_t row, 
                enum vga_color fg, enum vga_color bg) {
  // Remove blink at cursor
  //term_set_cursor_blink(0)

  size_t index = (row * VGA_WIDTH) + col;
  term_buf[index] = vga_entry(uc, fg, bg);
 
  // Screen bound checking and incrementing
  if (++term_col == VGA_WIDTH) {
    term_col = 0;
    if (++term_row == VGA_HEIGHT)
      term_row = 0;
  }

  // Add blink at cursor
  //term_set_cursor_blink(1);
}

/*
 * Terminal write
 *
 * Write len bytes of buf into VGA buffer at current cursor position.
 */
void term_write(const char *buf, size_t len) {
  for (size_t i = 0; i < len; i++)
    term_putc(buf[i], term_col, term_row, VGA_COLOR_WHITE, VGA_COLOR_BLACK);
}

/*
 * Terminal write string
 *
 * Write string into VGA buffer at current cursor position.
 */
void term_writestr(const char *str) {
  term_write(str, strlen(str));
}

/*
 * Initialize terminal buffer
 */
void term_init() {
  term_row = 0;
  term_col = 0;
  term_buf = (uint16_t *)VGA_BUF_ADDR;
  for (size_t y = 0; y < VGA_HEIGHT; y++) {
    for (size_t x = 0; x < VGA_WIDTH; x++) {
      term_putc(' ', x, y, VGA_COLOR_WHITE, VGA_COLOR_BLACK);  
    }
  }
}

/*
 * Kernel entry point
 */
void kmain() {
  term_init();
  term_writestr("Alive?");
}
