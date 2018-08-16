#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <node/node.h>
#include <node/uv.h>
#include "DownloadTest.h"

int main() {
    auto obj = std::make_shared<DownloadTest>();
    obj->init([]()->void{
        fprintf(stderr, "Download down\n");
    });
    obj->doWork();
    return uv_run(uv_default_loop(), UV_RUN_DEFAULT);
}