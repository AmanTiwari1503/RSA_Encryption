#include <gmp.h>
#include <bits/stdc++.h>
using namespace std;

int determine_block_size(int alpha_set, mpz_t n);
string decrypt_block(string block,mpz_t d,mpz_t n);
string form_decrypted_string(string c,int k_size,mpz_t d,mpz_t n);
string vigenere_decrypt(string c, string k);

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

	string c1 = "";int count = 0;
	printf("Enter the encrypted message - \n");
	cin.ignore();
	getline(cin,c1);

	int rep = (c1.length()*1.0)/ks;
	string c2,m1;
	if(ks==kr)
	{
		char p[rep];int tc = 0;
		for(int i = c1.length()-rep;i<c1.length();i++)
		{
			p[tc] = c1.at(i);
			tc = tc+1; 
		}
		c1 = c1.substr(0,c1.length()-rep);
		m1 = form_decrypted_string(c1,kr,dr,nr);
		for(int i=0;i<rep;i++)
		{
			m1 = m1+p[i];
		}
		c2 = form_decrypted_string(m1,kr,es,ns);
	}
	else if (ks<kr)
	{
		m1 = form_decrypted_string(c1,kr,dr,nr);
		c2 = form_decrypted_string(m1,ks,es,ns);
	}
	else
	{
		char p1[2*rep]; int tc = 0;
		for(int i=c1.length()-2*rep;i<c1.length();i++)
		{
			p1[tc] = c1.at(i);
			tc = tc+1;
		}
		c1 = c1.substr(0,c1.length()-(2*rep));
		m1 = form_decrypted_string(c1,kr,dr,nr);
		for(int i=0;i<2*rep;i++)
		{
			m1 = m1+p1[i];
		}
		c2 = form_decrypted_string(m1,kr+1,es,ns);
	}

	string vig_key = c2.substr(0,10);
	c2 = c2.substr(10,c2.length()-10);
	string m = vigenere_decrypt(c2,vig_key);
	
	cout<<"Decrypted string: ";
	cout<<m<<"\n";
	mpz_clears(es,ds,ns,er,dr,nr,NULL);

}

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