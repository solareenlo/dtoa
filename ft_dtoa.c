#include "gdtoaimp.h"

/* dtoa for IEEE arithmetic (dmg): convert double to ASCII string.
 *
 * Inspired by "How to Print Floating-Point Numbers Accurately" by
 * Guy L. Steele, Jr. and Jon L. White [Proc. ACM SIGPLAN '90, pp. 112-126].
 *
 * Modifications:
 *	1. Rather than iterating, we use a simple numeric overestimate
 *	   to determine k = floor(log10(d)).  We scale relevant
 *	   quantities using O(log2(k)) rather than O(k) multiplications.
 *	2. For some modes > 2 (corresponding to ecvt and fcvt), we don't
 *	   try to generate digits strictly left to right.  Instead, we
 *	   compute with fewer bits and propagate the carry if necessary
 *	   when rounding the final digit up.  This is often faster.
 *	3. Under the assumption that input will be rounded nearest,
 *	   mode 0 renders 1e23 as 1e23 rather than 9.999999999999999e22.
 *	   That is, we allow equality in stopping tests when the
 *	   round-nearest rule will give the same floating-point value
 *	   as would satisfaction of the stopping test with strict
 *	   inequality.
 *	4. We remove common factors of powers of 2 from relevant
 *	   quantities.
 *	5. When converting floating-point integers less than 1e16,
 *	   we use floating-point arithmetic rather than resorting
 *	   to multiple-precision integers.
 *	6. When asked to produce fewer than 15 digits, we first try
 *	   to get by with floating-point arithmetic; we resort to
 *	   multiple-precision integer arithmetic only if we cannot
 *	   guarantee that the floating-point calculation has given
 *	   the correctly rounded result.  For k requested digits and
 *	   "uniformly" distributed input, the probability is
 *	   something like 10^(k-15) that we must resort to the int
 *	   calculation.
 */

