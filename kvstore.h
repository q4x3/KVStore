#pragma once

#include "kvstore_api.h"
#include <vector>

struct Node{
    Node *right,*down;   //向右向下足矣
    uint64_t key;
    std::string val;
    Node(Node *right,Node *down, uint64_t key, std::string val): right(right), down(down), key(key), val(val){}
    Node(): right(nullptr), down(nullptr) {}
};

class Skiplist {
public:
	Node *head;
	size_t siz;
	Skiplist();
	~Skiplist();
	size_t size();
    void clear();
    std::string* get(const uint64_t& key);
    void put(const uint64_t& key, const std::string& val);
    bool remove(const uint64_t& key);
};

class KVStore : public KVStoreAPI {
	// You can add your implementation here
private:
	Skiplist *MemTable;
public:
	KVStore(const std::string &dir);

	~KVStore();

	void put(uint64_t key, const std::string &s) override;

	std::string get(uint64_t key) override;

	bool del(uint64_t key) override;

	void reset() override;

};
