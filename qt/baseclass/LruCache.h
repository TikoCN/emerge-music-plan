//
// Created by changzhi on 2026/3/24.
//

#ifndef MUSICPLAY_LRCCACHE_H
#define MUSICPLAY_LRCCACHE_H

#include <QString>

template<typename K, typename V>
class LruCache {
private:
    struct Entry {
        K         key;
        V         value;
        long long time;
    };

    std::list<Entry>                                           valueList;
    std::unordered_map<K, typename std::list<Entry>::iterator> keyCache;
    size_t                                                     cap;

    /**
     * @brief 循环删除
     */
    void removeBack() {
        while (valueList.size() > cap) {
            const K &key = valueList.back().key;

            keyCache.erase(key);
            valueList.pop_back();
        }
    }

public:
    explicit LruCache(const int cap)
        : cap(cap) {
    };

    /**
     * @brief 插入、更新缓存
     * @param key 保存的key
     * @param value 用于转移的实际值
     */
    void put(const K &key, V &&value) {
        auto it = keyCache.find(key);
        // 存在更新
        if (it != keyCache.end()) {
            valueList.splice(valueList.begin(), valueList, it->second);
            it->second->value = std::move(value);
        } else {
            valueList.emplace_front(Entry{key, std::move(value)});
            auto list_it           = valueList.begin();
            keyCache[list_it->key] = list_it;
        }

        removeBack();
    }

    /**
     * @brief 读取缓存
     * @param key 缓存key
     * @return 指针返回选项
     */
    V *get(const K &key) {
        auto it = keyCache.find(key);
        if (it == keyCache.end()) {
            return nullptr;
        }
        valueList.splice(valueList.begin(), valueList, it->second);

        return &it->second->value;
    }

    /**
     * @brief 设置最大容量
     * @param _cap 最大容量
     */
    void setCap(size_t _cap) {
        this->cap = _cap;

        removeBack();
    }

    /**
     * @brief 清空缓存
     */
    void clear() {
        valueList.clear();
        keyCache.clear();
    }
};

#endif //MUSICPLAY_LRCCACHE_H
