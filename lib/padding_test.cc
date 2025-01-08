#include <gtest/gtest.h>
#include "padding.h"

TEST(PaddingTest, PKCSIsValid) {
    EXPECT_EQ(PKCS::isValidPadding(Bytestring::FromHex("00093475893475934875938475840101")), true);
    EXPECT_EQ(PKCS::isValidPadding(Bytestring::FromHex("00000002347239847293843743840202")), true);
    EXPECT_EQ(PKCS::isValidPadding(Bytestring::FromHex("00000002347239847293843743841202")), false);
    EXPECT_EQ(PKCS::isValidPadding(Bytestring::FromHex("00000000000000000000000600030303")), true);
    EXPECT_EQ(PKCS::isValidPadding(Bytestring::FromHex("00000000000000000000000604040404")), true);
    EXPECT_EQ(PKCS::isValidPadding(Bytestring::FromHex("00000000000000000000000005050505")), false);
    EXPECT_EQ(PKCS::isValidPadding(Bytestring::FromHex("00000000000000000000000000000000")), false);
}

TEST(PaddingTest, PKCSPad) {
    Bytestring empty;
    PKCS::Pad(empty);
    EXPECT_EQ(empty.size(), 16);

    std::string halfString = "HalfFilled";
    auto half = Bytestring::FromString(halfString);
    PKCS::Pad(half);
    EXPECT_EQ(half.size(), 16);
    EXPECT_EQ(halfString, half.substring(0, halfString.size()).toAsciiString());
    for(int i = halfString.size(); i < PKCS::multiple; ++i) {
        EXPECT_EQ(half[i], std::byte(0x06));
    }

    std::string fullString = "This Is Filled!!";
    EXPECT_EQ(fullString.size(), 16);
    auto full = Bytestring::FromString("This Is Filled!!");
    PKCS::Pad(full);
    EXPECT_EQ(full.size(), 32);
    EXPECT_EQ(full.toAsciiString().substr(0, 16), fullString);
    for(int i = fullString.size(); i < full.size(); ++i) {
        EXPECT_EQ(full[i], std::byte(0x10));
    }
}

TEST(PaddingTest, PKCSUnpad) {
    Bytestring empty;
    PKCS::Pad(empty);
    PKCS::Unpad(empty);
    EXPECT_EQ(empty.size(), 0);

    std::string halfString = "HalfFilled";
    auto half = Bytestring::FromString(halfString);
    PKCS::Pad(half);
    PKCS::Unpad(half);
    EXPECT_EQ(half.toAsciiString(), halfString);

    std::string fullString = "This Is Filled!!";
    EXPECT_EQ(fullString.size(), 16);
    auto full = Bytestring::FromString("This Is Filled!!");
    PKCS::Pad(full);
    PKCS::Unpad(full);
    EXPECT_EQ(full.toAsciiString(), fullString);
}