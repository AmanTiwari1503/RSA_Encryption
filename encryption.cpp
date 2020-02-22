#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>

int main(){

	mpz_t sampled_value;
	gmp_randstate_t state;
	mpz_init(sampled_value);
	gmp_randinit_lc_2exp_size(state,37);
	while(true)
	{
    	mpz_urandomb(sampled_value, state, 512);
    	int p = mpz_probab_prime_p(sampled_value, 50);
    	if (p==2 || p==1)
          	break;
      	else
      		continue;
	}
	gmp_printf("%Zd\n", sampled_value);
	printf("%d\n", mpz_probab_prime_p(sampled_value, 50));
	mpz_clear(sampled_value);
	gmp_randclear(state);
}