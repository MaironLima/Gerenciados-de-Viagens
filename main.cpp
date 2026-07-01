#include <iostream>
#include <vector>
#include <string>
#include <cstdlib> 
#include "ControladorDeTransito.h"

using namespace std;

void exibirMenu() {
    cout << "\n=========================================\n";
    cout << "       SISTEMA CONTROLADOR DE VIAGENS    \n";
    cout << "=========================================\n";
    cout << "1. Cadastrar Passageiro\n";
    cout << "2. Cadastrar Transporte\n";
    cout << "3. Iniciar Nova Viagem\n";
    cout << "4. Mostrar Viagens em Andamento\n";
    cout << "5. Avancar Horas no Tempo\n";
    cout << "0. Sair\n"; 
    cout << "Escolha uma opcao: ";
}

int main() {
    ControladorDeTransito controlador;
    int opcao = -1;

    while (opcao != 0) {
        exibirMenu();
        if (!(cin >> opcao)) {
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        cin.ignore();

        switch (opcao) {
            case 1: {
                string nome, cidade;
                cout << "Nome do Passageiro: ";
                getline(cin, nome);
                cout << "Cidade de Localizacao Inicial: ";
                getline(cin, cidade);
                controlador.cadastrarPassageiro(nome, cidade);
                break;
            }
            case 2: {
                string nome, cidade;
                char tipo;
                int cap, vel, distDesc, tempoDesc;
                cout << "Nome do Transporte: ";
                getline(cin, nome);
                cout << "Tipo (T para Terrestre / A para Aereo): ";
                cin >> tipo;
                cout << "Capacidade maxima (nao pode ser 0): ";
                cin >> cap;
                cout << "Velocidade Media (km/h): ";
                cin >> vel;
                cout << "Distancia a percorrer ate precisar parar para descanso (km) [0 para nunca parar]: ";
                cin >> distDesc;
                cout << "Tempo gasto parado em cada pausa (horas): ";
                cin >> tempoDesc;
                cin.ignore();
                cout << "Cidade de Localizacao Inicial: ";
                getline(cin, cidade);
                
                controlador.cadastrarTransporte(nome, tipo, cap, vel, distDesc, tempoDesc, cidade);
                break;
            }
            case 3: {
                string t, o, d, pNome;
                int qtd;
                cout << "Nome do Transporte: ";
                getline(cin, t);
                
                Transporte* veiculo = controlador.buscarTransporte(t);
                if (!veiculo) {
                    cout << "Erro especifico: O veiculo '" << t << "' nao foi encontrado no sistema.\n";
                    break;
                }
                
                int maxCapacidade = veiculo->getCapacidade();

                cout << "Cidade de Origem: ";
                getline(cin, o);
                cout << "Cidade de Destino: ";
                getline(cin, d);
                
                cout << "Quantidade de passageiros (max: " << maxCapacidade << "): ";
                cin >> qtd;
                cin.ignore();

                if (qtd > maxCapacidade || qtd <= 0) {
                    cout << "\n[ERRO CRITICO] Quantidade de passageiros (" << qtd 
                        << ") invalida ou superior ao limite maximo do veiculo (max: " << maxCapacidade << "). Codigo abortado.\n";
                    exit(EXIT_FAILURE);
                }

                vector<string> pNomes;
                for (int i = 0; i < qtd; ++i) {
                    cout << "Nome do Passageiro " << i + 1 << ": ";
                    getline(cin, pNome);
                    pNomes.push_back(pNome);
                }
                controlador.iniciarViagem(t, pNomes, o, d);
                break;
            }
            case 4:
                controlador.mostrarViagensEmAndamento();
                break;
            case 5: {
                int h;
                cout << "Quantas horas deseja adiantar no sistema? ";
                cin >> h;
                cin.ignore();
                controlador.avancarTempo(h);
                break;
            }
            case 0:
                cout << "Encerrando o controlador...\n";
                break;
            default:
                cout << "Opcao invalida!\n";
        }
    }
    return 0;
}