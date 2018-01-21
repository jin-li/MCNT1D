#pragma once
#ifndef MATLAB_H
#define MATLAB_H

#include <string>

#include "engine.h"    // MATLAB calculating engine

#pragma comment (lib,"libmat.lib")
#pragma comment (lib,"libeng.lib")
#pragma comment (lib,"libmx.lib")

class iMatlab {
private:
	std::string matlabCmd;  //�洢Ҫִ�е�matlab����
	Engine *engine;         //matlab����
public:
	//���캯��
	iMatlab(std::string _matlabCmd) :matlabCmd(_matlabCmd) {}

	/*---------------------------------------------
	���ܣ�����matlab���档
	��������
	���أ�����ֵ�������ɹ������棬����ʧ�ܷ��ؼ�
	ʾ����mt.open();
	--------------------------------------------*/
	bool open();

	/*---------------------------------------------
		���ܣ�����matlab���档
		��������
		���أ�����ֵ�����гɹ������棬����ʧ�ܷ��ؼ�
		ʾ����mt.run();
	--------------------------------------------*/
	bool run();

	/*---------------------------------------------
	���ܣ��ر�matlab���档
	��������
	���أ���
	ʾ����mt.close();
	--------------------------------------------*/
	void close();
};

#endif // !MATLAB_H