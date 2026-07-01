#include "ModelosFicticios.h"
#include <iostream>

CidadeFicticia::CidadeFicticia(string n) : nome(n) {}
string CidadeFicticia::getNome() const { return nome; }

TrajetoFicticio::TrajetoFicticio(string o, string d, string t, int dist) : origem(o), destino(d), tipo(t), distancia(dist) {}
string TrajetoFicticio::getOrigem() const { return origem; }
string TrajetoFicticio::getDestino() const { return destino; }
string TrajetoFicticio::getTipo() const { return tipo; }
int TrajetoFicticio::getDistancia() const { return distancia; }

TransporteFicticio::TransporteFicticio(string n, string t, int cap, int vel, int distDesc, int tempDesc, string localIni, bool transito)
    : nome(n), tipo(t), capacidade(cap), velocidade(vel), distEntreDescansos(distDesc), tempoDescanso(tempDesc), localAtual(localIni), emTransito(transito) {}
string TransporteFicticio::getNome() const { return nome; }
string TransporteFicticio::getTipo() const { return tipo; }
int TransporteFicticio::getCapacidade() const { return capacidade; }
int TransporteFicticio::getVelocidade() const { return velocidade; }
int TransporteFicticio::getDistEntreDescansos() const { return distEntreDescansos; }
int TransporteFicticio::getTempoDescanso() const { return tempoDescanso; }
string TransporteFicticio::getLocalAtual() const { return localAtual; }
void TransporteFicticio::setLocalAtual(string local) { localAtual = local; }
bool TransporteFicticio::isEmTransito() const { return emTransito; }
void TransporteFicticio::setEmTransito(bool status) { emTransito = status; }

PassageiroFicticio::PassageiroFicticio(string n, string localIni, bool transito) : nome(n), localAtual(localIni), emTransito(transito) {}
string PassageiroFicticio::getNome() const { return nome; }
string PassageiroFicticio::getLocalAtual() const { return localAtual; }
void PassageiroFicticio::setLocalAtual(string local) { localAtual = local; }
bool PassageiroFicticio::isEmTransito() const { return emTransito; }
void PassageiroFicticio::setEmTransito(bool status) { emTransito = status; }

ViagemFicticia::ViagemFicticia(TransporteFicticio* v, vector<PassageiroFicticio*> p, string o, string d, const vector<string>& rota, int distTotal, int kmPerc, int hPass)
    : veiculo(v), passageiros(p), origem(o), destino(d), rotaCompleta(rota), distanciaTotal(distTotal), kmPercorridos(kmPerc), horasPassadas(hPass), concluida(false) {}

void ViagemFicticia::avancarHora() {
    if (concluida) return;
    horasPassadas++;
    kmPercorridos += veiculo->getVelocidade();
    
    if (kmPercorridos >= distanciaTotal) {
        kmPercorridos = distanciaTotal;
        concluida = true;
        veiculo->setEmTransito(false);
        veiculo->setLocalAtual(destino);
        for (auto p : passageiros) {
            p->setEmTransito(false);
            p->setLocalAtual(destino);
        }
        cout << "\n>>> CHEGADA: O transporte \"" << veiculo->getNome() << "\" chegou ao destino final: " << destino << "!\n";
    }
}