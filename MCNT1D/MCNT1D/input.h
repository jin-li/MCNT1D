#pragma once
#ifndef INPUT_H
#define INPUT_H

#include "line.h"
#include "exception.h"

#include <algorithm>
#include <map>
#include <sstream>

/*--------------------------------------------------------------------------------------------------
	��ʾ�����ļ������뿨��ʼ��ǵ�ö���ࡣCC��ʾ������������CP��ʾ�����������MT��ʾ���Ͽ���CL��ʾդԪ����
---------------------------------------------------------------------------------------------------*/
typedef enum { CC, CP, MT, CL } startFlag;

/*--------------------------------------------------------------------------
	���ܣ���һ���ı�תΪiLine����ɾȥ��//�������ע�����ݣ�����ȫ���ַ�תΪСд��
	������һ���ı�
	���أ�iLine����
	ʾ����translate(str);
---------------------------------------------------------------------------*/
iLine translate(std::string _line);

/*---------------------------------------
	���ܣ���ȡ���뿨��ʼ��ǡ�
	������תΪiLine��һ���ı�
	���أ���ʼ��ǵ�ö�������
	ʾ����getStartKeyword(ilineTemp)��
----------------------------------------*/
startFlag getStartKeyword(iLine _line) throw(mcException);

/*---------------------------------------------------
	���ܣ���ȡһ���ı��еļ���������
	������һ���ı�
	���أ�һ��ӳ�䣬�������������ؼ��ֺͶ�Ӧ������
	ʾ����getCalculationCondition(line);
---------------------------------------------------*/
//std::pair<std::string, int> getCalculationCondition(iLine _line) throw(mcException);
template<typename T>
std::pair<std::string, T> getCalculationCondition(iLine _line) throw(mcException) {
	if (_line.countWords() != 2) throw("Error! E003: Syntax error: illegal parameter number for calculation condition!");
	std::stringstream __ssTemp;
	__ssTemp << _line[1];
	T __numberTemp;
	//���ڶ������ʶ�Ӧ��ֵתΪint
	__ssTemp >> __numberTemp;
	std::pair<std::string, T> __conditionValueTemp(_line[0], __numberTemp);
	//��������������ֵ����ӳ������
	//make_pair(_line[0], __numberTemp));
	return __conditionValueTemp;
}

/*---------------------------------
	���ܣ����ַ���תΪ˫���ȸ�������
	�������ַ���
	���أ�˫���ȸ�����
	ʾ����s2d("2.33");
----------------------------------*/
double s2d(std::string _string);

/*---------------------------------
	���ܣ����ַ���תΪ������
	�������ַ���
	���أ�����
	ʾ����s2i("233");
----------------------------------*/
int s2i(std::string _string);

#endif // !INPUT_H

