#pragma once

#include <functional>
#include <string>
#include <vector>

namespace AEC {

typedef unsigned char byte;

class Config {

public:
    Config(std::pair<std::string, std::function<void(const char *, byte *)>> hashFunc, std::string &codeAlphabet, int tailleMin, int tailleMax);
    Config();

    int64_t totalClearText();
    byte *hash(const char *data, byte *empreinte);
    void algoToMethod(std::string algoCode);

    inline int64_t getN() const { return _totalN; }

    inline const std::string &getCodeAlphabet() const { return _codeAlphabet; }
    inline void setCodeAlphabet(const std::string &codeAlphabet) { _codeAlphabet = codeAlphabet; }
    inline int getTailleMin() const { return _tailleMin; }
    inline void setTailleMin(int tailleMin) { _tailleMin = tailleMin; }
    inline int getTailleMax() const { return _tailleMax; }
    inline void setTailleMax(int tailleMax) { _tailleMax = tailleMax; }
    inline const std::string &getAlphabet() const { return _alphabet; }
    inline void setAlphabet(const std::string &alphabet) { _alphabet = alphabet; }
    inline const std::vector<int64_t> &getAllClearText() const { return _allClearText; }

    std::string getHashFunctionName();

private:
    std::string _codeAlphabet;
    int _tailleMin;
    int _tailleMax;
    int64_t _totalN;
    std::vector<int64_t> _allClearText;

    void initAlphabet();

    std::string _alphabet;
    std::pair<std::string, std::function<void(const char *, byte *)>> _hashFunc;
};
}