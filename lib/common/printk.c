/*
===============================================================================
| COPYRIGHT (C) 2003-2007 Haishen Ruan, All rights reserved.
| SUMMARY: 
|   printk support.
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
=============================================================================*/
/*===========================================================================*/
#include <stdarg.h>
#include "arch/arch.h"
#include "inc/const.h"
#include "inc/cque.h"
#include "inc/kernel.h"
#include "inc/memory.h"
#include "inc/ipc.h"
#include "inc/device.h"
#include "inc/kapi.h"
#include "driver/driver.h"


#if CFG_DUMP_BUFFSZ > 0

static unsigned char sbuf[CFG_DUMP_BUFFSZ];

#ifdef _CTYPE_DISABLE_MACROS
/*
 * NOTE! This ctype does not handle EOF like the standard C
 * library is required to.
 */
#define _U  0x01    /* upper */
#define _L  0x02    /* lower */
#define _D  0x04    /* digit */
#define _C  0x08    /* cntrl */
#define _P  0x10    /* punct */
#define _S  0x20    /* white space (space/lf/tab) */
#define _X  0x40    /* hex digit */
#define _SP 0x80    /* hard space (0x20) */

static unsigned char _ctype[] = {
    _C,_C,_C,_C,_C,_C,_C,_C,                    /* 0-7 */
    _C,_C|_S,_C|_S,_C|_S,_C|_S,_C|_S,_C,_C,     /* 8-15 */
    _C,_C,_C,_C,_C,_C,_C,_C,                    /* 16-23 */
    _C,_C,_C,_C,_C,_C,_C,_C,                    /* 24-31 */
    _S|_SP,_P,_P,_P,_P,_P,_P,_P,                /* 32-39 */
    _P,_P,_P,_P,_P,_P,_P,_P,                    /* 40-47 */
    _D,_D,_D,_D,_D,_D,_D,_D,                    /* 48-55 */
    _D,_D,_P,_P,_P,_P,_P,_P,                    /* 56-63 */
    _P,_U|_X,_U|_X,_U|_X,_U|_X,_U|_X,_U|_X,_U,  /* 64-71 */
    _U,_U,_U,_U,_U,_U,_U,_U,                    /* 72-79 */
    _U,_U,_U,_U,_U,_U,_U,_U,                    /* 80-87 */
    _U,_U,_U,_P,_P,_P,_P,_P,                    /* 88-95 */
    _P,_L|_X,_L|_X,_L|_X,_L|_X,_L|_X,_L|_X,_L,  /* 96-103 */
    _L,_L,_L,_L,_L,_L,_L,_L,                    /* 104-111 */
    _L,_L,_L,_L,_L,_L,_L,_L,                    /* 112-119 */
    _L,_L,_L,_P,_P,_P,_P,_C,                    /* 120-127 */
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,            /* 128-143 */
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,            /* 144-159 */
    _S|_SP,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,   /* 160-175 */
    _P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,       /* 176-191 */
    _U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,       /* 192-207 */
    _U,_U,_U,_U,_U,_U,_U,_P,_U,_U,_U,_U,_U,_U,_U,_L,       /* 208-223 */
    _L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,       /* 224-239 */
    _L,_L,_L,_L,_L,_L,_L,_P,_L,_L,_L,_L,_L,_L,_L,_L        /* 240-255 */
};

#define __ismask(x) (_ctype[(int)(unsigned char)(x)])

#define isalnum(c)  ((__ismask(c)&(_U|_L|_D)) != 0)
#define isalpha(c)  ((__ismask(c)&(_U|_L)) != 0)
#define iscntrl(c)  ((__ismask(c)&(_C)) != 0)
#define isdigit(c)  ((__ismask(c)&(_D)) != 0)
#define isgraph(c)  ((__ismask(c)&(_P|_U|_L|_D)) != 0)
#define islower(c)  ((__ismask(c)&(_L)) != 0)
#define isprint(c)  ((__ismask(c)&(_P|_U|_L|_D|_SP)) != 0)
#define ispunct(c)  ((__ismask(c)&(_P)) != 0)
#define isspace(c)  ((__ismask(c)&(_S)) != 0)
#define isupper(c)  ((__ismask(c)&(_U)) != 0)
#define isxdigit(c) ((__ismask(c)&(_D|_X)) != 0)

