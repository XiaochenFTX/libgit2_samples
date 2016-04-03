//
// Created by 王晓辰 on 16/3/30.
//

#include <git2.h>

#include <iostream>


int file_cb(const git_diff_delta *delta, float progress, void *payload)
{
    std::cout<< "old file path: " << delta->old_file.path <<"\n";
    std::cout<< "new file path: " << delta->new_file.path <<"\n";
    std::cout<< "file number: " << delta->nfiles <<"\n";
    std::cout<< "status: " << delta->status <<"\n";
    std::cout<< "flags: " << delta->flags <<"\n";
    std::cout<< "progress: " << progress << "\n";
    std::cout<< "==================" << std::endl;

    return 0;
}

int binary_cb(const git_diff_delta *delta, const git_diff_binary *binary, void *payload)
{
    std::cout<< "old file path: " << delta->old_file.path <<"\n";
    std::cout<< "new file path: " << delta->old_file.path <<"\n";
    std::cout<< "=================" << std::endl;

    return 0;
}

int hunk_cb(const git_diff_delta *delta, const git_diff_hunk *hunk, void *payload)
{
    std::cout<< "old start: " << hunk->old_start <<"\n";
    std::cout<< "old lines: " << hunk->old_lines <<"\n";
    std::cout<< "new start: " << hunk->new_start <<"\n";
    std::cout<< "new lines: " << hunk->new_lines <<"\n";
    std::cout<< "header: " << hunk->header <<"\n";
    std::cout<< "=================" << std::endl;

    return 0;
}

int line_cb(const git_diff_delta *delta, const git_diff_hunk *hunk, const git_diff_line *line, void *payload)
{
    std::cout<< "origin: " << line->origin <<"\n";
    std::cout<< "old line number: " << line->old_lineno <<"\n";
    std::cout<< "new line number: " << line->new_lineno <<"\n";
    std::cout<< "num lines: " << line->num_lines <<"\n";
    std::cout<< "content offset: " << line->content_offset <<"\n";
    std::cout<< "=================" << std::endl;

    return 0;
}

int notify_cb(const git_diff *diff_so_far, const git_diff_delta *delta_to_add, const char *matched_pathspec, void *payload)
{
    std::cout<< "***notify cb***" <<"\n";
    std::cout<< "old file: " << delta_to_add->old_file.path <<"\n";
    std::cout<< "new file: " << delta_to_add->new_file.path <<"\n";
    std::cout<< "matched path spec: " << (matched_pathspec != nullptr ? matched_pathspec : "null") <<"\n";
    std::cout<< "*****************" << std::endl;

    return 0;
}

int progress_cb(const git_diff *diff_so_far, const char *old_path, const char *new_path, void *payload)
{
    std::cout<< "****pro cb****" <<"\n";
    std::cout<< "old path: " << old_path <<"\n";
    std::cout<< "new path: " << new_path <<"\n";
    std::cout<< "*****************" << std::endl;

    return 0;
}

const char* path = "/Users/xiaochen/Documents/data";

int main()
{
    git_libgit2_init();

    git_repository* rep = nullptr;
    int error = 0;

    git_diff* diff_index_to_workdir = nullptr;
    git_diff_options diff_opt = GIT_DIFF_OPTIONS_INIT;
    git_diff_find_options diff_find_opt = GIT_DIFF_FIND_OPTIONS_INIT;

    // git open
    error = git_repository_open(&rep, path);
    if (error < 0)
    {
        const git_error *e = giterr_last();
        std::cout << "Error: " << error << " / " << e->klass << " : " << e->message << std::endl;

        goto SHUTDOWN;
    }

//    diff_opt = GIT_DIFF_OPTIONS_INIT;
    error = git_diff_init_options(&diff_opt, GIT_DIFF_OPTIONS_VERSION);
    diff_opt.flags |= GIT_DIFF_INCLUDE_UNTRACKED;
    diff_opt.notify_cb = notify_cb;
    diff_opt.progress_cb = progress_cb;

    // diff index to work dir
    error = git_diff_index_to_workdir(&diff_index_to_workdir, rep, nullptr, &diff_opt);
    if (error < 0)
    {
        const git_error *e = giterr_last();
        std::cout << "Error: " << error << " / " << e->klass << " : " << e->message << std::endl;

        goto SHUTDOWN;
    }

    // diff copy or rename
    diff_find_opt.flags |= GIT_DIFF_FIND_RENAMES;
    diff_find_opt.flags |= GIT_DIFF_FIND_COPIES;
    diff_find_opt.flags |= GIT_DIFF_FIND_FOR_UNTRACKED;

    error = git_diff_find_similar(diff_index_to_workdir, &diff_find_opt);

    error = git_diff_foreach(diff_index_to_workdir, file_cb, binary_cb, hunk_cb, line_cb, nullptr);

    git_diff_free(diff_index_to_workdir);

SHUTDOWN:
    git_repository_free(rep);
    git_libgit2_shutdown();
    return 0;
}