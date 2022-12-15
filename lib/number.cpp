#include "number.h"

#include <cstring>
#include <iomanip>
#include <algorithm>

const uint32_t maxdig = 1000000000;

uint2022_t from_uint(uint32_t i) {
    uint2022_t temp;
    temp.digits[0] = i % maxdig;
    temp.digits[1] = i / maxdig;
    return temp;
}

uint2022_t from_string(const char* buff) {
    uint2022_t temp;
    uint32_t ten_degree = 1;
    uint32_t j = 0;
    for (int i = strlen(buff) - 1; i >= 0; i--, j++) {
        temp.digits[j / 9] += ten_degree * (buff[i] - '0');
        if (ten_degree == maxdig / 10) {
            ten_degree = 1;
        } else {
            ten_degree *= 10;
        }
    }
    return temp;
}

uint2022_t operator+(const uint2022_t& lhs, const uint2022_t& rhs) {
    uint2022_t temp;
    int k = 0;
    for (int i = 0; i < uint2022_quantity; i++) {
        int t = lhs.digits[i] + rhs.digits[i] + k;
        k = t / maxdig;
        temp.digits[i] = t % maxdig;
    }
    return temp;
}

uint2022_t operator-(const uint2022_t& lhs, const uint2022_t& rhs) {
    uint2022_t temp;
    int k = 0;
    for (int i = 0; i < uint2022_quantity; i++) {
        int t = lhs.digits[i] - rhs.digits[i] - k;
        k = 0;
        if (t < 0) {
            temp.digits[i] = t + maxdig;
            k = 1;
        } else {
            temp.digits[i] = t;
        }
    }
    return temp;
}

uint2022_t operator*(const uint2022_t& lhs, const uint2022_t& rhs) {
    uint2022_t temp;
    for (int i = 0; i < uint2022_quantity; i++) {
        for (int j = 0, carry = 0; j < uint2022_quantity; j++) {
            long long cur = temp.digits[i + j] + lhs.digits[i] * 1ll * rhs.digits[j] + carry;
            temp.digits[i + j] = cur % maxdig;
            carry = cur / maxdig;
        }
    }
    return temp;
}

uint2022_t operator/(const uint2022_t& lhs, const uint2022_t& rhs) {
    if (rhs == from_uint(0)) {
        throw std::overflow_error("Divide by zero exception");
    }
    if (lhs == from_uint(0)) {
        return from_uint(0);
    }

    uint2022_t temp;
    uint2022_t current;
    unsigned int lhs_size = 0;
    for (int i = uint2022_quantity - 1; i >= 0; i--) {
        if (lhs.digits[i] != 0) {
            lhs_size = i;
            break;
        }
    }

    for (int i = lhs_size; i >= 0; i--) {
        std::rotate (current.digits, current.digits + uint2022_quantity - 1, current.digits + uint2022_quantity);
        current.digits[0] = lhs.digits[i];
        uint32_t x = 0;
        uint32_t l = 0;
        uint32_t r = maxdig;
        while (l <= r) {
            uint32_t m = (l + r) / 2;
            uint2022_t t = rhs * from_uint(m);
            if (current > t || t == current) {
                x = m;
                l = m + 1;
            } else {
                r = m - 1;
            }
        }
        temp.digits[i] = x;
        uint2022_t minus = rhs * from_uint(x);
        current = current - minus;
    }
    return temp;
}

bool operator==(const uint2022_t& lhs, const uint2022_t& rhs) {
    for (int i = 0; i < uint2022_quantity; i++) {
        if (lhs.digits[i] != rhs.digits[i]) {
            return false;
        }
    }
    return true;
}

bool operator!=(const uint2022_t& lhs, const uint2022_t& rhs) {
    return !(lhs == rhs);
}

bool operator>(const uint2022_t& lhs, const uint2022_t& rhs) {
    for (int i = uint2022_quantity - 1; i >= 0; i--) {
        if (lhs.digits[i] != rhs.digits[i]) {
            return lhs.digits[i] > rhs.digits[i];
        }
    }
    return false;
}

std::ostream& operator<<(std::ostream& stream, const uint2022_t& value) {
    bool begin = false;
    for (int i = uint2022_quantity - 1; i >= 0; i--) {
        if (value.digits[i] != 0) {
            if (begin == false) {
                stream << value.digits[i];
                begin = true;
            } else {
                stream << std::setw(9) << std::setfill('0') << value.digits[i];
            }
        } else {
            if (begin == true) {
                stream << std::setw(9) << std::setfill('0') << value.digits[i];
            } else if (i == 0) {
                stream << 0;
            }
        }
    }
    return stream;
}
