
#ifndef __TransactionItem_h__
#define __TransactionItem_h__

#include <string>

namespace GAI
{
	class TransactionItem
	{
    public:
		static TransactionItem* createItem(std::string aProductCode, std::string aProductName, std::string aProductCategory, int64_t aPriceMicros, int32_t aQuantity);
    public:
		std::string getProductCode();

		std::string getProductName();

		std::string getProductCategory();

		int64_t getPriceMicros();

		int32_t getQuantity();
    private:
        TransactionItem();
        std::string _productCode;
        std::string _productName;
        std::string _productCategory;
        int64_t _priceMicros;
        int32_t _quantity;
	};
}

#endif
