#include <time.h>
#include "loadend.h" // 收获：懂得合理使用.h文件和.cpp文件
#include "user.h"
using namespace std;

string current_user = "\0";
string s = "\0";
User* usermanage = new User(); // 声明指针之后一定要实例化

// 功能：介绍项目背景与目的
// 参数：None
// 返回：None
void README() {
	cout << "README: " << endl
		<< "  This Self-check program was launched by Sbk on September 1st, 2022." << endl
		<< "  The purpose of this program is to stimulate users to finish their" << endl
		<< "  task on time and they will become better certainly. " << endl << endl;
}

// 功能：用户登录，获取用户对应任务列表，记录用户今日任务
// 参数：None
// 返回：None
void Comunicate() {
	while (1) {
		if (usermanage->UserConfirmed()) break;
		else continue;
	}
	usermanage->UserTaskDetail(usermanage->currentuser);
	return;
}

// 功能：显示当前时间，并给出相应天数的结束语，退出程序
// 参数：None
// 返回：None
void End() {
	time_t result = time(NULL);
	char str[26];
	cout << "Current time: ";
	ctime_s(str, sizeof(str), &result); // 将给定时间转化为当地日历并转化为文本表示
	for (int index = 0; index < 26; index++) cout << str[index];
	cout << "Mutual encouragement: ";
	GiveSentence(str[9] - '1');
	return;
}

int main() {
	README();
	Comunicate();
	End();
	delete usermanage;
	return 0;
}