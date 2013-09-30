/*
===============================================================================
| COPYRIGHT (C) 2003-2007 RSGuru, All rights reserved.
| SUMMARY:
|   <template file>
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
/*===========================================================================*/
#include <stdio.h>
#include "inc/kapi.h"
#include "lib/inc/readline.h"

extern int kbhit(void);
extern int getch(void);


static readline_t rl;

/* We must provide this function to use readline. */
static void shell_putc(char c)
{
    __ui_writec(&c, 1);
}


static void shell_puts(char *s)
{
    while (*s) {
        shell_putc(*s++);
    }
}

static char shell_getc(void)
{
    while (!kbhit())
        task_sleep(1);

    return getch();
}


static void shell_args(readline_io_t *io, int argc, char **argv)
{
    int i;
    char buff[256];
    
    for (i = 0; i < argc; i++) {
        sprintf(buff, "argv[%d]=\"%s\"\n", i, argv[i]);
        io->puts(buff);
    }    
}

static void command_foo(int argc, char **argv)
{
    readline_io_t *io;
    io = RETRIEVE_IO(argc, argv);

    io->puts("foo command called\n");
    shell_args(io, argc, argv);
}


static void command_item1(int argc, char **argv)
{
    readline_io_t *io;
    io = RETRIEVE_IO(argc, argv);

    io->puts("item1 command called\n");

    shell_args(io, argc, argv);
}

static void command_item2(int argc, char **argv)
{
    readline_io_t *io;
    io = RETRIEVE_IO(argc, argv);

    io->puts("item2 command called\n");
    shell_args(io, argc, argv);
}


COMMAND_BEGIN(top_cmd)
COMMAND("foo", "Foo command", command_foo)
COMMAND("ctx", "Contextual command", 0)
COMMAND_END

COMMAND_BEGIN(ctx_cmd)
COMMAND("item1", "First founction", command_item1)
COMMAND("item2", "Second founction", command_item2)
COMMAND_END


void shell_entry(arg_t arg)
{
    readline_io_t io;

    arg = arg;

    io.getc = shell_getc;
    io.putc = shell_putc;
    io.puts = shell_puts;
    
    readline_init(
            &rl,
            "#",
            &io,
            RS_OPT_HISTORY | RS_OPT_ECHO);

    /* Add commands */
    readline_add_cmd(&rl, NULL, top_cmd);

    /* Add sub commands */
    readline_add_cmd(&rl, "ctx", ctx_cmd);

    /* main loop */
    readline_loop(&rl);
    
    shell_puts("\b\b \bBye.\n");
}

/* Shell task create example. */
#if 0
#define RSI_SHELL_PRIO      RSI_IDLE_PRIO - 1
#define RSI_SHELL_STASZ     1024 * 50

unsigned char _shell_stacks[RSI_SHELL_STASZ];


void __shell_init(void)
{
    status_t err;

    /* Create shell task. */
    err = task_create(
              RSI_SHELL_PRIO,
              build_name('S', 'H', 'E', 'L'),
              shell_entry, 0,
              _shell_stacks, RSI_SHELL_STASZ,
              0);

    RSI_ASSERT(err == RS_EOK);
}
#endif

