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


#ifndef		__INCLUDED_LIBNICKSERV_H__
#define		__INCLUDED_LIBNICKSERV_H__
#define		E			extern

E	char		ModuleVersion[];
E	char		ModuleName[];
E	char		ModuleType[];


module_t	*this_module_p;

/*
---------------------------------------------------------------------------------------------------------------------------------
    DEFINES
---------------------------------------------------------------------------------------------------------------------------------
*/
#define		NICKSERV_SAVE_CYCLE		120		/* How often we save our databases */
#define		SERVICES_ADMIN_SAVE_CYCLE	120		/* How often we save our ServicesAdmin database */
#define		NICKSERV_EXPIRE_CYCLE		300		/* How often we check for expired nickname registrations */
#define		NICKSERV_JUPE_EXPIRE_CYCLE	30		/* How often we check for expired nick jupes */
#define		NICKSERV_VALIDATE_CYCLE		30		/* How long before we kill a user that doesn't identify */


/*
---------------------------------------------------------------------------------------------------------------------------------
    PROTOTYPES
---------------------------------------------------------------------------------------------------------------------------------
*/
E	void		init_module					(struct _module *);
E	void		init_service					(struct _module *);

#endif		/* __INCLUDED_LIBNICKSERV_H__ */
