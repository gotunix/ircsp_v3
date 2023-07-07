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
#include	<stdint.h>
#include	<unistd.h>
#include	<string.h>
#include	<errno.h>
#include	<assert.h>


/* IRCSP Core Includes */
#include	"log.h"
#include	"alloc.h"


/* Debbing */
#include	"debug.h"


/* Compiler Generated Includes */
#include	"config.h"


/*
---------------------------------------------------------------------------------------------------------------------------------
 FUNCTION       |    ircsp_malloc ()
                |
 DESCRIPTION    |
                |
 RETURNS        |
---------------------------------------------------------------------------------------------------------------------------------
*/
void		*ircsp_malloc					(size)
	long		size;
{
	LOG (main_logfile_p, LOG_FUNC,
		get_log_message (LOG_MESSAGE_FUNCSTART),
		__FILE__, __LINE__, __FUNCTION__
	);

	void		*data;

	if (size == 0)
	{
		LOG (main_logfile_p, LOG_WARNING,
			"[WARN] - [%s:%d:%s()]:  Attempted to allocate a NULL pointer\n",
			__FILE__, __LINE__, __FUNCTION__
		);


		LOG (main_logfile_p, LOG_FUNC,
			get_log_message (LOG_MESSAGE_FUNCEND),
			__FILE__, __LINE__, __FUNCTION__
		);

		return NULL;
	}
	else
	{
		data = malloc (size);
		if (data == NULL)
		{
			if (errno == ENOMEM)
			{
				LOG (main_logfile_p, LOG_CRITICAL,
					get_log_message (LOG_MESSAGE_OUTOFMEMORY),
					__FILE__, __LINE__, __FUNCTION__
				);
			}
			else
			{
				LOG (main_logfile_p, LOG_CRITICAL,
					"[CRIT] - [%s:%d:%s()]:  Unknown error while trying to allocate memory",
					__FILE__, __LINE__, __FUNCTION__
				);
			}

			LOG (main_logfile_p, LOG_FUNC,
				get_log_message (LOG_MESSAGE_FUNCEND),
				__FILE__, __LINE__, __FUNCTION__
			);

			return NULL;
		}
		else
		{
			LOG (main_logfile_p, LOG_INFO,
				"[INFO] - [%s:%d:%s()]: Allocated [%ld] bytes at [%p]\n",
				__FILE__, __LINE__, __FUNCTION__, size, data
			);

			LOG (main_logfile_p, LOG_FUNC,
				get_log_message (LOG_MESSAGE_FUNCEND),
				__FILE__, __LINE__, __FUNCTION__
			);

			return data;
		}
	}
}


/*
---------------------------------------------------------------------------------------------------------------------------------
 FUNCTION       |    ircsp_calloc ()
                |
 DESCRIPTION    |
                |
 RETURNS        |
---------------------------------------------------------------------------------------------------------------------------------
*/
void		*ircsp_calloc					(els, elsize)
	long		els;
	long		elsize;
{
	LOG (main_logfile_p, LOG_FUNC,
		get_log_message (LOG_MESSAGE_FUNCSTART),
		__FILE__, __LINE__, __FUNCTION__
	);

	void		*data;

	if ((els == 0) || (elsize == 0))
	{
		LOG (main_logfile_p, LOG_WARNING,
			"[WARN] - [%s:%d:%s()]:  Attempted to allocate a NULL pointer\n",
			__FILE__, __LINE__, __FUNCTION__
		);

		LOG (main_logfile_p, LOG_FUNC,
			get_log_message (LOG_MESSAGE_FUNCEND),
			__FILE__, __LINE__, __FUNCTION__
		);

		return NULL;
	}
	else
	{
		data = calloc (elsize, els);
		if (data == NULL)
		{
			if (errno == ENOMEM)
			{
				LOG (main_logfile_p, LOG_CRITICAL,
					get_log_message (LOG_MESSAGE_OUTOFMEMORY),
					__FILE__, __LINE__, __FUNCTION__
				);
			}
			else
			{
				LOG (main_logfile_p, LOG_CRITICAL,
					"[CRIT] - [%s:%d:%s()]:  Unknown error while trying to allocate memory\n",
					__FILE__, __LINE__, __FUNCTION__
				);
			}

			LOG (main_logfile_p, LOG_FUNC,
				get_log_message (LOG_MESSAGE_FUNCEND),
				__FILE__, __LINE__, __FUNCTION__
			);

			return NULL;
		}
		else
		{
			LOG (main_logfile_p, LOG_INFO,
				"[INFO] - [%s:%d:%s()]:  Allocated [%ld] bytes at [%p]\n",
				__FILE__, __LINE__, __FUNCTION__, (elsize * els), data
			);

			LOG (main_logfile_p, LOG_FUNC,
				get_log_message (LOG_MESSAGE_FUNCEND),
				__FILE__, __LINE__, __FUNCTION__
			);

			return data;
		}
	}
}


