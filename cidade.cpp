#include "Cidade.h"

using namespace std;

Cidade::Cidade(string nome) : nome(nome) {}

string Cidade::getNome() const { 
    return nome; 
}