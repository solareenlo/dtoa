#ifndef GDTOAIMP_H_INCLUDED
#define GDTOAIMP_H_INCLUDED

#include <stddef.h> /* for size_t */

#include "limits.h"
#include "stdlib.h"
#include "string.h"

#include "errno.h"
#include "float.h"

#define n_bigtens 5

typedef union	u_d {
	double			d;
	unsigned int	L[2];
}				t_u;

#define word0(x) (x)->L[1]
#define word1(x) (x)->L[0]
#define dval(x) (x)->d

/*
** Sign_bit : 10000000000000000000000000000000
** Exp_mask : 01111111111100000000000000000000
** Exp      : 00111111111100000000000000000000
** Exp_msk  : 00000000000100000000000000000000
** Frac_mask: 00000000000011111111111111111111
** Bletch   : 10000
*/

#define Sign_bit	0x80000000
#define Exp_mask	0x7ff00000
#define Exp			0x3ff00000
#define Exp_msk		0x100000
#define Exp_shift	20
#define Frac_mask	0xfffff
#define Bias		1023
#define Ten_pmax	22
#define P			53
#define Bletch		0x10
#define Quick_max	14
#define Int_max		14

typedef struct	s_bigint {
	struct			s_bigint *next;
	int				k, maxwds, sign, wds;
	unsigned int	x[1];
}				t_bigint;

/* Protect gdtoa-internal symbols */
#define	nrv_alloc	__nrv_alloc_D2A
 extern char *nrv_alloc(char*, char **, int);
#define	Balloc		__Balloc_D2A
 extern t_bigint *Balloc(int);
#define	Bfree		__Bfree_D2A
 extern void Bfree (t_bigint*);
#define	bigtens		__bigtens_D2A
 extern const double bigtens[];
#define	cmp		__cmp_D2A
 extern int cmp (t_bigint*, t_bigint*);
#define	d2b		__d2b_D2A
 extern t_bigint *d2b (double, int*, int*);
#define	diff		__diff_D2A
 extern t_bigint *diff (t_bigint*, t_bigint*);
#define	dtoa_result	__dtoa_result_D2A
 extern char *dtoa_result;
#define	hi0bits		__hi0bits_D2A
 extern int hi0bits (unsigned int);
#define	i2b		__i2b_D2A
 extern t_bigint *i2b (int);
#define	lshift		__lshift_D2A
 extern t_bigint *lshift (t_bigint*, int);
#define	mult		__mult_D2A
 extern t_bigint *mult (t_bigint*, t_bigint*);
#define	multadd		__multadd_D2A
 extern t_bigint *multadd (t_bigint*, int, int);
#define	pow5mult	__pow5mult_D2A
 extern t_bigint *pow5mult (t_bigint*, int);
#define	quorem		__quorem_D2A
 extern int quorem (t_bigint*, t_bigint*);
#define	rv_alloc	__rv_alloc_D2A
 extern char *rv_alloc (int);
#define	tens		__tens_D2A
 extern const double tens[];

#endif /* GDTOAIMP_H_INCLUDED */
