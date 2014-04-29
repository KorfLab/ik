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

#include "toolbox.h"



void   set_matrix (int);
double sw_mmg (const char*, const char*, int, int, int);
double sw_mmg_linear (const char*, const char*, int, int, int);
double sw_mat (const char*, const char*, int);
double sw_mat_linear (const char*, const char*, int);

#endif