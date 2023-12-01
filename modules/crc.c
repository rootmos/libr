#define CRC16_CCITT_POLY 0x1021

// https://en.wikipedia.org/wiki/Computation_of_cyclic_redundancy_checks#Bit_ordering_(endianness)
API uint16_t LIBR(crc16_ccitt)(const void* msg, size_t l)
{
    uint16_t crc = 0x0000;
    for(size_t i = 0; i < l; i++) {
        crc ^= ((unsigned char*)msg)[i] << 8;
        for(size_t j = 0; j < 8; j++) {
            if(crc & 0x8000) {
                crc = (crc << 1) ^ CRC16_CCITT_POLY;
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}
