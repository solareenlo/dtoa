#include "ft_gdtoaimp.h"

/*
** PURPOSE
** 		32 bit の右側からの bit の 0 の連続個数を数える
**		右端から 0 のところは bit shift で 0 を消していく
** 		右端に必ず 1 が立つようにする
** INPUT
** 		unsigned int *y;	: 右端の 0 を bit shift で消される数
** RETURN
** 		k					: 右端にあった 0 の個数
** NOTE
** 		int	k;				: 右端にある 0 の個数
** 		7					: 0000 0000 0000 0000 0000 0000 0000 0111
** 		0xffff				: 0000 0000 0000 0000 1111 1111 1111 1111
** 		0xff				: 0000 0000 0000 0000 0000 0000 1111 1111
** 		0xf					: 0000 0000 0000 0000 0000 0000 0000 1111
*/

static void	bitshift2(int *k, unsigned int *x)
{
	if (!(*x & 0xffff))
	{
		*k = 16;
		*x >>= 16;
	}
	if (!(*x & 0xff))
	{
		*k += 8;
		*x >>= 8;
	}
	if (!(*x & 0xf))
	{
		*k += 4;
		*x >>= 4;
	}
	if (!(*x & 0x3))
	{
		*k += 2;
		*x >>= 2;
	}
}

static int	bitshift1(int *k, unsigned int *x)
{
	bitshift2(k, x);
	if (!(*x & 1))
	{
		(*k)++;
		*x >>= 1;
		if (!*x)
			return (32);
	}
	return (0);
}

int			ft_lo0bits(unsigned int *y)
{
	int				k;
	unsigned int	x;

	x = *y;
	if (x & 7)
	{
		if (x & 1)
			return (0);
		if (x & 2)
		{
			*y = x >> 1;
			return (1);
		}
		*y = x >> 2;
		return (2);
	}
	k = 0;
	if (bitshift1(&k, &x) == 32)
		return (32);
	*y = x;
	return (k);
}
