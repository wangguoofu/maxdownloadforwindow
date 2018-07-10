//......
#include "max_debug.h"
#include <stdarg.h>
#include <stdio.h>
#include <memory.h>
#include <QDebug>

#define PRINT_BUFF_SIZE 2048
static char s_pPrintBuff[PRINT_BUFF_SIZE];
static char strBuff[PRINT_BUFF_SIZE];

void maxd_print(const char *fmt, ...)
{
        char *p = s_pPrintBuff;
        va_list args;

        memset(s_pPrintBuff,0,sizeof(s_pPrintBuff));

        va_start(args, fmt);
        vsprintf(p, fmt, args);
        va_end(args);

        qDebug()<<s_pPrintBuff;
}

char * maxd_strbuff(void)
{
         return strBuff;
}
