/*
===============================================================================
| COPYRIGHT (C) 2003-2007 RSGuru, All rights reserved.
| SUMMARY:
|   General facility for reading lines of input.
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
#include "inc/kapi.h"
#include "lib/inc/readline.h"


/* redefine some useful and maybe missing utilities to avoid conflicts */

#define puts    (rl->io.puts)
#define putc(c) \
do { \
    if (ISALPHA(c)) rl->io.putc(c); \
} while (0)

#define getc    (rl->io.getc)
#define echo    (rl->options & RS_OPT_ECHO)


#define LOOP_INIT(elm) \
    ((elm)->next = (elm));
    
#define LOOP_INSERT(looplm, elm) \
    do { \
        (elm)->next = (looplm)->next; \
        (looplm)->next = (elm); \
    } while (0)

#define	LOOP_NEXT(elm) \
    ((elm)->next);
    

#define	LOOP_DOEACH(var, looplm) \
    (var) = (looplm); \
    do {

#define	LOOP_DOEND(var, looplm) \
    (var) = LOOP_NEXT(var); \
    } while ((var) != (looplm));



#ifndef whitespace
#define whitespace(c)   (((c) == ' ') || ((c) == '\t'))
#endif

#ifndef wordend
#define wordend(c)      (((c) == ' ') || ((c) == '\0'))
#endif

#ifndef trim_left
#define trim_left(_line) \
    do { \
        while (whitespace(*(_line))) (_line)++; \
    } while (0)
#endif


enum {
    RL_INCMD_NULL,
    RL_INCMD_HELP,
    RL_INCMD_BACK,
    RL_INCMD_TOP,
    RL_INCMD_QUIT
};

enum {
    RL_OK,
    RL_AMBIGUITY,
    RL_MATCH_NULL,
    RL_MATCH_ENTIRE,
    RL_MATCH_PART
};


#define	E	0x00	/* Even parity. */
#define	O	0x80	/* Odd parity. */
#define	PARITY(c)	(char_type[c] & O)

#define	ALPHA	0x40	/* Alpha or underscore. */
#define	ISALPHA(c)	1//(char_type[c] & ALPHA)

#define	CCLASSMASK	0x3f
#define	CCLASS(c)	(char_type[c] & CCLASSMASK)

#define	BS	BACKSPACE
#define	CC	CONTROL
#define	CR	RETURN
#define	NA	ORDINARY | ALPHA
#define	NL	NEWLINE
#define	NO	ORDINARY
#define	TB	TAB
#define	VT	VTAB

