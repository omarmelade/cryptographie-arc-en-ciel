- KHIAL Omar
- LAFFONT Léo

# TP1 Cryptologie

## Compte-rendu

Pour build le TP :
```
make
```
Les commandes associées aux questions seront mises une à une dans ce compte-rendu.

### Question 1

Écriture de la fonction `std::string empreinteToString(byte *empreinte)`.

### Question 2

Gestion des arguments à l'appel du programme.
```
./build/main.out --md5 test hash Salut Bob
./build/main.out --sha1 test hash Salut Bob
```

### Question 3

Écriture de la fonction `i2c`.
```
./build/main.out -s 4 -A 26A test i2c 1234 197437
./build/main.out -m 1 -M 6 -A 52 test i2c 150106454 75324 1651 4173921
```

### Question 4 (Bonus)

Pas répondu pour l'instant.

### Question 5

Écriture de la fonction `h2i`.
```
./build/main.out -m 4 -M 5 -A 26 test h2i oups
```

### Question 6

Écriture de la fonction `h2i`.
```
./build/main.out -m 4 -M 5 -A 52 test nc 1 1 2 10 100 1000
```

### Question 7 

Le paramètre `t` sert à éviter les collisions dans la table arc en ciel 🌈.

### Question 8 

Écriture de la fonction `index_aléatoire` et `creer_table`.
```
./build/main.out -s 5 -A 26 test rand 500
./build/main.out -s 5 -A 26 test ct 100 200
./build/main.out -s 5 -A 26 test ct-rand 100 200
```