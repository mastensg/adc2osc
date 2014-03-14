int
spi_open_port(const char *path);

void
spi_write_read(int fd, uint8_t *buf, size_t buflen);