char *ft_dtoa(double d0, int mode, int ndigits, int *decpt, int *sign, char **rve)
{
 /*	Arguments ndigits, decpt, sign are similar to those
	of ecvt and fcvt; trailing zeros are suppressed from
	the returned string.  If not null, *rve is set to point
	to the end of the return value.  If d is +-Infinity or NaN,
	then *decpt is set to 9999.

	mode:
		2 ==> max(1,ndigits) significant digits.  This gives a
			return value similar to that of ecvt, except
			that trailing zeros are suppressed.
		3 ==> through ndigits past the decimal point.  This
			gives a return value similar to that from fcvt,
			except that trailing zeros are suppressed, and
			ndigits can be negative.

		Values of mode other than 0-9 are treated as mode 0.

		Sufficient space is allocated to the return value
		to hold the suppressed trailing zeros.
	*/

	int			be, bbits;
	int			b2, b5, dig, i, ieps, ilim, ilim0, ilim1, j, j1, k, k0, k_check, m2, s2, s5;
	int			L;
	t_bigint	*b, *S;
	t_u			d, d2, eps;
	double		ds;
	char		*s, *s0;

	d.d = d0;
	if (word0(&d) & Sign_bit)
	{
		/* set sign for everything, including 0's and NaNs */
		*sign = 1;
		word0(&d) &= ~Sign_bit;	/* clear sign bit */
	}
	else
		*sign = 0;

	if ((word0(&d) & Exp_mask) == Exp_mask)
	{
		/* Infinity or NaN */
		*decpt = 9999;
		if (!word1(&d) && !(word0(&d) & 0xfffff))
			return __nrv_alloc_D2A("Infinity", rve, 8);
		return nrv_alloc("NaN", rve, 3);
	}
	if (!dval(&d))
	{
		*decpt = 1;
		return nrv_alloc("0", rve, 1);
	}

	b = d2b(dval(&d), &be, &bbits);
	i = (int)(word0(&d) >> Exp_shift & (Exp_mask>>Exp_shift));
		dval(&d2) = dval(&d);
		word0(&d2) &= Frac_mask;
		word0(&d2) |= Exp;

		/* log(x)	~=~ log(1.5) + (x-1.5)/1.5
		 * log10(x)	 =  log(x) / log(10)
		 *		~=~ log(1.5)/log(10) + (x-1.5)/(1.5*log(10))
		 * log10(&d) = (i-Bias)*log(2)/log(10) + log10(&d2)
		 *
		 * This suggests computing an approximation k to log10(&d) by
		 *
		 * k = (i - Bias)*0.301029995663981
		 *	+ ( (d2-1.5)*0.289529654602168 + 0.176091259055681 );
		 *
		 * We want k to be too large rather than too small.
		 * The error in the first-order Taylor series approximation
		 * is in our favor, so we just round up the constant enough
		 * to compensate for any error in the multiplication of
		 * (i - Bias) by 0.301029995663981; since |i - Bias| <= 1077,
		 * and 1077 * 0.30103 * 2^-52 ~=~ 7.2e-14,
		 * adding 1e-13 to the constant term more than suffices.
		 * Hence we adjust the constant term to 0.1760912590558.
		 * (We could get a more accurate k by invoking log10,
		 *  but this is probably not worthwhile.)
		 */

		i -= Bias;
	ds = (dval(&d2)-1.5)*0.289529654602168 + 0.1760912590558 + i*0.301029995663981;
	k = (int)ds;
	if (ds < 0. && ds != k)
		k--;	/* want k = floor(ds) */
	k_check = 1;
	if (k >= 0 && k <= Ten_pmax)
	{
		if (dval(&d) < tens[k])
			k--;
		k_check = 0;
	}
	j = bbits - i - 1;
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

	ilim = ilim1 = -1;	/* Values for cases 0 and 1; done here to */
				/* silence erroneous "gcc -Wall" warning. */
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
	s = s0 = rv_alloc(i);

	if (ilim >= 0 && ilim <= Quick_max)
	{
		/* Try to get by with floating-point arithmetic. */
		i = 0;
		dval(&d2) = dval(&d);
		k0 = k;
		ilim0 = ilim;
		ieps = 2; /* conservative */
		if (k > 0)
		{
			ds = tens[k & 0xf];
			j = k >> 4;
			if (j & Bletch)
			{
				/* prevent overflows */
				j &= Bletch - 1;
				dval(&d) /= bigtens[n_bigtens - 1];
				ieps++;
			}
			for(; j; j >>= 1, i++)
				if (j & 1)
				{
					ieps++;
					ds *= bigtens[i];
				}
			dval(&d) /= ds;
		}
		else if (( j1 = -k )!=0)
		{
			dval(&d) *= tens[j1 & 0xf];
			for(j = j1 >> 4; j; j >>= 1, i++)
				if (j & 1)
				{
					ieps++;
					dval(&d) *= bigtens[i];
				}
		}
		if (k_check && dval(&d) < 1. && ilim > 0)
		{
			if (ilim1 <= 0)
				goto fast_failed;
			ilim = ilim1;
			k--;
			dval(&d) *= 10.;
			ieps++;
		}
		dval(&eps) = ieps*dval(&d) + 7.;
		word0(&eps) -= (P-1) * Exp_msk;
		if (ilim == 0)
		{
			S  = 0;
			dval(&d) -= 5.;
			if (dval(&d) > dval(&eps))
				goto one_digit;
			if (dval(&d) < -dval(&eps))
				goto no_digits;
			goto fast_failed;
		}
			/* Generate ilim digits, then fix them up. */
			dval(&eps) *= tens[ilim - 1];
			for(i = 1;; i++, dval(&d) *= 10.)
			{
				L = (int)(dval(&d));
				if (!(dval(&d) -= L))
					ilim = i;
				*s++ = '0' + (int)L;
				if (i == ilim)
				{
					if (dval(&d) > 0.5 + dval(&eps))
						goto bump_up;
					else if (dval(&d) < 0.5 - dval(&eps))
					{
						while(*--s == '0');
						s++;
						goto ret1;
					}
					break;
				}
			}
 fast_failed:
		s = s0;
		dval(&d) = dval(&d2);
		k = k0;
		ilim = ilim0;
	}

	/* Do we have a "small" integer? */
	if (be >= 0 && k <= Int_max)
	{
		/* Yes. */
		ds = tens[k];
		if (ndigits < 0 && ilim <= 0)
		{
			S = 0;
			if (ilim < 0 || dval(&d) <= 5 * ds)
				goto no_digits;
			goto one_digit;
		}
		for(i = 1;; i++, dval(&d) *= 10.)
		{
			L = (int)(dval(&d) / ds);
			dval(&d) -= L * ds;
			*s++ = '0' + (int)L;
			if (!dval(&d))
				break;
			if (i == ilim)
			{
				dval(&d) += dval(&d);
				if (dval(&d) > ds || (dval(&d) == ds && L & 1))
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
		b = pow5mult(b, b5);
	S = i2b(1);
	if (s5 > 0)
		S = pow5mult(S, s5);

	/* Check for special case that d is a normalized power of 2. */
	/* spec_case = 0; */

	/* Arrange for convenient computation of quotients:
	 * shift left if necessary so divisor has 4 leading 0 bits.
	 *
	 * Perhaps we should just compute leading 28 bits of S once
	 * and for all and pass them and a shift to quorem, so it
	 * can do shifts and ors to compute the numerator for q.
	 */
	if (( i = ((s5 ? 32 - hi0bits(S->x[S->wds - 1]) : 1) + s2) & 0x1f ) != 0)
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
		b = lshift(b, b2);
	if (s2 > 0)
		S = lshift(S, s2);
	if (k_check)
	{
		if (cmp(b,S) < 0)
		{
			k--;
			b = multadd(b, 10, 0);	/* we botched the k estimate */
			ilim = ilim1;
		}
	}
	if (ilim <= 0 && mode == 3)
	{
		if (ilim < 0 || cmp(b, S = multadd(S, 5, 0)) <= 0)
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
	for(i = 1;; i++)
	{
		*s++ = dig = quorem(b, S) + '0';
		if (!b->x[0] && b->wds <= 1)
			goto ret;
		if (i >= ilim)
			break;
		b = multadd(b, 10, 0);
	}

	/* Round off last digit */
	b = lshift(b, 1);
	j = cmp(b, S);
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
	Bfree(S);
 ret1:
	Bfree(b);
	*s = 0;
	*decpt = k + 1;
	if (rve)
		*rve = s;
	return s0;
}
