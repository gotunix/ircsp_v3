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
#include	<signal.h>


/* IRCSP Core Includes */
#include	"log.h"
#include	"signals.h"
#include	"ircsp.h"
#include	"rehash.h"


/* Debugging */
#include	"debug.h"


/* Compiler Generated Includes */
#include	"config.h"


/*
---------------------------------------------------------------------------------------------------------------------------------
 FUNCTION       |    ircsp_signal_handler ()
                |
 DESCRIPTION    |
                |
 RETURNS        |
---------------------------------------------------------------------------------------------------------------------------------
*/
int ircsp_signal_handler (void)
{
	struct sigaction sa;

	/* SIGHUP */
	sa.sa_handler = ircsp_signal_sighup;
	sa.sa_flags = 0;
	sigemptyset (&sa.sa_mask);
	sigaction (SIGHUP, &sa, (struct sigaction *) 0);

	/* SIGUSR1 */
	sa.sa_handler = ircsp_signal_sigusr1;
	sa.sa_flags = 0;
	sigemptyset (&sa.sa_mask);
	sigaction (SIGUSR1, &sa, (struct sigaction *) 0);

	/* SIGSEGV */
	sa.sa_handler = ircsp_signal_sigsegv;
	sa.sa_flags = 0;
	sigemptyset (&sa.sa_mask);
	sigaction (SIGSEGV, &sa, (struct sigaction *) 0);

	/* SIGINT */
	sa.sa_handler = ircsp_signal_sigint;
	sa.sa_flags = 0;
	sigemptyset (&sa.sa_mask);
	sigaction (SIGINT, &sa, (struct sigaction *) 0);

	/* SIGTERM */
	sa.sa_handler = ircsp_signal_sigterm;
	sa.sa_flags = 0;
	sigemptyset (&sa.sa_mask);
	sigaction (SIGTERM, &sa, (struct sigaction *) 0);

	return TRUE;
}


/*
---------------------------------------------------------------------------------------------------------------------------------
 FUNCTION       |    ircsp_signal_sighup ()
                |
 DESCRIPTION    |
                |
 RETURNS        |
---------------------------------------------------------------------------------------------------------------------------------
*/
void ircsp_signal_sighup (num)
	int num;
{
	if ((DEBUG) && (DEBUG_FUNC))
	{
		ircsp_log (F_MAINLOG,
			get_log_message (LOG_MESSAGE_FUNCSTART),
			__FILE__, __LINE__, __FUNCTION__
		);
	}

	ircsp_log (F_MAINLOG,
		"[%s:%d:%s()]: Signal HUP received, rehashing",
		__FILE__, __LINE__, __FUNCTION__
	);

	ircsp_rehash ();

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
 FUNCTION       |    ircsp_signal_sigusr1 ()
                |
 DESCRIPTION    |
                |
 RETURNS        |
---------------------------------------------------------------------------------------------------------------------------------
*/
void ircsp_signal_sigusr1 (num)
	int num;
{
	if ((DEBUG) && (DEBUG_FUNC))
	{
		ircsp_log (F_MAINLOG,
			get_log_message (LOG_MESSAGE_FUNCSTART),
			__FILE__, __LINE__, __FUNCTION__
		);
	}

	ircsp_log (F_MAINLOG, "[%s:%d:%s()]: Signal USR1 received, time to panic\n",
		__FILE__, __LINE__, __FUNCTION__);

	if ((DEBUG) && (DEBUG_FUNC))
	{
		ircsp_log (F_MAINLOG,
			get_log_message (LOG_MESSAGE_FUNCEND),
			__FILE__, __LINE__, __FUNCTION__
		);
	}

	exit (1);
}


/*
---------------------------------------------------------------------------------------------------------------------------------
 FUNCTION       |    ircsp_signal_sigsegv ()
                |
 DESCRIPTION    |
                |
 RETURNS        |
---------------------------------------------------------------------------------------------------------------------------------
*/
void ircsp_signal_sigsegv (num)
	int num;
{
	if ((DEBUG) && (DEBUG_FUNC))
	{
		ircsp_log (F_MAINLOG,
			get_log_message (LOG_MESSAGE_FUNCSTART),
			__FILE__, __LINE__, __FUNCTION__
		);
	}

	ircsp_log (F_MAINLOG, "[%s:%d:%s()]: Signal SEGV received, time to panic\n",
		__FILE__, __LINE__, __FUNCTION__);

	if ((DEBUG) && (DEBUG_FUNC))
	{
		ircsp_log (F_MAINLOG,
			get_log_message (LOG_MESSAGE_FUNCEND),
			__FILE__, __LINE__, __FUNCTION__
		);
	}

	ircsp_panic ("SIGSEGV Received");
}


/*
---------------------------------------------------------------------------------------------------------------------------------
 FUNCTION       |    ircsp_signal_sigint ()
                |
 DESCRIPTION    |
                |
 RETURNS        |
---------------------------------------------------------------------------------------------------------------------------------
*/
void ircsp_signal_sigint (num)
	int num;
{
	if ((DEBUG) && (DEBUG_FUNC))
	{
		ircsp_log (F_MAINLOG,
			get_log_message (LOG_MESSAGE_FUNCSTART),
			__FILE__, __LINE__, __FUNCTION__
		);
	}

	ircsp_log (F_MAINLOG, "[%s:%d:%s()]: Signal INT received, beginning shutdown proceedures\n",
		__FILE__, __LINE__, __FUNCTION__);

	if ((DEBUG) && (DEBUG_FUNC))
	{
		ircsp_log (F_MAINLOG,
			get_log_message (LOG_MESSAGE_FUNCEND),
			__FILE__, __LINE__, __FUNCTION__
		);
	}

	ircsp_exit (0, "SIGINT");
}


/*
---------------------------------------------------------------------------------------------------------------------------------
 FUNCTION       |    ircsp_signal_sigterm ()
                |
 DESCRIPTION    |
                |
 RETURNS        |
---------------------------------------------------------------------------------------------------------------------------------
*/
void ircsp_signal_sigterm (num)
	int num;
{
	if ((DEBUG) && (DEBUG_FUNC))
	{
		ircsp_log (F_MAINLOG,
			get_log_message (LOG_MESSAGE_FUNCEND),
			__FILE__, __LINE__, __FUNCTION__
		);
	}

	ircsp_log (F_MAINLOG, "[%s:%d:%s()]: Signal TERM received, beginning shutdown proceedures\n",
		__FILE__, __LINE__, __FUNCTION__);

	if ((DEBUG) && (DEBUG_FUNC))
	{
		ircsp_log (F_MAINLOG,
			get_log_message (LOG_MESSAGE_FUNCEND),
			__FILE__, __LINE__, __FUNCTION__
		);
	}

	ircsp_exit (0, "SIGTERM");
}
