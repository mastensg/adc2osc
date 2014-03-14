#include <stdint.h>
#include <stdlib.h>

#include "spi.h"

uint16_t
adc_sample(int fd, uint8_t channel)
{
    uint8_t buf[3];

    buf[0] = (1<<4) | (8 | (channel & 7));
    buf[1] = 0;
    buf[2] = 0;

    spi_write_read(fd, buf, 3);

    return (((buf[1] & 63) << 8) | buf[2]) >> 2;
}
