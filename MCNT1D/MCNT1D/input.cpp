#include "MonteCarlo.h"
#include "exception.h"
//#include "line.h"
#include "input.h"

#include <sstream>

iLine translate(std::string _line) {
	std::string lineWithoutComment = _line;
	if (_line.empty()) {
		iLine __tempLine("");
		return __tempLine;
	}
	//ɾȥע��
	for (int i = 0; i < _line.length() - 1; i++) {
		if (_line[i] == '/'&&_line[i + 1] == '/') {
			lineWithoutComment = _line.substr(0, i);
			break;
		}
	}
	//ȫ��תΪСд
	std::transform(lineWithoutComment.begin(), lineWithoutComment.end(), lineWithoutComment.begin(), ::tolower);
	iLine __tempLine(lineWithoutComment);
	return __tempLine;
}

startFlag getStartKeyword(iLine _line) throw(mcException) {
	if (_line.isEmpty())throw("Warning! W001:Empty line for getStartKeyword!");
	std::string __firstWord = _line[0];
	if (__firstWord == "start_calculationcondition") {
		return CC;
	}
	else if (__firstWord == "start_calculationparameter") {
		return CP;
	}
	else if (__firstWord == "start_material") {
		return MT;
	}
	else if (__firstWord == "start_cell") {
		return CL;
	}
	else {
		throw("Error! E002:Unknow start keyword!");
	}
}



double s2d(std::string _string) {
	std::stringstream __ssTemp;
	double __result = 0;
	__ssTemp << _string;
	__ssTemp >> __result;
	return __result;
}

int s2i(std::string _string) {
	std::stringstream __ssTemp;
	int __result = 0;
	__ssTemp << _string;
	__ssTemp >> __result;
	return __result;
}

