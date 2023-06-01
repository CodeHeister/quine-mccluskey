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
#define LEN 32

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NumSet {
	char num;
	char refers[8];
} NumSet_t;

extern int can_combine(char m1, char m2);
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
