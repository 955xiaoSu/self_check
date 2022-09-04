#include "loadend.h"

// 功能：加载end.txt文件，并返回此天数所对应的语句
// 参数：day天数
// 返回：string
std::string LoadEndTxt(const int& day) {
	std::string filepath = "./end.txt";
	std::ifstream fin;
	fin.open(filepath, std::ios::in);
	if (!fin) { // 对是否成功打开文件进行判断
		std::cerr << "Error: Fail to open end.txt file.";
		exit(1);
	}
	std::string s = "\0";  int line = 1;
	while (!fin.eof() && line < day) { // 跳过前面（day - 1）行字符串
		std::getline(fin, s);	line++;
	}
	getline(fin, s);
	fin.close();
	return s;
}

// 功能：打印对应天数的语句
// 参数：day天数
// 返回：None
void GiveSentence(const int& day) {
	std::string ans = LoadEndTxt(day);
	std::cout << ans << std::endl;
	return;
}
