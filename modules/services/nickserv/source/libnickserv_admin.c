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

/* System Includes */
#include	<dlfcn.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<stdarg.h>
#include	<stdint.h>
#include	<unistd.h>
#include	<string.h>
#include	<time.h>
#include	<ctype.h>
#include	<assert.h>


/* IRCSP Core Includes */
#include	"modules.h"
#include	"ircsp.h"
#include	"log.h"
#include	"text.h"
#include	"alloc.h"
#include	"conf.h"
#include	"servers.h"
#include	"users.h"
#include	"channels.h"
#include	"socket.h"
#include	"scheduler.h"


/* Debug */
#include	"debug.h"


/* Compiler Generated Includes */
#include	"config.h"


/* NickServ Module Includes */
#include	"libnickserv.h"
#include	"libnickserv_database.h"
#include	"libnickserv_nickinfo.h"
#include	"libnickserv_admin.h"


/*
---------------------------------------------------------------------------------------------------------------------------------
    FUNCTION       |    ServicesAdmin_Create ()
                   |
    DESCRIPTION    |    This function will create a new object in the SerivesAdmin list
                   |
    INPUTS         |    nickname = The nickname of the ServicesAdmin
                   |
    RETURNS        |    Returns pointer to the ServicesAdmin object if success
                   |    Returns NULL on failure
---------------------------------------------------------------------------------------------------------------------------------
*/
services_admin_t
*ServicesAdmin_Create (nickname)
	char		*nickname;
{
	if (DEBUG)
	{
		ircsp_log (F_MAINLOG, get_log_message (LOG_MESSAGE_FUNCSTART),
			__FILE__, __LINE__, __FUNCTION__
		);
	}


	services_admin_t		*sa_p;

	/* Input variables should never be NULL */
	assert (nickname != NULL);

	sa_p = (services_admin_t *)ircsp_calloc (1, sizeof (services_admin_t));
	if (!sa_p)
	{
		ircsp_log (F_MAINLOG, get_log_message (LOG_MESSAGE_OUTOFMEMORY),
			__FILE__, __LINE__, __FUNCTION__
		);

		if (DEBUG)
		{
			ircsp_log (F_MAINLOG, get_log_message (LOG_MESSAGE_FUNCEND),
				__FILE__, __LINE__, __FUNCTION__
			);
		}

		return NULL;
	}

	sa_p->nickname = (char *)ircsp_calloc (1, strlen (nickname) + 5);
	if (!sa_p->nickname)
	{
		ircsp_log (F_MAINLOG, get_log_message (LOG_MESSAGE_OUTOFMEMORY),
			__FILE__, __LINE__, __FUNCTION__
		);

		if (DEBUG)
		{
			ircsp_log (F_MAINLOG, get_log_message (LOG_MESSAGE_FUNCEND),
				__FILE__, __LINE__, __FUNCTION__
			);
		}

		return NULL;
	}

	strcpy (sa_p->nickname, nickname);
	sa_p->flags		= 0;
	sa_p->nickname_p	= NULL;
	sa_p->nickinfo_p	= NULL;

	ircsp_log (F_MAINLOG,
		"[%s:%d:%s()]: Services Admin [%s] added at [%p]\n",
		__FILE__, __LINE__, __FUNCTION__,
		sa_p->nickname, sa_p->nickname
	);

	if (!services_admin_h)
	{
		services_admin_h = sa_p;
		sa_p->next = NULL;
	}
	else
	{
		sa_p->next = services_admin_h;
		services_admin_h = sa_p;
	}

	if (DEBUG)
	{
		ircsp_log (F_MAINLOG, get_log_message (LOG_MESSAGE_FUNCEND),
			__FILE__, __LINE__, __FUNCTION__
		);
	}

	return sa_p;
}


