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


#ifndef __INCLUDED_CORE_CONFIG_MAIN_H__
#define __INCLUDED_CORE_CONFIG_MAIN_H__


/*
---------------------------------------------------------------------------------------------------------------------------------
 STRUCTURES
---------------------------------------------------------------------------------------------------------------------------------
*/
struct _ConfigOptions
{
	char		command[64];
	int		(*func)(char *[], int);
};


typedef struct _Config
{
	char		*server_name;					// Name of our server
	char		*server_comment;				// Description of our server
	char		*server_numeric;				// Numeric of our server (Only for Undernet)
	char		*uplink;					// Hostname/IP Address of our uplink server
	char		*uplink_pass;					// Password to connect to our uplink server
	int		uplink_port;					// Port to connect to our uplink server
	int		protocol;					// Protocol to be used
	int		xPos;						// Used by our numeric generator (for juped nicknames and service nicknames)
	int		yPos;						// Used by our numeric generator (for juped nicknames and service nicknames)
	int		yyPos;						// Used by our numeric generator (for juped nicknames and service nicknames)
	int		fork;						// Used to determine if we are going to run in foreground or background
	char		*operchannel_name;				// Name of our Operator Channel
	char		*operchannel_modes;				// Modes to set on our Operator Channel
	int		operator_notice;				// Flag on how we handle sending notices to our operators


	char		*uworld_nickname;				// This is for our old OperServ (Uworld) it will be removed in future releases
	char		*uworld_username;				// This is for our old OperServ (Uworld) it will be removed in future releases
	char		*uworld_hostname;				// This is for our old OperServ (Uworld) it will be removed in future releases
	char		*uworld_realname;				// This is for our old OperServ (Uworld) it will be removed in future releases
	char		*uworld_ircmodes;				// This is for our old OperServ (Uworld) it will be removed in future releases
	int		uworld_enable;					// This is for our old OperServ (Uworld) it will be removed in future releases
	int		uworld_online;
	char		*deny_message;					// This is for our old OperServ (Uworld) it will be removed in future releases


	char		*admin_db;
	char		*gline_db;
	char		*uw_cmddb;


	int		nickserv_loaded;
	int		operserv_loaded;
	int		chanserv_loaded;
	struct		_nickname			*nickserv_p;

	struct		_config_database		*database;	// Pointer to Database Config Options

	struct		_config_module			*ConfigModuleHead;	// HEAD of ModuleConfig list
} Config;


typedef struct _config_module
{
	char					*ModuleName;		// Name of the Module (file name without path)
	int					ModuleType;		// Type of Module (CORE, PROTOCOL, SERVICE, etc)
	struct		_config_module		*next;			// Pointer to next module record
} ConfigModule;

struct _config_nickserv
{
	char		*nickname;
	char		*username;
	char		*hostname;
	char		*realname;
	char		*ircmodes;
	int		enabled;
	int		online;
};

struct _config_operserv
{
	char		*nickname;
	char		*username;
	char		*hostname;
	char		*realname;
	char		*ircmodes;
	int		enabled;
	int		online;
};

typedef struct _config_database
{
	char		*server;				// Hostname or IP address of our MySQL server
	char		*username;				// Username we use to connect to mysql
	char		*password;				// Password used to connect to our database
	char		*dbname;				// Name of our database
	char		*table_nickserv;			// Database Table used for Main NickServ Registration Database
	char		*table_nickserv_access;			// Database Table used for NickServ Access database
	char		*table_services_admin;			// Database Tables used for Services Administrators
} Config_Database;


typedef struct _service_module_config
{
	char				*nickname;				/* Service Nickname */
	char				*username;				/* Service Username */
	char				*hostname;				/* Service Hostname */
	char				*ircmodes;				/* Service Modes */
	char				*realname;				/* Service Realname */
	int				flags;					/* Service Flags (ONLINE, etc) */

	struct _module			*p_Module;			/* Pointer to module using said configuration */
	struct _nickname		*p_Nickname;			/* Nickname record for said service */
	struct _service_module_config	*next;
} ConfigServiceModule;


