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
	
	system("pause");
	return 0;
}

/*
int main() {
int i[10] = { 0 };
for (int j = 0; j < 1000000; j++) {
double rd = (randomDirection()+1)/2;
if (rd >= 0 && rd < 0.1) i[0]++;
else if (rd >= 0.1&&rd < 0.2) i[1]++;
else if (rd >= 0.2&&rd < 0.3) i[2]++;
else if (rd >= 0.3&&rd < 0.4) i[3]++;
else if (rd >= 0.4&&rd < 0.5) i[4]++;
else if (rd >= 0.5&&rd < 0.6) i[5]++;
else if (rd >= 0.6&&rd < 0.7) i[6]++;
else if (rd >= 0.7&&rd < 0.8) i[7]++;
else if (rd >= 0.8&&rd < 0.9) i[8]++;
else if (rd >= 0.9&&rd < 1.0) i[9]++;
}
for (int j = 0; j < 10; j++)std::cout << i[j] / 1000000.0 << std::endl;

system("pause");
return 0;
}
*/
