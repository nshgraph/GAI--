
#include "TransactionItem.h"

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
    
    std::string TransactionItem::getProductCode() const{
        return this->_productCode;
    }
    
    std::string TransactionItem::getProductName() const{
        return this->_productName;
    }
    
    std::string TransactionItem::getProductCategory() const{
        return this->_productCategory;
    }
    
    double TransactionItem::getPrice() const{
        return this->_price;
    }
    
    int32_t TransactionItem::getQuantity() const{
        return this->_quantity;
    }
    
}