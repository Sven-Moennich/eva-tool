/** @file tools.h
 *
 * @brief A description of the module’s purpose.
 *
 * @par
 * COPYRIGHT NOTICE: (c) 2021 lan4lano.  All rights reserved.
 */

#ifndef EVA_SOCKET_H
#define EVA_SOCKET_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int create_socket(char *ifname, unsigned int port);

#endif /* EVA_TOOLS_H */

/*** end of file ***/
