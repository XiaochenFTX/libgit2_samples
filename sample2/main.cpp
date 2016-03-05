#include <git2.h>

#include <iostream>

const char* path = "/Users/xiaochen/Documents/data";

int index_matched_path_cb(const char *path, const char *matched_pathspec, void *payload)
{
    if (path != nullptr)
    {
        std::cout << "path: " << path << "\n";
    }

    if (matched_pathspec != nullptr)
    {
        std::cout << "matched_pathspec: " << matched_pathspec << std::endl;
    }

    /*
    * return 0 to add/remove this path,
    * a positive number to skip this path,
    * or a negative number to abort the operation.
    */
    return 0;
}

int main() {
    git_libgit2_init();

    git_repository* rep = nullptr;
    git_index* index = nullptr;
    char *strs[1];
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

    // get index
    git_repository_index(&index, rep);

    // git add file
    error = git_index_add_bypath(index, "file");
    if (error < 0)
    {
        const git_error *e = giterr_last();
        std::cout << "Error: " << error << " / " << e->klass << " : " << e->message << std::endl;
    }
    else
    {
        /* Write the in-memory index to disk */
        git_index_write(index);
    }

    //
    strs[0] = "dir/*";
    paths.strings = strs;
    paths.count   = 1;
//    error = git_index_add_all(index, nullptr, GIT_INDEX_ADD_DEFAULT, nullptr, nullptr);
    error = git_index_add_all(index, &paths, GIT_INDEX_ADD_DEFAULT, index_matched_path_cb, nullptr);
    if (error < 0)
    {
        const git_error *e = giterr_last();
        std::cout << "Error: " << error << " / " << e->klass << " : " << e->message << std::endl;
    }
    else
    {
        /* Write the in-memory index to disk */
        git_index_write(index);
    }

    //
    git_index_free(index);

SHUTDOWN:
    git_repository_free(rep);
    git_libgit2_shutdown();
    return 0;
}