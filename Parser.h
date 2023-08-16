//
// Created by eren ekrem kallıkçı on 12.08.2023.
//
#pragma once

#include <string>
#include <string_view>
#include <sstream>
#include <algorithm>
#include <arpa/inet.h>
#include <type_traits>

//#define htonll(x) ((1==htonl(1)) ? (x) : ((uint64_t)htonl((x) & 0xFFFFFFFF) << 32) | htonl((x) >> 32))
//#define ntohll(x) ((1==ntohl(1)) ? (x) : ((uint64_t)ntohl((x) & 0xFFFFFFFF) << 32) | ntohl((x) >> 32))

using byte = char;

enum Padding {
    RIGHT,
    LEFT
};

class Parser {
private:
    Parser() {}

    template<class T>
    static byte *asBytes(T &t) {
      return reinterpret_cast<byte *>(&t);
    }

    template<class T>
    static T fromBytes(const std::string &bytes) {
      T *res = reinterpret_cast<T *>(const_cast<char *>(bytes.c_str()));

      return *res;
    }

    template<class T>
    static T fromBytes(const std::string_view &bytes) {
      T *res = reinterpret_cast<T *>(const_cast<char *>(bytes.data()));

      return *res;
    }

    template<class T>
    static T toNetworkOrder(T t) {
      if (std::is_same<uint16_t, T>::value || std::is_same<int16_t, T>::value) {
        return htons(t);
      } else if (std::is_same<uint32_t, T>::value || std::is_same<int32_t, T>::value) {
        return htonl(t);
      } else if (std::is_same<uint64_t, T>::value || std::is_same<int64_t, T>::value) {
        return htonll(t);
      } else if (std::is_same<byte, T>::value) {
        return t;
      } else if (std::is_same<byte, T>::value || std::is_same<byte, T>::value) {
        return t;
      } else {
        throw "Integral endianness error";
      }
    }

    template<class T>
    static T toHostOrder(T t) {
      if (std::is_same<uint16_t, T>::value || std::is_same<int16_t, T>::value) {
        return ntohs(t);
      } else if (std::is_same<uint32_t, T>::value || std::is_same<int32_t, T>::value) {
        return ntohl(t);
      } else if (std::is_same<uint64_t, T>::value || std::is_same<int64_t, T>::value) {
        return ntohll(t);
      } else {
        return t;
      }
    }

    static void removeSpaces(std::string &input) {
      input.erase(std::remove_if(std::begin(input), std::end(input), [l = std::locale{}](auto ch) {
          return std::isspace(ch, l);
      }), std::end(input));
    }

public:
    inline static void copyString(int32_t &destinationIndex,
                           const std::string &source,
                           std::string &destination,
                           int32_t destinationLen,
                           Padding padding = Padding::RIGHT) {
      if (padding == Padding::RIGHT) {
        std::copy(source.begin(), source.begin() + source.size(), destination.begin() + destinationIndex);
      } else {
        int32_t offset = destinationLen - source.size();
        std::copy(source.begin(), source.begin() + source.size(), destination.begin() + destinationIndex + offset);
      }
      destinationIndex += destinationLen;
    }

    template<class T>
    inline static void copyNumeric(int32_t &destinationIndex, const T &source, std::string &destination) {
      static_assert(std::is_integral<T>::value, "Integral required.");

      int32_t totalBytes = sizeof(source);

      T sourceNetworkOrder = toNetworkOrder(source);
      byte *sourceAsBytes = asBytes(sourceNetworkOrder);
      std::copy(sourceAsBytes, sourceAsBytes + totalBytes, destination.begin() + destinationIndex);
      destinationIndex += totalBytes;
    }

    inline static void parseString(int32_t &sourceIndex,
                            const std::string &source,
                            std::string &destination,
                            int32_t destinationLen,
                            bool removeSpacing = true) {
      destination = std::string(destinationLen, ' ');
      std::copy(source.begin() + sourceIndex, source.begin() + sourceIndex + destinationLen, destination.begin());
      if (removeSpacing) {
        removeSpaces(destination);
      }
      sourceIndex += destinationLen;
    }

    inline static void parseString(int32_t &sourceIndex,
                            const std::string_view &source,
                            std::string &destination,
                            int32_t destinationLen,
                            bool removeSpacing = true) {
      destination = std::string(destinationLen, ' ');
      std::copy(source.begin() + sourceIndex, source.begin() + sourceIndex + destinationLen, destination.begin());
      if (removeSpacing) {
        removeSpaces(destination);
      }
      sourceIndex += destinationLen;
    }

    template<class T>
    inline static T parseNumeric(int32_t &sourceIndex, const std::string &source) {
      static_assert(std::is_integral<T>::value, "Integral required.");

      int32_t totalBytes = sizeof(T);

      T t = fromBytes<T>(std::string{source.begin() + sourceIndex, source.begin() + sourceIndex + totalBytes});
      sourceIndex += totalBytes;

      return (t);
    }

    template<class T>
    inline static T lookupNumeric(const int32_t &sourceIndex, const std::string &source) {
      static_assert(std::is_integral<T>::value, "Integral required.");

      int32_t totalBytes = sizeof(T);

      T t = fromBytes<T>(std::string{source.begin() + sourceIndex, source.begin() + sourceIndex + totalBytes});

      return toHostOrder(t);
    }

    template<class T>
    inline static T parseNumeric(int32_t &sourceIndex, const std::string_view &source) {
      static_assert(std::is_integral_v<T> || std::is_enum_v<T>, "Integral required.");

      int32_t totalBytes = sizeof(T);

      T t = fromBytes<T>(std::string_view{source}.substr(sourceIndex, totalBytes));
      sourceIndex += totalBytes;

      return (t);
    }

    template<class T>
    inline static T lookupNumeric(const int32_t &sourceIndex, const std::string_view &source) {
      static_assert(std::is_integral<T>::value, "Integral required.");

      int32_t totalBytes = sizeof(T);

      T t = fromBytes<T>(std::string_view{source}.substr(sourceIndex, totalBytes));

      return toHostOrder(t);
    }
};


