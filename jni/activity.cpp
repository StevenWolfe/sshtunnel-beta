#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

extern "C" {
#include "notify.h"
}

int send_intent(int type, const char *status)
{
    char command[PATH_MAX];

    sprintf(command, "/system/bin/am broadcast -a 'org.sshtunnel.NOTIFICATION' --ei type '%d' --ei status '%s' > /dev/null", type, status);	

    // before sending the intent, make sure the (uid and euid) and (gid and egid) match,
    // otherwise LD_LIBRARY_PATH is wiped in Android 4.0+.
    // Also, sanitize all secure environment variables (from linker_environ.c in linker).

    /* The same list than GLibc at this point */
    static const char* const unsec_vars[] = {
        "GCONV_PATH",
        "GETCONF_DIR",
        "HOSTALIASES",
        "LD_AUDIT",
        "LD_DEBUG",
        "LD_DEBUG_OUTPUT",
        "LD_DYNAMIC_WEAK",
        "LD_LIBRARY_PATH",
        "LD_ORIGIN_PATH",
        "LD_PRELOAD",
        "LD_PROFILE",
        "LD_SHOW_AUXV",
        "LD_USE_LOAD_BIAS",
        "LOCALDOMAIN",
        "LOCPATH",
        "MALLOC_TRACE",
        "MALLOC_CHECK_",
        "NIS_PATH",
        "NLSPATH",
        "RESOLV_HOST_CONF",
        "RES_OPTIONS",
        "TMPDIR",
        "TZDIR",
        "LD_AOUT_LIBRARY_PATH",
        "LD_AOUT_PRELOAD",
        // not listed in linker, used due to system() call
        "IFS",
    };
    const char* const* cp   = unsec_vars;
    const char* const* endp = cp + sizeof(unsec_vars)/sizeof(unsec_vars[0]);
    while (cp < endp) {
        unsetenv(*cp);
        cp++;
    }

    // sane value so "am" works
    setenv("LD_LIBRARY_PATH", "/vendor/lib:/system/lib", 1);
    setegid(getgid());
    seteuid(getuid());
    return system(command);
}
