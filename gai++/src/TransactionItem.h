
#ifndef __TransactionItem_h__
#define __TransactionItem_h__

#include <string>

namespace GAI
{
	class TransactionItem
	{
    public:
		static TransactionItem* createItem(const char* aProductCode, const char* aProductName, const char* aProductCategory, const double aPrice, const int aQuantity);
    public:
		const char* getProductCode() const;

		const char* getProductName() const;

		const char* getProductCategory() const;

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
