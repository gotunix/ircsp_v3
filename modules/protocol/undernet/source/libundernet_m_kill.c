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
#include	"alloc.h"
#include	"conf.h"
#include	"ircsp.h"
#include	"log.h"
#include	"modules.h"
#include	"users.h"
#include	"servers.h"
#include	"channels.h"


/* IRCSP Module Includes */
#include	"libundernet.h"
#include	"libundernet_parser.h"
#include	"libundernet_m_kill.h"


/* Debug */
#include	"debug.h"


/* Compiler Generated Includes */
#include	"config.h"


/*
---------------------------------------------------------------------------------------------------------------------------------
 FUNCTION       |    libundernet_m_kill ()
                |
 DESCRIPTION    |    Undernet Protocol (m_kill)
                |    This function will handle Server Message: KILL [D]
                |
                |        ABAAd D ADAAU :irc.gotunix.net!pool-98-113-218-11.nycmny.fios.verizon.net!evilicey2 (hi)
                |
                |
 INPUTS         |    sBuf   = Socket Buffer
                |
 RETURNS        |    TRUE
---------------------------------------------------------------------------------------------------------------------------------
*/
int libundernet_m_kill (sBuf)
	char		*sBuf;
{
	if (DEBUG)
	{
		ircsp_log (F_MAINLOG, get_log_message (LOG_MESSAGE_FUNCSTART),
			__FILE__, __LINE__, __FUNCTION__);
	}


	nickname_t		*p_nickname;
	nickname_channel_t	*p_nickname_channel;
	hostname_t		*p_hostname;
	channel_t		*p_channel;
	channel_nickname_t	*p_channel_nickname;
	server_t		*p_server;
	module_t		*module_p;
	module_callback_t	*module_callback_p;
	char			*argv[4];
	char			*c_buffer;


	assert (sBuf != NULL);


	/* First, lets make a copy of the Socket Buffer in case we need it unmodified */
	c_buffer = (char *)ircsp_calloc (1, strlen (sBuf) + 20);
	assert (c_buffer != NULL);


	strcpy (c_buffer, sBuf);


	argv[0] = strtok (c_buffer, " ");		/* Numeric of the Nickname/Server that issued the KILL */
	strtok (NULL, " ");				/* Server Message: KILL [D] - Ignore */
	argv[1] = strtok (NULL, " ");			/* Numeric of the user that is being KILLed */


	/*
	-------------------------------------------------------------------------------------------------------------------------
	We should add some logic to figure out of the nickname being KILLED is one of our Jupes or Service nicknames
	and bring them back online if thats the case.

	It shouldn't really matter most newer ircds have a user mode to mark a nickname as a service nickname and forbid killing
	it.  But just incase someone doesn't have the MODE enabled its not a bad idea?

	We will worry about that later.
	-------------------------------------------------------------------------------------------------------------------------
	*/

	p_nickname = Nickname_Find (argv[1]);
	if (!p_nickname)
	{
		/* Ok so we might have removed the user from cmd_kill or cmd_nickjupe
		   So it is possible that we won't get a result here. */

		ircsp_log (F_MAINLOG, get_log_message (LOG_MESSAGE_NOSUCHNICK),
			__FILE__, __LINE__, __FUNCTION__, argv[1]
		);

		if (DEBUG)
		{
			ircsp_log (F_MAINLOG, get_log_message (LOG_MESSAGE_FUNCEND),
				__FILE__, __LINE__, __FUNCTION__
			);
		}

		return FALSE;
	}

	/* Was the Nickname a Service */
	if (p_nickname->flags & NICK_IS_SERVICE)
	{
		module_p = Module_Find_Service (p_nickname->nickname);
		assert (module_p != NULL);


		ModuleCallback_Call (module_p, "set_service_offline");
		ircsp->services_online = FALSE;
	}


	/* Was the Nickname a IRC Operator? */
	if (p_nickname->flags & NICK_IS_OPER)
	{
		Operator_Del (p_nickname);
		p_nickname->flags &= ~NICK_IS_OPER;
	}

	/*
	-------------------------------------------------------------------------------------------------------------------------
	    Run any m_kill callbacks on our services modules
	-------------------------------------------------------------------------------------------------------------------------
	*/
	module_p = module_h;
	while (module_p)
	{
		if (module_p->ModuleType & MODULETYPE_SERVICE)
		{
			module_callback_p = ModuleCallback_Find (module_p, "m_kill");
			if (module_callback_p)
			{
				module_callback_p->func (module_p, p_nickname);
			}
		}

		module_p = module_p->next;
	}

	/* Was the user in any channels */
	p_nickname_channel = p_nickname->channel_h;
	while (p_nickname_channel)
	{
		p_channel = p_nickname_channel->channel_p;
		ChannelNickname_Del (p_channel, p_nickname);
		NicknameChannel_Del (p_nickname, p_channel);

		p_nickname_channel = p_nickname_channel->next;
	}


	/* Update Hostname Records */
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
	}

	/* Finally we delete the Nickname entry */
	Nickname_Del (p_nickname->nickname);


	ircsp_free (c_buffer);


	if (DEBUG)
	{
		ircsp_log (F_MAINLOG, get_log_message (LOG_MESSAGE_FUNCEND),
			__FILE__, __LINE__, __FUNCTION__);
	}


	return TRUE;
}