/*
---------------------------------------------------------------------------------------------------------------------------------
    FUNCTION       |    ServicesAdmin_Search ()
                   |
    DESCRIPTION    |    This function will search the ServicesAdmin list for a specific object.
                   |
    INPUTS         |    nickname = The nickname of the ServicesAdmin we are searching for
                   |
    RETURNS        |    Pointer to the ServicesAdmin object upon success
                   |    NULL for failures
---------------------------------------------------------------------------------------------------------------------------------
*/
services_admin_t
*ServicesAdmin_Search (nickname)
	char		*nickname;
{
	if (DEBUG)
	{
		ircsp_log (F_MAINLOG, get_log_message (LOG_MESSAGE_FUNCSTART),
			__FILE__, __LINE__, __FUNCTION__
		);
	}

	services_admin_t	*sa_p;

	assert (nickname != NULL);

	sa_p = services_admin_h;
	while (sa_p)
	{
		if (!strcasecmp (sa_p->nickname, nickname))
		{
			ircsp_log (F_MAINLOG,
				"[%s:%d:%s()]: Services Admin [%s] found at [%p]\n",
				__FILE__, __LINE__, __FUNCTION__,
				sa_p->nickname, sa_p->nickname
			);

			if (DEBUG)
			{
				ircsp_log (F_MAINLOG, get_log_message (LOG_MESSAGE_FUNCEND),
					__FILE__, __LINE__, __FUNCTION__
				);
			}

			return sa_p;
		}

		sa_p = sa_p->next;
	}

	if (DEBUG)
	{
		ircsp_log (F_MAINLOG, get_log_message (LOG_MESSAGE_FUNCEND),
			__FILE__, __LINE__, __FUNCTION__
		);
	}

	return NULL;
}


/*
---------------------------------------------------------------------------------------------------------------------------------
    FUNCTION       |    ServicesAdmin_Remove ()
                   |
    DESCRIPTION    |    This function will remove a object from the ServicesAdmin list
                   |
    INPUTS         |    nickname = The nickname of the object on the ServicesAdmin list we are removing.
                   |
    RETURNS        |    Nothing
---------------------------------------------------------------------------------------------------------------------------------
*/
void
ServicesAdmin_Remove (nickname)
	char		*nickname;
{
	if (DEBUG)
	{
		ircsp_log (F_MAINLOG, get_log_message (LOG_MESSAGE_FUNCSTART),
			__FILE__, __LINE__, __FUNCTION__
		);
	}

	services_admin_t	*sa_p;
	services_admin_t	*sa_delete_p;

	assert (nickname != NULL);

	sa_p = services_admin_h;
	while (sa_p)
	{
		if (!strcasecmp (sa_p->nickname, nickname))
		{
			ircsp_log (F_MAINLOG,
				"[%s:%d:%s()]: Deleting Services Admin [%s] found at [%p]\n",
				__FILE__, __LINE__, __FUNCTION__,
				sa_p->nickname, sa_p->nickname
			);

			if (services_admin_h == sa_p)
			{
				services_admin_h = sa_p->next;
			}
			else
			{
				for (sa_delete_p = services_admin_h; (sa_delete_p->next != sa_p) &&
					sa_delete_p; sa_delete_p = sa_delete_p->next);

				sa_delete_p->next = sa_p->next;
			}

			ircsp_free (sa_p->nickname);
			sa_p->flags		= 0;
			sa_p->nickname_p	= NULL;
			sa_p->nickinfo_p	= NULL;
			ircsp_free (sa_p);

			if (DEBUG)
			{
				ircsp_log (F_MAINLOG, get_log_message (LOG_MESSAGE_FUNCEND),
					__FILE__, __LINE__, __FUNCTION__
				);
			}

			return ;
		}

		sa_p = sa_p->next;
	}

	if (DEBUG)
	{
		ircsp_log (F_MAINLOG, get_log_message (LOG_MESSAGE_FUNCEND),
			__FILE__, __LINE__, __FUNCTION__
		);
	}
}


/*
---------------------------------------------------------------------------------------------------------------------------------
    FUNCTION       |    services_admin_unregister ()
                   |
    DESCRIPTION    |    This function will be a registered callback used by things outside of this module to unregister
                   |    a services admin, this will bascially NULL the pointers for nickinfo and nickname.
                   |
    RETURNS        |    Nothing
---------------------------------------------------------------------------------------------------------------------------------
*/
void
services_admin_unregister (nickname_p)
	struct _nickname	*nickname_p;
{
	if (DEBUG)
	{
		ircsp_log (F_MAINLOG, get_log_message (LOG_MESSAGE_FUNCEND),
			__FILE__, __LINE__, __FUNCTION__
		);
	}

	services_admin_t	*services_admin_p;

	assert (nickname_p != NULL);
	assert (nickname_p->nickname != NULL);

