#include "config.h"

#include <cmath>

namespace AEC {

Config::Config(std::pair<std::string, std::function<void(const char *, byte *)>> hashFunc, std::string &codeAlphabet, int tailleMin, int tailleMax)
    : _codeAlphabet(codeAlphabet),
      _tailleMin(tailleMin),
      _tailleMax(tailleMax),
      _totalN(0),
      _hashFunc(std::move(hashFunc)) {
    initAlphabet();
    _totalN = totalClearText();
}

void Config::initAlphabet() {
    std::string numbers = "0123456879";
    std::string lowercase = "abcdefghijklmnopqrstuvwxyz";
    std::string uppercase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::string special_chars = ",;:$";
    if (_codeAlphabet == "26") {
        _alphabet = lowercase;
    } else if (_codeAlphabet == "26A") {
        _alphabet = uppercase;
    } else if (_codeAlphabet == "36") {
        _alphabet = numbers + lowercase;
    } else if (_codeAlphabet == "36A") {
        _alphabet = numbers + uppercase;
    } else if (_codeAlphabet == "40") {
        _alphabet = numbers + lowercase + special_chars;
    } else if (_codeAlphabet == "40A") {
        _alphabet = numbers + uppercase + special_chars;
    } else if (_codeAlphabet == "52") {
        _alphabet = uppercase + lowercase;
    } else if (_codeAlphabet == "62") {
        _alphabet = numbers + uppercase + lowercase;
    } else if (_codeAlphabet == "66") {
        _alphabet = numbers + uppercase + lowercase + special_chars;
    }
}

int64_t Config::totalClearText() {
    int64_t acc = 0;
    for (int i = _tailleMin; i < _tailleMax + 1; ++i) {
        auto value = (int64_t) pow(_alphabet.size(), i);
        _allClearText.push_back(value);
        acc += value;
    }
    return acc;
}
byte *Config::hash(const char *data, byte *empreinte) {
    _hashFunc.second(data, empreinte);
    return empreinte;
}
std::string Config::getHashFunctionName() {
    return _hashFunc.first == "md5" ? "md5" : "sha1";
}

}