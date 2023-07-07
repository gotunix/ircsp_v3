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
#include	<string.h>
#include	<unistd.h>
#include	<stdarg.h>
#include	<signal.h>
#include	<fcntl.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<sys/time.h>
#include	<sys/wait.h>
#include	<arpa/inet.h>
#include	<netdb.h>
#include	<ctype.h>
#include	<time.h>
#include	<errno.h>
#include	<netinet/in.h>


/* IRCSP Core Includes */
#include	"rehash.h"
#include	"synch.h"
#include	"misc.h"
#include	"conf.h"
#include	"alloc.h"
#include	"log.h"
#include	"text.h"
#include	"socket.h"
#include	"ircsp.h"
#include	"users.h"


/* Debugging */
#include	"debug.h"


/* Compiler Generated Includes */
#include	"config.h"


/*
---------------------------------------------------------------------------------------------------------------------------------
 FUNCTION       |    ircsp_rehash
                |
 DESCRIPTION    |    Lets drop all the glines and admins from memory and reload them from our database. And eventually do the same
                |    with our config file.  However some config option will require a restart, such as UPLINKS, SERVER names, etc.
                |
 RETURNS        |
---------------------------------------------------------------------------------------------------------------------------------
*/
void ircsp_rehash (void)
{
	nickname_t *myuserp;
	nickname_t *userp;
//	Gline *glinep;

//	LOG (MAINLOG, "[%s:%d:%s()]:  Signal HUP (Rehash) received from SHELL\n",
//		__FILE__, __LINE__, __FUNCTION__);


	/* This shouldn't be an issue but lets make sure we are connected
	   and that we are already done with our initial burst. */

	if (ircsp->connected)
	{
		if (ircsp->initial_burst)
		{
			myuserp = Nickname_Find (config->uworld_nickname);
			if (!myuserp)
			{
//				LOG (MAINLOG, "[%s:%d:%s()]:  Failed to find UWORLD nickname\n",
//					__FILE__, __LINE__, __FUNCTION__);
				return ;
			}

//			ircsp_notice (
//				"@(#) - (%s:%d) %s():  Signal HUP Received.  (Rehashing)\n",
//				__FILE__, __LINE__, __FUNCTION__);

			ircsp->rehashing = 1;


			/* Remove GLINES from memory */

//			for (glinep = glineHEAD; glinep; glinep = glinep->next)
//			{
//				delGline (glinep->mask);
//			}

			/* Reload Administrators from DB */

//			loadAdminDB ();

			/* Reload GLINES from DB */

//			uw_loadGlineDB ();

			/* Sync ! */

			synch ();

			ircsp->rehashing = 0;
		}
	}

//	LOG (MAINLOG, "[%s:%d:%s()]:  End of signal HUP\n",
//		__FILE__, __LINE__, __FUNCTION__);
}