static u_char const char_type[] = {
	E|CC, O|CC, O|CC, E|CC, O|CC, E|CC, E|CC, O|CC,	/* nul - bel */
	O|BS, E|TB, E|NL, O|CC, E|VT, O|CR, O|CC, E|CC, /* bs - si */
	O|CC, E|CC, E|CC, O|CC, E|CC, O|CC, O|CC, E|CC, /* dle - etb */
	E|CC, O|CC, O|CC, E|CC, O|CC, E|CC, E|CC, O|CC, /* can - us */
	O|NO, E|NO, E|NO, O|NO, E|NO, O|NO, O|NO, E|NO, /* sp - ' */
	E|NO, O|NO, O|NO, E|NO, O|NO, E|NO, E|NO, O|NO, /* ( - / */
	E|NA, O|NA, O|NA, E|NA, O|NA, E|NA, E|NA, O|NA, /* 0 - 7 */
	O|NA, E|NA, E|NO, O|NO, E|NO, O|NO, O|NO, E|NO, /* 8 - ? */
	O|NO, E|NA, E|NA, O|NA, E|NA, O|NA, O|NA, E|NA, /* @ - G */
	E|NA, O|NA, O|NA, E|NA, O|NA, E|NA, E|NA, O|NA, /* H - O */
	E|NA, O|NA, O|NA, E|NA, O|NA, E|NA, E|NA, O|NA, /* P - W */
	O|NA, E|NA, E|NA, O|NO, E|NO, O|NO, O|NO, O|NA, /* X - _ */
	E|NO, O|NA, O|NA, E|NA, O|NA, E|NA, E|NA, O|NA, /* ` - g */
	O|NA, E|NA, E|NA, O|NA, E|NA, O|NA, O|NA, E|NA, /* h - o */
	O|NA, E|NA, E|NA, O|NA, E|NA, O|NA, O|NA, E|NA, /* p - w */
	E|NA, O|NA, O|NA, E|NO, O|NO, E|NO, E|NO, O|CC, /* x - del */
	/*
	 * Meta chars; should be settable per character set;
	 * for now, treat them all as normal characters.
	 */
	NA,   NA,   NA,   NA,   NA,   NA,   NA,   NA,
	NA,   NA,   NA,   NA,   NA,   NA,   NA,   NA,
	NA,   NA,   NA,   NA,   NA,   NA,   NA,   NA,
	NA,   NA,   NA,   NA,   NA,   NA,   NA,   NA,
	NA,   NA,   NA,   NA,   NA,   NA,   NA,   NA,
	NA,   NA,   NA,   NA,   NA,   NA,   NA,   NA,
	NA,   NA,   NA,   NA,   NA,   NA,   NA,   NA,
	NA,   NA,   NA,   NA,   NA,   NA,   NA,   NA,
	NA,   NA,   NA,   NA,   NA,   NA,   NA,   NA,
	NA,   NA,   NA,   NA,   NA,   NA,   NA,   NA,
	NA,   NA,   NA,   NA,   NA,   NA,   NA,   NA,
	NA,   NA,   NA,   NA,   NA,   NA,   NA,   NA,
	NA,   NA,   NA,   NA,   NA,   NA,   NA,   NA,
	NA,   NA,   NA,   NA,   NA,   NA,   NA,   NA,
	NA,   NA,   NA,   NA,   NA,   NA,   NA,   NA,
	NA,   NA,   NA,   NA,   NA,   NA,   NA,   NA,
};
#undef	BS
#undef	CC
#undef	CR
#undef	NA
#undef	NL
#undef	NO
#undef	TB
#undef	VT


/*
 * execute the given command by calling callback with appropriate
 * arguments */
static void readline_exec(
    readline_t     __p_* rl,
    readline_cmd_t __p_* cmd,
    char __p_* line
    )
{
    char *argv[RSI_MAX_ARGS];
    int argc = 0;

    /* Cut into arguments */
    argv[argc++] = cmd->name;
    
    while (*line && argc < RSI_MAX_ARGS - 1) {

        trim_left(line);

        if (*line == 0)
            break;

        argv[argc++] = line;

        while (*line && !whitespace(*line))
            line++;

        if (*line == 0)
            break;

        *line++ = 0;
    }

    argv[argc] = (char*)rl;
    
    /* Call command function if present. */
    if (cmd->handle) {
        cmd->handle(argc, &argv[0]);
    }
}


/* start a new line */
static void readline_newline(readline_t __p_* rl)
{
    readline_cmd_t __p_* cmd;
    
    /* Display start of new line. */
    puts(rl->prompt);

    cmd = rl->root_cmd;

    while (cmd->parent != NULL)
        cmd = cmd->parent;

    while (cmd != rl->root_cmd) {
    
        puts(cmd->name);
        putc('>');
        cmd = cmd->child;
    }

    rl->index = 0;
    putc(' ');
}


static int readline_match(char __p_* name, char __p_* __p_* line)
{
    char __p_* _line;
    int  match;

    _line = *line;
    
    trim_left(_line);

    if (*_line != '\0') {
    
        while (*name && *name == *_line) {
            name++;
            _line++;
        }
        
        if (*_line == 0 ||
            whitespace(*_line))
        {
            if (*name == 0)
                match = RL_MATCH_ENTIRE;
            else
                match = RL_MATCH_PART;

        } else {
            match = RL_MATCH_NULL;
        }
    }
    
    trim_left(_line);
    *line = _line;

    return match;
}