#define isascii(c) (((unsigned char)(c))<=0x7f)
#define toascii(c) (((unsigned char)(c))&0x7f)

#endif

int do_div(long* n,int base)
{
    int __res;
    __res = (*n) % (int) base;
    *n = (*n) / (int) base;
    return __res;
}
/*
#define do_div(n,base) ({ \
int __res; \
__res = ((long) n) % (int) base; \
n = ((long) n) / (int) base; \
__res; })*/


static mmsz_t __strnlen(const char * s, mmsz_t count)
{
    const char *sc;
    for (sc = s; count-- && *sc != '\0'; ++sc)
    /* nothing */;
    return sc - s;
}

static unsigned char __tolower(unsigned char c)
{
    if (isupper(c))
    c -= 'A'-'a';
    return c;
}

static unsigned char __toupper(unsigned char c)
{
    if (islower(c))
    c -= 'a'-'A';
    return c;
}

#define tolower(c) __tolower(c)
#define toupper(c) __toupper(c)


/**
 * simple_strtoul - convert a string to an unsigned long
 * @cp: The start of the string
 * @endp: A pointer to the end of the parsed string will be placed here
 * @base: The number base to use
 */
unsigned long simple_strtoul(const char *cp,char **endp,unsigned int base)
{
    unsigned long result = 0,value;

    if (!base) {
        base = 10;
        if (*cp == '0') {
            base = 8;
            cp++;
            if ((*cp == 'x') && isxdigit(cp[1])) {
                cp++;
                base = 16;
            }
        }
    }
    while (isxdigit(*cp) &&
           (value = isdigit(*cp) ? *cp-'0' : toupper(*cp)-'A'+10) < base) {
        result = result*base + value;
        cp++;
    }
    if (endp)
        *endp = (char *)cp;
    return result;
}

/**
 * simple_strtol - convert a string to a signed long
 * @cp: The start of the string
 * @endp: A pointer to the end of the parsed string will be placed here
 * @base: The number base to use
 */
long simple_strtol(const char *cp,char **endp,unsigned int base)
{
    if(*cp=='-')
        return -(long)simple_strtoul(cp+1,endp,base);
    return simple_strtoul(cp,endp,base);
}

/**
 * simple_strtoull - convert a string to an unsigned long long
 * @cp: The start of the string
 * @endp: A pointer to the end of the parsed string will be placed here
 * @base: The number base to use
 */
long simple_strtoull(const char *cp,char **endp,unsigned int base)
{
    long result = 0,value;

    if (!base) {
        base = 10;
        if (*cp == '0') {
            base = 8;
            cp++;
            if ((*cp == 'x') && isxdigit(cp[1])) {
                cp++;
                base = 16;
            }
        }
    }
    while (isxdigit(*cp) && (value = isdigit(*cp) ? *cp-'0' : (islower(*cp)
        ? toupper(*cp) : *cp)-'A'+10) < (long)base) {
        result = result*base + value;
        cp++;
    }
    if (endp)
        *endp = (char *)cp;
    return result;
}

/**
 * simple_strtoll - convert a string to a signed long long
 * @cp: The start of the string
 * @endp: A pointer to the end of the parsed string will be placed here
 * @base: The number base to use
 */
long simple_strtoll(const char *cp,char **endp,unsigned int base)
{
    if(*cp=='-')
        return -simple_strtoull(cp+1,endp,base);
    return simple_strtoull(cp,endp,base);
}

static int skip_atoi(const char **s)
{
    int i=0;

    while (isdigit(**s))
        i = i*10 + *((*s)++) - '0';
        
    return i;
}


