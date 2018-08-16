//
// Created by 李卫东 on 2018/8/16.
//
#include "node_head.h"
#include "DownloadTest.h"

namespace demo {
    using v8::FunctionCallbackInfo;
    using v8::Isolate;
    using v8::Local;
    using v8::Object;
    using v8::String;
    using v8::Value;
    using v8::Function;
    using v8::Persistent;

    void Method(const FunctionCallbackInfo<Value>& args) {
        Isolate* isolate = args.GetIsolate();
        v8::HandleScope scope(isolate);
        if (args.Length() < 1)
        {
            // Throw an Error that is passed back to JavaScript
            isolate->ThrowException(v8::Exception::TypeError(
                    String::NewFromUtf8(isolate, "Wrong number of arguments")));
            return;
        }
        if(!args[0]->IsFunction())
        {
            // Throw an Error that is passed back to JavaScript
            isolate->ThrowException(v8::Exception::TypeError(
                    String::NewFromUtf8(isolate, "Wrong type of arguments")));
            return;
        }

        Local<Function> cb = Local<Function>::Cast(args[0]);
        auto p_cb = std::make_shared<Persistent<Function>>(isolate,cb);
//        Persistent<Function> p_cb(isolate,cb); 不能复制传递
        auto obj = std::make_shared<DownloadTest>();
        obj->init([isolate,p_cb]()->void{
            v8::HandleScope scope(isolate);
            Local<Function> js_callback = Local<Function>::New(isolate,*(p_cb.get()));
            const unsigned argc = 1;
            Local<Value> argv[argc] = { String::NewFromUtf8(isolate, "hello world") };
            js_callback->Call(isolate->GetCurrentContext()->Global(), argc, argv);
        });
        obj->doWork();

        args.GetReturnValue().Set(v8::Number::New(isolate,1));
    }
    void init(Local<Object> exports) {
        NODE_SET_METHOD(exports, "hello", Method);
    }

    NODE_MODULE(test, init)

}  // namespace demo