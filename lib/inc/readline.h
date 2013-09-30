/*
===============================================================================
| COPYRIGHT (C) 2003-2007 RSGuru, All rights reserved.
| SUMMARY:
|   The names of functions callable from within readline.
|
|
| LICENSE INFORMATION
|   RS-RTOS is free software; you can redistribute it and/or modify it under
|   terms of the GNU General Public License as published by the
|   Free Software Foundation; either version 2, or (at your option) any
|   later version.
|
|   RS-RTOS is distributed in the hope that it will be useful,
|   but WITHOUT ANY WARRANTY; without even the implied warranty of
|   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
|   General Public License for more details.
|
|   You should have received a copy of the GNU General Public License
|   along with RS-RTOS; see file COPYING. If not, write to the Free Software
|   Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
|
|   As a special exception, including RS-RTOS header files in a file,
|   instantiating RS-RTOS generics or templates, or linking other files
|   with RS-RTOS objects to produce an executable application, does not
|   by itself cause the resulting executable application to be covered
|   by the GNU General Public License. This exception does not
|   however invalidate any other reasons why the executable file might be
|   covered by the GNU Public License.
|
|
| DESCRIPTION:
|   See http://www.RS-RTOS.org for documentation, latest information, license
|   and contact details.
|   email:ruanhaishen@gmail.com
|
|
| AUTHOR & LOG:
|   Haishen Ruan <ruanhaishen@gmail.com>:
|   31 Aug 2006, Created it.
|
|
=============================================================================*/

#ifndef __READLINE_H__
#define __READLINE_H__

#ifdef __cplusplus
extern "C" {
#endif
/*===========================================================================*/

#define RSI_PROMPT_SZ       16
#define RSI_HISTORY_DEPTH   16
#define RSI_INBUFF_SZ       256

#ifndef RSI_MAX_ARGS
#define RSI_MAX_ARGS        16
#endif


/* Character type information. */
#define	ORDINARY	0
#define	CONTROL		1
#define	BACKSPACE	2
#define	NEWLINE		3
#define	TAB		    4
#define	VTAB		5
#define	RETURN		6


#define RETRIEVE_IO(_argc, _argv) \
    (&((readline_t *)_argv[_argc])->io)


typedef void (*cmd_handle_t)(int argc, char **argv);


typedef struct _struct_readline_cmd
{
    struct _struct_readline_cmd __p_* parent;   /* NULL if top level command */
    struct _struct_readline_cmd __p_* next;     /* pointer to next command */
    struct _struct_readline_cmd __p_* child;    /* pointer to child command */
    char    __p_* name;         /* command input name, not 0 */
    char    __p_* help;         /* help string, can be 0 */
    cmd_handle_t handle;        /* function to launch on cmd, can be 0 */
} readline_cmd_t;

#define COMMAND_BEGIN(cmd) \
    readline_cmd_t cmd [] = {

#define COMMAND(name, help, handle) \
    {NULL, NULL, NULL, name, help, handle} ,

#define COMMAND_END \
    {NULL, NULL, NULL, NULL, NULL, NULL} };

typedef void (*putc_t)(char c);
typedef void (*puts_t)(char *s);
typedef char (*getc_t)(void);

typedef struct _struct_readline_io
{
    putc_t  putc;
    puts_t  puts;
    getc_t  getc;
} readline_io_t;


typedef struct _struct_readline
{
    char prompt[RSI_PROMPT_SZ];
    char line[RSI_HISTORY_DEPTH][RSI_INBUFF_SZ];
    unsigned int hist;
    unsigned int index;
    readline_cmd_t __p_* root_cmd;
    readline_io_t io;

    /* 1 if quit */
    int8u  quit;
    
    /* if echo input character. */
    int8u  options;
    
#if CFG_DEBUG > 0
    int8u  init;
#endif

} readline_t;



/*
 * Functions below are provided by the readline module. */

/* new character input */
void readline_charin(readline_t __p_* rl, char c);

/* change tinysh prompt */
void readline_set_prompt(readline_t __p_* rl, char __p_* s);

/* add a new command */
status_t readline_add_cmd(
    readline_t      __p_* rl,
    char            __p_* parent,
    readline_cmd_t  __p_* cmd
    );

status_t readline_loop(readline_t __p_* rl);

status_t readline_init(
        readline_t      __p_* rl,
        char            __p_* prompt,
        readline_io_t   __p_* iofnt,
        int8u  options
        );

status_t readline_quit(readline_t __p_* rl);

/*===========================================================================*/
#ifdef __cplusplus
} /* end of extern "C" */
#endif

#endif


