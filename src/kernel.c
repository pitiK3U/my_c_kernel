/*
 *  kernel.c
 */

//#include <assert.h>

#include "../include/def.h"
#include "common.h"
#include "monitor.h"

static inline size_t strlen( const char *str ) {
	const char *new = str;
	while ( *new != '\0' ) {
		++new;
	}

	return (size_t)( new - str );
}

// `sizeof(buf) >= (20 + 1)`
void utoa( size_t input, char *buf ) {
	// FIXME: value > 14 makes the kernel to reboot
	u8 i     = 14u;
	buf[--i] = '\0';

	while ( input > 0 && i > 0 ) {
		buf[--i] = (char)( ( input % 10 ) + '0' );
		input /= 10;
	}

	while ( i > 0u ) {
		buf[--i] = '0';
	}
}

void kmain( void ) {
	monitor_clear();

	const char *str = "\n !\"#$%&'()*+,-./0123456789:;<=>?@ "
			  "ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`"
			  "abcdefghijklmnopqrstuvwxyzabcdef{|}~\n";
	monitor_write( str );

	char timestamp[32];
	timestamp[31] = '\0';
	size_t max    = (size_t)SIZE_MAX;
	utoa( max, timestamp );
	monitor_write( timestamp );

	while ( 1 ) {}

	return;
}
