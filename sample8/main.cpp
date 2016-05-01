//
// Created by 王晓辰 on 16/4/29.
//

#include <git2.h>

#include <iostream>

const char* path = "/Users/xiaochen/Documents/data";

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
    git_strarray remote_list;

    git_remote_callbacks remote_conn_opt = GIT_REMOTE_CALLBACKS_INIT;

    int error = 0;

    // git open
    error = git_repository_open(&rep, path);
    if (error < 0)
    {
        const git_error *e = giterr_last();
        std::cout << "Error: " << error << " / " << e->klass << " : " << e->message << std::endl;

        goto SHUTDOWN;
    }

    // take a remote
//    git_remote_lookup(&remote, rep, "origin2");

    // git remote add <...>
    error = git_remote_create(&remote, rep, "origin2", "git@github.com:XiaochenFTX/libgit2_samples.git");

    // no need
//    git_remote_add_fetch(rep, "origin", "+refs/heads/*:refs/remotes/origin/*");


    // git remote
    git_remote_list(&remote_list, rep);
    std::cout<< "list count: " << remote_list.count << std::endl;
    for(int i = 0; i < remote_list.count; ++i)
    {
        std::cout<< remote_list.strings[i] <<std::endl;
    }

    // connect
    remote_conn_opt.credentials = cred_acquire_cb;
    error = git_remote_connect(remote, GIT_DIRECTION_FETCH, &remote_conn_opt, nullptr);
    if (error < 0)
    {
        const git_error *e = giterr_last();
        std::cout << "Error: " << error << " / " << e->klass << " : " << e->message << std::endl;
    }
    else
    {
        git_remote_disconnect(remote);
    }

    // git remote remove
    error = git_remote_delete(rep, "origin2");

SHUTDOWN:
    git_repository_free(rep);
    git_libgit2_shutdown();
    return 0;
}