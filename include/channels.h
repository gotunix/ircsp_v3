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

#ifndef __INCLUDED_CORE_LISTS_CHANNELS_H__
#define __INCLUDED_CORE_LISTS_CHANNELS_H__


/*
---------------------------------------------------------------------------------------------------------------------------------
 DEFINES
---------------------------------------------------------------------------------------------------------------------------------
*/
#define		E		extern


/*
---------------------------------------------------------------------------------------------------------------------------------
 FLAGS
---------------------------------------------------------------------------------------------------------------------------------
*/
enum
{
	CHANNEL_HAS_LIMIT		=	1,
	CHANNEL_HAS_KEY			=	2,
	CHANNEL_LOCKED			=	4
};

enum
{
	IS_CHANOP			=	1,
	IS_CHANVOICE			=	2
};


/*
---------------------------------------------------------------------------------------------------------------------------------
 STRUCTURES
---------------------------------------------------------------------------------------------------------------------------------
*/
typedef struct _channel
{
	char				*name;
	char				*key;
	int				flags;
	int				limit;
	time_t				created;
	int				numBans;
	int				numUsers;

	struct	_channel_ban		*channel_ban_h;
	struct	_channel_nickname	*channel_nickname_h;
	struct	_channel		*next;
} channel_t;


typedef struct _channel_nickname
{
	struct	_nickname		*nickname_p;
	int				flags;
	struct _channel_nickname	*next;
} channel_nickname_t;


typedef struct _channel_ban
{
		char			*mask;
		struct	_channel_ban	*next;
} channel_ban_t;

channel_t				*channel_h;


/*
---------------------------------------------------------------------------------------------------------------------------------
 PROTOTYPES
---------------------------------------------------------------------------------------------------------------------------------
*/
E	channel_t		*Channel_Add				(char *, int, time_t, char *, int);
E	channel_t		*Channel_Find				(char *);
E	void			Channel_Del				(char *);
E	channel_nickname_t	*ChannelNickname_Add			(struct _channel *channelp, struct _nickname *userp);
E	channel_nickname_t	*ChannelNickname_Find			(struct _channel *channelp, struct _nickname *userp);
E	void			ChannelNickname_Del			(struct _channel *channelp, struct _nickname *userp);
E	channel_ban_t		*ChannelBan_Add				(channel_t *, char *);
E	channel_ban_t		*ChannelBan_Find			(channel_t *, char *);
E	void			ChannelBan_Del				(channel_t *, char *);


E	void			channel_remove				(char *);
E	void			channel_user_add			(struct _channel *, struct _nickname *);
E	void			channel_user_op				(struct _channel *, struct _nickname *);
E	void			channel_user_deop			(struct _channel *, struct _nickname *);
E	void			channel_user_voice			(struct _channel *, struct _nickname *);
E	void			channel_user_devoice			(struct _channel *, struct _nickname *);


#endif		/* __INCLUDED_CORE_LISTS_CHANNELS_H__ */
