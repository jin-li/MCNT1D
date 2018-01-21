#include <iostream>
#include <string>
#include "MonteCarlo.h"
#include "rand.h"

int main(int argc, char* argv[])
{
	
	std::string inputFileName, outputFileName;
	//��ȡ����������
	if (argc == 2) {
		//ֻ����������ʱ������ļ�ʹ��Ĭ���ļ���
		inputFileName = argv[1];
		outputFileName = inputFileName + ".out";
	}
	else if (argc == 3) {
		//����������ʱ������ļ�ʹ���û�������ļ���
		inputFileName = argv[1];
		outputFileName = argv[2];
	}
	//������������
	else {
		std::cout << "Error! E000:Incorrect intput! Please use \"program_name input_file_name output_file_name\"" << std::endl;
	}

	MonteCarlo mcnt(inputFileName, outputFileName);
	mcnt.run();
	

	//for (int i = 0; i < 100; i++) {
	//	std::cout << random() << std::endl;
	//}
	system("pause");
	return 0;
}

