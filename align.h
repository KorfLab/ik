/******************************************************************************\
align.h

Copyright (C) 2003-2006 Ian Korf

\******************************************************************************/

#ifndef IK_ALIGN_H
#define IK_ALIGN_H

#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void   set_matrix (int);

struct _ik_align {
	double score;
	int    s1_beg;
	int    s1_end;
	int    s2_beg;
	int    s2_end;
	char * s1_seq;
	char * s2_seq;
	char * alignment;
};
typedef struct _ik_align * ik_align;
void ik_align_free (ik_align);

ik_align sw_mmg (const char*, const char*, int, int, int);
ik_align sw_mat (const char*, const char*, int);
double   sw_mmg_linear (const char*, const char*, int, int, int);
double   sw_mat_linear (const char*, const char*, int);

#endif