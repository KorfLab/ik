/******************************************************************************\
sequence.c

Copyright (C) 2003-2006 Ian Korf

\******************************************************************************/

#ifndef IK_SEQUENCE_C
#define IK_SEQUENCE_C

#include "sequence.h"


void ik_fasta_free (ik_fasta ff) {
	if (ff == NULL) return;
	if (ff->def) {
		ik_free(ff->def);
		ff->def = NULL;
	}
	if (ff->seq) {
		ik_free(ff->seq);
		ff->seq = NULL;
	}
	ik_free(ff);
	ff = NULL;
}

ik_fasta ik_fasta_new (const char * def, const char * seq) {
	ik_fasta ff = ik_malloc(sizeof(struct ik_fasta));
	ff->def	   = ik_malloc(strlen(def) +1);
	ff->seq	   = ik_malloc(strlen(seq) +1);
	ff->length = strlen(seq);
	strcpy(ff->def, def);
	strcpy(ff->seq, seq);
	return ff;
}

ik_fasta ik_fasta_read (FILE * stream) {
	char	 c;
	int		 i;
	size_t	 size;
	char   * seq;
	char   * def;
	ik_fasta ff;
	
	c = fgetc(stream);
	if (c == EOF) return NULL;
	if (c != '>') ik_exit(1, "ik_fasta: doesn't look like a fasta file");
	ungetc(c, stream);
	
	/* def */
	size = 256;
	i = 0;
	def = ik_malloc(size * sizeof(char));
	while ((c = fgetc(stream)) != EOF) {
		if (c == '\n') break;
		def[i] = c;
		i++;
		if (i == size) {
			size *= 2;
			def = ik_realloc(def, size);
		}
	}
	def[i] = '\0';
	
	/* seq */
	size = 65536;
	i = 0;
	seq = ik_malloc(size * sizeof(char));
	
	while ((c = fgetc(stream)) != EOF) {
		if (c == '>') {
			ungetc(c, stream);
			break;
		}
		if (isspace((int)c)) continue;
		seq[i] = c;
		i++;
		if (i == size) {
			size *= 2;
			seq = ik_realloc(seq, size);
		}
	}
	seq[i] = '\0';
	
	ff = ik_fasta_new(def+1, seq);
	ik_free(def);
	ik_free(seq);
	return ff;
}

static int FASTA_LINE_LENGTH = 50;

void ik_fasta_write (FILE * stream, const ik_fasta ff) {
	int i;
	
	if (ff->def[0] != '>') fprintf(stream, ">");
	fprintf(stream, "%s", ff->def);
	if (ff->def[strlen(ff->def) -1] != '\n') fprintf(stream, "\n");
	
	for (i = 0; i < ff->length; i++) {
		fputc(ff->seq[i], stream);
		if ((i+1) % FASTA_LINE_LENGTH == 0) fprintf(stream, "\n");
	}
	
	fprintf(stream, "\n");
}

void ik_fasta_set_line_length (int length) {
	FASTA_LINE_LENGTH = length;
}


/* dna alphabet
0	A
1	C
2	G
3	T
4	N	[ACGT]
5	R	[AG]
6	Y	[CT]
7	W	[AT]
8	S	[CG]
9	K	[GT]
10	M	[AC]
11	B	[CGT]
12	D	[AGT]
13	H	[ACT]
14	V	[ACG]
*/

void ik_dna_free (ik_dna dna) {
	ik_free(dna->def); dna->def = NULL;
	ik_free(dna->seq); dna->seq = NULL;
	ik_free(dna->num); dna->num = NULL;
	ik_free(dna);
}

void ik_pro_free (ik_pro pro) {
	ik_free(pro->def); pro->def = NULL;
	ik_free(pro->seq); pro->seq = NULL;
	ik_free(pro->num); pro->num = NULL;
	ik_free(pro);
}

