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
#include	"ircsp.h"
#include	"log.h"
#include	"socket.h"
#include	"modules.h"
#include	"users.h"
#include	"servers.h"



/* IRCSP Module Includes */
#include	"libundernet.h"
#include	"libundernet_cmds.h"
#include	"libundernet_parser.h"
#include	"libundernet_m_stats.h"


/* Debug */
#include	"debug.h"


/* Compiler Generated Includes */
#include	"config.h"


/*
---------------------------------------------------------------------------------------------------------------------------------
 FUNCTION       |    libundernet_m_stats ()
                |
 DESCRIPTION    |    Undernet Protocol (m_stats)
                |    This function will handle Server Message: STATS [R]
                |
                |
                |
 INPUTS         |    sBuf   = Socket Buffer
                |
 RETURNS        |    TRUE
---------------------------------------------------------------------------------------------------------------------------------
*/
int libundernet_m_stats (sBuf)
	char		*sBuf;
{
	if (DEBUG)
	{
		ircsp_log (F_MAINLOG, get_log_message (LOG_MESSAGE_FUNCSTART),
			__FILE__, __LINE__, __FUNCTION__);
	}


	nickname_t		*p_nickname;
	server_t		*p_server;
	char			*argv[2];


	assert (sBuf != NULL);
	assert (config != NULL);
	assert (config->server_name != NULL);


	argv[0] = strtok (sBuf, " ");		/* Numeric of the user requesting information */
	strtok (NULL, " ");			/* Server Message: STATS [R] - Ignored */
	argv[1] = strtok (NULL, " ");		/* Stats Paramater */


	p_server = Server_Find (config->server_name);
	assert (p_server != NULL);
	assert (p_server->name != NULL);
	assert (p_server->numeric != NULL);


	p_nickname = Nickname_Find (argv[0]);
	assert (p_nickname != NULL);
	assert (p_nickname->numeric != NULL);


	if (!(p_nickname->flags & NICK_IS_OPER))
	{
		cmd_notice (p_server->name, p_nickname->nickname, "\002Access Denied!!\002");
		return FALSE;
	}


	if ( (!strcasecmp (argv[1], "UPTIME")) || (!strcasecmp (argv[1], "U")) )
	{
		int		i_days;
		int		i_hours;
		int		i_mins;
		int		i_secs;
		time_t		t_curtime;
		time_t		t_uptime;


		assert (ircsp != NULL);
		assert (ircsp->uptime != 0);

		t_curtime = time (NULL);
		t_uptime = t_curtime - ircsp->uptime;


		i_days = t_uptime/86400;
		i_hours = (t_uptime/3600)%24;
		i_mins = (t_uptime/60)%60;
		i_secs = t_uptime%60;


		cmd_notice (p_server->name, p_nickname->nickname, "\002%d\002 Days, \002%d\002 Hours, \002%d\002 Minutes, \002%d\002 Seconds",
			i_days, i_hours, i_mins, i_secs);

		return TRUE;
	}


	if (DEBUG)
	{
		ircsp_log (F_MAINLOG, get_log_message (LOG_MESSAGE_FUNCEND),
			__FILE__, __LINE__, __FUNCTION__);
	}


	return TRUE;
}
