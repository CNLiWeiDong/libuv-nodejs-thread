# libuv-nodejs-thread

本案例主要为libuv或nodejs开发者提供libuv线程池或其他异步操作的面象对象使用方式。

nodejs中最大的便捷就是函数回调，正因为js中的闭包特性很好的利用了异步回调这个属性，使我们开发起来特别方便。
但node.js最大的问题就是单线程，这也是后端同学不愿意用nodejs最主要的原因。其实nodejs最主要的任务是处理IO的，试想1个线程都不能满足一台电脑上的所有IO操作，可以想象这台电脑除了CPU其他的设备都快~_~。

nodejs底层用的是libuv库，.NET底层用的也是这个库。所以node.js不是处理不了CPU密集型任务，是开发成本太大了。【所以我想利用c++11的高级特性来使用使用libuv】


首先libuv是由C语言开发，C语言回调只能用函数指针，没有闭包，只能用堆或全局内存来传递数据，
试想多个线程使用同一个函数指针，是不是得写线程锁。使用堆(new delete)是不是容易内存泄漏。再好的程序员也容易犯错，这些特性普通开发者都无法驾驭。

因此，我们要使用c++11中的lambda(可执行表达式)，shared_ptr(智能指针),class(面向对象)来使用libuv。

例如，本项目中的调用方式，开发者每次回调都是一个新的lambda，解放了多线程锁的问题。libuv使用到的数据指针都以对象的属性存储，我们也不用关心指针释放的问题。



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



最近一直在学习libuv源码，比起boost.asio使用难度大多了。不过libuv是一个框架，内部的线程池，通信，文件，进程等功能齐全。当然boost库功能更全，但功能全并不代表开发者能完美的结合这些功能。
大家对libuv感兴趣的可以互相交流，我的QQ号的 357879926