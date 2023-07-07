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

#ifndef __INCLUDED_MYSQL_H__
#define __INCLUDED_MYSQL_H__


/*
---------------------------------------------------------------------------------------------------------------------------------
 DATABASE TABLES
---------------------------------------------------------------------------------------------------------------------------------
*/
#define		NICKSERV_TABLE		"(						\
						id INT AUTO_INCREMENT PRIMARY KEY,	\
						nickname VARCHAR(255) NOT NULL,		\
						password VARCHAR(255) NOT NULL,		\
						last_login VARCHAR(255) NOT NULL,	\
						last_seen INT(11) NOT NULL,		\
						registered INT(11) NOT NULL,		\
						flags INT(8) NOT NULL			\
					)  ENGINE=INNODB;"

#define		NICKSERV_ACCESS_TABLE	"(						\
						id INT AUTO_INCREMENT PRIMARY KEY,	\
						nickname VARCHAR(255) NOT NULL,		\
						hostmask VARCHAR(255) NOT NULL		\
					)  ENGINE=INNODB;"

#define		SERVICES_ADMIN_TABLE	"(						\
						id INT AUTO_INCREMENT PRIMARY KEY,	\
						nickname VARCHAR(255) NOT NULL,		\
						flags INT(10) NOT NULL			\
						)  ENGINE=INNODB;"



#define		NSDB_NICKSERV			"nickserv"
#define		NSDB_NICKSERV_ACCESS		"nickserv_access"
#define		NSDB_SERVICES_ADMIN		"services_admin"


/*
---------------------------------------------------------------------------------------------------------------------------------
 DEFINES
---------------------------------------------------------------------------------------------------------------------------------
*/
#define		E		extern



#define		HOSTNAME_EXEMPT_TABLE	"(id INT AUTO_INCREMENT PRIMARY KEY,hostmask VARCHAR(255) NOT NULL,count INT(10) NOT NULL)  ENGINE=INNODB;"


/*
---------------------------------------------------------------------------------------------------------------------------------
 PROTOTYPES
---------------------------------------------------------------------------------------------------------------------------------
*/
E	void		*ircsp_mysql_connect			(void);
E	void		ircsp_mysql_disconnect			();
E	int		ircsp_mysql_backup_table		(char *);
E	int		ircsp_mysql_create_table		(char *, char *);
E	int		ircsp_mysql_insert			(char *, char *, ...);
E	int		ircsp_mysql_update			(char *, char *, ...);
E	int		ircsp_mysql_delete			(char *, char *, ...);

#endif		/* __INCLUDED_MYSQL_H__ */
