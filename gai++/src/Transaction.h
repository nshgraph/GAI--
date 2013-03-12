
#ifndef __Transaction_h__
#define __Transaction_h__

#include <string>
#include <list>

namespace GAI
{
	class TransactionItem;
    
	class Transaction
	{
    public:
		static Transaction* createTransaction( std::string transaction_id, std::string affiliation );
		
		~Transaction();
		
		void addItem( TransactionItem* item );
		
		std::string getTransactionId() const;
		std::string getAffiliation() const;
		
	private:
        Transaction( std::string transaction_id, std::string affiliation );
		
        typedef std::list<TransactionItem*> TransactionItemList;
        TransactionItemList mItems;
		
        const std::string	mTransactionId;
        const std::string	mAffiliation;
	};
}

#endif
