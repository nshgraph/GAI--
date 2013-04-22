
#ifndef __TransactionItem_h__
#define __TransactionItem_h__

#include <string>

namespace GAI
{
	class TransactionItem
	{
    public:
		static TransactionItem* createItem(const std::string& aProductCode, const std::string& aProductName, const std::string& aProductCategory, const double aPrice, const int aQuantity);
    public:
		std::string getProductCode() const;

		std::string getProductName() const;

		std::string getProductCategory() const;

		double getPrice() const;

		int getQuantity() const;
    private:
        TransactionItem();
        std::string mProductCode;
        std::string mProductName;
        std::string mProductCategory;
        double mPrice;
        int mQuantity;
	};
}

#endif
