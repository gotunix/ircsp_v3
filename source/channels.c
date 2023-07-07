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
#include	<unistd.h>
#include	<assert.h>


/* IRCSP Core Includes */
#include	"conf.h"
#include	"alloc.h"
#include	"log.h"
#include	"text.h"
#include	"ircsp.h"
#include	"users.h"
#include	"channels.h"


/* Debugging */
#include	"debug.h"


/* Compiler Generated Includes */
#include	"config.h"


/*
---------------------------------------------------------------------------------------------------------------------------------
                                           CHANNELS RELATED LINKED LISTS
---------------------------------------------------------------------------------------------------------------------------------
*/

/*
---------------------------------------------------------------------------------------------------------------------------------
 FUNCTION       |    Channel_Add ()
                |
 DESCRIPTION    |
                |
 RETURNS        |
---------------------------------------------------------------------------------------------------------------------------------
*/
channel_t *Channel_Add (name, flags, created, key, limit)
	char		*name;
	int		flags;
	time_t		created;
	char		*key;
	int		limit;
{
	LOG (main_logfile_p, LOG_FUNC,
		get_log_message (LOG_MESSAGE_FUNCSTART),
		__FILE__, __LINE__, __FUNCTION__
	);

	channel_t	*p_Channel;


	p_Channel = (channel_t *)ircsp_calloc(1, sizeof(channel_t));
	p_Channel->name = (char *)ircsp_calloc(1, strlen (name) + 5);
	if ( (!p_Channel) || (!p_Channel->name) )
	{
		LOG (main_logfile_p, LOG_CRITICAL,
			"[CRIT] - [%s:%d:%s()]:  Failed to allocate memory for channel_p\n",
			__FILE__, __LINE__, __FUNCTION__
		);

		LOG (main_logfile_p, LOG_FUNC,
			get_log_message (LOG_MESSAGE_FUNCEND),
			__FILE__, __LINE__, __FUNCTION__
		);

		return NULL;
	}

	if (key != NULL)
	{
		p_Channel->key = (char *)ircsp_calloc (1, strlen (key) + 5);
		if (!p_Channel->key)
		{
			LOG (main_logfile_p, LOG_CRITICAL,
				"[CRIT] - [%s:%d:%s()]:  Failed to allocate memory for channel_p->key\n",
				__FILE__, __LINE__, __FUNCTION__
			);

			LOG (main_logfile_p, LOG_FUNC,
				get_log_message (LOG_MESSAGE_FUNCEND),
				__FILE__, __LINE__, __FUNCTION__
			);

			return NULL;
		}
	}
	else
	{
		p_Channel->key = NULL;
	}


	strcpy(p_Channel->name, name);

	p_Channel->flags		= flags;
	p_Channel->created		= created;
	p_Channel->channel_ban_h	= NULL;
	p_Channel->channel_nickname_h	= NULL;

	LOG (main_logfile_p, LOG_INFO,
		"[INFO] - [%s:%d:%s()]:  Channel [%s] added at [%p]\n",
		__FILE__, __LINE__, __FUNCTION__,
		p_Channel->name, p_Channel->name
	);

	if (flags & CHANNEL_HAS_LIMIT)
	{
		p_Channel->limit = limit;
	}
	else
	{
		p_Channel->limit = 0;
	}

	if (flags & CHANNEL_HAS_KEY)
	{
		strcpy(p_Channel->key, key);
	}

	if (!channel_h)
	{
		channel_h = p_Channel;
		p_Channel->next = NULL;
	}
	else
	{
		p_Channel->next = channel_h;
		channel_h = p_Channel;
	}


	status->numChannels++;

	LOG (main_logfile_p, LOG_FUNC,
		get_log_message (LOG_MESSAGE_FUNCEND),
		__FILE__, __LINE__, __FUNCTION__
	);

	return p_Channel;
}


/*
---------------------------------------------------------------------------------------------------------------------------------
 FUNCTION       |    Channel_Find ()
                |
 DESCRIPTION    |
                |
 RETURNS        |
---------------------------------------------------------------------------------------------------------------------------------
*/
channel_t *Channel_Find (name)
	char		*name;
{
	LOG (main_logfile_p, LOG_FUNC,
		get_log_message (LOG_MESSAGE_FUNCSTART),
		__FILE__, __LINE__, __FUNCTION__
	);

	channel_t	*p_Channel;

	for (p_Channel = channel_h; p_Channel; p_Channel = p_Channel->next)
	{
		if (!strcasecmp(p_Channel->name, name))
		{
			LOG (main_logfile_p, LOG_INFO,
				"[INFO] - [%s:%d:%s()]:  Channel [%s] found at [%p]\n",
				__FILE__, __LINE__, __FUNCTION__,
				p_Channel->name, p_Channel->name
			);

			LOG (main_logfile_p, LOG_FUNC,
				get_log_message (LOG_MESSAGE_FUNCEND),
				__FILE__, __LINE__, __FUNCTION__
			);

			return p_Channel;
		}
	}

	LOG (main_logfile_p, LOG_FUNC,
		get_log_message (LOG_MESSAGE_FUNCEND),
		__FILE__, __LINE__, __FUNCTION__
	);

	return NULL;
}


