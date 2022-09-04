#include "user.h"

// 功能：初始化成员变量，加载user.txt以及task.txt文件
// 参数：None
// 返回：None
User::User() {
	// 成员变量的初始化
	users.clear();
	userlist.clear();
	userstasklist.clear();
	userspermission.clear();
	currentuser = "\0";

	// 导入用户信息
	std::ifstream fin;
	std::string filepath = "./user.txt";
	fin.open(filepath, std::ios::in);
	if (!fin) { // 判断用户文件是否正常打开
		std::cerr << "Error: Fail to open user.txt file.";
		exit(1);
	}
	std::string username = "\0", password = "\0";
	int permission = 0;
	while (!fin.eof()) { // 依次获取每一行文本中的用户名、账号、用户权限
		fin >> username; fin >> password; fin >> permission;
		users.insert(std::pair<std::string, std::string>(username, password));
		userspermission.insert(std::pair<std::string, int>(username, permission));
		userlist.push_back(username);
	}
	fin.close();

	// 导入用户任务列表
	filepath = "./task.txt";
	fin.open(filepath, std::ios::in); // 题外话：如果读写文件一直不正常，
										  // 也有可能是文本文件格式损坏，just more practice，不能放弃
	if (!fin) { // 判断用户文件是否正常打开
		std::cerr << "Error: Fail to open task.txt file.";
		exit(1);
	}
	std::string line = "\0"; std::stringstream sstream; // 用字符串流处理含空格的字符串
	std::string task = "\0";
	while (getline(fin, line)) {   // 依次获取每一行文本中的用户名、任务
		sstream.str(line); // sstream中的rdstate()为std::ios_base::eofbit
						   // 导致复用失败
		sstream >> username;
		while (sstream >> task) {
			userstasklist[username].push_back(task);  // 注意直接写入到类成员当中
		}
		sstream.clear();   // .clear()清空标志状态，将错误状态调回goodbit
		sstream.str("");   // .str("")清空缓冲区，减少内存占用
						   // 收获：原理性问题还得去查stackoverflow
	}
	fin.close();
	return;
}

// 功能：判断输入的命令是否合规
// 参数：command输入的命令
// 返回：命令合规返回true
inline bool IsLegal(const std::string& command) {
	if (command == "Yes" || command == "No") return true;
	else return false;
}

// 这个函数有问题
// 功能：添加用户
// 参数：username当前用户名
// 返回：如果当前用户为管理员，创建新用户并返回true
bool User::AddUser(const std::string& username) {
	if (userspermission[username] != 1) {
		std::cerr << "Error: Current doesn't have corresponding operation authority.";
		return false;
	}
	else {
		std::string name = "\0";  std::string password = "\0";
		std::cout << "Please enter new user account name : ";  std::cin >> name;
		std::cout << std::endl;
		std::cout << "Please enter new user account password : ";  std::cin >> password;
		std::cout << std::endl << "Congratulation! Create a new user!";
		return true;
	}
}

// 功能：确认是否存在该用户，并核验用户密码
// 参数：None
// 返回：如果存在该用户并校验密码通过，则返回true
bool User::UserConfirmed() {
	std::string username = "\0", password = "\0";
	bool flag = false;
	std::cout << "Please enter your username: "; getline(std::cin, username);
	for (std::vector<std::string>::iterator it = userlist.begin(); it != userlist.end(); it++) {
		if (*it == username) {
			flag = true;
			break;
		}
	}
	if (!flag) {
		std::cout << "Error: User \"" << username << "\" doesn't exist." << std::endl;
		return false;
	}
	std::cout << "Please enter your password: "; getline(std::cin, password);
	std::map<std::string, std::string> usermap = GetUsers();
	if (usermap[username] == password) {
		currentuser = username;
		return true;
	}
	else {
		std::cout << "Error: Wrong password. " << std::endl;
		return false;
	}
}

/* 以下函数是对用户任务进行管理 */

// 功能：选定今天要完成的任务并展示
// 参数：numbertask任务与编号匹配的map
// 返回：None
void RespondChoose(const std::map<int, std::string>& numbertask) {
	std::vector<std::string> taskchoosed; taskchoosed.clear();
	std::cout << std::endl << "Please enter the serial number of task you wanna finish."
		<< "(Enter 0 to indicate the end of input.)" << std::endl
		<< "$ ";
	int serial = 0;
	while (std::cin >> serial && serial) {
		taskchoosed.push_back(numbertask.at(serial));
		std::cout << "$ ";
	}
	std::cout << std::endl << "Today your task are listed below:" << std::endl;
	serial = 1;
	for (std::vector<std::string>::iterator it = taskchoosed.begin(); it != taskchoosed.end(); it++) {
		std::cout << serial << "." << *it << std::endl;
		serial++;
	}
	return;
}


