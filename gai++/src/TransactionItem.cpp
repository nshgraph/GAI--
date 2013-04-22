
#include "TransactionItem.h"

namespace GAI
{
    TransactionItem* TransactionItem::createItem(const char* aProductCode, const char* aProductName, const char* aProductCategory, const double aPrice, const int aQuantity)
    ///
    /// Create a Transaction Item while sanity checking to ensure the required product code and name are provided
    ///
    /// @param aProductCode
    ///     Product Code
    /// @param aProductName
    ///     Product Name
    /// @param aProductCategory
    ///     Product Category
    /// @param aPrice
    ///     Price of product
    /// @param aQuantity
    ///     Quantity of product
    ///
    /// @return
    ///     Created TransactionItem or NULL if sanity checking failed
    ///
    {
        // sanity check input values
        if( std::string(aProductCode) == "" )
            return NULL;
		
        if( std::string(aProductName) == "" )
            return NULL;
		
        // create and return item
        TransactionItem* new_item = new TransactionItem();
        new_item->mProductCode = aProductCode;
        new_item->mProductName = aProductName;
        new_item->mProductCategory = aProductCategory;
        new_item->mPrice = aPrice;
        new_item->mQuantity = aQuantity;
        
        return new_item;
    }
    
    
    TransactionItem::TransactionItem()
    ///
    /// Constructor does nothing. This is private and only accessible from the static createItem()
    ///
    {
        
    }
    
    const char* TransactionItem::getProductCode() const{
        ///
        /// Return the product code of this item
        ///
        /// @return
        ///     droduct code of item
        ///
        return this->mProductCode.c_str();
    }
    
    const char* TransactionItem::getProductName() const{
        ///
        /// Return the product name of this item
        ///
        /// @return
        ///     product name of item
        ///
        return this->mProductName.c_str();
    }
    
    const char* TransactionItem::getProductCategory() const{
        ///
        /// Return the product category of this item
        ///
        /// @return
        ///     product category of item
        ///
        return this->mProductCategory.c_str();
    }
    
    double TransactionItem::getPrice() const{
        ///
        /// Return the Quantity of this item
        ///
        /// @return
        ///     quantity of item
        ///
        return this->mPrice;
    }
    
    int TransactionItem::getQuantity() const
    ///
    /// Return the Quantity of this item
    ///
    /// @return
    ///     quantity of item
    ///
    {
        return this->mQuantity;
    }
    
}
