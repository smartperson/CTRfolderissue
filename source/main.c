#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <errno.h>
#include <stdarg.h>
#include <unistd.h>

#include <fcntl.h>

#include <sys/stat.h>
#include <sys/types.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <3ds.h>

#include "remote_files.h"

#define APP_TITLE "CTRsync 0.0.1 Pre"
#define INSTRUCTIONS_MENU      "      L+R Switch direction   A+B Begin xfer      "
#define INSTRUCTIONS_OPERATING "              B to cancel gracefully             "

#define STACKSIZE (100 * 1024)

#define localSharedPath "/JKSV2"

int rc;

int runUserMenu();

void threadMain(void *arg)
{
    printf("Session initialized.");

    struct stat dir_stat = {0};
    if (stat(localSharedPath, &dir_stat) == -1) {
        // printf("Creating local directory.");
        mkdir(localSharedPath, 0755);
    }
    push_local_directory("");
    printf("One print cycle complete.\n");
    push_local_directory("");
    printf("Print cycle complete.\nPress A to hang...?\n");
    threadExit(0);
}

//---------------------------------------------------------------------------------
int main(int argc, char **argv) {
//--------------------------------------------------------------------------------
    gfxInitDefault();

    // register gfxExit to be run when app quits
    // this can help simplify error handling
    atexit(gfxExit);

    consoleInit(GFX_BOTTOM, NULL);
    
    Thread threads[1];
    s32 prio = 0;
    svcGetThreadPriority(&prio, CUR_THREAD_HANDLE);
    while (runUserMenu() >= 0){
        threads[0] = threadCreate(threadMain, NULL, STACKSIZE, prio-1, -2, true);
        threadJoin(threads[0], U64_MAX);
        threadFree(threads[0]);
    }
    gfxExit();
    return 0;
}

// returns selected option, or -1 is user has chosen to exit
int runUserMenu() {
    while (aptMainLoop())
    {
        //Scan all the inputs. This should be done once for each frame
        hidScanInput();
        //hidKeysDown returns information about which buttons have been just pressed (and they weren't in the previous frame)
        u32 kDown = hidKeysDown();
        //hidKeysHeld returns information about which buttons have are held down in this frame
        u32 kHeld = hidKeysHeld();
        //hidKeysUp returns information about which buttons have been just released
        if (kDown & KEY_START)
            return -1;
        // break in order to return to hbmenu
    
        if (kHeld == (KEY_A))
            return 0;

        // Flush and swap framebuffers
        gfxFlushBuffers();
        gfxSwapBuffers();

        //Wait for VBlank
        gspWaitForVBlank();
    }
    return -1;
}
