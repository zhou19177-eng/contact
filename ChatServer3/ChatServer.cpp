// ChatServer.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "LogicSystem.h"
#include <csignal>
#include <thread>
#include <mutex>
#include "AsioIOServicePool.h"
#include "CServer.h"
#include "ConfigMgr.h"
#include "RedisMgr.h"
#include "ChatServiceImpl.h"
#include "const.h"

using namespace std;
bool bstop = false;
std::condition_variable cond_quit;
std::mutex mutex_quit;

int main()
{
	auto& cfg = ConfigMgr::Inst();
	auto server_name = cfg["SelfServer"]["Name"];
	try {
		auto pool = AsioIOServicePool::GetInstance();
		
		RedisMgr::GetInstance()->HSet(LOGIN_COUNT, server_name, "0");
		//将登录数设置为0
		//如果用大白话来说，它的作用就是：新店开张，向总公司的系统里挂牌报备，并宣布“目前本店客流量为 0”。

		/*我们可以从以下几个维度详细拆解它的精妙作用：

			1. 服务注册（向全网宣告“我上线了”）
			在分布式系统中，可能有几十台 ChatServer。网关或状态服务器怎么知道当前有哪些 ChatServer 是活着的呢？
			代码中的 LOGIN_COUNT 就像是 Redis 里的一个“全局在线服务器花名册”。
			当这台 ChatServer（比如名字叫 chatserver1）刚启动时，它把自己写进这个花名册，这就相当于完成了服务注册。
			2. 初始化负载（在线人数清零）
			这里的 "0" 代表当前连接到这台服务器的客户端数量为 0。
			因为服务器是刚启动的，之前的内存数据全空了，不管上一次宕机前有多少人连着，现在一切重新开始，所以必须重置为 0。
			3. 为 StatusServer（总调度中心）提供负载均衡依据
			您还记得 StatusServer 负责给刚登录的用户分配服务器吗？
			在您提供的 StatusServiceImpl.cpp 代码中，有一段逻辑（虽然部分被注释或改为了轮询）
			是会去查 Redis 的：RedisMgr::GetInstance()->HGet(LOGIN_COUNT, server.second.name)。
			状态服务器正是通过读取这行代码写进去的 LOGIN_COUNT，来对比所有 ChatServer 的在线人数，从而把新登录的用户分配给人数最少、最空闲的那台机器。*/



		Defer derfer ([server_name]() {
				RedisMgr::GetInstance()->HDel(LOGIN_COUNT, server_name);
				RedisMgr::GetInstance()->Close();
			});



		
		boost::asio::io_context  io_context;
		auto port_str = cfg["SelfServer"]["Port"];
		//创建Cserver智能指针
		auto pointer_server = std::make_shared<CServer>(io_context, atoi(port_str.c_str()));
		//启动定时器
		pointer_server->StartTimer();




		//定义一个GrpcServer

		std::string server_address(cfg["SelfServer"]["Host"] + ":" + cfg["SelfServer"]["RPCPort"]);
		
		ChatServiceImpl service;
		/*逻辑：创建了 ChatServiceImpl 类的对象。
		作用：这就是我们之前聊过的**“跨区收件站”**！当其他聊天服务器（比如上海服）有消息要发给这台服务器
		比如北京服）的用户时，就是由这个 service 对象里的函数（如 NotifyTextChatMsg、NotifyAddFriend）
		来具体接收和处理的。*/

		
		grpc::ServerBuilder builder;
		// 监听端口和添加服务
		builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
		builder.RegisterService(&service);
		/*逻辑：使用 gRPC 官方提供的 ServerBuilder（建造者）来组装服务器。
		AddListeningPort：让服务器监听刚才拼接好的地址，并且使用 InsecureServerCredentials
		（不加密的明文传输，因为是机房内部服务器之间通信，追求速度，通常不需要像外网那样做复杂的 SSL/TLS 加密）。
		RegisterService：把刚才建好的“收件站” (service) 挂载到这个 gRPC 服务器上，告诉系统：
		“以后收到这个频道的请求，全交接给它处理”。*/



		
		service.RegisterServer(pointer_server);
		/*逻辑：把刚才启动的面向外部客户端的 TCP 服务器指针（也就是我们说的“保安队长” pointer_server）交给了内部的 gRPC 服务。
		作用：为什么要这么做？想象一下，如果【上海服】发来一个跨服踢人指令：“请把你那边的张三踢下线”。
		这台服务器的 ChatServiceImpl（收件站）接到指令后，自己是没有权限拔张三网线的。有了这行代码，它就可以通过 _p_server->ClearSession(...)
		直接呼叫本地的 TCP 服务器去切断用户的连接。*/
		
		std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
		std::cout << "RPC Server listening on " << server_address << std::endl;
		// 构建并启动gRPC服务器
		/*逻辑：正式构建并启动这个 gRPC 服务器，并在控制台打印一行日志宣告启动成功。
		总结：
		这段代码让一台 ChatServer 拥有了**“双重身份”**：它既是面向千千万万用户的 TCP 服务器，
		又是面向集群内部其他兄弟节点的 gRPC 服务器。这种设计是现代微服务架构中最经典、最标准的做法！*/

		
		//单独启动一个线程处理grpc服务
		std::thread  grpc_server_thread([&server]() {
				server->Wait();
			});

	
		
		boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);
		signals.async_wait([&io_context, pool, &server](auto, auto) {
			io_context.stop();
			pool->Stop();
			server->Shutdown();
			});
		/*逻辑：这里向操作系统注册了两个信号：SIGINT（通常是运维在黑框框里按 Ctrl+C）和 SIGTERM（通常是输入 kill 命令准备关服升级）。
		作用：如果没有这段代码，你一按 Ctrl+C，服务器进程瞬间被系统强杀（猝死），很多正在处理的聊天消息就丢失了。
		有了这段代码，服务器收到关机信号后，不会立刻死掉，而是会触发一个异步回调，从容地执行三件事：
		io_context.stop()：停止底层的主事件循环，不再接收外部新用户的请求。
		pool->Stop()：通知后台的 Asio 线程池，把手头剩下的活儿干完再下班。
		server->Shutdown()：通知内部的 gRPC 服务器，停止接待其他兄弟服务器发来的跨服请求。
		这就是高端服务端开发中常说的**“优雅停机 (Graceful Shutdown)”**。*/
		
	
		
		LogicSystem::GetInstance()->SetServer(pointer_server);
		//将Cserver注册给逻辑类方便以后清除连接
		/*逻辑：这一步咱们刚才聊过，把底层网络模块（也就是保安队长 pointer_server）的指针交给了业务大管家（LogicSystem）。
		作用：确保在服务器真正运转起来之前，业务层就已经拿到了底层网络的控制权，
		从而在后续遇到“异地登录”时，能够顺利地踢掉旧设备的网线。*/
		
		io_context.run();
		/*逻辑：这是 Boost.Asio 框架中最最最核心的一句阻塞代码。

		作用：前面的所有代码其实都只是在“招兵买马”、“定规矩”、“布置场地”。
		只有当 io_context.run() 执行的这一瞬间，当前线程才真正进入了无限循环的监听状态。
		只要没有触发上面的关机信号，代码就会一直“卡”在这行，默默处理着成千上万的并发事件。*/

		
		grpc_server_thread.join();
		pointer_server->StopTimer();
		/*逻辑：这两行代码只有在刚才的“紧急停机按钮”被按下，导致 io_context.run() 退出循环后，才会被执行到。
		作用：join()：主线程乖乖阻塞在这里，等待那个负责跑 gRPC 服务的独立线程（grpc_server_thread）安全退出，
		确保不会出现“主线程跑了，子线程变成孤儿”的崩溃情况。

		StopTimer()：把之前保安队长开启的“心跳巡逻定时器”彻底销毁，回收资源。*/
		return 0;
	}
	catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << endl;
	}

}
//int main()
//{
//    try {
//        auto& cfg = ConfigMgr::Inst();
//        auto pool = AsioIOServicePool::GetInstance();
//        boost::asio::io_context io_context;
//        boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);
//        signals.async_wait([&io_context, pool](auto, auto) {
//            io_context.stop();
//            pool->Stop();
//            });
//
//        auto port_str = cfg["SelfServer"]["Port"];
//        CServer s(io_context, atoi(port_str.c_str()));
//        io_context.run();
//    }
//    catch (std::exception& e) {
//        std::cerr << "Exception: " << e.what() << endl;
//    }
//}
