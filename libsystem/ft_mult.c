/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mult.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tayamamo <tayamamo@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 00:03:46 by tayamamo          #+#    #+#             */
/*   Updated: 2020/09/17 00:06:55 by tayamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

t_bigint	*ft_mult(t_bigint *a, t_bigint *b)
{
	t_bigint			*c;
	int					k, wa, wb, wc;
	unsigned int		*x, *xa, *xae, *xb, *xbe, *xc, *xc0;
	unsigned int		y;
	unsigned long long	carry, z;

	if (a->wds < b->wds)
	{
		c = a;
		a = b;
		b = c;
	}
	k = a->k;
	wa = a->wds;
	wb = b->wds;
	wc = wa + wb;
	if (wc > a->maxwds)
		k++;
	c = ft_balloc(k);
	for(x = c->x, xa = x + wc; x < xa; x++)
		*x = 0;
	xa = a->x;
	xae = xa + wa;
	xb = b->x;
	xbe = xb + wb;
	xc0 = c->x;
	for(; xb < xbe; xc0++)
	{
		if ( (y = *xb++) !=0)
		{
			x = xa;
			xc = xc0;
			carry = 0;
			do {
				z = *x++ * (unsigned long long)y + *xc + carry;
				carry = z >> 32;
				*xc++ = z & 0xffffffffUL;
			} while(x < xae);
			*xc = carry;
		}
	}
	for(xc0 = c->x, xc = xc0 + wc; wc > 0 && !*--xc; --wc)
		;
	c->wds = wc;
	return (c);
}
