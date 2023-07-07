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
#include	"misc.h"
#include	"conf.h"
#include	"log.h"
#include	"socket.h"
#include	"ircsp.h"
#include	"users.h"


/* Debugging */
#include	"debug.h"


/* Compiler Generated Includes */
#include	"config.h"


/*
---------------------------------------------------------------------------------------------------------------------------------
 FUNCTION       |    setflags ()
                |
 DESCRIPTION    |    Used to set flags
                |
                |        SETFL_SET    -    Clears flags and sets the given ones.
                |        SETFL_DELETE -    Deletes given flags, if they are set
                |        SETFL_APPEND -    Adds flags to the flag var if they are not already set.
                |
                |    To use multiple flags be sure to seperate them via |
                |
                |        Example:   setflags (userp->flags, SETFL_DELETE, NICK_IS_OPER|NICK_IS_ADMIN);
                |
 RETURNS        |
---------------------------------------------------------------------------------------------------------------------------------
*/
u_int setflags (u_int *flagvar, u_int mode, u_int flags)
{
	if (DEBUG)
	{
		ircsp_log (F_MAINLOG, get_log_message (LOG_MESSAGE_FUNCSTART),
			__FILE__, __LINE__, __FUNCTION__);
	}


	if (mode == SETFL_SET)
	{
		return (*flagvar = flags);
	}

	if (mode == SETFL_DELETE)
	{
		return (*flagvar &= ~flags);
	}

	if (mode == SETFL_APPEND)
	{
		return (*flagvar |= flags);
	}

	if (DEBUG)
	{
		ircsp_log (F_MAINLOG, get_log_message (LOG_MESSAGE_FUNCEND),
			__FILE__, __LINE__, __FUNCTION__);
	}

	return TRUE;
}


/*
---------------------------------------------------------------------------------------------------------------------------------
 FUNCTION       |    ircsp_warning ()
                |
 DESCRIPTION    |
                |
 RETURNS        |
---------------------------------------------------------------------------------------------------------------------------------
*/
void ircsp_warning (char *format, ...)
{
	nickname_t *myuserp;

	char buf[256];
	va_list msg;

	va_start(msg, format);
	vsprintf(buf, format, msg);

	myuserp = Nickname_Find (config->uworld_nickname);
	if (myuserp)
		ssprintf(s, "%s O %s :\002WARNING\002 - %s",
			myuserp->numeric, config->operchannel_name, buf);
}


/*
---------------------------------------------------------------------------------------------------------------------------------
 FUNCTION       |    checkGlines ()
                |
 DESCRIPTION    |
                |
 RETURNS        |
---------------------------------------------------------------------------------------------------------------------------------
*/
/*
void checkGlines (void)
{
	Gline *glinep;
	time_t curtime;

	curtime = time(NULL);

	for (glinep = glineHEAD; glinep; glinep = glinep->next)
	{
		if (glinep->expires < curtime)
		{
			ssprintf(s, "%s WA :Removing Expired GLINE for [%s]\n",
				config->server_numeric, glinep->mask);


			ssprintf(s, "%s GL * -%s %lu %lu\n",
				config->server_numeric, glinep->mask, (time(NULL) - 60),
				time(NULL));

			delGline(glinep->mask);
		}
	}

	uw_saveGlineDB ();
}
*/
