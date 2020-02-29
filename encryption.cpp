#include <gmp.h>
#include <bits/stdc++.h>
using namespace std;

int determine_block_size(int alpha_set, mpz_t n);
string encrypt_block(string block,mpz_t e,mpz_t n);
string form_encrypted_string(string m,int k_size,mpz_t e,mpz_t n);
string vigenere_encrypt(string m, string k);

int determine_block_size(int alpha_set, mpz_t n){
	int p = 0;
	mpz_t tmp;
	mpz_init(tmp);
	while(true){
		mpz_ui_pow_ui(tmp,alpha_set,p);
		int h = mpz_cmp(tmp,n);
		if(h>0)
			break;
		p = p+1;
	}
	mpz_clear(tmp);
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
		c_n = (int)ch - 97;
		mpz_ui_pow_ui(temp,26,b_size);
		mpz_addmul_ui(m,temp,c_n);
	}
	mpz_powm(c,m,e,n);
	mpz_clear(m);
	mpz_t q;
	mpz_init(q);
	string encrypt = "";
	for(int i=block.length();i>=0;i--)
	{
		mpz_ui_pow_ui(temp,26,i);
		mpz_tdiv_qr(q,c,c,temp);
		encrypt += (char)(mpz_get_ui(q)+97);
	}
	mpz_clears(c, q, temp, NULL);
	return encrypt;
}

string form_encrypted_string(string m,int k_size,mpz_t e,mpz_t n)
{
	int count = 0;
	string temp = "",c = "";
	for(int i=0;i<m.length();i++)
	{
		count = count+1;
		temp += m.at(i);
		if(count>=k_size)
		{
			c += encrypt_block(temp,e,n);
			count = 0;
			temp="";
		}
	}
	return c;
}

string vigenere_encrypt(string m, string k){
	int len = k.length();
	string c = "";
	for (int i=0;i<m.length();i++)
	{
		int p1 = (int)m.at(i)-97;
		int p2 = (int)k.at(i%len)-97;
		p1 = (p1+p2)%26;
		c += (char)(p1+97);
	}
	return c;
}