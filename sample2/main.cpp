#include <git2.h>

#include <iostream>

const char* path = "/Users/xiaochen/Documents/data";

int main() {
    git_libgit2_init();

    git_repository* rep = nullptr;
    git_index* index = nullptr;
    git_strarray paths = {nullptr, 0};
    int error = 0;

    // git open
    error = git_repository_open(&rep, path);
    if (error < 0)
    {
        const git_error *e = giterr_last();
        std::cout << "Error: " << error << " / " << e->klass << " : " << e->message << std::endl;

        goto SHUTDOWN;
    }


    git_repository_index(&index, rep);

    //git add file
    error = git_index_add_bypath(index, "file");
    if (error < 0)
    {
        const git_error *e = giterr_last();
        std::cout << "Error: " << error << " / " << e->klass << " : " << e->message << std::endl;
    }
    else
    {
        git_index_write(index);
    }

    git_index_free(index);

SHUTDOWN:
    git_repository_free(rep);
    git_libgit2_shutdown();
    return 0;
}