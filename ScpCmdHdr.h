#ifndef __SCPCMDHDR_H__
#define __SCPCMDHDR_H__
/*
  FORMAT	C TYPE	PYTHON TYPE	STANDARD SIZE	NOTES
  x	pad byte	no value	 	 
  c	char	string of length 1	1	 
  b	signed char	integer	1	(3)
  B	unsigned char	integer	1	(3)
  ?	_Bool	bool	1	(1)
  h	short	integer	2	(3)
  H	unsigned short	integer	2	(3)
  i	int	integer	4	(3)
  I	unsigned int	integer	4	(3)
  l	long	integer	4	(3)
  L	unsigned long	integer	4	(3)
  q	long long	integer	8	(2), (3)
  Q	unsigned long long	integer	8	(2), (3)
  f	float	float	4	(4)
  d	double	float	8	(4)
  s	char[]	string	 	 
  p	char[]	string	 	 
  P	void *	integer	 	(5), (3)
*/
//FMT = '>cccBHBB'
typedef struct scpcmdhdr_structure
{
  char sync[3];
  unsigned char ctl;
  unsigned short dl;
  unsigned char id;
  unsigned char cks;
  unsigned char extra;

  unsigned char *data_bin;
  unsigned int data_length;
} scpcmdhdr_data;

void scpcmdhdr__init__(scpcmdhdr_data* scpcmdhdr_p,unsigned char *data,unsigned int length);
unsigned char scpcmdhdr_parseData(scpcmdhdr_data* scpcmdhdr_p,unsigned char *data,unsigned int length);
int scpcmdhdr__eq__(scpcmdhdr_data* scpcmdhdr_p,scpcmdhdr_data* other);
int scpcmdhdr__ne__(scpcmdhdr_data* scpcmdhdr_p,scpcmdhdr_data* other);
void scpcmdhdr__str__(scpcmdhdr_data *scpmdhdr_p, char* str);

#endif
