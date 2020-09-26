/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_balloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tayamamo <tayamamo@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/15 22:13:57 by tayamamo          #+#    #+#             */
/*   Updated: 2020/09/21 19:29:58 by tayamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_gdtoaimp.h"

/*
** The k > KMAX case does not need ACQUIRE_DTOA_LOCK(0),
** but this case seems very unlikely.
*/

t_bigint	*ft_balloc(int k)
{
	int			x;
	t_bigint	*rv;

	if (k <= KMAX && (rv = freelist[k]))
		freelist[k] = rv->next;
	else {
		x = 1 << k;
		rv = (t_bigint *)malloc(sizeof(t_bigint) + (x - 1) * sizeof(unsigned long long));
		rv->k = k;
		rv->maxwds = x;
	}
	rv->sign = rv->wds = 0;
	return (rv);
}

void		ft_bfree(t_bigint *v)
{
	if (v)
	{
		if (v->k > KMAX)
			free((void *)v);
		else
		{
			v->next = freelist[v->k];
			freelist[v->k] = v;
		}
	}
}
