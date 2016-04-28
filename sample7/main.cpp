//
// Created by 王晓辰 on 16/4/26.
//

#include <git2.h>

#include <iostream>

const char* path = "/Users/xiaochen/Documents/data";

int main()
{
    git_libgit2_init();

    git_repository* rep = nullptr;

    const git_annotated_commit* their_head[10];
    git_merge_options merge_opt = GIT_MERGE_OPTIONS_INIT;
    git_checkout_options checkout_opt = GIT_CHECKOUT_OPTIONS_INIT;

    git_reference* branch = nullptr;
    git_index* index = nullptr;
    git_index_conflict_iterator* conflict_iterator = nullptr;
    git_oid new_tree_id;

    git_tree* new_tree = nullptr;
    git_signature* me = nullptr;
    git_reference* head = nullptr;
    git_commit* parent_our = nullptr;
    git_commit* parent_their = nullptr;
    git_oid commit_id;

    int error = 0;

    // git open
    error = git_repository_open(&rep, path);
    if (error < 0)
    {
        const git_error *e = giterr_last();
        std::cout << "Error: " << error << " / " << e->klass << " : " << e->message << std::endl;

        goto SHUTDOWN;
    }

    // git merge <branch>
    git_reference_dwim(&branch, rep, "new");
    git_annotated_commit_from_ref((git_annotated_commit **)&their_head[0], rep, branch);
    error = git_merge(rep, their_head, 1, &merge_opt, &checkout_opt);
    if (error < 0)
    {
        const git_error *e = giterr_last();
        std::cout << "Error: " << error << " / " << e->klass << " : " << e->message << std::endl;

        goto SHUTDOWN;
    }

    git_repository_index(&index, rep);

    // reslove conflicts
    if (git_index_has_conflicts(index))
    {
        const git_index_entry* ancestor_out = nullptr;
        const git_index_entry* our_out = nullptr;
        const git_index_entry* their_out = nullptr;

        git_index_conflict_iterator_new(&conflict_iterator, index);

        while (git_index_conflict_next(&ancestor_out, &our_out, &their_out, conflict_iterator) != GIT_ITEROVER)
        {
            if (ancestor_out) std::cout<< "ancestor: " << ancestor_out->path <<std::endl;
            if (our_out) std::cout<< "our: " << our_out->path <<std::endl;
            if (their_out) std::cout<< "their: " << their_out->path <<std::endl;

            // do sth. or rewrite file by code. ...
            // and remove the conflict
//            git_index_conflict_remove(index, "...");
        }

        // git checkout --theirs <file>
        git_checkout_options opt = GIT_CHECKOUT_OPTIONS_INIT;
        opt.checkout_strategy |= GIT_CHECKOUT_USE_THEIRS;
//        const char* p = "file";
//        opt.paths.strings = (char**)&p;
//        opt.paths.count = 1;
        git_checkout_index(rep, index, &opt);

        git_index_conflict_iterator_free(conflict_iterator);
    }

    // add and commit
    git_index_update_all(index, nullptr, nullptr, nullptr);
    git_index_write(index);
    git_index_write_tree(&new_tree_id, index);
    git_tree_lookup(&new_tree, rep, &new_tree_id);

    git_signature_now(&me, "XiaochenFTX", "xiaochenftx@gmail.com");

    git_repository_head(&head, rep);
    git_commit_lookup(&parent_our, rep, git_reference_target(head));
    git_commit_lookup(&parent_their, rep, git_reference_target(branch));

    git_commit_create_v(&commit_id, rep, "HEAD", me, me, "UTF-8", "merge commit", new_tree, 2, parent_our, parent_their);

    git_repository_state_cleanup(rep);

SHUTDOWN:
    git_repository_free(rep);
    git_libgit2_shutdown();
    return 0;
}