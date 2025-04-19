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

#include <cassert>
#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

namespace file{

namespace fs = std::filesystem;

/**
 * @brief 由文件系统抽象出来的数据库类。
 */
class DataBase;

/**
 * @brief 读结构体。
 * @param filePath 文件路径
 * @param structure 结构体
 * @return 读成功与否
 */
bool ReadStructure(const std::string &filePath, auto &structure) noexcept
{
    std::ifstream file(filePath, std::ios::in | std::ios::binary);
    if (!file.good()) return false;
    file.read((char *)&structure, sizeof(structure));
    return true;
}
/**
 * @brief 写结构体。
 * @param filePath 文件路径
 * @param structure 结构体
 * @return 写成功与否
 */
bool WriteStructure(const std::string &filePath, auto &structure) noexcept
{
    std::ofstream file(filePath, std::ios::out | std::ios::binary);
    if (!file.good()) return false;
    file.write((char *)&structure, sizeof(structure));
    return true;
}
/**
 * @brief 追加结构体。
 * @param filePath 文件路径
 * @param structure 结构体
 * @return 追加成功与否
 */
bool AppendStructure(const std::string &filePath, auto &structure) noexcept
{
    std::ofstream file(filePath, std::ios::app | std::ios::binary);
    if (!file.good()) return false;
    file.write((char *)&structure, sizeof(structure));
    return true;
}

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
 * @brief 创建目录。
 * @param directoryPath 目录路径
 * @return 创建成功与否
 */
bool CreateDirectory(const std::string &directoryPath) noexcept;

/**
 * @brief 删除目录。
 * @param directoryPath 目录路径
 * @return 删除成功与否
 */
bool DeleteDirectory(const std::string &directoryPath) noexcept;

/**
 * @brief 获取文件路径。
 * @param directionPath 目录路径
 * @param fileName 文件名
 * @return 文件路径
 */
std::string GetFilePath(const std::string &directionPath, const std::string &fileName) noexcept;

class DataBase {
public:
    /**
     * @brief 构造函数，设置工作目录。
     * @param workSpace 工作目录
     */
    DataBase(const std::string &workSpace) noexcept : space(workSpace)
    {
        if (!CheckDirectoryExists(workSpace)) {
            space = ".\\";
        }
    }
    /**
     * @brief 当成目录进行索引。
     * @param keyName 键名
     * @return 下一层目录的对象
     */
    DataBase operator[](const std::string &keyName) noexcept;
    /**
     * @brief 当成目录进行计数。
     */
    unsigned long long Count() const noexcept;
    /**
     * @brief 当成文件进行增改。
     * @param value 值
     * @return 原值
     */
    const std::string &operator=(const std::string &value) const noexcept;
    /**
     * @brief 当成文件进行读取。
     * @return 值
     */
    operator std::string() const noexcept;
    /**
     * @brief 当成文件进行删除。
     * @note 将析构。
     */
    void Remove() noexcept;
    /**
     * @brief 当成目录进行删除。
     * @note 将析构。
     */
    void Cut() noexcept;
private:
    std::string space;
};

}

#endif
