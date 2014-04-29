#include <stdio.h>
#include "toolbox.h"
#include "sequence.h"
#include "align.h"

static int ITERATIONS = 1000;
void test_ivec (void);
void test_fvec (void);
void test_tvec (void);
void test_vec (void);
void test_imap (void);
void test_fmap (void);
void test_tmap (void);
void test_map (void);
void test_xtree (void);
void test_memory (void);
void test_dna (void);
void test_stuff (void);
void test_sw (void);

static char usage[] = "\
usage: ik-test [options]\n\
options:\n\
  -all\n\
  -ivec\n\
  -fvec\n\
  -tvec\n\
  -vec\n\
  -imap\n\
  -fmap\n\
  -tmap\n\
  -map\n\
  -xtree\n\
  -dna\n\
  -iterate <int> [1000]\n\
  -memory <float> [none, use gigabytes]\n\
  -stuff\n\
  -sw\n\
";

int main (int argc, char ** argv) {
	
	/* preamble */
	if (argc == 1) {
		fprintf(stderr, "%s", usage);
		exit(1);
	}
	
	/* options */
	ik_set_program_name(argv[0]);
	ik_register_option("-iterate", 1);
	ik_register_option("-ivec", 0);
	ik_register_option("-fvec", 0);
	ik_register_option("-tvec", 0);
	ik_register_option("-vec", 0);
	ik_register_option("-imap", 0);
	ik_register_option("-fmap", 0);
	ik_register_option("-tmap", 0);
	ik_register_option("-map", 0);
	ik_register_option("-xtree", 0);
	ik_register_option("-dna", 0);
	ik_register_option("-all", 0);
	ik_register_option("-memory", 1);
	ik_register_option("-stuff", 0);
	ik_register_option("-sw", 0);
	ik_parse_options(&argc, argv);
	
	/* control */
	if (ik_option("-stuff")) test_stuff();
	if (ik_option("-sw")) test_sw();
	if (ik_option("-iterate")) ITERATIONS = atoi(ik_option("-iterate"));
	if (ik_option("-ivec")) test_ivec();
	if (ik_option("-fvec")) test_fvec();
	if (ik_option("-tvec")) test_tvec();
	if (ik_option("-vec"))	test_vec();
	if (ik_option("-imap")) test_imap();
	if (ik_option("-fmap")) test_fmap();
	if (ik_option("-tmap")) test_tmap();
	if (ik_option("-map"))	test_map();
	if (ik_option("-xtree")) test_xtree();
	if (ik_option("-dna")) test_dna();
	if (ik_option("-all")) {
		test_ivec();
		test_fvec();
		test_tvec();
		test_imap();
		test_fmap();
		test_tmap();
		test_xtree();
		test_dna();
	}
	if (ik_option("-memory")) test_memory();

	return 0;
}

#define ILIMIT 10000

void test_stuff (void) {
	int array[ILIMIT][ILIMIT];
	int i, j, k;
	int max = 0;
	
	printf("test begin...");
	for (k = 0; k < 100; k++) {
		fprintf(stderr, "  %d\r", k);
		for (i = 0; i < ILIMIT; i++) {
			for (j = 0; j < ILIMIT; j++) {
				if (array[i][j] > max) max = array[i][j];
			}
		}
	}
	printf("done\n");
}

void test_dna (void) {
	int    i, j;
	ik_dna dna;
	ik_dna anti;
	
	for (i = 0; i < ITERATIONS; i++) {
		for (j = 0; j < 1000; j++) {
			dna = ik_dna_new(">def", "AAAAACCCCCGGGGGTTTTTAAAAACCCCCGGGGGTTTTT");
			anti = ik_dna_anti(">anti", dna);
			ik_dna_free(dna);
			ik_dna_free(anti);
		}
	}
}

void test_memory (void) {
	float gb = atof(ik_option("-memory"));
	float b = gb * 1024 * 1024 * 1024;
	size_t max, i, test;
	char * mem;
	
	max = b;
	
	fprintf(stderr, "checking %ld bytes\n", max);
	mem = ik_malloc(b);
	fprintf(stderr, "memory allocated\n");
	for (test = 0; test < 256; test += 127) {
		fprintf(stderr, "test %d\n", (char)test);
		for (i = 0; i < max; i++) mem[i] = (char)test;
		for (i = 0; i < max; i++) {
			if (mem[i] != test) {
				fprintf(stderr, "memory check failed\n");
				fprintf(stderr, "%d != %d at %ld\n", mem[i], (char)test, i);
				exit(1);
			}
		}
	}
}

