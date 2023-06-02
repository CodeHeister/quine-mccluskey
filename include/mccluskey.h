#ifndef MCCLUSKEY_H_
#define MCCLUSKEY_H_

#include <inttypes.h>

#define GET_BIT(x, y) (((x) >> (y)) & 1)
#define CLR_BIT(x, y) ((x) &= ~((1) << (y)))
#define SET_BIT(x, y) ((x) |= ((1) << (y)))
#define GET_POS(x, y) (GET_BIT(x, y*2) | (GET_BIT(x, y*2+1) << 1))
#define SET_POS(x, y) SET_BIT(x, y*2); CLR_BIT(x, y*2+1)
#define UNSET_POS(x, y) CLR_BIT(x, y*2); CLR_BIT(x, y*2+1)
#define LOCK_POS(x, y) SET_BIT(x, y*2); SET_BIT(x, y*2+1);
#define ROWS 16

#ifdef __cplusplus
extern "C" {
#endif

extern void scanf_minmap(uint16_t *buffer);
extern void print_row(char *rows, int8_t n);
extern void print_rows(char *rows);
extern void clean_rows(char *rows);
extern void set_row(char *rows, int8_t n);
extern void set_rows(char *rows);
extern void int2rows(char *rows, const uint16_t *buffer);
extern void minimalise(char *rows, const uint16_t *buffer);

#ifdef __cplusplus
}
#endif

#endif
