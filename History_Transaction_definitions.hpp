#ifndef _HISTORY_TRANSACTION_HPP_
#define _HISTORY_TRANSACTION_HPP_

#include <cassert>
#include <iomanip>
#include <iostream>

#include "project3.hpp"
#include "History.hpp"
#include "Transaction.hpp"

////////////////////////////////////////////////////////////////////////////////
// Definitions for Transaction class
////////////////////////////////////////////////////////////////////////////////
//
//

// Constructor
// TASK
//
Transaction :: Transaction( std::string ticker_symbol,  unsigned int day_date,
        unsigned int month_date,  unsigned year_date,
        bool buy_sell_trans,  unsigned int number_shares,
        double trans_amount )
{
	symbol = ticker_symbol;
	day = day_date;
	month = month_date;
	if(buy_sell_trans == 1)
	{
		trans_type = "Buy";
	}
	else
	{
		trans_type = "Sell";
	}
	year = year_date;
	shares = number_shares;
	amount = trans_amount;
	share_balance = 0;
	acb_per_share = 0;
	p_next = nullptr;
	cgl = 0;
	acb = 0;
	trans_id =assigned_trans_id;
	++assigned_trans_id;

}


// Destructor
// TASK
//

Transaction :: ~Transaction()
{
	p_next = nullptr;
}