ik_dna ik_dna_new (const char * def, const char * seq) {
	int    i, illegal[256];
	ik_dna dna;
	
	dna = ik_malloc(sizeof(struct ik_dna));
	
	for (i = 0; i < 256; i++) illegal[i] = 0;
	
	dna->def = ik_malloc(strlen(def)+1);
	dna->def = strcpy(dna->def, def);
	
	dna->length = strlen(seq);
	dna->seq = ik_malloc(dna->length +1);
	dna->num = ik_malloc(dna->length +1);
	dna->seq = strcpy(dna->seq, seq);
	
	/* check the alphabet */
	for (i = 0; i < dna->length; i++) {
		switch (seq[i]) {
			case 'A': case 'a': dna->num[i] =  0; break;
			case 'C': case 'c': dna->num[i] =  1; break;
			case 'G': case 'g': dna->num[i] =  2; break;
			case 'T': case 't': dna->num[i] =  3; break;
			case 'N': case 'n': 
			case 'R': case 'r':
			case 'Y': case 'y':
			case 'W': case 'w':
			case 'S': case 's':
			case 'K': case 'k':
			case 'M': case 'm':
			case 'B': case 'b':
			case 'D': case 'd':
			case 'H': case 'h':
			case 'V': case 'v': dna->num[i] = 4; break;
			default:
				dna->num[i] = 4;
				illegal[(int)dna->seq[i]]++;
		}
	}
	
	for (i = 0; i < 256; i++) {
		if (illegal[i]) {
			fprintf(stderr, "illegal DNA symbol (%c) used %d times\n",
				i, illegal[i]);
		}
	}
	
	return dna;
}

ik_dna ik_dna_anti (const char *def, const ik_dna dna) {
	int i;
	char *str = ik_malloc(dna->length +1);
	ik_dna anti;
	
	str[dna->length] = '\0';
	
	for (i = 1; i <= dna->length; i++) {
		switch (dna->seq[i-1]) {
			case 'A': str[dna->length -i] = 'T'; break;
			case 'a': str[dna->length -i] = 't'; break;
			case 'C': str[dna->length -i] = 'G'; break;
			case 'c': str[dna->length -i] = 'g'; break;
			case 'G': str[dna->length -i] = 'C'; break;
			case 'g': str[dna->length -i] = 'c'; break;
			case 'T': str[dna->length -i] = 'A'; break;
			case 't': str[dna->length -i] = 'a'; break;
			case 'N': str[dna->length -i] = 'N'; break;
			case 'n': str[dna->length -i] = 'n'; break;
			case 'R': str[dna->length -i] = 'Y'; break;
			case 'r': str[dna->length -i] = 'y'; break;
			case 'Y': str[dna->length -i] = 'R'; break;
			case 'y': str[dna->length -i] = 'r'; break;
			case 'W': str[dna->length -i] = 'S'; break;
			case 'w': str[dna->length -i] = 's'; break;
			case 'S': str[dna->length -i] = 'W'; break;
			case 's': str[dna->length -i] = 'w'; break;
			case 'K': str[dna->length -i] = 'M'; break;
			case 'k': str[dna->length -i] = 'm'; break;
			case 'M': str[dna->length -i] = 'K'; break;
			case 'm': str[dna->length -i] = 'k'; break;
			case 'B': str[dna->length -i] = 'V'; break;
			case 'b': str[dna->length -i] = 'v'; break;
			case 'D': str[dna->length -i] = 'H'; break;
			case 'd': str[dna->length -i] = 'h'; break;
			case 'H': str[dna->length -i] = 'D'; break;
			case 'h': str[dna->length -i] = 'd'; break;
			case 'V': str[dna->length -i] = 'B'; break;
			case 'v': str[dna->length -i] = 'b'; break;
			default:  str[dna->length -i] = 'N'; 
		}
	}

	anti = ik_dna_new(def, str);
	free(str);
	return anti;
}

ik_pro ik_pro_new (const char * def, const char * seq) {
	ik_pro pro = ik_malloc(sizeof(struct ik_pro));
	
	/* completely unfinished */
	
	return pro;
}

#endif
