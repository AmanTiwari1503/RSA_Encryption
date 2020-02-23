#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <unistd.h>
using namespace std;

void generate_prime(mpz_t prime_value);
int main(){
	mpz_t p,q,n;
	mpz_inits(p,q,n,NULL);
	generate_prime(p);
	usleep(5000000);
	generate_prime(q);
	mpz_mul(n,p,q);
	gmp_printf("%Zd\n%Zd\n%Zd\n", p,q,n);
	mpz_clears(p,q,n,NULL);
}

void generate_prime(mpz_t prime_value){
	gmp_randstate_t state;
	unsigned long seed = static_cast<unsigned long>(time(NULL));
	gmp_randinit_mt(state);
	gmp_randseed_ui(state,seed);
	while(true)
	{
    	mpz_urandomb(prime_value, state,256);
    	int p = mpz_probab_prime_p(prime_value, 50);
    	if (p==2 || p==1)
          	break;
      	else
      		continue;
	}
	gmp_randclear(state);
}