void test_ivec (void) {
	int i, j;
	ik_ivec ivec;
	
	printf("ivec\n");
	for (i = 0; i < ITERATIONS; i++) {
		ivec = ik_ivec_new();
		for (j = 0; j < 4000; j++) {
			ik_ivec_push(ivec, j);
		}
		ik_ivec_free(ivec);
	}
}

void test_fvec (void) {
	int i, j;
	ik_fvec fvec;
	
	printf("fvec\n");
	for (i = 0; i < ITERATIONS; i++) {
		fvec = ik_fvec_new();
		for (j = 0; j < 3000; j++) {
			ik_fvec_push(fvec, j);
		}
		ik_fvec_free(fvec);
	}
}

void test_tvec (void) {
	int i, j;
	ik_tvec tvec;
	char text[64];
	
	printf("tvec\n");
	for (i = 0; i < ITERATIONS; i++) {
		tvec = ik_tvec_new();
		for (j = 0; j < 200; j++) {
			sprintf(text, "string %d", j);
			ik_tvec_push(tvec, text);
		}
		ik_tvec_free(tvec);
	}
}

void test_vec (void) {
	int i, j;
	ik_vec vec;
	char text[64];
	
	printf("vec\n");
	for (i = 0; i < ITERATIONS; i++) {
		vec = ik_vec_new();
		for (j = 0; j < 200; j++) {
			sprintf(text, "string %d", j);
			ik_vec_push(vec, text);
		}
		ik_vec_free(vec);
	}
}

void test_imap (void) {
	int i, j;
	ik_imap imap;
	char text[64];
	
	printf("imap\n");
	for (i = 0; i < ITERATIONS; i++) {
		imap = ik_imap_new();
		for (j = 0; j < 40; j++) {
			sprintf(text, "key %d", j);
			ik_imap_set(imap, text, j);
		}
		ik_imap_free(imap);
	}
}

void test_fmap (void) {
	int i, j;
	ik_fmap fmap;
	char text[64];
	
	printf("fmap\n");
	for (i = 0; i < ITERATIONS; i++) {
		fmap = ik_fmap_new();
		for (j = 0; j < 40; j++) {
			sprintf(text, "key %d", j);
			ik_fmap_set(fmap, text, j);
		}
		ik_fmap_free(fmap);
	}
}

void test_tmap (void) {
	int i, j;
	ik_tmap tmap;
	char text[64];
	
	printf("tmap\n");
	for (i = 0; i < ITERATIONS; i++) {
		tmap = ik_tmap_new();
		for (j = 0; j < 40; j++) {
			sprintf(text, "key %d", j);
			ik_tmap_set(tmap, text, text);
		}
		ik_tmap_free(tmap);
	}
}

void test_map (void) {
	int i, j;
	ik_map map;
	char text[64];
	
	printf("map\n");
	for (i = 0; i < ITERATIONS; i++) {
		map = ik_map_new();
		for (j = 0; j < 40; j++) {
			sprintf(text, "key %d", j);
			ik_map_set(map, text, text);
		}
		ik_map_free(map);
	}
}

void test_xtree (void) {
	int i, j;
	ik_xtree xtree;
	char text[64];
	
	printf("xtree\n");
	for (i = 0; i < ITERATIONS; i++) {
		xtree = ik_xtree_new();
		for (j = 0; j < 100; j++) {
			sprintf(text, "key %d", j);
			ik_xtree_set(xtree, text, text);
		}
		ik_xtree_free(xtree);
	}
}

void test_sw (void) {
	int i, j;
	char s1[10000], s2[10000];
	int limit = 1000;
	int r;
	double d1, d2;
	
	for (i = 0; i < 1000; i++) {
		for (j = 0; j < limit; j++) {
			r = 4 * (double)rand()/RAND_MAX;
			switch(r) {
				case 0: s1[j] = 'A'; break;
				case 1: s1[j] = 'C'; break;
				case 2: s1[j] = 'G'; break;
				case 3: s1[j] = 'T'; break;
			}
		}
		for (j = 0; j < limit; j++) {
			r = 4 * (double)rand()/RAND_MAX;
			switch(r) {
				case 0: s2[j] = 'A'; break;
				case 1: s2[j] = 'C'; break;
				case 2: s2[j] = 'G'; break;
				case 3: s2[j] = 'T'; break;
			}
		}
		s1[limit] = '\0';
		s2[limit] = '\0';
		d1 = sw_mat_linear(s1, s2, 0);
		d2 = sw_mat(s1, s2, 0);
		if (d1 != d2) ik_exit(1, "fail");
	}
}



