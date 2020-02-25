#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <bits/stdc++.h>
#include "prime_generation.cpp"

int determine_block_size(int alpha_set, mpz_t n);
string encrypt_block(string block,mpz_t e,mpz_t n);

int main(){
	mpz_t e,d,n;
	mpz_inits(e,d,n,NULL);
	generate_keys(e,d,n);
	int k = determine_block_size(29,n);
	// gmp_printf("%Zd\n",pr);
	// printf("Block Size :%d\n", k);
	string m,tmp = "";int count = 0;
	printf("Enter the message to be encrypted\n");
	getline(cin,m);
	transform(m.begin(), m.end(), m.begin(), ::toupper); 
	for(int i=0;i<m.length();i++)
	{
		count = count+1;
		tmp += m.at(i);
		if(count>=k)
		{
			tmp = encrypt_block(tmp,e,n);
			cout<<tmp<<"\n";
			count = 0;
			tmp="";
		}
	}
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

string encrypt_block(string block,mpz_t e,mpz_t n){
	mpz_t m,c,temp;
	mpz_inits(m, c, temp, NULL);
	int b_size = block.length();
	for(int i=0;i<block.length();i++)
	{
		b_size = b_size-1;
		char ch = block.at(i);
		int c_n = 0;
		if(ch == '.')
			c_n = 26;
		else if(ch == ',')
			c_n = 27;
		else if(ch == '.')
			c_n = 28;
		else
			c_n = (int)ch - 97;

		mpz_ui_pow_ui(temp,29,b_size);
		mpz_addmul_ui(m,temp,c_n);
	}
	mpz_powm(c,m,e,n);
	mpz_clear(m);
	mpz_t q;
	mpz_init(q);
	string encypt = "";
	for(int i=block.length();i>=0;i--)
	{
		mpz_ui_pow_ui(temp,29,i);
		mpz_tdiv_qr(q,c,c,temp);
		encypt += (char)(mpz_get_ui(q)+97);
	}
	mpz_clears(c, q, temp, NULL);
	return encypt;
}