#ifndef __FIlE_SYSTEM_HPP__
#define __FIlE_SYSTEM_HPP__

/**
 * @brief 提供最基本的文件操作函数，不适合进行大量输入输出。
 * @namespace file
 * 
 * @par REQUIRES
 * NONE
 * 
 * @par PROVIDES
 * TO_COMPLETE
 */

#include <string>
#include <filesystem>
#include <fstream>
#include <vector>

namespace file{

namespace fs = std::filesystem;

/**
 * @brief 判断文件是否存在。
 * @param filePath 文件路径
 * @return 文件存在与否
 * @note 返回值为假时表示文件不存在或权限不足。
 */
bool CheckFileExists(const std::string &filePath) noexcept;
/**
 * @brief 读文件。
 * @param filePath 文件路径
 * @return 读取成功与否以及文件全部内容
 * @note 读取不成功时第二项为空。
 */
std::pair<bool, std::string> ReadFile(const std::string &filePath) noexcept;
/**
 * @brief 覆盖写文件。
 * @param filePath 文件路径
 * @param content 写入内容
 * @return 写入成功与否
 */
bool WriteFile(const std::string &filePath, const std::string &content) noexcept;
/**
 * @brief 追加写文件。
 * @param filePath 文件路径
 * @param content 写入内容
 * @return 写入成功与否
 */
bool AppendFile(const std::string &filePath, const std::string &content) noexcept;
/**
 * @brief 删除文件。
 * @param filePath 文件路径
 * @return 删除成功与否
 */
bool DeleteFile(const std::string &filePath) noexcept;

/**
 * @brief 判断目录是否存在。
 * @param directoryPath 目录路径
 * @return 目录存在与否
 * @note 返回值为假时表示目录不存在或权限不足。
 */
bool CheckDirectoryExists(const std::string &directoryPath) noexcept;
/**
 * @brief 获取目录下所有文件名。
 * @param directoryPath 目录路径
 * @return 获取成功与否以及目录下所有文件名
 * @note 获取不成功时第二项为空。
 */
std::pair<bool, std::vector<std::string>> ListDirectory(const std::string &directoryPath) noexcept;

/**
 * @brief 获取文件路径。
 * @param directionPath 目录路径
 * @param fileName 文件名
 * @return 文件路径
 */
std::string GetFilePath(const std::string &directionPath, const std::string &fileName) noexcept;

}

#endif
