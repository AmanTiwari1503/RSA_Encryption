#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>
#include "prime_generation.cpp"

int determine_block_size(int alpha_set, mpz_t n);
int main(){
	mpz_t pr;
	mpz_init(pr);
	generate_random_number(pr,512);
	int k = determine_block_size(29,pr);
	// gmp_printf("%Zd\n",pr);
	// printf("Block Size :%d\n", k);
}

int determine_block_size(int alpha_set, mpz_t n){
	int p = 0;
	mpz_t tmp;
	mpz_init(tmp);
	while(true){
		mpz_ui_pow_ui(tmp,alpha_set,p);
		// gmp_printf("%d - %Zd\n",p,tmp);
		int h = mpz_cmp(tmp,n);
		if(h>0)
			break;
		p = p+1;
	}
	return p-1;
}