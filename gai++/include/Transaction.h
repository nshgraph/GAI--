
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
		static Transaction* createTransaction( const char* transaction_id, const char* affiliation );
		
		~Transaction();
		
		void addItem( TransactionItem* item );
		
		const char* getTransactionId() const;
		const char* getAffiliation() const;
        
        TransactionItemList getTransactionItems() const;
		
	private:
        Transaction( std::string transaction_id, std::string affiliation );
		
        TransactionItemList mItems;
		
        const std::string	mTransactionId;
        const std::string	mAffiliation;
	};
}

#endif