/*
---------------------------------------------------------------------------------------------------------------------------------
 FUNCTION       |    ircsp_realloc ()
                |
 DESCRIPTION    |
                |
 RETURNS        |
---------------------------------------------------------------------------------------------------------------------------------
*/
void		*ircsp_realloc					(ptr, size)
	void		*ptr;
	long		size;
{
	LOG (main_logfile_p, LOG_FUNC,
		get_log_message (LOG_MESSAGE_FUNCSTART),
		__FILE__, __LINE__, __FUNCTION__
	);

	void		*data;

	if (size == 0)
	{
		LOG (main_logfile_p, LOG_WARNING,
			"[WARN] - [%s:%d:%s()]:  Attempted to realloc() a NULL pointer\n",
			__FILE__, __LINE__, __FUNCTION__
		);

		if ((DEBUG) && (DEBUG_FUNC))
		{
			ircsp_log (F_MAINLOG,
				get_log_message (LOG_MESSAGE_FUNCEND),
				__FILE__, __LINE__, __FUNCTION__
			);
		}

		return NULL;
	}
	else
	{
		data = realloc (ptr, size);
		if (data == NULL)
		{
			if (errno == ENOMEM)
			{
				LOG (main_logfile_p, LOG_CRITICAL,
					get_log_message (LOG_MESSAGE_OUTOFMEMORY),
					__FILE__, __LINE__, __FUNCTION__
				);
			}
			else
			{
				LOG (main_logfile_p, LOG_CRITICAL,
					"[CRIT] - [%s:%d:%s()]:  Unknown error when attempting to realloc memory\n",
					__FILE__, __LINE__, __FUNCTION__
				);
			}

			LOG (main_logfile_p, LOG_FUNC,
				get_log_message (LOG_MESSAGE_FUNCEND),
				__FILE__, __LINE__, __FUNCTION__
			);

			return NULL;
		}
		else
		{
			LOG (main_logfile_p, LOG_INFO,
				"[INFO] - [%s:%d:%s()]:  Reallocated to [%ld] bytes at [%p]\n",
				__FILE__, __LINE__, __FUNCTION__, size, data
			);

			LOG (main_logfile_p, LOG_FUNC,
				get_log_message (LOG_MESSAGE_FUNCEND),
				__FILE__, __LINE__, __FUNCTION__
			);

			return data;
		}
	}
}


/*
---------------------------------------------------------------------------------------------------------------------------------
 FUNCTION       |    ircsp_free ()
                |
 DESCRIPTION    |
                |
 RETURNS        |
---------------------------------------------------------------------------------------------------------------------------------
*/
void		ircsp_free					(ptr)
	void		*ptr;
{
	LOG (main_logfile_p, LOG_FUNC,
		get_log_message (LOG_MESSAGE_FUNCSTART),
		__FILE__, __LINE__, __FUNCTION__
	);

	if (ptr == NULL)
	{
		LOG (main_logfile_p, LOG_WARNING,
			"[WARN] - [%s:%d:%s()]:  Attempted to free() a NULL pointer\n",
			__FILE__, __LINE__, __FUNCTION__
		);
	}
	else
	{
		LOG (main_logfile_p, LOG_INFO,
			"[INFO] - [%s:%d:%s()]:  Released memory at [%p]\n",
			__FILE__, __LINE__, __FUNCTION__, ptr
		);

		free (ptr);
	}

	LOG (main_logfile_p, LOG_FUNC,
		get_log_message (LOG_MESSAGE_FUNCEND),
		__FILE__, __LINE__, __FUNCTION__
	);
}
