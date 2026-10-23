#ifndef NAMEKEY_H
#define NAMEKEY_H
#include <cpp-pinyin/Pinyin.h>

/**
 * @brief 将名称转换为拼音首字母分类。
 */
class NameKey {
public:
    /**
     * @brief 创建名称分类器。
     */
    NameKey();

    /**
     * @brief 获取名称对应的拼音首字母分类。
     * @param name 待分类的名称。
     * @return 大写英文字母 A 到 Z；无法分类时返回“未知”。
     */
    [[nodiscard]] QString find(const QString &name);

};

#endif // NAMEKEY_H
