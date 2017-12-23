#pragma once
#ifndef MONTECARLO_H
#define MONTECARLO_H

#include <vector>
#include <fstream>
#include <string>

#include "particle.h"
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
	template<class P>
	std::vector<std::vector<P>> multiGroupParticleSourceBank;          //Դ���ӿ�
	template<class P>
	std::vector<std::vector<P>> multiGroupNextParticleSourceBank;      //�Ӵ����ӿ�
	
	std::vector<std::vector<double>> flux;                             //��ͨ��
	std::vector<std::vector<double>> averageFlux;                      //ƽ��ͨ��
	double totalKeff;                                                  //����Ч��ֳϵ��

	std::string inputFileName;                                         //�����ļ���
	std::string outputFileName;                                        //����ļ���
	std::ifstream inputFile;                                           //�����ļ���
	std::ofstream outputFile;                                          //����ļ���
	


public:
	void init();
	void readInput();
	void transport();
	void output();
};

#endif // !MONTECARLO_H
