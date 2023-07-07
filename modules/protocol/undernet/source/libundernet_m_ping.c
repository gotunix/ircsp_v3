/* ---------------------------------------------------------------------------
 * Copyright (c) GOTUNIX Networks
 * Copyright (c) GOTUNIXCODE
 * Copyright (c) Justin Ovens
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ---------------------------------------------------------------------------
 */

/* Standard Includes */
#include	<stdio.h>
#include	<stdlib.h>
#include	<stdint.h>
#include	<stdarg.h>
#include	<assert.h>
#include	<unistd.h>
#include	<string.h>
#include	<time.h>
#include	<sys/time.h>
#include	<ctype.h>


/* IRCSP Core Includes */
#include	"ircsp.h"
#include	"log.h"
#include	"alloc.h"
#include	"conf.h"
#include	"socket.h"
#include	"users.h"
#include	"servers.h"
#include	"channels.h"
#include	"modules.h"


/* IRCSP Module Includes */
#include	"libundernet.h"
#include	"libundernet_parser.h"
#include	"libundernet_m_ping.h"


/* Debug */
#include	"debug.h"


/* Compiler Generated Includes */
#include	"config.h"


/*
---------------------------------------------------------------------------------------------------------------------------------
 FUNCTION       |    libundernet_m_ping ()
                |
 DESCRIPTION    |    Undernet Protocol (m_ping)
                |    This function will handle Server Message: PING [G]
                |
                |        AC G !1236971750.709779 UWorld.GOTUNIX.NET 1236971750.709779
                |        @(#) - (socket.c:168) ssprintf():  Socket(SEND): AE Z AC AC -32768 4646262703.916172
                |
                |    We need to reploy with the following:
                |        <my_numeric> Z <remote_numeric> !<remotets> <difference> <localts>
                |
 INPUTS         |    sBuf   = Socket Buffer
                |
 RETURNS        |    TRUE
---------------------------------------------------------------------------------------------------------------------------------
*/
int libundernet_m_ping (sBuf)
	char		*sBuf;
{
	if (DEBUG)
	{
		ircsp_log (F_MAINLOG, get_log_message (LOG_MESSAGE_FUNCSTART),
			__FILE__, __LINE__, __FUNCTION__);
	}


	struct timeval		orig;
	struct timeval		now;
	char			*c_delim;
	char			*argv[4];
	int			i_diff;


	assert (sBuf != NULL);


	argv[0] = strtok (sBuf, " ");			/* Remote Numeric */
	strtok (NULL, " ");				/* Server Message: PING [G] - Ignored */
	argv[1] = strtok (NULL, " ");			/* Remote TS with '!' */
	strtok (NULL, " ");				/* Our Server Numeric */
	argv[2] = strtok (NULL, " ");			/* Remote TS without ! (orig_ts) */


	orig.tv_sec = strtoul(argv[2], &c_delim, 10);
	orig.tv_usec = (*c_delim == '.') ? strtoul(c_delim + 1, NULL, 10) : 0;
	gettimeofday(&now, NULL);
	i_diff = (now.tv_sec - orig.tv_sec) * 1000 + (now.tv_usec - orig.tv_usec) / 1000;


	ssprintf (s,
		"%s Z %s %s %d %d.%d\n",
		config->server_numeric, argv[0], argv[1], i_diff, now.tv_sec,
		(unsigned)now.tv_usec);


	if (DEBUG)
	{
		ircsp_log (F_MAINLOG, get_log_message (LOG_MESSAGE_FUNCEND),
			__FILE__, __LINE__, __FUNCTION__);
	}


	return TRUE;
}
