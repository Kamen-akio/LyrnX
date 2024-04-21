#pragma once
#include <thread>
#include <mutex>
#include <condition_variable>

using std::thread;
using std::mutex;
using std::condition_variable;

#ifndef _STACKBLUR_H_

namespace Core {
void StackBlur(unsigned char* ptrBitmap, unsigned int width,
               unsigned int height, unsigned int radius, int threadCount);
}

#endif  // !_STACKBLUR_H_
