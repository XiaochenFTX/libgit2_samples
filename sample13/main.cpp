//
// Created by 王晓辰 on 16/7/19.
//

#include <git2.h>

#include <iostream>

const char* path = "/Users/xiaochen/Documents/data";

int main()
{
    git_libgit2_init();

    git_repository* rep = nullptr;

    git_rebase* prebase = nullptr;
    git_rebase_options rebase_opt = GIT_REBASE_OPTIONS_INIT;

    git_reference* onto_branch = nullptr;
    git_annotated_commit* onto = nullptr;

    git_rebase_operation* operation = nullptr;

    size_t entrycount;
    size_t current;

    git_signature* me = nullptr;

    git_index* index = nullptr;
    git_oid new_commit;

    int error = 0;

    // git open
    git_repository_open(&rep, path);

    error = git_rebase_open(&prebase, rep, &rebase_opt);
    // There is no rebase in progress
    if (error == GIT_ENOTFOUND)
    {
        git_branch_lookup(&onto_branch, rep, "new", GIT_BRANCH_LOCAL);
        git_annotated_commit_from_ref(&onto, rep, onto_branch);
        git_rebase_init(&prebase, rep, nullptr /* current branch */, nullptr /* upstream */
                , onto /* branch to rebase onto */, &rebase_opt);
    }
    else if (error < 0)
    {
        const git_error *e = giterr_last();
        std::cout << "Error: " << error << " / " << e->klass << " : " << e->message << std::endl;
        goto SHUTDOWN;
    }

    entrycount = git_rebase_operation_entrycount(prebase);
    std::cout<< "rebase entry count: " << entrycount << "\n";

    while (git_rebase_next(&operation, prebase) != GIT_ITEROVER)
    {

        current = git_rebase_operation_current(prebase); // (called `init` but not yet `next`) then this returns `GIT_REBASE_NO_OPERATION`
        std::cout<< "rebase current index: " << current << "\n";
        if (GIT_REBASE_NO_OPERATION == current)
        {
            std::cout<< "rebase no operation" << "\n";
        }
        operation = git_rebase_operation_byindex(prebase, 0);
    }

    // reslove conflicts
    git_repository_index(&index, rep);
    if (git_index_has_conflicts(index))
    {
        // git checkout --theirs
        git_checkout_options opt = GIT_CHECKOUT_OPTIONS_INIT;
        opt.checkout_strategy |= GIT_CHECKOUT_USE_THEIRS;
        git_checkout_index(rep, index, &opt);
    }

    // add
    git_index_update_all(index, nullptr, nullptr, nullptr);
    git_index_write(index);

    // commit
    git_signature_now(&me, "XiaochenFTX", "xiaochenftx@gmail.com");
    git_rebase_commit(&new_commit, prebase, me, me, "UTF-8", "new rebase");

    error = git_rebase_finish(prebase, me);
    if (error < 0)
    {
        const git_error *e = giterr_last();
        std::cout << "Error: " << error << " / " << e->klass << " : " << e->message << std::endl;
        goto SHUTDOWN;
    }

//    git_rebase_abort(prebase);

    git_rebase_free(prebase);

SHUTDOWN:
    git_repository_free(rep);
    git_libgit2_shutdown();
    return 0;
}