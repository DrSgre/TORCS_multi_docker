#include<stdio.h>
#include<iostream>
#include<etcd/Client.hpp>

int main(void)
{
    etcd::Client etcd("http://172.23.0.2:2379");
    etcd::Response response = etcd.get("foo").get();
    std::cout << response.value().as_string();
    return 0;
}

