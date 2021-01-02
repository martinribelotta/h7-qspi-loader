/*
 * semihosting.h
 *
 *  Created on: 1 ene. 2021
 *      Author: martin
 */

#ifndef INC_SEMIHOSTING_H_
#define INC_SEMIHOSTING_H_

enum {
    SEMIHOSTING_EnterSVC = 0x17,
    SEMIHOSTING_ReportException = 0x18,
    SEMIHOSTING_SYS_CLOSE = 0x02,
    SEMIHOSTING_SYS_CLOCK = 0x10,
    SEMIHOSTING_SYS_ELAPSED = 0x30,
    SEMIHOSTING_SYS_ERRNO = 0x13,
    SEMIHOSTING_SYS_FLEN = 0x0C,
    SEMIHOSTING_SYS_GET_CMDLINE = 0x15,
    SEMIHOSTING_SYS_HEAPINFO = 0x16,
    SEMIHOSTING_SYS_ISERROR = 0x08,
    SEMIHOSTING_SYS_ISTTY = 0x09,
    SEMIHOSTING_SYS_OPEN = 0x01,
    SEMIHOSTING_SYS_READ = 0x06,
    SEMIHOSTING_SYS_READC = 0x07,
    SEMIHOSTING_SYS_REMOVE = 0x0E,
    SEMIHOSTING_SYS_RENAME = 0x0F,
    SEMIHOSTING_SYS_SEEK = 0x0A,
    SEMIHOSTING_SYS_SYSTEM = 0x12,
    SEMIHOSTING_SYS_TICKFREQ = 0x31,
    SEMIHOSTING_SYS_TIME = 0x11,
    SEMIHOSTING_SYS_TMPNAM = 0x0D,
    SEMIHOSTING_SYS_WRITE = 0x05,
    SEMIHOSTING_SYS_WRITEC = 0x03,
    SEMIHOSTING_SYS_WRITE0 = 0x04,
};

enum {
	SYS_OPEN_R = 0,
	SYS_OPEN_RB = 1,
	SYS_OPEN_RP = 2,
	SYS_OPEN_RPB = 3,
	SYS_OPEN_W = 4,
	SYS_OPEN_WB = 5,
	SYS_OPEN_WP = 6,
	SYS_OPEN_WPB = 7,
	SYS_OPEN_A = 8,
	SYS_OPEN_AB = 9,
	SYS_OPEN_AP = 10,
	SYS_OPEN_APB = 11,
};

#if 0
#define CALL_HOST_INLINE
static inline int __attribute__((always_inline)) call_host(int reason, void *arg)
{
    int value;
    __asm__ volatile(" mov r0, %[rsn]  \n"
    		         " mov r1, %[arg]  \n"
                     " bkpt %[swi]\n"
                     " mov %[val], r0"
                     : [ val ] "=r"(value)
                     : [ rsn ] "r"(reason), [ arg ] "r"(arg), [ swi ] "i"(0xAB)
                     : "memory");

    // Accordingly to page 13-77 of ARM DUI 0040D other registers
    // can also be clobbered. Some memory positions may also be
    // changed by a system call, so they should not be kept in
    // registers. Note: we are assuming the manual is right and
    // Angel is respecting the APCS.
    return value;
}
#else
#undef CALL_HOST_INLINE
extern __attribute__((naked)) int call_host(int reason, void *args);
#endif

extern void host_echo(const char *s);
extern int host_open(const char *name, int mode);
extern int host_flen(int fd);
extern void host_close(int fd);
extern int host_read(int fd, void *buf, int len);
extern int host_write(int fd, const void *buf, int len);
extern void host_cmdline(char *buffer, int buflen);
extern int host_printf(const char *fmt, ...);

static inline void host_halt(int reason)
{
	(void) reason;
	__asm__ volatile("bkpt #0");
}

#endif /* INC_SEMIHOSTING_H_ */
