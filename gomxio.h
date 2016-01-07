/*
 * Copyright (C) TinyFactory inc. All 
 *
 * 2015 Dec 21 Gautam Dey
 *
 */

#ifndef _GOMXIO_H
#define _GOMXIO_H

#include "stdlib.h"
#include "stdio.h"
#include "mxio.h"


// Init the system so that we can get driver.
int initIF(WORD *wIFCount);
char *getIFInfo(WORD *wIFCount, int *err);
char *selectIF(DWORD index, int* err);
MODULE_LIST *autoSearch(int retry, int timeout, int deviceType,int IFidx, char *IFInfo, int *deviceCount, int *err ); 
#endif
