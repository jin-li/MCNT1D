#pragma once
#ifndef SAMPLE_H
#define SAMPLE_H

#include <cmath>

#include "cell.h"
#include "neutron.h"
#include "rand.h"

/*---------------------------------------------------------------------------------------------------
	�Ӷ�Ⱥ����Դ�е�ĳһȺ�г�ȡһ�����ӡ�
	������ĳȺ���ӿ�
	���أ������õ���һ������
	eg�� sampleFromParticleSourceBank(multiGroupParticleSourceBank[groupNumber]);
---------------------------------------------------------------------------------------------------*/
template <class P>
P sampleFromParticleSourceBank(std::vector<P> &_sourceBank) {
	P sampledParticle = _sourceBank.back();
	_sourceBank.pop_back();
	return sampledParticle;
}

/*----------------------------------------------------------
	������Ⱥ��դԪ��ȡ�������ȡ�
	��������Ⱥ�ţ�դԪ��
	���أ���������
	eg: samplePathLength(geometry,neutron);
----------------------------------------------------------*/
double samplePathLength(geometry _geometry, neutron _neutron) {
	double __x = _neutron.x;
	double __sigmaT = (_geometry.getMaterial(_geometry.getCellID(__x))).sigmaT;
	return -log(random()) / __sigmaT;
}

#endif // !SAMPLE_H
