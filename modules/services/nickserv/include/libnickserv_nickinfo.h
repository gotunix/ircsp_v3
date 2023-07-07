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

#ifndef		__INCLUDED_LIBNICKSERV_NICKINFO_H__
#define		__INCLUDED_LIBNICKSERV_NICKINFO_H__
#define		E		extern


/*
---------------------------------------------------------------------------------------------------------------------------------
 DEFINES
---------------------------------------------------------------------------------------------------------------------------------
*/
#define		NICKSERV_EXPIRE			2592000			/* Time in Seconds when a nickname will be automatically
									   DROPPED from the Registration database.
									*/

#define		NICKSERV_JUPE_EXPIRE		1800			/* Time in seconds before a nick is released automatically
									   after it is JUPED.
									*/


/*
---------------------------------------------------------------------------------------------------------------------------------
 FLAGS
---------------------------------------------------------------------------------------------------------------------------------
*/
enum
{
	NICKSERV_KILLPROTECT		=	1,			/* NickInfo flag for autokill protection */
	NICKSERV_SECURE			=	2,			/* NickInfo flag to enable nick secure */
	NICKSERV_AUTOIDENTIFY		=	4,			/* NickInfo flag to enable auto identify */
	NICKSERV_SUSPENDED		=	256			/* Nickinfo flag for susendending nickname */
};


/*
---------------------------------------------------------------------------------------------------------------------------------
 STRUCTURES
---------------------------------------------------------------------------------------------------------------------------------
*/
typedef struct _nickinfo
{
	char				*nickname;			/* NickServ Registered Nickname */
	char				*password;			/* Password */
	char				*last_login;			/* user@host that last IDENTIFIED */
	time_t				last_seen;			/* Timestamp user was last seen */
	time_t				registered;			/* Timestamp the nickname was registered */
	int				flags;				/* Flags set on the nick */


	struct _nickname		*nickname_p;			/* Pointer to the nickname IDENTIFIED for the nick */
	struct _nickinfo_access		*nickinfo_access_h;		/* Access list head */
	struct _nickinfo		*next;				/* Pointer to the next record in the list */
} nickinfo_t;


typedef struct _nickinfo_access
{
	char				*hostmask;			/* USER@HOST in Access List */

	struct _nickinfo		*nickinfo_p;			/* Pointer to the Registered Nickname this entry belongs to */
	struct _nickinfo_access		*next;				/* Pointer to next record in the list */
} nickinfo_access_t;


typedef struct _nickinfo_jupe
{
	char				*nickname;			/* JUPE Nickname */
	time_t				timestamp;			/* Timestamp JUPE was created */
	struct _nickname		*nickname_p;			/* Pointer to nickname record for the JUPE */
	struct _nickinfo		*nickinfo_p;			/* Pointer to the registration */
	struct _nickinfo_jupe		*next;				/* Pointer to the next record on the JUPE list */
} nickinfo_jupe_t;


nickinfo_t				*nickinfo_h;
nickinfo_jupe_t				*nickinfo_jupe_h;


/*
---------------------------------------------------------------------------------------------------------------------------------
 PROTOTYPES
---------------------------------------------------------------------------------------------------------------------------------
*/
E	nickinfo_t			*Nickinfo_Create			(char *);
E	nickinfo_t			*Nickinfo_Search			(char *);
E	void				Nickinfo_Remove				(char *);
E	void				nickinfo_unregister			(struct _nickname *);
E	int				nickinfo_expire				(void);
E	int				nickinfo_validate			(void);
E	nickinfo_access_t		*NickinfoAccess_Create			(struct _nickinfo *, char *);
E	nickinfo_access_t		*NickinfoAccess_Search			(struct _nickinfo *, char *);
E	nickinfo_access_t		*NickinfoAccess_Wildcard		(struct _nickinfo *, char *);
E	void				NickinfoAccess_Remove			(struct _nickinfo *, char *);
E	nickinfo_jupe_t			*NickinfoJupe_Create			(char *);
E	nickinfo_jupe_t			*NickinfoJupe_Search			(char *);
E	void				NickinfoJupe_Remove			(char *);
E	int				nickinfo_jupe_expire			(void);


#endif		/* __INCLUDED_NICKSERV_H__ */
