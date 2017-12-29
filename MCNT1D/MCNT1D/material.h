#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H
#include <vector>

class material{
public:
    std::vector<double> nuSigmaF;                      //�ѱ���棬���Ϊ��Ⱥ��
    std::vector<double> sigmaT;                        //�ܽ��棬���Ϊ��Ⱥ��
    std::vector<std::vector<double>> sigmaS;           //ɢ����棬��һά����Ⱥ���ڶ�ά��ɢ�������Ⱥ
    std::vector<std::vector<double>> yield;            //���ʣ���һά����Ⱥ���ڶ�ά���ѱ������Ⱥ

	//Ĭ�Ϲ��캯��
	material() = default;

	//���캯��
    material(std::vector<double> _nuSigmaF, std::vector<double> _sigmaT, std::vector<std::vector<double>> _sigmaS, std::vector<std::vector<double>> _yield){
        nuSigmaF=_nuSigmaF;
        sigmaT=_sigmaT;
        sigmaS=_sigmaS;
        yield=_yield;
    }

	//���ƹ��캯��
    material(const material &_mat){
        nuSigmaF=_mat.nuSigmaF;
        sigmaT=_mat.sigmaT;
        sigmaS=_mat.sigmaS;
        yield=_mat.yield;
    }
};

#endif
