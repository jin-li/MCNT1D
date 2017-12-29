#pragma once
#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>
#include "cell.h"
#include "material.h"
#include "neutron.h"

//����С��
#define INF_MIN 0.000000001  

//�߽�������A-���գ�R-���䣬 W-�ױ߽�
typedef enum { A, R, W } boundaryCondition;

class geometry {
	double left;                       //������߽�
	double right;                      //�����ұ߽�
	boundaryCondition leftCondition;   //��߽�����
	boundaryCondition rightCondition;  //�ұ߽�����
	int cellNumber;                    //դԪ��

public:
	std::vector<cell> geometryCell;    //����������դԪ

	//Ĭ�Ϲ��캯��
	geometry() = default;

	//������߽�λ��
	void setLeft(double _leftPosition) { left = _leftPosition; }

	//�����ұ߽�λ��
	void setRight(double _rightPosition) { right = _rightPosition; }

	/*------------------------------------------------------
		���ܣ�������߽�������
		��������ʾ�߽����������֣�����Ϊ0��ȫ����Ϊ1���ױ߽�Ϊ2
		���أ���
		ʾ����setLeftBoundaryCondition(1);
	-------------------------------------------------------*/
	void setLeftBoundaryCondition(int _boundaryCondition);

	/*------------------------------------------------------
		���ܣ������ұ߽�������
		��������ʾ�߽����������֣�����Ϊ0��ȫ����Ϊ1���ױ߽�Ϊ2
		���أ���
		ʾ����setRighttBoundaryCondition(1);
	-------------------------------------------------------*/
	void setRightBoundaryCondition(int _boundaryCondition);

	//���캯��
	geometry(double _left, double _right, std::vector<cell> _geomCell) :left(_left), right(_right), geometryCell(_geomCell) {}
	
	/*------------------------------
		���ܣ�����ָ��λ��������դԪ��š�
		����������
		���أ�դԪ���
		ʾ��: getCellID(x);
	------------------------------*/
	int getCellID(double _x);

	/*------------------------------
		���ܣ�����ָ��λ�������Ĳ��ϡ�
		����������
		���أ����϶���
		ʾ��: getMaterial(x);
	------------------------------*/
	material getMaterial(double _x);

	/*----------------------------------------------------
		���ܣ��ж������Ƿ񴩹�դԪ�߽硣
		���������ӣ���������
		���أ�����Ϊ�棬δ����Ϊ��
		ʾ����ifCrossCellBoundary(myNeutron, myPath);
	-----------------------------------------------------*/
	bool ifCrossCellBoundary(neutron _neutron, double _pathLength);

	/*----------------------------------------------------
		���ܣ��ж������Ƿ�ɳ����Ρ�
		���������ӣ���������
		���أ��ɳ�Ϊ�棬δ�ɳ�Ϊ��
		ʾ����ifBeyondGeometry(myNeutron, myPath);
	-----------------------------------------------------*/
	bool ifBeyondGeometry(neutron _neutron, double _pathLength);

	/*-----------------------------------------------------
		���ܣ�������դԪ�߽�򼸺α߽�����ӡ�
		���������ӣ����ã�����������
		���أ���
		ʾ����setCrossBoundaryPosition(myNeutron, myPath);
	------------------------------------------------------*/
	void setCrossBoundaryPosition(neutron &_neutron, double _pathLength);

};

#endif // !GEOMETRY_H
