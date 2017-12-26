#include "MonteCarlo.h"
#include "sample.h"

#include <iostream>
#include <numeric>

void MonteCarlo::transport() {
	//Դ����
	for (int iGenerationCount = 1; iGenerationCount <= this->totalGenerationNumber; iGenerationCount++) {
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
						double pathLength = samplePathLength(iGroupCount, this->inputGeometry, myNeutron);
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

		//������Ч��ֳϵ����ͨ��
		if (iGenerationCount <= this->inactiveGenerationNumber) {
			//�ǻ�Ծ��
			currentKeff(iGenerationCount);
			currentFlux(iGenerationCount);
		}
		else {
			//��Ծ��
			activeKeff(iGenerationCount);
			activeFlux(iGenerationCount);
		}

		//��������Դ
		int nextGenerationNeutronNumber = 0;
		for (int i = 0; i < this->groupNumber; i++)nextGenerationNeutronNumber += this->multiGroupNextParticleSourceBank[i].size();

		int nextNeutronWeight = this->currentParticleNumber / nextGenerationNeutronNumber;
		//�������ӿ�
		multiGroupParticleSourceBank = multiGroupNextParticleSourceBank;
		for (int i = 0; i < this->groupNumber; i++) {
			if (multiGroupParticleSourceBank[i].size > 0) {
				for (int j = 0; j < multiGroupParticleSourceBank[i].size(); j++)multiGroupParticleSourceBank[i][j].weight = nextNeutronWeight;
			}
			//��ջ�������ӿ�
			this->multiGroupNextParticleSourceBank[i].clear();
		}

	} //���д��������
}
