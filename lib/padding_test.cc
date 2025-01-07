#include <gtest/gtest.h>
#include "padding.h"

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