#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <paths.h>

#include <android/log.h>

#ifndef LOG_TAG
#define LOG_TAG "SSHTunnel"
#endif
#ifndef LOGD
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#endif

static unsigned int get_userid(unsigned int uid) {
    if (uid > 99999) {
        return uid / 100000;
    }
    return 0;
}

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
