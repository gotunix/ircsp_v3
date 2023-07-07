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

#ifndef		__INCLUDED_MODULES_H__
#define		__INCLUDED_MODULES_H__
#define         E                       	extern


/*
---------------------------------------------------------------------------------------------------------------------------------
 DEFINES
---------------------------------------------------------------------------------------------------------------------------------
*/
enum
{
		MODULETYPE_CORE			=		1,			/* Core Modules (Database, alloc, logging, etc) */
		MODULETYPE_PROTOCOL		=		2,			/* Protocol Modules (dalnet, efnet, undernet, etc) */
		MODULETYPE_SERVICE		=		4			/* Service Modules (NickServ, ChanServ, OperServ, etc) */
};


enum
{
		SERVICETYPE_NICKSERV		=		1,			/* [BUILT-IN] - Required */
		SERVICETYPE_OPERSERV		=		2,			/* [BUILT-IN] - Optional */
		SERVICETYPE_CHANSERV		=		4,			/* [BUILT-IN] - Optional */
		SERVICETYPE_HELPSERV		=		8,			/* [BUILT-IN] - Optional */
		SERVICETYPE_MEMOSERV		=		16,			/* [BUILT-IN] - Optional */
		SERVICETYPE_MISC		=		32			/* [ADDON]    - Optonal Add On Modules */
};


# define FUNCPTR(type,name,rest) \
    type _##name##_t rest; \
    typeof(_##name##_t) *name
# define E_FUNCPTR(type,name,rest) \
    type _##name##_t rest; \
    extern typeof(_##name##_t) *name


/*
---------------------------------------------------------------------------------------------------------------------------------
 Structures
---------------------------------------------------------------------------------------------------------------------------------
*/
typedef struct _module
{
	char				*FileName;		/* Module file name */
	char				*ServiceNickname;	/* Service Nickname */
	char				*ModuleName;		/* Name of Module */
	char				*ModuleVersion;		/* Version of Module */
	int				ModuleType;		/* Type of module (0 = core, 1 = protocol, 2 = service) */
	int				ServiceType;		/* Core Service Type */
	void				*dllHandler;		/* Module handler */
	struct _module_callback		*module_callback_h;	/* Callback Head for Module */
	struct _module			*next;			/* Next module in list */
} module_t;


typedef struct _module_callback
{
	char				*name;			/* Name of Callback Function */
	void				(*func)();		/* Pointer to VOID Function in module */
	int				(*int_func)();		/* Pointer to INT Function in module */
	struct _module			*module_p;		/* Pointer to module */
	struct _module_callback		*next;			/* Pointer to next Function in module */
} module_callback_t;


module_t				*module_h;		/* Module list HEAD */


/*
---------------------------------------------------------------------------------------------------------------------------------
 PROTOTYPES
---------------------------------------------------------------------------------------------------------------------------------
*/
E	module_t		*Module_Add			(char *);
E	module_t		*Module_Find			(char *);
E	module_t		*Module_FindProtocol		(void);
E	module_t		*Module_Find_Service		(char *);
E	module_t		*module_find_nickserv		(void);
E	void			Module_Del			(char *);
E	module_callback_t	*ModuleCallback_Add		(struct _module *, char *);
E	module_callback_t	*ModuleCallback_Find		(struct _module *, char *);
E	void			ModuleCallback_Del		(struct _module *, char *);
E	void			ModuleCallback_DelAll		(struct _module *);
E	void			*ircsp_dlopen			(char *);
E	void			ircsp_dlclose			(void *);
E	void			*ircsp_dlsym			(void *, char *);
E	int			ModuleCallback_Register		(struct _module *, char *);
E	void			ModuleCallback_Call		(struct _module *, char *);
E	void			ModuleCallback_Call_1_Array	(struct _module *, char *, char *[]);
E	void			ModuleCallback_Call_1_Char	(struct _module *, char *, char *);
E	void			*ircsp_get_module_symbol	(struct _module *, char *);
E	int			Module_Load_Protocol		(void);
E	int			Module_Load_Services		(void);


/*
---------------------------------------------------------------------------------------------------------------------------------
 PROTOCOL POINTERS
---------------------------------------------------------------------------------------------------------------------------------
*/
E_FUNCPTR (void, cmd_notice, (char *, char *, char *, ...));
E_FUNCPTR (void, cmd_wallops, (char *, char *, ...));
E_FUNCPTR (void, cmd_join, (char *, char *));
E_FUNCPTR (void, cmd_part, (char *, char *));
E_FUNCPTR (void, cmd_chanop, (char *, char *));
E_FUNCPTR (void, cmd_chandeop, (char *, char *));
E_FUNCPTR (void, cmd_chanvoice, (char *, char *));
E_FUNCPTR (void, cmd_chandevoice, (char *, char *));
E_FUNCPTR (void, cmd_kill, (char *, char *, char *, ...));
E_FUNCPTR (void, cmd_nickjupe, (char *, char *, char *, char *, ...));
E_FUNCPTR (void, introdouce_service, (char *[]));
E_FUNCPTR (void, introdouce_server, (void));
E_FUNCPTR (void, m_parse, (char *));


/*
---------------------------------------------------------------------------------------------------------------------------------
    NICKSERV POINTERS
---------------------------------------------------------------------------------------------------------------------------------
*/
//E_FUNCPTR (void, services_root_unregister, (struct _nickname *));

#endif		/* __INCLUDED_MODULES_H__ */
