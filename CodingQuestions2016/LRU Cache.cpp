//
//  LRU.cpp
//  CodingQuestions2016
//
//  Created by Fang Liu on 2/4/17.
//  Copyright © 2017 Fang Liu. All rights reserved.
//

/*
 Design and implement a data structure for Least Recently Used (LRU) cache. It should support the following operations: get and put.

 get(key) - Get the value (will always be positive) of the key if the key exists in the cache, otherwise return -1.
 put(key, value) - Set or insert the value if the key is not already present. When the cache reached its capacity, it should invalidate the least recently used item before inserting a new item.

 Follow up:
 Could you do both operations in O(1) time complexity?

 Example:

 LRUCache cache = new LRUCache(2) // capacity is 2;

cache.put(1, 1);
cache.put(2, 2);
cache.get(1);       // returns 1
cache.put(3, 3);    // evicts key 2
cache.get(2);       // returns -1 (not found)
cache.put(4, 4);    // evicts key 1
cache.get(1);       // returns -1 (not found)
cache.get(3);       // returns 3
cache.get(4);       // returns 4
 */

#include <unordered_map>
#include <iostream>
using namespace std;

class LRUCache {
private:
    // 双向链表的节点结构
    struct CacheNode{
        int key;
        int value;
        CacheNode* pre;
        CacheNode* next;
        CacheNode():key(0), value(0), pre(NULL), next(NULL){}
        CacheNode(int k, int v):key(k), value(v), pre(NULL), next(NULL){}
    };

    unordered_map<int, CacheNode*> mp;

    CacheNode* head; // 指向双链表的头结点
    CacheNode* tail; // 指向双链表的尾节点
    int capacity; // cache的容量
    int count; // 计数

public:
    LRUCache(int capacity);
    ~LRUCache();
    int get(int key); // 查询数据
    void put(int key, int value); // 未满时插入，已满时替换

private:
    void removeLRUNode(); // 删除尾节点（最久未使用）
    void detachNode(CacheNode* node); // 分离当前节点
    void insertToFront(CacheNode* node); // 节点插入到头部
};

LRUCache::LRUCache(int capacity){
    this->capacity = capacity;
    this->count = 0;
    head = new CacheNode();
    tail = new CacheNode();
    head->next = tail;
    head->pre = NULL;
    tail->next = NULL;
    tail->pre = head;
}

LRUCache::~LRUCache(){
    delete head;
    delete tail;
}

int LRUCache::get(int key){
    if(mp.find(key) == mp.end()){
        return -1;
    }else{
        CacheNode* curNode = mp[key];
        detachNode(curNode);
        insertToFront(curNode); // 命中，移至头部
        return curNode->value;
    }
}

void LRUCache::put(int key, int value){
    if(mp.find(key) == mp.end()){
        CacheNode* newNode = new CacheNode(key,value);
        if(count == capacity){
            removeLRUNode();
            // count--;
        }
        mp[key] = newNode; // 插入hashtable
        insertToFront(newNode); // 插入链表头部
        count++;
    }else{
        CacheNode* targetNode = mp[key];
        targetNode->value = value;
        detachNode(targetNode);
        insertToFront(targetNode);
    }
}

void LRUCache::removeLRUNode(){
    CacheNode* node = tail->pre;
    detachNode(node);
    mp.erase(node->key); // 记得要同时将此结点从map内删除
    count--;
}

void LRUCache::detachNode(CacheNode* node){
    node->pre->next = node->next;
    node->next->pre = node->pre;
}

void LRUCache::insertToFront(CacheNode* node){
    node->next = head->next;
    node->pre = head;
    head->next = node;
    node->next->pre = node;
}


/**
 * Your LRUCache object will be instantiated and called as such:
 * LRUCache obj = new LRUCache(capacity);
 * int param_1 = obj.get(key);
 * obj.put(key,value);
 */
