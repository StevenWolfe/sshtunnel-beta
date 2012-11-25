#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <paths.h>

static unsigned int get_userid(unsigned int uid) {
    if (uid > 99999) {
        return uid / 100000;
    }
    return 0;
}

static void set_identity(unsigned int uid)
{
    if (seteuid(0)) {
        exit(EXIT_FAILURE);
    }
    if (setresgid(uid, uid, uid)) {
        exit(EXIT_FAILURE);
    }
    if (setresuid(uid, uid, uid)) {
        exit(EXIT_FAILURE);
    }
}

int send_intent(int type, const char *status)
{
    char command[PATH_MAX];

    sprintf(command, "exec /system/bin/am broadcast --user %d -a \"org.sshtunnel.NOTIFICATION\" --ei type \"%d\" --es status \"%s\" > /dev/null", get_userid(getuid()), type, status);	
    char *args[] = { "sh", "-c", command, NULL, };

    set_identity(getuid());
    execv(_PATH_BSHELL, args);

    return 0;
}
