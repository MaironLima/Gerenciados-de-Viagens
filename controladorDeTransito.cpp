#include "ControladorDeTransito.h"
#include "distancias.h"
#include <iostream>
#include <algorithm>
#include <cctype>

using namespace std;

ControladorDeTransito::~ControladorDeTransito() {
    for (auto par : cidades) delete par.second;
    for (auto t : transportes) delete t;
    for (auto p : passageiros) delete p;
    for (auto v : viagensAtivas) delete v;
}

string ControladorDeTransito::tornarMinusculo(string str) {
    transform(str.begin(), str.end(), str.begin(), [](unsigned char c){ return tolower(c); });
    return str;
}

string ControladorDeTransito::formatarCapitalizar(string str) const {
    if (str.empty()) return str;
    str[0] = toupper(str[0]);
    return str;
}

Cidade* ControladorDeTransito::obterOuCriarCidade(const string& nome) {
    string nF = tornarMinusculo(nome);
    if (cidades.find(nF) == cidades.end()) {
        cidades[nF] = new Cidade(nF);
    }
    return cidades[nF];
}

Transporte* ControladorDeTransito::buscarTransporte(const string& nome) {
    string nF = tornarMinusculo(nome);
    for (auto t : transportes) {
        if (tornarMinusculo(t->getNome()) == nF) return t;
    }
    return nullptr;
}

Passageiro* ControladorDeTransito::buscarPassageiro(const string& nome) {
    string nF = tornarMinusculo(nome);
    for (auto p : passageiros) {
        if (tornarMinusculo(p->getNome()) == nF) return p;
    }
    return nullptr;
}

void ControladorDeTransito::cadastrarTransporte(const string& nome, char tipo, int cap, int vel, int horasDesc, int tempoDesc, const string& cidadeInicial) {
    string nMin = tornarMinusculo(nome);
    string cMin = tornarMinusculo(cidadeInicial);
    
    if (cap <= 0) {
        cout << "Erro especifico: A capacidade do transporte nao pode ser de 0 ou menos passageiros.\n";
        return;
    }
    Cidade* cid = obterOuCriarCidade(cMin);
    
    transportes.push_back(new Transporte(nMin, toupper(tipo), cap, vel, horasDesc, tempoDesc, cid));
    cout << "Transporte '" << formatarCapitalizar(nMin) << "' registrado com sucesso em '" << formatarCapitalizar(cid->getNome()) << "'.\n";
}

void ControladorDeTransito::cadastrarPassageiro(const string& nome, const string& cidadeInicial) {
    string nMin = tornarMinusculo(nome);
    string cMin = tornarMinusculo(cidadeInicial);
    
    Cidade* cid = obterOuCriarCidade(cMin);
    passageiros.push_back(new Passageiro(nMin, cid));
    cout << "Passageiro '" << formatarCapitalizar(nMin) << "' registrado com sucesso em '" << formatarCapitalizar(cid->getNome()) << "'.\n";
}

void ControladorDeTransito::iniciarViagem(const string& nomeTransporte, const vector<string>& nomesPassageiros, const string& origem, const string& destino) {
    string oF = tornarMinusculo(origem);
    string dF = tornarMinusculo(destino);

    if (oF == dF) {
        cout << "Erro especifico: A cidade de destino '" << formatarCapitalizar(destino) << "' e igual a de origem. Nao havera deslocamento.\n";
        return;
    }

    Transporte* veiculo = buscarTransporte(nomeTransporte);
    if (!veiculo) {
        cout << "Erro especifico: O veiculo '" << formatarCapitalizar(nomeTransporte) << "' nao foi encontrado no sistema.\n";
        return;
    }

    if (veiculo->isEmTransito()) {
        cout << "Erro especifico: O veiculo '" << formatarCapitalizar(nomeTransporte) << "' ja esta em transito.\n";
        return;
    }

    Cidade* cidOrigem = obterOuCriarCidade(oF);
    if (veiculo->getLocalAtual() != cidOrigem) {
        cout << "Erro especifico: O veiculo '" << formatarCapitalizar(nomeTransporte) << "' nao esta na origem '" << formatarCapitalizar(origem) << "'.\n";
        return;
    }

    Cidade* cidDestino = obterOuCriarCidade(dF);

    vector<Passageiro*> alocados;
    for (const auto& n : nomesPassageiros) {
        Passageiro* pass = buscarPassageiro(n);
        if (!pass) {
            cout << "Erro especifico: O passageiro '" << formatarCapitalizar(n) << "' nao esta cadastrado.\n";
            return;
        }
        if (pass->isEmTransito() || pass->getLocalAtual() != cidOrigem) {
            cout << "Erro especifico: O passageiro '" << formatarCapitalizar(n) << "' nao esta disponivel em '" << formatarCapitalizar(origem) << "'.\n";
            return;
        }
        alocados.push_back(pass);
    }

    // BUSCA DE DISTÂNCIA DINÂMICA BASEADA NO TIPO DE VEÍCULO
    int km = getDistancia(oF, dF, veiculo->getTipo());
    if (km < 0) {
        cout << "Erro especifico: Nao existe rota cadastrada conectando as capitais.\n";
        return;
    }

    Viagem* novaViagem = new Viagem(veiculo, alocados, cidOrigem, cidDestino, km);
    viagensAtivas.push_back(novaViagem);
    cout << "\n>>> VIAGEM INICIADA COM SUCESSO! <<<\n";
}

