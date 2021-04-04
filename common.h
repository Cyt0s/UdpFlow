//
// Created by root on 12/30/18.
//

#ifndef UDPFLOW_COMMON_H
#define UDPFLOW_COMMON_H

/**
 * @file    common.h
 * @brief   Common definitions for the project.
 * @author  ez
 * @date    16/12/18
*/
/** Headers ********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include "errors.h"
#include <string.h>
#include <ctype.h>

/** Macros *********************************************************/

/** Invalid value for a file descriptor. */
#define INVALID_FD                                              (-1)

/**
 * @brief   Free a valid (=not NULL) pointer and nullify it.
*/
#define FREE(ptr)           \
    if (NULL != (ptr)) {    \
        free(ptr);          \
        (ptr) = NULL;       \
    }

/**
 * @brief   Close a valid (=not INVALID_FD) file descriptor and set it
 *          to INVALID_FD.
*/
#define CLOSE(fd)                   \
    if (INVALID_FD != (fd)) {       \
        (void)close(fd);            \
        (fd) = INVALID_FD;          \
    }

/**
 * @brief   Print a formatted message with debugging information.
 *
 * @note    Usually this is defined inside an ifdef of _DEBUG (so on release
 *          version this define would be empty) and contains stuff like line
 *          number and file name, but this is just an example and I'm lazy :p
*/
#define DEBUG_PRINT(...)                \
    (void)printf(__VA_ARGS__)


#endif //UDPFLOW_COMMON_H
