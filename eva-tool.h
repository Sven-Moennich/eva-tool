/** @file eva-tool.h
 *
 * @brief A description of the module’s purpose.
 *
 * @par
 * COPYRIGHT NOTICE: (c) 2021 lan4lano.  All rights reserved.
 */

#ifndef EVA_TOOL_H
#define EVA_TOOL_H


#include <byteswap.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include <arpa/inet.h>

#include <linux/if.h>
#include <linux/in.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <evaftplib.h>

#include "tools.h"
#include "socket.h"


#include "version.h"

#define EVA_ADDRESS         "255.255.255.255"
#define EVA_PORT            5035

#define EVA_PACKET_SIZE     16
#define EVA_PACKET_IP_OFF   8

#define RCV_BUF_SIZE        25

#define IP4_ADDRESS_SIZE    4


#define CMD_BRANDING 0
#define CMD_NO_REBOOT 1
#define CMD_BOOTPARTITION 2
#define CMD_QUOTE 3
#define CMD_FLASH_BACKUP 4
#define CMD_FLASH_UPDATE 5







#endif /* EVA_TOOL_H */

/*** end of file ***/
