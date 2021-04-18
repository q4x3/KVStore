#include "kvstore.h"
#include <string>

Skiplist::Skiplist() {
	siz = 0;
	head = new Node;
}

Skiplist::~Skiplist() {
	Node *headPtr = head;
    while(headPtr) {
        Node *ptr = headPtr;
        headPtr = headPtr->down;
        while(ptr) {
            Node *tmp = ptr;
            ptr = ptr->right;
            delete tmp;
        }
    }
}

size_t Skiplist::size() {
	return siz;
}

void Skiplist::clear() {
	Node *headPtr = head;
    while(headPtr) {
        Node *ptr = headPtr;
        headPtr = headPtr->down;
        while(ptr) {
            Node *tmp = ptr;
            ptr = ptr->right;
            delete tmp;
        }
    }
    siz = 0;
    head = new Node;
}

std::string* Skiplist::get(const uint64_t& key) {
	Node *p = head;
    while(p) {
        while(p->right && p->right->key < key) {
            p = p->right;
        }
        if(p->right && p->right->key == key) {
            p = p->right;
            std::string* res = &(p->val);
            return res;
        } else {
            p = p->down;
        }
    }
    return nullptr;
}

void Skiplist::put(const uint64_t& key, const std::string& val) {
    ++ siz;
    std::vector<Node*> pathList;    //从上至下记录搜索路径
    Node *p = head;
    while(p){
        while(p->right && p->right->key < key){ 
            p = p->right;
        }
        pathList.push_back(p);
        p = p->down;
    }
    bool insertUp = true;
    Node* downNode= nullptr;
    while(insertUp && pathList.size() > 0){   //从下至上搜索路径回溯，50%概率
        Node *insert = pathList.back();
        pathList.pop_back();
        insert->right = new Node(insert->right, downNode, key, val); //add新结点
        downNode = insert->right;    //把新结点赋值为downNode
        insertUp = (rand()&1);   //50%概率
    }
    if(insertUp){  //插入新的头结点，加层
        Node * oldHead = head;
        head = new Node;
        head->right = new Node(NULL, downNode, key, val);
        head->down = oldHead;
    }
}

bool Skiplist::remove(const uint64_t& key) {
    Node *p = head;
    Node *prev = nullptr;
    while(p) {
        while(p->right && p->right->key < key) {
            p = p->right;
        }
        if(p->right && p->right->key == key) {
            prev = p;
            break;
        } else {
            p = p->down;
        }
    }
    if(prev == nullptr) {
        return false;
    } else {
        -- siz;
        std::string valtmp = prev->val;
        while(prev) {
            Node *tmp = prev->right;
            Node *toberemove = tmp->down;
            prev->right = tmp->right;
            delete tmp;
            prev = prev->down;
            while(prev && prev->right && prev->right != toberemove) {
                prev = prev->right;
            }
        }
        return true;
    }
}

KVStore::KVStore(const std::string &dir): KVStoreAPI(dir)
{
	MemTable = new Skiplist;
}

KVStore::~KVStore()
{
	delete MemTable;
}

/**
 * Insert/Update the key-value pair.
 * No return values for simplicity.
 */
void KVStore::put(uint64_t key, const std::string &s)
{
	MemTable->put(key, s);
}
/**
 * Returns the (string) value of the given key.
 * An empty string indicates not found.
 */
std::string KVStore::get(uint64_t key)
{
	std::string *tmp = MemTable->get(key);
	if(tmp) return *tmp;
    else return "";
}
/**
 * Delete the given key-value pair if it exists.
 * Returns false iff the key is not found.
 */
bool KVStore::del(uint64_t key)
{
	return MemTable->remove(key);
}

/**
 * This resets the kvstore. All key-value pairs should be removed,
 * including memtable and all sstables files.
 */
void KVStore::reset()
{
	MemTable->clear();
}
