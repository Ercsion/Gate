#include <stdlib.h>
#include "GLogHelper.h"

//���������־��Ŀ¼��
#define LOGDIR "log"
#define MKDIR "mkdir -p "LOGDIR

//����Ϣ������������ļ��� LOG(ERROR)
void SignalHandle(const char *data, int size)
{
    std::string str = std::string(data, size);
    /*
    std::ofstream fs("glog_dump.log",std::ios::app);
    fs<<str;
    fs.close();
    */
    LOG(ERROR) << str;
    /*Ҳ����ֱ�������﷢���ʼ������֪ͨ��������������Ǳ��ص���εģ�ÿ�λص�ֻ���һ�д�����Ϣ������������ļ�¼���ļ���Ҳ��Ҫ>��׷��ģʽ���ɣ����������﷢�ʼ�����Ų��Ǻ��ʺϣ��������ǿ��Ե���һ�� SHELL �� PYTHON �ű������˽ű����� sleep 3�����ң�Ȼ�󽫴�
    ����Ϣͨ���ʼ�����ŷ��ͳ�ȥ�������Ͳ���Ҫ��ؽű���ʱ��Ƶ��ִ�У��˷�Ч���ˡ�*/
}

//GLOG���ã�
GLogHelper::GLogHelper(char *program)
{
    system(MKDIR);
    FLAGS_stderrthreshold = google::INFO; //���ü������ google::INFO ����־ͬʱ�������Ļ
    FLAGS_colorlogtostderr = true;  //�����������Ļ����־��ʾ��Ӧ��ɫ
    //FLAGS_logtominservity = true;  //���ý�����log�������߼�LOG�ļ���
    FLAGS_v = 3;
    //Ҳ��ʹ��ϵͳ��������GOOGLE_LOG_DIR����log��Ŀ¼
    FLAGS_log_dir = "/mnt/nand/log/";
    google::SetLogDestination(google::INFO,    LOGDIR"/INF_");  //���� google::INFO �������־�洢·�����ļ���ǰ׺
    google::SetLogDestination(google::WARNING, LOGDIR"/WAR_");  //���� google::WARNING �������־�洢·�����ļ���ǰ׺
    google::SetLogDestination(google::ERROR,   LOGDIR"/ERR_");  //���� google::ERROR �������־�洢·�����ļ���ǰ׺
    google::SetLogDestination(google::FATAL,   LOGDIR"/FAT_");  //���� google::FATAL �������־�洢·�����ļ���ǰ׺
    FLAGS_logbufsecs = 0;       //������־�����Ĭ��Ϊ30�룬�˴���Ϊ�������
    FLAGS_max_log_size = 100; //�����־��СΪ 100MB
    FLAGS_stop_logging_if_full_disk = true;     //�����̱�д��ʱ��ֹͣ��־���
	google::InitGoogleLogging(program);
    google::SetLogFilenameExtension("pc");     //�����ļ�����չ����ƽ̨����������Ҫ���ֵ���Ϣ
    google::InstallFailureSignalHandler();      //��׽ core dumped
    google::InstallFailureWriter(&SignalHandle);    //Ĭ�ϲ�׽ SIGSEGV �ź���Ϣ���������� stderr������ͨ������ķ����Զ������>��ʽ��
}
//GLOG�ڴ�����
GLogHelper::~GLogHelper()
{
    google::ShutdownGoogleLogging();
}
