#include "Utils.h"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <sstream>

void Utils::gen_packet_crc(char *data, const int len) {
    // the first byte must be 0x03
    // length shuoud be less than 63
    if (len <= 2)
        return;
    int cur = 0;
    for (auto i = 2; i < len; ++i) {
        cur += data[i];
    }
    cur &= 0xff;
    data[1] = cur;
}

std::string Utils::gen_packet_crc(const std::string &data) {
    char *d = new char[data.size()];
    memcpy(d, data.data(), data.size());
    gen_packet_crc(d, data.size());
    std::string ret(d, data.size());
    delete[] d;
    return ret;
}

std::vector<std::string> Utils::hex_byte_to_char(const std::string &data) {
    std::vector<std::string> ret;
    for (size_t i = 0; i < data.size(); ++i) {
        auto d = int_to_hex(data[i]);
        d = d.size() > 1 ? d : "0" + d;
        d = "0x" + d;
        ret.push_back(d);
    }
    // return vector[0x01,0x02,0x03,0xa0,0xff]
    return ret;
}

std::vector<std::string> Utils::hex_byte_to_char_raw(const std::string &data) {
    std::vector<std::string> ret;
    for (size_t i = 0; i < data.size(); ++i) {
        auto d = int_to_hex(data[i]);
        d = d.size() > 1 ? d : "0" + d;
        ret.push_back(d);
    }
    // return vector[01,02,03,a0,ff]

    return ret;
}

std::string Utils::hex_byte_to_string_raw(const std::string &data) {
    std::string ret;
    for (size_t i = 0; i < data.size(); ++i) {
        auto d = int_to_hex(data[i]);
        d = d.size() > 1 ? d : "0" + d;
        ret = ret + d;
    }
    // return vector[01,02,03,a0,ff]

    return ret;
}

void Utils::print_hex_byte(const std::vector<std::string> &vec) {
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i] << " ";
    }
}

std::string Utils::int_to_hex(int num) {
    std::stringstream ss;
    ss << std::hex << num;
    return ss.str();
}

bool Utils::start_with(const std::string &data, const std::string &start) {
    if (data.size() < start.size()) {
        return false;
    }
    for (size_t i = 0; i < start.size(); ++i) {
        if (data[i] != start[i]) {
            return false;
        }
    }
    return true;
}

bool Utils::end_with(const std::string &data, const std::string &end) {
    // powered by AI, not testing ^_^
    if (data.length() >= end.length()) {
        return (data.compare(data.length() - end.length(), end.length(), end) ==
                0);
    }
    return false;
}

// std::string Utils::ws2s(const std::wstring &ws) {
//     std::string curLocale = setlocale(LC_ALL, NULL); // curLocale="C"
//     setlocale(LC_ALL, "chs");
//     const wchar_t *wcs = ws.c_str();
//     size_t dByteNum = sizeof(wchar_t) * ws.size() + 1;

//    char *dest = new char[dByteNum];
//    wcstombs_s(NULL, dest, dByteNum, wcs, _TRUNCATE);
//    std::string result = dest;
//    delete[] dest;
//    setlocale(LC_ALL, curLocale.c_str());
//    return result;
//}

bool Utils::bytes_equal(const std::string &lhs, const std::string &rhs) {
    auto len = lhs.size() > rhs.size() ? rhs.size() : lhs.size();

    for (size_t i = 0; i < len; i++) {
        if (lhs[i] != rhs[i]) {
            return false;
        }
    }
    return true;
}

std::string Utils::current_time() {
    auto now = std::chrono::system_clock::now();
    auto since_epoch = now.time_since_epoch();
    auto microseconds =
        std::chrono::duration_cast<std::chrono::microseconds>(since_epoch);
    return std::to_string(microseconds.count());
}

size_t Utils::hash_packet(const std::string &data) {
    std::hash<std::string> szHash;
    return szHash(current_time() + data);
}

uint32_t Utils::big_endian_hex_str_to_int(const std::string &hex) {
    // make sure the hex never overflow the max value of int
    std::string rehex = hex;
    if (start_with(rehex, "0x")) {
        rehex = rehex.substr(2, rehex.size() - 2);
    }
    std::reverse(rehex.begin(), rehex.end());
    int ret = 0;
    for (size_t i = 0; i < rehex.size(); ++i) {
        std::string str;
        str.push_back(rehex[i]);
        int number = std::stoi(str, 0, 16);
        ret = ret + number * std::pow(16, i);
    }
    return ret;
}

uint32_t Utils::little_endian_hex_str_to_int(const std::string &hex) {
    // make sure the hex never overflow the max value of int
    std::string rehex = hex;
    if (start_with(rehex, "0x")) {
        rehex = rehex.substr(2, rehex.size() - 2);
    }

    int ret = 0;
    for (size_t i = 0; i < rehex.size(); ++i) {
        std::string str;
        str.push_back(rehex[i]);
        int number = std::stoi(str, 0, 16);
        ret = ret + number * std::pow(16, i);
    }
    return ret;
}

std::vector<std::string> Utils::split_by_chunk(const std::string &data,
                                               int chunkSize) {
    std::vector<std::string> ret;
    for (size_t i = 0; i < data.size(); i += chunkSize) {
        int end = std::min(i + chunkSize, data.size());

        std::string buff(data.data() + i, data.data() + end);
        ret.push_back(buff);
    }
    return ret;
}