// 功能：添加任务到本地文件
// 参数：fin文件指针，content新任务内容
// 返回：None
void AddTaskToTXT(const std::string& content, const std::string& username) {
	std::ofstream filewrite;
	filewrite.open("./task.txt", std::ios::app); // 只能采用追加的方式进行写文件
	filewrite << "\n";		// 使程序识别换行
	filewrite << username;
	filewrite << " ";		// 满足用户名与任务之间有空格的格式要求
	filewrite << content;
	return;
}

// 功能：删除本地文件中的对应任务（把删除行的内容略过，重新写入文件）
// 参数：content被删除的任务内容，username用户名
// 返回：None
void DeleteTaskInTXT(const std::string& content, const std::string& username) {
	std::ifstream fileread;
	fileread.open("./task.txt", std::ios::in);
	std::string s = "\0";
	std::string target = username + " " + content;
	std::string finalresult = "\0";
	while (getline(fileread, s)) {
		if (s == target) continue; // 读文件时跳过需删除的内容
		else {
			finalresult += s;
			finalresult += "\n";
		}
	}
	fileread.close();
	std::ofstream filewrite;
	filewrite.open("./task.txt", std::ios::out);
	filewrite << finalresult; // 重新写入文件
	filewrite.close();
	return;
}


// 功能：添加用户任务(为RespondEdit服务的子函数)
// 参数：mylist原任务列表（会被修改），username用户名
// 返回：None
void AddTask(std::vector<std::string>& mylist, const std::string& username) {
	std::string addtask = "\0", command = "\0";
	while (1) { // 着重实现多次添加任务的功能
		std::cout << std::endl << "Please enter new task into your original task list.";
		std::cout << std::endl << "$ ";
		getline(std::cin, addtask);  mylist.push_back(addtask);
		AddTaskToTXT(addtask, username);
		std::cout << std::endl << "Add new task successfully!" << std::endl
			<< "Do you want to continue adding new task?(Yes/No): ";
		while (getline(std::cin, command)) { // 过滤掉除Yes或No之外的答案
			if (IsLegal(command)) break;
			else {
				std::cout << std::endl << "Error: Command is wrong. Please enter Yes or No." << std::endl
					<< "$ ";
			}
		}
		if (command == "Yes") continue; // 继续输入新任务
		else break; // 结束输入新任务
	}
	return;
}

// 功能：删除用户任务(为RespondEdit服务的子函数)
// 参数：numbertask任务与编号匹配的map， mylist原任务列表（会被修改），username用户名
// 返回：None
void DeleteTask(const std::map<int, std::string>& numbertask, std::vector<std::string>& mylist, const std::string& username) {
	int serial = 0;  std::string command = "\0";
	while (1) {
		std::cout << std::endl << "Please enter the serial of task you want to delete."
			<< std::endl << "$ ";
		std::cin >> serial; std::cin.ignore();
		DeleteTaskInTXT(numbertask.at(serial), username);
		// 找到对应编号的任务，并在myList中删除该任务
		for (std::vector<std::string>::iterator it = mylist.begin(); it != mylist.end(); it++) {
			if (*it == numbertask.at(serial)) { // const STL：[]操作符不是const成员函数，
												// 会增加STL新的条目，改变STL
				mylist.erase(it);
				break;
			}
		}
		std::cout << std::endl << "Delete task successfully!" << std::endl
			<< "Do you want to continue deleting task?(Yes/No): ";
		while (getline(std::cin, command)) { // 过滤掉除Yes/No之外的答案
			if (IsLegal(command)) break;
			else {
				std::cout << std::endl << "Error: Command is wrong. Please enter Yes or No." << std::endl
					<< "$ ";
			}
		}
		if (command == "Yes") continue; // 继续删除任务
		else break; // 结束删除任务
	}
	return;
}


