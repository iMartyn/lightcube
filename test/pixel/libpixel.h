#ifndef PIXEL_H
#define PIXEL_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void start_data(void);
void end_data(void);
void init_pixel(void);
void out_color(uint8_t r, uint8_t g, uint8_t b);

#ifdef __cplusplus
}
#endif

#endif
