
#ifndef __TransactionItem_h__
#define __TransactionItem_h__

#include <string>

namespace GAI
{
	class TransactionItem
	{
    public:
		static TransactionItem* createItem(std::string aProductCode, std::string aProductName, std::string aProductCategory, double aPrice, int32_t aQuantity);
    public:
		std::string getProductCode() const;

		std::string getProductName() const;

		std::string getProductCategory() const;

		double getPrice() const;

		int32_t getQuantity() const;
    private:
        TransactionItem();
        std::string _productCode;
        std::string _productName;
        std::string _productCategory;
        double _price;
        int32_t _quantity;
	};
}

#endif