/*
---------------------------------------------------------------------------------------------------------------------------------
 FUNCTION       |    Channel_Del ()
                |
 DESCRIPTION    |
                |
 RETURNS        |
---------------------------------------------------------------------------------------------------------------------------------
*/
void Channel_Del	 (name)
	char		*name;
{
	LOG (main_logfile_p, LOG_FUNC,
		get_log_message (LOG_MESSAGE_FUNCSTART),
		__FILE__, __LINE__, __FUNCTION__
	);

	channel_t	*p_Channel;
	channel_t	*p_Next;
	channel_t	*p_Delete;

	for (p_Channel = channel_h; p_Channel; p_Channel = p_Channel->next)
	{
		p_Next = p_Channel->next;

		if (!strcasecmp(p_Channel->name, name))
		{
			LOG (main_logfile_p, LOG_INFO,
				"[INFO] - [%s:%d:%s()]:  Deleting Channel [%s] found at [%p]\n",
				__FILE__, __LINE__, __FUNCTION__,
				p_Channel->name, p_Channel->name
			);

			if (channel_h == p_Channel)
			{
				channel_h = p_Channel->next;
			}
			else
			{
				for (p_Delete = channel_h; (p_Delete->next != p_Channel) && p_Delete; p_Delete = p_Delete->next);
				p_Delete->next = p_Channel->next;
			}

			status->numChannels--;

			ircsp_free (p_Channel->name);

			if (p_Channel->key != NULL)
			{
				ircsp_free (p_Channel->key);
			}

			p_Channel->flags = 0;
			p_Channel->limit = 0;
			p_Channel->created = 0;

			p_Channel->numBans = 0;
			p_Channel->numUsers = 0;

			ircsp_free (p_Channel);

			LOG (main_logfile_p, LOG_FUNC,
				get_log_message (LOG_MESSAGE_FUNCEND),
				__FILE__, __LINE__, __FUNCTION__
			);

			return ;
		}
	}

	LOG (main_logfile_p, LOG_FUNC,
		get_log_message (LOG_MESSAGE_FUNCEND),
		__FILE__, __LINE__, __FUNCTION__
	);
}


/*
---------------------------------------------------------------------------------------------------------------------------------
 FUNCTION       |    ChannelNickname_Add ()
                |
 DESCRIPTION    |
                |
 RETURNS        |
---------------------------------------------------------------------------------------------------------------------------------
*/
channel_nickname_t *ChannelNickname_Add (p_Channel, p_Nickname)
	struct _channel		*p_Channel;
	struct _nickname	*p_Nickname;
{
	LOG (main_logfile_p, LOG_FUNC,
		get_log_message (LOG_MESSAGE_FUNCSTART),
		__FILE__, __LINE__, __FUNCTION__
	);

	channel_nickname_t		*p_ChannelNickname;

	p_ChannelNickname = (channel_nickname_t *)ircsp_calloc(1, sizeof(channel_nickname_t));
	if (!p_ChannelNickname)
	{
		LOG (main_logfile_p, LOG_CRITICAL,
			"[CRIT] - [%s:%d:%s()]:  Failed to allocate memory for ChannelNickname_p\n",
			__FILE__, __LINE__, __FUNCTION__
		);

		LOG (main_logfile_p, LOG_FUNC,
			get_log_message (LOG_MESSAGE_FUNCEND),
			__FILE__, __LINE__, __FUNCTION__
		);

		return NULL;
	}

	p_ChannelNickname->nickname_p	= p_Nickname;
	p_ChannelNickname->flags	= 0;

	LOG (main_logfile_p, LOG_INFO,
		"[INFO] - [%s:%d:%s()]:  Channel [%s] added to Nickname [%s]\n",
		__FILE__, __LINE__, __FUNCTION__,
		p_Channel->name, p_Nickname->nickname
	);

	if (!p_Channel->channel_nickname_h)
	{
		p_Channel->channel_nickname_h = p_ChannelNickname;
		p_ChannelNickname->next = NULL;
	}
	else
	{
		p_ChannelNickname->next = p_Channel->channel_nickname_h;
		p_Channel->channel_nickname_h = p_ChannelNickname;
	}

	LOG (main_logfile_p, LOG_FUNC,
		get_log_message (LOG_MESSAGE_FUNCEND),
		__FILE__, __LINE__, __FUNCTION__
	);

	return p_ChannelNickname;
}


