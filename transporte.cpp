#include "Transporte.h"

using namespace std;

Transporte::Transporte(string nome, char tipo, int capacidade, int velocidade, int distEntreDescansos, int tempoDescanso, Cidade* localInicial)
    : nome(nome), tipo(tipo), capacidade(capacidade), velocidade(velocidade), 
      distEntreDescansos(distEntreDescansos), tempoDescanso(tempoDescanso), localAtual(localInicial), emTransito(false) {}

string Transporte::getNome() const { return nome; }
char Transporte::getTipo() const { return tipo; }
int Transporte::getCapacidade() const { return capacidade; }
int Transporte::getVelocidade() const { return velocidade; }
int Transporte::getDistEntreDescansos() const { return distEntreDescansos; }
int Transporte::getTempoDescanso() const { return tempoDescanso; }
Cidade* Transporte::getLocalAtual() const { return localAtual; }
void Transporte::setLocalAtual(Cidade* local) { localAtual = local; }
bool Transporte::isEmTransito() const { return emTransito; }
void Transporte::setEmTransito(bool estado) { emTransito = estado; }