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
#include	"ircsp.h"
#include	"conf.h"
#include	"alloc.h"
#include	"log.h"
#include	"socket.h"


/* Debugging */
#include	"debug.h"


/* Compiler Generated Includes */
#include	"config.h"


/*
---------------------------------------------------------------------------------------------------------------------------------
 VARIABLES
---------------------------------------------------------------------------------------------------------------------------------
*/
u_long	btx		= 0;
u_long	brx		= 0;


/*
---------------------------------------------------------------------------------------------------------------------------------
 FUNCTION       |    ircsp_connect ()
                |
 DESCRIPTION    |
                |
 RETURNS        |
---------------------------------------------------------------------------------------------------------------------------------
*/
void ircsp_connect (void)
{
	if ((DEBUG) && (DEBUG_FUNC))
	{
		ircsp_log (F_MAINLOG,
			get_log_message (LOG_MESSAGE_FUNCSTART),
			__FILE__, __LINE__, __FUNCTION__
		);
	}

	struct hostent *hp;
	struct sockaddr_in sin;

	if ((DEBUG) && (DEBUG_SOCKET))
	{
		ircsp_log (F_MAINLOG,
			"[%s:%d:%s()]: Connecint to UPLINK [%s] PORT [%d]\n",
			__FILE__, __LINE__, __FUNCTION__, config->uplink,
			config->uplink_port
		);
	}

	if ((hp = gethostbyname(config->uplink)) == NULL)
	{
		if ((DEBUG) && (DEBUG_SOCKET))
		{
			ircsp_log (F_MAINLOG,
				"[%s:%d:%s()]: Unknown hostname [%s]\n",
				__FILE__, __LINE__, __FUNCTION__, config->uplink
			);
		}

		ircsp_panic ("ERROR - Unknown hostname");
	}

	if ((s = socket (AF_INET, SOCK_STREAM, 0)) < 0)
	{
		if ((DEBUG) && (DEBUG_SOCKET))
		{
			ircsp_log (F_MAINLOG,
				"[%s:%d:%s()]: Client:Socket\n",
				__FILE__, __LINE__, __FUNCTION__
			);
		}

		ircsp_panic ("Client:Socket");
	}

	sin.sin_family = AF_INET;
	sin.sin_port = htons(config->uplink_port);
	bcopy (hp->h_addr, &sin.sin_addr, hp->h_length);

	if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
	{
		if ((DEBUG) && (DEBUG_SOCKET))
		{
			ircsp_log (F_MAINLOG,
				"[%s:%d:%s()]: Client:Connect\n",
				__FILE__, __LINE__, __FUNCTION__
			);
		}

		ircsp_panic ("Client:Connect");
	}

	serv = fdopen (s, "a+");
	ircsp->connected = 1;

	if ((DEBUG) && (DEBUG_FUNC))
	{
		ircsp_log (F_MAINLOG,
			get_log_message (LOG_MESSAGE_FUNCEND),
			__FILE__, __LINE__, __FUNCTION__
		);
	}
}


/*
---------------------------------------------------------------------------------------------------------------------------------
 FUNCTION       |    ssprintf ()
                |
 DESCRIPTION    |
                |
 RETURNS        |
---------------------------------------------------------------------------------------------------------------------------------
*/
int ssprintf (int socket, const char *fmt, ...)
{
	if ((DEBUG) && (DEBUG_FUNC))
	{
		ircsp_log (F_MAINLOG,
			get_log_message (LOG_MESSAGE_FUNCSTART),
			__FILE__, __LINE__, __FUNCTION__
		);
	}

	int len;
	char *string;
	va_list args;

	string = (char *)ircsp_calloc(1, BUFFER_SIZE);
	va_start (args, fmt);
	vsnprintf (string, BUFFER_SIZE, fmt, args);
	va_end (args);

	if ((DEBUG) && (DEBUG_SOCKET))
	{
		ircsp_log (F_MAINLOG,
			"[%s:%d:%s()]: Socket(SEND): %s",
			__FILE__, __LINE__, __FUNCTION__, string
		);
	}

	send (socket, string, strlen(string), 0);
	len = strlen (string);
	ircsp_free (string);

	if ((DEBUG) && (DEBUG_FUNC))
	{
		ircsp_log (F_MAINLOG,
			get_log_message (LOG_MESSAGE_FUNCEND),
			__FILE__, __LINE__, __FUNCTION__
		);
	}

	return (len);
}


/*
---------------------------------------------------------------------------------------------------------------------------------
 FUNCTION       |    ssread ()
                |
 DESCRIPTION    |
                |
 RETURNS        |
---------------------------------------------------------------------------------------------------------------------------------
*/
int ssread (s, buf)
	int s;
	char *buf;
{
	if ((DEBUG) && (DEBUG_FUNC))
	{
		ircsp_log (F_MAINLOG,
			get_log_message (LOG_MESSAGE_FUNCSTART),
			__FILE__, __LINE__, __FUNCTION__
		);
	}

	char inc;
	int bufnum, n;
	fd_set fds;
	struct timeval wait;


	bufnum = 0;

	if (s == -1)
	{
		if ((DEBUG) && (DEBUG_FUNC))
		{
			ircsp_log (F_MAINLOG,
				get_log_message (LOG_MESSAGE_FUNCEND),
				__FILE__, __LINE__, __FUNCTION__
			);
		}

		return (-1);
	}

	wait.tv_sec = 0L;
	wait.tv_usec = 2500L;
	FD_ZERO(&fds);
	FD_SET(s, &fds);

	if (select(s+1, &fds, NULL, 0, &wait) > 0)
	{
		do
		{
			n = read(s, &inc, 1);
			if (n == 0)
			{
				if ((DEBUG) && (DEBUG_FUNC))
				{
					ircsp_log (F_MAINLOG,
						get_log_message (LOG_MESSAGE_FUNCEND),
						__FILE__, __LINE__, __FUNCTION__
					);
				}

				return -1;
			}

			if (bufnum < BUFFER_SIZE - 1)
			{
				buf[bufnum++] = inc;
			}
		}
		while (inc != '\n');
		buf[bufnum] = '\0';

		brx += bufnum;

		if ((DEBUG) && (DEBUG_FUNC))
		{
			ircsp_log (F_MAINLOG,
				get_log_message (LOG_MESSAGE_FUNCEND),
				__FILE__, __LINE__, __FUNCTION__
			);
		}

		return bufnum;
	}

	if ((DEBUG) && (DEBUG_FUNC))
	{
		ircsp_log (F_MAINLOG,
			get_log_message (LOG_MESSAGE_FUNCEND),
			__FILE__, __LINE__, __FUNCTION__
		);
	}

	return 0;
}
