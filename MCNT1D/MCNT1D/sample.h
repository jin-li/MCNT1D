#pragma once
#ifndef SAMPLE_H
#define SAMPLE_H

#include <cmath>

#include "cell.h"
#include "neutron.h"
#include "rand.h"
#include "geometry.h"

/*---------------------------------------------------------------------------------------------------
	�Ӷ�Ⱥ����Դ�е�ĳһȺ�г�ȡһ�����ӡ�
	������ĳȺ���ӿ�
	���أ������õ���һ������
	eg�� sampleFromParticleSourceBank(multiGroupParticleSourceBank[groupNumber]);
---------------------------------------------------------------------------------------------------*/
template <class P>
P sampleFromParticleSourceBank(std::vector<P> &_sourceBank);

/*----------------------------------------------------------
	������Ⱥ��դԪ��ȡ�������ȡ�
	��������Ⱥ�ţ�դԪ��
	���أ���������
	eg: samplePathLength(geometry,neutron);
----------------------------------------------------------*/
double samplePathLength(int _group, geometry _geometry, neutron _neutron);

#endif // !SAMPLE_H
