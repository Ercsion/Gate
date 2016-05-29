#ifndef __GLOGHELPER_H__
#define __GLOGHELPER_H__

#include "Common.h"

#define ILOG LOG(INFO)
#define WLOG LOG(WARNING)
#define ELOG LOG(ERROR)
#define FLOG LOG(FATAL)

//将信息输出到单独的文件和 LOG(ERROR)
void SignalHandle(const char *data, int size);

class GLogHelper
{
public:
    //GLOG配置：
    GLogHelper(char *program);
    //GLOG内存清理：
    ~GLogHelper();
};

#endif /* __GLOGHELPER_H__ */
