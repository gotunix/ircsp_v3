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


/* IRCSP Core Includes */
#include	"conf.h"
#include	"ircsp.h"
#include	"alloc.h"
#include	"log.h"
#include	"servers.h"


/* IRCSP Module Includes */
#include	"libundernet.h"
#include	"libundernet_parser.h"
#include	"libundernet_m_server.h"


/* Debug */
#include	"debug.h"


/* Compiler Generated Includes */
#include	"config.h"


/*
---------------------------------------------------------------------------------------------------------------------------------
 FUNCTION       |    libundernet_m_server ()
                |
 DESCRIPTION    |    Undernet Protocol (m_server)
                |    This function will handle Server Message: SERVER [S]
                |
                |
                |
 INPUTS         |    sBuf   = Socket Buffer
                |
 RETURNS        |    TRUE
---------------------------------------------------------------------------------------------------------------------------------
*/
int libundernet_m_server (sBuf)
	char		*sBuf;
{
	if (DEBUG)
	{
		ircsp_log (F_MAINLOG, get_log_message (LOG_MESSAGE_FUNCSTART),
			__FILE__, __LINE__, __FUNCTION__);
	}


	server_t		*p_server;
	server_t		*p_server_mine;
	server_t		*p_server_uplink;
	char			*argv[6];
	char			*c_server;
	char			c_uplink = '\0';
	char			*c_numeric;


	assert (sBuf != NULL);
	assert (config != NULL);
	assert (config->server_name != NULL);
	assert (config->server_numeric != NULL);


	c_server = (char *)ircsp_calloc (1, strlen (sBuf) + 20);
	assert (c_server != NULL);


	strcpy (c_server, sBuf);

	argv[0] = strtok (sBuf, " ");				/* Protocol Command */
	if (!strcasecmp (argv[0], "SERVER"))
	{
		/*
		---------------------------------------------------------------------------------------------------------
		 Buffer: SERVER hub01.gotunix.net 1 1571100042 1572725126 J10 ADAB] +h6 :GOTUNIX Development Server
		---------------------------------------------------------------------------------------------------------
		*/
		strtok (c_server, " ");			/* Ignore Protocol Command */
		argv[1] = strtok (NULL, " ");		/* Server Name */
		strtok (NULL, " ");			/* Hops (Don't think we will ever need this) */
		strtok (NULL, " ");			/* Start Time (Don't think we will ever need this) */
		argv[2] = strtok (NULL, " ");		/* Link Timestamp */
		argv[3] = strtok (NULL, " ");		/* Protocol */
		argv[4] = strtok (NULL, " ");		/* Remote Server Numeric */


		if ( (!strcmp (argv[3], "J10")) && (!strcmp (argv[3], "P10")) )
		{
			/*
			-------------------------------------------------------------------------------------------------
			 Unsupported protocol, we should probably SQUIT the server.  For now we will just end.
			-------------------------------------------------------------------------------------------------
			*/

			ircsp_log (F_MAINLOG, "[%s:%d:%s()]: SERVER [%s] is using an unsupported protocol!\n",
				__FILE__, __LINE__, __FUNCTION__, argv[1]);


			if (DEBUG)
			{
				ircsp_log (F_MAINLOG, get_log_message (LOG_MESSAGE_FUNCEND),
					__FILE__, __LINE__, __FUNCTION__);
			}


			return 0;
		}
		else
		{
			c_numeric = (char *)ircsp_calloc (1, strlen (argv[4]) + 5);
			assert (c_numeric != NULL);


			sprintf (c_numeric, "%c%c", argv[4][0], argv[4][1]);


			p_server = Server_Add (argv[1], atoi (argv[2]));
			assert (p_server != NULL);
			assert (p_server->name != NULL);


			p_server->numeric = (char *)ircsp_calloc (1, strlen (c_numeric) + 5);
			assert (p_server->numeric != NULL);


			strcpy (p_server->numeric, c_numeric);


			p_server->flags |= SERVER_IS_MY_UPLINK;


			p_server_mine = Server_Find (config->server_name);
			assert (p_server_mine != NULL);


			p_server_mine->my_uplink_p = p_server;
		}
	}
	else
	{
		/*
		---------------------------------------------------------------------------------------------------------
		 Buffer: AD S client03.gotunix.net 2 0 1572713520 P10 AGAB] +6 :GOTUNIX Development Server
		---------------------------------------------------------------------------------------------------------
		*/
		argv[1] = strtok (c_server, " ");		/* Uplink Numeric */
		strtok (NULL, " ");				/* Server Command (Ignore) */
		argv[2] = strtok (NULL, " ");			/* Server Name */
		strtok (NULL, " ");				/* HOP Count (Ignore) */
		strtok (NULL, " ");				/* Start time (Ignore) */
		argv[3] = strtok (NULL, " ");			/* Link Timestamp */
		argv[4] = strtok (NULL, " ");			/* Protocol */
		argv[5] = strtok (NULL, " ");			/* Server Numeric */


		if ( (!strcmp (argv[4], "J10")) && (!strcmp (argv[4], "P10")) )
		{
			/*
			-------------------------------------------------------------------------------------------------
			 Unsupported protocol, we should probably SQUIT the server. for now we just end.
			-------------------------------------------------------------------------------------------------
			*/
			ircsp_log (F_MAINLOG, "[%s:%d:%s()]: SERVER [%s] is using an unsupported protocol\n",
				__FILE__, __LINE__, __FUNCTION__, argv[2]);


			if (DEBUG)
			{
				ircsp_log (F_MAINLOG, get_log_message (LOG_MESSAGE_FUNCEND),
					__FILE__, __LINE__, __FUNCTION__);
			}


			return 0;
		}
		else
		{
			c_numeric = (char *)ircsp_calloc (1, strlen (argv[5]) + 5);
			assert (c_numeric != NULL);


			sprintf (c_numeric, "%c%c", argv[5][0], argv[5][1]);


			p_server = Server_Add (argv[2], atoi (argv[3]));
			assert (p_server != NULL);
			assert (p_server->name != NULL);


			p_server->numeric = (char *)ircsp_calloc (1, strlen (c_numeric) + 5);
			assert (p_server->numeric != NULL);


			strcpy (p_server->numeric, c_numeric);



			p_server_uplink = Server_Find (argv[1]);
			assert (p_server_uplink != NULL);


			p_server->my_uplink_p = p_server_uplink;
		}
	}


	ircsp_free (c_server);


	if (DEBUG)
	{
		ircsp_log (F_MAINLOG, get_log_message (LOG_MESSAGE_FUNCEND), __FILE__, __LINE__, __FUNCTION__);
	}
}
