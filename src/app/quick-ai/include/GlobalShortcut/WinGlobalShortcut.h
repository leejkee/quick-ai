//
// Created by 31305 on 2026/2/20.
//
#pragma once
#include <functional>
#include <memory>
#include <qnamespace.h>

class WinGlobalShortcut
{
public:
    static WinGlobalShortcut& instance();

    WinGlobalShortcut(const WinGlobalShortcut&) = delete;
    WinGlobalShortcut& operator=(const WinGlobalShortcut&) = delete;
    WinGlobalShortcut(WinGlobalShortcut&&) = delete;
    WinGlobalShortcut& operator=(WinGlobalShortcut&&) = delete;

    int registerShortcut(Qt::KeyboardModifiers mods,
                         Qt::Key key,
                         std::function<void()> callback);

    void unregisterShortcut(int id);

    void unregisterAll();

private:
    WinGlobalShortcut();
    ~WinGlobalShortcut();

    class Private;
    std::unique_ptr<Private> d;
};
