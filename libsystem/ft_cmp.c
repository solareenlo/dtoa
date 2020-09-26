/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cmp.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tayamamo <tayamamo@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/16 20:05:33 by tayamamo          #+#    #+#             */
/*   Updated: 2020/09/16 22:53:39 by tayamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_gdtoaimp.h"

int ft_cmp(t_bigint *a, t_bigint *b)
{
	unsigned int	*xa, *xa0, *xb, *xb0;
	int				i;
	int				j;

	i = a->wds;
	j = b->wds;
	if (i -= j)
		return (i);
	xa0 = a->x;
	xa = xa0 + j;
	xb0 = b->x;
	xb = xb0 + j;
	while (1)
	{
		if (*--xa != *--xb)
			return (*xa < *xb ? -1 : 1);
		if (xa <= xa0)
			break ;
	}
	return (0);
}
