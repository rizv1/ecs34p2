#include <gtest/gtest.h>
#include "StringDataSource.h"
#include "StringDataSink.h"
#include "XMLWriter.h"
#include "XMLReader.h"

//read tests

TEST(CXMLReaderTest, BasicRead) {
    auto InputStream = std::make_shared<CStringDataSource>("<example attr=\"Hello World\"></example>");
    CXMLReader Reader(InputStream);
    SXMLEntity E;

    EXPECT_TRUE(Reader.ReadEntity(E));
    EXPECT_EQ(E.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(E.AttributeValue("attr"), "Hello World");

    EXPECT_TRUE(Reader.ReadEntity(E));
    EXPECT_EQ(E.DType, SXMLEntity::EType::EndElement);
}

TEST(CXMLReaderTest, SpecialCharacterAttributeTest) {
    auto InputStream = std::make_shared<CStringDataSource>(
        "<element attribute=\"&lt;value&gt;\"></element>");

    CXMLReader Reader(InputStream);
    SXMLEntity E;

    EXPECT_TRUE(Reader.ReadEntity(E));
    EXPECT_EQ(E.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(E.DNameData, "element");
    EXPECT_EQ(E.AttributeValue("attribute"), "<value>");

    EXPECT_TRUE(Reader.ReadEntity(E));
    EXPECT_EQ(E.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(E.DNameData, "element");
}

TEST(CXMLReaderTest, NestedElements) {
    // Test input XML with nested elements
    auto InputStream = std::make_shared<CStringDataSource>(
        "<parent><child1 attr=\"Value1\"></child1><child2 attr=\"Value2\"></child2></parent>");

    // Create XML reader
    CXMLReader Reader(InputStream);
    SXMLEntity E;

    // Read start element for parent
    EXPECT_TRUE(Reader.ReadEntity(E));
    EXPECT_EQ(E.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(E.DNameData, "parent");

    // Read start element for child1
    EXPECT_TRUE(Reader.ReadEntity(E));
    EXPECT_EQ(E.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(E.DNameData, "child1");
    EXPECT_EQ(E.AttributeValue("attr"), "Value1");

    // Read end element for child1
    EXPECT_TRUE(Reader.ReadEntity(E));
    EXPECT_EQ(E.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(E.DNameData, "child1");

    // Read start element for child2
    EXPECT_TRUE(Reader.ReadEntity(E));
    EXPECT_EQ(E.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(E.DNameData, "child2");
    EXPECT_EQ(E.AttributeValue("attr"), "Value2");

    // Read end element for child2
    EXPECT_TRUE(Reader.ReadEntity(E));
    EXPECT_EQ(E.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(E.DNameData, "child2");

    // Read end element for parent
    EXPECT_TRUE(Reader.ReadEntity(E));
    EXPECT_EQ(E.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(E.DNameData, "parent");
}



//XML Write Tests


TEST(CXMLWriterTest, BasicWrite) {
    auto OutputStream = std::make_shared<CStringDataSink>();
    CXMLWriter Writer(OutputStream);

    EXPECT_TRUE(Writer.WriteEntity({SXMLEntity::EType::StartElement, "example", {{"attr","Hello World"}}}));
    EXPECT_TRUE(Writer.WriteEntity({SXMLEntity::EType::EndElement, "example", {}}));

    EXPECT_EQ(OutputStream->String(), "<example attr=\"Hello World\"></example>");
}

TEST(CXMLWriterTest, WriteSpecialCharacters) {
    auto OutputStream = std::make_shared<CStringDataSink>();
    CXMLWriter Writer(OutputStream);

    // Write element with special characters in attribute
    EXPECT_TRUE(Writer.WriteEntity({SXMLEntity::EType::StartElement, "element", {{"attr", "&lt;value&gt;"}}}));
    EXPECT_TRUE(Writer.WriteEntity({SXMLEntity::EType::EndElement, "element", {}}));

    EXPECT_EQ(OutputStream->String(), "<element attr=\"&lt;value&gt;\"></element>");
}

TEST(CXMLWriterTest, WriteNestedElements) {
    auto OutputStream = std::make_shared<CStringDataSink>();
    CXMLWriter Writer(OutputStream);

    // Write nested elements
    EXPECT_TRUE(Writer.WriteEntity({SXMLEntity::EType::StartElement, "parent", {}}));
    EXPECT_TRUE(Writer.WriteEntity({SXMLEntity::EType::StartElement, "child1", {{"attr", "Value1"}}}));
    EXPECT_TRUE(Writer.WriteEntity({SXMLEntity::EType::EndElement, "child1", {}}));
    EXPECT_TRUE(Writer.WriteEntity({SXMLEntity::EType::StartElement, "child2", {{"attr", "Value2"}}}));
    EXPECT_TRUE(Writer.WriteEntity({SXMLEntity::EType::EndElement, "child2", {}}));
    EXPECT_TRUE(Writer.WriteEntity({SXMLEntity::EType::EndElement, "parent", {}}));

    EXPECT_EQ(OutputStream->String(),
              "<parent><child1 attr=\"Value1\"></child1><child2 attr=\"Value2\"></child2></parent>");
}

TEST(CXMLWriterTest, WriteEmptyElement) {
    auto OutputStream = std::make_shared<CStringDataSink>();
    CXMLWriter Writer(OutputStream);

    // Write an empty element
    EXPECT_TRUE(Writer.WriteEntity({SXMLEntity::EType::StartElement, "empty", {}}));
    EXPECT_TRUE(Writer.WriteEntity({SXMLEntity::EType::EndElement, "empty", {}}));

    EXPECT_EQ(OutputStream->String(), "<empty></empty>");
}