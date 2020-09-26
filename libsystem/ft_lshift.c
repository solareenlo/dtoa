/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lshift.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tayamamo <tayamamo@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/16 22:33:03 by tayamamo          #+#    #+#             */
/*   Updated: 2020/09/26 16:45:45 by tayamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_gdtoaimp.h"

t_bigint *ft_lshift(t_bigint *b, int k)
{
	t_bigint		*b1;
	int				i, k1, n, n1;
	unsigned int	*x, *x1, *xe, z;

	n = k >> KSHIFT;
	k1 = b->k;
	n1 = n + b->wds + 1;
	for(i = b->maxwds; n1 > i; i <<= 1)
		k1++;
	b1 = ft_balloc(k1);
	x1 = b1->x;
	for(i = 0; i < n; i++)
		*x1++ = 0;
	x = b->x;
	xe = x + b->wds;
	if (k &= KMASK)
	{
		k1 = 32 - k;
		z = 0;
		do {
			*x1++ = *x << k | z;
			z = *x++ >> k1;
		} while(x < xe);
		if ((*x1 = z) !=0)
			++n1;
	}
	else do
		*x1++ = *x++;
		while(x < xe);
	b1->wds = n1 - 1;
	ft_bfree(b);
	return (b1);
}
