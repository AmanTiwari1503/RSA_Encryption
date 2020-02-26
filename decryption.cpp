#include <gmp.h>
#include <bits/stdc++.h>

string decrypt_block(string block,mpz_t d,mpz_t n);

string decrypt_block(string block,mpz_t d,mpz_t n){
	mpz_t m,c,temp;
	mpz_inits(m, c, temp, NULL);
	int b_size = block.length();
	for(int i=0;i<block.length();i++)
	{
		b_size = b_size-1;
		char ch = block.at(i);
		int c_n = 0;
		// if(ch == '.')
		// 	c_n = 26;
		// else if(ch == ',')
		// 	c_n = 27;
		// else if(ch == ' ')
		// 	c_n = 28;
		// else
		c_n = (int)ch - 97;
		mpz_ui_pow_ui(temp,26,b_size);
		mpz_addmul_ui(c,temp,c_n);
	}
	mpz_powm(m,c,d,n);
	mpz_clear(c);
	mpz_t q;
	mpz_init(q);
	string decrypt = "";
	for(int i=block.length()-2;i>=0;i--)
	{
		mpz_ui_pow_ui(temp,26,i);
		mpz_tdiv_qr(q,m,m,temp);
		decrypt += (char)(mpz_get_ui(q)+97);
	}
	mpz_clears(m, q, temp, NULL);
	return decrypt;
}