#include "utility.h"

#include <cstring>
#include <string>
#include <iostream>
#include <iomanip>
#include <openssl/md5.h>
#include <openssl/sha.h>


namespace AEC {

const std::unordered_map<std::string, std::function<void(const char *, AEC::byte *)>> hashFunctionMap;

std::string Utility::empreinteToString(byte *empreinte)
{
    std::stringstream ss;
    for (int i = 0; i < MD5_DIGEST_LENGTH; i++)
        ss << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << (int) empreinte[i];
    return ss.str();
}

std::string Utility::i2c(Config &CFG, uint64_t index)
{
    std::string clearText;
    int wordSize = 0;
    uint64_t euclideanRemainder = index;
    auto T = CFG.getAllClearText();
    uint64_t borne = (uint64_t) T[wordSize];

    while (euclideanRemainder >= borne) {
        euclideanRemainder -= borne;
        borne = (uint64_t) T[++wordSize];
    }

    wordSize += CFG.getTailleMin();
    for (int i = 0; i < wordSize; ++i) {
        int letterIndex = euclideanRemainder % CFG.getAlphabet().size();
        euclideanRemainder = (uint64_t) euclideanRemainder / CFG.getAlphabet().size();
        clearText = CFG.getAlphabet().at(letterIndex) + clearText;
    }
    return clearText;
}

uint64_t Utility::h2i(Config &CFG, byte* empreinte, int t)
{
    uint64_t* ptr = (uint64_t*) empreinte;
    uint64_t y = ptr[0];
    return (y + t) % CFG.getN();
}

uint64_t Utility::i2i(Config &CFG, uint64_t index, int t)
{
    byte *empreinte = (byte *) malloc(sizeof(byte) * MD5_DIGEST_LENGTH);
    CFG.hash(i2c(CFG, index).c_str(), empreinte);
    return h2i(CFG, empreinte, t);
}

uint64_t Utility::nouvelle_chaine(Config &CFG, uint64_t idx1, int largeur)
{
    uint64_t curIdx = idx1;
    for (int i = 1; i < largeur; ++i) {
        curIdx = i2i(CFG,curIdx, i);
    }
    return curIdx;
}

uint64_t Utility::index_aleatoire()
{
    uint64_t randRes = rand();
    randRes = randRes<<32;
    randRes += rand();
    return randRes;
}

int Utility::recherche(std::map<uint64_t, std::vector<uint64_t>> &table, uint64_t idx, int &a, int &b) 
{
    std::cout idx;
    auto it = table.find(251367);
    if (it == table.end()) {
        return 0;
    }
    std::cout << "woow" << std::endl;
    for (auto &val : it->second) { 
        std::cout << val << ", "; 
    }
    std::cout << std::endl;
    a = 0;
    b = 0;
    return 1;
}

int Utility::inverse(Config &CFG, std::map<uint64_t, std::vector<uint64_t>> &table, int largeur, int hauteur, byte* h, std::string clair) 
{
    int nb_candidats = 0;
    (void) nb_candidats;
    (void) clair;
    (void) hauteur;
    for (int t = largeur - 1; t > 0; t--) {
        uint64_t idx = h2i(CFG, h, t);
        for (int i = t + 1; i < largeur; i++) {
            idx = i2i(CFG, idx, i);
        }
        int a, b;
        if (recherche(table, idx, a, b) > 0) {
            std::cout << "found" << std::endl;
        }
    }
    return 1;
}

void Utility::hash_MD5(const char *s, byte *empreinte)
{
    MD5((unsigned char *) s, strlen(s), empreinte);
}

void Utility::hash_SHA1(const char *s, byte *empreinte)
{
    SHA1((unsigned char *) s, strlen(s), empreinte);
}


}
