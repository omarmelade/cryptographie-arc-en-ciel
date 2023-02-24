#pragma once

#include <functional>
#include <string>
#include <vector>

namespace AEC {

typedef unsigned char byte;

class Config {

public:
    Config(std::string hashFuncName, std::function<void(const char *, byte *)> hashFunc, std::string &codeAlphabet, int tailleMin, int tailleMax);
    Config();

    uint64_t totalClearText();
    byte *hash(const char *data, byte *empreinte);
    void algoToMethod(std::string algoCode);

    void initAlphabet();
    void updateConfig();

    inline uint64_t getN() const { return _totalN; }
    inline void setN(uint64_t N) { _totalN = N; }
    inline const std::string &getCodeAlphabet() const { return _codeAlphabet; }
    inline void setCodeAlphabet(const std::string &codeAlphabet) { _codeAlphabet = codeAlphabet; }
    inline int getTailleMin() const { return _tailleMin; }
    inline void setTailleMin(int tailleMin) { _tailleMin = tailleMin; }
    inline int getTailleMax() const { return _tailleMax; }
    inline void setTailleMax(int tailleMax) { _tailleMax = tailleMax; }
    inline const std::string &getAlphabet() const { return _alphabet; }
    inline void setAlphabet(const std::string &alphabet) { _alphabet = alphabet; }
    inline const std::vector<uint64_t> &getAllClearText() const { return _allClearText; }
    inline std::string getHashFunctionName() { return _hashFuncName; }
    inline void setHashFunctionName(std::string &funcName) { _hashFuncName = funcName; }
    inline void setHashFunction (std::function<void(const char *, byte *)> hashFunc) { _hashFunc = hashFunc; }

private:
    std::string _codeAlphabet;
    int _tailleMin;
    int _tailleMax;
    uint64_t _totalN;
    std::vector<uint64_t> _allClearText;


    std::string _alphabet;
    std::string _hashFuncName;
    std::function<void(const char *, byte *)> _hashFunc;
};
}