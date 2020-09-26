#include "ft_gdtoaimp.h"

/*
** freedtoa(s) must be used to free values s returned by dtoa
** when MULTIPLE_THREADS is #defined.  It should be used in all cases,
** but for consistency with earlier versions of dtoa, it is optional
** when MULTIPLE_THREADS is not defined.
*/

void	ft_freedtoa(char *s)
{
	t_bigint *b;

	b = (t_bigint *)((int *)s - 1);
	b->maxwds = 1 << (b->k = *(int*)b);
	ft_bfree(b);
	if (s == dtoa_result)
		dtoa_result = NULL;
}
