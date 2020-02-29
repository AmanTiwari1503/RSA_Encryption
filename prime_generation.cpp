#include <bits/stdc++.h>
#include <gmp.h>
#include <unistd.h>
#include <cstdio>
using namespace std;

void generate_keys(mpz_t e, mpz_t d, mpz_t n);
void generate_prime(mpz_t prime_value, int bits);
void generate_random_number(mpz_t r_number, int size);
void build_dictionary(mpz_t dict[][2], mpz_t p_key[],int c_size);
void generate_strong_prime(mpz_t pf, mpz_t p1, mpz_t p2);

int main()
{
	FILE *dic;
	FILE *pri;
	int companies = 0;
	printf("Enter the number of companies:");
	cin>>companies;
	mpz_t dict[companies][2],p_key[companies];
	build_dictionary(dict,p_key,companies);
	dic = fopen("directory.txt","w");
	pri = fopen("private.txt","w");
	fprintf(dic, "Directory:\n");
	int k = 0;
	try{
		for(int i=0;i<companies;i++)
		{
			fprintf(dic,"Company %d\n",(i+1));
			fprintf(dic,"Public Keys:\ne:");
			k = mpz_out_str(dic,10,dict[i][0]);
			if(k==0)
				throw "Error occured while input-output operation";
			fprintf(dic, "\nn:");
			k = mpz_out_str(dic,10,dict[i][1]);
			if(k==0)
				throw "Error occured while input-output operation";
			fprintf(dic, "\n");
			fprintf(pri,"Company %d\n",(i+1));
			fprintf(pri, "Private Key:\nd:");
			k = mpz_out_str(pri,10,p_key[i]);
			if(k==0)
				throw "Error occured while input-output operation";
			fprintf(pri, "\n");
		}		
	}
	catch(string e)
	{
		cout<<e<<"\n";
	}
	fclose(dic);
	fclose(pri);
}

void build_dictionary(mpz_t dict[][2], mpz_t p_key[], int c_size){
	mpz_t e,d,n;
	for(int i=0;i<c_size;i++)
	{
		mpz_inits(dict[i][0],p_key[i],dict[i][1], NULL);
		generate_keys(dict[i][0],p_key[i],dict[i][1]);
		gmp_printf("%Zd\n%Zd\n%Zd\n",dict[i][0],p_key[i],dict[i][1]);
	}
}

void generate_keys(mpz_t e, mpz_t d, mpz_t n){
	mpz_t p,q,s,t,phi_n;
	mpz_inits(p,q,s,t,phi_n,NULL);
	generate_prime(s,256);
	usleep(2000000);
	generate_prime(t,256);
	generate_strong_prime(p,s,t);
	usleep(2000000);
	generate_prime(s,256);
	usleep(2000000);
	generate_prime(t,256);
	generate_strong_prime(q,s,t);
	mpz_mul(n,p,q);
	mpz_sub_ui(p,p,1);
	mpz_sub_ui(q,q,1);
	mpz_mul(phi_n,p,q);
	while(true){
		generate_random_number(e,500);
		int p = mpz_invert(d,e,phi_n);
		if(p==0)
			continue;
		else
			break;
	}
	mpz_clears(p,q,s,t,phi_n,NULL);
}

void generate_prime(mpz_t prime_value, int bits){
	gmp_randstate_t state;
	unsigned long seed = static_cast<unsigned long>(time(NULL));
	gmp_randinit_mt(state);
	gmp_randseed_ui(state,seed);
	while(true)
	{
		mpz_urandomb(prime_value,state,bits);
		//generate_random_number(prime_value, bits);
		int p = mpz_probab_prime_p(prime_value, 25);
		if (p==2 || p==1)
			break;
		else
			continue;
	}
	gmp_randclear(state);
}
void generate_random_number(mpz_t r_number, int size){
	gmp_randstate_t state;
	unsigned long seed = static_cast<unsigned long>(time(NULL));
	gmp_randinit_mt(state);
	gmp_randseed_ui(state,seed);
	mpz_urandomb(r_number,state,size);
	gmp_randclear(state);
}

void generate_strong_prime(mpz_t pf, mpz_t p1, mpz_t p2){
	int i=1;
	mpz_t r,p0,r1;
	mpz_inits(r,p0,r1,NULL);
	while(true)
	{
		cout<<i<<" ";
		mpz_mul_ui(r,p2,2*i);
		mpz_add_ui(r,r,1);
		int p = mpz_probab_prime_p(r, 25);
		if (p==2 || p==1)
			break;
		else
		{
			i = i+1;
			continue;
		}
	}
	cout<<"\n";
	mpz_sub_ui(r1,r,2);
	mpz_powm(p0,p1,r1,r);
	mpz_mul_ui(p0,p0,2);
	mpz_mod(p0,p0,r);	
	mpz_mul(p0,p0,p1);
	mpz_sub_ui(p0,p0,1);
	i = 1;
	while(true)
	{
		cout<<i<<" ";
		mpz_mul(pf,r,p1);
		mpz_mul_ui(pf,pf,2*i);
		mpz_add(pf,pf,p0);
		int p = mpz_probab_prime_p(pf, 25);
		if (p==2 || p==1)
			break;
		else
		{
			i = i+1;
			continue;
		}
	}
	mpz_clears(r,p0,r1,NULL);
}
