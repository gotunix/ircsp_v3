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

#ifndef __INCLUDED_CORE_LISTS_NICKNAME_H__
#define __INCLUDED_CORE_LISTS_NICKNAME_H__


/*
---------------------------------------------------------------------------------------------------------------------------------
 DEFINES
---------------------------------------------------------------------------------------------------------------------------------
*/
#define		E			extern
#define		NUMNICKLOG		6
#define		NUMNICKMASK		63


/*
---------------------------------------------------------------------------------------------------------------------------------
 FLAGS
---------------------------------------------------------------------------------------------------------------------------------
*/
enum
{
	NICK_IS_JUPE		=	1,
	NICK_IS_OPER		=	2,
	NICK_IS_IDENTIFIED	=	4,
	NICK_IS_ADMIN		=	8,
	NICK_IS_ROOT		=	16,
	NICK_IS_SERVICE		=	32
};


/*
---------------------------------------------------------------------------------------------------------------------------------
 STRUCTURES
---------------------------------------------------------------------------------------------------------------------------------
*/
typedef struct _nickname
{
	char				*nickname;			/* Nickname */
	char				*username;			/* Username */
	struct _hostname		*hostname_p;			/* Pointer to hostname record for my hostname */
	struct _server			*server_p;			/* Pointer to the server record user is on */
	char				*numeric;			/* Numeric (only needed for undernet) */
	int				flags;				/* NICK_IS flags */
	time_t				stamp;				/* Timestamp user connected */
	int				numChans;			/* How many channels am i in */
	struct	_nickname_channel	*channel_h;			/* Channels that the User is in */
	struct	_nickname		*next;				/* Next record in Users list */
	struct	_admin_list		*admin_p;
} nickname_t;


typedef struct _nickname_numeric
{
	char				*numeric;
	struct _nickname		*nickname_p;			/* Pointer to nickname using the numeric */
	struct _nickname_numeric	*next;
} nickname_numeric_t;


typedef struct _hostname
{
	char				*hostname;
	int				numHosts;

	struct	_hostname		*next;
} hostname_t;


typedef struct _operator
{
	struct	_nickname		*nickname_p;
	struct _operator		*next;
} operator_t;


typedef struct _nickname_channel
{
	struct	_channel		*channel_p;
	int				flags;
	struct	_nickname_channel	*next;
} nickname_channel_t;


nickname_t				*nickname_h;
nickname_numeric_t			*nickname_numeric_h;
operator_t				*operator_h;
hostname_t				*hostname_h;


/*
---------------------------------------------------------------------------------------------------------------------------------
 PROTOTYPES
---------------------------------------------------------------------------------------------------------------------------------
*/
E	const char			convert2y[];
E	const unsigned int		convert2n[];
E	unsigned int			base64toint				(const char *);
E	char				*intobase64				(char *buf, unsigned int, unsigned int);
E	nickname_t			*Nickname_Add				(char *, char *, struct _hostname *, struct _server *, time_t);
E	nickname_t			*Nickname_Find				(char *);
E	void				Nickname_Del				(char *);
E	void				Nickname_DelServer			(struct _server *);
E	hostname_t			*Hostname_Add				(char *);
E	hostname_t			*Hostname_Find				(char *);
E	void				Hostname_Del				(char *);
E	operator_t			*Operator_Add				(struct _nickname *);
E	operator_t			*Operator_Find				(struct _nickname *);
E	void				Operator_Del				(struct _nickname *);
E	nickname_channel_t		*NicknameChannel_Add			(nickname_t *, struct _channel *);
E	nickname_channel_t		*NicknameChannel_Find			(nickname_t *, struct _channel *);
E	void				NicknameChannel_Del			(nickname_t *, struct _channel *);
E	char				*NicknameNumeric_Generate		(char *);
E	nickname_numeric_t		*NicknameNumeric_Add			(char *);
E	nickname_numeric_t		*NicknameNumeric_Find			(char *);
E	void				NicknameNumeric_Del			(char *);

#endif		/* __INCLUDED_CORE_LISTS_NICKNAME_H__ */
