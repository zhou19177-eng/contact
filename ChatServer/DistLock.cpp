#include "DistLock.h"
#include <thread>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <hiredis.h>


//定义单例模式
DistLock& DistLock::Inst() {
	static DistLock lock;
	return lock;
}

// 使用 Boost UUID 生成全局唯一标识符（UUID）
static std::string generateUUID() {
	boost::uuids::uuid uuid = boost::uuids::random_generator()();
	return to_string(uuid);
}

// 尝试获取锁，返回锁的唯一标识符（UUID），如果获取失败则返回空字符串
std::string DistLock::acquireLock(redisContext* context, const std::string& lockName,
    int lockTimeout, int acquireTimeout) {
    std::string identifier = generateUUID();
    std::string lockKey = "lock:" + lockName;
    /*通俗解释：你要去抢充电宝，为了防止等会儿还充电宝的时候，别人把你抢到的给还了，
    你必须给自己随机生成一个全世界独一无二的密码（identifier）。

    面试亮点：面试官经常问“锁的值为什么不能随便写个 1，而要用 UUID？” 
    你就可以回答：“为了防止锁被误删。将来释放锁的时候，必须拿着这个 UUID 去比对，
    证明‘这把锁确实是我加的’，我才能解开它。”*/
    auto endTime = std::chrono::steady_clock::now() + std::chrono::seconds(acquireTimeout);

    while (std::chrono::steady_clock::now() < endTime) {
    /*通俗解释：你不可能为了一个充电宝在火车站等一辈子。acquireTimeout 就是你的耐心极限（比如 5 秒）。
    在这个时间段内，你会一直盯着机柜看（while 循环），一旦超过这个时间还没抢到，你就直接放弃回家（return ""）。

    术语：这在并发编程里叫**“带有超时机制的自旋锁（Spin Lock with Timeout）”***/
        // 使用 SET 命令尝试加锁：SET lockKey identifier NX EX lockTimeout
        redisReply* reply = (redisReply*)redisCommand(context, "SET %s %s NX EX %d",
            lockKey.c_str(), identifier.c_str(), lockTimeout);
        if (reply != nullptr) {
            // 判断返回结果是否为 OK
            if (reply->type == REDIS_REPLY_STATUS && std::string(reply->str) == "OK") {
                freeReplyObject(reply);
                return identifier;
            }
            freeReplyObject(reply);
        }
        // 暂停 1 毫秒后重试，防止忙等待
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return "";
}

// 释放锁，只有锁的持有者才能释放，返回是否成功
bool DistLock::releaseLock(redisContext* context, const std::string& lockName,
    const std::string& identifier) {
    std::string lockKey = "lock:" + lockName;
    // Lua 脚本：判断锁标识是否匹配，匹配则删除锁
    const char* luaScript = "if redis.call('get', KEYS[1]) == ARGV[1] then \
                                return redis.call('del', KEYS[1]) \
                             else \
                                return 0 \
                             end";
    // 调用 EVAL 命令执行 Lua 脚本，第一个参数为脚本，后面依次为 key 的数量、key 以及对应的参数
    redisReply* reply = (redisReply*)redisCommand(context, "EVAL %s 1 %s %s",
        luaScript, lockKey.c_str(), identifier.c_str());
    bool success = false;
    if (reply != nullptr) {
        // 当返回整数值为 1 时，表示成功删除了锁
        if (reply->type == REDIS_REPLY_INTEGER && reply->integer == 1) {
            success = true;
        }
        freeReplyObject(reply);
    }
    return success;
}