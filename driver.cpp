#include <gmp.h>
#include <bits/stdc++.h>
#include "prime_generation.cpp"
#include "encryption.cpp"
#include "decryption.cpp"

int main(){
	mpz_t e,d,n;
	mpz_inits(e,d,n,NULL);
	generate_keys(e,d,n);
	int k = determine_block_size(26,n);
	// gmp_printf("%Zd\n",pr);
	// printf("Block Size :%d\n", k);
	string m,tmp = "";int count = 0;
	string c = "";
	//printf("Enter the message to be encrypted\n");
	//getline(cin,m);
	m = "she doesnt have anything to prove but she is going ANYWAY THATS her Character sHe kNoWs she doesnt have to but still i know that she is a very crafty person and i do not like to be in her influence";
	transform(m.begin(), m.end(), m.begin(), ::tolower);
	for(int i=0;i<m.length();i++)
	{
		if(m.at(i)==' ')
			continue;
		count = count+1;
		tmp += m.at(i);
		if(count>=k)
		{
			c += encrypt_block(tmp,e,n);
			cout<<c<<"\n";
			count = 0;
			tmp="";
		}
	}
	m="";
	m = decrypt_block(c,d,n);
	cout<<m<<"\n";
}