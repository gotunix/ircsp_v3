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


#ifndef		__INCLUDED_LIBNICKSERV_COMMANDS_H__
#define		__INCLUDED_LIBNICKSERV_COMMANDS_H__
#define		E			extern


/*
---------------------------------------------------------------------------------------------------------------------------------
    DEFINES
---------------------------------------------------------------------------------------------------------------------------------
*/
#define		NICKSERV_NUM_COMMANDS		9


/*
---------------------------------------------------------------------------------------------------------------------------------
    STRUCTURES
---------------------------------------------------------------------------------------------------------------------------------
*/
struct nickserv_commands
{
	char		command [64];					/* Name of the Command */
	int		(*func)(int, char *[]);				/* Function that handles the command */
	int		minargs;					/* Minimum arguments required for the command */
	int		disabled;					/* Is the command disabled */
	int		ident;						/* IDENT required for command */
	int		minlevel;					/* Minimum Level required for command */
};


/*
---------------------------------------------------------------------------------------------------------------------------------
    PROTOTYPES
---------------------------------------------------------------------------------------------------------------------------------
*/
E	void		m_privmsg		(struct _module *, char *, char *, char *);
E	int		ns_version		(int, char *[]);
E	int		ns_help			(int, char *[]);
E	int		ns_register		(int, char *[]);
E	int		ns_identify		(int, char *[]);
E	int		ns_drop			(int, char *[]);
E	int		ns_access		(int, char *[]);
E	int		ns_set			(int, char *[]);
E	int		ns_ghost		(int, char *[]);
E	int		ns_recover		(int, char *[]);
E	int		ns_release		(int, char *[]);


#endif		/* __INCLUDED_LIBNICKSERV_COMMANDS_H__ */