#define ZEROPAD 1       /* pad with zero */
#define SIGN    2       /* unsigned/signed long */
#define PLUS    4       /* show plus */
#define SPACE   8       /* space if plus */
#define LEFT    16      /* left justified */
#define SPECIAL 32      /* 0x */
#define LARGE   64      /* use 'ABCDEF' instead of 'abcdef' */

static int number(
    char *  buff,
    int     size,
    long    num,
    int     base,
    int     width,
    int     precision,
    int     type
    )
{
    char c,sign,tmp[66];
    const char *digits;
    static const char small_digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    static const char large_digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int i;
    int bi = 0;

    digits = (type & LARGE) ? large_digits : small_digits;
    if (type & LEFT)
        type &= ~ZEROPAD;
    if (base < 2 || base > 36)
        return 0;
    c = (type & ZEROPAD) ? '0' : ' ';
    sign = 0;
    if (type & SIGN) {
        if (num < 0) {
            sign = '-';
            num = -num;
            width--;
        } else if (type & PLUS) {
            sign = '+';
            width--;
        } else if (type & SPACE) {
            sign = ' ';
            width--;
        }
    }
    if (type & SPECIAL) {
        if (base == 16)
            width -= 2;
        else if (base == 8)
            width--;
    }
    i = 0;
    if (num == 0)
        tmp[i++]='0';
    else while (num != 0)
        tmp[i++] = digits[do_div(&num,base)];
    if (i > precision)
        precision = i;
    width -= precision;
    if (!(type&(ZEROPAD+LEFT))) {
        while(width-->0) {
            if (bi < size)
                buff[bi++] = ' ';
        }
    }
    if (sign) {
        if (bi < size)
            buff[bi++] = sign;
    }
    if (type & SPECIAL) {
        if (base==8) {

            if (bi < size)
                buff[bi++] = '0';

        } else if (base==16) {

            if (bi < size)
                buff[bi++] = '0';
                
            if (bi < size)
                buff[bi++] = 'x';/*digits[33];*/
        }
    }
    if (!(type & LEFT)) {
        while (width-- > 0) {
            if (bi < size)
                buff[bi++] = c;
        }
    }
    while (i < precision--) {

        if (bi < size)
            buff[bi++] = '0';
    }
    while (i-- > 0) {
        if (bi < size)
            buff[bi++] = tmp[i];
    }
    while (width-- > 0) {
        if (bi < size)
            buff[bi++] = ' ';
    }
    
    return bi;
}


int sprintf(char * buf, const char *fmt, ...);

