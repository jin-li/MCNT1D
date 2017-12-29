#ifndef LINE_H
#define LINE_H


#include <iostream>
#include <string>
#include <fstream>
#include <sstream>    //string stream
#include <iomanip>    //format control

//#include "engine.h"                                                // MATLAB calculating engine

//#pragma comment (lib,"libmat.lib")
//#pragma comment (lib,"libeng.lib")
//#pragma comment (lib,"libmx.lib")

class iLine {
private:
	std::string _aLine_;
public:

	/*---------------------------------------------------------------
		���ܣ����캯����
		������һ���ı�
		���أ�iLine����
		ʾ����iLine("This is a new line to replace the old line.");
	---------------------------------------------------------------*/
	iLine(std::string _aLine_);
	
	//Ĭ�Ϲ��캯��
	iLine() = default;

	/*--------------------------
		���ܣ������ı����ȡ�
		��������
		���أ��ı�����
		ʾ����lineLength();
	--------------------------*/
	unsigned int lineLength();

	/*---------------------
		���ܣ���ȡԭ�ַ�����
		��������
		���أ��ַ���
		ʾ����str();
	---------------------*/
	std::string str();

	/*----------------------------------------------------------
		���ܣ���ѯ�Ƿ����ĳ�����ʹؼ��֡�
		�������ؼ���
		���أ�����ֵ������Ϊ�棬������Ϊ��
		ʾ����	iLine egLine("This is a test line.");
				std::cout << egLine.isThere("test") << endl;
			The output of the two lines will be "1".
	---------------------------------------------------------*/
	bool isThere(std::string _iKeyword_);

	/*---------------------------------------------------------
		���ܣ��ж��Ƿ�Ϊ�ա�
		��������
		���أ�����ֵ����Ϊ�棬����Ϊ��
		ʾ����	iLine egLine("This is a test line.");
				std::cout << egLine.isThere("test") << endl;
			The output of the two lines will be "1".
	---------------------------------------------------------*/
	bool isEmpty();

	/*----------------------------------------------------------
	���ܣ���ѯ�Ƿ����ĳ�����ʹؼ��ֵ�λ�á�
	�������ؼ���
	���أ��ؼ�������λ�õ���ţ���Ŵ�1��ʼ
	ʾ����	iLine egLine("This is a test line.");
			std::cout << egLine.whereIs("test") << endl;
		The output of the two lines will be "11".
	---------------------------------------------------------*/
	unsigned int whereIs(std::string _iKeyword_);

	/*-----------------------------------------------------
		���ܣ�ͳ�Ƶ��ʸ������Կո�Ϊ�ָ�����
		��������
		���أ����ʸ���
		ʾ����	iLine egLine("This is a test line.");
				std::cout << egLine.countWords() << endl;
			The output of the two lines will be "5".
	------------------------------------------------------*/
	unsigned int countWords();

	/*-----------------------------------------------------------
		���ܣ�����ĳ���ؼ��ֵ���š�
		�������ؼ���
		���أ��ؼ���������ţ���0��ʼ
		ʾ����	iLine egLine("This is a test line.");
				std::cout << egLine.before(6) << endl;
			The output of the two lines will be "This i".
	-----------------------------------------------------------*/
	unsigned int wordNumber(std::string _iKeyword_);

	/*-------------------------------------------------------
		���ܣ�ȡĳ��λ�ã�������ǰ�������ַ���
		������λ��
		���أ��Ӵ�
		ʾ����	iLine egLine("This is a test line.");
				std::cout << egLine.before(6) << endl;
			The output of the two lines will be "This i".
	------------------------------------------------------*/
	std::string before(unsigned int _charNumber_);

	/*-------------------------------------------------------
		���ܣ�ȡĳ��λ�ã���������������ַ���
		������λ��
		���أ��Ӵ�
		ʾ����	iLine egLine("This is a test line.");
				std::cout << egLine.after(6) << endl;
			The output of the two lines will be "s a test line.".
	------------------------------------------------------*/
	std::string after(unsigned int _charNumber_);

	/*-----------------------------------------------------
		���ܣ�����[]�������ȡ��i�����ʡ�
		������������ţ���0��ʼ
		���أ�����
		ʾ����	iLine egLine("This is a test line.");
				std::cout << egLine[3] << endl;
			The output of the two lines will be "test".
	------------------------------------------------------*/
	std::string operator[] (unsigned int _wNumber_);

	/*-----------------------------------------------
		���ܣ������������������ʹ��ֱ�������
		��������������������������ұ���iLine����
		���أ����������
		ʾ����std::cout<<myLine<<std::endl;
	-----------------------------------------------*/
	friend std::ostream &operator<< (std::ostream &out, const iLine &_line_);
};

/*---------------------------------------------------------------------
This function calls MATLAB engine to excute the command you enter.
The argument is the command you want to excute.
----------------------------------------------------------------------*/
//bool iMatlab(std::string _matlabCommand_);

#endif // !LINE_H
