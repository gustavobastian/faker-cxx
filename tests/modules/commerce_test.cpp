#include <algorithm>
#include <string_view>

#include "gtest/gtest.h"

#include "commerce_data.h"
#include "common/string_helper.h"
#include "faker-cxx/commerce.h"
#include "string_data.h"

using namespace ::testing;
using namespace faker;
using namespace faker::commerce;

class CommerceTest : public Test
{
public:
};

TEST_F(CommerceTest, shouldGenerateCommerceDepartment)
{
    const auto generatedDepartment = department();

    ASSERT_TRUE(std::ranges::any_of(departments, [generatedDepartment](const std::string_view& department)
                                    { return department == generatedDepartment; }));
}

TEST_F(CommerceTest, shouldGenerateSku)
{
    const auto generatedSku = sku();

    ASSERT_EQ(generatedSku.size(), 4);
    ASSERT_TRUE(std::ranges::all_of(generatedSku,
                                    [](char skuCharacter)
                                    {
                                        return std::ranges::any_of(string::numericCharacters,
                                                                   [skuCharacter](char numericCharacter)
                                                                   { return skuCharacter == numericCharacter; });
                                    }));
}

TEST_F(CommerceTest, shouldGenerateSkuWithSpecifiedLength)
{
    const auto skuLength = 8;

    const auto generatedSku = sku(skuLength);

    ASSERT_EQ(generatedSku.size(), skuLength);
    ASSERT_TRUE(std::ranges::all_of(generatedSku,
                                    [](char skuCharacter)
                                    {
                                        return std::ranges::any_of(string::numericCharacters,
                                                                   [skuCharacter](char numericCharacter)
                                                                   { return skuCharacter == numericCharacter; });
                                    }));
}
TEST_F(CommerceTest, shouldGenerateProductFullName)
{
    const auto generatedProductFullName = productFullName();
    const auto words = common::split(generatedProductFullName, " ");

    bool isValid = false;

    for (size_t i = 1; i < words.size(); ++i)
    {
        for (size_t j = i + 1; j < words.size(); ++j)
        {
            const std::string adjective = common::join(std::vector<std::string_view>(words.begin(), words.begin() + i), " ");
            const std::string material = common::join(std::vector<std::string_view>(words.begin() + i, words.begin() + j), " ");
            const std::string name     = common::join(std::vector<std::string_view>(words.begin() + j, words.end()), " ");

            const bool adjectiveMatch = std::ranges::any_of(productAdjectives, [&adjective](const std::string_view& a)
                                                            { return a == adjective; });

            const bool materialMatch = std::ranges::any_of(productMaterials, [&material](const std::string_view& m)
                                                           { return m == material; });

            const bool nameMatch = std::ranges::any_of(productNames, [&name](const std::string_view& n)
                                                       { return n == name; });

            if (adjectiveMatch && materialMatch && nameMatch)
            {
                isValid = true;
                break;
            }
        }
        if (isValid)
            break;
    }

    ASSERT_TRUE(isValid) << "Generated product full name \"" << generatedProductFullName << "\" is not a valid combination.";
}


TEST_F(CommerceTest, shouldGenerateProductAdjective)
{
    const auto generatedProductAdjective = productAdjective();

    ASSERT_TRUE(std::ranges::any_of(productAdjectives, [generatedProductAdjective](const std::string_view& adjective)
                                    { return adjective == generatedProductAdjective; }));
}

TEST_F(CommerceTest, shouldGenerateProductMaterial)
{
    const auto generatedProductMaterial = productMaterial();

    ASSERT_TRUE(std::ranges::any_of(productMaterials, [generatedProductMaterial](const std::string_view& material)
                                    { return material == generatedProductMaterial; }));
}

TEST_F(CommerceTest, shouldGenerateProductName)
{
    const auto generatedProductName = productName();

    ASSERT_TRUE(std::ranges::any_of(productNames, [generatedProductName](const std::string_view& productName)
                                    { return productName == generatedProductName; }));
}

TEST_F(CommerceTest, shouldGenerateEan13)
{
    const auto generatedEan13 = EAN13();

    int sum = 0;
    for (size_t i = 0; i < 13; i++)
    {
        if (i % 2 == 0)
        {
            sum += (generatedEan13[i] - '0');
        }
        else
        {
            sum += 3 * (generatedEan13[i] - '0');
        }
    }

    ASSERT_EQ(generatedEan13.size(), 13);
    ASSERT_TRUE(sum % 10 == 0);
}

