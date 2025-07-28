#include "icmp_message.h"
#include <cstring>

/**
 * @brief Default constructor.
 *
 * Initializes an ICMP Echo Request message with default values:
 * - Type = 8 (Echo Request)
 * - Code = 0
 * - Checksum = 0
 * - Identifier and sequence number = 0
 * - Data = empty
 */
IcmpMessage::IcmpMessage()
    : type_(8), code_(0), checksum_(0), identifier_(0), sequence_number_(0), data_() {}

/**
 * @brief Parameterized constructor.
 *
 * Initializes all fields of the ICMP message.
 *
 * @param type ICMP message type (e.g., 8 for Echo Request, 0 for Echo Reply)
 * @param code ICMP code (usually 0)
 * @param id Identifier for the ICMP message
 * @param seq Sequence number
 * @param data Payload data (optional)
 */
IcmpMessage::IcmpMessage(uint8_t type, uint8_t code, uint16_t id,
                         uint16_t seq, const std::vector<uint8_t>& data)
    : type_(type), code_(code), checksum_(0), identifier_(id),
      sequence_number_(seq), data_(data) {}

/**
 * @brief Encodes the ICMP message into a byte buffer.
 *
 * Constructs a raw ICMP packet by serializing the header and payload
 * according to RFC 792. The checksum is calculated and inserted into the header.
 *
 * @return A vector containing the encoded message as bytes.
 */
std::vector<uint8_t> IcmpMessage::Encode() const {
  std::vector<uint8_t> buffer;

  // Reserve space for 8-byte header + payload
  buffer.reserve(8 + data_.size());

  // Header: type, code, checksum placeholder, identifier, sequence number
  buffer.push_back(type_);
  buffer.push_back(code_);
  buffer.push_back(0); // checksum high byte placeholder
  buffer.push_back(0); // checksum low byte placeholder
  buffer.push_back(identifier_ >> 8);
  buffer.push_back(identifier_ & 0xFF);
  buffer.push_back(sequence_number_ >> 8);
  buffer.push_back(sequence_number_ & 0xFF);

  // Append data payload
  buffer.insert(buffer.end(), data_.begin(), data_.end());

  // Compute checksum over full buffer
  uint16_t checksum = ComputeChecksum(buffer);

  // Insert checksum into buffer (big endian)
  buffer[2] = (checksum >> 8) & 0xFF;
  buffer[3] = checksum & 0xFF;

  return buffer;
}

/**
 * @brief Computes the Internet checksum for the ICMP message.
 *
 * Follows the standard Internet checksum algorithm as described in RFC 1071.
 * The checksum is computed over the entire ICMP message (header + data),
 * treating the data as a sequence of 16-bit words.
 *
 * @param buffer The byte buffer to calculate the checksum for.
 * @return The 16-bit checksum value.
 */
uint16_t IcmpMessage::ComputeChecksum(const std::vector<uint8_t>& buffer) const {
  uint32_t sum = 0;

  // Sum 16-bit words
  for (size_t i = 0; i + 1 < buffer.size(); i += 2) {
    uint16_t word = (buffer[i] << 8) | buffer[i + 1];
    sum += word;
  }

  // If odd number of bytes, pad with 0 on the right
  if (buffer.size() % 2 == 1) {
    uint16_t last = buffer.back() << 8;
    sum += last;
  }

  // Fold 32-bit sum to 16 bits by adding carry
  while (sum >> 16) {
    sum = (sum & 0xFFFF) + (sum >> 16);
  }

  // One's complement
  return static_cast<uint16_t>(~sum);
}
