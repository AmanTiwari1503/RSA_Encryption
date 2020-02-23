#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <unistd.h>
using namespace std;

void generate_prime(mpz_t prime_value, int bits);
int generate_keys(mpz_t d, mpz_t e, mpz_t n);
void generate_random_number(mpz_t r_number, int size);
int main(){
	mpz_t p,q,n,one,e,phi_n,d;
	mpz_inits(p,q,n,phi_n,e,d,NULL);
	mpz_init_set_ui(one,1);
	generate_prime(p,256);
	usleep(5000000);
	generate_prime(q,256);
	gmp_printf("p:%Zd\nq:%Zd\n",p,q);
	mpz_mul(n,p,q);
	mpz_sub(p,p,one);
	mpz_sub(q,q,one);
	mpz_mul(phi_n,p,q);
	generate_keys(d,e,phi_n);
	gmp_printf("Public Keys:\ne:%Zd\nn:%Zd\n",e,n);
	gmp_printf("Private Key:\nd:%Zd\n",d);
	mpz_clears(p,q,n,one,phi_n,e,d,NULL);
}

void generate_prime(mpz_t prime_value, int bits){
	gmp_randstate_t state;
	unsigned long seed = static_cast<unsigned long>(time(NULL));
	gmp_randinit_mt(state);
	gmp_randseed_ui(state,seed);
	while(true)
	{
		mpz_urandomb(prime_value,state,bits);
		//generate_random_number(prime_value, bits);
		int p = mpz_probab_prime_p(prime_value, 50);
		if (p==2 || p==1)
			break;
		else
			continue;
	}
	gmp_randclear(state);
}
int generate_keys(mpz_t d, mpz_t e, mpz_t n){
	while(true){
		generate_random_number(e,500);
		int p = mpz_invert(d,e,n);
		if(p==0)
			continue;
		else
			break;
	}
	
}
void generate_random_number(mpz_t r_number, int size){
	gmp_randstate_t state;
	unsigned long seed = static_cast<unsigned long>(time(NULL));
	gmp_randinit_mt(state);
	gmp_randseed_ui(state,seed);
	mpz_urandomb(r_number,state,size);
	gmp_randclear(state);
}
