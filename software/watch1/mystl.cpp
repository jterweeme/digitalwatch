#include <stdarg.h>
#include "misc.h"

namespace Utility
{

int sprintf(char *str, const char *format, ...)
{
    va_list argp;
    va_start(argp, format);

    for (const char *p = format; *p != '\0'; p++)
    {
        if (*p != '%')
        {
            *str++ = *p;
            continue;
        }
    }

    va_end(argp);
    return 0;
}

};

