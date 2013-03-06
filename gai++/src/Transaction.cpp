#include <exception>
#include <string>


#include "Transaction.h"
#include "Transaction_Item.h"

namespace GAI
{
    
    Transaction* Transaction::createTransaction(std::string aTransactionId, std::string aAffiliation) {
        if( aTransactionId == "" )
            return NULL;
        Transaction* new_transaction = new Transaction();
        new_transaction->_transactionId = aTransactionId;
        new_transaction->_affiliation = aAffiliation;
        return new_transaction;
    }
    
    Transaction::Transaction()
    {
        
    }
    Transaction::~Transaction()
    {
        // we need to empty the list of transaction items
        for( TransactionItemList::iterator it = _items.begin(), it_end = _items.end(); it != it_end; it++ )
            delete *it;
        _items.clear();
    }
    
    void Transaction::addItem(TransactionItem* aItem) {
        if( aItem != NULL)
            _items.push_back(aItem);
    }
    
    std::string Transaction::getTransactionId() {
        return this->_transactionId;
    }
    
    std::string Transaction::getAffiliation() {
        return this->_affiliation;
    }
    
}