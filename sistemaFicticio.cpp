#include "SistemaFicticio.h"
#include <iostream>
#include <algorithm>

using namespace std;

CidadeFicticia::CidadeFicticia(string n) : nome(n) {}
string CidadeFicticia::getNome() const { return nome; }

TrajetoFicticio::TrajetoFicticio(string o, string d, string t, int dist) : origem(o), destino(d), tipo(t), distancia(dist) {}
string TrajetoFicticio::getOrigem() const { return origem; }
string TrajetoFicticio::getDestino() const { return destino; }
string TrajetoFicticio::getTipo() const { return tipo; }
int TrajetoFicticio::getDistancia() const { return distancia; }

TransporteFicticio::TransporteFicticio(string n, string t, int cap, int vel, int distDesc, int tempDesc, string localIni)
    : nome(n), tipo(t), capacidade(cap), velocidade(vel), distEntreDescansos(distDesc), tempoDescanso(tempDesc), localAtual(localIni), emTransito(false) {}
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

PassageiroFicticio::PassageiroFicticio(string n, string localIni) : nome(n), localAtual(localIni), emTransito(false) {}
string PassageiroFicticio::getNome() const { return nome; }
string PassageiroFicticio::getLocalAtual() const { return localAtual; }
void PassageiroFicticio::setLocalAtual(string local) { localAtual = local; }
bool PassageiroFicticio::isEmTransito() const { return emTransito; }
void PassageiroFicticio::setEmTransito(bool status) { emTransito = status; }

ViagemFicticia::ViagemFicticia(TransporteFicticio* v, vector<PassageiroFicticio*> p, string o, string d, const vector<string>& rota, int distTotal)
    : veiculo(v), passageiros(p), origem(o), destino(d), rotaCompleta(rota), distanciaTotal(distTotal), kmPercorridos(0), horasPassadas(0), concluida(false) {}

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

// Algoritmo de Busca com rastro de caminho para transitividade
bool encontrarRotaComplexa(string atual, string destino, string tipoRequerido, const vector<TrajetoFicticio>& trajetos, vector<string>& caminho, int& distAcumulada) {
    if (atual == destino) {
        caminho.push_back(atual);
        return true;
    }
    caminho.push_back(atual);

    for (const auto& t : trajetos) {
        if (t.getOrigem() == atual && t.getTipo() == tipoRequerido) {
            if (find(caminho.begin(), caminho.end(), t.getDestino()) == caminho.end()) {
                int distOriginal = distAcumulada;
                distAcumulada += t.getDistancia();
                
                if (encontrarRotaComplexa(t.getDestino(), destino, tipoRequerido, trajetos, caminho, distAcumulada)) {
                    return true;
                }
                distAcumulada = distOriginal; 
            }
        }
    }
    caminho.pop_back();
    return false;
}

bool cidadeExiste(const vector<CidadeFicticia>& cidades, const string& nome) {
    for (const auto& c : cidades) { if (c.getNome() == nome) return true; }
    return false;
}

