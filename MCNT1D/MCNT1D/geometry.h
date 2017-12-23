#pragma once
#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>
#include "cell.h"

//�߽�������A-���գ�R-���䣬 W-�ױ߽�
typedef enum { A, R, W } boundaryCondition;

class geometry {
public:
	double left;
	double right;
	boundaryCondition leftCondition;
	boundaryCondition rightCondition;

	std::vector<cell> geomCell;

	geometry(double _left, double _right, std::vector<cell> _geomCell) :left(_left), right(_right), geomCell(_geomCell) {}

};

#endif // !GEOMETRY_H
