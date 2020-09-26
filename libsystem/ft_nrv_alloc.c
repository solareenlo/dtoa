#include "ft_gdtoaimp.h"

char	*ft_rv_alloc(int i)
{
	int	j;
	int	k;
	int	*r;

	j = sizeof(unsigned int);
	for(k = 0; (int)sizeof(t_bigint) - (int)sizeof(unsigned int) - (int)sizeof(int) + j <= i; j <<= 1)
		k++;
	r = (int *)ft_balloc(k);
	*r = k;
	return (dtoa_result = (char *)(r + 1));
}

char	*ft_nrv_alloc(char *s, char **rve, int n)
{
	char	*rv;
	char	*t;

	t = rv = ft_rv_alloc(n);
	while((*t = *s++) != 0)
		t++;
	if (rve)
		*rve = t;
	return (rv);
}
