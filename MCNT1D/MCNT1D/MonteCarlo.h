#pragma once
#ifndef MONTECARLO_H
#define MONTECARLO_H

#include <vector>
#include <fstream>
#include <string>

//#include "particle.h"
#include "neutron.h"
#include "material.h"
#include "cell.h"
#include "geometry.h"

//Monte Carlo�ࣺ����������������˼��㣬������
class MonteCarlo {
private:
	//�����û�����ļ�������
	int groupNumber;                                         //��Ⱥ��
	int cellNumber;                                          //դԪ��
	int materialNumber;                                      //������
	int neutronNumber;                                       //ģ��������
	int inactiveGenerationNumber;                            //�ǻ�Ծ����
	int totalGenerationNumber;                               //�ܴ���
	geometry inputGeometry;                                  //����
	std::vector<cell> inputcell;                             //դԪ
	std::vector<material> inputMaterial;                     //����


	//��������еı���
	int currentParticleNumber;

	//template<class P>
	std::vector<std::vector<neutron>> multiGroupParticleSourceBank;          //Դ���ӿ�
	//template<class P>
	std::vector<std::vector<neutron>> multiGroupNextParticleSourceBank;      //�Ӵ����ӿ�
	
	std::vector<std::vector<double>> flux;                             //��ͨ��
	std::vector<std::vector<double>> averageFlux;                      //��Ծ��ƽ��ͨ��
	double Keff;                                                       //��ǰ����Ч��ֳϵ��
	double averageKeff;                                                //��Ծ��ƽ����Ч��ֳϵ��

	std::string inputFileName;                                         //�����ļ���
	std::string outputFileName;                                        //����ļ���
	std::ifstream inputFile;                                           //�����ļ���
	std::ofstream outputFile;                                          //����ļ���

	double random() { return rand() / (double)RAND_MAX; }                   //����һ��0~1֮��������
	double randomDirection() { return 2 * rand() / (double)RAND_MAX - 1; }  //����һ��-1~1֮��������

	double weightMax;
	double weightMin;

	/*-------------------------------------------------------------------------
		���ܣ�ģ��һ�����ӵ���Ⱥ���ѱ���̡�
		������Ҫ�ѱ�����ӣ����ã���������Ⱥ����һ�����ӿ⣨���ã�
		���أ���
		ʾ����fission(myNeutron, groupID, multiGroupNextNeutronSourceBank);
	-------------------------------------------------------------------------*/
	void fission(neutron &_neutron, int _group, std::vector<std::vector<neutron>> &_nextNeutronSourceBank);

	/*-------------------------------------------------------------------------
		���ܣ�ģ��һ�����ӵ���Ⱥ��ɢ����̡�
		������Ҫɢ������ӣ����ã���������Ⱥ����ǰ���ӿ⣨���ã�
		���أ���
		ʾ����scatter(myNeutron, groupID, multiGroupNeutronSourceBank);
	-------------------------------------------------------------------------*/
	void scatter(neutron &_neutron, int _group, std::vector<std::vector<neutron>> &_currentNeutronSourceBank);

	/*---------------------------------------
		���ܣ��ж����ӿ��Ƿ�ȫ�ա�
		��������
		���أ�ȫ�շ����棬���շ��ؼ�
		ʾ����ifNeutronSourceBankEmpty();
	---------------------------------------*/
	bool ifNeutronSourceBankEmpty();

	void inactiveKeff();

	void inactiveFlux();

	void activeKeff();

	void activeFlux();

public:
	//void init();
	void readInput();
	void transport();
	void output();
};

#endif // !MONTECARLO_H