static int readline_lookup(
        readline_cmd_t __p_* __p_* cmd,
        char __p_* __p_* line
        )
{
    readline_cmd_t __p_* _cmd;
    readline_cmd_t __p_* _found;
    char __p_* _line;
    int match;
    
    _found = NULL;

    LOOP_DOEACH(_cmd, *cmd)
    _line = *line;
    match = readline_match(_cmd->name, &_line);

    if (match == RL_MATCH_ENTIRE) {
        /* match */
        _found = _cmd;
        break;
    }
    /* See if match partially. */
    else if (match == RL_MATCH_PART) {

        if (_found != NULL)
            return RL_AMBIGUITY;

        /* save command */
        _found = _cmd;
    }
    LOOP_DOEND(_cmd, *cmd)


    if (_found != NULL) {

        *cmd = _found; 
    }
    else {
        /* Not match. */
        *cmd = NULL;
        return RL_MATCH_NULL;
    }
    
    *line = _line;
    
    return RL_OK;
}


/* Display command help info. */
void readline_help(
    readline_t __p_* rl,
    readline_cmd_t __p_* cmd,
    unsigned int space)
{
    unsigned int i;
    
    /* Show command name. */
    puts(cmd->name);

    for (i = strlen(cmd->name); i < space; i++)
        putc(' ');

    puts(": ");

    if(cmd->help)
        puts(cmd->help);
    else
        puts("<No help available>");

    putc('\n');
}


/* Display all loop commands help info. */
void readline_dohelp(readline_t __p_* rl, readline_cmd_t __p_* cmd)
{
    readline_cmd_t __p_* _cmd;
    unsigned int len = 0;
    
    /* Find the maximum length. */
    LOOP_DOEACH(_cmd, cmd)
    if (len < strlen(_cmd->name))
        len = strlen(_cmd->name);
    LOOP_DOEND(_cmd, cmd)

    len += 2;
    
    /* Display command help string. */
    LOOP_DOEACH(_cmd, cmd)
    readline_help(rl, _cmd, len);
    LOOP_DOEND(_cmd, cmd)
}

/* Function inner command help info. */
void readline_inhelp(readline_t __p_* rl)
{
    puts("?         Display help on given or available commands.\n");
    puts(".         Goes back one command level.\n");
    puts("..        Goes back top command level.\n");
    puts("<TAB>     Auto-completion.\n");
    puts("<CR>      Execute command line.\n");
    puts("CTRL-P    Recall previous input line.\n");
    puts("CTRL-N    Recall next input line.\n");
    puts("quit      Quit command.\n");
}


static int readline_search(
    readline_cmd_t __p_* __p_* cmd,
    char __p_* __p_* line
    )
{
    readline_cmd_t __p_* _cmd;
    char __p_* _line;
    int match;
    
    _cmd = *cmd;
    _line = *line;

    for (;;) {
        
        match = readline_lookup(&_cmd, &_line);

        /* See if command found. */
        if (match != RL_OK) {
            /* Not match. */
            break;
        }
        
        /* Since.
         * command has found. */
        if (*_line == '\0' ||
            (*_line == '?' &&
            wordend(*(_line+1))))
        {
            break;
        }

        if (_cmd->child == NULL)
            break;

        _cmd = _cmd->child;
    }

    *cmd = _cmd;
    *line = _line;
    
    return match;
}

static int readline_incmd(char __p_* line)
{
    int incmd = RL_INCMD_NULL;

    if (line[0] == '?')
    {
        line += 1;
        incmd = RL_INCMD_HELP;
    }
    else if (line[0] == '.' &&
        line[1] == '.')
    {
        line += 2;
        incmd = RL_INCMD_TOP;
    }
    else if (line[0] == '.')
    {
        line += 1;
        incmd = RL_INCMD_BACK;
    }
    else if (line[0] == 'h' &&
        line[1] == 'e' &&
        line[2] == 'l' &&
        line[3] == 'p')
    {
        line += 4;
        incmd = RL_INCMD_HELP;
    }
    else if (line[0] == 'q' &&
        line[1] == 'u' &&
        line[2] == 'i' &&
        line[3] == 't')
    {
        line += 4;
        incmd = RL_INCMD_QUIT;
    }
    
    trim_left(line);
    
    if (line[0] == 0)
        return incmd;

    return RL_INCMD_NULL;
}