Config					*config;
ConfigServiceModule			*ConfigServiceModuleHead;


/*
---------------------------------------------------------------------------------------------------------------------------------
 FLAGS
---------------------------------------------------------------------------------------------------------------------------------
*/
#define		OPERNOTICE_CHANNEL		0x0000000001
#define		OPERNOTICE_DIRECT		0x0000000002
#define		OPERNOTICE_WALLOPS		0x0000000004

enum
{
	PROTOCOL_IS_UNDERNET = 1,
	PROTOCOL_IS_DALNET = 2,
	PROTOCOL_IS_EFNET = 4
};


/* Flags for Service Modules */
enum
{
	SERVICEMODULE_ENABLED		=	1,
	SERVICEMODULE_ONLINE		=	2
};


/*
---------------------------------------------------------------------------------------------------------------------------------
 MACROS
---------------------------------------------------------------------------------------------------------------------------------
*/
#define		IsProtocolUndernet(x)			((x)->protocol & PROTOCOL_IS_UNDERNET)			/* Protocol Is Undernet */
#define		IsProtocolDalnet(x)			((x)->protocol & PROTOCOL_IS_DALNET)			/* Protocol is Dalnet */
#define		IsProtocolEfnet(x)			((x)->protocol & PROTOCOL_IS_EFNET)			/* Protocol Is Efnet */

#define		SetProtocolUndernet(x)			((x)->protocol |= PROTOCOL_IS_UNDERNET)			/* Set Protocol to Undernet */
#define		SetProtocolDalnet(x)			((x)->protocol |= PROTOCOL_IS_DALNET)			/* Set Protocol to Dalnet */
#define		SetProtocolEfnet(x)			((x)->protocol |= PROTOCOL_IS_EFNET)			/* Set Protocol to Efnet */


/*
---------------------------------------------------------------------------------------------------------------------------------
 DEFINES
---------------------------------------------------------------------------------------------------------------------------------
*/
#define		E		extern
#define		CHANLEN		1024
#define		NICKLEN		15
#define		USERLEN		10
#define		HOSTLEN		256
#define		SERVLEN		1024
#define		KEYLEN		28
#define		PASSLEN		20
#define		COMMLEN		2096

#define		CONFIG_OPTIONS_COUNT		32


/*
---------------------------------------------------------------------------------------------------------------------------------
 PROTOTYPES
---------------------------------------------------------------------------------------------------------------------------------
*/
E	ConfigModule		*ConfigModule_Add					(char *);
E	ConfigModule		*ConfigModule_Find					(char *);
E	ConfigModule		*ConfigModule_FindProtocol				();
E	void			ConfigModule_Del					(char *);
E	ConfigServiceModule	*ConfigServiceModule_add				(struct _module *, char *, char *, char *, char *, char *);
E	ConfigServiceModule	*ConfigServiceModule_find				(char *);
E	void			ConfigServiceModule_del					(char *);

E	int		ircsp_loadConfig					(void);
E	int		ircsp_parseConfig					(int, char *[]);
E	int		ircsp_config_option_server				(char *[], int);
E	int		ircsp_config_option_server_numeric			(char *[], int);
E	int		ircsp_config_option_server_protocol			(char *[], int);
E	int		ircsp_config_option_uplink				(char *[], int);
E	int		ircsp_config_option_numeric_generator			(char *[], int);
E	int		ircsp_config_option_operchannel				(char *[], int);
E	int		ircsp_config_option_clones				(char *[], int);
E	int		ircsp_config_option_service_nickname			(char *[], int);
E	int		ircsp_config_option_uworld				(char *[], int);	/*  Will be replaced by a new OperServ at some point */
E	int		ircsp_config_option_database_connection			(char *[], int);
E	int		ircsp_config_option_database_table			(char *[], int);
E	int		ircsp_config_option_operator_notice			(char *[], int);
E	int		ircsp_config_option_services_root			(char *[], int);
E	int		ConfigOption_Module					(char *[], int);

#endif		/* __INCLUDED_CORE_CONFIG_MAIN_H__ */
