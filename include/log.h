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

#ifndef		__INCLUDED_CORE_LOG_H__
#define		__INCLUDED_CORE_LOG_H__
#define		E		extern

/*
---------------------------------------------------------------------------------------------------------------------------------
 VARIABLES
---------------------------------------------------------------------------------------------------------------------------------
*/
extern	FILE		*F_MAINLOG;
extern	int		MAINLOG_OPENED;


/*
---------------------------------------------------------------------------------------------------------------------------------
    STRUCTURES
---------------------------------------------------------------------------------------------------------------------------------
*/
typedef struct _logfile
{
	char			*log_name;
	FILE			*log_file;
	int			log_open;

	struct _logfile		*next;
} logfile_t;


logfile_t			*logfile_h;
logfile_t			*main_logfile_p;


/*
---------------------------------------------------------------------------------------------------------------------------------
    LOG LEVELS
---------------------------------------------------------------------------------------------------------------------------------
*/
enum
{
		LOG_CRITICAL		=	1,
		LOG_ERROR		=	2,
		LOG_WARNING		=	4,
		LOG_INFO		=	8,
		LOG_FUNC		=	16,
		LOG_ALL			=	32
};


#define		LOG_LEVEL		0x000032		/* What level ware we logging */


/*
---------------------------------------------------------------------------------------------------------------------------------
 DEFINES
---------------------------------------------------------------------------------------------------------------------------------
*/
#define			LOG_MESSAGE_COUNT					200

/* General Messages - 1 - 50 */

#define			LOG_MESSAGE_OUTOFMEMORY					1		// 0001
#define			LOG_MESSAGE_FUNCSTART					4		// 0004
#define			LOG_MESSAGE_FUNCEND					5		// 0005
#define			LOG_MESSAGE_FUNCEND_ERROR				6		// 0006
#define			LOG_MESSAGE_IRCSP_BUILD					10		// 0010
#define			LOG_MESSAGE_IRCSP_INIT					11		// 0011
#define			LOG_MESSAGE_NULLPTR					15		// 0015
#define			LOG_MESSAGE_ALLOCFAILFOR				20		// 0020

#define			LOG_MESSAGE_NOSUCHNICK					40		// 0040

/* Protocol Undernet (BURST) Messages (81 -) */
#define			BURST_MSG_PROC						81		// 0081
#define			BURST_MSG_PROC_CONTINUE					82		// 0082
#define			BURST_MSG_PROC_USERLIST					83		// 0083
#define			BURST_MSG_PROC_BANLIST					84		// 0084


/*
---------------------------------------------------------------------------------------------------------------------------------
 PROTOTYPES
---------------------------------------------------------------------------------------------------------------------------------
*/
E	void		ircsp_log					(FILE *, char *, ...);
E	void		LOG						(logfile_t *, int, char *, ...);
E	void		ircsp_error					(int, int, char *, ...);
E	char		*get_log_message				(int);
E	logfile_t	*logfile_register				(char *);
E	logfile_t	*logfile_search					(char *);
E	void		logfile_remove					(char *);


#endif		/* __INCLUDED_CORE_LOG_H__ */
