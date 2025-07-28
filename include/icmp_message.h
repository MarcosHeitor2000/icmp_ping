#pragma once

#include <cstdint>
#include <vector>

/**
 * @brief Represents an ICMP Echo Request or Echo Reply message (RFC 792).
 */
class IcmpMessage {
 public:
  /**
   * @brief Default constructor for an ICMP Echo Request with empty payload.
   */
  IcmpMessage();

  /**
   * @brief Constructs an ICMP message with specified fields.
   * @param type ICMP type (8 = Echo Request, 0 = Echo Reply)
   * @param code ICMP code (usually 0)
   * @param id Identifier to match requests and replies
   * @param seq Sequence number of the message
   * @param data Payload data to include in the message
   */
  IcmpMessage(uint8_t type, uint8_t code, uint16_t id, uint16_t seq, const std::vector<uint8_t>& data);

  /**
   * @brief Serializes the ICMP message into a byte array (header + data).
   * @return A vector containing the encoded message.
   */
  std::vector<uint8_t> Encode() const;

  /**
   * @brief Decodes a byte array into an ICMP message object.
   * @param buffer The raw byte buffer representing the ICMP message.
   * @return True if the buffer is a valid ICMP Echo Request/Reply, false otherwise.
   */
  bool Decode(const std::vector<uint8_t>& buffer);

  /**
   * @brief Returns the type field of the ICMP message.
   */
  uint8_t type() const { return type_; }

  /**
   * @brief Returns the code field of the ICMP message.
   */
  uint8_t code() const { return code_; }

  /**
   * @brief Returns the identifier field (used to match requests and replies).
   */
  uint16_t identifier() const { return identifier_; }

  /**
   * @brief Returns the sequence number field.
   */
  uint16_t sequence_number() const { return sequence_number_; }

  /**
   * @brief Returns the data (payload) of the message.
   */
  const std::vector<uint8_t>& data() const { return data_; }

 private:
  uint8_t type_;                ///< ICMP Type: 8 = Request, 0 = Reply
  uint8_t code_;                ///< ICMP Code (0 for Echo messages)
  uint16_t checksum_;           ///< Checksum for error detection
  uint16_t identifier_;         ///< Identifier field
  uint16_t sequence_number_;    ///< Sequence number field
  std::vector<uint8_t> data_;   ///< Payload data

  /**
   * @brief Computes the ICMP checksum over the given buffer.
   * @param buffer The byte buffer representing the ICMP message.
   * @return The computed 16-bit checksum value.
   */
  uint16_t ComputeChecksum(const std::vector<uint8_t>& buffer) const;
};
