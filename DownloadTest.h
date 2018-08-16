//
// Created by 李卫东 on 2018/8/15.
//

#ifndef TEST_DOWNLOADTEST_H
#define TEST_DOWNLOADTEST_H

#include "node_head.h"
#include <functional>

class DownloadTest : public std::enable_shared_from_this<DownloadTest>{

private:
    uv_async_t async;
    int size = 10240;
    double percentage;
    uv_work_t req;
    std::shared_ptr<DownloadTest> uv_data_ptr = nullptr;
    std::function<void()> end_callback = nullptr;
    void onEnd();
    void onDownload();
    void onAfterWork();
    void onProgress();
    static void fake_download(uv_work_t *req);
    static void after(uv_work_t *req, int status);
    static void print_progress(uv_async_t *handle);
public:
    DownloadTest(){}
    void init(std::function<void()> fun);
    void doWork();
};


#endif //TEST_DOWNLOADTEST_H
