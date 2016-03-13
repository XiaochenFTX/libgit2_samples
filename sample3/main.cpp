//
// Created by 王晓辰 on 16/3/11.
//

#include <git2.h>

#include <iostream>


const char* path = "/Users/xiaochen/Documents/data";


int main()
{
    git_libgit2_init();

    git_repository* rep = nullptr;
    git_reference* ref_head = nullptr;
    git_index* index = nullptr;
    git_signature* me = nullptr;
    size_t parent_count = 0;
    git_commit* parent_commit = nullptr;
    git_tree* new_tree = nullptr;
    git_oid new_tree_id;
    git_oid new_commit;
    int error = 0;

    const git_commit *parents[] = {nullptr};

    // git open
    error = git_repository_open(&rep, path);
    if (error < 0)
    {
        const git_error *e = giterr_last();
        std::cout << "Error: " << error << " / " << e->klass << " : " << e->message << std::endl;

        goto SHUTDOWN;
    }

    // head ref
    error = git_repository_head(&ref_head, rep);

    // has parent
    if (error == GIT_OK)
    {
        // get parent commit
        git_commit_lookup(&parent_commit, rep, git_reference_target(ref_head));
        parents[0] = parent_commit;
        parent_count = 1;
        git_reference_free(ref_head);
    }
    else if (error != GIT_EUNBORNBRANCH)
    {
        const git_error *e = giterr_last();
        std::cout << "Error: " << error << " / " << e->klass << " : " << e->message << std::endl;

        goto SHUTDOWN;
    }

    // get index
    git_repository_index(&index, rep);

    // git add .
    error = git_index_add_all(index, nullptr, GIT_INDEX_ADD_DEFAULT, nullptr, nullptr);
    if (error < 0)
    {
        const git_error *e = giterr_last();
        std::cout << "Error: " << error << " / " << e->klass << " : " << e->message << std::endl;
        git_index_free(index);
        goto SHUTDOWN;
    }
    else
    {
        /* Write the in-memory index to disk */
        git_index_write(index);
    }

    // write index to tree
    error = git_index_write_tree(&new_tree_id, index);
    if (error < 0)
    {
        const git_error *e = giterr_last();
        std::cout << "Error: " << error << " / " << e->klass << " : " << e->message << std::endl;
        goto SHUTDOWN;
    }
    else
    {
        git_index_free(index);
    }

    git_tree_lookup(&new_tree, rep, &new_tree_id);

    // signature
    git_signature_now(&me, "XiaochenFTX", "xiaochenftx@gmail.com");

    // new commit
    error = git_commit_create(&new_commit, rep, "HEAD", me, me, "UTF-8", "commit message", new_tree, parent_count,
                              parents);

    git_commit_free(parent_commit);
    git_tree_free(new_tree);
    git_signature_free(me);

SHUTDOWN:
    git_repository_free(rep);
    git_libgit2_shutdown();

    return 0;
}