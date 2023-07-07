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

/* Stdandard Includes */
#include	<stdio.h>
#include	<stdlib.h>
#include	<stdarg.h>
#include	<stdint.h>
#include	<unistd.h>
#include	<string.h>
#include	<signal.h>
#include	<mariadb/mysql.h>


/* IRCSP Core Includes */
#include	"conf.h"
#include	"signals.h"
#include	"ircsp.h"
#include	"log.h"
#include	"alloc.h"
#include	"mysql.h"


/* Debugging */
#include	"debug.h"


/* Compiler Generated Includes */
#include	"config.h"



/*
---------------------------------------------------------------------------------------------------------------------------------
 FUNCTION       |    ircsp_mysql_connect ()
                |
 DESCRIPTION    |
                |
 RETURNS        |
---------------------------------------------------------------------------------------------------------------------------------
*/
void *ircsp_mysql_connect (void)
{
	if ((DEBUG) && (DEBUG_FUNC))
	{
		ircsp_log (F_MAINLOG,
			get_log_message (LOG_MESSAGE_FUNCEND),
			__FILE__, __LINE__, __FUNCTION__
		);
	}

	MYSQL		*db_connection;

	if ((DEBUG) && (DEBUG_MYSQL))
	{
		ircsp_log (F_MAINLOG,
			"[%s:%d:%s()]: Connecting to mysql server [%s]\n",
			__FILE__, __LINE__, __FUNCTION__, config->database->server
		);
	}

	db_connection = mysql_init (NULL);
	if (!db_connection)
	{
		if (DEBUG)
		{
			if (DEBUG_MYSQL)
			{
				ircsp_log (F_MAINLOG,
					"[%s:%d:%s()]: mysql_init() failed\n",
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

	if (mysql_real_connect (db_connection, config->database->server, config->database->username,
		config->database->password, config->database->dbname, 0, NULL, 0) == NULL)
	{
		if (DEBUG)
		{
			if (DEBUG_MYSQL)
			{
				ircsp_log (F_MAINLOG,
					"[%s:%d:%s()]: mysql_real_connect() error %s\n",
					__FILE__, __LINE__, __FUNCTION__,
					mysql_error (db_connection)
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

	if ((DEBUG) && (DEBUG_FUNC))
	{
		ircsp_log (F_MAINLOG,
			get_log_message (LOG_MESSAGE_FUNCEND),
			__FILE__, __LINE__, __FUNCTION__
		);
	}

	return db_connection;
}


/*
---------------------------------------------------------------------------------------------------------------------------------
 FUNCTION       |    ircsp_mysql_disconnect ()
                |
 DESCRIPTION    |
                |
 RETURNS        |
---------------------------------------------------------------------------------------------------------------------------------
*/
void ircsp_mysql_disconnect (db_connection)
	MYSQL		*db_connection;
{
	if (DEBUG)
	{
		if (DEBUG_FUNC)
		{
			ircsp_log (F_MAINLOG,
				get_log_message (LOG_MESSAGE_FUNCEND),
				__FILE__, __LINE__, __FUNCTION__
			);
		}

		if (DEBUG_MYSQL)
		{
			ircsp_log (F_MAINLOG,
				"[%s:%d:%s()]: Disconnecting from mysql server\n",
				__FILE__, __LINE__, __FUNCTION__
			);
		}

	}

	mysql_close (db_connection);

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
 FUNCTION       |    ircsp_mysql_backup_table ()
                |
 DESCRIPTION    |
                |
 RETURNS        |    TRUE  = Success
                |    FALSE = Failure
---------------------------------------------------------------------------------------------------------------------------------
*/
int ircsp_mysql_backup_table (table)
	char		*table;
{
	if ((DEBUG) && (DEBUG_FUNC))
	{
		ircsp_log (F_MAINLOG,
			get_log_message (LOG_MESSAGE_FUNCSTART),
			__FILE__, __LINE__, __FUNCTION__
		);
	}

	MYSQL		*connection;
	char		*c_query;

	connection = ircsp_mysql_connect ();

	c_query = (char *)ircsp_calloc(1, 1024);
	if (!c_query)
	{
		if (DEBUG)
		{
			if (DEBUG_ALLOC)
			{
				ircsp_log (F_MAINLOG,
					"[%s:%d:%s()]: Failed to allocate memory for c_query\n",
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

		return FALSE;
	}

	sprintf (c_query, "DROP TABLE IF EXISTS %s%s", table, "BACKUP");
	if (mysql_query (connection, c_query))
	{
		if ((DEBUG) && (DEBUG_MYSQL))
		{
			ircsp_log (F_MAINLOG,
				"[%s:%d:%s()]: mysql_query() - %s\n",
				__FILE__, __LINE__, __FUNCTION__, mysql_error (connection)
			);
		}

		ircsp_mysql_disconnect (connection);

		if ((DEBUG) && (DEBUG_FUNC))
		{
			ircsp_log (F_MAINLOG,
				get_log_message (LOG_MESSAGE_FUNCEND),
				__FILE__, __LINE__, __FUNCTION__
			);
		}

		return FALSE;
	}


	sprintf (c_query, "RENAME TABLE %s to %s%s", table, table, "BACKUP");
	if (mysql_query (connection, c_query))
	{
		if ((DEBUG) && (DEBUG_MYSQL))
		{
			ircsp_log (F_MAINLOG,
				"[%s:%d:%s()]: mysql_query() - %s\n",
				__FILE__, __LINE__, __FUNCTION__, mysql_error (connection)
			);
		}

		ircsp_mysql_disconnect (connection);

		if ((DEBUG) && (DEBUG_FUNC))
		{
			ircsp_log (F_MAINLOG,
				get_log_message (LOG_MESSAGE_FUNCEND),
				__FILE__, __LINE__, __FUNCTION__
			);
		}

		return FALSE;
	}

	ircsp_free (c_query);

	ircsp_mysql_disconnect (connection);

	if ((DEBUG) && (DEBUG_FUNC))
	{
		ircsp_log (F_MAINLOG,
			get_log_message (LOG_MESSAGE_FUNCEND),
			__FILE__, __LINE__, __FUNCTION__
		);
	}

	return TRUE;
}


/*
---------------------------------------------------------------------------------------------------------------------------------
 FUNCTION       |    ircsp_mysql_create_table ()
                |
 DESCRIPTION    |
                |
 RETURNS        |    TRUE  = Success
                |    FALSE = Failure
---------------------------------------------------------------------------------------------------------------------------------
*/
int ircsp_mysql_create_table (table, data)
	char		*table;
	char		*data;
{
	if ((DEBUG) && (DEBUG_FUNC))
	{
		ircsp_log (F_MAINLOG,
			get_log_message (LOG_MESSAGE_FUNCSTART),
			__FILE__, __LINE__, __FUNCTION__
		);
	}

	MYSQL		*connection;
	char		*c_query;

	connection = ircsp_mysql_connect ();

	c_query = (char *)ircsp_calloc(1, 8192);
	if (!c_query)
	{
		if (DEBUG)
		{
			if (DEBUG_ALLOC)
			{
				ircsp_log (F_MAINLOG,
					"[%s:%d:%s()]: Failed to allocate memory for c_query\n",
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

		return FALSE;
	}

	sprintf (c_query, "CREATE TABLE IF NOT EXISTS %s %s", table, data);
	if (mysql_query (connection, c_query))
	{
		if ((DEBUG) && (DEBUG_MYSQL))
		{
			ircsp_log (F_MAINLOG,
				"[%s:%d:%s()]: mysql_query() - %s\n",
				__FILE__, __LINE__, __FUNCTION__, mysql_error (connection)
			);
		}

		ircsp_mysql_disconnect (connection);

		if ((DEBUG) && (DEBUG_FUNC))
		{
			ircsp_log (F_MAINLOG,
				get_log_message (LOG_MESSAGE_FUNCEND),
				__FILE__, __LINE__, __FUNCTION__
			);
		}

		return FALSE;
	}

	ircsp_free (c_query);
	ircsp_mysql_disconnect (connection);

	if ((DEBUG) && (DEBUG_FUNC))
	{
		ircsp_log (F_MAINLOG,
			get_log_message (LOG_MESSAGE_FUNCEND),
			__FILE__, __LINE__, __FUNCTION__
		);
	}

	return 1;

}


/*
---------------------------------------------------------------------------------------------------------------------------------
 FUNCTION       |    ircsp_mysql_insert ()
                |
 DESCRIPTION    |
                |
 RETURNS        |    TRUE  = Success
                |    FALSE = Failure
---------------------------------------------------------------------------------------------------------------------------------
*/
int ircsp_mysql_insert (char *table, char *format, ...)
{
	if ((DEBUG) && (DEBUG_FUNC))
	{
		ircsp_log (F_MAINLOG,
			get_log_message (LOG_MESSAGE_FUNCSTART),
			__FILE__, __LINE__, __FUNCTION__
		);
	}

	MYSQL		*db_connection;
	char		buf[256];
	char		*query;
	va_list		msg;

	va_start (msg, format);
	vsprintf (buf, format, msg);

	db_connection = ircsp_mysql_connect ();
	if (!db_connection)
	{
		if (DEBUG)
		{
			if (DEBUG_MYSQL)
			{
				ircsp_log (F_MAINLOG,
					"[%s:%d:%s()]: Failed to connect to database server\n",
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

		return FALSE;
	}

	query = ircsp_calloc (1, 8192);
	if (!query)
	{
		if (DEBUG)
		{
			if (DEBUG_ALLOC)
			{
				ircsp_log (F_MAINLOG,
					"[%s:%d:%s()]: Failed to allocate memory for query\n",
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

		return FALSE;
	}

	sprintf (query, "INSERT INTO %s VALUES (%s)", table, buf);
	if (mysql_query (db_connection, query))
	{
		if ((DEBUG) && (DEBUG_MYSQL))
		{
			ircsp_log (F_MAINLOG,
				"[%s:%d:%s()]: mysql_query() - %s\n",
				__FILE__, __LINE__, __FUNCTION__, mysql_error (db_connection)
			);
		}

		ircsp_free (query);

		if ((DEBUG) && (DEBUG_FUNC))
		{
			ircsp_log (F_MAINLOG,
				get_log_message (LOG_MESSAGE_FUNCEND),
				__FILE__, __LINE__, __FUNCTION__
			);
		}

		return FALSE;
	}

	if ((DEBUG) && (DEBUG_FUNC))
	{
		ircsp_log (F_MAINLOG,
			get_log_message (LOG_MESSAGE_FUNCEND),
			__FILE__, __LINE__, __FUNCTION__
		);
	}

	return TRUE;
}


/*
---------------------------------------------------------------------------------------------------------------------------------
 FUNCTION       |    ircsp_mysql_update ()
                |
 DESCRIPTION    |
                |
 RETURNS        |    TRUE  = Success
                |    FALSE = Failure
---------------------------------------------------------------------------------------------------------------------------------
*/
int ircsp_mysql_update (char *table, char *format, ...)
{
	if ((DEBUG) && (DEBUG_FUNC))
	{
		ircsp_log (F_MAINLOG,
			get_log_message (LOG_MESSAGE_FUNCSTART),
			__FILE__, __LINE__, __FUNCTION__
		);
	}

	MYSQL		*db_connection;
	char		buf[256];
	char		*query;
	va_list		msg;

	va_start (msg, format);
	vsprintf (buf, format, msg);

	db_connection = ircsp_mysql_connect ();
	if (!db_connection)
	{
		if (DEBUG)
		{
			if (DEBUG_MYSQL)
			{
				ircsp_log (F_MAINLOG,
					"[%s:%d:%s()]: Failed to connect to database server\n",
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

		return FALSE;
	}

	query = ircsp_calloc (1, 8192);
	if (!query)
	{
		if (DEBUG)
		{
			if (DEBUG_ALLOC)
			{
				ircsp_log (F_MAINLOG,
					"[%s:%d:%s()]: Failed to allocate memory for query\n",
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

		return FALSE;
	}

	sprintf (query, "UPDATE %s %s", table, buf);
	if (mysql_query (db_connection, query))
	{
		if ((DEBUG) && (DEBUG_MYSQL))
		{
			ircsp_log (F_MAINLOG,
				"[%s:%d:%s()]: mysql_query() - %s\n",
				__FILE__, __LINE__, __FUNCTION__, mysql_error (db_connection)
			);
		}

		ircsp_free (query);

		if ((DEBUG) && (DEBUG_FUNC))
		{
			ircsp_log (F_MAINLOG,
				get_log_message (LOG_MESSAGE_FUNCEND),
				__FILE__, __LINE__, __FUNCTION__
			);
		}

		return FALSE;
	}

	ircsp_free (query);

	if ((DEBUG) && (DEBUG_FUNC))
	{
		ircsp_log (F_MAINLOG,
			get_log_message (LOG_MESSAGE_FUNCEND),
			__FILE__, __LINE__, __FUNCTION__
		);
	}

	return TRUE;
}


/*
---------------------------------------------------------------------------------------------------------------------------------
 FUNCTION       |    ircsp_mysql_delete ()
                |
 DESCRIPTION    |
                |
 RETURNS        |    TRUE  = Success
                |    FALSE = Failure
---------------------------------------------------------------------------------------------------------------------------------
*/
int ircsp_mysql_delete (char *table, char *format, ...)
{
	if ((DEBUG) && (DEBUG_FUNC))
	{
		ircsp_log (F_MAINLOG,
			get_log_message (LOG_MESSAGE_FUNCSTART),
			__FILE__, __LINE__, __FUNCTION__
		);
	}

	MYSQL		*db_connection;
	char		buf[256];
	char		*query;
	va_list		msg;

	va_start (msg, format);
	vsprintf (buf, format, msg);

	db_connection = ircsp_mysql_connect ();
	if (!db_connection)
	{
		if (DEBUG)
		{
			if (DEBUG_MYSQL)
			{
				ircsp_log (F_MAINLOG,
					"[%s:%d:%s()]: Failed to connect to database server\n",
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

		return FALSE;
	}

	query = ircsp_calloc (1, 8192);
	if (!query)
	{
		if (DEBUG)
		{
			if (DEBUG_ALLOC)
			{
				ircsp_log (F_MAINLOG,
					"[%s:%d:%s()]: Failed to allocate memory for query\n",
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

		return FALSE;
	}

	sprintf (query, "DELETE FROM %s WHERE %s", table, buf);
	if (mysql_query (db_connection, query))
	{
		if ((DEBUG) && (DEBUG_MYSQL))
		{
			ircsp_log (F_MAINLOG,
				"[%s:$d:%s()]: mysql_query() - %s\n",
				__FILE__, __LINE__, __FUNCTION__, mysql_error (db_connection)
			);
		}

		ircsp_free (query);

		if ((DEBUG) && (DEBUG_FUNC))
		{
			ircsp_log (F_MAINLOG,
				get_log_message (LOG_MESSAGE_FUNCEND),
				__FILE__, __LINE__, __FUNCTION__
			);
		}

		return FALSE;
	}

	ircsp_free (query);

	if ((DEBUG) && (DEBUG_FUNC))
	{
		ircsp_log (F_MAINLOG,
			get_log_message (LOG_MESSAGE_FUNCEND),
			__FILE__, __LINE__, __FUNCTION__
		);
	}

	return TRUE;
}