	if (nickname_p->flags & NICK_IS_ADMIN)
	{
		services_admin_p = ServicesAdmin_Search (nickname_p->nickname);
		assert (services_admin_p != NULL);

		services_admin_p->nickinfo_p = NULL;
		services_admin_p->nickname_p = NULL;

		nickname_p->flags &= ~NICK_IS_ADMIN;
	}

	if (DEBUG)
	{
		ircsp_log (F_MAINLOG, get_log_message (LOG_MESSAGE_FUNCEND),
			__FILE__, __LINE__, __FUNCTION__
		);
	}
}


/*
---------------------------------------------------------------------------------------------------------------------------------
    FUNCTION       |    ServicesRoot_Create ()
                   |
    DESCRIPTION    |    This function will create a new ServicesRoot object in our linked list.
                   |
    RETURNS        |    Returns a pointer to the object on success, otherwise we return NULL.
---------------------------------------------------------------------------------------------------------------------------------
*/
services_root_t
*ServicesRoot_Create (nickname)
	char		*nickname;
{
	if (DEBUG)
	{
		ircsp_log (F_MAINLOG, get_log_message (LOG_MESSAGE_FUNCSTART),
			__FILE__, __LINE__, __FUNCTION__
		);
	}

	services_root_t		*sr_p;

	assert (nickname != NULL);

	sr_p = (services_root_t *)ircsp_calloc (1, sizeof (services_root_t));
	if (!sr_p)
	{
		ircsp_log (F_MAINLOG, get_log_message (LOG_MESSAGE_OUTOFMEMORY),
			__FILE__, __LINE__, __FUNCTION__
		);

		if (DEBUG)
		{
			ircsp_log (F_MAINLOG, get_log_message (LOG_MESSAGE_FUNCEND),
				__FILE__, __LINE__, __FUNCTION__
			);
		}

		return NULL;
	}

	sr_p->nickname = (char *)ircsp_calloc (1, strlen (nickname) + 5);
	if (!sr_p->nickname)
	{
		ircsp_log (F_MAINLOG, get_log_message (LOG_MESSAGE_OUTOFMEMORY),
			__FILE__, __LINE__, __FUNCTION__
		);

		if (DEBUG)
		{
			ircsp_log (F_MAINLOG, get_log_message (LOG_MESSAGE_FUNCEND),
				__FILE__, __LINE__, __FUNCTION__
			);
		}

		return NULL;
	}

	strcpy (sr_p->nickname, nickname);
	sr_p->flags		= 0;
	sr_p->nickname_p	= NULL;
	sr_p->nickinfo_p	= NULL;

	ircsp_log (F_MAINLOG,
		"[%s:%d:%s()]: Services Root [%s] added at [%p]\n",
		__FILE__, __LINE__, __FUNCTION__,
		sr_p->nickname, sr_p->nickname
	);

	if (!services_root_h)
	{
		services_root_h = sr_p;
		sr_p->next = NULL;
	}
	else
	{
		sr_p->next = services_root_h;
		services_root_h = sr_p;
	}

	if (DEBUG)
	{
		ircsp_log (F_MAINLOG, get_log_message (LOG_MESSAGE_FUNCEND),
			__FILE__, __LINE__, __FUNCTION__
		);
	}

	return sr_p;
}


/*
---------------------------------------------------------------------------------------------------------------------------------
    FUNCTION       |    ServicesRoot_Search ()
                   |
    DESCRIPTION    |    This function will search the ServicesRoot list for a specific object.
                   |
    INPUTS         |    nickname = the nickname of the object we are searching for
                   |
    RETURNS        |    Returns a pointer to the object if there is a match
                   |    Returns NULL if we find nothing
---------------------------------------------------------------------------------------------------------------------------------
*/
services_root_t
*ServicesRoot_Search (nickname)
	char		*nickname;
{
	if (DEBUG)
	{
		ircsp_log (F_MAINLOG, get_log_message (LOG_MESSAGE_FUNCSTART),
			__FILE__, __LINE__, __FUNCTION__
		);
	}

	services_root_t		*sr_p;

	assert (nickname != NULL);

	sr_p = services_root_h;
	while (sr_p)
	{
		if (!strcasecmp (sr_p->nickname, nickname))
		{
			ircsp_log (F_MAINLOG,
				"[%s:%d:%s()]: Services Root [%s] found at [%p]\n",
				__FILE__, __LINE__, __FUNCTION__,
				sr_p->nickname, sr_p->nickname
			);

			if (DEBUG)
			{
				ircsp_log (F_MAINLOG, get_log_message (LOG_MESSAGE_FUNCEND),
					__FILE__, __LINE__, __FUNCTION__
				);
			}

			return sr_p;
		}

		sr_p = sr_p->next;
	}

	if (DEBUG)
	{
		ircsp_log (F_MAINLOG, get_log_message (LOG_MESSAGE_FUNCEND),
			__FILE__, __LINE__, __FUNCTION__
		);
	}

	return NULL;
}


