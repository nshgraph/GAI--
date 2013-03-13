#include <exception>
#include <string>


#ifndef __Transaction_Item_h__
#define __Transaction_Item_h__

namespace GAI
{
	class TransactionItem
	{
    public:
		static TransactionItem* createItem(std::string aProductCode, std::string aProductName, std::string aProductCategory, double aPrice, int32_t aQuantity);
    public:
		std::string getProductCode();

		std::string getProductName();

		std::string getProductCategory();

		double getPrice();

		int32_t getQuantity();
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
