#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "../include/DSMgr.h"
DSMgr::DSMgr() { ; }
int DSMgr::OpenFile(string filename) { return 0; }
int DSMgr::CloseFile() { return 0; }
int DSMgr::ReadPage(int page_id, void* frame_data) {
  totalIOs++;
  return 0;
}
int DSMgr::WritePage(int page_id, void* frame_data) {
  totalIOs++;
  return 0;
}

int DSMgr::AllocateNewPage() { return numPages++; }

void DSMgr::IncNumPages() { numPages++; }
int DSMgr::GetNumPages() { return numPages; }
void DSMgr::SetUse(int page_id, int use_bit) { pages[page_id] = use_bit; }
int DSMgr::GetUse(int page_id) { return pages[page_id]; }