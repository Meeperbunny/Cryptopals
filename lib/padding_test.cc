#include <gtest/gtest.h>
#include "padding.h"

TEST(PaddingTest, PKCSPad) {
    Bytestring empty;
    PKCS::Pad(empty);
    EXPECT_EQ(empty.size(), 0);

    std::string halfString = "HalfFilled";
    auto half = Bytestring::FromString(halfString);
    int halfSizeBeforeFilled = half.size();
    PKCS::Pad(half);
    EXPECT_EQ(half.size() % PKCS::multiple, 0);
    EXPECT_EQ(halfString, half.substring(0, halfString.size()).toAsciiString());
    for(int i = halfSizeBeforeFilled; i < PKCS::multiple; ++i) {
        EXPECT_EQ(half[i], std::byte(0x06));
    }

    std::string fullString = "This Is Filled!!";
    EXPECT_EQ(fullString.size(), 16);
    auto full = Bytestring::FromString("This Is Filled!!");
    PKCS::Pad(full);
    EXPECT_EQ(full.toAsciiString(), fullString);
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