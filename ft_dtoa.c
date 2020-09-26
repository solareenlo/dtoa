#include "ft_printf.h"

/* static void	put_ui2bit(unsigned int val, size_t size) */
/* { */
/* 	size_t	i; */
/*  */
/* 	i = size; */
/* 	while (i--) */
/* 	{ */
/* 		if (i != (size - 1) && i % 4 == 3) */
/* 			printf(" "); */
/* 		printf("%d", (val >> i) & 1); */
/* 	} */
/* } */
/*  */
/* static void put_d2bit(double val) */
/* { */
/* 	t_d		d; */
/* 	size_t			size; */
/*  */
/* 	d.d = val; */
/* 	size = sizeof(unsigned int) * 8; */
/* 	put_ui2bit(d.L[1], size); */
/* 	printf(" "); */
/* 	put_ui2bit(d.L[0], size); */
/* 	printf("\n"); */
/* } */

char *ft_dtoa(double d0, int mode, int ndigits, int *decpt, int *sign, char **rve)
{
	int			be, bbits;
	int			b2, b5, dig, i, ieps, ilim, ilim0, ilim1, j, j1, k, k0, k_check, m2, s2, s5;
	int			L;
	t_bigint	*b, *S;
	t_d			d, d2, eps;
	double		ds;
	char		*s, *s0;
	const double bigtens[] = { 1e16, 1e32, 1e64, 1e128, 1e256 };
	const double tens[] = {
		1e0, 1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8, 1e9,
		1e10, 1e11, 1e12, 1e13, 1e14, 1e15, 1e16, 1e17, 1e18, 1e19,
		1e20, 1e21, 1e22
		};

	d.d = d0;
	if (WORD0(&d) & SIGN_BIT)
	{
		*sign = 1;
		WORD0(&d) &= ~SIGN_BIT;
	}
	else
		*sign = 0;
	if ((WORD0(&d) & EXP_MASK) == EXP_MASK)
	{
		*decpt = 9999;
		if (!WORD1(&d) && !(WORD0(&d) & 0xfffff))
			return ft_nrv_alloc("Infinity", rve, 8);
		return (ft_nrv_alloc("NaN", rve, 3));
	}
	if (!DVAL(&d))
	{
		*decpt = 1;
		return (ft_nrv_alloc("0", rve, 1));
	}
	b = ft_d2b(DVAL(&d), &be, &bbits);
	/* printf("be:%d\n", be); */
	i = (int)(WORD0(&d) >> EXP_SHIFT & (EXP_MASK >> EXP_SHIFT));
	DVAL(&d2) = DVAL(&d);
	WORD0(&d2) &= FRAC_MASK;
	WORD0(&d2) |= EXP;
	/* put_d2bit(DVAL(&d)); */
	/* printf("d1:%f\n", DVAL(&d)); */
	/* put_d2bit(DVAL(&d2)); */
	/* printf("d2:%f\n", DVAL(&d2)); */
	i -= BIAS;
	ds = (DVAL(&d2) - 1.5) * 0.289529654602168 + 0.1760912590558 + i * 0.301029995663981;
	k = (int)ds;
	/* printf("ds:%g k:%d\n", ds, k); */
	if (ds < 0. && ds != k)
		k--;
	k_check = 1;
	if (k >= 0 && k <= TEN_PMAX)
	{
		if (DVAL(&d) < tens[k])
			k--;
		k_check = 0;
	}
	/* printf("k:%d\n", k); */
	j = bbits - i - 1;
	/* printf("bbits:%d\n", bbits); */
	/* printf("i:%d\n", i); */
	/* printf("j:%d\n", j); */
	if (j >= 0)
	{
		b2 = 0;
		s2 = j;
	}
	else
	{
		b2 = -j;
		s2 = 0;
	}
	if (k >= 0)
	{
		b5 = 0;
		s5 = k;
		s2 += k;
	}
	else
	{
		b2 -= k;
		b5 = -k;
		s5 = 0;
	}
	ilim = ilim1 = -1;
	if (mode == 2)
	{
		if (ndigits <= 0)
			ndigits = 1;
		ilim = ilim1 = i = ndigits;
	}
	else if (mode == 3)
	{
		i = ndigits + k + 1;
		ilim = i;
		ilim1 = i - 1;
		if (i <= 0)
			i = 1;
	}
	/* printf("ndigits:%d\n", ndigits); */
	/* printf("ilim:%d\n", ilim); */
	/* printf("i:%d\n", i); */
	s = s0 = ft_rv_alloc(i);
	if (ilim >= 0 && ilim <= QUICK_MAX)
	{
		/* Try to get by with floating-point arithmetic. */
		i = 0;
		DVAL(&d2) = DVAL(&d);
		k0 = k;
		ilim0 = ilim;
		ieps = 2; /* conservative */
		if (k > 0)
		{
			ds = tens[k & 0xf];
			j = k >> 4;
			if (j & BLETCH)
			{
				/* prevent overflows */
				j &= BLETCH - 1;
				DVAL(&d) /= bigtens[n_bigtens - 1];
				ieps++;
			}
			while (j)
			{
				if (j & 1)
				{
					ieps++;
					ds *= bigtens[i];
				}
				j >>= 1;
				i++;
			}
			DVAL(&d) /= ds;
		}
		else if ((j1 = -k) != 0)
		{
			DVAL(&d) *= tens[j1 & 0xf];
			j = j1 >> 4;
			while (j)
			{
				if (j & 1)
				{
					ieps++;
					DVAL(&d) *= bigtens[i];
				}
				j >>= 1;
				i++;
			}
		}
		/* printf("DVAL(&d):%f\n", DVAL(&d)); */
		if (k_check && DVAL(&d) < 1. && ilim > 0)
		{
			if (ilim1 <= 0)
				goto fast_failed;
			ilim = ilim1;
			k--;
			DVAL(&d) *= 10.;
			ieps++;
		}
		DVAL(&eps) = ieps * DVAL(&d) + 7.;
		WORD0(&eps) -= (P - 1) * EXP_MSK;
		if (ilim == 0)
		{
			S  = 0;
			DVAL(&d) -= 5.;
			if (DVAL(&d) > DVAL(&eps))
				goto one_digit;
			if (DVAL(&d) < -DVAL(&eps))
				goto no_digits;
			goto fast_failed;
		}
		/* Generate ilim digits, then fix them up. */
		DVAL(&eps) *= tens[ilim - 1];
		i = 1;
		while (1)
		{
			L = (int)(DVAL(&d));
			if (!(DVAL(&d) -= L))
				ilim = i;
			*s++ = '0' + (int)L;
			if (i == ilim)
			{
				if (DVAL(&d) > 0.5 + DVAL(&eps))
					goto bump_up;
				else if (DVAL(&d) < 0.5 - DVAL(&eps))
				{
					while(*--s == '0')
						;
					s++;
					goto ret1;
				}
				break;
			}
			i++;
			DVAL(&d) *= 10.;
		}
 fast_failed:
		s = s0;
		DVAL(&d) = DVAL(&d2);
		k = k0;
		ilim = ilim0;
	}
	/* Do we have a "small" integer? */
	if (be >= 0 && k <= INTMAX)
	{
		/* Yes. */
		ds = tens[k];
		if (ndigits < 0 && ilim <= 0)
		{
			S = 0;
			if (ilim < 0 || DVAL(&d) <= 5 * ds)
				goto no_digits;
			goto one_digit;
		}
		i = 1;
		while (1)
		{
			L = (int)(DVAL(&d) / ds);
			DVAL(&d) -= L * ds;
			*s++ = '0' + (int)L;
			if (!DVAL(&d))
				break;
			if (i == ilim)
			{
				DVAL(&d) += DVAL(&d);
				if (DVAL(&d) > ds || (DVAL(&d) == ds && L & 1))
				{
 bump_up:
					while(*--s == '9')
						if (s == s0)
						{
							k++;
							*s = '0';
							break;
						}
					++*s++;
				}
				break;
			}
			i++;
			DVAL(&d) *= 10.;
		}
		goto ret1;
	}
	m2 = b2;
	if (m2 > 0 && s2 > 0)
	{
		i = m2 < s2 ? m2 : s2;
		b2 -= i;
		m2 -= i;
		s2 -= i;
	}
	if (b5 > 0)
		b = ft_pow5mult(b, b5);
	S = ft_i2b(1);
	if (s5 > 0)
		S = ft_pow5mult(S, s5);
	/* Check for special case that d is a normalized power of 2. */
	/* spec_case = 0; */
	/* Arrange for convenient computation of quotients:
	 * shift left if necessary so divisor has 4 leading 0 bits.
	 *
	 * Perhaps we should just compute leading 28 bits of S once
	 * and for all and pass them and a shift to quorem, so it
	 * can do shifts and ors to compute the numerator for q.
	 */
	if ((i = ((s5 ? 32 - ft_hi0bits(S->x[S->wds - 1]) : 1) + s2) & 0x1f) != 0)
		i = 32 - i;
	if (i > 4)
	{
		i -= 4;
		b2 += i;
		m2 += i;
		s2 += i;
	}
	else if (i < 4)
	{
		i += 28;
		b2 += i;
		m2 += i;
		s2 += i;
	}
	if (b2 > 0)
		b = ft_lshift(b, b2);
	if (s2 > 0)
		S = ft_lshift(S, s2);
	if (k_check)
	{
		if (ft_cmp(b, S) < 0)
		{
			k--;
			b = ft_multadd(b, 10, 0);	/* we botched the k estimate */
			ilim = ilim1;
		}
	}
	if (ilim <= 0 && mode == 3)
	{
		if (ilim < 0 || ft_cmp(b, S = ft_multadd(S, 5, 0)) <= 0)
		{
			/* no digits, fcvt style */
 no_digits:
			k = -1 - ndigits;
			goto ret;
		}
 one_digit:
		*s++ = '1';
		k++;
		goto ret;
	}
	i = 1;
	while (1)
	{
		*s++ = dig = ft_quorem(b, S) + '0';
		if (!b->x[0] && b->wds <= 1)
			goto ret;
		if (i >= ilim)
			break;
		b = ft_multadd(b, 10, 0);
		i++;
	}
	b = ft_lshift(b, 1);
	j = ft_cmp(b, S);
	if (j > 0 || (j == 0 && dig & 1))
	{
		while(*--s == '9')
			if (s == s0)
			{
				k++;
				*s++ = '1';
				goto ret;
			}
		++*s++;
	}
	else
	{
		while(*--s == '0')
			;
		s++;
	}
 ret:
	ft_bfree(S);
 ret1:
	ft_bfree(b);
	*s = 0;
	*decpt = k + 1;
	if (rve)
		*rve = s;
	return s0;
}
