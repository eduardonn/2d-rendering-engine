#pragma once

#if defined(DEBUG_MODE) || (RELEASE_MODE)
#include <iostream>

#define PRINT_FILE_LINE() std::cout << "LOG: [" << __FILE__ << ":" << __LINE__ << "] "
#define RAW_LOG(x) std::cout << x
#define RAW_LOG_LN(x) std::cout << x << std::endl
#define LOG(x) PRINT_FILE_LINE(); RAW_LOG(x)
#define LOG_LN(x) PRINT_FILE_LINE(); RAW_LOG_LN(x)
#define ASSERT(x, errorMsg) if (!x) LOG_LN(errorMsg)
#else
#define PRINT_FILE_LINE()
#define RAW_LOG(x)
#define RAW_LOG_LN(x)
#define LOG(x)
#define LOG_LN(x)
#define ASSERT(x, errorMsg)
#endif