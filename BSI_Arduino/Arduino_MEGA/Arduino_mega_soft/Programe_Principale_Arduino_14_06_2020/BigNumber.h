#ifndef _BigNumber_h
#define _BigNumber_h

typedef union BigNumber {
   uint32_t value;
   uint8_t bytes[4];
};

#endif