void executarSistemaFicticio() {
    vector<CidadeFicticia> cidadesCadastradas;
    vector<TrajetoFicticio> trajetosCadastrados;
    vector<TransporteFicticio> transportesCadastrados;
    vector<PassageiroFicticio> passageirosCadastrados;
    vector<ViagemFicticia> viagensEmAndamento;
    int opcao = -1;

    while (opcao != 0) {
        cout << "\n=========================================\n";
        cout << "   CONTROLADOR DE CIDADES CADASTRADAS\n";
        cout << "=========================================\n";
        cout << "1. Cadastrar Nova Cidade\n";
        cout << "2. Cadastrar Trajeto entre Cidades\n";
        cout << "3. Cadastrar Transporte\n";
        cout << "4. Cadastrar Passageiro\n";
        cout << "5. Iniciar Nova Viagem entre Cidades\n";
        cout << "6. Mostrar Cidades Cadastradas\n";
        cout << "7. Mostrar Todos os Passageiros Cadastrados\n";
        cout << "8. Mostrar Todos os Trajetos Cadastrados\n";
        cout << "9. Mostrar Viagens em Andamento\n";
        cout << "10. Avancar Tempo (Horas)\n"; // Avançar tempo realocado para a última opção ativa
        cout << "0. Sair\n";
        cout << "Escolha uma opcao: ";
        
        if (!(cin >> opcao)) { cin.clear(); cin.ignore(10000, '\n'); continue; }
        cin.ignore(10000, '\n'); 

        switch (opcao) {
            case 1: {
                string nomeCidade; cout << "\nDigite o nome da cidade: "; getline(cin, nomeCidade);
                if (!nomeCidade.empty() && !cidadeExiste(cidadesCadastradas, nomeCidade)) {
                    cidadesCadastradas.push_back(CidadeFicticia(nomeCidade));
                    cout << "Cidade \"" << nomeCidade << "\" cadastrada com sucesso!\n";
                } else { cout << "Erro ou cidade ja existente.\n"; }
                break;
            }
            case 2: {
                string o, d, t; int dist;
                cout << "Origem: "; getline(cin, o); cout << "Destino: "; getline(cin, d);
                if (!cidadeExiste(cidadesCadastradas, o) || !cidadeExiste(cidadesCadastradas, d)) {
                    cout << "[ERRO] As cidades devem estar cadastradas.\n"; break;
                }
                cout << "Tipo de Trajeto (A - Aquatico / T - Terrestre): "; char tc; cin >> tc;
                t = (tc == 'A' || tc == 'a') ? "Aquatico" : "Terrestre";
                cout << "Distancia (Km): "; cin >> dist; cin.ignore(10000, '\n');
                trajetosCadastrados.push_back(TrajetoFicticio(o, d, t, dist));
                cout << "Trajeto cadastrado com sucesso!\n";
                break;
            }
            case 3: {
                string n, t, c; char tc; int cap, vel, distD, tempD = 0;
                cout << "Nome do Transporte: "; getline(cin, n);
                cout << "Tipo (A - Aquatico / T - Terrestre): "; cin >> tc;
                t = (tc == 'A' || tc == 'a') ? "Aquatico" : "Terrestre";
                cout << "Capacidade: "; cin >> cap; cout << "Velocidade (km/h): "; cin >> vel;
                
                cout << "Distancia entre descansos em Km [0 para nunca parar]: "; 
                cin >> distD;
                
                if (distD > 0) {
                    cout << "Tempo de Descanso (horas): "; cin >> tempD;
                } else {
                    tempD = 0;
                }
                cin.ignore(10000, '\n'); cout << "Cidade Inicial: "; getline(cin, c);
                
                if (!cidadeExiste(cidadesCadastradas, c)) { cout << "[ERRO] Cidade nao cadastrada.\n"; break; }
                transportesCadastrados.push_back(TransporteFicticio(n, t, cap, vel, distD, tempD, c));
                cout << "Transporte cadastrado com sucesso!\n";
                break;
            }
            case 4: {
                string n, c; cout << "Nome do Passageiro: "; getline(cin, n);
                cout << "Cidade Inicial: "; getline(cin, c);
                if (!cidadeExiste(cidadesCadastradas, c)) { cout << "[ERRO] Cidade nao cadastrada.\n"; break; }
                passageirosCadastrados.push_back(PassageiroFicticio(n, c));
                cout << "Passageiro cadastrado com sucesso!\n";
                break;
            }
            case 6:
                cout << "\n--- CIDADES CADASTRADAS --- \n";
                if(cidadesCadastradas.empty()) cout << "Nenhuma cidade no sistema.\n";
                for (size_t i = 0; i < cidadesCadastradas.size(); ++i) cout << i+1 << ". " << cidadesCadastradas[i].getNome() << "\n";
                break;
            case 5: {
                string o, d, transNome;
                cout << "Cidade de Origem: "; getline(cin, o);
                cout << "Cidade de Destino: "; getline(cin, d);
                cout << "Nome do Transporte: "; getline(cin, transNome);

                TransporteFicticio* veiculo = nullptr;
                for (auto& tr : transportesCadastrados) { if (tr.getNome() == transNome) veiculo = &tr; }

                if (!veiculo || veiculo->isEmTransito() || veiculo->getLocalAtual() != o) {
                    cout << "[ERRO] Transporte indisponivel ou nao localizado na origem.\n"; break;
                }

                vector<string> caminhoRota; int distTotalCalculada = 0;
                if (!encontrarRotaComplexa(o, d, veiculo->getTipo(), trajetosCadastrados, caminhoRota, distTotalCalculada)) {
                    cout << "[ERRO] Nao existe rota de tipo [" << veiculo->getTipo() << "] entre " << o << " e " << d << ".\n"; break;
                }

                // Exibição da transitividade se passar por mais conexões
                if (caminhoRota.size() > 2) {
                    cout << "\n[TRANSITIVIDADE DETECTADA] A rota passara por: ";
                    for(size_t i = 1; i < caminhoRota.size() - 1; ++i) {
                        cout << "\"" << caminhoRota[i] << "\" ";
                    }
                    cout << "\n";
                }

                cout << "Rota validada! Distancia total: " << distTotalCalculada << " Km.\n";
                cout << "Quantidade de passageiros (Max " << veiculo->getCapacidade() << "): ";
                int qtd; cin >> qtd; cin.ignore(10000, '\n');

                if (qtd > veiculo->getCapacidade() || qtd <= 0) { cout << "[ERRO] Capacidade incompativel.\n"; break; }

                vector<PassageiroFicticio*> selecionados;
                for (int i = 0; i < qtd; ++i) {
                    string pNome; cout << "Nome do Passageiro " << i + 1 << ": "; getline(cin, pNome);
                    PassageiroFicticio* pass = nullptr;
                    for (auto& p : passageirosCadastrados) { if (p.getNome() == pNome && p.getLocalAtual() == o && !p.isEmTransito()) pass = &p; }
                    if (pass) selecionados.push_back(pass);
                    else { cout << "[AVISO] Passageiro indisponivel na origem.\n"; break; }
                }

                if ((int)selecionados.size() != qtd) break;

                veiculo->setEmTransito(true);
                veiculo->setLocalAtual("Em transito");
                for (auto s : selecionados) { s->setEmTransito(true); s->setLocalAtual("Em transito"); }

                viagensEmAndamento.push_back(ViagemFicticia(veiculo, selecionados, o, d, caminhoRota, distTotalCalculada));
                cout << "\n>>> PARTIDA REGISTRADA!\n";
                break;
            }
            case 7:
                cout << "\n--- PASSAGEIROS --- \n";
                for (const auto& p : passageirosCadastrados) cout << "- " << p.getNome() << " (Local: " << p.getLocalAtual() << (p.isEmTransito() ? " [EM VIAGEM]" : "") << ")\n";
                break;
            case 8:
                cout << "\n--- TRAJETOS --- \n";
                for (const auto& t : trajetosCadastrados) cout << "- " << t.getOrigem() << " -> " << t.getDestino() << " [" << t.getTipo() << "] (" << t.getDistancia() << " Km)\n";
                break;
            case 9:
                cout << "\n--- VIAGENS EM ANDAMENTO --- \n";
                if (viagensEmAndamento.empty()) cout << "Nenhuma viagem ativa.\n";
                for (const auto& v : viagensEmAndamento) {
                    cout << "- " << v.veiculo->getNome() << " [" << v.origem << " -> " << v.destino << "] - Rota: ";
                    for(size_t i = 0; i < v.rotaCompleta.size(); ++i) {
                        cout << v.rotaCompleta[i] << (i == v.rotaCompleta.size()-1 ? "" : " -> ");
                    }
                    cout << " | Progresso: " << v.kmPercorridos << "/" << v.distanciaTotal << " Km\n";
                }
                break;
            case 10: {
                int horas; cout << "Avancar quantas horas? "; cin >> horas; cin.ignore(10000, '\n');
                for (int h = 0; h < horas; ++h) {
                    for (auto& v : viagensEmAndamento) v.avancarHora();
                }
                viagensEmAndamento.erase(remove_if(viagensEmAndamento.begin(), viagensEmAndamento.end(), [](const ViagemFicticia& v) { return v.concluida; }), viagensEmAndamento.end());
                break;
            }
        }
    }
}