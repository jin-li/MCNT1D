#include "MonteCarlo.h"
#include "particle.h"
#include "neutron.h"
#include "sample.h"

#include <iostream>
#include <numeric>                      

void MonteCarlo::run() {
	this->readInput();
	this->init();
	this->transport();
	this->output();
}

MonteCarlo::MonteCarlo(const std::string _inFileName, const std::string _outFileName) {
	this->inputFileName = _inFileName;
	this->outputFileName = _outFileName;
	this->inputFile.open(_inFileName);
	//��������ļ��Ƿ����
	if (!inputFile) {
		std::cout << "Error! E001: Can't open file \"" << _inFileName << "\"!" << std::endl;
		return;
	}
	this->outputFile.open(_outFileName);
	//�������ļ��Ƿ��ܴ�
	if (!outputFile) {
		std::cout << "Error! E001: Can't open file \"" << _outFileName << "\"!" << std::endl;
		return;
	}
}

int MonteCarlo::roulette(double _weight) {
	if (_weight - (int)_weight < random())return (int)_weight;
	else return (int)(_weight + 1);
}

void MonteCarlo::fission(neutron &_neutron, int _group,double _fissionWeight, std::vector<std::vector<neutron>> &_nextNeutronSourceBank) {
	//�����ѱ�����ڸ�Ⱥ�ķֲ�
	for (int iFissionProductGroup = 1; iFissionProductGroup <= groupNumber; iFissionProductGroup++) {
		//�����ѱ�����ڸ�Ⱥ��Ȩ��
		double fissionWeight = (this->inputGeometry.geometryCell[this->inputGeometry.getCellID(_neutron.x)]).mat.yield[_group][iFissionProductGroup] * _neutron.weight*_fissionWeight;
		
		//���̶�ȷ���ѱ������������
		int fissionNeutronNumber = roulette(fissionWeight);

		//���ѱ���������Ӵ�����һ�����ӿ�
		for (int i = 0; i < fissionNeutronNumber; i++) {
			//��������һ���ѱ���Ⱥ��Ϊ��ǰѭ�����ѱ�Ⱥ���������������Ȩ��Ϊ1
			neutron fissionNeutronProduct(_group, _neutron.x, randomDirection(), 1);

			//���ѱ���������Ӵ�����һ�����ӿ�
			_nextNeutronSourceBank[iFissionProductGroup].push_back(fissionNeutronProduct);
		}
	}

	//ɱ���ѱ�Դ����
	//_neutron.weight = 0;
}

void MonteCarlo::scatter(neutron &_neutron, int _group, std::vector<std::vector<neutron>> &_currentNeutronSourceBank) {
	//ɢ�䵽����Ⱥ
	for (int iScatterGroup = 1; iScatterGroup <= groupNumber; iScatterGroup++) {
		//ɢ�䵽iScatterGroupȺ��Ȩ��
		double scatterWeight = _neutron.weight*(this->inputGeometry.geometryCell[this->inputGeometry.getCellID(_neutron.x)]).mat.sigmaS[_group][iScatterGroup]/ (this->inputGeometry.geometryCell[this->inputGeometry.getCellID(_neutron.x)]).mat.sigmaT[_group];

		if (scatterWeight > 0) {
			//��������ɢ������
			neutron scatterNeutron(iScatterGroup, _neutron.x, randomDirection(), scatterWeight);
			//���뵱ǰ���ӿ�
			multiGroupParticleSourceBank[iScatterGroup].push_back(scatterNeutron);
		}

		if (scatterWeight < weightMin && scatterWeight > 0) {
			//Ȩ�ع�С�����̶ľ�������
			if (roulette(0.1)) {
				//��Ӯ����Ȩ10��
				scatterWeight *= 10;
			}
			else {
				//����ı�ɱ��
				scatterWeight = 0;
			}
		}
	}
	//ɱ��ɢ��Դ����
	_neutron.weight = 0;
}

bool MonteCarlo::ifNeutronSourceBankEmpty() {
	for (int i = 1; i <= this->groupNumber; i++) {
		//���ӿ�ĳһȺ���ռ����ؼ�
		if (!this->multiGroupParticleSourceBank[i].empty()) return false;
	}
	//ȫ�շ�����
	return true;
}

void MonteCarlo::currentKeff(int _iterationNumber) {
	int nextParticleNumber = 0;
	
	//�ۼӸ�Ⱥ��һ��������
	for (int i = 1; i <= this->groupNumber; i++)nextParticleNumber += multiGroupNextParticleSourceBank[i].size();
	//Keff�ļ���:�ǻ�Ծ��ֻ����Keff��Keffƽ��ֵ��ͳ��������Ϊ0����Ծ�����ô˺���ʱƽ��ֵ��ͳ���������¸���
	this->Keff[_iterationNumber] = (nextParticleNumber+0.0) / this->currentParticleNumber;
	this->averageKeff[_iterationNumber] = 0;
	this->KeffStandardDeviation[_iterationNumber] = 0;
}

void MonteCarlo::currentFlux(int _iterationNumber) {
	//ͨ�����ݴ洢��MonteCarlo::flux�У���һά����Ⱥ�ţ��ڶ�ά��դԪ��
}

void MonteCarlo::activeKeff(int _iterationNumber) {
	//���㵱ǰ��Keff���洢
	currentKeff(_iterationNumber);

	double accmKeff = std::accumulate(Keff.begin() + inactiveGenerationNumber, Keff.begin() + _iterationNumber, 0.0);
	averageKeff[_iterationNumber] = accmKeff / (_iterationNumber - this->inactiveGenerationNumber);

	//����Keff��ͳ�����
	double standardDeviation = 0;
	if (_iterationNumber > this->inactiveGenerationNumber + 1) {
		standardDeviation = sqrt((this->accumulateKeffSquare / (_iterationNumber - this->inactiveGenerationNumber) - (this->averageKeff[_iterationNumber - 1])*(this->averageKeff[_iterationNumber - 1])) / (_iterationNumber - this->inactiveGenerationNumber - 1)) / this->averageKeff[_iterationNumber - 1];
	}
	//����Keffͳ�����洢
	this->KeffStandardDeviation[_iterationNumber] = standardDeviation;
}

void MonteCarlo::activeFlux(int _iterationNumber) {
	for (int iGroup = 1; iGroup <= this->groupNumber; iGroup++) {
		for (int iCell = 1; iCell <= this->cellNumber; iCell++) {
			this->accumulateFlux[iGroup][iCell] += this->flux[iGroup][iCell];
			this->accumulateFluxSquare[iGroup][iCell] += (this->flux[iGroup][iCell])*(this->flux[iGroup][iCell]);
			this->averageFlux[iGroup][iCell] = this->accumulateFlux[iGroup][iCell] / (_iterationNumber - this->inactiveGenerationNumber);
			//��һ����Ծ��û��ͳ�����
			if(_iterationNumber!=this->inactiveGenerationNumber + 1) this->fluxStandardDeviation[iGroup][iCell] = sqrt(((this->accumulateFluxSquare[iGroup][iCell] / (_iterationNumber - this->inactiveGenerationNumber) - (this->averageFlux[iGroup][iCell])*(this->averageFlux[iGroup][iCell])) / (_iterationNumber - this->inactiveGenerationNumber - 1))) / this->averageFlux[iGroup][iCell];
			else this->fluxStandardDeviation[iGroup][iCell] = 0;
		}
	}
}