/*
---------------------------------------------------------------------------------------------------------------------------------
 FUNCTION       |    ChannelNickname_Find ()
                |
 DESCRIPTION    |
                |
 RETURNS        |
---------------------------------------------------------------------------------------------------------------------------------
*/
channel_nickname_t *ChannelNickname_Find (p_Channel, p_Nickname)
	struct _channel		*p_Channel;
	struct _nickname	*p_Nickname;
{
	LOG (main_logfile_p, LOG_FUNC,
		get_log_message (LOG_MESSAGE_FUNCSTART),
		__FILE__, __LINE__, __FUNCTION__
	);

	channel_nickname_t		*p_ChannelNickname;

	for (p_ChannelNickname = p_Channel->channel_nickname_h; p_ChannelNickname; p_ChannelNickname = p_ChannelNickname->next)
	{
		if (p_ChannelNickname->nickname_p == p_Nickname)
		{
			LOG (main_logfile_p, LOG_INFO,
				"[INFO] - [%s:%d:%s()]:  Channel [%s] found on Nickname [%s]\n",
				__FILE__, __LINE__, __FUNCTION__,
				p_Channel->name, p_Nickname->nickname
			);

			LOG (main_logfile_p, LOG_FUNC,
				get_log_message (LOG_MESSAGE_FUNCEND),
				__FILE__, __LINE__, __FUNCTION__
			);

			return p_ChannelNickname;
		}
	}

	LOG (main_logfile_p, LOG_FUNC,
		get_log_message (LOG_MESSAGE_FUNCEND),
		__FILE__, __LINE__, __FUNCTION__
	);

	return NULL;
}


/*
---------------------------------------------------------------------------------------------------------------------------------
 FUNCTION       |    ChannelNickname_Del ()
                |
 DESCRIPTION    |
                |
 RETURNS        |
---------------------------------------------------------------------------------------------------------------------------------
*/
void ChannelNickname_Del (p_Channel, p_Nickname)
	struct _channel		*p_Channel;
	struct _nickname	*p_Nickname;
{
	LOG (main_logfile_p, LOG_FUNC,
		get_log_message (LOG_MESSAGE_FUNCSTART),
		__FILE__, __LINE__, __FUNCTION__
	);

	channel_nickname_t		*prev;
	channel_nickname_t		*p_ChannelNickname;

	prev = NULL;

	for (p_ChannelNickname = p_Channel->channel_nickname_h; p_ChannelNickname; p_ChannelNickname = p_ChannelNickname->next)
	{
		if (p_ChannelNickname->nickname_p == p_Nickname)
		{
			LOG (main_logfile_p, LOG_INFO,
				"[INFO] - [%s:%d:%s()]:  Deleting Channel [%s] from Nickname [%s]\n",
				__FILE__, __LINE__, __FUNCTION__,
				p_Channel->name, p_Nickname->nickname
			);

			if (!prev)
			{
				p_Channel->channel_nickname_h = p_ChannelNickname->next;
			}
			else
			{
				prev->next = p_ChannelNickname->next;
			}

			ircsp_free (p_ChannelNickname);
		}
		prev = p_ChannelNickname;
	}

	LOG (main_logfile_p, LOG_FUNC,
		get_log_message (LOG_MESSAGE_FUNCEND),
		__FILE__, __LINE__, __FUNCTION__
	);
}


/*
---------------------------------------------------------------------------------------------------------------------------------
 FUNCTION       |    ChannelBan_Add ()
                |
 DESCRIPTION    |
                |
 RETURNS        |
---------------------------------------------------------------------------------------------------------------------------------
*/
channel_ban_t *ChannelBan_Add (p_Channel, mask)
	channel_t	*p_Channel;
	char		*mask;
{
	LOG (main_logfile_p, LOG_FUNC,
		get_log_message (LOG_MESSAGE_FUNCSTART),
		__FILE__, __LINE__, __FUNCTION__
	);

	channel_ban_t	*p_ChannelBan;

	p_ChannelBan = (channel_ban_t *)ircsp_calloc(1, sizeof(channel_ban_t));
	p_ChannelBan->mask = (char *)ircsp_calloc(1, strlen (chop(mask)) + 10);
	if ( (!p_ChannelBan) || (!p_ChannelBan->mask) )
	{
		LOG (main_logfile_p, LOG_CRITICAL,
			"[CRIT] - [%s:%d:%s()]:  Failed to allocate memory for ChannelBan_p\n",
			__FILE__, __LINE__, __FUNCTION__
		);

		LOG (main_logfile_p, LOG_FUNC,
			get_log_message (LOG_MESSAGE_FUNCEND),
			__FILE__, __LINE__, __FUNCTION__
		);

		return NULL;
	}

	strcpy (p_ChannelBan->mask, chop(mask));

	LOG (main_logfile_p, LOG_INFO,
		"[INFO] - [%s:%d:%s()]: ChannelBan [%s] added to Channel [%s] at [%p]\n",
		__FILE__, __LINE__, __FUNCTION__,
		p_ChannelBan->mask, p_Channel->name, p_ChannelBan->mask
	);

	if (!p_Channel->channel_ban_h)
	{
		p_Channel->channel_ban_h = p_ChannelBan;
		p_ChannelBan->next = NULL;
	}
	else
	{
		p_ChannelBan->next = p_Channel->channel_ban_h;
		p_Channel->channel_ban_h = p_ChannelBan;
	}

	LOG (main_logfile_p, LOG_FUNC,
		get_log_message (LOG_MESSAGE_FUNCEND),
		__FILE__, __LINE__, __FUNCTION__
	);

	return p_ChannelBan;
}


