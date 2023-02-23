#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <map>

#include <getopt.h>
#include <openssl/md5.h>
#include <openssl/sha.h>

#include "utility.h"

namespace AEC {

void help() {
    std::cout << "Mauvaise commande : se référer au readme." << std::endl;
}

std::map<int64_t, int64_t> create_table_test(Config &CFG, int largeur, int hauteur) {
    std::map<int64_t, int64_t> table;
    for (int i = 0; i < hauteur; ++i) {
        int64_t chaineX = Utility::nouvelle_chaine(CFG, i, largeur);
        table[chaineX] = i;
    }
    return table;
}

std::map<int64_t, int64_t> create_table(Config &CFG, int largeur, int hauteur) {
    std::map<int64_t, int64_t> table;
    for (int i = 0; i < hauteur; ++i) {
        int64_t idx = Utility::index_aleatoire() % CFG.getN();
        int64_t chaineX = Utility::nouvelle_chaine(CFG, idx, largeur);
        table[chaineX] = idx;
    }
    return table;
}
/*
void sauve_table(Config &CFG, std::map<int64_t, int64_t> &table, const char *filename)
{
// write header in file (largeur, hauteur, alphabet, taille_min, taille_max)
FILE *f = fopen(filename, "wb", 0);
fprintf(f, "%d %d %s %d %d", CFG.getLargeur(), CFG.getHauteur(), CFG.getAlphabet().c_str(), CFG.getTailleMin(), CFG.getTailleMax());
// write all map elements in bytes
for (auto it = table.begin(); it != table.end(); ++it) {
    fwrite(&it->first, sizeof(int64_t), 1, f);
    fwrite(&it->second, sizeof(int64_t), 1, f);
}
fclose(f);
}

void ouvre_table()
{

}
*/
int main_test(Config &CFG, int argc, char *argv[]) {
    std::cout << std::endl;
    if (0 == strcmp(argv[0], "hash")) {
        // Test hash;
        for (int i = 1; i < argc; ++i) {
            byte *empreinte;
            char *data = argv[i];
            empreinte = (byte *) malloc(sizeof(byte) * MD5_DIGEST_LENGTH);
            std::cout << Utility::empreinteToString(CFG.hash(data, empreinte)) << " (" << data << ")" << std::endl;
        }
    } else if (0 == strcmp(argv[0], "i2c")) {
        if (argc < 2) {
            help();
            return 0;
        }
        for (int i = 1; i < argc; ++i) {
            if (atoll(argv[i]) >= CFG.getN()) {
                std::cout << "i2c(" << argv[i] << ")"
                          << " = "
                          << "OUT OF BOUNDS" << std::endl;
                continue;
            }
            std::cout << "i2c(" << argv[i] << ")"
                      << " = " << Utility::i2c(CFG, atoll(argv[i])) << std::endl;
        }
    } else if (0 == strcmp(argv[0], "h2i")) {
        if (argc < 2) {
            help();
            return 0;
        }
        int t = argc == 3 ? atoi(argv[2]) : 1;

        byte *empreinte;
        char *data = argv[1];
        empreinte = (byte *) malloc(sizeof(byte) * MD5_DIGEST_LENGTH);
        CFG.hash(data, empreinte);
        std::cout << CFG.getHashFunctionName() << "(\"" << data << "\") = " << Utility::empreinteToString(empreinte) << std::endl;
        std::cout << "h2i(" << CFG.getHashFunctionName() << "(\"" << data << "\")"
                  << ", " << t << ")"
                  << " = " << Utility::h2i(CFG, empreinte, t) << std::endl;
    } else if (0 == strcmp(argv[0], "nc")) {
        if (argc < 3) {
            help();
            return 0;
        }
        int64_t idx1 = atoll(argv[1]);
        for (int i = 2; i < argc; ++i) {
            int largeur = atoi(argv[i]);
            std::cout << "chaine de taille " << largeur << " : " << idx1 << " ... " <<  Utility::nouvelle_chaine(CFG, idx1, largeur) << std::endl;
        }
    } else if (0 == strcmp(argv[0], "ct")) {
        if (argc < 3) {
            help();
            return 0;
        }
        int64_t hauteur = atoll(argv[1]);
        int64_t largeur = atoll(argv[2]);
        auto tablearc = create_table_test(CFG, largeur, hauteur);
        for (auto &t: tablearc) {
            std::cout << t.second << " --> " << t.first << std::endl;
        }
    } else if (0 == strcmp(argv[0], "ct-rand")) {
        if (argc < 3) {
            help();
            return 0;
        }
        int64_t hauteur = atoll(argv[1]);
        int64_t largeur = atoll(argv[2]);
        auto tablearc = create_table(CFG, largeur, hauteur);
        for (auto &t: tablearc) {
            std::cout << t.second << " --> " << t.first << std::endl;
        }
    } else if (0 == strcmp(argv[0], "rand")) {
        if (argc < 2) {
            help();
            return 0;
        }
        for (int i = 0; i < atoi(argv[1]); ++i) {
            std::cout << Utility::index_aleatoire() << std::endl;
        }
    }
    return 0;
}

} // namespace AEC

int main(int argc, char *argv[])
{
    srand(time(nullptr));

    if (argc == 1) {
        AEC::help();
        return 1;
    }

    char short_options[] = "A:s:m:M:";
    static struct option long_options[] = {
            {"md5", no_argument, 0, 0},
            {"sha1", no_argument, 0, 0},
            {"abc", required_argument, 0, 'A'},
            {"size", required_argument, 0, 's'},
            {"min-size", required_argument, 0, 'm'},
            {"max-size", required_argument, 0, 'M'},
            {0, 0, 0, 0}};

    std::pair<std::string, std::function<void(const char *, AEC::byte *)>> hashFunc("md5", &AEC::Utility::hash_MD5);
    std::string alphabetCode = "26";
    int minSize = 1;
    int maxSize = 4;

    int opt;
    int long_index;
    while ((opt = getopt_long(argc, argv, short_options, long_options, &long_index)) != -1) {
        switch (opt) {
            case 0:
                if (0 == strcmp("md5", long_options[long_index].name)) {
                    hashFunc = {"md5", &AEC::Utility::hash_MD5};
                } else if (0 == strcmp("sha1", long_options[long_index].name)) {
                    hashFunc = {"sha1", &AEC::Utility::hash_SHA1};
                } else {
                    std::cout << "Unknown option..." << std::endl;
                    return 1;
                }
                break;
            case 'A':
                alphabetCode = optarg;
                break;
            case 'm':
                minSize = atoi(optarg);
                break;
            case 'M':
                maxSize = atoi(optarg);
                break;
            case 's':
                minSize = atoi(optarg);
                maxSize = atoi(optarg);
                break;
            default: /* '?' */
                // TODO
                return 1;
        }
    }

    // Init configuration
    auto CFG = AEC::Config(hashFunc, alphabetCode, minSize, maxSize);

    // Print config
    std::cout << "fonction de hash = " << CFG.getHashFunctionName() << std::endl;
    std::cout << "alphabet = \"" << CFG.getAlphabet() << "\"" << std::endl;
    std::cout << "minSize = " << CFG.getTailleMin() << std::endl;
    std::cout << "maxSize = " << CFG.getTailleMax() << std::endl;
    std::cout << "N = " << CFG.getN() << std::endl;

    if (argc <= optind) {
        return 0;
    } else if (0 == strcmp(argv[optind], "test")) {
        AEC::main_test(CFG, argc - optind - 1, argv + optind + 1);
        return 0;
    } else {
        AEC::help();
        return 2;
    }
}