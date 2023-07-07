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

#ifndef __INCLUDED_TEXT_H__
#define __INCLUDED_TEXT_H__


/*
---------------------------------------------------------------------------------------------------------------------------------
 PROTOTYPES
---------------------------------------------------------------------------------------------------------------------------------
*/
extern	int		is_comment				(const char *);
extern	int		is_blank				(const char *);
extern	char		*lrange					(char *, int);
extern	char		*translate				(char *, char *);
extern	char		*token					(char *, int);
extern	int		tokenize				(char *, char);
extern	int		numtokens				(char *);
extern	char		*strtoupper				(char *);
extern	int		strcascmp				(char *, char *);
extern	char		*chop					(char *);
extern	char		*crypt_sha256				(char *);
extern	char		*crypt_sha512				(char *);

#endif		/* __INCLUDED_TEXT_H__ */
