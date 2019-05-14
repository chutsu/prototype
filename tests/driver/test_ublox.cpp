#include "prototype/munit.hpp"
#include "prototype/driver/ublox.hpp"

namespace proto {

int test_ubx_msg_t() {
  ubx_msg_t msg;

  MU_CHECK(msg.ok == false);
  MU_CHECK(msg.msg_class == 0);
  MU_CHECK(msg.msg_id == 0);
  MU_CHECK(msg.payload_length == 0);
  MU_CHECK(msg.payload[0] == 0);
  MU_CHECK(msg.ck_a == 0);
  MU_CHECK(msg.ck_b == 0);

  return 0;
}

int test_ubx_msg_parse_and_serialize() {
  // Create frame
  uint8_t data[10] = {0};
  data[0] = 0xB5;  // SYNC 1
  data[1] = 0x62;  // SYNC 2
  data[2] = 0x01;  // Message class
  data[3] = 0x22;  // Message id
  data[4] = 0x0;   // Payload length low
  data[5] = 0x0;   // Payload length hi
  // -- Calculate checksum
  ubx_msg_checksum(
    data[2],
    data[3],
    (data[4] << 8) | (data[5]),
    nullptr,
    data[6],
    data[7]
  );

  // Parse
  ubx_msg_t msg;
  ubx_msg_parse(msg, data);
  MU_CHECK(msg.ok);
  MU_CHECK(msg.msg_class == 0x01);
  MU_CHECK(msg.msg_id == 0x22);
  MU_CHECK(msg.payload_length  == 0x0);
  MU_CHECK(msg.ck_a == 0x23);
  MU_CHECK(msg.ck_b == 0x6a);

  // Serialize
  uint8_t frame[1024] = {0};
  size_t frame_size = 0;
  ubx_msg_serialize(msg, frame, frame_size);

  MU_CHECK(frame_size == 8);
  for (size_t i = 0; i < frame_size; i++) {
    MU_CHECK(frame[i] == data[i]);
  }

  return 0;
}

int test_ubx_msg_checksum() {
  // Create frame
  uint8_t data[10] = {0};
  data[0] = 0xB5;  // SYNC 1
  data[1] = 0x62;  // SYNC 2
  data[2] = 0x01;  // Message class
  data[3] = 0x22;  // Message id
  data[4] = 0x0;   // Payload length low
  data[5] = 0x0;   // Payload length hi
  // -- Calculate checksum
  ubx_msg_checksum(
    data[2],
    data[3],
    (data[4] << 8) | (data[5]),
    nullptr,
    data[6],
    data[7]
  );

  // Create message
  ubx_msg_t msg{data};

  uint8_t ck_a = 0;
  uint8_t ck_b = 0;
  ubx_msg_checksum(msg, ck_a, ck_b);

  MU_CHECK(ck_a == msg.ck_a);
  MU_CHECK(ck_b == msg.ck_b);
  MU_CHECK(msg.ok);

  return 0;
}

int test_ubx_msg_print() {
  // Create frame
  uint8_t data[10] = {0};
  data[0] = 0xB5;  // SYNC 1
  data[1] = 0x62;  // SYNC 2
  data[2] = 0x01;  // Message class
  data[3] = 0x22;  // Message id
  data[4] = 0x0;   // Payload length low
  data[5] = 0x0;   // Payload length hi
  // -- Calculate checksum
  ubx_msg_checksum(
    data[2],
    data[3],
    (data[4] << 8) | (data[5]),
    nullptr,
    data[6],
    data[7]
  );

  // Create message
  ubx_msg_t msg{data};
  ubx_msg_print(msg);

  return 0;
}

int test_ublox_t() {
  ublox_t ublox;
  return 0;
}

int test_ubx_val_set_and_get() {
  ublox_t ublox;
  uint32_t key = CFG_MSGOUT_RTCM_3X_TYPE1005_USB;
  uint32_t val = 1;
  uint8_t val_size = 1;
  ubx_val_set(ublox, 1, key, val, val_size);

  uint32_t value = 0;
  ubx_val_get(ublox, 0, key, value);
  MU_CHECK(value == val);

  return 0;
}

int test_ublox_version() {
  ublox_t ublox;
  ublox_version(ublox);
  return 0;
}

int test_ublox_parse_rtcm3() {
  ublox_t base{"/dev/ttyACM0"};
  uint8_t data[25 + 129 + 201 + 176 + 14] = {
    // RTCM3 1005
    0xD3, 0x00, 0x13, 0x3E, 0xD0, 0x00, 0x03, 0x89, 0x43, 0x50,
    0xA5, 0x6B, 0xBF, 0xF8, 0xEC, 0xBB, 0xD8, 0x0B, 0x91, 0x87,
    0xEA, 0xA2, 0x09, 0xAA, 0xF3,

    // RTCM3 1074
    0xD3, 0x00, 0x7B, 0x43, 0x20, 0x00, 0x4B, 0x63, 0xBE, 0x62,
    0x00, 0x00, 0x00, 0x00, 0x8C, 0x35, 0x00, 0x00, 0x00, 0x00,
    0x20, 0x00, 0x80, 0x00, 0x55, 0xFE, 0x88, 0x9C, 0x8C, 0x86,
    0x94, 0xA0, 0xA5, 0x6B, 0x9C, 0x6E, 0xCE, 0x73, 0x19, 0x2B,
    0xE5, 0x08, 0x8A, 0x1E, 0xE7, 0x79, 0x5D, 0xF1, 0x9D, 0x62,
    0x57, 0xDD, 0x9D, 0xB7, 0x60, 0x77, 0xD0, 0xDE, 0x3E, 0x71,
    0x1C, 0x7D, 0x01, 0x15, 0x14, 0xFB, 0xB6, 0x13, 0xCB, 0x11,
    0x5F, 0x19, 0xF9, 0xBC, 0x4E, 0x26, 0xF7, 0x5B, 0x67, 0xDB,
    0xF0, 0xF0, 0x77, 0x77, 0x01, 0xB8, 0xB4, 0xF9, 0xBB, 0x9B,
    0xE5, 0x08, 0xBD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0x00, 0x18,
    0x59, 0x7E, 0x76, 0x5D, 0x75, 0xD3, 0x46, 0xF8, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4B, 0x63, 0xF7,

    // RTCM3 1077
    0xD3, 0x00, 0xC3, 0x43, 0x50, 0x00, 0x4B, 0x63, 0xDD, 0xA2,
    0x00, 0x00, 0x00, 0x00, 0x8C, 0x35, 0x00, 0x00, 0x00, 0x00,
    0x20, 0x00, 0x80, 0x00, 0x55, 0xFE, 0x88, 0x9C, 0x8C, 0x86,
    0x94, 0xA0, 0xA4, 0x00, 0x00, 0x00, 0x16, 0xA9, 0xBE, 0xEC,
    0xE7, 0x51, 0x72, 0xC6, 0x53, 0x01, 0xF2, 0x13, 0x07, 0xEF,
    0xFF, 0xB9, 0x04, 0x25, 0xEC, 0x9F, 0xA9, 0xC6, 0xE8, 0x33,
    0xB5, 0x67, 0x87, 0x80, 0xF7, 0x85, 0x6C, 0x98, 0x1E, 0xC6,
    0x1B, 0x78, 0xA1, 0x7A, 0x46, 0x7A, 0xA6, 0xC7, 0x61, 0xE9,
    0x86, 0x35, 0x37, 0xFE, 0x4E, 0x1B, 0xA7, 0xD5, 0xED, 0x8A,
    0x14, 0x1E, 0x10, 0xF9, 0xE1, 0x61, 0x9D, 0xE0, 0x93, 0xA0,
    0x06, 0xC9, 0xC8, 0x06, 0x0B, 0x76, 0x1E, 0x96, 0x14, 0x1D,
    0x6C, 0xDD, 0xE1, 0x73, 0xD3, 0xE0, 0x80, 0xC8, 0xDA, 0xB6,
    0xAD, 0xAB, 0x6A, 0xDB, 0xB6, 0xAD, 0xBB, 0x6A, 0xDB, 0xB6,
    0xAD, 0xB8, 0x00, 0xC0, 0x2C, 0x0B, 0xC3, 0x30, 0xB0, 0x2E,
    0x0B, 0x82, 0xE0, 0x98, 0x23, 0x07, 0xC0, 0x40, 0xD8, 0x2D,
    0x8E, 0x04, 0x3D, 0xB1, 0xBB, 0x5E, 0x0F, 0x67, 0x1E, 0x4F,
    0xC5, 0x6F, 0x8C, 0x1E, 0x22, 0xFC, 0x66, 0xA0, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x39,
    0xE7,

    // RTCM3 1097
    0xD3, 0x00, 0xAA, 0x44, 0x90, 0x00, 0x4B, 0x63, 0xCE, 0x02,
    0x00, 0x00, 0x20, 0x18, 0x00, 0xC0, 0x08, 0x00, 0x00, 0x00,
    0x20, 0x01, 0x00, 0x00, 0x7F, 0xFA, 0xBA, 0x42, 0x52, 0x6A,
    0x62, 0xC8, 0x00, 0x00, 0x07, 0x77, 0xEC, 0xC7, 0xD4, 0x97,
    0x89, 0xAF, 0x03, 0xC8, 0x06, 0xB7, 0xD8, 0x1F, 0x09, 0x80,
    0x04, 0x10, 0xCE, 0xCE, 0x59, 0x6C, 0x15, 0x30, 0x95, 0x09,
    0x87, 0xDC, 0x48, 0xB9, 0x48, 0x8A, 0x2F, 0x81, 0xD9, 0x66,
    0x1F, 0x99, 0x3F, 0x1A, 0x8F, 0xF1, 0x21, 0x6F, 0xBB, 0xB5,
    0x7B, 0xDC, 0x2F, 0xB3, 0x2B, 0x57, 0xB0, 0xEB, 0x60, 0x25,
    0x52, 0x98, 0x1F, 0xC6, 0xD0, 0x2D, 0x5D, 0x50, 0x28, 0x8A,
    0x70, 0x77, 0x09, 0x68, 0x7E, 0x2F, 0xD7, 0xC6, 0x96, 0x3F,
    0xC4, 0xBC, 0xBF, 0xEF, 0xCB, 0x2F, 0xF0, 0x26, 0x13, 0x68,
    0xDB, 0xB6, 0x8D, 0xBB, 0x68, 0xDA, 0x36, 0x8D, 0xBB, 0x68,
    0xDB, 0xB6, 0x8D, 0xB8, 0x00, 0x5A, 0x16, 0x05, 0xA1, 0x58,
    0x56, 0x15, 0x85, 0xA1, 0x70, 0x60, 0x17, 0x85, 0x21, 0x50,
    0x1A, 0x1E, 0x34, 0x16, 0x0D, 0x10, 0x1B, 0x47, 0x08, 0x5E,
    0x0F, 0x39, 0x7F, 0xB3, 0x0F, 0x8E, 0x31, 0x1C, 0x4A, 0x4D,
    0x6C, 0x99, 0xB8, 0x05, 0xDC, 0x86,

    // RTCM3 1230
    0xD3, 0x00, 0x08, 0x4C, 0xE0, 0x00, 0x8A, 0x00, 0x00, 0x00,
    0x00, 0xA8, 0xF7, 0x2A,
  };

  for (int i = 0; i < 129 + 25 + 176 + 14 + 201; i++) {
    if (ublox_parse_rtcm3(base, data[i])) {
      MU_CHECK(base.rtcm3_parser.buf_pos == 0);
      MU_CHECK(base.rtcm3_parser.msg_len == 0);
      MU_CHECK(base.msg_type == "");
    }
  }

  return 0;
}

int test_ublox_base() {
  ublox_t base;
  ublox_base_station_run(base);

  return 0;
}

int test_ublox_rover() {
  ublox_t rover;
  ublox_rover_run(rover);

  return 0;
}

void test_suite() {
  MU_ADD_TEST(test_ubx_msg_t);
  MU_ADD_TEST(test_ubx_msg_parse_and_serialize);
  MU_ADD_TEST(test_ubx_msg_checksum);
  MU_ADD_TEST(test_ubx_msg_print);

  MU_ADD_TEST(test_ublox_t);
  MU_ADD_TEST(test_ubx_val_set_and_get);

  MU_ADD_TEST(test_ublox_version);
  MU_ADD_TEST(test_ublox_parse_rtcm3);

  // MU_ADD_TEST(test_ublox_base);
  MU_ADD_TEST(test_ublox_rover);
}

} // namespace proto
MU_RUN_TESTS(proto::test_suite);