static void readline_linein(readline_t __p_* rl, char __p_* line)
{
    int status;
    readline_cmd_t __p_* _cmd;
    

    trim_left(line);

    status = readline_incmd(line);

    switch (status) {
    
    case RL_INCMD_HELP:
    
        /* Display root command help. */
        readline_inhelp(rl);
        break;
        
    case RL_INCMD_BACK:
    
        /* return to back level. */
        if (rl->root_cmd->parent != NULL)
            rl->root_cmd = rl->root_cmd->parent;

        break;
        
    case RL_INCMD_TOP:

        _cmd = rl->root_cmd;

        while (_cmd->parent != NULL)
            _cmd = _cmd->parent;

        rl->root_cmd = _cmd;

        break;
        
    case RL_INCMD_QUIT:

        readline_quit(rl);
        break;
        
    default:
        _cmd = rl->root_cmd;
        
        /* See if empty line. */
        if (*line != '\0') {
        
            /* Not empty line. */
            status = readline_search(&_cmd, &line);
            
            if (status == RL_OK) {
            
                if (_cmd->child == NULL)
                    readline_exec(rl, _cmd, line);
                else if (*line == '\0')
                    rl->root_cmd = _cmd->child;
                else if (*line == '?' &&
                    wordend(*(line+1)))
                {
                    readline_dohelp(rl, _cmd->child);
                }

            } else if (status == RL_AMBIGUITY) {
            
                puts("Ambiguity: ");
                puts(line);
                putc('\n');
                
            } else if (status == RL_MATCH_NULL) {
            
                puts("No such command: ");
                puts(line);
                putc('\n');
            }

            /* Add line to history. */
            if (++rl->hist == RSI_HISTORY_DEPTH)
                rl->hist = 0;
        }
    }
}

/* try to complete current command line
 */
static void readline_complete(readline_t __p_* rl)
{
    readline_cmd_t __p_* _cmd;
    char __p_* _line;
    char __p_* _p;
    int match, i;
    
    _cmd = rl->root_cmd;
    _line = rl->line[rl->hist];

    trim_left(_line);

    if (*_line == 0) {
        putc('\n');
        readline_dohelp(rl, rl->root_cmd);
        readline_newline(rl);
        return;
    }
    
    for (;;) {

        _p = _line;
        match = readline_lookup(&_cmd, &_p);

        /* See if command found. */
        if (match == RL_MATCH_NULL) {
            /* Not match. */
            break;
        }
        
        i = 0;
        while (_line[i] && !whitespace(_line[i]))
            i++;

        /* append line. */
        if (_line[i] == '\0') {

            for ( ; ; i++) {

                if (_cmd->name[i] == '\0')
                    break;

                _line[i] = _cmd->name[i];
                _line[i+1] = '\0';
                rl->index++;

                _p = _line;
                if (match != readline_lookup(&_cmd, &_p)) {
                
                    _line[i] = '\0';
                    rl->index--;
                    break;
                }
                
                putc(_line[i]);
            }
            
            break;
        }

        /* See if command neither match and nor the end-word. */
        if (match == RL_AMBIGUITY)
            break;
        
        /* command match and not the end-word. */
        _cmd = _cmd->child;
        _line = _p;
    }
}


/* character parse. */
static void readline_cparse(readline_t __p_* rl, char c)
{
    RSI_REG char __p_* line;

    line = rl->line[rl->hist];
    line[rl->index] = 0;

    switch (c) {

    /* validate command */
    case '\r':
        c = '\n';
    case '\n':
        /* first, echo the newline */
        if (echo)
            putc(c);

        readline_linein(rl, line);
        readline_newline(rl);

        break;

    /* backspace */
    case 8:
    case 127:
        if (rl->index > 0) {
        
            puts("\b \b");
            rl->index--;
            line[rl->index] = 0;
        }
        break;

    /* CTRL-N: next in history */
    case 14:
    {
        int nextline = (rl->hist + 1) % RSI_HISTORY_DEPTH;

        line = rl->line[nextline];
        
        if (line[0]) {
        
            /* Fill the rest of the line with spaces */
            while (rl->index-- > strlen(line))
                puts("\b \b");
            putc('\r');
            readline_newline(rl);
            puts(line);
            rl->index = strlen(line);
            rl->hist = nextline;
        }

        break;
    }
    
    /* CTRL-P: back in history */
    case 16:
    {
        int prevline = (rl->hist + RSI_HISTORY_DEPTH - 1) % RSI_HISTORY_DEPTH;

        line = rl->line[prevline];

        if (line[0]) {
        
            /* Fill the rest of the line with spaces */
            while (rl->index-- > strlen(line))
                puts("\b \b");
                
            putc('\r');
            readline_newline(rl);
            puts(line);
            rl->index = strlen(line);
            rl->hist = prevline;
        }

        break;
    }

    /* TAB: autocompletion */
    case 9:
    case '!':
        readline_complete(rl);

        break;
    
    /* Any input character */
    default:
        if (rl->index < RSI_INBUFF_SZ - 1) {
            if (echo)
                putc(c);
            line[rl->index++] = c;
        }
    }
}

