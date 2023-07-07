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
#include	"alloc.h"
#include	"users.h"
#include	"modules.h"


/* IRCSP Module Includes */
#include	"libundernet.h"
#include	"libundernet_parser.h"
#include	"libundernet_m_privmsg.h"


/* Debug */
#include	"debug.h"


/* Compiler Generated Includes */
#include	"config.h"


/*
---------------------------------------------------------------------------------------------------------------------------------
 FUNCTION       |    libundernet_m_privmsg ()
                |
 DESCRIPTION    |    Undernet Protocol (m_privmsg)
                |    This function will handle Server Message: PRIVMSG [P]
                |
                |        [ircsp.c:725:ircsp_mainloop()]: Socket (READ): ADAAY P AIAAB :SUP
                |
                |
 INPUTS         |    sBuf   = Socket Buffer
                |
 RETURNS        |    TRUE
---------------------------------------------------------------------------------------------------------------------------------
*/
int libundernet_m_privmsg (sBuf)
	char		*sBuf;
{
	if (DEBUG)
	{
		ircsp_log (F_MAINLOG, get_log_message (LOG_MESSAGE_FUNCSTART),
			__FILE__, __LINE__, __FUNCTION__);
	}


	nickname_t		*p_nickname_src;
	nickname_t		*p_nickname_dst;
	module_t		*p_Module;
	module_callback_t	*p_ModuleCallback;
	ConfigServiceModule	*p_ConfigServiceModule;
	char			*argv[2];
	char			*c_privmsg;


	c_privmsg = (char *)ircsp_calloc (1, strlen (sBuf) + 20);
	assert (c_privmsg != NULL);


	strcpy (c_privmsg, sBuf);

	argv[0] = strtok (c_privmsg, " ");
	strtok (NULL, " ");
	argv[1] = strtok (NULL, " ");
	argv[2] = strtok (NULL, "\n");


	if (argv[1][0] == '#')
	{
		/*
		-----------------------------------------------------------------------------------------------------------------
		We don't do anything with channel messges yet.
		-----------------------------------------------------------------------------------------------------------------
		*/


		return 0;
	}


	p_nickname_src = Nickname_Find (argv[0]);
	assert (p_nickname_src != NULL);


	p_nickname_dst = Nickname_Find (argv[1]);
	assert (p_nickname_dst != NULL);


	p_ConfigServiceModule = ConfigServiceModule_find (p_nickname_dst->nickname);
	if (p_ConfigServiceModule)
	{
		if (p_ConfigServiceModule->p_Module)
		{
			p_ModuleCallback = ModuleCallback_Find (p_ConfigServiceModule->p_Module, "m_privmsg");
			if (p_ModuleCallback)
			{
				p_ModuleCallback->func (p_ConfigServiceModule->p_Module, argv[0], argv[1], argv[2]);
			}

		}
	}

	if (DEBUG)
	{
		ircsp_log (F_MAINLOG, get_log_message (LOG_MESSAGE_FUNCEND), __FILE__, __LINE__, __FUNCTION__);
	}


	return TRUE;
}
