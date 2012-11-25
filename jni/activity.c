#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <paths.h>

#include <android/log.h>
#ifndef LOGE
#define LOGE(...) ALOGE(__VA_ARGS__)
#endif
#ifndef LOGD
#define LOGD(...) ALOGD(__VA_ARGS__)
#endif
#ifndef LOGW
#define LOGW(...) ALOGW(__VA_ARGS__)
#endif

int send_intent(int type, const char *status)
{
    char command[PATH_MAX];

    sprintf(command, "exec /system/bin/am broadcast --user %d -a \"org.sshtunnel.NOTIFICATION\" --ei type \"%d\" --es status \"%s\" > /dev/null", get_userid(getuid()), type, status);	
    char *args[] = { "sh", "-c", command, NULL, };

    setenv("LD_LIBRARY_PATH", "/vendor/lib:/system/lib", 0);
    setgroups(0, NULL);
    setegid(getgid());
    seteuid(getuid());
    LOGD("gid: %u, uid: %u, userid: %u", getgid(), getuid(), get_userid(getuid()));
    execv(_PATH_BSHELL, args);

    return 0;
}
