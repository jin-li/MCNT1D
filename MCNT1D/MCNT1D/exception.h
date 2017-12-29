#pragma once
#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <string>
#include <stdexcept>

class mcException {
private:
	std::string message;         //�쳣��Ϣ
public:
	//���캯��
	mcException(const std::string &_message) :message(_message) {}

	//��������
	~mcException() = default;
	
	//�鿴�쳣��Ϣ
	const std::string &getMessage() const { return message; }
};

#endif // !EXCEPTION_H