/*
---------------------------------------------------------------------------------------------------------------------------------
 FUNCTION       |    ChannelBan_Find ()
                |
 DESCRIPTION    |
                |
 RETURNS        |
---------------------------------------------------------------------------------------------------------------------------------
*/
channel_ban_t *ChannelBan_Find (p_Channel, mask)
	channel_t	*p_Channel;
	char		*mask;
{
	LOG (main_logfile_p, LOG_FUNC,
		get_log_message (LOG_MESSAGE_FUNCSTART),
		__FILE__, __LINE__, __FUNCTION__
	);

	channel_ban_t	*p_ChannelBan;

	for (p_ChannelBan = p_Channel->channel_ban_h; p_ChannelBan; p_ChannelBan = p_ChannelBan->next)
	{
		if (!strcasecmp(p_ChannelBan->mask, mask))
		{
			LOG (main_logfile_p, LOG_INFO,
				"[INFO] - [%s:%d:%s()]:  ChannelBan [%s] found on Channel [%s] at [%p]\n",
				__FILE__, __LINE__, __FUNCTION__,
				p_ChannelBan->mask, p_Channel->name, p_ChannelBan->mask
			);

			LOG (main_logfile_p, LOG_FUNC,
				get_log_message (LOG_MESSAGE_FUNCEND),
				__FILE__, __LINE__, __FUNCTION__
			);

			return p_ChannelBan;
		}
	}

	LOG (main_logfile_p, LOG_FUNC,
		get_log_message (LOG_MESSAGE_FUNCEND),
		__FILE__, __LINE__, __FUNCTION__
	);

	return NULL;
}


/*
---------------------------------------------------------------------------------------------------------------------------------
 FUNCTION       |    ChannelBan_Del ()
                |
 DESCRIPTION    |
                |
 RETURNS        |
---------------------------------------------------------------------------------------------------------------------------------
*/
void ChannelBan_Del (p_Channel, mask)
	channel_t	*p_Channel;
	char		*mask;
{
	LOG (main_logfile_p, LOG_FUNC,
		get_log_message (LOG_MESSAGE_FUNCSTART),
		__FILE__, __LINE__, __FUNCTION__
	);

	channel_ban_t	*p_ChannelBan;
	channel_ban_t	*p_Next;
	channel_ban_t	*p_Delete;


	for (p_ChannelBan = p_Channel->channel_ban_h; p_ChannelBan; p_ChannelBan = p_Next)
	{
		p_Next = p_ChannelBan->next;
		if (!strcasecmp(p_ChannelBan->mask, mask))
		{
			LOG (main_logfile_p, LOG_INFO,
				"[INFO] - [%s:%d:%s()]:  Deleting ChannelBan [%s] found on Channel [%s] at [%p]\n",
				__FILE__, __LINE__, __FUNCTION__,
				p_ChannelBan->mask, p_Channel->name, p_ChannelBan->mask
			);

			if (p_Channel->channel_ban_h == p_ChannelBan)
			{
				p_Channel->channel_ban_h = p_ChannelBan->next;
			}
			else
			{
				for (p_Delete = p_Channel->channel_ban_h; (p_Delete->next != p_ChannelBan) && p_Delete; p_Delete = p_Delete->next);
				p_Delete->next = p_ChannelBan->next;
			}


			ircsp_free (p_ChannelBan->mask);
			ircsp_free (p_ChannelBan);

			LOG (main_logfile_p, LOG_FUNC,
				get_log_message (LOG_MESSAGE_FUNCEND),
				__FILE__, __LINE__, __FUNCTION__
			);

			return;
		}
	}

	LOG (main_logfile_p, LOG_FUNC,
		get_log_message (LOG_MESSAGE_FUNCEND),
		__FILE__, __LINE__, __FUNCTION__
	);
}


/*
---------------------------------------------------------------------------------------------------------------------------------
                                         END CHANNELS RELATED LINKED LISTS
---------------------------------------------------------------------------------------------------------------------------------
*/
