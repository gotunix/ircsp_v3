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
#include	"conf.h"
#include	"alloc.h"
#include	"match.h"
#include	"log.h"


/* Debugging */
#include	"debug.h"


/* Compiler Generated Includes */
#include	"config.h"


/*
---------------------------------------------------------------------------------------------------------------------------------
 FUNCTION       |    match ()
                |
 DESCRIPTION    |    Long yes, but it is iterative versus recursive.  Recursive would mean multiple function calls which costs
                |    CPU time and memory. Overall, this method is faster than the recursive match, though the source is longer.
                |
 RETURNS        |    0 if match
                |    1 if no match
---------------------------------------------------------------------------------------------------------------------------------
*/
int match(mask, name)
	const char	*mask;
	const char	*name;
{
	if ((DEBUG) && (DEBUG_FUNC))
	{
		ircsp_log (F_MAINLOG,
			get_log_message (LOG_MESSAGE_FUNCSTART),
			__FILE__, __LINE__, __FUNCTION__
		);
	}

	const char	*m = mask;
	const char	*n = name;
	const char	*ma = mask;
	const char	*na = name;
	int		wild = 0;
	int		q = 0;

	while (1)
	{
		if (*m == '*')
		{
			while (*m == '*')
			{
				m++;
			}
			wild = 1;
			ma = m;
			na = n;
		}

		if (!*m)
		{
			if (!*n)
			{
				if ((DEBUG) && (DEBUG_FUNC))
				{
					ircsp_log (F_MAINLOG,
						get_log_message (LOG_MESSAGE_FUNCEND),
						__FILE__, __LINE__, __FUNCTION__
					);
				}

				return 0;
			}

			for (m--; (m > mask) && (*m == '?'); m--);
			if ((*m == '*') && (m > mask) && (m[-1] != '\\'))
			{
				if ((DEBUG) && (DEBUG_FUNC))
				{
					ircsp_log (F_MAINLOG,
						get_log_message (LOG_MESSAGE_FUNCEND),
						__FILE__, __LINE__, __FUNCTION__
					);
				}

				return 0;
			}

			if (!wild)
			{
				if ((DEBUG) && (DEBUG_FUNC))
				{
					ircsp_log (F_MAINLOG,
						get_log_message (LOG_MESSAGE_FUNCEND),
						__FILE__, __LINE__, __FUNCTION__
					);
				}

				return 1;
			}

			m = ma;
			n = ++na;
		}
		else if (!*n)
		{
			while (*m == '*')
			{
				m++;
			}

			if ((DEBUG) && (DEBUG_FUNC))
			{
				ircsp_log (F_MAINLOG,
					get_log_message (LOG_MESSAGE_FUNCEND),
					__FILE__, __LINE__, __FUNCTION__
				);
			}

			return (*m != 0);
		}

		if ((*m == '\\') && ((m[1] == '*') || (m[1] == '?')))
		{
			m++;
			q = 1;
		}
		else
		{
			q = 0;
		}

		if ((tolower(*m) != tolower(*n)) && ((*m != '?') || q))
		{
			if (!wild)
			{
				if ((DEBUG) && (DEBUG_FUNC))
				{
					ircsp_log (F_MAINLOG,
						get_log_message (LOG_MESSAGE_FUNCEND),
						__FILE__, __LINE__, __FUNCTION__
					);
				}

				return 1;
			}

			m = ma;
			n = ++na;
		}
		else
		{
			if (*m)
			{
				m++;
			}

			if (*n)
			{
				n++;
			}
		}
	}

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
 FUNCTION       |    match_wild ()
                |
 DESCRIPTION    |
                |
                |
 RETURNS        |    0 if match
                |    1 if no match
---------------------------------------------------------------------------------------------------------------------------------
*/
int match_wild(const char *pattern, const char *str)
{
	if ((DEBUG) && (DEBUG_FUNC))
	{
		ircsp_log (F_MAINLOG,
			get_log_message (LOG_MESSAGE_FUNCEND),
			__FILE__, __LINE__, __FUNCTION__
		);
	}

	char c;
	const char *s;

	/*
	-------------------------------------------------------------------------------------------------------------------------
	This will eventully terminate: either by *pattern == 0, or by a trailing '*'
	-------------------------------------------------------------------------------------------------------------------------
	*/

	for (;;)
	{
		switch (c = *pattern++)
		{
			case 0:
				if (!*str)
				{
					if ((DEBUG) && (DEBUG_FUNC))
					{
						ircsp_log (F_MAINLOG,
							get_log_message (LOG_MESSAGE_FUNCEND),
							__FILE__, __LINE__, __FUNCTION__
						);
					}

					return 1;
				}

				if ((DEBUG) && (DEBUG_FUNC))
				{
					ircsp_log (F_MAINLOG,
						get_log_message (LOG_MESSAGE_FUNCEND),
						__FILE__, __LINE__, __FUNCTION__
					);
				}

				return 0;

			case '?':
				++str;
				break;

			case '*':
				if (!*pattern)
				{
					if ((DEBUG) && (DEBUG_FUNC))
					{
						ircsp_log (F_MAINLOG,
							get_log_message (LOG_MESSAGE_FUNCEND),
							__FILE__, __LINE__, __FUNCTION__
						);
					}

					return 1;	/* trailing '*' matches everything else */
				}

				s = str;
				while (*s)
				{
					if (*s == *pattern && match_wild(pattern, s))
					{
						if ((DEBUG) && (DEBUG_FUNC))
						{
							ircsp_log (F_MAINLOG,
								get_log_message (LOG_MESSAGE_FUNCEND),
								__FILE__, __LINE__, __FUNCTION__
							);
						}

						return 1;
					}

					++s;
				}
				break;

			default:
				if (*str++ != c)
				{
					if ((DEBUG) && (DEBUG_FUNC))
					{
						ircsp_log (F_MAINLOG,
							get_log_message (LOG_MESSAGE_FUNCEND),
							__FILE__, __LINE__, __FUNCTION__
						);
					}

					return 0;
				}

				break;
		}
	}
}
