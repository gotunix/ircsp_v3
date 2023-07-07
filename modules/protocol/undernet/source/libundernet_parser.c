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
#include	<unistd.h>
#include	<string.h>
#include	<assert.h>
#include	<time.h>


/* IRCSP Core Includes */
#include	"conf.h"
#include	"alloc.h"
#include	"log.h"
#include	"ircsp.h"
#include	"text.h"
#include	"modules.h"


/* IRCSP Module Includes */
#include	"libundernet.h"
#include	"libundernet_m_burst.h"
#include	"libundernet_m_create.h"
#include	"libundernet_m_destruct.h"
#include	"libundernet_m_eob.h"
#include	"libundernet_m_info.h"
#include	"libundernet_m_join.h"
#include	"libundernet_m_kill.h"
#include	"libundernet_m_mode.h"
#include	"libundernet_m_motd.h"
#include	"libundernet_m_nick.h"
#include	"libundernet_m_part.h"
#include	"libundernet_m_ping.h"
#include	"libundernet_m_privmsg.h"
#include	"libundernet_m_quit.h"
#include	"libundernet_m_server.h"
#include	"libundernet_m_squit.h"
#include	"libundernet_m_stats.h"
#include	"libundernet_m_version.h"
#include	"libundernet_parser.h"


/* Debug */
#include	"debug.h"


/* Compiler Generated Includes */
#include	"config.h"


/*
---------------------------------------------------------------------------------------------------------------------------------
 Server Messages


   Message, Function
---------------------------------------------------------------------------------------------------------------------------------
*/
struct _server_messages Server_Messages[] = {
	{	"B",		&libundernet_m_burst,		},		/* [B]  - BURST */
	{	"C",		&libundernet_m_create,		},		/* [C]  - CREATE */
	{	"D",		&libundernet_m_kill,		},		/* [D]  - KILL */
	{	"DE",		&libundernet_m_destruct,	},		/* [DE] - DESTRUCT */
	{	"EA",		&libundernet_m_eob_ack,		},		/* [EA] - END_OF_BURST_ACK */
	{	"EB",		&libundernet_m_eob,		},		/* [EB] - END_OF_BURST */
	{	"F",		&libundernet_m_info,		},		/* [F]  - INFO */
	{	"G",		&libundernet_m_ping,		},		/* [G]  - PING */
	{	"J",		&libundernet_m_join,		},		/* [J]  - JOIN */
	{	"L",		&libundernet_m_part,		},		/* [L]  - PART */
	{	"M",		&libundernet_m_mode,		},		/* [M]  - MODE */
	{	"N",		&libundernet_m_nick,		},		/* [N]  - NICK */
	{	"P",		&libundernet_m_privmsg,		},		/* [P]  - PRIVMSG */
	{	"Q",		&libundernet_m_quit,		},		/* [Q]  - QUIT */
	{	"R",		&libundernet_m_stats,		},		/* [R]  - STATS */
	{	"S",		&libundernet_m_server,		},		/* [S]  - SERVER */
	{	"SERVER",	&libundernet_m_server,		},		/* [S]  - SERVER */
	{	"SQ",		&libundernet_m_squit,		},		/* [SQ] - SQUIT */
	{	"V",		&libundernet_m_version,		},		/* [V]  - VERSION */
};


/*
---------------------------------------------------------------------------------------------------------------------------------
 FUNCTION       |    libundernet_parser ()
                |
 DESCRIPTION    |    Undernet Protocol (parser)
                |    This function will handle parsing the socket buffer and forwarding commands to their specific
                |    parsers.
                |
 INPUTS         |    sBuf  = Socket Buffer
                |
 RETURNS        |    NONE
---------------------------------------------------------------------------------------------------------------------------------
*/
void libundernet_parser (sBuf)
	char		*sBuf;		/* Socket Buffer */
{
	if (DEBUG)
	{
		ircsp_log (F_MAINLOG, get_log_message (LOG_MESSAGE_FUNCSTART),
			__FILE__, __LINE__, __FUNCTION__);
	}


	char		*c_parse;
	char		*argv[2];
	int		i_cmd;


	assert (sBuf != NULL);

	c_parse = (char *)ircsp_calloc (1, strlen (sBuf) + 20);
	if (!c_parse)
	{
		ircsp_log (F_MAINLOG, get_log_message (LOG_MESSAGE_OUTOFMEMORY),
			__FILE__, __LINE__, __FUNCTION__);

		ircsp_panic (get_log_message (LOG_MESSAGE_OUTOFMEMORY),
			__FILE__, __LINE__, __FUNCTION__);
	}


	strcpy (c_parse, sBuf);
	argv[0] = strtok (c_parse, " ");
	argv[1] = strtok (NULL, " ");


	for (i_cmd = 0; i_cmd <= LIBUNDERNET_NUM_MSGS; i_cmd++)
	{
		if ( (!strcasecmp (argv[0], Server_Messages[i_cmd].command)) || (!strcasecmp (argv[1], Server_Messages[i_cmd].command)) )
		{
			if (Server_Messages[i_cmd].func (sBuf))
			{

			}
		}
	}

	ircsp_free (c_parse);


	if (DEBUG)
	{
		ircsp_log (F_MAINLOG, get_log_message (LOG_MESSAGE_FUNCEND),
			__FILE__, __LINE__, __FUNCTION__);
	}
}
