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

#define TEST_PAGE_MAX 50000

int main() {
  int fileDescriptor;
  const char* filename = "./data.dbf";

  // 打开文件
  fileDescriptor = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fileDescriptor < 0) {
    perror("Failed to open the file");
    return -1;
  }

  // 初始化页面
  char emptyPage[PAGE_SIZE];
  memset(emptyPage, 0, PAGE_SIZE);  // 将页面初始化为全0

  for (int page_id = 0; page_id < TEST_PAGE_MAX; ++page_id) {
    // 在页面开头写入page_id
    memcpy(emptyPage, &page_id, sizeof(page_id));

    // 计算偏移量
    off_t offset = page_id * PAGE_SIZE;

    // 使用pwrite写入数据
    ssize_t bytesWritten = pwrite(fileDescriptor, emptyPage, PAGE_SIZE, offset);
    if (bytesWritten < PAGE_SIZE) {
      perror("Error: failed to write file");
      close(fileDescriptor);  // 关闭文件描述符
      return -1;
    }
  }

  // 关闭文件描述符
  close(fileDescriptor);
  return 0;
}