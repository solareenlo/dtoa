#include "ft_gdtoaimp.h"

static t_bigint *p5s;

t_bigint	*ft_pow5mult(t_bigint *b, int k)
{
	t_bigint	*b1, *p5, *p51;
	int			i;
	static int	p05[3] = { 5, 25, 125 };

	if ( (i = k & 3) !=0)
		b = ft_multadd(b, p05[i - 1], 0);
	if (!(k >>= 2))
		return (b);
	if ((p5 = p5s) == 0)
	{
		/* first time */
		p5 = p5s = ft_i2b(625);
		p5->next = 0;
	}
	while (1)
	{
		if (k & 1)
		{
			b1 = ft_mult(b, p5);
			ft_bfree(b);
			b = b1;
		}
		if (!(k >>= 1))
			break;
		if ((p51 = p5->next) == 0)
		{
			p51 = p5->next = ft_mult(p5, p5);
			p51->next = 0;
		}
		p5 = p51;
	}
	return (b);
}
