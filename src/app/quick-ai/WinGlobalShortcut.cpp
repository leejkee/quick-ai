//
// Created by 31305 on 2026/2/20.
//
#include <GlobalShortcut/WinGlobalShortcut.h>
#include <QAbstractNativeEventFilter>
#include <QDebug>
#include <QGuiApplication>
#include <QMap>

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

class WinGlobalShortcut::Private : public QAbstractNativeEventFilter
{
public:
    Private() : m_nextId(1000)
    {
        if (qApp)
        {
            qApp->installNativeEventFilter(this);
        }
    }

    ~Private() override
    {
        unregisterAll();
        if (qApp)
        {
            qApp->removeNativeEventFilter(this);
        }
    }

    int registerShortcut(Qt::KeyboardModifiers mods,
                         Qt::Key key,
                         std::function<void()> callback)
    {
        UINT nativeMods = MOD_NOREPEAT;
        UINT nativeKey = 0;

        if (mods & Qt::ShiftModifier)
        {
            nativeMods |= MOD_SHIFT;
        }
        if (mods & Qt::ControlModifier)
        {
            nativeMods |= MOD_CONTROL;
        }
        if (mods & Qt::AltModifier)
        {
            nativeMods |= MOD_ALT;
        }
        if (mods & Qt::MetaModifier)
        {
            nativeMods |= MOD_WIN;
        }

        if (key >= Qt::Key_A && key <= Qt::Key_Z)
        {
            nativeKey = key;
        }
        else if (key >= Qt::Key_0 && key <= Qt::Key_9)
        {
            nativeKey = key;
        }
        else if (key >= Qt::Key_F1 && key <= Qt::Key_F24)
        {
            nativeKey = VK_F1 + (key - Qt::Key_F1);
        }
        else if (key == Qt::Key_Space)
        {
            nativeKey = VK_SPACE;
        }
        else if (key == Qt::Key_Escape)
        {
            nativeKey = VK_ESCAPE;
        }
        else if (key == Qt::Key_Return || key == Qt::Key_Enter)
        {
            nativeKey = VK_RETURN;
        }
        else
        {
            qWarning() << "WinGlobalShortcut: 暂未映射该按键，注册失败。";
            return -1;
        }

        int id = m_nextId++;
        if (RegisterHotKey(NULL, id, nativeMods, nativeKey))
        {
            m_callbacks[id] = std::move(callback);
            return id;
        }

        qWarning() << "WinGlobalShortcut: 快捷键注册失败，可能被占用。";
        return -1;
    }

    void unregisterShortcut(int id)
    {
        if (m_callbacks.contains(id))
        {
            UnregisterHotKey(NULL, id);
            m_callbacks.remove(id);
        }
    }

    void unregisterAll()
    {
        for (const int id : m_callbacks.keys())
        {
            UnregisterHotKey(NULL, id);
        }
        m_callbacks.clear();
    }

    bool nativeEventFilter(const QByteArray& eventType,
                           void* message,
                           qintptr* result) override
    {
        Q_UNUSED(result);
        if (eventType == "windows_generic_MSG" ||
            eventType == "windows_dispatcher_MSG")
        {
            if (const auto msg = static_cast<MSG*>(message);
                msg->message == WM_HOTKEY)
            {
                if (const int id = static_cast<int>(msg->wParam);
                    m_callbacks.contains(id))
                {
                    m_callbacks[id]();
                    return true;
                }
            }
        }
        return false;
    }

    int m_nextId;
    QMap<int, std::function<void()>> m_callbacks;
};

WinGlobalShortcut& WinGlobalShortcut::instance()
{
    static WinGlobalShortcut instance;
    return instance;
}

WinGlobalShortcut::WinGlobalShortcut() : d(std::make_unique<Private>()) {}

WinGlobalShortcut::~WinGlobalShortcut() = default;

int WinGlobalShortcut::registerShortcut(Qt::KeyboardModifiers mods,
                                        Qt::Key key,
                                        std::function<void()> callback)
{
    return d->registerShortcut(mods, key, std::move(callback));
}

void WinGlobalShortcut::unregisterShortcut(int id)
{
    d->unregisterShortcut(id);
}

void WinGlobalShortcut::unregisterAll() { d->unregisterAll(); }
