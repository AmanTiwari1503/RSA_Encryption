#include <gmp.h>
#include <bits/stdc++.h>

string decrypt_block(string block,mpz_t d,mpz_t n);
string form_decrypted_string(string c,int k_size,mpz_t d,mpz_t n);
string vigenere_decrypt(string c, string k);

string decrypt_block(string block,mpz_t d,mpz_t n){
	mpz_t m,c,temp;
	mpz_inits(m, c, temp, NULL);
	int b_size = block.length();
	for(int i=0;i<block.length();i++)
	{
		b_size = b_size-1;
		char ch = block.at(i);
		int c_n = 0;
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

string form_decrypted_string(string c,int k_size,mpz_t d,mpz_t n)
{
	int count = 0;
	string temp = "",m = "";
	for(int i=0;i<c.length();i++)
	{
		if(c.at(i)==' ')
			continue;
		count = count+1;
		temp += c.at(i);
		if(count>k_size)
		{
			m += decrypt_block(temp,d,n);
			count = 0;
			temp="";
		}
	}
	return m;
}

string vigenere_decrypt(string c, string k){
	int len = k.length();
	string m = "";
	for (int i=0;i<c.length();i++)
	{
		int p1 = (int)c.at(i)-97;
		int p2 = (int)k.at(i%len)-97;
		p1 = p1-p2;
		if(p1<0)
			p1 = p1+26;
		m += (char)(p1+97);
	}
	return m;
}