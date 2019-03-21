#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <ftw.h>
#include <string.h>
#include <stdint.h>


#define EVENT_SIZE  (sizeof(struct inotify_event))
#define BUF_LEN     (1024 * (EVENT_SIZE + 16))
#define _XOPEN_SOURCE 500
char *directory;
int wd;
int fd;
static void getTree(char dir[]);
static void displayInotifyEvent(struct inotify_event *i);


static void displayInotifyEvent(struct inotify_event *i){
    if (i->len > 0) infof("        directory %s", i->name);
    if (i->mask & IN_CREATE){
	    infof(" was created ");
	    if (i->mask == 1073742080) getTree(directory);
    }
    if (i->mask & IN_DELETE){
	    infof(" was deleted");
	    if (i->mask == 1073742336) getTree(directory);
    }
    if (i->mask & IN_MOVED_FROM){
	    infof(" was renamed ");
	    if (i->mask == 1073741888) getTree(directory);
    }
    printf("\n");
}



static int addWatchers(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf){
    if((intmax_t) sb->st_size == 4096){
        wd = inotify_add_watch(fd, fpath, IN_MODIFY | IN_CREATE | IN_DELETE | IN_MOVED_FROM);
    }
    return 0;           /* To tell nftw() to continue */
}

static void getTree(char dir[]){
    int flags = 0;
   if (nftw(dir, addWatchers, 20, flags)
            == -1) {
        errorf("nftw");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char **argv) {
    int length, i;
    char buffer[BUF_LEN];
    directory =argv[1];
    if(argc < 2){
        errorf("Incorrect usage, do: ./main [directory]\n");
        return 0;
    }

    initLogger("stdout");   

    fd = inotify_init();

    if (fd < 0) {
        errorf("inotify_init");
    }

    getTree(directory);

    for (;;) {
        length = read(fd, buffer, BUF_LEN);

        if (length < 0) {
            perror("read");
        }

	for (i = 0; i < length;) {
            struct inotify_event *event =
                (struct inotify_event *) &buffer;
            displayInotifyEvent(event);
            i += EVENT_SIZE + event->len;
        }
    }

    (void) inotify_rm_watch(fd, wd);
    (void) close(fd);

    return 0;
}
