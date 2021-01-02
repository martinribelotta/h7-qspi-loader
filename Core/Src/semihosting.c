/*
 * semihosting.c
 *
 *  Created on: 1 ene. 2021
 *      Author: martin
 */

#include "semihosting.h"

#include <string.h>
#include <stdarg.h>

#include "printf.h"

#ifndef CALL_HOST_INLINE
__attribute__((naked)) int call_host(int reason, void *args)
{
	__asm__ volatile("bkpt 0xAB\n"
			         "bx lr"::: "memory");
}
#endif


void host_echo(const char *s) {
	call_host(SEMIHOSTING_SYS_WRITE0, (void*) s);
}

int host_open(const char *name, int mode) {
	struct {
		const char *name;
		int mode;
		int name_len;
	} params = { name, mode, strlen(name) };
	return call_host(SEMIHOSTING_SYS_OPEN, (void*) &params);
}

int host_flen(int fd)
{
	return call_host(SEMIHOSTING_SYS_FLEN, (void*) &fd);
}


void host_close(int fd) {
	call_host(SEMIHOSTING_SYS_CLOSE, (void*) &fd);
}

int host_read(int fd, void *buf, int len) {
	struct {
		int fd;
		void *buf;
		int len;
	} params = { fd, buf, len };
	return call_host(SEMIHOSTING_SYS_READ, (void*) &params);
}

int host_write(int fd, const void *buf, int len) {
	struct {
		int fd;
		const void *buf;
		int len;
	} params = { fd, buf, len };
	return call_host(SEMIHOSTING_SYS_WRITE, (void*) &params);
}

void host_cmdline(char *buffer, int buflen) {
	struct {
		char *buffer;
		int buflen;
	} params = { buffer, buflen };
	call_host(SEMIHOSTING_SYS_GET_CMDLINE, (void*) &params);
}

int host_printf(const char *fmt, ...)
{
	static char buf[1024];
	va_list va;
	va_start(va, fmt);
	int n = vsnprintf(buf, sizeof(buf), fmt, va);
	va_end(va);
	host_echo(buf);
	return n;
}
