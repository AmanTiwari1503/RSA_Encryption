#include <gmp.h>
#include <bits/stdc++.h>
#include "prime_generation.cpp"
#include "encryption.cpp"
#include "decryption.cpp"

string form_decrypted_string(string c,int k_size,mpz_t d,mpz_t n);
string form_encrypted_string(string m,int k_size,mpz_t e,mpz_t n);
int main(){
	int companies = 0;
	printf("Enter the number of companies:");
	cin>>companies;
	mpz_t dict[companies][2],p_key[companies];
	build_dictionary(dict,p_key,companies);
	printf("Directory:\n");
	for(int i=0;i<companies;i++)
	{
		printf("Company %d\n",(i+1));
		gmp_printf("Public Keys:\ne:%Zd\nn:%Zd\n",dict[i][0], dict[i][1]);
		//gmp_printf("Private Key:\nd:%Zd\n",p_key[i]);
	}
	printf("\n");
	int sm,rm;
	while(true){
		printf("Select sender company:");
		cin>>sm;
		if(sm>companies || sm<=0)
		{
			printf("Enter the valid company name\n");
		}
		else
			break;
	}
	while(true){
		printf("Select receiver company:");
		cin>>rm;
		if(sm == rm)
		{
			printf("Sender and Receiver cannot be the same. Try again.\n");
		}
		else if(rm>companies || rm<=0)
		{
			printf("Enter the valid company name\n");
		}
		else
			break;
	}
	sm = sm-1;
	rm = rm-1;
	mpz_t es,ds,ns;
	mpz_t er,dr,nr;
	mpz_inits(es,ds,ns,er,dr,nr,NULL);
	mpz_set(es,dict[sm][0]);mpz_set(ns,dict[sm][1]);mpz_set(ds,p_key[sm]);
	mpz_set(er,dict[rm][0]);mpz_set(nr,dict[rm][1]);mpz_set(dr,p_key[rm]);
	int kr = determine_block_size(26,nr);
	int ks = determine_block_size(26,ns);
	cout<<ks<<" "<<kr<<"\n";
	// // gmp_printf("%Zd\n",pr);
	// // printf("Block Size :%d\n", k);
	string m = "",tmp = "",c = "";int count = 0;
	// printf("Enter the message to be encrypted\n");
	// getline(cin,m);
	m = "she doesnt have anything to prove but she is going ANYWAY THATS her Character sHe kNoWs she doesnt have to but still i know that she is a very crafty person and i do not like to be in her influence";
	transform(m.begin(), m.end(), m.begin(), ::tolower);
	cout<<m<<"\n";
	string c1,c2,m1,m2;
	// mpz_mul(es,es,ds);
	// mpz_mod(es,es,ns);
	// gmp_printf("%Zd\n",es);
	if(ks==kr)
	{
		m2 = form_encrypted_string(m,ks,ds,ns);
		char p = m2.at(m2.length()-1);
		m2 = m2.substr(0,m2.length()-1);
		c1 = form_encrypted_string(m2,kr,er,nr);
		m1 = form_decrypted_string(c1,kr,dr,nr);
		m1 = m1+p;
		c2 = form_decrypted_string(m1,kr,es,ns);
	}
	else if (ks<kr)
	{
		m2 = form_decrypted_string(m,ks,es,ns);
		c1 = form_encrypted_string(m2,kr,er,nr);
		m1 = form_decrypted_string(c1,kr,dr,nr);
		c2 = form_decrypted_string(m1,kr-1,es,ns);
	}
	else
	{
		m2 = form_encrypted_string(m,ks,ds,ns);
		char p1 = m2.at(m2.length()-1);
		char p2 = m2.at(m2.length()-2);
		m2 = m2.substr(0,m2.length()-2);
		c1 = form_encrypted_string(m2,kr,er,nr);
		m1 = form_decrypted_string(c1,kr,dr,nr);
		m1 = m1+p1+p2;
		c2 = form_decrypted_string(m1,kr+2,es,ns);	
	}
	cout<<m2<<" "<<m2.length()<<"\n";
	cout<<c1<<" "<<c1.length()<<"\n";
	cout<<m1<<" "<<m1.length()<<"\n";
	cout<<c2<<" "<<c2.length()<<"\n";
	mpz_clears(es,ds,ns,er,dr,nr,NULL);
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
	// if(temp != "")
	// {
	// 	for(int i = count+1;i<=(k_size+1);i++)
	// 	{
	// 		temp+='a';
	// 	}
	// }
	// m += decrypt_block(temp,d,n);
	return m;
}

string form_encrypted_string(string m,int k_size,mpz_t e,mpz_t n)
{
	int count = 0;
	string temp = "",c = "";
	for(int i=0;i<m.length();i++)
	{
		if(m.at(i)==' ')
			continue;
		count = count+1;
		temp += m.at(i);
		if(count>=k_size)
		{
			c += encrypt_block(temp,e,n);
			count = 0;
			temp="";
		}
	}
	// if(temp != "")
	// {
	// 	for(int i = count+1;i<=(k_size+1);i++)
	// 	{
	// 		temp+='a';
	// 	}
	// }
	// c += encrypt_block(temp,e,n);
	return c;
}