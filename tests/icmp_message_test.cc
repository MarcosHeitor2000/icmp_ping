#include "icmp_message.h"
#include <gtest/gtest.h>

TEST(IcmpMessageTest, EncodeReturnsCorrectSize) {
  IcmpMessage msg;
  auto encoded = msg.Encode();
  EXPECT_EQ(encoded.size(), 4);
}

TEST(IcmpMessageTest, DecodeAcceptsEchoRequest) {
  IcmpMessage msg;
  std::vector<uint8_t> buffer = {8, 0, 0, 0};  // Echo Request
  EXPECT_TRUE(msg.Decode(buffer));
}

TEST(IcmpMessageTest, DecodeRejectsInvalidBuffer) {
  IcmpMessage msg;
  std::vector<uint8_t> buffer = {99};  // inválido
  EXPECT_FALSE(msg.Decode(buffer));
}
