/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_d2b.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tayamamo <tayamamo@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/16 20:39:00 by tayamamo          #+#    #+#             */
/*   Updated: 2020/09/25 19:00:43 by tayamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_gdtoaimp.h"

/*
** PURPOSE
**		double 型の数値の bit 解析
** INPUT
** 		double	dd;		: 解析される数
** 		int		*e;		: log2(指数部) - 仮数部の bit が立っているまでの個数
** 		int		*bits	: 仮数部の左から bit が立っているまでの個数
** RETURN
**		解析された結果
** 		int				k;		: bit での右端にある 0 の個数
** 		int				wds;	: 仮数部の個数(1 or 2)
** 		unsigned int	x[1];	: x[0]:	仮数部の右から0を除いた部分
** 								: x[1]:	仮数部の前半部分 >>= k;
** 										or
** 										仮数部の前半部分そのまま
** NOTE
** 		t_d			d;	: double 型の前半 32 bit と後半 32 bit にアクセス
** 		int			de;	: 符号(1 bit) + 指数部(11 bit)
** 		int			k;	: 右端にある 0 の個数
** 		unsigned int y;	: 仮数部の後半(32 bit)
** 		unsigned int z;	: 仮数部の前半(20 bit)
** 		FRAC_MASK		: 0000 0000 0000 1111 1111 1111 1111 1111
**		0x7fffffff		: 0111 1111 1111 1111 1111 1111 1111 1111
**		EXP_MSK			: 0000 0000 0001 0000 0000 0000 0000 0000
**		EXP_SHIFT		: 20
**		BIAS			: 1023
**		P				: 53
*/

t_bigint	*ft_d2b(double dd, int *e, int *bits)
{
	t_bigint		*b;
	t_d				d;
	int				i;
	int				de;
	int				k;
	unsigned int	*x;
	unsigned int	y;
	unsigned int	z;

	d.d = dd;
	b = ft_balloc(1);
	x = b->x;
	z = WORD0(&d) & FRAC_MASK;
	WORD0(&d) &= 0x7fffffff;
	if ((de = (int)(WORD0(&d) >> EXP_SHIFT)) !=0)
		z |= EXP_MSK;
	if ((y = WORD1(&d)) != 0)
	{
		if ((k = ft_lo0bits(&y)) != 0)
		{
			x[0] = y | z << (32 - k);
			z >>= k;
		}
		else
			x[0] = y;
		i = b->wds = (x[1] = z) != 0 ? 2 : 1;
	}
	else
	{
		k = ft_lo0bits(&z);
		x[0] = z;
		i = b->wds = 1;
		k += 32;
	}
	if (de)
	{
		*e = de - BIAS - (P - 1) + k;
		*bits = P - k;
	}
	else
	{
		*e = de - BIAS - (P - 1) + 1 + k;
		*bits = 32 * i - ft_hi0bits(x[i - 1]);
	}
	return (b);
}