void ControladorDeTransito::avancarTempo(int horas) {
    if (horas <= 0) return;
    cout << "\n--- AVANCANDO " << horas << " HORA(S) NO SISTEMA ---\n";

    for (int h = 0; h < horas; ++h) {
        for (auto it = viagensAtivas.begin(); it != viagensAtivas.end(); ) {
            Viagem* v = *it;
            bool concluida = v->avancarUmaHora();
            
            if (concluida) {
                cout << "\n=========================================\n";
                cout << "     [VIAGEM CONCLUIDA COM SUCESSO]       \n";
                cout << "=========================================\n";
                cout << "Origem: " << formatarCapitalizar(v->getOrigem()->getNome()) << "\n";
                cout << "Destino: " << formatarCapitalizar(v->getDestino()->getNome()) << "\n";
                cout << "Veiculo: " << formatarCapitalizar(v->getVeiculo()->getNome()) << "\n";
                cout << "Velocidade Media: " << v->getVeiculo()->getVelocidade() << " km/h\n";
                cout << "Tempo total de deslocamento: " << v->getHorasPassadas() << " hora(s)\n";
                cout << "Regra de Pausa: A cada " << v->getVeiculo()->getHorasEntreDescansos() 
                     << " hora(s) para por " << v->getVeiculo()->getTempoDescanso() << " hora(s).\n";
                cout << "Passageiros de desembarque:\n";
                for (auto p : v->getPassageiros()) {
                    cout << " - " << formatarCapitalizar(p->getNome()) << "\n";
                }
                cout << "=========================================\n";
                
                delete v;
                it = viagensAtivas.erase(it);
            } else {
                ++it;
            }
        }
    }
}

void ControladorDeTransito::mostrarViagensEmAndamento() const {
    cout << "\n--- VIAGENS EM ANDAMENTO (" << viagensAtivas.size() << ") ---\n";
    if (viagensAtivas.empty()) {
        cout << "Nenhuma viagem sendo executada no momento.\n";
        return;
    }
    for (auto v : viagensAtivas) {
        v->exibirEstado();
        cout << "-----------------------------------------\n";
    }
}

void ControladorDeTransito::relatarLocalizacaoPassageiros() const {
    cout << "\n--- LOCALIZACAO DE PASSAGEIROS ---\n";
    if (passageiros.empty()) {
        cout << "Nenhum passageiro cadastrado no sistema.\n";
        return;
    }
    for (auto p : passageiros) {
        cout << " - Passageiro: " << formatarCapitalizar(p->getNome()) << " | Estado: ";
        if (p->isEmTransito()) {
            cout << "Em transito (Viajando)\n";
        } else {
            cout << "Estacionado em '" << formatarCapitalizar(p->getLocalAtual()->getNome()) << "'\n";
        }
    }
}

void ControladorDeTransito::relatarLocalizacaoTransportes() const {
    cout << "\n--- LOCALIZACAO DE TRANSPORTES ---\n";
    if (transportes.empty()) {
        cout << "Nenhum transporte cadastrado no sistema.\n";
        return;
    }
    for (auto t : transportes) {
        cout << " - Veiculo: " << formatarCapitalizar(t->getNome()) << " [" << t->getTipo() << "] | Estado: ";
        if (t->isEmTransito()) {
            cout << "Em transito (Em rota)\n";
        } else {
            cout << "Disponivel em '" << formatarCapitalizar(t->getLocalAtual()->getNome()) << "'\n";
        }
    }
}