/*
---------------------------------------------------------------------------------------------------------------------------------
    FUNCTION       |    ServicesRoot_Remove ()
                   |
    DESCRIPTION    |    This function will delete a ServicesRoot object from the linked list.
                   |
    INPUT          |    nickname = The nickname of the ServicesRoot object we are deleting.
                   |
    RETURNS        |    Returns nothing.
---------------------------------------------------------------------------------------------------------------------------------
*/
void
ServicesRoot_Remove (nickname)
	char		*nickname;
{
	if (DEBUG)
	{
		ircsp_log (F_MAINLOG, get_log_message (LOG_MESSAGE_FUNCSTART),
			__FILE__, __LINE__, __FUNCTION__
		);
	}

	services_root_t		*sr_p;
	services_root_t		*sr_delete_p;

	/* The nickname variable should NEVER be null */
	assert (nickname != NULL);

	sr_p = services_root_h;
	while (sr_p)
	{
		if (!strcasecmp (sr_p->nickname, nickname))
		{
			ircsp_log (F_MAINLOG,
				"[%s:%d:%s()]: Deleting Services Root [%s] found at [%p]\n",
				__FILE__, __LINE__, __FUNCTION__,
				sr_p->nickname, sr_p->nickname
			);

			if (services_root_h == sr_p)
			{
				services_root_h = sr_p->next;
			}
			else
			{
				for (sr_delete_p = services_root_h; (sr_delete_p->next != sr_p) &&
					sr_delete_p; sr_delete_p = sr_delete_p->next);

				sr_delete_p->next = sr_p->next;
			}

			ircsp_free (sr_p->nickname);
			sr_p->flags		= 0;
			sr_p->nickname_p	= NULL;
			sr_p->nickinfo_p	= NULL;
			ircsp_free (sr_p);

			if (DEBUG)
			{
				ircsp_log (F_MAINLOG, get_log_message (LOG_MESSAGE_FUNCEND),
					__FILE__, __LINE__, __FUNCTION__
				);
			}

			return ;
		}

		sr_p = sr_p->next;
	}

	if (DEBUG)
	{
		ircsp_log (F_MAINLOG, get_log_message (LOG_MESSAGE_FUNCEND),
			__FILE__, __LINE__, __FUNCTION__
		);
	}
}


/*
---------------------------------------------------------------------------------------------------------------------------------
    FUNCTION       |    services_root_unregister ()
                   |
    DESCRIPTION    |    This function will be a registered callback used by things outside of this module to unregister
                   |    a services root, this will bascially NULL the pointers for nickinfo and nickname.
                   |
    RETURNS        |    Nothing
---------------------------------------------------------------------------------------------------------------------------------
*/
void
services_root_unregister (nickname_p)
	struct _nickname	*nickname_p;
{
	if (DEBUG)
	{
		ircsp_log (F_MAINLOG, get_log_message (LOG_MESSAGE_FUNCEND),
			__FILE__, __LINE__, __FUNCTION__
		);
	}

	services_root_t		*services_root_p;

	assert (nickname_p != NULL);
	assert (nickname_p->nickname != NULL);

	if (nickname_p->flags & NICK_IS_ROOT)
	{
		services_root_p = ServicesRoot_Search (nickname_p->nickname);
		assert (services_root_p != NULL);

		services_root_p->nickinfo_p = NULL;
		services_root_p->nickname_p = NULL;

		nickname_p->flags &= ~NICK_IS_ROOT;
	}

	if (DEBUG)
	{
		ircsp_log (F_MAINLOG, get_log_message (LOG_MESSAGE_FUNCEND),
			__FILE__, __LINE__, __FUNCTION__
		);
	}
}
