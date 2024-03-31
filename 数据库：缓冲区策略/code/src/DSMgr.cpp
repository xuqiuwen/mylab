#include "../include/DSMgr.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
DSMgr::DSMgr() { ; }
int DSMgr::OpenFile(string filename) {
  int fd = open(filename.c_str(),
                O_RDWR | O_DIRECT);  // 以直接I/O方式打开文件，绕过OS的缓冲区
  if (fd == -1) {
    perror("Error: failed to open file");
    return -1;
  }
  currFile = fd;
  return 0;
}
int DSMgr::CloseFile() {
  if (close(currFile) == -1) {
    perror("Error: failed to close file");
    return -1;
  }

  return 0;
}
int DSMgr::ReadPage(int page_id, void* frame_data) {
  // 计算偏移量
  off_t offset = page_id * PAGE_SIZE;

  // 使用pread读取数据
  ssize_t bytesRead = pread(currFile, frame_data, PAGE_SIZE, offset);
  if (bytesRead < PAGE_SIZE) {
    if (bytesRead == -1) {
      perror("Error: failed to read file");
    } else {
      printf("Warning: end of file reached or partial read.\n");
    }
    return -1;
  }
  return 0;
}
int DSMgr::WritePage(int page_id, void* frame_data) {
  // 计算偏移量
  off_t offset = page_id * PAGE_SIZE;

  // 使用pwrite写入数据
  ssize_t bytesWritten = pwrite(currFile, frame_data, PAGE_SIZE, offset);
  if (bytesWritten < PAGE_SIZE) {
    perror("Error: failed to write file");
    return -1;
  }

  // 确保文件写入磁盘
  if (fsync(currFile) == -1) {
    // 错误处理
    perror("Error: fsync failed");
  }
  return 0;
}

int DSMgr::AllocateNewPage() {
  // 扩展文件大小
  off_t newSize = (numPages + 1) * PAGE_SIZE;
  if (ftruncate(currFile, newSize) == -1) {
    // ftruncate失败
    perror("Error: failed to extending file");
    return -1;
  }

  // 增加页面计数并返回新页面的ID
  return numPages++;
}

void DSMgr::IncNumPages() { numPages++; }
int DSMgr::GetNumPages() { return numPages; }
void DSMgr::SetUse(int page_id, int use_bit) { pages[page_id] = use_bit; }
int DSMgr::GetUse(int page_id) { return pages[page_id]; }