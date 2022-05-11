#include <gtest/gtest.h>
#include <SFCoder.cpp>

TEST(SFCoder, oneLetter)
{
    std::string text = "a";
    SFCoder mycoder(text);

    EXPECT_EQ(mycoder.compression_ratio(), 0);
    EXPECT_EQ(mycoder.get_encoded(), "");
    EXPECT_EQ(mycoder.get_decoded(), text);
    EXPECT_EQ(mycoder.get_orsize(), 8);
    EXPECT_EQ(mycoder.get_ensize(), 0);
}

TEST(SFCoder, twoLetters)
{
    std::string text = "ab";
    SFCoder mycoder(text);

    EXPECT_EQ(mycoder.compression_ratio(), 0.125);
    EXPECT_EQ(mycoder.get_encoded(), "01");
    EXPECT_EQ(mycoder.get_decoded(), text);
    EXPECT_EQ(mycoder.get_orsize(), 16);
    EXPECT_EQ(mycoder.get_ensize(), 2);
}

TEST(SFCoder, smallPhrase)
{
    std::string text = "I'll be back";
    SFCoder mycoder(text);

    EXPECT_EQ(mycoder.compression_ratio(), (38 * 1.f/96));
    EXPECT_EQ(mycoder.get_encoded(), "10011110010010100010000100011001101101");
    EXPECT_EQ(mycoder.get_decoded(), text);
    EXPECT_EQ(mycoder.get_orsize(), 96);
    EXPECT_EQ(mycoder.get_ensize(), 38);
}