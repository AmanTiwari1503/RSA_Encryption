#include <gmp.h>
#include <string>
#include <sstream>
#include <fstream>
#include <bits/stdc++.h>
using namespace std;

int determine_block_size(int alpha_set, mpz_t n);
string encrypt_block(string block,mpz_t e,mpz_t n);
string form_encrypted_string(string m,int k_size,mpz_t e,mpz_t n);

int main()
{
	string line;int companies = 0;	
	string s2 = "Company";
	ifstream infile("directory.txt");
	while(getline(infile,line))
	{
		if(line.find(s2)!=string::npos){
			companies+=1;
		}
	}
	infile.close();
	mpz_t dict[companies][2],p_key[companies];

	FILE *data;
	char p[500];char q[500];*p = '\0';*q = '\0';
	data = fopen("directory.txt","r");
	int i=0,j=0,index = 2;
	while(fscanf(data,"%[^\n]\n",p)!=EOF)
	{
		// printf("%s\n", p);
		if(p[0]=='e')
		{
			mpz_inits(dict[i][0], NULL);
			while(index<500)
			{
				q[index-2] = p[index];
				index = index+1;
			}
			q[498] = '\0';q[499] = '\0';
			int h = mpz_set_str(dict[i][0],q,10);
			// gmp_printf("%Zd\n",dict[i][0]);
			i = i+1;
			index = 2;
		}
		if(p[0]=='n')
		{
			mpz_inits(dict[j][1], NULL);
			while(index<500)
			{
				q[index-2] = p[index];
				index = index+1;
			}
			q[498] = '\0';q[499] = '\0';
			int h = mpz_set_str(dict[j][1],q,10);
			// gmp_printf("%Zd\n",dict[j][1]);
			j = j+1;
			index = 2;
		}
	}
	fclose(data);

	FILE *secret;
	secret = fopen("private.txt","r");
	i=0,index = 2;
	while(fscanf(secret,"%[^\n]\n",p)!=EOF)
	{
		// printf("%s\n", p);
		if(p[0]=='d')
		{
			mpz_inits(p_key[i], NULL);
			while(index<500)
			{
				q[index-2] = p[index];
				index = index+1;
			}
			q[498] = '\0';q[499] = '\0';
			int h = mpz_set_str(p_key[i],q,10);
			// gmp_printf("%Zd\n",p_key[i]);
			i = i+1;
			index = 2;
		}
	}
	fclose(secret);

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
	string m = "",tmp = "",c = "";int count = 0;
	// printf("Enter the message to be encrypted\n");
	// getline(cin,m);
	m = "she doesnt have anything to prove but she is going ANYWAY THATS her Character sHe kNoWs she doesnt have to but still i know that she is a very crafty person and i do not like to be in her influence";
	transform(m.begin(), m.end(), m.begin(), ::tolower);
	cout<<m<<"\n";
	string c1,c2,m1,m2;
	if(ks==kr)
	{
		m2 = form_encrypted_string(m,ks,ds,ns);
		char p = m2.at(m2.length()-1);
		m2 = m2.substr(0,m2.length()-1);
		c1 = form_encrypted_string(m2,kr,er,nr);
	}
	else if (ks<kr)
	{
		m2 = form_encrypted_string(m,ks,es,ns);
		c1 = form_encrypted_string(m2,kr,er,nr);
	}
	else
	{
		m2 = form_encrypted_string(m,ks,ds,ns);
		char p1 = m2.at(m2.length()-1);
		char p2 = m2.at(m2.length()-2);
		m2 = m2.substr(0,m2.length()-2);
		c1 = form_encrypted_string(m2,kr,er,nr);
	}
	cout<<"Encrypted string: ";
	cout<<c1<<" "<<c1.length()<<"\n";
	mpz_clears(es,ds,ns,er,dr,nr,NULL);
}

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