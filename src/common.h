#ifndef _KERNEL_COMMON_H
#define _KERNEL_COMMON_H

#include "../include/def.h"

/* See: https://wiki.osdev.org/Inline_Assembly/Examples#I.2FO_access */

static inline void port_write( u16 port, u8 value ) {
	__asm__ volatile( "outb %0, %1" : : "a"( value ), "Nd"( port ) );
	/* There's an outb %al, $imm8  encoding, for compile-time constant port
	 * numbers that fit in 8b.  (N constraint). Wider immediate constants
	 * would be truncated at assemble-time (e.g. "i" constraint). The  outb
	 * %al, %dx  encoding is the only option for all other cases.
	 * %1 expands to %dx because  port  is a uint16_t.  %w1 could be used if
	 * we had the port number a wider C type */
}

static inline void port_write_word( u16 port, u16 value ) {
	__asm__ volatile( "outw %0, %1" : : "a"( value ), "Nd"( port ) );
}

static inline u8 port_read( u16 port ) {
	u8 ret;
	__asm__ volatile( "inb %1, %0" : "=a"( ret ) : "Nd"( port ) );
	return ret;
}

#endif // _KERNEL_COMMON_H
