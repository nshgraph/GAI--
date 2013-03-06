#include <exception>
#include <string>
#include <list>


#ifndef __Transaction_h__
#define __Transaction_h__


namespace GAI
{
	class TransactionItem;
    
	class Transaction
	{
    public:

		static Transaction* createTransaction(std::string aTransactionId, std::string aAffiliation);
    public:
        ~Transaction();
		void addItem(TransactionItem* aItem);

		std::string getTransactionId();

		std::string getAffiliation();
    private:
        typedef std::list<TransactionItem*> TransactionItemList;
    private:
        Transaction();
        std::string _transactionId;
        std::string _affiliation;
        TransactionItemList _items;
	};
}

#endif
