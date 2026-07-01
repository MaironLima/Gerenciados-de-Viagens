#include "Passageiros.h"

using namespace std;

Passageiro::Passageiro(string nome, Cidade* localInicial) 
    : nome(nome), localAtual(localInicial), emTransito(false) {}

string Passageiro::getNome() const { return nome; }
Cidade* Passageiro::getLocalAtual() const { return localAtual; }
void Passageiro::setLocalAtual(Cidade* local) { localAtual = local; }
bool Passageiro::isEmTransito() const { return emTransito; }
void Passageiro::setEmTransito(bool estado) { emTransito = estado; }