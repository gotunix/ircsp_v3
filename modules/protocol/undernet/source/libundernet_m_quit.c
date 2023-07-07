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
#include	"log.h"
#include	"users.h"
#include	"channels.h"
#include	"servers.h"
#include	"modules.h"


/* IRCSP Module Includes */
#include	"libundernet.h"
#include	"libundernet_parser.h"
#include	"libundernet_m_quit.h"


/* Debug */
#include	"debug.h"


/* Compiler Generated Includes */
#include	"config.h"


/*
---------------------------------------------------------------------------------------------------------------------------------
 FUNCTION       |    libundernet_m_quit ()
                |
 DESCRIPTION    |    Undernet Protocol (m_quit)
                |    This function will handle Server Message: QUIT [Q]
                |
                |        [ircsp.c:725:ircsp_mainloop()]: Socket (READ): ADAAY Q :Signed off
                |
                |
 INPUTS         |    sBuf   = Socket Buffer
                |
 RETURNS        |    TRUE
---------------------------------------------------------------------------------------------------------------------------------
*/
int libundernet_m_quit (sBuf)
	char		*sBuf;
{
	if (DEBUG)
	{
		ircsp_log (F_MAINLOG, get_log_message (LOG_MESSAGE_FUNCSTART),
			__FILE__, __LINE__, __FUNCTION__);
	}


	nickname_t		*p_nickname;
	nickname_channel_t	*p_nickname_channel;
//	NickInfo		*p_nickinfo;
	hostname_t		*p_hostname;
//	services_root_t		*p_services_root;
//	services_admin_t	*p_services_admin;
	channel_t		*p_channel;
	server_t		*p_server;
	module_t		*module_p;
	module_callback_t	*module_callback_p;
	char			*argv[1];


	assert (sBuf != NULL);


	argv[0] = strtok (sBuf, " ");		/* Numeric of the user QUITING */


	p_nickname = Nickname_Find (argv[0]);
	assert (p_nickname != NULL);
	assert (p_nickname->numeric != NULL);
	assert (p_nickname->nickname != NULL);


	/* Was the user an IRC Operator */
	if (p_nickname->flags & NICK_IS_OPER)
	{
		Operator_Del (p_nickname);
		p_nickname->flags &= ~NICK_IS_OPER;
	}

	/*
	-------------------------------------------------------------------------------------------------------------------------
	    Run any m_quit callbacks on our service modules
	-------------------------------------------------------------------------------------------------------------------------
	*/
	module_p = module_h;
	while (module_p)
	{
		if (module_p->ModuleType & MODULETYPE_SERVICE)
		{
			module_callback_p = ModuleCallback_Find (module_p, "m_quit");
			if (module_callback_p)
			{
				module_callback_p->func (module_p, p_nickname);
			}
		}

		module_p = module_p->next;
	}


	/* Was the user on any channels */
	p_nickname_channel = p_nickname->channel_h;
	while (p_nickname_channel)
	{
		p_channel = p_nickname_channel->channel_p;
		ChannelNickname_Del (p_channel, p_nickname);
		NicknameChannel_Del (p_nickname, p_channel);

		p_nickname_channel = p_nickname_channel->next;
	}


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

	Nickname_Del (p_nickname->nickname);


	if (DEBUG)
	{
		ircsp_log (F_MAINLOG, get_log_message (LOG_MESSAGE_FUNCEND),
			__FILE__, __LINE__, __FUNCTION__);
	}


	return TRUE;
}
