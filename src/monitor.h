// monitor.h -- Defines the interface for monitor.h
// From JamesM's kernel development tutorials.

#ifndef MONITOR_H
#define MONITOR_H

#include "common.h"

#define COLUMNS           80
#define ROWS              25
#define VIDEO_MEMORY_SIZE ( COLUMNS * ROWS )

typedef enum VGA_COLORS {
	VGA_BLACK = 0,
	VGA_BLUE,
	VGA_GREEN,
	VGA_CYAN,
	VGA_RED,
	VGA_PURPLE,
	VGA_BROWN,
	VGA_GRAY,
	VGA_DARK_GRAY,
	VGA_LIGHT_BLUE,
	VGA_LIGHT_GREEN,
	VGA_LIGHT_CYAN,
	VGA_LIGHT_PURPLE,
	VGA_YELLOW,
	VGA_WHITE,
} VGA_COLORS;

// Write a single character out to the screen.
void monitor_put( char c );

// Clear the screen to all black.
void monitor_clear();

// Output a null-terminated ASCII string to the monitor.
void monitor_write( const char *c );

#endif // MONITOR_H
