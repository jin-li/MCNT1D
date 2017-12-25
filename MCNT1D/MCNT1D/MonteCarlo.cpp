#include "MonteCarlo.h"
#include "particle.h"
#include "neutron.h"
#include "sample.h"

#include <iostream>
#include <numeric>

/*------------------------------------------------------------------
	���ܣ����̶ġ�
	����һ��0~1���������������Ȩ�ص�С�����֣�������ȡ������������ȡ����
	������Ȩ��
	���أ��ĵĽ��
	ʾ��: roulette(weight);
-------------------------------------------------------------------*/
int roulette(double _weight) {
	if (_weight - (int)_weight < random())return (int)_weight;
	else return (int)(_weight + 1);
}


void MonteCarlo::fission(neutron &_neutron, int _group, std::vector<std::vector<neutron>> &_nextNeutronSourceBank) {
	//�����ѱ�����ڸ�Ⱥ�ķֲ�
	for (int iFissionProductGroup = 1; iFissionProductGroup <= groupNumber; iFissionProductGroup++) {
		//�����ѱ�����ڸ�Ⱥ��Ȩ��
		double fissionWeight = (this->inputGeometry.geometryCell[this->inputGeometry.getCellID(_neutron.x)]).mat.yield[_group][iFissionProductGroup];
		
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
	_neutron.weight = 0;
}

void MonteCarlo::scatter(neutron &_neutron, int _group, std::vector<std::vector<neutron>> &_currentNeutronSourceBank) {
	//ɢ�䵽����Ⱥ
	for (int iScatterGroup = 1; iScatterGroup <= groupNumber; iScatterGroup++) {
		//ɢ�䵽iScatterGroupȺ��Ȩ��
		double scatterWeight = _neutron.weight*(this->inputGeometry.geometryCell[this->inputGeometry.getCellID(_neutron.x)]).mat.sigmaS[_group][iScatterGroup];

		if (scatterWeight < weightMin && scatterWeight > 0) {
			//Ȩ�ع�С�����̶ľ�������
			if (!roulette(0.1)) {
				//��Ӯ����Ȩ10��
				scatterWeight *= 10;
			}
			else {
				//����ı�ɱ��
				scatterWeight = 0;
			}
		}

		if (scatterWeight > 0) {
			//��������ɢ������
			neutron scatterNeutron(iScatterGroup, _neutron.x, randomDirection(), scatterWeight);

			//���뵱ǰ���ӿ�
			multiGroupParticleSourceBank[iScatterGroup].push_back(scatterNeutron);
		}
		
		//ɱ��ɢ��Դ����
		_neutron.weight = 0;
	}
}

bool MonteCarlo::ifNeutronSourceBankEmpty() {
	for (int i = 0; i < this->groupNumber; i++) {
		//���ӿ�ĳһȺ���ռ����ؼ�
		if (!this->multiGroupParticleSourceBank[i].empty()) return false;
	}
	//ȫ�շ�����
	return true;
}

void MonteCarlo::inactiveKeff() {
	int nextParticleNumber = 0;
	double currentKeff = 0;
	//�ۼӸ�Ⱥ��һ��������
	for (int i = 1; i <= this->groupNumber; i++)nextParticleNumber += multiGroupNextParticleSourceBank[i].size();
	//Keff�Ķ���
	currentKeff = nextParticleNumber / this->currentParticleNumber;
}

void MonteCarlo::inactiveFlux() {
	//ͨ�����ݴ洢��MonteCarlo::flux�У���һά����Ⱥ�ţ��ڶ�ά��դԪ��
}

void MonteCarlo::activeKeff() {

}

void MonteCarlo::transport() {
	//Դ����
	for (int iGenerationCount = 0; iGenerationCount < this->totalGenerationNumber; iGenerationCount++) {
		//��Ⱥ����
		while (!ifNeutronSourceBankEmpty()) {
			for (int iGroupCount = 1; iGroupCount <= this->groupNumber; iGroupCount++) {
				while (!this->multiGroupParticleSourceBank[iGroupCount].empty()) {
					neutron myNeutron;
					//������Դ�г�ȡһ������
					myNeutron = sampleFromParticleSourceBank(this->multiGroupParticleSourceBank[iGroupCount]);

					//Ȩ�ع�������ӽ������̶ģ���Ϊ����Ȩ��Ϊ1������
					if (myNeutron.weight > weightMax) {
						int iRouletteResult = roulette(myNeutron.weight);
						myNeutron.weight = 0;   //���̶�֮��ɱ��ԭ����
						//�������̶Ľ������
						for (int i = 0; i < iRouletteResult; i++) {
							neutron newNeutron = neutron(iGroupCount, myNeutron.x, myNeutron.direction, 1);
							multiGroupParticleSourceBank[iGroupCount].push_back(newNeutron);
						}

						continue;              //ֱ�����³���
					}

					//Ȩ�ز�Ϊ0�����ӽ�������ģ��
					while (myNeutron.weight > 0) {
						//�Ծ������ȳ���
						double pathLength = samplePathLength(this->inputGeometry, myNeutron);
						//�ж��Ƿ񴩹�դԪ�߽�
						if (this->inputGeometry.ifCrossCellBoundary(myNeutron, pathLength)) {
							//ͳ��ͨ��
							if (myNeutron.direction > 0) {
								double distanceToBoundary = (this->inputGeometry.geometryCell[this->inputGeometry.getCellID(myNeutron.x)]).right - myNeutron.x;
								this->flux[iGroupCount][this->inputGeometry.getCellID(myNeutron.x)] += std::fabs(distanceToBoundary / myNeutron.direction*myNeutron.weight);
							}
							else if (myNeutron.direction < 0) {
								double distanceToBoundary = myNeutron.x - (this->inputGeometry.geometryCell[this->inputGeometry.getCellID(myNeutron.x)]).left;
								this->flux[iGroupCount][this->inputGeometry.getCellID(myNeutron.x)] += std::fabs(distanceToBoundary / myNeutron.direction*myNeutron.weight);
							}
							else {
								std::cout << "Warning! W:Neutron direction = 0!" << std::endl;
							}

							//���ɹ�դԪ�߽�����ӷ�������դԪ�߽紦�����ɳ����α߽������ɱ��
							this->inputGeometry.setCrossBoundaryPosition(myNeutron, pathLength);

							//�����ӷŵ���һդԪ�ı߽紦֮������Ծ������ȳ�����ray-tracking?
							continue;
						}
						else {
							//ͳ��ͨ��
							this->flux[iGroupCount][this->inputGeometry.getCellID(myNeutron.x)] += pathLength * myNeutron.weight;

							//�����ӷŵ���ײ��λ��
							myNeutron.x += pathLength * myNeutron.direction;
						} //ͨ��ͳ����ϣ���ײ��ȷ��

						//���̶��ж��������ѱ仹��ɢ��
						//���㵱����ɢ�����
						double nativeSigmaS = std::accumulate(((this->inputGeometry.getMaterial(myNeutron.x)).sigmaS[iGroupCount]).begin(), ((this->inputGeometry.getMaterial(myNeutron.x)).sigmaS[iGroupCount]).end(), 0);
						//��ȡ�����ܽ���
						double nativeSigmaT = (this->inputGeometry.getMaterial(myNeutron.x)).sigmaT[iGroupCount];
						if (roulette(nativeSigmaS / nativeSigmaT)) { //��ɢ���Ȩ�ضģ�Ӯ�򷵻�0
							//����1˵�����̶�����������ѱ�
							fission(myNeutron, iGroupCount, multiGroupNextParticleSourceBank);
						}
						else {
							//����ɢ��
							scatter(myNeutron, iGroupCount, multiGroupParticleSourceBank);
						}
					} //Ȩ�ز�Ϊ0�������������
				} //��ǰȺ�������
			} //����Ⱥ�������
		} //����ɢ�������Ⱥ�������

		std::cout << "The " << iGenerationCount << "th generation neutron simulation was finished!" << std::endl;

		
	} //���д��������
}