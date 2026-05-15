#include "StatusServiceImpl.h"
#include "ConfigMgr.h"
#include "const.h"
#include "RedisMgr.h"
#include <climits>

std::string generate_unique_string() {
	// 创建UUID对象
	boost::uuids::uuid uuid = boost::uuids::random_generator()();

	// 将UUID转换为字符串
	std::string unique_string = to_string(uuid);

	return unique_string;
}

Status StatusServiceImpl::GetChatServer(ServerContext* context, const GetChatServerReq* request, GetChatServerRsp* reply)
{
	std::string prefix("llfc status server has received :  ");
	const auto& server = getChatServer();
	reply->set_host(server.host);
	reply->set_port(server.port);
	reply->set_error(ErrorCodes::Success);
	reply->set_token(generate_unique_string());
	insertToken(request->uid(), reply->token());
	return Status::OK;
}

StatusServiceImpl::StatusServiceImpl()
{
	auto& cfg = ConfigMgr::Inst();
	auto server_list = cfg["chatservers"]["Name"];

	std::vector<std::string> words;

	std::stringstream ss(server_list);
	std::string word;

	while (std::getline(ss, word, ',')) {
		words.push_back(word);
	}

	for (auto& word : words) {
		if (cfg[word]["Name"].empty()) {
			continue;
		}

		ChatServer server;
		server.port = cfg[word]["Port"];
		server.host = cfg[word]["Host"];
		server.name = cfg[word]["Name"];
		_servers[server.name] = server;
	}

}



ChatServer StatusServiceImpl::getChatServer() {
	std::lock_guard<std::mutex> guard(_server_mtx);

	// 1. 【关键】先判断列表是否为空，防止崩溃，并打印日志
	if (_servers.empty()) { // ⬅️ 新增安全检查
		std::cout << ">>> [ERROR] getChatServer failed: No ChatServers registered! _servers is empty." << std::endl; // ⬅️ 新增日志
		return ChatServer(); // 返回一个空对象，或者抛出异常，千万别继续往下走
	}

	// ================= 轮询逻辑开始 =================
  // 1. 获取所有服务器及其名称的列表
	auto it = _servers.begin();

	// 2. 根据计数器移动迭代器 (实现 A -> B -> A -> B)
	// 比如 size是2，index是0 -> 第1个; index是1 -> 第2个; index是2 -> 回到第1个
	std::advance(it, server_index % _servers.size());

	// 3. 选定服务器
	auto targetServer = it->second;

	// 4. 计数器加 1
	server_index++;

	std::cout << ">>> [DEBUG] Round-Robin selected: " << targetServer.name << std::endl;
	return targetServer;
	// ================= 轮询逻辑结束 =================
	//auto minServer = _servers.begin()->second;

	//// ... 获取 minServer 的 count 逻辑 ...
	//auto count_str = RedisMgr::GetInstance()->HGet(LOGIN_COUNT, minServer.name);
	//if (count_str.empty()) {
	//	minServer.con_count = INT_MAX;
	//}
	//else {
	//	minServer.con_count = std::stoi(count_str);
	//}

	//// 使用范围基于for循环
	//for (auto& server : _servers) {
	//	if (server.second.name == minServer.name) {
	//		continue;
	//	}

	//	auto count_str = RedisMgr::GetInstance()->HGet(LOGIN_COUNT, server.second.name);
	//	if (count_str.empty()) {
	//		server.second.con_count = INT_MAX;
	//	}
	//	else {
	//		server.second.con_count = std::stoi(count_str);
	//	}

	//	// 3. (可选) 打印遍历过程中的每个服务器信息
	//	// std::cout << ">>> [DEBUG] Checking " << server.second.name << " load: " << server.second.con_count << std::endl; // ⬅️ 可选日志

	//	if (server.second.con_count < minServer.con_count) {
	//		minServer = server.second;
	//	}
	//}

	//// 4. 打印最终选中的服务器
	//std::cout << ">>> [DEBUG] Best server selected: " << minServer.name << " (Load: " << minServer.con_count << ")" << std::endl; // ⬅️ 新增日志

	//return minServer;
}
//ChatServer StatusServiceImpl::getChatServer() {
//	std::lock_guard<std::mutex> guard(_server_mtx);
//	auto minServer = _servers.begin()->second;
//	
//	auto count_str = RedisMgr::GetInstance()->HGet(LOGIN_COUNT, minServer.name);
//	if (count_str.empty()) {
//		//不存在则默认设置为最大
//		minServer.con_count = INT_MAX;
//	}
//	else {
//		minServer.con_count = std::stoi(count_str);
//	}
//
//
//	// 使用范围基于for循环
//	for ( auto& server : _servers) {
//		
//		if (server.second.name == minServer.name) {
//			continue;
//		}
//	
//		auto count_str = RedisMgr::GetInstance()->HGet(LOGIN_COUNT, server.second.name);
//		if (count_str.empty()) {
//			server.second.con_count = INT_MAX;
//		}
//		else {
//			server.second.con_count = std::stoi(count_str);
//		}
//	
//		if (server.second.con_count < minServer.con_count) {
//			minServer = server.second;
//		}
//	}
//
//	return minServer;
//}

Status StatusServiceImpl::Login(ServerContext* context, const LoginReq* request, LoginRsp* reply)
{
	auto uid = request->uid();
	auto token = request->token();

	std::string uid_str = std::to_string(uid);
	std::string token_key = USERTOKENPREFIX + uid_str;
	std::string token_value = "";
	bool success = RedisMgr::GetInstance()->Get(token_key, token_value);
	if (success) {
		reply->set_error(ErrorCodes::UidInvalid);
		return Status::OK;
	}
	
	if (token_value != token) {
		reply->set_error(ErrorCodes::TokenInvalid);
		return Status::OK;
	}
	reply->set_error(ErrorCodes::Success);
	reply->set_uid(uid);
	reply->set_token(token);
	return Status::OK;
}

void StatusServiceImpl::insertToken(int uid, std::string token)
{
	std::string uid_str = std::to_string(uid);
	std::string token_key = USERTOKENPREFIX + uid_str;
	RedisMgr::GetInstance()->Set(token_key, token);
	std::cout << "token_key is" << token_key;
}

