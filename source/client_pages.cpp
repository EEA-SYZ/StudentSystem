#include "client_pages.hpp"

void clpg::EnterSystem::Load(ui::Screen *screen) noexcept
{
    btn = new ui::Button;{
        btn->AddTo(screen);
        btn->SetPreset(ui::Control::Preset::WRAP_AT_CENTER);
        btn->SetCaption("进入系统");
    }
}

void clpg::EnterSystem::Logic(ui::Screen *screen) noexcept
{
    btn->SetClickCallback(UI_CALLBACK{
        screen->HideAll();
        auto [success, reply] = WaitServer(screen, {trm::rqs::CHECK_ONLINE}, "正在检查服务端在线状态");
        screen->FreeAllVisible();
        screen->ShowAll();
        if (success == 1) {
            if (reply[0] == trm::rpl::YES) {
                SwitchTo(new eea::Login);
            } else {
                assert(false); // Unexpected reply.
            }
        } else if (success == 0) {
            SwitchTo(new eea::Retry);
        }
    });
}

void clpg::EnterSystem::Ready(ui::Screen *screen) noexcept
{
    ;
}
