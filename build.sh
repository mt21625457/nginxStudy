#!/bin/bash

export CFLAGS="-I/usr/local/include"
./configure --prefix=/usr/local \
    --with-cc=clang \
    --with-cpp=clang++ \
    --with-http_ssl_module \
    --with-http_v2_module \
    --with-http_ssl_module \
    --with-http_image_filter_module \
    --with-stream=dynamic \
    --with-stream_ssl_module \
    --with-stream_realip_module \
    --with-google_perftools_module \
    --user=mt \
    --group=mt \
    