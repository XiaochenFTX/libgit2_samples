//
// Created by 王晓辰 on 16/3/25.
//

#include <git2.h>

#include <iostream>

const char* path = "/Users/xiaochen/Documents/data";

int status_cb(const char *path,
              unsigned int status_flags,
              void *payload)
{
    std::cout << "path: " << path;
    std::cout << "\nflag: " << status_flags << std::endl;

    return 0;
}

int main()
{
    git_libgit2_init();

    git_repository* rep = nullptr;
    int error = 0;
    git_status_options opt = GIT_STATUS_OPTIONS_INIT;
    git_status_list* statuses = nullptr;
    size_t count = 0;

    // git open
    error = git_repository_open(&rep, path);
    if (error < 0)
    {
        const git_error *e = giterr_last();
        std::cout << "Error: " << error << " / " << e->klass << " : " << e->message << std::endl;

        goto SHUTDOWN;
    }

    // detailed
    opt.flags = GIT_STATUS_OPT_DEFAULTS;
    error = git_status_list_new(&statuses, rep, &opt);
    if (error < 0)
    {
        const git_error *e = giterr_last();
        std::cout << "Error: " << error << " / " << e->klass << " : " << e->message << std::endl;

        goto SHUTDOWN;
    }

    count = git_status_list_entrycount(statuses);
    std::cout<< "count: " << count << std::endl;
    for (size_t i = 0; i < count; ++i)
    {
        const git_status_entry* entry = git_status_byindex(statuses, i);
        std::cout<<"status: " << entry->index_to_workdir->status << "\tpath: " << entry->index_to_workdir->new_file.path << std::endl;
    }

    git_status_list_free(statuses);

    // simple
    error = git_status_foreach(rep, status_cb, nullptr);
    if (error < 0)
    {
        const git_error *e = giterr_last();
        std::cout << "Error: " << error << " / " << e->klass << " : " << e->message << std::endl;
    }

SHUTDOWN:
    git_repository_free(rep);
    git_libgit2_shutdown();
    return 0;
}