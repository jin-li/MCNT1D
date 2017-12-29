#pragma once
#ifndef MONTECARLO_H
#define MONTECARLO_H

#include <vector>
#include <fstream>
#include <string>

#include "neutron.h"
#include "material.h"
#include "cell.h"
#include "geometry.h"
#include "input.h"

//Monte Carlo�ࣺ����������������˼��㣬������
class MonteCarlo {
private:
	//�����û�����ļ�������
	int groupNumber;                                         //��Ⱥ��
	int cellNumber;                                          //դԪ��
	int materialNumber;                                      //������
	int repetitiveNumber;                                    //�ظ�դԪ����
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
	
	std::vector<std::vector<double>> flux;                             //��ͨ������һά����Ⱥ�ţ��ڶ�ά��դԪ��
	std::vector<std::vector<double>> averageFlux;                      //��Ծ��ƽ��ͨ��
	std::vector<std::vector<double>> accumulateFlux;                   //��Ծ��ͨ���ۻ�
	std::vector<std::vector<double>> accumulateFluxSquare;             //ͨ��ƽ���ۼӣ����ڼ��㷽��
	std::vector<std::vector<double>> fluxStandardDeviation;            //��ͨ������

	std::vector<double> Keff;                                          //������Ч��ֳϵ��
	std::vector<double> averageKeff;                                   //����ƽ����Ч��ֳϵ�����ǻ�Ծ����Ϊ0
	double accumulateKeffSquare;                                       //��Ч��ֳϵ��ƽ���ۼӣ����ڼ��㷽��
	std::vector<double> KeffStandardDeviation;                         //����Keffͳ�����

	std::string inputFileName;                                         //�����ļ���
	std::string outputFileName;                                        //����ļ���
	std::ifstream inputFile;                                           //�����ļ���
	std::ofstream outputFile;                                          //����ļ���

	//double random() { return rand() / (double)RAND_MAX; }                   //����һ��0~1֮��������
	//double randomDirection() { return 2 * rand() / (double)RAND_MAX - 1; }  //����һ��-1~1֮��������

	double weightMax;
	double weightMin;

	/*-------------------------------------------------------------------------
		���ܣ�ģ��һ�����ӵ���Ⱥ���ѱ���̡�
		������Ҫ�ѱ�����ӣ����ã���������Ⱥ����һ�����ӿ⣨���ã�
		���أ���
		ʾ����fission(myNeutron, groupID, multiGroupNextNeutronSourceBank);
	-------------------------------------------------------------------------*/
	void fission(neutron &_neutron, int _group, double _fissionWeight, std::vector<std::vector<neutron>> &_nextNeutronSourceBank);

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

	/*----------------------------------------------
		���ܣ����㵱ǰ����Ч��ֳϵ�����洢��Keff������
		������Դ�����������
		���أ���
		ʾ����currentKeff(interationNumber);
	----------------------------------------------*/
	void currentKeff(int _iterationNumber);

	/*--------------------------------------------------
		���ܣ����㵱ǰ������Ⱥ��դԪͨ�����洢����ά�����С�
		������Դ�����������
		���أ���
		ʾ����currentFlux(interationNumber);
	--------------------------------------------------*/
	void currentFlux(int _iterationNumber);

	/*----------------------------------------------
		���ܣ������Ծ����Ч��ֳϵ�����洢��Keff������
		������Դ�����������
		���أ���
		ʾ����currentKeff(interationNumber);
	----------------------------------------------*/
	void activeKeff(int _iterationNumber);

	/*---------------------------------------------------
		���ܣ������Ծ������Ⱥ��դԪͨ�����洢����ά�����С�
		������Դ�����������
		���أ���
		ʾ����currentKeff(interationNumber);
	---------------------------------------------------*/
	void activeFlux(int _iterationNumber);

	/*------------------------------------------------------------------
	���ܣ����̶ġ�
	����һ��0~1���������������Ȩ�ص�С�����֣�������ȡ������������ȡ����
	������Ȩ��
	���أ��ĵĽ��
	ʾ��: roulette(weight);
	-------------------------------------------------------------------*/
	int roulette(double _weight);

public:
	/*-------------------------------------------------
		���ܣ����캯����
		�����������ļ��ļ���������ļ��ļ���
		���أ�MonteCarlo����
		ʾ����MonteCarlo mcnt("file.in", "file.out");
	--------------------------------------------------*/
	MonteCarlo(const std::string _inFileName, const std::string _outFileName = ".out");

	/*-----------------------
		���ܣ��ɿ�ģ�����С�
		��������
		���أ���
		ʾ����mcnt.run();
	-----------------------*/
	void run();

	/*--------------------------------------------------
		���ܣ���ȡ������������Ԫ������������input.cpp�С�
		������MonteCarlo�������ã�����ȡ�����ı�
		���أ���
		ʾ����getCalculationCondition(mcnt, lineRead);
	---------------------------------------------------*/
	friend void getCalculationCondition(MonteCarlo &_mc, iLine _line) throw(mcException);

	/*-------------------------------
		���ܣ���ʼ����Ϊ���������ڴ�ȡ�
		��������
		���أ���
		ʾ����mcnt.init();
	--------------------------------*/
	void init();

	/*------------------------------
		���ܣ����������ļ���
		��������
		���أ���
		ʾ����mcnt.readInput();
	------------------------------*/
	void readInput();

	/*-----------------------------
		���ܣ��ɿ��������˹��̡�
		��������
		���أ���
		ʾ����mcnt.transport();
	-----------------------------*/
	void transport();

	/*-----------------------------
		���ܣ�����ɿ����˼�������
		��������
		���أ���
		ʾ����mcnt.output();
	-----------------------------*/
	void output();
};

#endif // !MONTECARLO_H
