/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_quorem.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tayamamo <tayamamo@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 01:00:14 by tayamamo          #+#    #+#             */
/*   Updated: 2020/09/17 01:03:43 by tayamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_gdtoaimp.h"

int	ft_quorem(t_bigint *b, t_bigint *S)
{
	int					n;
	unsigned int		*bx, *bxe, q, *sx, *sxe;
	unsigned long long	borrow, carry, y, ys;

	n = S->wds;
	if (b->wds < n)
		return 0;
	sx = S->x;
	sxe = sx + --n;
	bx = b->x;
	bxe = bx + n;
	q = *bxe / (*sxe + 1);	/* ensure q <= true quotient */
	if (q)
	{
		borrow = 0;
		carry = 0;
		do {
			ys = *sx++ * (unsigned long long)q + carry;
			carry = ys >> 32;
			y = *bx - (ys & 0xffffffffUL) - borrow;
			borrow = y >> 32 & 1UL;
			*bx++ = y & 0xffffffffUL;
		} while(sx <= sxe);
		if (!*bxe)
		{
			bx = b->x;
			while(--bxe > bx && !*bxe)
				--n;
			b->wds = n;
		}
	}
	if (ft_cmp(b, S) >= 0)
	{
		q++;
		borrow = 0;
		carry = 0;
		bx = b->x;
		sx = S->x;
		do {
			ys = *sx++ + carry;
			carry = ys >> 32;
			y = *bx - (ys & 0xffffffffUL) - borrow;
			borrow = y >> 32 & 1UL;
			*bx++ = y & 0xffffffffUL;
		} while(sx <= sxe);
		bx = b->x;
		bxe = bx + n;
		if (!*bxe)
		{
			while(--bxe > bx && !*bxe)
				--n;
			b->wds = n;
		}
	}
	return (q);
}
