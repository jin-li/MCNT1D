#pragma once
#ifndef PARTICLE_H
#define PARTICLE_H

class particle {
public:
	double x;				//λ��
	double direction;       //����
	double weight;          //Ȩ��

	//���캯��
	particle(double _x = 0, double _direction = 1, double _weight = 1) :x(_x), direction(_direction), weight(_weight) {};

	//���ƹ��캯��
	particle(const particle &_p) {
		x = _p.x;
		direction = _p.direction;
		weight = _p.weight;
	}

	//��������
	virtual ~particle() {};
};

#endif // !PARTICLE_H
