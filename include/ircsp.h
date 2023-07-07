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

#ifndef __INCLUDED_CORE_IRCSP_H__
#define __INCLUDED_CORE_IRCSP_H__


/*
---------------------------------------------------------------------------------------------------------------------------------
 DEFINES
---------------------------------------------------------------------------------------------------------------------------------
*/
#define		E		extern
#define		TRUE		1
#define		FALSE		0


/*
---------------------------------------------------------------------------------------------------------------------------------
 VARIABLES
---------------------------------------------------------------------------------------------------------------------------------
*/
//E	int		DEBUG;


/*
---------------------------------------------------------------------------------------------------------------------------------
 STRUCTURES
---------------------------------------------------------------------------------------------------------------------------------
*/
typedef struct _ircsp
{
	int		argc;
	char**		argv;
	pid_t		pid;
	uid_t		uid;
	uid_t		euid;
	time_t		uptime;
	int		connected;
	int		initial_burst;
	int		services_online;
	time_t		scheduler_last_run;			/* Last time the scheduler ran */
	time_t		last_save_cycle;
	time_t		last_gline_cycle;
	time_t		last_services_admin_save;		/* Last time we saved the ServicesAdmin database */
	time_t		last_nickserv_save;			/* Last time we saved NickServ databases */
	time_t		last_nickserv_expire;			/* Last time we expired NickServ registrations */
	time_t		last_nickserv_jupe_expire;		/* Last time we expired NickServ Jupes */
	time_t		last_nickserv_validate;			/* Last time we validated online users (Identify/autokill/etc) */
	int		rehashing;
	int		quitting;
} IRCSP;


typedef struct _status
{
	int		numUsers;
	int		numServers;
	int		numChannels;
	int		numOpers;
	int		numAdmins;
	int		numHosts;
	int		numGlines;
	int		numNumeric;
} Status;


typedef struct _service_list
{
	struct _nickname		*nickserv;
	struct _nickname		*operserv;
} Service;


IRCSP *ircsp;
Status *status;
Service *service;


/*
---------------------------------------------------------------------------------------------------------------------------------
 PROTOTYPES
---------------------------------------------------------------------------------------------------------------------------------
*/
E	int		ircsp_file_exists				(char *);
E	int		ircsp_check_access				(const char *, int);
E	int		ircsp_init					(int, char *[]);
E	void		ircsp_exit					(int, char *, ...);
E	void		ircsp_panic					(char *, ...);
E	void		ircsp_restart					(void);
E	void		ircsp_spawnUW					(void);
E	void		ircsp_mainloop					(void);
E	void		ircsp_join_operchannel				(void);


#endif		/* __INCLUDED_CORE_IRCSP_H__ */
