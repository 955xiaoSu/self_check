#pragma once
#ifndef USER_H_
#define USER_H_
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <fstream>
#include <vector>

class User {
private:
	std::map<std::string, std::string> users;   // 用户的账号与密码
	std::map<std::string, int> userspermission; // 用户的权限标识，1为管理员，2为普通用户
	std::map<std::string, std::vector<std::string>> userstasklist;  // 用户的任务列表

public:
	User();
	std::string currentuser; // 当前用户名
	std::vector<std::string> userlist; // 记录所有用户名
	std::map<std::string, std::vector<std::string>> GetUsersTaskList();
	const std::map<std::string, std::string> GetUsers();
	const std::map<std::string, int> GetUsersPermission();
	void UserTaskDetail(const std::string& username);
	bool UserConfirmed();
	bool AddUser(const std::string& username);
};

#endif
