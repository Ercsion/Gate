#ifndef __GLOGHELPER_H__
#define __GLOGHELPER_H__

#include "Clicom.h"

#include "glog/logging.h"
#include "glog/log_severity.h"
#include "glog/raw_logging.h"
#include "glog/stl_logging.h"
#include "glog/vlog_is_on.h"

#define ILOG LOG(INFO)
#define WLOG LOG(WARNING)
#define ELOG LOG(ERROR)
#define FLOG LOG(FATAL)

//���������־��Ŀ¼��
#ifdef PC
#define LOGDIR "./log"
#else
#define LOGDIR "/mnt/nand/log"
#endif
#define MKDIR "mkdir -p "LOGDIR

//����Ϣ������������ļ��� LOG(ERROR)
void SignalHandle(const char *data, int size);

class GLogHelper
{
public:
    //GLOG���ã�
    GLogHelper(char *program);
    //GLOG�ڴ�����
    ~GLogHelper();
};

#endif /* __GLOGHELPER_H__ */

