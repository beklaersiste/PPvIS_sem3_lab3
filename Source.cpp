#include <iostream>
#include <vector>
using namespace std;

/// класс банк
/**
* \param database база данных об аккаунтах клиентов
* \param this_banknum индекс банка
* \param banknum количество всех банков
*/
class Bank
{
	/// закрытый класс банковского счёта
    /**
    * \param username имя клиента
	* \param number индекс аккаунта в базе
	* \param sum сумма на счёте
    */
	class Account
	{
	private:
		Account(int number)
		{
			this->number = number;
		}
		void link_user(string username)
		{
			this->username = username;
		}
		int get_number()
		{
			return number;
		}
		int get_sum()
		{
			return sum;
		}

		string username;
		int number;
		int sum;

		friend Bank;
	};
	vector <Account> database;
	static int banknum;
	int this_banknum;
	
public:
	
	int set_new_account(string username)
	{
		database.push_back(this_banknum+database.size());
		database[database.size() - 1].link_user(username);
		return database[database.size() - 1].get_number();
	}
	bool find_account(const int number)
	{
		for (int i = 0; i < database.size(); i++)
			if (number == database[i].get_number())
				return 1;
		return 0;
	}
	int check_sum(int num)
	{
		database[num - this_banknum].get_sum();
	}
	void write_off_money(int num, int sum)
	{
		database[num - this_banknum].sum -= sum;
	}
	void charge_money(int num, int sum)
	{
		database[num - this_banknum].sum += sum;
	}
	Bank()
	{
		banknum++;
		this_banknum = banknum*1000;
	}

};
int Bank::banknum = 0;

/// класс банковской карты
/**
* \param bank_account аккаунт, к которому привязвнв карта
* \param number номер карты
* \param password пинкод от карты
*/
class BankCard
{
public:
	BankCard(string number, string password)
	{
		this->number = number;
		this->password = password;
	}
	void link_to_account(int  number)
	{
		bank_account = number;
	}
	string find_passvord()
	{
		return password;
	}
	int get_bank_account()
	{
		return bank_account;
	}

private:
	int bank_account;
	string number;
	string password;
};

/// класс человек
/**
* \param name имя человека
* \param cash наличные
* \param bank_account_number номер его банковского счёта
*/
class Human
{
public:
	Human(string name)
	{
		this->name = name;
	}

	void set_bancaccount(Bank& bank)
	{
		bank_account_number.push_back(bank.set_new_account(name));
	}

	int get_bank_account_quantity()
	{
		return bank_account_number.size();
	}

	int get_account_number(int i)
	{
		return bank_account_number[i];
	}

	void link_card_to_account(BankCard& card, int  number)
	{
		card.link_to_account(number);
	}

	void set_cash(int cash)
	{
		this->cash = cash;
	}
	int get_cash()
	{
		return cash;
	}

	string i_know_passvord_from_my_card(BankCard& card)
	{
		return card.find_passvord();
	}

	void insert_card(ATM& atm, BankCard& card, const string& passvord)
	{
		atm.insert_card(*this, card, passvord);
	}

private:
	string name;
	int cash;
	vector <int> bank_account_number;
};

/// класс банкомат
/**
* \param interface механизм взаимодействия с пользователем
* \param bankarchive механизм связи с банком
* \param billMechanism механизм обработки денег
* \param cardMechanism механизм обработки карты
* \param banks_atm банк, к которому привязан банкомат
*/
class ATM
{
public:
	ATM(Bank& bank, int money_amount)
	{
		billMechanism.money_amount = money_amount;
		bankArchive.banks_atm = &bank;
	}

	void insert_card(Human& man, BankCard& card, const string& passvord)
	{
		cardMechanism.insert_card();
		card_account_num = cardMechanism.get_account_num(card);
		if (!cardMechanism.check_passvord(card, passvord))
		{
			interface.incor_passvord();
			cardMechanism.take_out_card();
		}
		else if (!bankArchive.check_account(card_account_num))
		{
			interface.card_error();
			cardMechanism.take_out_card();
		}
		else
			while (cardMechanism.card_inside)
			{
				switch (interface.start_ATM())
				{
				case 1:
				{
					interface.balance(bankArchive.show_money(card_account_num));
					break;
				}

				case 2:
				{
					int sumy = interface.withdraw_money();
					bankArchive.write_off_money(card_account_num, sumy);
					billMechanism.give_money(man, sumy);
					break;
				}

				case 3:
				{
					int sumy = interface.put_money();
					bankArchive.charge_money(card_account_num, sumy);
					billMechanism.take_money(man, sumy);
					break;
				}

				case 4:
				{
					cardMechanism.take_out_card();
					card_account_num = 0;
					break;
				}

				default:
					break;
				}
			}
	}

private:
	int card_account_num = 0;
	class BankArchive
	{
	private:
		Bank* banks_atm;
		bool check_account(int num)
		{
			return banks_atm->find_account(num);
		}
		int show_money(int num)
		{
			banks_atm->check_sum(num);
		}
		void write_off_money(int num, int sum)
		{
			if (banks_atm->check_sum(num) >= sum)
				banks_atm->write_off_money(num, sum);
		}
		void charge_money(int num, int sum)
		{
			banks_atm->charge_money(num, sum);
		}
		friend ATM;
	};
	BankArchive bankArchive;

	class Interface
	{
	private:
		int start_ATM()
		{
			int a;
			cout << "choose operation:" << endl;
			cout << "1 - balance" << endl;
			cout << "2 - withdraw money" << endl;
			cout << "3 - put money" << endl;
			cout << "4 - return card" << endl;
			cin >> a;
			return a;
		}
		void balance(int sum)
		{
			cout << "Your balance: " << sum << endl;
			system("pause");
		}
		int withdraw_money()
		{
			int a;
			cout << "How much do you want to withdraw: " << endl;
			cin >> a;
			return a;
		}
		int put_money()
		{
			int a;
			cout << "Insert money: " << endl;
			cin >> a;
			return a;
		}
		void card_error()
		{
			cout << "Error!!! This ATM can not process this card!" << endl;
		}
		void incor_passvord()
		{
			cout << "Passvord entered incorrectly!" << endl;
		}
		friend ATM;
	};
	Interface interface;

	class BillMechanism
	{
	private:
		int money_amount;
		void give_money(Human& man, int sum)
		{
			if (money_amount >= sum)
			{
				man.set_cash(man.get_cash() + sum);
				money_amount -= sum;
			}
		}
		void take_money(Human& man, int sum)
		{
			if (man.get_cash() >= sum)
			{
				man.set_cash(man.get_cash() - sum);
				money_amount += sum;
			}
		}


		friend ATM;
	};
	BillMechanism billMechanism;

	class CardMechanism
	{
	private:
		bool check_passvord(BankCard& card, const string& insert_passvord)
		{
			if (card.find_passvord() == insert_passvord)
				return 1;
			else return 0;
		}
		bool card_inside = 0;
		void insert_card()
		{
			card_inside = 1;
		}
		void take_out_card()
		{
			card_inside = 0;
		}
		int get_account_num(BankCard& card)
		{
			return card.get_bank_account();
		}
		friend ATM;
	};
	CardMechanism cardMechanism;
};
