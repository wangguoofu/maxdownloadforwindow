//for line operation
#include "max_download.h"
#include "lineop.h"
#include<ctype.h>
#include<string.h>
#include "max_debug.h"

char* line_strip(char *str)
{/*
  char *p = str;
  char *q = str;
 
  while( *q++ ); q -= 2;
  while( p <= q && isspace(*q) ) q--;
  while( p <= q && isspace(*p) ) p++;
  while( p <= q ) *str++ = *p++;
 
  *str = '\0';
  */
  int len = strlen(str);
  if(str[len-1] == '\n')
    str[len-1] = '\0';
  return str;
}

int line_contains(char *str, char *substr)
{
  char* p=NULL;

  p = strstr(str,substr);
  if(p)
    return 1;
  else
    return 0;
}

int line_get_id_cmd(char *str, int *id, char *cmd)
{
  char *tmp[10];
  char *p = str;
  int num_pos = 0;

  while(*(str+num_pos)!='.')
    num_pos++;

  num_pos++;
  memcpy(tmp, str+num_pos, 7);
  tmp[8] = '\0';
  //MAXD_PRINT("tmp=%s\n",tmp);
  *id = atoi((const char *)tmp);

  if(strstr(str, "host"))
    p = p+num_pos+7+6;
  else
    p = p+num_pos+7+4;

  while(*p!= '.')
    {
      *cmd=*p;
      p++;
      cmd++;
    }
  *cmd = '\0';

  return 1;
}


