#include "StringDataSource.h"
#include "XMLReader.h"
#include "gtest/gtest.h"  // Include the Google Test header


TEST(CXMLReaderTest, BasicRead) {
    auto inputStream = std::make_shared<CStringDataSource>("<example>Hello World</example>");
    CXMLReader reader(inputStream);
    SXMLEntity entity;

    EXPECT_TRUE(reader.ReadEntity(entity, false));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(entity.DNameData, "example");



    EXPECT_TRUE(reader.ReadEntity(entity, false));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(entity.DNameData, "example");

    EXPECT_FALSE(reader.ReadEntity(entity, false));
}
