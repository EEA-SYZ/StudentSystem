#ifndef __TRANSMITTER_HPP__
#define __TRANSMITTER_HPP__

/**
 * @brief 通过文件系统进行进程间通信，类比起网络的作用。
 * @namespace trm
 * 
 * @par REQUIRES
 * NONE
 * 
 * @par PROVIDES
 * TO_COMPLETE
 */

#include "file_system.hpp"

namespace trm{

namespace rqs{
    /**
     * @brief 检查服务端在线状态。
     * @param NONE
     * @return YES
     */
    const std::string CHECK_ONLINE = "Are you on?";
    /**
     * @brief 检查帐号是否有效。
     * @param username 帐号
     * @param password 密码
     * @return YES or NO
     */
    const std::string CHECK_ACCOUNT = "CK ACC";
    /**
     * @brief 修改成绩。
     * @param studentCode 学生编号
     * @param score 新成绩
     * @return YES or NO
     */
    const std::string MODIFY_SCORE = "MOD SCORE";
}
namespace rpl{
    const std::string YES = "Yes.";
    const std::string NO = "No.";
}

using Infomation = std::vector<std::string>;
using Message = std::string;
struct Request{
    unsigned long long id; // 请求编号
    std::string sender;    // 请求发送者链接（相对于服务端）
    Infomation content;    // 请求内容
};

/**
 * @brief 客户端用以生成请求编号。
 * @return 请求编号
 */
unsigned long long GenerateID() noexcept;

/**
 * @brief 客户端用以发送请求。
 * @param link 目标链接（文件夹）（相对于自身）
 * @param request 请求内容
 * @return 发送成功与否
 */
bool MakeRequest(const std::string &link, const Request &request) noexcept;

/**
 * @brief 服务端用以获取请求队列。
 * @param self 自身链接（文件夹）（相对于自身）
 * @return 获取成功与否以及请求队列
 * @note 获取不成功时第二项为空。
 */
std::pair<bool, std::vector<Request>> GetRequests(const std::string &self) noexcept;

/**
 * @brief 服务端用以发送回复。
 * @param link 目标链接（文件夹）（相对于自身）
 * @param reply 回复内容
 * @return 发送成功与否
 */
bool SendReply(const std::string &link, unsigned long long id, const Infomation &reply) noexcept;

/**
 * @brief 客户端用以接收回复。
 * @param self 自身链接（文件夹）（相对于自身）
 * @param id 请求编号
 * @return 是否获得回复以及回复内容
 * @note 未获取时第二项为空。
 */
std::pair<bool, Infomation> PollReply(const std::string &self, unsigned long long id) noexcept;

/**
 * @brief 将信息转化为消息。
 * @param infomation 待转换的信息
 * @return 转换后的消息
 */
Message Encode(const Infomation &infomation) noexcept;
/**
 * @brief 将消息转化为信息。
 * @param message 待转换的消息
 * @return 转换后的信息
 */
Infomation Decode(const Message &message) noexcept;

/**
 * @brief 将任意类型转为字符串。
 * @param t 待转换的类型
 * @return 转换后的字符串
 */
std::string ToStr(auto t) noexcept
{
    std::stringstream ss;
    ss << t;
    return ss.str();
}

/**
 * @brief 将字符串转为无符号超长整数。
 * @param s 待转换的字符串
 * @return 转换后的无符号超长整数
 */
unsigned long long ToUll(const std::string &s) noexcept;

}

#endif
