
#include "Transaction.h"
#include "TransactionItem.h"

namespace GAI
{
    
    Transaction* Transaction::createTransaction( const char* transaction_id, const char* affiliation )
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
        if( std::string(transaction_id).empty() )
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
    
    const char* Transaction::getTransactionId() const
	///
	/// Get the Transaction ID
	///
	/// @return
	///  The transaction ID string
	///
	{
        return mTransactionId.c_str();
    }
    
    const char* Transaction::getAffiliation() const
	///
	/// Get the Transaction affiliation
	///
	/// @return
	///  The affiliation string
	///
	{
        return mAffiliation.c_str();
    }
    
    Transaction::TransactionItemList Transaction::getTransactionItems() const
	///
	/// Get the list of items associated with this transaction
	///
	/// @return
	///  The list of transaction items
	///
    {
        return mItems;
    }
    
}
