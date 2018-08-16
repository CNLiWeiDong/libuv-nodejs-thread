//
// Created by 李卫东 on 2018/8/15.
//

#include "DownloadTest.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



void DownloadTest::fake_download(uv_work_t *req) {
    auto obj = *((std::shared_ptr<DownloadTest>*) req->data);
    obj->onDownload();
}
void DownloadTest::after(uv_work_t *req, int status) {
    auto obj = *((std::shared_ptr<DownloadTest>*) req->data);
    obj->onAfterWork();
}

void DownloadTest::print_progress(uv_async_t *handle) {
    auto obj = *((std::shared_ptr<DownloadTest>*) handle->data);
    obj->onProgress();
}

void DownloadTest::init(std::function<void()> fun)
{
    uv_data_ptr = shared_from_this();
    end_callback = fun;
}
void DownloadTest::doWork()
{
    assert(uv_data_ptr&&end_callback);
    auto loop = uv_default_loop();
    //thread
    req.data = (void*)&uv_data_ptr;
    uv_queue_work(loop,&req,DownloadTest::fake_download, DownloadTest::after);
    //async
    async.data = (void*)&uv_data_ptr;
    uv_async_init(loop,&async,DownloadTest::print_progress);
}
void DownloadTest::onDownload()
{
    int downloaded = 0;
    while (downloaded < size) {
        percentage = downloaded*100.0/size;
        uv_async_send(&async);
        sleep(1);
        downloaded += (200+random())%1000;
    }
}
void DownloadTest::onAfterWork()
{
    onEnd();
    fprintf(stderr, "Download complete\n");
    uv_close((uv_handle_t*) &async, NULL);
}
void DownloadTest::onProgress()
{
    fprintf(stderr, "Downloaded %.2f%%\n", percentage);
}
void DownloadTest::onEnd()
{
    uv_data_ptr = nullptr ;
    end_callback();
    end_callback = nullptr;
}