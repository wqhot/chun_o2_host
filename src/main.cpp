#ifndef NATIVE
#include <Arduino.h>
#include <node.hpp>
#include <collect.h>

NodeList list;
Collection collection(list);
Display display(list);

void setup()
{
    display.drawText(0, 0, "hello world!");
}

void loop()
{
    collection.recv();
    delay(10);
}

#endif