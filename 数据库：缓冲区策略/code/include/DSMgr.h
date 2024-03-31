#ifndef _DSMGR_H
#define _DSMGR_H
#include <string>

#include "mylimits.h"
using namespace std;

extern int totalIOs;

class DSMgr {
 public:
  DSMgr();                        // 构造函数
  int OpenFile(string filename);  // 打开文件
  int CloseFile();                // 关闭文件
  int ReadPage(int page_id,
               void* frame_data);  // 从数据库文件中读取指定ID的页面
  int WritePage(int page_id,
                void* frame_data);  // 将指定的页面内容写入数据库文件

  int AllocateNewPage();  // 分配一个新页面

  void IncNumPages();                     // 页计数器加一
  int GetNumPages();                      // 获取页面计数器的值
  void SetUse(int page_id, int use_bit);  // 设置use_bit
  int GetUse(int page_id);                // 获取use_bit

 private:
  int currFile;         // 当前打开的数据库文件的文件描述符
  int numPages;         // 页的数量
  int pages[MAX_PAGE];  // use_bit数组，用来追踪页面的使用情况。
};

#endif