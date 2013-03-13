
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
        typedef std::list<TransactionItem*> TransactionItemList;
    public:
		static Transaction* createTransaction( std::string transaction_id, std::string affiliation );
		
		~Transaction();
		
		void addItem( TransactionItem* item );
		
		std::string getTransactionId() const;
		std::string getAffiliation() const;
        
        TransactionItemList getTransactionItems() const;
		
	private:
        Transaction( std::string transaction_id, std::string affiliation );
		
        TransactionItemList mItems;
		
        const std::string	mTransactionId;
        const std::string	mAffiliation;
	};
}

#endif
