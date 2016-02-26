#include <git2.h>

#include <iostream>

int main() {

    git_libgit2_init();

    const char* path = "/Users/XiaochenFTX/Documents/data";

    git_repository* rep = nullptr;

    // git init
    int error = git_repository_init(&rep, path, 0);
    if (error < 0) {
        const git_error *e = giterr_last();
        std::cout << "Error: " << error << " / " << e->klass << " : " << e->message << std::endl;

        goto SHUTDOWN;
    }

SHUTDOWN:
    git_repository_free(rep);
    git_libgit2_shutdown();

    return 0;
}