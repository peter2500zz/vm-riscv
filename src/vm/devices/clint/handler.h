#ifndef CLINT_HANDLER_H
#define CLINT_HANDLER_H
#include <stdint.h>

extern uint32_t msip;
extern uint64_t mtime;
extern uint64_t mtimecmp;

int readMSIP(void *target);

int writeMSIP(void *target);

int readMTIMECMPlo(void *target);

int writeMTIMECMPlo(void *target);

int readMTIMECMPhi(void *target);

int writeMTIMECMPhi(void *target);

int readMTIMElo(void *target);

int writeMTIMElo(void *target);

int readMTIMEhi(void *target);

int writeMTIMEhi(void *target);

#endif // CLINT_HANDLER_H
