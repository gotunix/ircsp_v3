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
#include	<sys/time.h>


/* IRCSP Core Includes */
#include	"alloc.h"
#include	"ircsp.h"
#include	"users.h"
#include	"channels.h"
#include	"servers.h"
#include	"log.h"
#include	"modules.h"
#include	"conf.h"
#include	"socket.h"


/* IRCSP Module Includes */
#include	"libundernet.h"
#include	"libundernet_cmds.h"
#include	"libundernet_introdouce.h"


/* Debug */
#include	"debug.h"


/* Compile Generated Includes */
#include	"config.h"


/*
---------------------------------------------------------------------------------------------------------------------------------
 FUNCTION       |    libundernet_introdouce_service ()
                |
 DESCRIPTION    |    This function will bring up a service nickname using the Undernet P10 protocol
                |
                |    Arguments *MUST* be the same on all protocols.
                |
 INPUTS         |        argv[0] = Nickname
                |        argv[1] = Username
                |        argv[2] = Hostname
                |        argv[3] = IRCModes
                |        argv[4] = Realname
                |
 RETURNS        |
---------------------------------------------------------------------------------------------------------------------------------
*/
void libundernet_introdouce_service (argv)
	char		*argv[];
{
	if (DEBUG)
	{
		ircsp_log (F_MAINLOG, get_log_message (LOG_MESSAGE_FUNCSTART),
			__FILE__, __LINE__, __FUNCTION__);
	}


	nickname_t		*p_nickname;
	nickname_numeric_t	*p_nickname_numeric;
	nickname_channel_t	*p_nickname_channel;
	channel_t		*p_channel;
	channel_nickname_t	*p_channel_nickname;
	hostname_t		*p_hostname;
	server_t		*p_server;
	char			*c_numeric;


	assert (argv[0] != NULL);
	assert (argv[1] != NULL);
	assert (argv[2] != NULL);
	assert (argv[3] != NULL);
	assert (argv[4] != NULL);


	/* First lets generate a numeric */
	c_numeric = ircsp_calloc (1, 20);
	NicknameNumeric_Generate (c_numeric);
	p_nickname_numeric = NicknameNumeric_Add (c_numeric);

	assert (p_nickname_numeric != NULL);
	ircsp_free (c_numeric);


	assert (config != NULL);
	assert (config->server_name != NULL);
	assert (config->server_numeric != NULL);


	p_server = Server_Find (config->server_name);
	assert (p_server != NULL);
	assert (p_server->name != NULL);
	assert (p_server->numeric != NULL);


	p_hostname = Hostname_Find (argv[2]);
	if (p_hostname)
	{
		p_hostname->numHosts++;
	}
	else
	{
		p_hostname = Hostname_Add (argv[2]);
	}


	assert (p_hostname != NULL);
	assert (p_hostname->hostname != NULL);


	p_nickname = Nickname_Find (argv[0]);
	if (p_nickname)
	{
		/*
		-----------------------------------------------------------------------------------------------------------------
		Something or someone is using our nickname, before we COLLIDE them off lets delete them and update our lists
		accordingly.
		-----------------------------------------------------------------------------------------------------------------
		*/
		p_hostname = p_nickname->hostname_p;
		assert (p_hostname != NULL);
		assert (p_hostname->hostname != NULL);


		if (p_hostname->numHosts > 1)
		{
			p_hostname->numHosts--;
		}
		else
		{
			Hostname_Del (p_hostname->hostname);
			status->numHosts--;
		}

		if (p_nickname->flags & NICK_IS_OPER)
		{
			p_nickname->flags &= ~NICK_IS_OPER;
			Operator_Del (p_nickname);
		}

		p_nickname_channel = p_nickname->channel_h;
		while (p_nickname_channel)
		{
			p_channel = p_nickname_channel->channel_p;
			ChannelNickname_Del (p_channel, p_nickname);
			NicknameChannel_Del (p_nickname, p_channel);

			p_nickname_channel = p_nickname_channel->next;
		}


		Nickname_Del (p_nickname->nickname);
	}
	else
	{
		p_hostname = Hostname_Find (argv[2]);
		if (p_hostname)
		{
			p_hostname->numHosts++;
		}
		else
		{
			p_hostname = Hostname_Add (argv[2]);
			assert (p_hostname != NULL);
			assert (p_hostname->hostname != NULL);
		}


		p_nickname = Nickname_Add (argv[0], argv[1], p_hostname, p_server, 0);
		assert (p_nickname != NULL);
		assert (p_nickname->nickname != NULL);
		assert (p_nickname->hostname_p != NULL);
		assert (p_nickname->server_p != NULL);

		p_nickname->numeric = (char *)ircsp_calloc (1, strlen (p_nickname_numeric->numeric) + 5);
		assert (p_nickname->numeric != NULL);

		strcpy (p_nickname->numeric, p_nickname_numeric->numeric);
		status->numUsers++;

		p_nickname->flags |= NICK_IS_OPER;
		p_nickname->flags |= NICK_IS_SERVICE;


		ssprintf (s,
			"%s N %s 1 0 %s %s %s AAAAAA %s :%s\n",
			p_server->numeric, argv[0], argv[1], argv[2], argv[3], p_nickname->numeric, argv[4]);
	}


	if (DEBUG)
	{
		ircsp_log (F_MAINLOG, get_log_message (LOG_MESSAGE_FUNCEND),
			__FILE__, __LINE__, __FUNCTION__);
	}
}


/*
---------------------------------------------------------------------------------------------------------------------------------
 FUNCTION       |    libundernet_introdouce_server ()
                |
 DESCRIPTION    |    This function will introdouce our server using the IRCU protocol
                |
                |    Arguments *MUST* be the same on all protocols.
                |
 INPUTS         |    NONE
                |
 RETURNS        |    NONE
---------------------------------------------------------------------------------------------------------------------------------
*/
void libundernet_introdouce_server (void)
{
	if (DEBUG)
	{
		ircsp_log (F_MAINLOG, get_log_message (LOG_MESSAGE_FUNCSTART),
			__FILE__, __LINE__, __FUNCTION__);
	}

	assert (config != NULL);
	assert (config->server_name != NULL);
	assert (config->server_numeric != NULL);
	assert (config->server_comment != NULL);
	assert (config->uplink_pass != NULL);




	server_t	*p_server;


	p_server = Server_Add (config->server_name, time (NULL));
	assert (p_server != NULL);
	assert (p_server->name != NULL);

	p_server->numeric = (char *)ircsp_calloc (1, strlen (config->server_numeric) + 5);
	assert (p_server->numeric != NULL);

	strcpy (p_server->numeric, config->server_numeric);


	ssprintf (s,
		"PASS :%s\n",
		config->uplink_pass);

	ssprintf (s,
		"SERVER %s 1 %d %d J10 %s]]] :%s\n",
		config->server_name, time (NULL), time (NULL), config->server_numeric,
		config->server_comment);


	if (DEBUG)
	{
		ircsp_log (F_MAINLOG, get_log_message (LOG_MESSAGE_FUNCEND),
			__FILE__, __LINE__, __FUNCTION__);
	}
}
