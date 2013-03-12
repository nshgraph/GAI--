
#include "TransactionItem.h"

namespace GAI
{
    TransactionItem* TransactionItem::createItem(std::string aProductCode, std::string aProductName, std::string aProductCategory, int64_t aPriceMicros, int32_t aQuantity)
    {
        // sanity check input values
        if( aProductCode == "" )
            return NULL;
		
        if( aProductName == "" )
            return NULL;
		
        // create and return item
        TransactionItem* new_item = new TransactionItem();
        new_item->_productCode = aProductCode;
        new_item->_productName = aProductName;
        new_item->_productCategory = aProductCategory;
        new_item->_priceMicros = aPriceMicros;
        new_item->_quantity = aQuantity;
        
        return new_item;
    }
    
    
    TransactionItem::TransactionItem()
    {
        
    }
    
    std::string TransactionItem::getProductCode() {
        return this->_productCode;
    }
    
    std::string TransactionItem::getProductName() {
        return this->_productName;
    }
    
    std::string TransactionItem::getProductCategory() {
        return this->_productCategory;
    }
    
    int64_t TransactionItem::getPriceMicros() {
        return this->_priceMicros;
    }
    
    int32_t TransactionItem::getQuantity() {
        return this->_quantity;
    }
    
}