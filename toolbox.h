/******************************************************************************\
toolbox.h

Copyright (C) 2003-2006 Ian Korf

\******************************************************************************/

#ifndef IK_TOOLBOX_H
#define IK_TOOLBOX_H

#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* numeric constats */
extern const int ik_POWER[6][8];
extern const float MIN_SCORE;
extern const float MAX_SCORE;

/* library and program info */
char * ik_get_version_number (void);
void   ik_set_program_name (const char *);
char * ik_get_program_name (void);

/* specialized output */
void ik_exit (int, const char *, ...);
void ik_output (FILE *, int, ...);

/* memory */
void * ik_malloc (size_t);
void * ik_calloc (size_t, size_t);
void * ik_realloc (void *, size_t);
void   ik_free (void *);

/* integer vector */
struct _ik_ivec	{
	int * elem;
	int	  size;
	int	  limit;
	int	  last;
};
typedef struct _ik_ivec * ik_ivec;
void	ik_ivec_free (ik_ivec);
ik_ivec ik_ivec_new (void);
void	ik_ivec_push (ik_ivec, int);

/* float vector */
struct _ik_fvec	{
	float * elem;
	int		size;
	int		limit;
	int		last;
};
typedef struct _ik_fvec * ik_fvec;
void	ik_fvec_free (ik_fvec);
ik_fvec ik_fvec_new (void);
void	ik_fvec_push (ik_fvec, float);

/* text vector */
struct _ik_tvec {
	char ** elem;
	int		size;
	int		limit;
	char  * last;
};
typedef struct _ik_tvec * ik_tvec;
void	ik_tvec_free (ik_tvec);
ik_tvec ik_tvec_new (void);
void	ik_tvec_push (ik_tvec, const char *);

/* generic void * vector */
struct _ik_vec {
	void ** elem;
	int		size;
	int		limit;
	void  * last;
};
typedef struct _ik_vec * ik_vec;
void   ik_vec_free (ik_vec);
ik_vec ik_vec_new (void);
void   ik_vec_push (ik_vec, void *);

/* generic map (text key, void * value) */
struct _ik_map  {
	int		 level;
	int		 slots;
	ik_tvec	 keys;
	ik_vec	 vals;
	ik_vec * key;
	ik_vec * val;
};
typedef struct _ik_map * ik_map;
void	ik_map_free (ik_map);
ik_map	ik_map_new (void);
void	ik_map_set (ik_map, const char *, void *);
void *	ik_map_get (const ik_map, const char *);
ik_tvec ik_map_keys (const ik_map);
ik_vec	ik_map_vals (const ik_map);
void	ik_map_stat (const ik_map);

/* integer map */
struct _ik_imap {
	ik_map	hash;
	ik_ivec ivec;
};
typedef struct _ik_imap * ik_imap;
void	ik_imap_free (ik_imap);
ik_imap ik_imap_new (void);
void	ik_imap_set (ik_imap, const char *, int);
int		ik_imap_get (const ik_imap, const char *);
ik_tvec ik_imap_keys (const ik_imap);

/* float map */
struct _ik_fmap {
	ik_map hash;
	ik_fvec fvec;
};
typedef struct _ik_fmap * ik_fmap;
void	ik_fmap_free (ik_fmap);
ik_fmap ik_fmap_new (void);
void	ik_fmap_set (ik_fmap, const char *, float);
float	ik_fmap_get (ik_fmap, const char *);
ik_tvec ik_fmap_keys (const ik_fmap);

/* text map */
struct _ik_tmap {
	ik_map hash;
	ik_tvec tvec;
};
typedef struct _ik_tmap * ik_tmap;
void	ik_tmap_free (ik_tmap);
ik_tmap ik_tmap_new (void);
void	ik_tmap_set (ik_tmap, const char *, const char *);
char *	ik_tmap_get (ik_tmap, const char *);
ik_tvec ik_tmap_keys (const ik_tmap);

/* generic suffix tree */
struct _ik_xnode {
	ik_vec children;
	void  * data;
	char	c;
};
typedef struct _ik_xnode * ik_xnode; /* note: all methods private */
ik_xnode ik_xnode_new (char);
ik_xnode ik_xnode_search (const ik_xnode, char c);

struct _ik_xtree {
	ik_xnode head;
	ik_vec	 alloc;
};
typedef struct _ik_xtree * ik_xtree;
void	 ik_xtree_free (ik_xtree);
ik_xtree ik_xtree_new (void);
void *	 ik_xtree_get (const ik_xtree, const char *);
int		 ik_xtree_check (const ik_xtree, const char *);
ik_xnode ik_xtree_node (const ik_xtree, const char *);
void	 ik_xtree_set (ik_xtree, const char *, void *);
ik_tvec	 ik_xtree_keys (const ik_xtree);

/* command line processing */
void   ik_register_option (const char *, int);
void   ik_parse_options (int *, char **);
char * ik_option(const char *);

/* pipe */
struct _ik_pipe {
	int	   mode; /* 0 = read, 1 = write, 2 = r+ */
	char * name;
	int	   gzip;
	FILE * stream;
};
typedef struct _ik_pipe * ik_pipe;
void	ik_pipe_free (ik_pipe);
ik_pipe ik_pipe_open (const char *, const char *);
void	ik_pipe_close (ik_pipe);

#endif
