// отделения банка лаба 3.3
#include <iostream>
#include"BankBuilder.h"
#include <filesystem>

int main()
{
    BankBuilder builder;
    Bank* bank = builder.buildBank();
    bank->start();
    delete bank;
}

