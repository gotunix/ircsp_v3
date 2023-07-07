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
#include	<stdint.h>
#include	<stdarg.h>
#include	<string.h>
#include	<time.h>
#include	<errno.h>
#include	<assert.h>


/* IRCSP Core Includes */
#include	"alloc.h"
#include	"log.h"
#include	"ircsp.h"
#include	"conf.h"
#include	"modules.h"
#include	"scheduler.h"


/* Debugging */
#include	"debug.h"


/* Compiler Generated Includes */
#include	"config.h"


/*
---------------------------------------------------------------------------------------------------------------------------------
    FUNCTION       |    Scheduler_add ()
                   |
    DESCRIPTION    |    This function will add a new task to be run at a specified interval
                   |
    RETURNS        |    Pointer to the Scheduler List if successfull, orhterwise NULL
---------------------------------------------------------------------------------------------------------------------------------
*/
scheduler_t *Scheduler_add (name, interval, func)
	char		*name;
	int		interval;
	int		(*func)(void);
{
	if ((DEBUG) && (DEBUG_FUNC))
	{
		ircsp_log (F_MAINLOG,
			get_log_message (LOG_MESSAGE_FUNCSTART),
			__FILE__, __LINE__, __FUNCTION__
		);
	}

	scheduler_t	*p_Scheduler;

	assert (name != NULL);
	assert (interval != 0);

	p_Scheduler = (scheduler_t *)ircsp_calloc (1, sizeof (scheduler_t));
	p_Scheduler->task_name = (char *)ircsp_calloc (1, strlen (name) + 5);
	if ((!p_Scheduler) || (!p_Scheduler->task_name))
	{
		if (DEBUG)
		{
			if (DEBUG_ALLOC)
			{
				ircsp_log (F_MAINLOG,
					"[%s:%d:%s()]: Failed to allocate memory for Scheduler_p\n",
					__FILE__, __LINE__, __FUNCTION__
				);
			}

			if (DEBUG_FUNC)
			{
				ircsp_log (F_MAINLOG,
					get_log_message (LOG_MESSAGE_FUNCEND),
					__FILE__, __LINE__, __FUNCTION__
				);
			}
		}

		return NULL;
	}

	strcpy (p_Scheduler->task_name, name);
	p_Scheduler->task_last_run = time (NULL);
	p_Scheduler->task_interval = interval;

	p_Scheduler->func = func;

	if ((DEBUG) && (DEBUG_SCHEDULER))
	{
		ircsp_log (F_MAINLOG,
			"[%s:%d:%s()]: Scheduled task [%s] added at [%p]\n",
			__FILE__, __LINE__, __FUNCTION__, p_Scheduler->task_name,
			p_Scheduler->task_name
		);
	}

	if (!scheduler_h)
	{
		scheduler_h = p_Scheduler;
		p_Scheduler->next = NULL;
	}
	else
	{
		p_Scheduler->next = scheduler_h;
		scheduler_h = p_Scheduler;
	}

	if ((DEBUG) && (DEBUG_FUNC))
	{
		ircsp_log (F_MAINLOG,
			get_log_message (LOG_MESSAGE_FUNCEND),
			__FILE__, __LINE__, __FUNCTION__
		);
	}

	return p_Scheduler;
}


/*
---------------------------------------------------------------------------------------------------------------------------------
    FUNCTION       |    Scheduler_find ()
                   |
    DESCRIPTION    |    This function will search for a specific task on the Scheduler List
                   |
    RETURNS        |    Pointer to the Scheduler List if successfull, orhterwise NULL
---------------------------------------------------------------------------------------------------------------------------------
*/
scheduler_t *Scheduler_find (task)
	char		*task;
{
	if ((DEBUG) && (DEBUG_FUNC))
	{
		ircsp_log (F_MAINLOG,
			get_log_message (LOG_MESSAGE_FUNCSTART),
			__FILE__, __LINE__, __FUNCTION__
		);
	}

	scheduler_t		*p_Scheduler;

	assert (task != NULL);

	p_Scheduler = scheduler_h;
	while (p_Scheduler)
	{
		if (!strcasecmp (p_Scheduler->task_name, task))
		{
			if (DEBUG)
			{
				if (DEBUG_SCHEDULER)
				{
					ircsp_log (F_MAINLOG,
						"[%s:%d:%s()]: Scheduled Task [%s] found at [%p]\n",
						__FILE__, __LINE__, __FUNCTION__, p_Scheduler->task_name,
						p_Scheduler->task_name
					);
				}

				if (DEBUG_FUNC)
				{
					ircsp_log (F_MAINLOG,
						get_log_message (LOG_MESSAGE_FUNCEND),
						__FILE__, __LINE__, __FUNCTION__
					);
				}
			}

			return p_Scheduler;
		}

		p_Scheduler = p_Scheduler->next;
	}

	if ((DEBUG) && (DEBUG_FUNC))
	{
		ircsp_log (F_MAINLOG,
			get_log_message (LOG_MESSAGE_FUNCEND),
			__FILE__, __LINE__, __FUNCTION__
		);
	}

	return NULL;
}


/*
---------------------------------------------------------------------------------------------------------------------------------
    FUNCTION       |    Scheduler_del ()
                   |
    DESCRIPTION    |    This function will delete a specific task from our Scheduler
                   |
    RETURNS        |    NONE
---------------------------------------------------------------------------------------------------------------------------------
*/
void Scheduler_del (task)
	char		*task;
{
	if ((DEBUG) && (DEBUG_FUNC))
	{
		ircsp_log (F_MAINLOG,
			get_log_message (LOG_MESSAGE_FUNCSTART),
			__FILE__, __LINE__, __FUNCTION__
		);
	}

	scheduler_t		*p_Scheduler;
	scheduler_t		*p_Delete;

	assert (task != NULL);

	p_Scheduler = scheduler_h;
	while (p_Scheduler)
	{
		if (!strcasecmp (p_Scheduler->task_name, task))
		{
			if ((DEBUG) && (DEBUG_SCHEDULER))
			{
				ircsp_log (F_MAINLOG,
					"[%s:%d:%s()]: Deleting scheduled task [%s] found at [%p]\n",
					__FILE__, __LINE__, __FUNCTION__, p_Scheduler->task_name,
					p_Scheduler->task_name
				);
			}

			if (scheduler_h == p_Scheduler)
			{
				scheduler_h = p_Scheduler->next;
			}
			else
			{
				for (p_Delete = scheduler_h; (p_Delete->next != p_Scheduler) && p_Delete; p_Delete = p_Delete->next);
				p_Delete->next = p_Scheduler->next;
			}

			ircsp_free (p_Scheduler->task_name);
			p_Scheduler->task_last_run = 0;
			p_Scheduler->task_interval = 0;
			p_Scheduler->func = NULL;
			ircsp_free (p_Scheduler);

			if ((DEBUG) && (DEBUG_FUNC))
			{
				ircsp_log (F_MAINLOG,
					get_log_message (LOG_MESSAGE_FUNCEND),
					__FILE__, __LINE__, __FUNCTION__
				);
			}

			return ;
		}

		p_Scheduler = p_Scheduler->next;
	}

	if ((DEBUG) && (DEBUG_FUNC))
	{
		ircsp_log (F_MAINLOG,
			get_log_message (LOG_MESSAGE_FUNCEND),
			__FILE__, __LINE__, __FUNCTION__
		);
	}
}
