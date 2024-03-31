#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <sstream>

#include "../include/BMgr.h"
#include "../include/DSMgr.h"
#include "../include/mylimits.h"
using namespace std;

int totals = 0;  // 总次数
int hits = 0;    // 命中次数
int losthits = 0;
int totalIOs = 0;  // IO次数
int writes = 0;
int reads = 0;

int main() {
  DSMgr dsmgr = DSMgr();
  dsmgr.OpenFile("./data.dbf");  // 打开数据文件
  BMgr bmgr = BMgr(&dsmgr);      // 创建缓冲区管理器实例，注入DSMgr

  ifstream traceFile("./data-5w-50w-zipf.txt");  // 打开trace文件
  if (!traceFile.is_open()) {
    cerr << "Failed to open the trace file." << endl;
    return -1;
  }
  string line;
  while (getline(traceFile, line)) {
    int operation, page_id;
    char comma;
    istringstream iss(line);
    if (!(iss >> operation >> comma >> page_id)) {
      // 错误处理: 格式不匹配
      continue;
    }

    if (operation == 1)
      writes++;
    else
      reads++;
    // 根据操作类型调用FixPage
    int frame_id = bmgr.FixPage(page_id - START_ID, operation);
    if (frame_id == -1) {
      cerr << "Failed to fix page: " << page_id << endl;
      continue;
    }

    // 增加I/O计数，假设每次FixPage都会触发一次I/O
    totals++;

    // 一旦页面不再需要，确保调用UnfixPage
    bmgr.UnfixPage(page_id - START_ID);
  }

  traceFile.close();
  // cout << "总次数 " << totals << endl;
  // cout << "总I/O次数 " << totalIOs << endl;
  // cout << "缓存命中次数 " << hits << endl;
  // cout << "缓存未命中次数 " << losthits << endl;
  cout << "页框数 " << BUFF_SIZE << endl;
  cout << "平均每个请求的I/O数 " << (double)totalIOs / totals << endl;
  cout << "命中率 " << (double)hits / totals << endl;
  // cout << "读次数 " << reads << endl;
  // cout << "写次数 " << writes << endl;
  return 0;
}