int __vsprintf(char *buff, int size, const char *fmt, va_list args)
{
    int len;
    unsigned long num;
    int i, base;
    char c;
    const char *s;

    int flags;      /* flags to number() */

    int field_width;    /* width of output field */
    int precision;      /* min. # of digits for integers; max
                   number of chars for from string */
    int qualifier;      /* 'h', 'l', or 'L' for integer fields */
                /* 'z' support added 23/7/1999 S.H.    */
                /* 'z' changed to 'Z' --davidm 1/25/99 */

    int bi = 0;

    /* Reserve a byte for null. */
    size--;

    for (; *fmt; ++fmt) {
    
        if (*fmt != '%') {
            /**
             * Ordinary character */
            
            if (bi < size)
                buff[bi++] = *fmt;
            continue;
        }

        /* process flags */
        flags = 0;
        
    repeat:
        /* This also skips first '%'. */
        ++fmt;
        switch (*fmt) {
            case '-': flags |= LEFT;    goto repeat;
            case '+': flags |= PLUS;    goto repeat;
            case ' ': flags |= SPACE;   goto repeat;
            case '#': flags |= SPECIAL; goto repeat;
            case '0': flags |= ZEROPAD; goto repeat;
        }

        /* get field width */
        field_width = -1;
        if (isdigit(*fmt))
            field_width = skip_atoi(&fmt);

        /* get the precision */
        precision = -1;
        if (*fmt == '.') {
            ++fmt;  
            if (isdigit(*fmt))
                precision = skip_atoi(&fmt);
        }

        /* get the conversion qualifier */
        qualifier = -1;
        if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L' ||
            *fmt =='Z' || *fmt == 'z') {
            qualifier = *fmt;
            ++fmt;
            if (qualifier == 'l' && *fmt == 'l') {
                qualifier = 'L';
                ++fmt;
            }
        }

        /* default base */
        base = 10;

        switch (*fmt) {
        case 'c':
            if (!(flags & LEFT)) {
                while (--field_width > 0) {

                    if (bi < size)
                        buff[bi++] = ' ';
                }
            }
            c = (unsigned char)va_arg(args, int);

            if (bi < size)
                buff[bi++] = c;
                
            while (--field_width > 0) {

                if (bi < size)
                    buff[bi++] = ' ';
            }
            continue;

        case 's':
            s = va_arg(args, char *);
            if (!s)
                s = "<NULL>";

            len = __strnlen(s, precision);

            if (!(flags & LEFT)) {
                while (len < field_width--) {
                    if (bi < size)
                        buff[bi++] = ' ';
                }
            }
            for (i = 0; i < len; ++i) {

                if (bi < size)
                    buff[bi++] = *s++;
            }
            while (len < field_width--) {
                if (bi < size)
                    buff[bi++] = ' ';
            }
            continue;

        case 'P':
            flags |= LARGE;

        case 'p':
            if (field_width == -1) {
                field_width = 2*sizeof(void *);
                flags |= ZEROPAD;
            }
            bi += number(buff + bi, size - bi,
                    (unsigned long) va_arg(args, void *),
                    16, field_width, precision, flags);
            continue;


        case 'n':
            /* FIXME:
            * What does C99 say about the overflow case here? */
            if (qualifier == 'l') {
                long * ip = va_arg(args, long *);
                *ip = bi;
            } else if (qualifier == 'Z' || qualifier == 'z') {
                mmsz_t * ip = va_arg(args, mmsz_t *);
                *ip = bi;
            } else {
                int * ip = va_arg(args, int *);
                *ip = bi;
            }
            continue;

        case '%':
            if (bi < size)
                buff[bi++] = '%';
            continue;

            /* integer number formats - set up the flags and "break" */
        case 'o':
            base = 8;
            break;

        case 'X':
            flags |= LARGE;
        case 'x':
            base = 16;
            break;

        case 'd':
        case 'i':
            flags |= SIGN;
        case 'u':
            break;

        default:
            if (bi < size)
                buff[bi++] = '%';

            if (*fmt) {
                if (bi < size)
                    buff[bi++] = *fmt;
            } else {
                --fmt;
            }
            continue;
        }
        
        if (qualifier == 'L')
            num = va_arg(args, long);
        else if (qualifier == 'l') {
            num = va_arg(args, unsigned long);
            if (flags & SIGN)
                num = (signed long) num;
        } else if (qualifier == 'Z' || qualifier == 'z') {
            num = va_arg(args, mmsz_t);
        } else if (qualifier == 'h') {
            num = (unsigned short) va_arg(args, int);
            if (flags & SIGN)
                num = (signed short) num;
        } else {
            num = va_arg(args, unsigned int);
            if (flags & SIGN)
                num = (signed int) num;
        }
        bi += number(buff + bi, size - bi, num, base,
                field_width, precision, flags);
    }

    buff[bi] = '\0';

    /**
     * The trailing null byte doesn't count towards the total. */
    return bi;
}

int sprintf(char * buf, const char *fmt, ...)
{
    va_list args;
    int i;
    va_start(args, fmt);
    i = __vsprintf(buf, CFG_DUMP_BUFFSZ, fmt, args);
    va_end(args);
    return i;
}

void printk(const char *fmt, ...)
{
    va_list args;
    int i;
    va_start(args, fmt);
    i = __vsprintf(sbuf, CFG_DUMP_BUFFSZ, fmt, args);
    va_end(args);
    __ui_writec(sbuf, i);
}

#endif


/*===========================================================================*/


