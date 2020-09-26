#ifndef GDTOAIMP_H_INCLUDED
# define GDTOAIMP_H_INCLUDED

# include <stddef.h>

# include "limits.h"
# include "stdlib.h"

# define n_bigtens 5

typedef union	u_d {
	double			d;
	unsigned int	L[2];
}				t_d;

# define WORD0(x) (x)->L[1]
# define WORD1(x) (x)->L[0]
# define DVAL(x) (x)->d

/*
** dtoa for IEEE arithmetic (dmg): convert double to ASCII string.
**
** Inspired by "How to Print Floating-Point Numbers Accurately" by
** Guy L. Steele, Jr. and Jon L. White [Proc. ACM SIGPLAN '90, pp. 112-126].
**
** Modifications:
**	1. Rather than iterating, we use a simple numeric overestimate
**	   to determine k = floor(log10(d)).  We scale relevant
**	   quantities using O(log2(k)) rather than O(k) multiplications.
**	2. For some modes > 2 (corresponding to ecvt and fcvt), we don't
**	   try to generate digits strictly left to right.  Instead, we
**	   compute with fewer bits and propagate the carry if necessary
**	   when rounding the final digit up.  This is often faster.
**	3. Under the assumption that input will be rounded nearest,
**	   mode 0 renders 1e23 as 1e23 rather than 9.999999999999999e22.
**	   That is, we allow equality in stopping tests when the
**	   round-nearest rule will give the same floating-point value
**	   as would satisfaction of the stopping test with strict
**	   inequality.
**	4. We remove common factors of powers of 2 from relevant
**	   quantities.
**	5. When converting floating-point integers less than 1e16,
**	   we use floating-point arithmetic rather than resorting
**	   to multiple-precision integers.
**	6. When asked to produce fewer than 15 digits, we first try
**	   to get by with floating-point arithmetic; we resort to
**	   multiple-precision integer arithmetic only if we cannot
**	   guarantee that the floating-point calculation has given
**	   the correctly rounded result.  For k requested digits and
**	   "uniformly" distributed input, the probability is
**	   something like 10^(k-15) that we must resort to the int
**	   calculation.
*/

/*
** Arguments ndigits, decpt, sign are similar to those
** of ecvt and fcvt; trailing zeros are suppressed from
** the returned string.  If not null, *rve is set to point
** to the end of the return value.  If d is +-Infinity or NaN,
** then *decpt is set to 9999.
** mode:
** 2 ==> max(1,ndigits) significant digits.  This gives a
**		return value similar to that of ecvt, except
**		that trailing zeros are suppressed.
**	3 ==> through ndigits past the decimal point.  This
**		gives a return value similar to that from fcvt,
**		except that trailing zeros are suppressed, and
**		ndigits can be negative.
** Values of mode other than 0-9 are treated as mode 0.
** Sufficient space is allocated to the return value
** to hold the suppressed trailing zeros.
*/

/*
** set sign for everything, including 0's and NaNs
*/

/*
** x: 1.??? となるように d の仮数部だけを取り出している．
** log(x)	~=~ log(1.5) + (x-1.5)/1.5
** log10(x)	 =  log(x) / log(10)
**		~=~ log(1.5)/log(10) + (x-1.5)/(1.5*log(10))
** log10(&d) = (i-Bias)*log(2)/log(10) + log10(&d2)
**
** This suggests computing an approximation k to log10(&d) by
**
** k = (i - Bias)*0.301029995663981
**	+ ( (d2-1.5)*0.289529654602168 + 0.176091259055681 );
**
** We want k to be too large rather than too small.
** The error in the first-order Taylor series approximation
** is in our favor, so we just round up the constant enough
** to compensate for any error in the multiplication of
** (i - Bias) by 0.301029995663981; since |i - Bias| <= 1077,
** and 1077 * 0.30103 * 2^-52 ~=~ 7.2e-14,
** adding 1e-13 to the constant term more than suffices.
** Hence we adjust the constant term to 0.1760912590558.
** (We could get a more accurate k by invoking log10,
** but this is probably not worthwhile.)
**
** ndigits		: presicion
** b			: 入力値の bit データ(構造体)
** be;			: log2(指数部) - 仮数部の bit が立っているまでの個数
** bbits		: 仮数部の左から bit が立っているまでの個数 + 1
** d2			: d の指数部分が 0 の時の値 (仮数部の値)
** i			: 指数部分の値(マイナスも含む)
** ds			: 入力値の log10 の値 + log 計算の誤差を補う定数
** k			: ds 以下で最大の整数
** j			: bbits - i - 1
** mode == 2	: %e, %g
** mode == 3	: %f
** ilim 		: 左側の 0 以外の表示する数の個数
** 				: 表示する数までちょうど届かないときは0
** 				: 表示する数まで届かないときはマイナスの値になる
** ft_rv_alloc(i): 10進数にした時の文字数文だけ malloc で確保
** s0			: retrun value の最初の文字へのポインタ
** s			: return value の最後の文字へのポインタ
*/

/*
** SIGN_BIT : 1000 0000 0000 0000 0000 0000 0000 0000
** EXP_MASK : 0111 1111 1111 0000 0000 0000 0000 0000
** EXP      : 0011 1111 1111 0000 0000 0000 0000 0000
** 1023		: 0011 1111 1111 0000 0000 0000 0000 0000
** EXP_MSK  : 0000 0000 0001 0000 0000 0000 0000 0000
** FRAC_MASK: 0000 0000 0000 1111 1111 1111 1111 1111
** BLETCH   : 10000
** 0xf		: 1111
*/

# define SIGN_BIT		0x80000000
# define EXP_MASK		0x7ff00000
# define EXP			0x3ff00000
# define EXP_MSK		0x100000
# define EXP_SHIFT		20
# define FRAC_MASK		0xfffff
# define BIAS			1023
# define TEN_PMAX		22
# define P				53
# define BLETCH			0x10
# define QUICK_MAX		14
# define INTMAX			14
# define KMAX			9
# define PRIVATE_MEM	2304
# define PRIVATE_mem	((PRIVATE_MEM + sizeof(double) - 1) / sizeof(double))
# define KSHIFT			5
# define KMASK			0x1f

typedef struct	s_bigint {
	struct s_bigint	*next;
	int				k;
	int				maxwds;
	int				sign;
	int				wds;
	unsigned int	x[1];
}				t_bigint;

static char		*dtoa_result;
static t_bigint	*freelist[KMAX + 1];

char		*ft_rv_alloc(int i);
char		*ft_nrv_alloc(char *s, char **rve, int n);
t_bigint	*ft_balloc(int k);
void 		ft_bfree(t_bigint *v);
int 		ft_cmp(t_bigint *a, t_bigint *b);
t_bigint 	*ft_d2b(double dd, int *e, int *bits);
void		ft_freedtoa(char *s);
int			ft_lo0bits(unsigned int *y);
int			ft_hi0bits(unsigned int x);
t_bigint	*ft_i2b(int i);
t_bigint	*ft_lshift(t_bigint *b, int k);
t_bigint	*ft_multadd(t_bigint *b, int m, int a);
t_bigint	*ft_mult(t_bigint *a, t_bigint *b);
t_bigint	*ft_pow5mult(t_bigint *b, int k);
int			ft_quorem(t_bigint *b, t_bigint *S);

#endif