/* New character input. */
void readline_charin(readline_t __p_* rl, char c)
{
    RSI_ASSERT(rl != NULL);
    RSI_ASSERT(rl->init == RSI_INITED);

    /*
     * Filter characters here. */
     
    readline_cparse(rl, c);
}


/* Set shell prompt. */
void readline_set_prompt(readline_t __p_* rl, char __p_* s)
{
    RSI_REG int i;

    RSI_ASSERT(rl != NULL);
    RSI_ASSERT(rl->init == RSI_INITED);
    RSI_ASSERT(s != NULL);
    
    for (i = 0; i < RSI_PROMPT_SZ; i++) {
    
        rl->prompt[i] = s[i];

        if (s[i] == '\0')
            break;
    }

    /* prevent string crash. */
    rl->prompt[RSI_PROMPT_SZ - 1] = '\0';
}


/* Add a new command */
status_t readline_add_cmd(
    readline_t      __p_* rl,
    char            __p_* parent,
    readline_cmd_t  __p_* cmd
    )
{
    int match;
    readline_cmd_t __p_* _cmd;
    char __p_* _line;

    if (parent && !rl->root_cmd)
        return RS_ENULL;

    for (_cmd = cmd; _cmd->name != NULL; _cmd++) {
    
        if (parent != NULL) {

            readline_cmd_t __p_* _root_cmd;

            _root_cmd = rl->root_cmd;
            _line = parent;
            
            match = readline_search(&_root_cmd, &_line);

            if (match == RL_OK) {

                _cmd->parent = _root_cmd;
                if (_root_cmd->child == NULL) {
                    _cmd->next = _cmd;
                    _root_cmd->child = _cmd;
                } else {
                    _cmd->next = _root_cmd->child->next;
                    _root_cmd->child->next = _cmd;
                }
            }
            
        } else {
        
            if (rl->root_cmd == NULL) {
                _cmd->next = _cmd;
                rl->root_cmd = _cmd;
            } else {
                _cmd->next = rl->root_cmd->next;                
                rl->root_cmd->next = _cmd;
            }
        }
    }

    return RS_EOK;
}


status_t readline_quit(readline_t __p_* rl)
{
    RSI_ASSERT(rl != NULL);
    RSI_ASSERT(rl->init == RSI_INITED);

    rl->quit = 1;

    return RS_EOK;
}


status_t readline_loop(readline_t __p_* rl)
{
    RSI_REG char c;

    RSI_ASSERT(rl != NULL);
    RSI_ASSERT(rl->init == RSI_INITED);

    readline_newline(rl);
    
    /* main loop */
    while (rl->quit == 0) {
    
        c = getc();

        readline_charin(rl, c);
    }

    return RS_EOK;
}


status_t readline_init(
        readline_t      __p_* rl,
        char            __p_* prompt,
        readline_io_t   __p_* iofnt,
        int8u  options
        )
{
    int i;
    
    RSI_ASSERT(rl != NULL);
    RSI_ASSERT(iofnt != NULL);

    rl->root_cmd = NULL;
    rl->hist = 0;
    rl->index = 0;
    rl->quit = 0;

    for (i = 0; i < RSI_HISTORY_DEPTH; i++)
        rl->line[i][0] = 0;
    
    if (iofnt != NULL)
        rl->io = *iofnt;

    rl->prompt[0] = '#';
    rl->prompt[1] = '\0';

    rl->options = options;
    
#if CFG_DEBUG > 0
    /* Initialized flag. */
    rl->init = RSI_INITED;
#endif

    if (prompt != NULL)
        readline_set_prompt(rl, prompt);

    /* Return succeed. */
    return RS_EOK;
}


