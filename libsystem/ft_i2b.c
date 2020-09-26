#include "ft_gdtoaimp.h"

t_bigint	*ft_i2b(int i)
{
	t_bigint *b;

	b = ft_balloc(1);
	b->x[0] = i;
	b->wds = 1;
	return (b);
}
