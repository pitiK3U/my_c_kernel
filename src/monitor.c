#include "monitor.h"

volatile u16 *const video_memory = (u16 *)0xb8000; // video mem begins here.
struct _cursor_struct {
	u8 x;
	u8 y;
} cursor = { .x = 0, .y = 0 };

static u16 vga_char( char c, u8 background_color, u8 foreground_color ) {
	// The attribute byte is made up of two nibbles - the lower being the
	// foreground colour, and the upper the background colour.
	u8 attributeByte =
	    (u8)( ( background_color << 4 ) | ( foreground_color & 0x0f ) );
	// The attribute byte is the top 8 bits of the word we have to send to
	// the VGA board;
	u16 character = (u16)( c | ( attributeByte << 8 ) );

	return character;
}

// Updates the hardware cursor.
static void move_cursor() {
	u16 cursorLocation = cursor.y * COLUMNS + cursor.x;

	port_write(
	    0x3D4,
	    14 ); // Tell the VGA board we are setting the high cursor byte.
	port_write( 0x3D5, cursorLocation >> 8 ); // Send the high cursor byte.
	port_write(
	    0x3D4,
	    15 ); // Tell the VGA board we are setting the low cursor byte.
	port_write( 0x3D5, (u8)cursorLocation ); // Send the low cursor byte.
}

// Scrolls the text on the screen up by one line.
static void scroll() {
	// Get a space character with the default colour attributes.
	u16 blank = vga_char( ' ', VGA_BLACK, VGA_WHITE );

	// Row 25 is the end, this means we need to scroll up
	if ( cursor.y >= ROWS ) {
		// Move the current text chunk that makes up the screen
		// back in the buffer by a line
		int i;
		for ( i = 0 * COLUMNS; i < ( ROWS - 1 ) * COLUMNS; i++ ) {
			video_memory[i] = video_memory[i + COLUMNS];
		}

		// The last line should now be blank. Do this by writing
		// 80 spaces to it.
		for ( i = ( ROWS - 1 ) * COLUMNS; i < VIDEO_MEMORY_SIZE; i++ ) {
			video_memory[i] = blank;
		}
		// The cursor should now be on the last line.
		cursor.y = ROWS - 1;
	}
}

static int isprint( const char ch ) { return 32 <= ch && ch <= 126; }

void monitor_put( char c ) {
	u8            background_color = VGA_BLACK;
	u8            foreground_color = VGA_WHITE;
	volatile u16 *location;

	switch ( c ) {
	case '\n':
		cursor.x = 0;
		cursor.y++;
		break;
	case '\r':
		cursor.x = 0;
		break;
	case '\b':
		cursor.x--;
		break;
	case '\t':
		// Handle a tab by increasing the cursor's X, but only to a
		// point where it is divisible by 8.
		cursor.x = ( cursor.x + 8 ) & ~( 8 - 1 );
	}

	if ( isprint( c ) ) {
		location  = &video_memory[cursor.y * COLUMNS + cursor.x];
		*location = vga_char( c, background_color, foreground_color );
		cursor.x++;
	}

	// Check if we need to insert a new line because we have reached the end
	// of the screen.
	if ( cursor.x >= COLUMNS ) {
		cursor.x = 0;
		cursor.y++;
	}

	// Scroll the screen if needed.
	scroll();
	// Move the hardware cursor.
	move_cursor();
}

// Clears the screen, by copying lots of spaces to the framebuffer.
void monitor_clear() {
	// Make an attribute byte for the default colours
	u16 blank = vga_char( ' ', VGA_BLACK, VGA_WHITE );

	for ( int i = 0; i < VIDEO_MEMORY_SIZE; i++ ) {
		video_memory[i] = blank;
	}

	// Move the hardware cursor back to the start.
	cursor.x = 0;
	cursor.y = 0;
	move_cursor();
}

// Outputs a null-terminated ASCII string to the monitor.
void monitor_write( const char *c ) {
	int i = 0;
	while ( c[i] ) {
		monitor_put( c[i++] );
	}
}
