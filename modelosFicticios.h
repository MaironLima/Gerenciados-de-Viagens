#ifndef MODELOSFICTICIOS_H
#define MODELOSFICTICIOS_H

#include <string>
#include <vector>

using namespace std;

class CidadeFicticia {
private:
    string nome;
public:
    CidadeFicticia(string n);
    string getNome() const;
};

class TrajetoFicticio {
private:
    string origem;
    string destino;
    string tipo; 
    int distancia;
public:
    TrajetoFicticio(string o, string d, string t, int dist);
    string getOrigem() const;
    string getDestino() const;
    string getTipo() const;
    int getDistancia() const;
};

class TransporteFicticio {
private:
    string nome;
    string tipo; 
    int capacidade;
    int velocidade;
    int distEntreDescansos;
    int tempoDescanso;
    string localAtual;
    bool emTransito;
public:
    TransporteFicticio(string n, string t, int cap, int vel, int distDesc, int tempDesc, string localIni, bool transito = false);
    string getNome() const;
    string getTipo() const;
    int getCapacidade() const;
    int getVelocidade() const;
    int getDistEntreDescansos() const;
    int getTempoDescanso() const;
    string getLocalAtual() const;
    void setLocalAtual(string local);
    bool isEmTransito() const;
    void setEmTransito(bool status);
};

class PassageiroFicticio {
private:
    string nome;
    string localAtual;
    bool emTransito;
public:
    PassageiroFicticio(string n, string localIni, bool transito = false);
    string getNome() const;
    string getLocalAtual() const;
    void setLocalAtual(string local);
    bool isEmTransito() const;
    void setEmTransito(bool status);
};

class ViagemFicticia {
public:
    TransporteFicticio* veiculo;
    vector<PassageiroFicticio*> passageiros;
    string origem;
    string destino;
    vector<string> rotaCompleta;
    int distanciaTotal;
    int kmPercorridos;
    int horasPassadas;
    bool concluida;

    ViagemFicticia(TransporteFicticio* v, vector<PassageiroFicticio*> p, string o, string d, const vector<string>& rota, int distTotal, int kmPerc = 0, int hPass = 0);
    void avancarHora();
};

#endif