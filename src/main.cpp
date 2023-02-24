#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>
#include <sstream>

#include <getopt.h>
#include <openssl/md5.h>
#include <openssl/sha.h>

#include "utility.h"
#include "table.h"

namespace AEC {

void help() {
    std::cout << "Mauvaise commande : se référer au readme." << std::endl;
}

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
            if (std::strtoull(argv[i], nullptr, 0) >= CFG.getN()) {
                std::cout << "i2c(" << argv[i] << ")"
                          << " = "
                          << "OUT OF BOUNDS" << std::endl;
                continue;
            }
            std::cout << "i2c(" << argv[i] << ")"
                      << " = " << Utility::i2c(CFG, strtoull(argv[i], nullptr, 0)) << std::endl;
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
        uint64_t idx1 = std::strtoull(argv[1], nullptr, 0);
        for (int i = 2; i < argc; ++i) {
            int largeur = atoi(argv[i]);
            std::cout << "chaine de taille " << largeur << " : " << idx1 << " ... " <<  Utility::nouvelle_chaine(CFG, idx1, largeur) << std::endl;
        }
    } else if (0 == strcmp(argv[0], "ct")) {
        if (argc < 3) {
            help();
            return 0;
        }
        uint64_t hauteur = std::strtoull(argv[1], nullptr, 0);
        uint64_t largeur = std::strtoull(argv[2], nullptr, 0);
        Table tablearc(CFG, largeur, hauteur, true);
        tablearc.affiche_table();
        if (argc == 5 && 0 == strcmp(argv[3], "save")) {
            std::string filename = argv[4];
            if (tablearc.sauve_table(filename)) {
                std::cout << "Table sauvegardée dans " << filename << std::endl;
            } else {
                std::cout << "Erreur lors de la sauvegarde de la table dans " << filename << std::endl;
                return 2;
            }
        }
    } else if (0 == strcmp(argv[0], "ct-rand")) {
        if (argc < 3) {
            help();
            return 0;
        }
        uint64_t hauteur = std::strtoull(argv[1], nullptr, 0);
        uint64_t largeur = std::strtoull(argv[2], nullptr, 0);
        Table tablearc(CFG, largeur, hauteur);
        tablearc.affiche_table();
    } else if (0 == strcmp(argv[0], "ct-load")) {
        if (argc < 2) {
            help();
            return 0;
        }
        std::string filename = argv[1];
        Table tablearc(CFG);
        if (tablearc.ouvre_table(filename)) {
            std::cout << "Table chargée depuis " << filename << std::endl;
        } else {
            std::cout << "Erreur lors du chargement de la table depuis " << filename << std::endl;
            return 2;
        }
        tablearc.affiche_table();
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

    auto hashFunc = AEC::Utility::hashFunctionMap.find("md5");
    std::string alphabetCode = "26";
    int minSize = 1;
    int maxSize = 4;

    int opt;
    int long_index;
    while ((opt = getopt_long(argc, argv, short_options, long_options, &long_index)) != -1) {
        switch (opt) {
            case 0:
                if (0 == strcmp("md5", long_options[long_index].name)) {
                    hashFunc = AEC::Utility::hashFunctionMap.find("md5");
                } else if (0 == strcmp("sha1", long_options[long_index].name)) {
                    hashFunc = AEC::Utility::hashFunctionMap.find("sha1");
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
    auto CFG = AEC::Config(hashFunc->first, hashFunc->second, alphabetCode, minSize, maxSize);

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