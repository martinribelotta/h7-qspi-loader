/*
 * burner.c
 *
 *  Created on: 1 ene. 2021
 *      Author: martin
 */
#include "quadspi.h"
#include "qspi.h"
#include "semihosting.h"
#include "printf.h"
#include "burner.h"

#include <string.h>

static uint8_t membuffer[65536];
static uint8_t checkbuf[65536];

static void die(const char *msg)
{
	host_echo(msg);
	host_halt(-1);
}

static void qspi_check(const char *msg, uint8_t ret)
{
	if (ret != QSPI_OK) {
		die(msg);
	}
}

#define QSPI_CHECK(expr) do { \
	qspi_check(#expr, expr); \
} while(0)

static void PerformErasePage(unsigned int addr)
{
	host_printf("Erasing at 0x%08X\n", addr);
	QSPI_CHECK(QSPI_WriteEnable(&hqspi));
	QSPI_CHECK(QSPI_EraseBlock(&hqspi, addr));
	host_halt(0);
}

static void PerformFlashCopy(unsigned int flashaddr, unsigned int size)
{
	host_printf("Writing at 0x%08X\n", flashaddr);
	for (unsigned int writed = 0; writed < size; writed += 256) {
		QSPI_CHECK(QSPI_WriteEnable(&hqspi));
		QSPI_CHECK(QSPI_Write(&hqspi, membuffer + writed, flashaddr + writed, 256));
	}
	host_halt(0);
}

static void PerformFlashVerify(unsigned int flashaddr, unsigned int size)
{
	host_printf("Verifing at 0x%08X\n", flashaddr);
	QSPI_CHECK(QSPI_Read(&hqspi, checkbuf, flashaddr, size));
	if (memcmp(checkbuf, membuffer, size) != 0) {
		die("VERIFY FAIL\n");
	}
	host_halt(0);
}

const FlashAlgorithm_t FlashAlgo = {
	.MemBuffer = membuffer,
	.MemBufferSize = sizeof(membuffer),
	.ErasePage = PerformErasePage,
	.FlashCopy = PerformFlashCopy,
	.FlashVerify = PerformFlashVerify,
};

#if 0
Parameters_t ParamBlock __attribute__((section(".no_init")));

void hexdump(const uint8_t *p, size_t n) {
	static char linebuf[128];
	char *ptr = linebuf;
	*ptr = 0;
	int remain = sizeof(linebuf);
	for (size_t i = 0; i < n; i++) {
		if ((i % 16) == 0) {
			host_echo(linebuf);
			int writed = snprintf(linebuf, remain, "\n%04X: ", i);
			remain = sizeof(linebuf) - writed;
			ptr = linebuf + writed;
		}
		int writed = snprintf(ptr, remain, "%02X ", p[i]);
		remain -= writed;
		ptr += writed;
	}
	strcat(linebuf, "\n");
	host_echo(linebuf);
}

static void die(const char *msg)
{
	host_echo(msg);
	host_halt(-1);
}

static void qspi_check(const char *msg, uint8_t ret)
{
	if (ret != QSPI_OK) {
		die(msg);
	}
}

#define QSPI_CHECK(expr) do { \
	qspi_check(#expr, expr); \
} while(0)

void burner(void) {
	static char filename[128];

	host_printf("Loader builded at " __DATE__ " " __TIME__ "\n");
	host_cmdline(filename, sizeof(filename));
	int fd = host_open(filename, SYS_OPEN_RB);
	if (fd == -1) {
		die("Error opening file\n");
	}
#define CHUNK 4096
#define PAGE_WRITE 256
	uint32_t addr = ParamBlock.address;
	int len = host_flen(fd);
	if (len == -1) {
		die("Cannot determine file size\n");
	}
	int readed = 0;
	host_printf("Writing %d bytes from %s to 0x%08X\n", len, filename, addr);
	while (readed < len) {
		static uint8_t buf[CHUNK];
		static uint8_t rbuf[CHUNK];
		memset(buf, 0xFF, sizeof(buf));
		int r = host_read(fd, buf, sizeof(buf));
		if (r == -1) {
			die("Error reading chunk\n");
		}
		if (r == sizeof(buf)) {
			host_echo("EOF\n");
			break;
		}
		host_echo("E");
		QSPI_CHECK(QSPI_WriteEnable(&hqspi));
		QSPI_CHECK(QSPI_EraseSector(&hqspi, addr));
		for (int off = 0; off < CHUNK; off += PAGE_WRITE) {
			host_echo("W");
			QSPI_CHECK(QSPI_WriteEnable(&hqspi));
			QSPI_CHECK(QSPI_Write(&hqspi, buf + off, addr + off, PAGE_WRITE));
		}
		host_echo("R");
		QSPI_CHECK(QSPI_Read(&hqspi, rbuf, addr, sizeof(rbuf)));
		if (memcmp(rbuf, buf, sizeof(buf)) != 0) {
			die("!!!\nFailed verifing memory\n");
		}
		addr += CHUNK;
		readed += CHUNK;
		if (readed >= len) {
			host_echo(" EOF\n");
		} else {
			host_printf(" remain %d\n", len - readed);
		}
	}

	host_halt(0);
}
#endif
