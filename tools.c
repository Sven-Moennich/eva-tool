/** @file tools.c
 *
 * @brief A description of the module’s purpose.
 *
 * @par
 * COPYRIGHT NOTICE: (c) 2021 lan4lano.  All rights reserved.
 */

#include "tools.h"



void reverse(char *input, char *output, size_t len) {
	for (unsigned long int i = 0; i < len; i++) {
		output[i] = input[len - 1 - i];
	}
}
