
#include "Transaction.h"
#include "Transaction_Item.h"

namespace GAI
{
    
    Transaction* Transaction::createTransaction( std::string transaction_id, std::string affiliation )
	///
	/// Static function to create a Transaction. Transactions must have an ID, this function checks
	/// the transaction_id string is not empty.
	///
	/// @param transaction_id
	///  The ID string of this Transaction
	///
	/// @param affiliation
	///  The affiliation string of this Transaction
	///
	/// @return
	///  Transaction object
	///
	{
        if( transaction_id.empty() )
		{
            return NULL;
		}
		
        Transaction* new_transaction = new Transaction(transaction_id, affiliation);
		
        return new_transaction;
    }
    
    Transaction::Transaction( std::string transaction_id, std::string affiliation )
	///
	/// Constructor
	///
	/// @param transaction_id
	///  The ID string of this Transaction
	///
	/// @param affiliation
	///  The affiliation string of this Transaction
	///
	:
	mTransactionId( transaction_id ),
	mAffiliation( affiliation )
    {
        
    }
	
    Transaction::~Transaction()
	///
	/// Destructor
	///
    {
        // We need to empty the list of transaction items
        for( TransactionItemList::iterator it = mItems.begin(); it != mItems.end(); it++ )
		{
            delete *it;
		}
		
        mItems.clear();
    }
    
    void Transaction::addItem( TransactionItem* item )
	///
	/// Add a TransactionItem to this Transaction
	///
	/// @param item
	///  The TransactionItem to add
	///
	/// @return
	///  Nothing
	///
	{
        if( item != NULL )
		{
			mItems.push_back( item );
		}
    }
    
    std::string Transaction::getTransactionId() const
	///
	/// Get the Transaction ID
	///
	/// @return
	///  The transaction ID string
	///
	{
        return mTransactionId;
    }
    
    std::string Transaction::getAffiliation() const
	///
	/// Get the Transaction affiliation
	///
	/// @return
	///  The affiliation string
	///
	{
        return mAffiliation;
    }
    
}
