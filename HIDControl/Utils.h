#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

class Utils {
public:
    Utils() = default;

public:
    static std::string gen_packet_crc(const std::string &data);
    static void gen_packet_crc(char *data, const int len);

    static std::string current_time();

    static size_t hash_packet(const std::string &data);

    static uint32_t big_endian_hex_str_to_int(const std::string &hex);
    static uint32_t little_endian_hex_str_to_int(const std::string &hex);

    static std::vector<std::string> hex_byte_to_char(const std::string &data);

    static std::vector<std::string>
    hex_byte_to_char_raw(const std::string &data);
    static std::string hex_byte_to_string_raw(const std::string &data);

    static void print_hex_byte(const std::vector<std::string> &vec);

    static std::string int_to_hex(int num);

    static char *USB_API_wideCharToMultiByte(wchar_t *pWCStrKey);

    static std::vector<std::string> split(std::string str, std::string pattern);

    static std::string ws2s(const std::wstring &ws);

    static bool start_with(const std::string &data, const std::string &start);
    static bool end_with(const std::string &data, const std::string &end);

    static bool bytes_equal(const std::string &lhs, const std::string &rhs);

    static std::vector<std::string> split_by_chunk(const std::string &data,
                                                   int chunkSize);
};

#endif // UITLS_H
