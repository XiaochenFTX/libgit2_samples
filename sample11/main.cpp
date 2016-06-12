//
// Created by 王晓辰 on 16/6/12.
//

#include <git2.h>

#include <iostream>

const char* path = "/Users/xiaochen/Documents/clone";

int cred_acquire_cb(git_cred **cred, const char *url, const char *username_from_url, unsigned int allowed_types, void *payload)
{

    git_cred_ssh_key_new(cred, username_from_url, nullptr, "/Users/xiaochen/.ssh/id_rsa", nullptr);

    return 0;
}

int main()
{
    git_libgit2_init();

    git_repository* rep = nullptr;

    git_remote* remote = nullptr;
    git_push_options opts = GIT_PUSH_OPTIONS_INIT;

    int error = 0;

    // git open
    git_repository_open(&rep, path);

    // take remote
    git_remote_lookup(&remote, rep, "origin");

    const char *refs[] = {"refs/heads/master:refs/heads/master"};
    git_strarray strarr = {(char**)refs, 1};

    opts.callbacks.credentials = cred_acquire_cb;
    error = git_remote_push(remote, &strarr, &opts);
    if (error < 0)
    {
        const git_error *e = giterr_last();
        std::cout << "Error: " << error << " / " << e->klass << " : " << e->message << std::endl;

        goto SHUTDOWN;
    }

SHUTDOWN:
    git_repository_free(rep);
    git_libgit2_shutdown();
}