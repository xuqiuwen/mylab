#ifndef _BMGR_H
#define _BMGR_H
#include <list>
#include <unordered_map>

#include "DSMgr.h"
#include "mylimits.h"

extern int hits;
extern int losthits;

using namespace std;
struct FCB {
  bool not_idle;  // 是否不在空闲链表
  int page_id;
  int frame_id;
  int count;  // 计数，表示页面被多少进程占用
  int dirty;
  list<int>::iterator list_it;  // FCB链表(空闲或者LRU)的指针
};

struct Frame {
  char data[PAGE_SIZE];
};

class BMgr {
 public:
  BMgr(DSMgr* ptr);
  // Interface functions
  int FixPage(int page_id, int prot);  // 0-读 1-写
  void FixNewPage();
  int UnfixPage(int page_id);
  int NumFreeFrames();

 private:
  // Internal Functions
  int SelectVictim();
  void PrintFrame(int frame_id);

  list<int> idleList;                   // 空闲链表
  list<int> LRUList;                    // LRU链表
  FCB* fcbs;                            // 所有FCB
  Frame* frames;                        // 所有页框
  unordered_map<int, int> pagetoframe;  // 通过页快速查找页框

  DSMgr* dsmgr_ptr;  // 存储管理器的实例
};
#endif