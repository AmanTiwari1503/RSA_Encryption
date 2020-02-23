#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <unistd.h>
#include <tuple>
#include <bits/stdc++.h>
using namespace std;

void generate_keys(mpz_t e, mpz_t d, mpz_t n);
void generate_prime(mpz_t prime_value, int bits);
void generate_random_number(mpz_t r_number, int size);
void build_dictionary(mpz_t dict[][3], int c_size);

void build_dictionary(mpz_t dict[][3], int c_size){
	mpz_t e,d,n;
	for(int i=0;i<c_size;i++)
	{
		mpz_inits(dict[i][0],dict[i][1],dict[i][2], NULL);
		generate_keys(dict[i][0],dict[i][1],dict[i][2]);
		printf("Company %d\n",(i+1));
		gmp_printf("Public Keys:\ne:%Zd\nn:%Zd\n",dict[i][0], dict[i][2]);
		gmp_printf("Private Key:\nd:%Zd\n",dict[i][1]);
	}
}

void generate_keys(mpz_t e, mpz_t d, mpz_t n){
	mpz_t p,q,one,phi_n;
	mpz_inits(p,q,phi_n,NULL);
	mpz_init_set_ui(one,1);
	generate_prime(p,256);
	usleep(3000000);
	generate_prime(q,256);
	mpz_mul(n,p,q);
	mpz_sub(p,p,one);
	mpz_sub(q,q,one);
	mpz_mul(phi_n,p,q);
	while(true){
		generate_random_number(e,500);
		int p = mpz_invert(d,e,phi_n);
		if(p==0)
			continue;
		else
			break;
	}
	mpz_clears(p,q,one,phi_n,NULL);
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
void generate_random_number(mpz_t r_number, int size){
	gmp_randstate_t state;
	unsigned long seed = static_cast<unsigned long>(time(NULL));
	gmp_randinit_mt(state);
	gmp_randseed_ui(state,seed);
	mpz_urandomb(r_number,state,size);
	gmp_randclear(state);
}
