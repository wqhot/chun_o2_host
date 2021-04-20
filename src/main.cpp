#ifndef NATIVE
#include <Arduino.h>
#include <node.hpp>
#include <collect.h>

NodeList list;
Collection collection(list);
Display display(list);

void setup()
{
}

void loop()
{
    collection.recv();
}

#endif