// TASK
// Overloaded < operator.
//
bool Transaction :: operator<( Transaction const &other )
{
	if(year < other.get_year())
	{
		return true;
	}
	else if(year> other.get_year())
	{
		return false;
	}
	else
	{
		if(month < other.get_month())
		{
			return true;
		}
		else if (month > other.get_month())
		{
			return false;
		}
		else
		{
			if(day < other.get_day())
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
}


// GIVEN
// Member functions to get values. 
//
std::string Transaction::get_symbol() const { return symbol; }
unsigned int Transaction::get_day() const { return day; }
unsigned int Transaction::get_month() const { return month; }
unsigned int Transaction::get_year() const { return year; }
unsigned int Transaction::get_shares() const { return shares; }
double Transaction::get_amount() const { return amount; }
double Transaction::get_acb() const { return acb; }
double Transaction::get_acb_per_share() const { return acb_per_share; }
unsigned int Transaction::get_share_balance() const { return share_balance; }
double Transaction::get_cgl() const { return cgl; }
bool Transaction::get_trans_type() const { return (trans_type == "Buy") ? true: false ; }
unsigned int Transaction::get_trans_id() const { return trans_id; }
Transaction *Transaction::get_next() { return p_next; }

// GIVEN
// Member functions to set values. 
//
void Transaction::set_acb( double acb_value ) { acb = acb_value; }
void Transaction::set_acb_per_share( double acb_share_value ) { acb_per_share = acb_share_value; }
void Transaction::set_share_balance( unsigned int bal ) { share_balance = bal ; }
void Transaction::set_cgl( double value ) { cgl = value; }
void Transaction::set_next( Transaction *p_new_next ) { p_next = p_new_next; }

// GIVEN
// Print the transaction.
//
void Transaction::print() {
  std::cout << std::fixed << std::setprecision(2);
  std::cout << std::setw(4) << get_trans_id() << " " 
    << std::setw(4) << get_symbol() << " " 
    << std::setw(4) << get_day() << " "  
    << std::setw(4) << get_month() << " " 
    << std::setw(4) << get_year() << " ";


  if ( get_trans_type() ) { 
    std::cout << "  Buy  "; 
  } else { std::cout << "  Sell "; }

  std::cout << std::setw(4) << get_shares() << " " 
    << std::setw(10) << get_amount() << " " 
    << std::setw(10) << get_acb() << " " << std::setw(4) << get_share_balance() << " "
    << std::setw(10) << std::setprecision(3) << get_acb_per_share() << " "
    << std::setw(10) << std::setprecision(3) << get_cgl() 
    << std::endl;
}


////////////////////////////////////////////////////////////////////////////////
// Definitions for the History class
////////////////////////////////////////////////////////////////////////////////
//
//


// Constructor
// TASK
//
History :: History()
{
	p_head = nullptr;
}

// Destructor
// TASK
//
History :: ~History()
{
	while(p_head != nullptr)
	{
		Transaction *temp = p_head;
		p_head = p_head->get_next();
		delete temp;
		temp = nullptr;
	}
}

// TASK
// read_transaction(...): Read the transaction history from file. 
//
void History :: read_history()
{
	ece150::open_file();

	while( ece150::next_trans_entry() )
	{

		Transaction *p_new_trans =  new Transaction{ece150::get_trans_symbol(), ece150::get_trans_day(),
				ece150::get_trans_month(), ece150::get_trans_year(),
				ece150::get_trans_type(), ece150::get_trans_shares(),
				ece150::get_trans_amount()};
		insert(p_new_trans);

		}
	ece150::close_file();
}

// insert(...): Insert transaction into linked list.
//
void History :: insert( Transaction *p_new_trans )
{
	Transaction *tmp = p_head;
	if(p_head== nullptr)
	{
		p_head = p_new_trans;
		p_head->set_next(nullptr);
	}
	else
	{
	while(tmp->get_next() != nullptr)
	{
		tmp = tmp->get_next();
	}
	tmp->set_next(p_new_trans);
	p_new_trans->set_next(nullptr);
	}
}


// TASK
// sort_by_date(): Sort the linked list by trade date.
//
void History :: sort_by_date()
{
		Transaction *check = p_head;
		Transaction *prev = nullptr;
		Transaction *curr = check->get_next();
		Transaction *tmp = curr->get_next();
		Transaction *tail = p_head;
		while (tmp != nullptr) {
			prev = nullptr;
			check = p_head;
			curr = tail->get_next();
			tmp = curr->get_next();
			while (*check < *curr)
			{
				prev = check;
				check = check->get_next();
			}
			if (prev == nullptr) {
				p_head = curr;
			} else {
				if (check == curr) {
					tail = curr;
				}
				prev->set_next(curr);
			}
			curr->set_next(check);
			tail->set_next(tmp);
		}
	}



// TASK
// update_acb_cgl(): Updates the ACB and CGL values. 
//

void History::update_acb_cgl() {
	Transaction *p_updated_list { p_head };
	double acb;
	double acb_per_share;
	unsigned int share_balance;
	double cgl;
	acb = p_updated_list->get_amount();
	p_updated_list->set_acb(acb);
	share_balance = p_updated_list->get_shares();
	p_updated_list->set_share_balance(share_balance);
	acb_per_share = p_updated_list->get_acb() / p_updated_list->get_share_balance();

	cgl = 0;
	p_updated_list->set_cgl(cgl);
	p_updated_list->set_acb_per_share(acb_per_share);
	p_updated_list = p_updated_list->get_next();
	while (p_updated_list != nullptr) {

		if (p_updated_list->get_trans_type()) { //if buy
			acb = acb + p_updated_list->get_amount();
			share_balance = share_balance + p_updated_list->get_shares();
			acb_per_share = acb / share_balance;

		} else { //if sell

			share_balance = share_balance - p_updated_list->get_shares();
			acb = acb - (p_updated_list->get_shares() * acb_per_share);
			cgl = p_updated_list->get_amount()
					- (p_updated_list->get_shares() * acb_per_share);
			p_updated_list->set_cgl(cgl);

		}

		p_updated_list->set_acb(acb);
		p_updated_list->set_share_balance(share_balance);
		acb_per_share = p_updated_list->get_acb()
				/ p_updated_list->get_share_balance();
		p_updated_list->set_acb_per_share(acb_per_share);

		p_updated_list = p_updated_list->get_next();

	}

}


// TASK
// compute_cgl(): )Compute the ACB, and CGL.
//
double History :: compute_cgl(unsigned int year )
{
	double total =0.0;
	Transaction *traverse = p_head;
	while(traverse != nullptr)
	{
		if(traverse->get_year()==year)
		{
			total = total + traverse->get_cgl();
		}
		traverse = traverse->get_next();
	}
	return total;
}



// TASK
// print() Print the transaction history.
//
void History :: print()
{
	std::cout << "===== BEGIN TRANSACTION HISTORY =====" << std::endl;
	Transaction *tmp = p_head;
	while(tmp != nullptr)
	{
		tmp->print();
		tmp=tmp->get_next();
	}
	std::cout << "===== END TRANSACTION HISTORY =====" << std::endl;
}



// GIVEN
// get_p_head(): Full access to the linked list.
// 
Transaction *History::get_p_head() { return p_head; }


#endif

