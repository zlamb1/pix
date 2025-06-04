#ifndef PIX_ERROR_H
#define PIX_ERROR_H 1

#include "pix_type.h"

#define PIX_SUCCESS              0
#define PIX_ERR_ALLOC           -1
#define PIX_ERR_INVALID_ARG     -2
#define PIX_ERR_NOT_FOUND       -3
#define PIX_ERR_BUF_EMPTY       -4
#define PIX_ERR_BUF_FULL        -5
#define PIX_ERR_TIMEOUT         -6
#define PIX_ERR_LOCK            -7
#define PIX_ERR_RELEASE         -8
#define PIX_ERR_INVALID_THREAD  -9

#define PIX_ERR_UNSUPPORTED     -126
#define PIX_ERR_PLATFORM        -127
#define PIX_ERR_UNKNOWN         -128

#endif
