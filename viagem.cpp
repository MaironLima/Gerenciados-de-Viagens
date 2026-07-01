#include "Viagem.h"
#include <iostream>
#include <cmath>

using namespace std;

Viagem::Viagem(Transporte* v, vector<Passageiro*> p, Cidade* o, Cidade* d, int distTotal)
    : veiculo(v), passageiros(p), origem(o), destino(d), 
    distanciaTotal(distTotal), kmPercorridos(0), kmDesdeUltimoDescanso(0), 
    horasRestantesDescanso(0), horasPassadas(0) 
{
    // Ativa trânsito
    veiculo->setEmTransito(true);
    veiculo->setLocalAtual(nullptr);
    for(auto pass : passageiros) {
        pass->setEmTransito(true);
        pass->setLocalAtual(nullptr);
    }

    // Cálculo da estimativa inicial de tempo em horas baseado nas regras de descanso
    int tempoMovimento = ceil((double)distanciaTotal / veiculo->getVelocidade());
    int paradas = 0;
    if (veiculo->getDistEntreDescansos() > 0 && veiculo->getTempoDescanso() > 0) {
        // Quantas vezes o limite de km para descanso é atingido antes de chegar ao fim
        int kmAcumulado = 0;
        for (int h = 1; h <= tempoMovimento; ++h) {
            kmAcumulado += veiculo->getVelocidade();
            if (kmAcumulado >= distanciaTotal) break;
            
            if (h * veiculo->getVelocidade() >= veiculo->getDistEntreDescansos()) {
                paradas++;
                kmAcumulado = 0; 
            }
        }
    }
    tempoTotalEstimado = tempoMovimento + (paradas * veiculo->getTempoDescanso());
}

bool Viagem::avancarUmaHora() {
    horasPassadas++;

    // Se está em tempo de descanso
    if (horasRestantesDescanso > 0) {
        horasRestantesDescanso--;
        return false;
    }

    // Deslocamento
    kmPercorridos += veiculo->getVelocidade();
    kmDesdeUltimoDescanso += veiculo->getVelocidade();

    // Se chegou ao destino
    if (kmPercorridos >= distanciaTotal) {
        veiculo->setEmTransito(false);
        veiculo->setLocalAtual(destino);
        for(auto pass : passageiros) {
            pass->setEmTransito(false);
            pass->setLocalAtual(destino);
        }
        return true; 
    }

    // Verifica se precisa parar para descansar
    if (veiculo->getDistEntreDescansos() > 0 && kmDesdeUltimoDescanso >= veiculo->getDistEntreDescansos()) {
        horasRestantesDescanso = veiculo->getTempoDescanso();
        kmDesdeUltimoDescanso = 0;
    }

    return false;
}

void Viagem::exibirEstado() const {
    int horasFaltando = (tempoTotalEstimado - horasPassadas > 0) ? (tempoTotalEstimado - horasPassadas) : 0;
    
    cout << " - Veiculo: " << veiculo->getNome() << " [" << origem->getNome() << " -> " << destino->getNome() << "]\n";
    cout << "   Progresso: " << kmPercorridos << "km / " << distanciaTotal << "km\n";
    if (horasRestantesDescanso > 0) {
        cout << "   Status Atual: DESCANSANDO (Restam " << horasRestantesDescanso << "h de pausa)\n";
    } else {
        cout << "   Status Atual: EM DESLOCAMENTO (Velocidade: " << veiculo->getVelocidade() << " km/h)\n";
    }
    cout << "   Horas estimadas restantes para chegada: " << horasFaltando << "h\n";
}