// 功能：询问用户是否继续修改任务
// 参数：None
// 返回：退出修改任务返回true
bool WhetherContinue() {
	std::string command = "\0";
	std::cout << "Do you want to quit editing task?(Yes/No): ";
	while (1) { // 过滤掉非Yes / No的命令
		getline(std::cin, command);
		if (IsLegal(command)) break;
		else {
			std::cout << std::endl << "Error: Command is wrong. Please enter Yes or No." << std::endl
				<< "$ ";
		}
	}
	if (command == "Yes") return true;
	else return false;
}

// 功能：修改原任务列表。执行增加或删除任务的操作，最后展示今天的任务列表
// 参数：numbertask任务与编号匹配的map（会被修改），mylist原任务列表（会被修改），username用户名
// 返回：None
void RespondEdit(std::map<int, std::string>& numbertask, std::vector<std::string>& mylist, const std::string& username) {
	std::string command = "\0"; int serial = 0;
	while (1) {
		std::cout << std::endl << "Now enter one of these commands to continue. " << std::endl
			<< "1.add task" << std::endl
			<< "2.delete task" << std::endl
			<< "$ ";
		getline(std::cin, command);
		// 将增、删、改任务，这三个功能分流到各自对应函数实现
		if (command == "add task") {
			AddTask(mylist, username);
			if (WhetherContinue()) break;
			else continue;
		} else if (command == "delete task") {
			  DeleteTask(numbertask, mylist, username);
			  if (WhetherContinue()) break;
			  else continue;
		} else {
			  std::cout << std::endl << "Error: Command is wrong. Please enter again.";
		}
	}
	// 增加或删除任务都改变了原任务列表，因此重新编号
	numbertask.clear();  serial = 1;
	for (std::vector<std::string>::iterator it = mylist.begin(); it != mylist.end(); it++) {
		numbertask.insert(std::pair<int, std::string>(serial, *it));  serial++;
	}
	std::cout << std::endl << "After editing, your task list is: " << std::endl;
	serial = 1;
	for (std::vector<std::string>::iterator it = mylist.begin(); it != mylist.end(); it++) {
		std::cout << serial << "." << *it << std::endl; serial++;
	}
	RespondChoose(numbertask);
	return;
}


// 功能：罗列任务列表，选定今日任务并标记，执行增加、修改或删除任务的操作
// 参数：username用户名
// 返回：None
void User::UserTaskDetail(const std::string& username) {
	std::map<std::string, std::vector<std::string>> mytask = GetUsersTaskList();
	std::vector<std::string> mylist = mytask[username];
	std::cout << std::endl << "Dear user: " << username << std::endl
		<< "Your indentity has been confirmed. " << std::endl
		<< "Your original task are listed below: " << std::endl;
	std::map<int, std::string> numbertask; // 将任务与编号对应
	numbertask.clear();
	int serial = 1;
	for (std::vector<std::string>::iterator it = mylist.begin(); it != mylist.end(); it++) {
		std::cout << serial << "." << *it << std::endl;
		numbertask.insert(std::pair<int, std::string>(serial, *it));
		serial++;
	}
	std::cout << std::endl << "Now enter one of these commands to continue: " << std::endl
		<< "1.choose today's task" << std::endl
		<< "2.edit task list" << std::endl
		<< "$ ";
	std::string command = "\0";
	while (1) { // 将choose和edit两个命令效果分流到各自对应函数实现
		getline(std::cin, command); // 注意缓冲区在之前的输入中是否清空
		if (command == "choose today's task") {
			RespondChoose(numbertask);
			break;
		}
		else if (command == "edit task list") {
			RespondEdit(numbertask, mylist, username);
			break;
		}
		else {
			std::cout << std::endl << "Error: Command is wrong. Please enter again." << std::endl
				<< "$ ";
		}
	}
	std::cout << std::endl << "Ending. Good luck!" << std::endl;
	return;
}

/* 以下函数是是提供对私有数据成员的调用接口*/

// 功能：提供私有成员变量users调用接口
// 参数：None
// 返回：None
const std::map<std::string, std::string> User::GetUsers() {
	return this->users;
}

// 功能：提供私有成员变量usersPermission调用接口
// 参数：None
// 返回：None
const std::map<std::string, int> User::GetUsersPermission() {
	return this->userspermission;
}

// 功能：提供私有成员变量usersTaskList调用接口
// 参数：None
// 返回：None
std::map<std::string, std::vector<std::string>> User::GetUsersTaskList() {
	return this->userstasklist;
}
