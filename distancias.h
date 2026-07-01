#ifndef DISTANCIAS_H
#define DISTANCIAS_H

#include <string>
#include <unordered_map>

using namespace std;

extern unordered_map<string, unordered_map<string, int>> distancias;

int getDistancia(const string& origem, const string& destino);
char getTipoTrajetoNecessario(const string& origem, const string& destino);

#endif