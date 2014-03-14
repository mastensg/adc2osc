#include <err.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <lo/lo.h>

#include "adc.h"
#include "spi.h"

char *paths[16] = 
{
    "/macmini/pot/3/2",
    "/macmini/pot/2/2",
    "/macmini/pot/1/2",
    "/macmini/pot/0/2",
    "/macmini/pot/3/3",
    "/macmini/pot/2/3",
    "/macmini/pot/1/3",
    "/macmini/pot/0/3",
    "/macmini/pot/3/0",
    "/macmini/pot/2/0",
    "/macmini/pot/1/0",
    "/macmini/pot/0/0",
    "/macmini/pot/3/1",
    "/macmini/pot/2/1",
    "/macmini/pot/1/1",
    "/macmini/pot/0/1",
};

int
main(int argc, char *argv[])
{
    uint16_t samples[16];
    int adc0_fd;
    int adc1_fd;
    int i;
    lo_address addr;

    if (2 != argc)
        errx(EXIT_FAILURE, "Usage: %s URL", argv[0]);

    adc0_fd = spi_open_port("/dev/spidev0.0");
    adc1_fd = spi_open_port("/dev/spidev0.1");

    addr = lo_address_new_from_url(argv[1]);

    for (;;)
    {

        for (i = 0; i < 8; ++i)
            samples[i] = adc_sample(adc0_fd, i);

        for (i = 0; i < 8; ++i)
            samples[8 + i] = adc_sample(adc1_fd, i);

        for (i = 0; i < 16; ++i)
            lo_send(addr, paths[i], "i", samples[i]);

        usleep(100000);
    }

    return EXIT_SUCCESS;
}
