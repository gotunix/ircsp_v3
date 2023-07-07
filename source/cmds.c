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
#include	<stdarg.h>
#include	<string.h>
#include	<time.h>
#include	<ctype.h>
#include	<unistd.h>


/* IRCSP Core Includes */
#include	"channels.h"
#include	"users.h"
#include	"servers.h"
#include	"cmds.h"
#include	"alloc.h"
#include	"ircsp.h"
#include	"log.h"
#include	"conf.h"
#include	"socket.h"
#include	"modules.h"


/* Debugging */
#include	"debug.h"


/* Compiler Generated Includes */
#include	"config.h"


/*
---------------------------------------------------------------------------------------------------------------------------------
 FUNCTION       |    cmd_notice_operators ()
                |
 DESCRIPTION    |
                |
 RETURNS        |
---------------------------------------------------------------------------------------------------------------------------------
*/
void cmd_notice_operators (char *src, char *format, ...)
{
	LOG (main_logfile_p, LOG_FUNC,
		get_log_message (LOG_MESSAGE_FUNCSTART),
		__FILE__, __LINE__, __FUNCTION__
	);

	if (src != NULL)
	{
		nickname_t		*p_nickname_src;


		p_nickname_src = Nickname_Find (src);
		if (p_nickname_src != NULL)
		{
			char		buf[256];
			va_list		msg;

			va_start (msg, format);
			vsprintf (buf, format, msg);


			if (config->operator_notice & OPERNOTICE_CHANNEL)
			{
				channel_t		*p_channel;
				nickname_channel_t	*p_nickname_chanlist;


				if (config->operchannel_name != NULL)
				{
					p_channel = Channel_Find (config->operchannel_name);
					if (p_channel != NULL)
					{
						p_nickname_chanlist = NicknameChannel_Find (p_nickname_src, p_channel);
						if (p_nickname_chanlist != NULL)
						{
							cmd_notice (p_nickname_src->numeric, p_channel->name, buf);
						}
					}
					else
					{
						LOG (main_logfile_p, LOG_WARNING,
							"[WARN] - [%s:%d:%s()]:  Failed to locate operchannel [%s] in Channel_p\n",
							__FILE__, __LINE__, __FUNCTION__,
							config->operchannel_name
						);
					}
				}
				else
				{
					LOG (main_logfile_p, LOG_WARNING,
						"[WARN] - [%s:%d:%s()]:  Operator channel appears to be undefined, check conf file [%s]\n",
						__FILE__, __LINE__, __FUNCTION__, CONFFILE
					);
				}
			}

			else if (config->operator_notice & OPERNOTICE_DIRECT)
			{
				operator_t		*p_Operator;


				for (p_Operator = operator_h; p_Operator; p_Operator = p_Operator->next)
				{
					cmd_notice (p_nickname_src->numeric, p_Operator->nickname_p->numeric, buf);
				}
			}

			else if (config->operator_notice & OPERNOTICE_WALLOPS)
			{
				cmd_wallops (p_nickname_src->numeric, buf);
			}
		}
		else
		{
			LOG (main_logfile_p, LOG_WARNING,
				get_log_message (LOG_MESSAGE_NOSUCHNICK),
				__FILE__, __LINE__, __FUNCTION__, src
			);
		}
	}

	LOG (main_logfile_p, LOG_FUNC,
		get_log_message (LOG_MESSAGE_FUNCEND),
		__FILE__, __LINE__, __FUNCTION__
	);
}
