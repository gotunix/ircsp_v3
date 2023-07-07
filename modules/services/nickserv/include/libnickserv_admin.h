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

#ifndef		__INCLUDED_LIBNICKSERV_ADMIN_H__
#define		__INCLUDED_LIBNICKSERV_ADMIN_H__
#define		E			extern


/*
---------------------------------------------------------------------------------------------------------------------------------
    FLAGS
---------------------------------------------------------------------------------------------------------------------------------
*/
enum
{
	ADMIN_SUSPENDED	=	1,
	ADMIN_INUSE	=	2
};


/*
---------------------------------------------------------------------------------------------------------------------------------
    STRUCTURES
---------------------------------------------------------------------------------------------------------------------------------
*/
typedef struct _services_admin
{
	char			*nickname;			/* Nickname of Admin */
	int			flags;				/* Flags */

	struct _nickname	*nickname_p;			/* Nickname currently logged in */
	struct _nickinfo	*nickinfo_p;			/* NickServ account linked to SA */
	struct _services_admin	*next;				/* Pointer to next SA record */
} services_admin_t;


typedef struct _services_root
{
	char			*nickname;			/* Nickname of ROOT */
	int			flags;				/* Flags */

	struct _nickname	*nickname_p;			/* Pointer to Nickname Record */
	struct _nickinfo	*nickinfo_p;			/* Pointer to Nickinfo Record */
	struct _services_root	*next;				/* Pointer to next ROOT record */
} services_root_t;


services_root_t			*services_root_h;
services_admin_t		*services_admin_h;


/*
---------------------------------------------------------------------------------------------------------------------------------
    PROTOTYPES
---------------------------------------------------------------------------------------------------------------------------------
*/
E	services_root_t			*ServicesRoot_Create			(char *);
E	services_root_t			*ServicesRoot_Search			(char *);
E	void				ServicesRoot_Remove			(char *);
E	void				NS_ServicesRoot_unregister		(struct _nickname *);
E	services_admin_t		*ServicesAdmin_Create			(char *);
E	services_admin_t		*ServicesAdmin_Search			(char *);
E	void				ServicesRoot_Remove			(char *);
E	void				services_admin_unregister		(struct _nickname *);



#endif		/* __INCLUDED_LIBNICKSERV_ADMIN_H__ */
