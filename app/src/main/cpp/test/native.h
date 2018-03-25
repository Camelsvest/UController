#ifndef UCONTROLLER_NATIVE_H
#define UCONTROLLER_NATIVE_H
#include "hrbbus.h"
int ucontroller_HRBBus_init(void);
void ucontroller_HRBBus_uninit(void);
int ucontroller_HRBBUus_sendMsg(node_t obj, unsigned short int status, unsigned short int dest_address);

#endif

