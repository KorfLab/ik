/******************************************************************************\
sequence.h
\******************************************************************************/

#ifndef IK_SEQUENCE_H
#define IK_SEQUENCE_H

#include <stdio.h>
#include <string.h>
#include "toolbox.h"

struct ik_fasta {
	int	   length;
	char * def;
	char * seq;
};
typedef struct ik_fasta * ik_fasta;

void	 ik_fasta_free (ik_fasta);
ik_fasta ik_fasta_new (const char *, const char *);
ik_fasta ik_fasta_read (FILE *);
void	 ik_fasta_write (FILE *, const ik_fasta);
void	 ik_fasta_set_line_length (int);

struct ik_dna {
	int	   length;
	char * def;
	char * seq;
	char * num;
};
typedef struct ik_dna * ik_dna;

struct ik_pro {
	int	   length;
	char * def;
	char * seq;
	char * num;
};
typedef struct ik_pro * ik_pro;

void ik_dna_free (ik_dna);
void ik_pro_free (ik_pro);

ik_dna ik_dna_new (const char *, const char *);
ik_dna ik_dna_anti (const char *, const ik_dna);
ik_pro ik_pro_new (const char *, const char *);


#endif
