#pragma once

#include "config.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>


namespace AEC {

typedef unsigned char byte;

class Utility
{
public:

    static std::string empreinteToString(byte *empreinte);
    static std::string i2c(Config &CFG, uint64_t index);
    static uint64_t h2i(Config &CFG, byte* empreinte, int t);
    static uint64_t i2i(Config &CFG, uint64_t index, int t);
    static uint64_t nouvelle_chaine(Config &CFG, uint64_t idx1, int largeur);
    static uint64_t index_aleatoire();
    static void hash_MD5(const char *s, byte *empreinte);
    static void hash_SHA1(const char *s, byte *empreinte);

    inline static const std::unordered_map<std::string, std::function<void(const char *, AEC::byte *)>> hashFunctionMap {
            {"md5", &hash_MD5}, {"sha1", &hash_SHA1}
    };
};

}

