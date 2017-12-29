#include "MonteCarlo.h"
#include "rand.h"

void MonteCarlo::init() {
	//��ʼ������Դ
	this->currentParticleNumber = this->neutronNumber;
	this->multiGroupParticleSourceBank.resize(groupNumber + 1);
	this->multiGroupNextParticleSourceBank.resize(groupNumber + 1);
	int particleNumberForEachGroup = neutronNumber / groupNumber;
	for (int i = 1; i <= groupNumber; i++) {
		for (int j = 0; j < particleNumberForEachGroup; j++) {
			//�������һ������
			//�ڼ��α߽����������λ��
			double __randPosition = random()*this->inputGeometry.geometryCell[this->cellNumber].right;
			neutron __initNeutronTemp(i, __randPosition, randomDirection(), 1.0);
			multiGroupParticleSourceBank[i].push_back(__initNeutronTemp);
		}
		//��ǰȺԴ���Ӳ������
	}
	//��ͷȫ�����ڵ�1Ⱥ��
	for (int i = 0; i < neutronNumber - particleNumberForEachGroup * groupNumber; i++) {
			double __randPosition = random()*this->inputGeometry.geometryCell[this->cellNumber].right;
			neutron __initNeutronTemp(1, __randPosition, randomDirection(), 1.0);
			multiGroupParticleSourceBank[1].push_back(__initNeutronTemp);
	}
	//Դ����ȫ���������

	//Ϊ��һά����ռ䣬��һά��ʾȺ�ţ���1��ʼ����0��Ԫ�ز���
	this->flux.resize((groupNumber+1));
	this->averageFlux.resize(groupNumber+1);
	this->accumulateFlux.resize(groupNumber+1);
	this->accumulateFluxSquare.resize(groupNumber+1);
	this->fluxStandardDeviation.resize(groupNumber+1);
	//Ϊ�ڶ�ά����ռ䣬�ڶ�ά��ʾդԪ�ţ���1��ʼ����0��Ԫ�ز���
	for (int i = 0; i <= groupNumber; i++) {
		this->flux[i].resize((cellNumber + 1));
		this->averageFlux[i].resize(cellNumber + 1);
		this->accumulateFlux[i].resize(cellNumber + 1);
		this->accumulateFluxSquare[i].resize(cellNumber + 1);
		this->fluxStandardDeviation[i].resize(cellNumber + 1);
	}

	//���Ŵ�1��ʼ����0��Ԫ�ز���
	this->Keff.resize(totalGenerationNumber + 1);
	this->averageKeff.resize(totalGenerationNumber + 1);
	this->accumulateKeffSquare = 0;
	this->KeffStandardDeviation.resize(totalGenerationNumber + 1);

}