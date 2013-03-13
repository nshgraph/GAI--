#include <exception>
#include <string>


#include "Transaction_Item.h"
namespace GAI
{
    TransactionItem* TransactionItem::createItem(std::string aProductCode, std::string aProductName, std::string aProductCategory, double aPrice, int32_t aQuantity)
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
        new_item->_price = aPrice;
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