void MonteCarlo::readInput() throw(mcException) {
	if (!(this->inputFile)) throw("Error! E001: Cannot open input file!");
	//�Ƿ����һ��section�ı��
	bool isReadSection = false;
	while (!(this->inputFile).eof()) {
		//�������ļ��ж�ȡһ�в�תΪiLine����
		std::string lineTemp;
		std::getline(this->inputFile, lineTemp);
		iLine lineRead = translate(lineTemp);

		//����ע������Ϊ��
		while (!lineRead.isEmpty()) {
			//���������һ��section���ٶ�һ���������������
			if (isReadSection) {
				std::getline(this->inputFile, lineTemp);
				lineRead = translate(lineTemp);
				isReadSection = false;
				break;
			}
			//��һ����ע���У�Ӧ���ǿ�ʼ���
			startFlag sf = getStartKeyword(lineRead);
			switch (sf)
			{
			//��ȡ��������
			case CC: {
				std::string ccLineTemp;
				iLine ccLineRead;
				std::map<std::string, int> calculationConditionMap;
				do {
					//��ȡ��������
					std::getline(this->inputFile, ccLineTemp);
					ccLineRead = translate(ccLineTemp);
					//�ж϶������ǲ��Ǽ�������
					if (ccLineRead.countWords() == 2) {
						//�����������
						calculationConditionMap.insert(getCalculationCondition<int>(ccLineRead));
					}
				} while (ccLineRead[0] != "end_calculationcondition");
				//�������м��������󣬶Ե�ǰMonteCarlo�����ʼ��
				this->cellNumber = calculationConditionMap["cellnumber"];
				this->groupNumber = calculationConditionMap["groupnumber"];
				this->materialNumber = calculationConditionMap["materialnumber"];
				this->repetitiveNumber = calculationConditionMap["repetitivenumber"];

				//��ʼ��MonteCarlo�е�material
				this->inputMaterial.resize(materialNumber + 1);
				for (int i = 0; i <= materialNumber; i++) {
					inputMaterial[i].nuSigmaF.resize(groupNumber + 1);
					inputMaterial[i].sigmaT.resize(groupNumber + 1);
					inputMaterial[i].sigmaS.resize(groupNumber + 1);
					inputMaterial[i].yield.resize(groupNumber + 1);
					//sigmaS��yield�Ƕ�ά������groupNumber*groupNumber
					for (int j = 0; j <= groupNumber; j++) {
						inputMaterial[i].sigmaS[j].resize(groupNumber + 1);
						inputMaterial[i].yield[j].resize(groupNumber + 1);
					}
				}

				//��ʼ��MonteCarlo�е�geometry��cell
				this->inputcell.resize(cellNumber + 1);

				isReadSection = true;
				//����swaitch���ص�ѭ�������ҿ�ʼ���
				break;
			}
			//��ȡ����
			case MT: {
				for (int i = 1; i <= this->materialNumber; i++) {
					//��ȡ������Ϣ��תΪiLine
					std::string mtLineTemp;
					std::getline(this->inputFile, mtLineTemp);

					iLine mtLine = translate(mtLineTemp);
					for (int j = 1; j <= this->groupNumber; j++) {
						if (!mtLine.isThere("nusigmaf")) throw("Error! E004: nuSigmaF not found!");
						if (!mtLine.isThere("sigmat")) throw("Error! E005: sigmaT not found!");
						if (!mtLine.isThere("sigmas")) throw("Error! E006: sigmaS not found!");
						if (!mtLine.isThere("yield")) throw("Error! E007: yield not found!");
						//�洢��ȡ����nuSigmaF��sigmaTֵ
						(this->inputMaterial[i]).nuSigmaF[j] = s2d(mtLine[mtLine.wordNumber("nusigmaf") + j - 1]);
						(this->inputMaterial[i]).sigmaT[j] = s2d(mtLine[mtLine.wordNumber("sigmat") + j - 1]);
						//sigmaS��yield��ֵ�Ƕ�ά�ģ���Ҫ����һ����Ⱥѭ��
						double sigmaSsum = 0;
						for (int k = 1; k <= this->groupNumber; k++) {
							(this->inputMaterial[i]).sigmaS[j][k] = s2d(mtLine[mtLine.wordNumber("sigmas") + (j-1) * groupNumber + k-1]);
							sigmaSsum += this->inputMaterial[i].sigmaS[j][k];
							(this->inputMaterial[i]).yield[j][k] = s2d(mtLine[mtLine.wordNumber("yield") + (j-1) * groupNumber + k-1]);
						}
						//����ĳȺ��ɢ�����
						this->inputMaterial[i].sigmaS[j][0] = sigmaSsum;
					}
				}
				std::string endMaterial;
				std::getline(this->inputFile, endMaterial);
				isReadSection = true;

				break;
			}
			//��ȡ�ظ�դԪ��Ϣ
			case CL: {
				//�����ظ�դԪ����ʼ����
				double repetitiveCellPosition = 0;
				for (int i = 0; i < this->repetitiveNumber; i++) {
					//��ȡ�ظ�դԪ��Ϣ
					std::string clLineTemp;
					std::getline(this->inputFile, clLineTemp);
					iLine clLine(clLineTemp);

					//��ȡդԪ������ÿ��ѭ������һ�Σ����ľ��ǽ��
					this->cellNumber = s2i(clLine[2]);

					for (int j = s2i(clLine[1]); j <= s2i(clLine[2]); j++) {
						//��ʼ��MonteCarloդԪ
						this->inputcell[j].left = (j-s2i(clLine[1]))*s2d(clLine[3]) + repetitiveCellPosition;
						this->inputcell[j].right = (j+1-s2i(clLine[1]))*s2d(clLine[3]) + repetitiveCellPosition;
						this->inputcell[j].mat = this->inputMaterial[s2i(clLine[5])];
					}
					//������һ���ظ�դԪ����ʼ����
					repetitiveCellPosition += (s2i(clLine[2]) - s2i(clLine[1]) + 1)*s2d(clLine[3]);
	
					this->inputGeometry.setRight(repetitiveCellPosition);
				}
				this->inputGeometry.geometryCell = this->inputcell;
				std::string endCell;
				std::getline(this->inputFile, endCell);

				isReadSection = true;
				break;
			}
			//��ȡ�������
			case CP: {
				std::string cpLineTemp;
				iLine cpLine;
				std::map<std::string, double> calculationParameterMap;
				do {
					//��ȡ��������
					std::getline(this->inputFile, cpLineTemp);
					cpLine= translate(cpLineTemp);
					//����������������Ϣ
					if (cpLine[0] == "neutron") {
						this->neutronNumber = s2i(cpLine[1]);
						this->totalGenerationNumber = s2i(cpLine[2]);
						this->inactiveGenerationNumber = s2i(cpLine[3]);
					}
					else if (cpLine[0]=="boundarycondition") {
						this->inputGeometry.setLeftBoundaryCondition(s2i(cpLine[1]));
						this->inputGeometry.setRightBoundaryCondition(s2i(cpLine[2]));
					}
					else if (cpLine.countWords()==2) {
						calculationParameterMap.insert(getCalculationCondition<double>(cpLine));
					}
				} while (cpLine[0] != "end_calculationparameter");
				//�洢Ȩ����С
				this->weightMax = calculationParameterMap["weightmax"];
				this->weightMin = calculationParameterMap["weightmin"];
				isReadSection = true;

				break;
			}

			default:
				break;
			}
		}
	}
	this->inputFile.close();
}