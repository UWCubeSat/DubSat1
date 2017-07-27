#ifndef test_spi_H
#define test_spi_H

void spiInit(uint16_t csPins);
void spiTransceive(uint8_t *pTxBuf, uint8_t *pRxBuf, size_t num, uint16_t csPin);
#endif