TEST_F(CommerceTest, shouldGenerateEan8)
{
    const auto generatedEan8 = EAN8();

    int sum = 0;
    for (size_t i = 0; i < 8; i++)
    {
        if (i % 2 == 0)
        {
            sum += 3 * (generatedEan8[i] - '0');
        }
        else
        {
            sum += (generatedEan8[i] - '0');
        }
    }

    ASSERT_EQ(generatedEan8.size(), 8);
    ASSERT_TRUE(sum % 10 == 0);
}

TEST_F(CommerceTest, shouldGenerateIsbn13)
{
    const auto generatedIsbn13 = ISBN13();

    int sum = 0;
    for (size_t i = 0; i < 13; i++)
    {
        if (i % 2 == 0)
        {
            sum += (generatedIsbn13[i] - '0');
        }
        else
        {
            sum += 3 * (generatedIsbn13[i] - '0');
        }
    }

    ASSERT_EQ(generatedIsbn13.size(), 13);
    ASSERT_TRUE(sum % 10 == 0);
}

TEST_F(CommerceTest, shouldGenerateIsbn10)
{
    const auto generatedIsbn10 = ISBN10();

    int sum = 0, weight = 10;
    if (generatedIsbn10[9] != 'X')
    {
        for (size_t i = 0; i < 10; i++)
        {
            sum += (generatedIsbn10[i] - '0') * weight;
            weight--;
        }

        ASSERT_TRUE(sum % 11 == 0);
    }

    ASSERT_EQ(generatedIsbn10.size(), 10);
}

TEST_F(CommerceTest, shouldGenerateIsbn10WithX)
{
    std::string generatedIsbn10;
    bool foundXCase = false;

    for (size_t i = 0; i < 100 && !foundXCase; i++)
    {
        generatedIsbn10 = ISBN10();
        if (generatedIsbn10[9] == 'X')
        {
            foundXCase = true;
        }
    }

    if (!foundXCase)
    {
        generatedIsbn10 = "094339600X";
    }

    int sum = 0, weight = 10;
    for (size_t i = 0; i < 9; i++)
    {
        sum += (generatedIsbn10[i] - '0') * weight;
        weight--;
    }
    sum += 10;

    ASSERT_EQ(generatedIsbn10.size(), 10);
    ASSERT_TRUE(sum % 11 == 0);
}

TEST_F(CommerceTest, shouldGeneratePaymentType)
{
    const auto generatedPaymentType = paymentType();

    ASSERT_TRUE(std::ranges::any_of(paymentTypes, [generatedPaymentType](const std::string_view& paymentType)
                                    { return paymentType == generatedPaymentType; }));
}

TEST_F(CommerceTest, shouldGeneratePaymentProvider)
{
    const auto generatedPaymentProvider = paymentProvider();

    ASSERT_TRUE(std::ranges::any_of(paymentProviders,
                                    [generatedPaymentProvider](const std::string_view& paymentProvider)
                                    { return paymentProvider == generatedPaymentProvider; }));
}

TEST_F(CommerceTest, shouldGenerateProductDescription)
{
    const auto generatedProductDescription = productDescription();

    ASSERT_TRUE(std::ranges::any_of(productDescriptions,
                                    [generatedProductDescription](const std::string_view& productDescription)
                                    { return productDescription == generatedProductDescription; }));
}

TEST_F(CommerceTest, shouldGenerateProductCategory)
{
    const auto generatedProductCategory = productCategory();

    ASSERT_TRUE(std::ranges::any_of(productCategoryNames,
                                    [generatedProductCategory](const std::string_view& productCategory)
                                    { return productCategory == generatedProductCategory; }));
}

TEST_F(CommerceTest, shouldGenerateProductReview)
{
    const auto generatedProductReview = productReview();

    ASSERT_TRUE(std::ranges::any_of(productReviews, [generatedProductReview](const std::string_view& productReview)
                                    { return productReview == generatedProductReview; }));
}

TEST_F(CommerceTest, shouldGenerateDiscountType)
{
    const auto generatedDiscountType = discountType();

    ASSERT_TRUE(std::ranges::any_of(discountTypes, [generatedDiscountType](const std::string_view& discountType)
                                    { return discountType == generatedDiscountType; }));
}

TEST_F(CommerceTest, shouldGenerateOrderStatus)
{
    const auto generatedOrderStatus = orderStatus();

    ASSERT_TRUE(std::ranges::any_of(orderStatuses, [generatedOrderStatus](const std::string_view& orderStatus)
                                    { return orderStatus == generatedOrderStatus; }));
}

TEST_F(CommerceTest, shouldGenerateShippingCarrier)
{
    const auto generatedShippingCarrier = shippingCarrier();

    ASSERT_TRUE(std::ranges::any_of(shippingCarriers,
                                    [generatedShippingCarrier](const std::string_view& shippingCarrier)
                                    { return shippingCarrier == generatedShippingCarrier; }));
}
