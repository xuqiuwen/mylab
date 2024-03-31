#include "../include/BMgr.h"

#include <cstring>
#include <iomanip>
#include <iostream>
BMgr::BMgr(DSMgr* ptr) {
  dsmgr_ptr = ptr;  // 实例化存储管理器
  // 初始化所有FCB和页框
  fcbs = (FCB*)calloc(BUFF_SIZE, sizeof(FCB));
  frames = (Frame*)calloc(BUFF_SIZE, sizeof(Frame));
  for (int i = 0; i < BUFF_SIZE; i++) {
    fcbs[i].not_idle = false;
    fcbs[i].page_id = -1;
    fcbs[i].frame_id = i;
    fcbs[i].count = 0;
    fcbs[i].dirty = false;
    idleList.push_back(i);               // 所有页框初始状态为idle
    fcbs[i].list_it = --idleList.end();  // 设置迭代器指向idleList中的相应位置
  }
}

int BMgr::FixPage(int page_id, int prot) {
  int frame_id;
  auto it = pagetoframe.find(page_id);
  if (it != pagetoframe.end()) {  // 页面已在缓冲区，命中
    // cout << page_id << "命中" << endl;
    hits++;
    frame_id = it->second;
    // 将页框移动到LRU链表头部
    fcbs[frame_id].count++;
    LRUList.erase(fcbs[frame_id].list_it);
    LRUList.push_front(frame_id);
    fcbs[frame_id].list_it = LRUList.begin();
  } else {  // 页面不在缓冲区
    losthits++;
    // cout << page_id << "未命中" << endl;
    frame_id = SelectVictim();
    if (frame_id == -1) {
      return -1;  // 无法选择牺牲页框
    }
    // 从存储管理器中读取页面
    dsmgr_ptr->ReadPage(page_id, frames[frame_id].data);
    // 更新FCB和pagetoframe映射
    fcbs[frame_id].page_id = page_id;
    fcbs[frame_id].not_idle = true;
    fcbs[frame_id].count++;
    fcbs[frame_id].dirty = (prot == 1);
    pagetoframe[page_id] = frame_id;
    // 更新LRU链表
    LRUList.push_front(frame_id);
    fcbs[frame_id].list_it = LRUList.begin();
    // 如果frame_id原本在idleList中，需要从idleList中移除
    if (!fcbs[frame_id].not_idle) {
      idleList.erase(fcbs[frame_id].list_it);
    }
  }
  fcbs[frame_id].not_idle = true;  // 标记页框正在使用
  return frame_id;
}

int BMgr::UnfixPage(int page_id) {
  int frame_id = pagetoframe[page_id];
  if (frame_id == -1) {
    perror("Error: frame not fount");
    return -1;  // 页面不在缓冲区
  }
  fcbs[frame_id].count--;
  // if (fcbs[frame_id].count == 0) {
  //   // 页面不再被任何进程使用
  //   if (!fcbs[frame_id].dirty) {
  //     LRUList.erase(fcbs[frame_id].list_it);
  //     // 如果页面未被修改（不是脏页），移动到idleList
  //     pagetoframe.erase(fcbs[frame_id].page_id);
  //     idleList.push_front(frame_id);
  //     fcbs[frame_id].list_it = idleList.begin();
  //     fcbs[frame_id].not_idle = false;
  //   }
  // }
  return 0;
}
void BMgr::PrintFrame(int frame_id) {
  if (frame_id < 0 || frame_id >= BUFF_SIZE) {
    cerr << "Error: frame_id out of bounds." << endl;
    return;
  }

  FCB& fcb = fcbs[frame_id];
  if (!fcb.not_idle) {
    cout << "Frame " << frame_id << " is not in use." << endl;
    return;
  }

  cout << "  Frame ID: " << frame_id << ":" << endl;
  cout << "  Page ID: " << fcb.page_id << endl;
  cout << "  Frame ID: " << fcb.frame_id << endl;
  cout << "  Fixed Count: " << fcb.count << endl;
  cout << "  Dirty: " << (fcb.dirty ? "Yes" : "No") << endl;
  cout << "  Data: ";

  // 打印页框的数据
  for (int i = 0; i < PAGE_SIZE; i++) {
    cout << setw(2) << setfill('0') << hex
         << (unsigned int)(unsigned char)frames[frame_id].data[i];
    if ((i + 1) % 16 == 0) {  // 每16个字节换一行，便于查看
      cout << endl;
      if (i + 1 < PAGE_SIZE) {
        cout << "        ";  // 对齐
      }
    } else {
      cout << " ";  // 字节之间的空格
    }
  }
  cout << dec << endl;  // 恢复到十进制输出
}

int BMgr::SelectVictim() {
  if (!idleList.empty()) {
    int frame_id = idleList.front();
    idleList.pop_front();
    return frame_id;
  } else if (!LRUList.empty()) {
    int frame_id = LRUList.back();
    LRUList.pop_back();
    // 如果选择的页框是脏的，需要写回磁盘
    if (fcbs[frame_id].dirty) {
      dsmgr_ptr->WritePage(fcbs[frame_id].page_id, frames[frame_id].data);
      fcbs[frame_id].dirty = false;
    }
    // 从pagetoframe映射中移除
    pagetoframe.erase(fcbs[frame_id].page_id);
    fcbs[frame_id].not_idle = false;  // 标记页框不在使用
    return frame_id;
  }
  return -1;  // 无法找到牺牲页框
}

int BMgr::NumFreeFrames() { return idleList.size(); }

void BMgr::FixNewPage() {
  int frame_id;

  if (!idleList.empty()) {
    // 如果有空闲页框，从空闲链表中获取一个页框
    frame_id = idleList.front();
    idleList.pop_front();
  } else {
    // 如果没有空闲页框，选择一个牺牲页框进行替换
    frame_id = SelectVictim();
    if (frame_id == -1) {
      std::cerr << "Error: No frame available for replacement." << std::endl;
      return;
    }
  }

  // 初始化新页面的数据
  memset(frames[frame_id].data, 0, PAGE_SIZE);

  // 分配新的页面ID
  int page_id = dsmgr_ptr->AllocateNewPage();

  // 更新页框的FCB信息
  fcbs[frame_id].not_idle = true;
  fcbs[frame_id].page_id = page_id;
  fcbs[frame_id].frame_id = frame_id;
  fcbs[frame_id].count = 1;                  // 新页面被当前操作固定
  fcbs[frame_id].dirty = true;               // 新页面默认为脏页
  fcbs[frame_id].list_it = LRUList.begin();  // 更新迭代器指向LRU链表的位置

  // 更新页到页框的映射
  pagetoframe[page_id] = frame_id;

  // 将页框放到LRU链表的前端
  LRUList.push_front(frame_id);
}
