#include "ft_printf.h"

/*
** multiply by m and add a
*/

t_bigint	*ft_multadd(t_bigint *b, int m, int a)
{
	int					i;
	int					wds;
	unsigned int		*x;
	unsigned long long	carry;
	unsigned long long	y;
	t_bigint			*b1;

	i = 0;
	wds = b->wds;
	x = b->x;
	carry = a;
	do {
		y = *x * (unsigned long long)m + carry;
		carry = y >> 32;
		*x++ = y & 0xffffffffUL;
	} while(++i < wds);
	if (carry)
	{
		if (wds >= b->maxwds)
		{
			b1 = ft_balloc(b->k+1);
			ft_memcpy(&b1->sign, &b->sign, b->wds * sizeof(unsigned int) + 2*sizeof(int));
			ft_bfree(b);
			b = b1;
		}
		b->x[wds++] = carry;
		b->wds = wds;
	}
	return (b);
}
