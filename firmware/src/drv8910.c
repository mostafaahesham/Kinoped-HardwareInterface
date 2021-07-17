#include "drv8910.h"
#include "config/default/peripheral/spi/spi_master/plib_spi_master_common.h"
#include "config/default/peripheral/spi/spi_master/plib_spi3_master.h"

SPI_TRANSFER_SETUP s = {120000000,SPI_CLOCK_PHASE_LEADING_EDGE,SPI_CLOCK_POLARITY_IDLE_LOW,SPI_DATA_BITS_16};
