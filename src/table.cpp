#include "table.h"
#include "utility.h"

namespace AEC {

Table::Table(Config &CFG, int largeur, int hauteur, bool test /*= false*/)
    : _CFG(CFG),
      _largeur(largeur),
      _hauteur(hauteur)
{
    if (test) {
        create_table_test();
    }
    else {
        create_table();
    }
}

Table::Table(Config &CFG)
    : _CFG(CFG),
      _largeur(0),
      _hauteur(0)
{
}

void Table::create_table_test()
{
    _table.clear();
    for (int i = 0; i < _hauteur; ++i) {
        uint64_t chaineX = Utility::nouvelle_chaine(_CFG, i, _largeur);
        _table[chaineX].push_back(i);
    }
}

void Table::create_table()
{
    _table.clear();
    for (int i = 0; i < _hauteur; ++i) {
        uint64_t idx = Utility::index_aleatoire() % _CFG.getN();
        uint64_t chaineX = Utility::nouvelle_chaine(_CFG, idx, _largeur);
        _table[chaineX].push_back(i);
    }
}

// TODO: use ifstream filename instead of FILE *f
bool Table::sauve_table(std::string filename)
{
    std::ofstream f;
    f.open (filename, std::ios::out | std::ios::binary);
    // write header in file (largeur, hauteur, alphabet, taille_min, taille_max)
    f << _largeur << " " << _hauteur << " " << _CFG.getHashFunctionName() << " " << _CFG.getAlphabet() << " " << _CFG.getCodeAlphabet() << " " << _CFG.getTailleMin() << " " << _CFG.getTailleMax() << " " << _CFG.getN() << std::endl;
    // write all map elements in bytes
    for (auto it = _table.begin(); it != _table.end(); ++it) {
        f.write((char *)&it->first, sizeof(uint64_t));
        size_t size = it->second.size();
        f.write((char *)&size, sizeof(size_t));
        for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
            uint64_t element = *it2;
            f.write((char *)&element, sizeof(uint64_t));
        }
    }
    f.close();
    return true;
}

// TODO: use ifstream filename instead of FILE *f
bool Table::ouvre_table(std::string filename)
{
    // read header in file (largeur, hauteur, alphabet, taille_min, taille_max)
    std::ifstream f;
    f.open(filename, std::ios::in | std::ios::binary);
    int largeur, hauteur, tailleMin, tailleMax;
    uint16_t n;
    std::string header, alphabet, alphabetCode, hashName;
    getline(f, header, '\n');
    std::stringstream ss(header);
    ss >> largeur >> hauteur >> hashName >> alphabet >> alphabetCode >> tailleMin >> tailleMax >> n;

    auto itFunc = Utility::hashFunctionMap.find(hashName);
    if (itFunc == Utility::hashFunctionMap.end()) { return false; }

    _CFG.setCodeAlphabet(alphabetCode);
    _CFG.setTailleMin(tailleMin);
    _CFG.setTailleMax(tailleMax);
    _CFG.setHashFunctionName(hashName);
    _CFG.setHashFunction(itFunc->second);
    _CFG.updateConfig();
    _hauteur = hauteur;
    _largeur = largeur;
    // read all map elements in bytes
    _table.clear();
    for (int i = 0; i < _hauteur; ++i) {
        std::vector<uint64_t> idx;
        uint64_t chaineX;
        size_t size;
        f.read((char *)&chaineX, sizeof(uint64_t));
        f.read((char *)&size, sizeof(size_t));
        for (size_t j = 0; j < size; ++j) {
            uint64_t tmp;
            f.read((char *)&tmp, sizeof(uint64_t));
            idx.push_back(tmp);
        }
        _table[chaineX] = idx;
    }
    f.close();
    return true;
}

void Table::affiche_table()
{
    int truncatedSize = _hauteur / 10;
    int i = 0;
    for (auto it = _table.begin(); it != _table.end(); ++it) {
        if (i < truncatedSize || i > _hauteur - truncatedSize) {
            std::cout << it->first << " --> ";
            for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
                std::cout << *it2 << " ";
            }
            std::cout << std::endl;
        }
        else if (i == truncatedSize) {
            std::cout << " ... " << std::endl;
        }
        ++i;
    }
}

}