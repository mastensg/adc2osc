#include <stdint.h>
#include <stdlib.h>
#include <err.h>
#include <fcntl.h>
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>

#include "spi.h"

static uint8_t spi_mode = SPI_MODE_0;
static uint8_t spi_bits_per_word = 8;
static uint32_t spi_speed = 1000000;

int
spi_open_port(const char *path)
{
    int fd;

    if (-1 == (fd = open(path, O_RDWR)))
        err(EXIT_FAILURE, "couldn't open spi port %s", path);

    if (-1 == ioctl(fd, SPI_IOC_WR_MODE, &spi_mode))
        err(EXIT_FAILURE, "couldn't set spi write mode");

    if (-1 == ioctl(fd, SPI_IOC_RD_MODE, &spi_mode))
        err(EXIT_FAILURE, "couldn't set spi read mode");

    if (-1 == ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &spi_bits_per_word))
        err(EXIT_FAILURE, "couldn't set spi write bits per word");

    if (-1 == ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &spi_bits_per_word))
        err(EXIT_FAILURE, "couldn't set spi read bits per word");

    if (-1 == ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &spi_speed))
        err(EXIT_FAILURE, "couldn't set spi write speed");

    if (-1 == ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &spi_speed))
        err(EXIT_FAILURE, "couldn't set spi read speed");

    return fd;
}

void
spi_write_read(int fd, uint8_t *buf, size_t buflen)
{
    struct spi_ioc_transfer spi[buflen];
    int i;

    for (i = 0; i < 3; ++i)
    {
        spi[i].tx_buf = (uint32_t)(buf + i);
        spi[i].rx_buf = (uint32_t)(buf + i);
        spi[i].len = 1;
        spi[i].delay_usecs = 2;
        spi[i].speed_hz = spi_speed;
        spi[i].bits_per_word = spi_bits_per_word;
        spi[i].cs_change = 0;
    }

    if (-1 == ioctl(fd, SPI_IOC_MESSAGE(buflen), &spi))
        err(EXIT_FAILURE, "couldn't send/receive spi message");

    //printf("response: 0x%02x%02x\n", buf[1]&127, buf[2]);
}
