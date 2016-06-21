//
// Created by 王晓辰 on 16/6/18.
//

#include <git2.h>

#include <iostream>

const char* path = "/Users/xiaochen/Documents/data";

int tag_foreach_cb(const char *name, git_oid *oid, void *payload)
{
    git_repository* rep = (git_repository*) payload;
    git_tag* tag = nullptr;
    int error = 0;
    std::cout<< "tag name: " << name << "\n";

    error = git_tag_lookup(&tag, rep, oid);
    if (error < 0)
    {
        const git_error *e = giterr_last();
        std::cout << "Error: " << error << " / " << e->klass << " : " << e->message << std::endl;
        return 0;
    }
    std::cout<< "tag message: " << git_tag_message(tag) << "\n";

    git_tag_free(tag);

    return 0;
}

int cred_acquire_cb(git_cred **cred, const char *url, const char *username_from_url, unsigned int allowed_types, void *payload)
{

    git_cred_ssh_key_new(cred, username_from_url, nullptr, "/Users/xiaochen/.ssh/id_rsa", nullptr);

    return 0;
}

int main()
{
    git_libgit2_init();

    git_repository* rep = nullptr;
    git_strarray tags = {0};
    git_reference* head = nullptr;
    git_annotated_commit* commit = nullptr;
    git_object* target = nullptr;
    git_oid tag_oid;
    git_signature* me = nullptr;
    git_checkout_options opts = GIT_CHECKOUT_OPTIONS_INIT;
    git_remote* remote = nullptr;

    int error = 0;

    // git open
    git_repository_open(&rep, path);

    // git tag
    git_tag_list(&tags, rep);
    std::cout<< "tag list: \n";
    for(int i = 0; i < tags.count; ++i)
    {
        std::cout<< "tag: " << tags.strings[i] <<"\n";
    }
    std::cout<< "================ \n";

    // foreach
    std::cout<< "tag foreach: \n";
    git_tag_foreach(rep, tag_foreach_cb, rep);
    std::cout<< "================ \n";

    // git tag -l "v0.*"
    std::cout<< "tag search: \n";
    tags = {0};
    git_tag_list_match(&tags, "v0.*", rep);
    for(int i = 0; i < tags.count; ++i)
    {
        std::cout<< "tag: " << tags.strings[i] <<"\n";
    }
    std::cout<< "================ \n";

    // git tag -d
    std::cout<< "tag delete: \n";
    error = git_tag_delete(rep, "v1");
    if (error < 0)
    {
        // error == -3(GIT_ENOTFOUND) : tag not found
        const git_error *e = giterr_last();
        std::cout << "Error: " << error << " / " << e->klass << " : " << e->message << std::endl;
    }
    std::cout<< "================ \n";

    // git tag [tag name]
    std::cout<< "tag add lightweight : \n";
    git_repository_head(&head, rep);
    git_annotated_commit_from_ref(&commit, rep, head);
    git_object_lookup(&target, rep, git_annotated_commit_id(commit), GIT_OBJ_COMMIT);

    error = git_tag_create_lightweight(&tag_oid, rep, "v1.2.3", target, false);
    if (error == GIT_EEXISTS)
    {
        std::cout<< "tag 123 already exists \n";
    }
    std::cout<< "================ \n";

    // git tag -a
    std::cout<< "tag add message: \n";
    git_repository_head(&head, rep);
    git_annotated_commit_from_ref(&commit, rep, head);
    git_object_lookup(&target, rep, git_annotated_commit_id(commit), GIT_OBJ_COMMIT);
    git_signature_now(&me, "XiaochenFTX", "xiaochenftx@gmail.com");
    error = git_tag_create(&tag_oid, rep, "v3.2.1", target, me, "message", false);
    if (error == GIT_EEXISTS)
    {
        std::cout<< "tag 321 already exists \n";
    }
    std::cout<< "================ \n";

    // git checkout [tag name]
    std::cout<< "checkout tag: \n";
    error = git_repository_set_head(rep, "refs/tags/v3.2.1");
    if (error < 0)
    {
        const git_error *e = giterr_last();
        std::cout << "Error: " << error << " / " << e->klass << " : " << e->message << std::endl;
    }
    std::cout<< "================ \n" <<std::endl;

    // git push --tags
    git_push_options push_opts = GIT_PUSH_OPTIONS_INIT;
    push_opts.callbacks.credentials = cred_acquire_cb;
    const char *refs[] = {"refs/tags/*:refs/tags/*"};
    git_strarray strarr = {(char**)refs, 1};
    git_remote_lookup(&remote, rep, "origin");
    git_remote_push(remote, &strarr, &push_opts);

SHUTDOWN:
    git_repository_free(rep);
    git_libgit2_shutdown();
    return 0;
}