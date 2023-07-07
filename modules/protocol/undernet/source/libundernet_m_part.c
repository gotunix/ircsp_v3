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
#include	"text.h"
#include	"users.h"
#include	"channels.h"


/* IRCSP Module Includes */
#include	"libundernet.h"
#include	"libundernet_parser.h"
#include	"libundernet_m_part.h"


/* Debug */
#include	"debug.h"


/* Compiler Generated Includes */
#include	"config.h"


/*
---------------------------------------------------------------------------------------------------------------------------------
 FUNCTION       |    libundernet_m_part ()
                |
 DESCRIPTION    |    Undernet Protocol (m_part)
                |    This function will handle Server Message: PART [P]
                |
                |        [ircsp.c:725:ircsp_mainloop()]: Socket (READ): ADAAY L #fuck
                |
                |
 INPUTS         |    sBuf   = Socket Buffer
                |
 RETURNS        |    TRUE
---------------------------------------------------------------------------------------------------------------------------------
*/
int libundernet_m_part (sBuf)
	char		*sBuf;
{
	if (DEBUG)
	{
		ircsp_log (F_MAINLOG, get_log_message (LOG_MESSAGE_FUNCSTART),
			__FILE__, __LINE__, __FUNCTION__);
	}


	nickname_t		*nickname_p;
	channel_t		*channel_p;
	char			*argv[16];
	char			*chanlist = NULL;
	int			count = 0;
	int			loop = 0;
	int			argc = 1;

	assert (sBuf != NULL);


	argv[0] = strtok (sBuf, " ");			/* Numeric of the user leaving the channel */
	strtok (NULL, " ");				/* Server Message: PART [L] - Ignore */
	chanlist = strtok (NULL, " ");			/* Channel the user left */


	nickname_p = Nickname_Find (argv[0]);
	assert (nickname_p != NULL);


	count = tokenize (chanlist, ',');

	if (count == 1)
	{
		channel_p = Channel_Find (chop(chanlist));
		assert (channel_p != NULL);


		NicknameChannel_Del (nickname_p, channel_p);
		ChannelNickname_Del (channel_p, nickname_p);
	}
	else
	{
		argv[argc] = strtok(chanlist, " ");
		channel_p = Channel_Find (argv[argc]);
		assert (channel_p != NULL);

		NicknameChannel_Del (nickname_p, channel_p);
		ChannelNickname_Del (channel_p, nickname_p);


		while (argv[argc])
		{
			argv[++argc] = strtok (NULL, " ");
			if (argv[argc] != NULL)
			{
				channel_p = Channel_Find (argv[argc]);
				assert (channel_p != NULL);

				NicknameChannel_Del (nickname_p, channel_p);
				ChannelNickname_Del (channel_p, nickname_p);
			}
		}
	}


	if (DEBUG)
	{
		ircsp_log (F_MAINLOG, get_log_message (LOG_MESSAGE_FUNCEND),
			__FILE__, __LINE__, __FUNCTION__);
	}


	return TRUE;
}
