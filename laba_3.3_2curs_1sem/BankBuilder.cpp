#include "BankBuilder.h"

Bank* BankBuilder::buildBank()
{
	//int n = 3;
	int k1 = 20;
	int k2 = 30;
	int k3 = 40;
	int t = 2;
	int td = 1;
	int l = 10;
	int lmax = 2;

	Bank* bank = new Bank;
	bank->add_dep(k1);
	bank->add_dep(k2);
	bank->add_dep(k3);
	bank->bank_configuration(t, td, lmax, l);
	return bank;
	
}
