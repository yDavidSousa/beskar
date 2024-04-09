#ifndef PACMAN_GUID_H
#define PACMAN_GUID_H

#include <array>

#ifdef __APPLE__
#include <CoreFoundation/CFUUID.h>
#elif
#endif

class guid
{
public:
    guid(const std::array<unsigned char, 16> &bytes) : _bytes(bytes){};

    std::string str() const
    {
        char one[10], two[6], three[6], four[6], five[14];

        snprintf(one, 10, "%02x%02x%02x%02x", _bytes[0], _bytes[1], _bytes[2], _bytes[3]);
        snprintf(two, 6, "%02x%02x", _bytes[4], _bytes[5]);
        snprintf(three, 6, "%02x%02x", _bytes[6], _bytes[7]);
        snprintf(four, 6, "%02x%02x", _bytes[8], _bytes[9]);
        snprintf(five, 14, "%02x%02x%02x%02x%02x%02x", _bytes[10], _bytes[11], _bytes[12], _bytes[13], _bytes[14], _bytes[15]);

        std::string out(one);
        out += two;
        out += three;
        out += four;
        out += five;
        return out;
    };

    operator std::string() const
    {
        return str();
    };
private:
    std::array<unsigned char, 16> _bytes;

    friend std::ostream &operator<<(std::ostream &s, const guid &guid)
    {
        std::ios_base::fmtflags f(s.flags());
        s << std::hex << std::setfill('0')
          << std::setw(2) << (int)guid._bytes[0]
          << std::setw(2) << (int)guid._bytes[1]
          << std::setw(2) << (int)guid._bytes[2]
          << std::setw(2) << (int)guid._bytes[3]
          << std::setw(2) << (int)guid._bytes[4]
          << std::setw(2) << (int)guid._bytes[5]
          << std::setw(2) << (int)guid._bytes[6]
          << std::setw(2) << (int)guid._bytes[7]
          << std::setw(2) << (int)guid._bytes[8]
          << std::setw(2) << (int)guid._bytes[9]
          << std::setw(2) << (int)guid._bytes[10]
          << std::setw(2) << (int)guid._bytes[11]
          << std::setw(2) << (int)guid._bytes[12]
          << std::setw(2) << (int)guid._bytes[13]
          << std::setw(2) << (int)guid._bytes[14]
          << std::setw(2) << (int)guid._bytes[15];
        s.flags(f);
        return s;
    }
};

class guid_generator
{
public:
    static const guid new_guid()
    {
        CFUUIDRef uuid = CFUUIDCreate(NULL);
        CFUUIDBytes uuid_bytes = CFUUIDGetUUIDBytes(uuid);
        CFRelease(uuid);

        std::array<unsigned char, 16> bytes =
        {
            {
                uuid_bytes.byte0,
                uuid_bytes.byte1,
                uuid_bytes.byte2,
                uuid_bytes.byte3,
                uuid_bytes.byte4,
                uuid_bytes.byte5,
                uuid_bytes.byte6,
                uuid_bytes.byte7,
                uuid_bytes.byte8,
                uuid_bytes.byte9,
                uuid_bytes.byte10,
                uuid_bytes.byte11,
                uuid_bytes.byte12,
                uuid_bytes.byte13,
                uuid_bytes.byte14,
                uuid_bytes.byte15
            }
        };

        return guid(std::move(bytes));
    };
};

#endif //PACMAN_GUID_H
