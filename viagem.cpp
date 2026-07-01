#include "Viagem.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <cctype>

using namespace std;

string Viagem::formatarCapitalizar(string str) const {
    if (str.empty()) return str;
    str[0] = toupper(str[0]);
    return str;
}

Viagem::Viagem(Transporte* v, vector<Passageiro*> p, Cidade* o, Cidade* d, int distTotal)
    : veiculo(v), passageiros(p), origem(o), destino(d), 
      distanciaTotal(distTotal), kmPercorridos(0), horasConsecutivasMovimento(0), 
      horasRestantesDescanso(0), horasPassadas(0) 
{
    veiculo->setEmTransito(true);
    veiculo->setLocalAtual(nullptr);
    for(auto pass : passageiros) {
        pass->setEmTransito(true);
        pass->setLocalAtual(nullptr);
    }

    int tempoMovimento = ceil((double)distanciaTotal / veiculo->getVelocidade());
    int paradas = 0;
    if (veiculo->getHorasEntreDescansos() > 0 && veiculo->getTempoDescanso() > 0) {
        int horasAcumuladas = 0;
        for (int h = 1; h < tempoMovimento; ++h) {
            horasAcumuladas++;
            if (horasAcumuladas >= veiculo->getHorasEntreDescansos()) {
                paradas++;
                horasAcumuladas = 0;
            }
        }
    }
    tempoTotalEstimado = tempoMovimento + (paradas * veiculo->getTempoDescanso());
}

bool Viagem::avancarUmaHora() {
    horasPassadas++;

    if (horasRestantesDescanso > 0) {
        horasRestantesDescanso--;
        if (horasRestantesDescanso == 0) {
            horasConsecutivasMovimento = 0; 
        }
        return false;
    }

    kmPercorridos += veiculo->getVelocidade();
    horasConsecutivasMovimento++;

    if (kmPercorridos >= distanciaTotal) {
        veiculo->setEmTransito(false);
        veiculo->setLocalAtual(destino);
        for(auto pass : passageiros) {
            pass->setEmTransito(false);
            pass->setLocalAtual(destino);
        }
        return true; 
    }

    if (veiculo->getHorasEntreDescansos() > 0 && horasConsecutivasMovimento >= veiculo->getHorasEntreDescansos()) {
        horasRestantesDescanso = veiculo->getTempoDescanso();
    }

    return false;
}

void Viagem::exibirEstado() const {
    int horasFaltando = (tempoTotalEstimado - horasPassadas > 0) ? (tempoTotalEstimado - horasPassadas) : 0;
    
    cout << " - Veiculo: " << formatarCapitalizar(veiculo->getNome()) << " [" << formatarCapitalizar(origem->getNome()) << " -> " << formatarCapitalizar(destino->getNome()) << "]\n";
    cout << "   Progresso: " << (kmPercorridos > distanciaTotal ? distanciaTotal : kmPercorridos) << "km / " << distanciaTotal << "km\n";
    
    if (horasRestantesDescanso > 0) {
        cout << "   Status Atual: DESCANSANDO (Restam " << horasRestantesDescanso << "h de pausa)\n";
        cout << "   Tempo ate a proxima parada: 0h (Ja esta parado)\n";
    } else {
        cout << "   Status Atual: EM DESLOCAMENTO\n";
        if (veiculo->getHorasEntreDescansos() > 0) {
            int tempoAteParar = veiculo->getHorasEntreDescansos() - horasConsecutivasMovimento;
            cout << "   Tempo ate a proxima parada: " << (tempoAteParar > 0 ? tempoAteParar : 0) << "h\n";
        } else {
            cout << "   Tempo ate a proxima parada: Nao para (Direto)\n";
        }
    }
    
    cout << "   Horas estimadas restantes para chegada: " << horasFaltando << "h\n";
    cout << "   Passageiros a bordo:\n";
    for (auto p : passageiros) {
        cout << "     * " << formatarCapitalizar(p->getNome()) << "\n";
    }
}