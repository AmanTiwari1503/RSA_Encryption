#include <gmp.h>
#include <bits/stdc++.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include "encryption.cpp"
#include "decryption.cpp"

string form_message_string(string m,int k_size);
string clean_string(string m);

int main(){
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
	cout<<ks<<" "<<kr<<"\n";
	string m = "",tmp = "",c = "";int count = 0;
	printf("Enter the message to be encrypted\n");
	cin.ignore();
	getline(cin,m);
	cout<<m<<"\n";
	// m = "she doesnt have anything to prove but she is going ANYWAY THATS her Character sHe kNoWs she doesnt have to but still i know that she is a very crafty person and i do not like to be in her influence";
	transform(m.begin(), m.end(), m.begin(), ::tolower);
	m = clean_string(m);
	string vig_key="";
	// printf("Enter the key for vigenere cipher: ");
	// cin>>vig_key;
	vig_key = "Hellofella";
	transform(vig_key.begin(), vig_key.end(), vig_key.begin(), ::tolower);
	m = vigenere_encrypt(m,vig_key);
	m = vig_key+m;
	m = form_message_string(m,ks);
	int rep = m.length()/ks;
	cout<<rep<<"\n";
	string c1,c2,m1,m2;
	if(ks==kr)
	{
		char p[rep];
		m2 = form_encrypted_string(m,ks,ds,ns);
		for(int i=0;i<rep;i++)
		{
			p[i] = m2.at(m2.length()-(i+1));
		}
		m2 = m2.substr(0,m2.length()-rep);
		c1 = form_encrypted_string(m2,kr,er,nr);
		m1 = form_decrypted_string(c1,kr,dr,nr);
		for(int i=0;i<rep;i++)
		{
			m1 = m1+p[rep-1-i];
		}
		c2 = form_decrypted_string(m1,kr,es,ns);
	}
	else if (ks<kr)
	{
		m2 = form_encrypted_string(m,ks,ds,ns);
		c1 = form_encrypted_string(m2,kr,er,nr);
		m1 = form_decrypted_string(c1,kr,dr,nr);
		c2 = form_decrypted_string(m1,ks,es,ns);
	}
	else
	{
		m2 = form_encrypted_string(m,ks,ds,ns);
		char p1 = m2.at(m2.length()-1);
		char p2 = m2.at(m2.length()-2);
		m2 = m2.substr(0,m2.length()-2);
		c1 = form_encrypted_string(m2,kr,er,nr);
		m1 = form_decrypted_string(c1,kr,dr,nr);
		m1 = m1+p2+p1;
		c2 = form_decrypted_string(m1,kr+1,es,ns);
	}
	// cout<<"Encrypted string: ";
	cout<<m2<<" "<<m2.length()<<"\n";
	cout<<c1<<" "<<c1.length()<<"\n";
	// cout<<c1<<"\n";
	cout<<m1<<" "<<m1.length()<<"\n";
	cout<<c2<<" "<<c2.length()<<"\n";

	vig_key = c2.substr(0,10);
	c2 = c2.substr(10,c2.length()-10);
	m = vigenere_decrypt(c2,vig_key);
	
	cout<<"Decrypted string: ";
	cout<<m<<"\n";
	mpz_clears(es,ds,ns,er,dr,nr,NULL);
}

string form_message_string(string m,int k_size)
{
	int count = 0;
	string temp = "",c = "";
	for(int i=0;i<m.length();i++)
	{
		count = count+1;
		temp += m.at(i);
		if(count>=k_size)
		{
			c +=temp;
			count = 0;
			temp="";
		}
	}
	if(temp != "")
	{
		for(int i = count+1;i<(k_size+1);i++)
		{
			temp+='a';
		}
	}
	c += temp;
	return c;
}
string clean_string(string m){
	string temp = "";
	for(int i=0;i<m.length();i++)
	{
		if(!(m.at(i)>='a' && m.at(i)<='z'))
			continue;
		temp += m.at(i);
	}
	return temp;
}