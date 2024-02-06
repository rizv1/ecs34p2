#include <gtest/gtest.h>
#include "StringDataSource.h"
#include "StringDataSink.h"
#include "DSVReader.h"
#include "DSVWriter.h"


//DSVWriter
TEST(CDSVWriterTest, BasicWrite) {
    auto Sink = std::make_shared<CStringDataSink>();
    CDSVWriter Writer(Sink,'&');
    std::vector<std::string> input = {"Hello", "World!"};

    EXPECT_TRUE(Writer.WriteRow(input));
    EXPECT_EQ(Sink->String(),"Hello&World!");
}

TEST(CDSVWriterTest, QuoteValuesWithDelimiter) {
    auto Sink = std::make_shared<CStringDataSink>();
    CDSVWriter Writer(Sink, ',', true);
    std::vector<std::string> input = {"John", "Doe, Jr.", "30"};

    EXPECT_TRUE(Writer.WriteRow(input));
    EXPECT_EQ(Sink->String(), "\"John\",\"Doe, Jr.\",\"30\"");
}

TEST(CDSVWriterTest, ReplaceDoubleQuotes) {
    auto Sink = std::make_shared<CStringDataSink>();
    CDSVWriter Writer(Sink, ',', true);
    std::vector<std::string> input = {"John", "Doe\"Junior", "30"};

    EXPECT_TRUE(Writer.WriteRow(input));
    EXPECT_EQ(Sink->String(), "\"John\",\"Doe\"\"Junior\",\"30\"");
}

TEST(CDSVWriterTest, EmptyRow) {
    auto Sink = std::make_shared<CStringDataSink>();
    CDSVWriter Writer(Sink, ',', true);
    std::vector<std::string> input;

    EXPECT_TRUE(Writer.WriteRow(input));
    EXPECT_EQ(Sink->String(), "");
}

TEST(CDSVWriterTest, DoubleQuoteAsDelimiter) {
    auto Sink = std::make_shared<CStringDataSink>();
    CDSVWriter Writer(Sink, '"', true);
    std::vector<std::string> input = {"John", "Doe", "30"};

    EXPECT_TRUE(Writer.WriteRow(input));
    EXPECT_EQ(Sink->String(), "\"John\",\"Doe\",\"30\"");
}







//DSVReader
TEST(CDSVReaderTest, BasicRead) {
    auto Source = std::make_shared<CStringDataSource>("Hello\nWorld!");
    CDSVReader Reader(Source,'\n');
    std::vector<std::string> output;

    EXPECT_TRUE(Reader.ReadRow(output));
    ASSERT_EQ(output.size(),2);
    EXPECT_EQ(output[0],"Hello");
    EXPECT_EQ(output[1],"World!");
}

TEST(CDSVReaderTest, EmptyString) {
    auto Source = std::make_shared<CStringDataSource>("");
    CDSVReader Reader(Source, ',');
    std::vector<std::string> output;

    EXPECT_FALSE(Reader.ReadRow(output));
    EXPECT_TRUE(output.empty());
}


TEST(CDSVReaderTest, MultipleElements) {
    auto Source = std::make_shared<CStringDataSource>("One,Two,Three");
    CDSVReader Reader(Source, ',');
    std::vector<std::string> output;

    EXPECT_TRUE(Reader.ReadRow(output));
    ASSERT_EQ(output.size(), 3);
    EXPECT_EQ(output[0], "One");
    EXPECT_EQ(output[1], "Two");
    EXPECT_EQ(output[2], "Three");
}

TEST(CDSVReaderTest, DelimiterNotFound) {
    auto Source = std::make_shared<CStringDataSource>("whereisthedelimter???");
    CDSVReader Reader(Source, ',');
    std::vector<std::string> output;

    EXPECT_TRUE(Reader.ReadRow(output));
    ASSERT_EQ(output.size(), 1);
    EXPECT_EQ(output[0], "whereisthedelimter???");
}

TEST(CDSVReaderTest, CustomDelimiter) {
    auto Source = std::make_shared<CStringDataSource>("A|B|C|D");
    CDSVReader Reader(Source, '|');
    std::vector<std::string> output;

    EXPECT_TRUE(Reader.ReadRow(output));
    ASSERT_EQ(output.size(), 4);
    EXPECT_EQ(output[0], "A");
    EXPECT_EQ(output[1], "B");
    EXPECT_EQ(output[2], "C");
    EXPECT_EQ(output[3], "D");
}