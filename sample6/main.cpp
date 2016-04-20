//
// Created by 王晓辰 on 16/4/20.
//

#include <git2.h>

#include <iostream>

const char* path = "/Users/xiaochen/Documents/data";

int main()
{
    git_libgit2_init();

    git_repository* rep = nullptr;
    git_reference* head = nullptr;
    git_reference* new_branch = nullptr;
    const git_oid* commit_id = nullptr;
    git_commit* last_commit = nullptr;

    git_branch_iterator* branch_iterator = nullptr;
    git_reference* tmp_branch = nullptr;
    git_branch_t branch_type;

    git_reference* lookup_branch = nullptr;

    git_reference* renamed_branch = nullptr;

    int error = 0;

    // git open
    error = git_repository_open(&rep, path);
    if (error < 0)
    {
        const git_error *e = giterr_last();
        std::cout << "Error: " << error << " / " << e->klass << " : " << e->message << std::endl;

        goto SHUTDOWN;
    }

    git_repository_head(&head, rep);
    commit_id = git_reference_target(head);

    std::cout<< "last commit: " << git_oid_tostr_s(commit_id) <<std::endl;

    git_commit_lookup(&last_commit, rep, commit_id);

    // new branch
    error = git_branch_create(&new_branch, rep, "new", last_commit, 0);
//    if (error < 0)
//    {
//        const git_error *e = giterr_last();
//        std::cout << "Error: " << error << " / " << e->klass << " : " << e->message << std::endl;
//    }

    // list branch
    git_branch_iterator_new(&branch_iterator, rep, GIT_BRANCH_LOCAL);
    while (GIT_ITEROVER != git_branch_next(&tmp_branch, &branch_type, branch_iterator))
    {
        const char* branch_name;
        git_branch_name(&branch_name, tmp_branch);
        bool is_head = git_branch_is_head(tmp_branch);
        if (is_head)
        {
            std::cout<< "*";
        }
        std::cout<< "name: " << branch_name <<std::endl;
    }

    // delete branch
//    git_branch_lookup(&lookup_branch, rep, "new", GIT_BRANCH_LOCAL);
//    if (lookup_branch != nullptr && !git_branch_is_head(lookup_branch))
//    {
//        git_branch_delete(lookup_branch);
//    }

    // rename branch
//    git_branch_lookup(&lookup_branch, rep, "new", GIT_BRANCH_LOCAL);
//    git_branch_move(&renamed_branch, lookup_branch, "new3", 0);

    // checkout branch
//    git_branch_lookup(&lookup_branch, rep, "new", GIT_BRANCH_LOCAL);
//    git_repository_set_head(rep, git_reference_name(lookup_branch));

    git_reference_free(new_branch);
    git_reference_free(head);
    git_commit_free(last_commit);

SHUTDOWN:
    git_repository_free(rep);
    git_libgit2_shutdown();
    return 0;
}