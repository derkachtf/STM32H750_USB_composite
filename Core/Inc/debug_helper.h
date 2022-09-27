

#ifndef __DEBUG_HELPER_H
#define __DEBUG_HELPER_H
#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
//#include <strings.h>

#define ABS(x) ((x) > 0 ? (x) : -(x))

void printBits(size_t const size, void const * const ptr);
void DebugThread(void);
void printMass8bit(const uint8_t *buff, uint32_t count);
void printkeymap(void);
void IsrPrintDbug(void);
int _write(int fd, char* ptr, int len);
void OSprint(void);
void InitOSprint(void);

#ifdef __cplusplus
}
#endif
#endif  // __DEBUG_HELPER_H