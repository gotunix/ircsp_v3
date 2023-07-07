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
#include	"ircsp.h"
#include	"log.h"
#include	"alloc.h"
#include	"conf.h"
#include	"servers.h"
#include	"users.h"
#include	"channels.h"
#include	"socket.h"
#include	"modules.h"


/* Debug */
#include	"debug.h"


/* Compiler Generated Includes */
#include	"config.h"


/* Undernet Module Includes */
#include	"libundernet.h"
#include	"libundernet_cmds.h"
#include	"libundernet_introdouce.h"
#include	"libundernet_parser.h"


char ModuleVersion[] = "1.0.0";
char ModuleName[] = "Protocol_IRCUP10";
char ModuleType[] = "protocol";


/*
---------------------------------------------------------------------------------------------------------------------------------
 FUNCTION       |    init_module ()
                |
 DESCRIPTION    |    This function will initialize our module
                |
 RETURNS        |    Nothing
---------------------------------------------------------------------------------------------------------------------------------
*/
void init_module (p_Module)
	struct _module		*p_Module;
{
	if (LOG_FUNC)
	{
		ircsp_log (F_MAINLOG,
			get_log_message (LOG_MESSAGE_FUNCSTART),
			__FILE__, __LINE__, __FUNCTION__
		);
	}

	module_callback_t	*p_ModuleCallback;

	p_Module->ModuleType |= MODULETYPE_PROTOCOL;

//	ircsp_add_callback (p_Module, "do_parse");

	cmd_notice		=	libundernet_cmd_notice;
	cmd_wallops		=	libundernet_cmd_wallops;
	cmd_join		=	libundernet_cmd_join;
	cmd_part		=	libundernet_cmd_part;
	cmd_chanop		=	libundernet_cmd_chanop;
	cmd_chandeop		=	libundernet_cmd_chandeop;
	cmd_chanvoice		=	libundernet_cmd_chanvoice;
	cmd_chandevoice		=	libundernet_cmd_chandevoice;
	cmd_kill		=	libundernet_cmd_kill;
	cmd_nickjupe		=	libundernet_cmd_nickjupe;

	introdouce_service	=	libundernet_introdouce_service;
	introdouce_server	=	libundernet_introdouce_server;

	m_parse			=	libundernet_parser;

	if (LOG_FUNC)
	{
		ircsp_log (F_MAINLOG,
			get_log_message (LOG_MESSAGE_FUNCEND),
			__FILE__, __LINE__, __FUNCTION__
		);
	}
}
