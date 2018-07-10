#ifndef __LINEOP_H__
#define __LINEOP_H__

char* line_strip(char *str);
int line_contains(char *str, char *substr);
int line_get_id_cmd(char *str, int *id, char *cmd);

#endif
