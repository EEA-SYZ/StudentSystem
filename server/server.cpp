#define SELF ".\\space"

/**
 * @brief 服务端主程序。
 * 
 * @par REQUIRES
 * NONE
 * 
 * @par PROVIDES
 * - @fn main 主程序入口
 * 
 * @author 梁祖章
 */

#include "transmitter.hpp"
#include <iostream>
#include "SFML/System.hpp"
#include "student_system.hpp"

int main() noexcept
{
    std::cout << "Server started." << std::endl;
    sf::Clock interval;
    while (true) {
        if (interval.getElapsedTime().asMilliseconds() > 1000) {
            interval.restart();
            auto ok_requests = trm::GetRequests(SELF);
            if (!ok_requests.first) {
                std::cout << "Get requests failed." << std::endl;
                sf::Clock wait;
                while (true) {
                    if (wait.getElapsedTime().asMilliseconds() > 10000) {
                        break;
                    }
                }
                continue;
            }

            for (const auto &request : ok_requests.second) {
                assert(request.content.size() > 0); // Abnormal request infomation size.
                if (request.content[0] == trm::rqs::CHECK_ONLINE) {
                    trm::SendReply(request.sender, request.id, {trm::rpl::YES});
                } else if (request.content[0] == trm::rqs::CHECK_ACCOUNT) {
                    trm::SendReply(request.sender, request.id, ssys::CheckValid(request.content));
                } else {
                    assert(false); // Unknown request.
                    std::cout << "Unknown request: " << request.content[0] << std::endl;
                }
            }
        }
    }
    return 0;
}

