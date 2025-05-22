#include "client_pages.hpp"

void lab::EnterCourse::Load(ui::Screen *screen) noexcept
{
    auto mar=new ui::Margin();{
        mar->AddTo(screen);
        mar->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
        mar->SetMargin(80, 80, 200, 200);
    }
    {
        auto flat=new ui::Flat();{
            mar->Add(flat);
            flat->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
        }
        {
            backbtn = new ui::Button; {
                backbtn->AddTo(flat);
                backbtn->SetHPreset(ui::Control::Preset::WRAP_AT_END);
                backbtn->SetVPreset(ui::Control::Preset::WRAP_AT_FRONT);
                backbtn->SetCaption("返回");//private
            }
            auto hbox1 = new ui::HorizontalBox();{
                hbox1->AddTo(flat);
                hbox1->SetHPreset(ui::Control::Preset::FILL_FROM_CENTER);
                hbox1->SetVPreset(ui::Control::Preset::WRAP_AT_FRONT);
                hbox1->SetHSize(75);
                hbox1->SetVAnchor(5);
            }
            {   
                input = new ui::InputBox;{
                    input->AddTo(hbox1);
                    input->SetPreset(ui::Control::Preset::FILL_FROM_END);//private
                }
                auto hbtnbox1 = new ui::HorizontalBox();{
                    hbtnbox1->AddTo(hbox1);
                    hbtnbox1->SetHPreset(ui::Control::Preset::WRAP_AT_FRONT);
                    hbtnbox1->SetVPreset(ui::Control::Preset::FILL_FROM_CENTER);
                }
                {
                    btn1 = new ui::Button;{
                        btn1->AddTo(hbtnbox1);
                        btn1->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
                        btn1->SetCaption("查询");//private
                    }
                }
                
            }
            glabel=new ui::Label();{
                glabel->AddTo(flat);
                glabel->SetHPreset(ui::Control::Preset::WRAP_AT_CENTER);
                glabel->SetVPreset(ui::Control::Preset::WRAP_AT_FRONT);
                glabel->SetVAnchor(15);
                glabel->SetSizeWrap(ui::Control::Direction::HORIZONTAL, false);
                glabel->SetHSize(700);//private
            }
            addbtn = new ui::Button;{
                addbtn->AddTo(flat);
                addbtn->SetPreset(ui::Control::Preset::WRAP_AT_CENTER);
                addbtn->SetVAnchor(30);
                addbtn->SetCaption("添加课程");//private
            }
            btn2 = new ui::Button;{
                    btn2->AddTo(flat);
                    btn2->SetHPreset(ui::Control::Preset::WRAP_AT_CENTER);
                    btn2->SetVPreset(ui::Control::Preset::WRAP_AT_END);
                    btn2->SetCaption("查看课程");//private
                    btn2->SetVAnchor(55);
            }
        }
    }
}

void lab::EnterCourse::Logic(ui::Screen *screen) noexcept
{
    backbtn->SetClickCallback(UI_CALLBACK{
        SwitchTo(new eea::MainPage);
    });
    btn2->SetClickCallback(UI_CALLBACK{
        SwitchTo(new lab::CourseList);
    });
    input->SetInputCallback(UI_CALLBACK{
        coursename = input->GetText();
        glabel->SetContent("请输入课程名称");
        glabel->Show();
    });
    btn1->SetClickCallback(UI_CALLBACK{
        btn1->Enable(false);
       if(coursename=="") 
       {
        glabel->SetContent("课程名称不能为空");
        glabel->Show();
       }
       Listen(new trm::Sender({trm::rqs::SEARCH_COURSE_INFORMATION,account.code,coursename}),SD_CALLBACK{
        //if (reply[0] == trm::rpl::TIME_OUT) {
        //    glabel->SetContent("服务端未响应，请检查后重试");
        //    glabel->Show();
        //}
        if(reply[0] == trm::rpl::NO) {
            glabel->SetContent("无匹配课程,请重新输入");
            glabel->Show();
        }
        else {
            btn2->Hide();
            auto hbox = new ui::HorizontalBox; {
                hbox->AddTo(screen);
                hbox->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
                hbox->SetSizeWrap(ui::Control::Direction::HORIZONTAL, false);
                hbox->SetSizeWrap(ui::Control::Direction::VERTICAL, false);
                hbox->SetHSize(500);
                hbox->SetVSize(400);
            }
            {
                auto label0 = new ui::Label; {
                    label0->AddTo(hbox);
                    label0->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
                    label0->SetContent("课程编号："+reply[1]);
                }
                auto coursereply=trm::Split(reply[2]);
                auto label1 = new ui::Label; {
                    label1->AddTo(hbox);
                    label1->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
                    label1->SetContent("课程名称："+coursereply[0]);
                }
                auto label2 = new ui::Label; {
                    label2->AddTo(hbox);
                    label2->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
                    label2->SetContent("教师："+coursereply[1]);
                }
                auto label3 = new ui::Label; {
                    label3->AddTo(hbox);
                    label3->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
                    label3->SetContent("教室："+coursereply[2]);
                }
                auto label4 = new ui::Label; {
                    label4->AddTo(hbox);
                    label4->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
                    label4->SetContent("上课周数："+coursereply[3]);//存疑
                }
            }
            hbox->FreeAll();
            btn2->Show();
            btn1->Enable();
        }
       });
    });
    addbtn->SetClickCallback(UI_CALLBACK{
        SwitchTo(new lab::AddCourse);
    });
}

void lab::EnterCourse::Ready(ui::Screen *screen) noexcept
{
    ;
}

void lab::CourseList::Load(ui::Screen *screen) noexcept
{
    auto mar = new ui::Margin; {
        mar->AddTo(screen);
        mar->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
        mar->SetMargin(80, 80, 200, 200);
    }
    {
        auto flat = new ui::Flat; {
            mar->Add(flat);
            flat->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
        }
        {
            backbtn = new ui::Button; {
                backbtn->AddTo(flat);
                backbtn->SetHPreset(ui::Control::Preset::WRAP_AT_END);
                backbtn->SetVPreset(ui::Control::Preset::WRAP_AT_FRONT);
                backbtn->SetCaption("返回");//private
            }
            vsbox = new ui::VerticalScrollingBox;{
                vsbox->AddTo(flat);
                vsbox->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
                vsbox->SetSize(ui::Control::Direction::HORIZONTAL,90);
                vsbox->SetSize(ui::Control::Direction::VERTICAL, 80);
            }
        }
    }
}

void lab::CourseList::Logic(ui::Screen *screen) noexcept
{
    backbtn->SetClickCallback(UI_CALLBACK{
        SwitchTo(new lab::EnterCourse);
    });
}

void lab::CourseList::Ready(ui::Screen *screen) noexcept
{
    Listen(new trm::Sender({trm::rqs::CHECK_ALL_COURSE,account.code}),SD_CALLBACK{
        if(reply[0] == trm::rpl::FAIL) {
            auto glabel = new ui::Label; {
                glabel->AddTo(vsbox);
                glabel->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
                glabel->SetContent("没有课程存在");
                glabel->Show();
            }
        }
        else {
            for(auto course:reply) {
                auto hbox = new ui::HorizontalBox; {
                    hbox->AddTo(vsbox);
                    hbox->SetVPreset(ui::Control::Preset::WRAP_AT_FRONT);
                    hbox->SetHPreset(ui::Control::Preset::FILL_FROM_CENTER);
                    hbox->SetGap(50);
                }
                auto coursereply=trm::Split(course);
                {
                    auto label0 = new ui::Label;{
                        label0->AddTo(hbox);
                        label0->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
                        label0->SetContent("课程编号"+coursereply[0]);
                    }
                    auto label1 = new ui::Label;{
                        label1->AddTo(hbox);
                        label1->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
                        label1->SetContent("课程名称"+coursereply[1]);
                    }
                    auto label2 = new ui::Label;{
                        label2->AddTo(hbox);
                        label2->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
                        label2->SetContent("上课老师"+coursereply[2]);
                    }
                    auto label3 = new ui::Label;{
                        label3->AddTo(hbox);
                        label3->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
                        label3->SetContent("上课地点"+coursereply[3]);
                    }
                    auto label4 = new ui::Label;{
                        label4->AddTo(hbox);
                        label4->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
                        label4->SetContent("上课周数"+coursereply[4]);
                    }
                    debtn=new ui::Button;{
                        debtn->AddTo(hbox);
                        debtn->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
                        debtn->SetCaption("删除");
                    }
                }
            } 
       }
    });
}

void lab::AddCourse::Load(ui::Screen *screen) noexcept
{
    auto mar = new ui::Margin; {
        mar->AddTo(screen);
        mar->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
        mar->SetMargin(200, 200, 200, 200);
    }
    {
        auto flat = new ui::Flat; {
            mar->Add(flat);
            flat->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
        }
        {
            auto hbox1=new ui::HorizontalBox; {
                hbox1->AddTo(flat);
                hbox1->SetHPreset(ui::Control::Preset::FILL_FROM_CENTER);
                hbox1->SetVPreset(ui::Control::Preset::WRAP_AT_FRONT);
                hbox1->SetVAnchor(10);
                hbox1->SetHSize(80);
            }
            {
                input = new ui::InputBox; {
                    input->AddTo(hbox1);
                    input->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);//private
                }
                btn1 = new ui::Button; {
                    btn1->AddTo(hbox1);
                    btn1->SetPreset(ui::Control::Preset::WRAP_AT_END);//private
                    btn1->SetCaption("查找");
                }
            }
            glabel = new ui::Label; {
                glabel->AddTo(flat);
                glabel->SetHPreset(ui::Control::Preset::WRAP_AT_CENTER);
                glabel->SetVPreset(ui::Control::Preset::WRAP_AT_FRONT);
                glabel->SetVAnchor(25);
                glabel->SetSizeWrap(ui::Control::Direction::HORIZONTAL, false);
                glabel->SetHSize(700);//private
            }
            backbtn = new ui::Button; {
                backbtn->AddTo(flat);
                backbtn->SetHPreset(ui::Control::Preset::WRAP_AT_END);
                backbtn->SetVPreset(ui::Control::Preset::WRAP_AT_FRONT);
                backbtn->SetCaption("返回");//private
            }
            hbox = new ui::HorizontalBox; {
                hbox->AddTo(flat);
                hbox->SetVPreset(ui::Control::Preset::WRAP_AT_CENTER);
                hbox->SetHPreset(ui::Control::Preset::FILL_FROM_CENTER);
                hbox->SetSizeWrap(ui::Control::Direction::VERTICAL, false);
                hbox->SetVSize(200);
                hbox->SetVAnchor(55);
                }
                {
                    label0 = new ui::Label; {
                        label0->AddTo(hbox);
                        label0->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);//private
                    }
                    label1 = new ui::Label; {
                    label1->AddTo(hbox);
                    label1->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);//private
                    }
                    addbtn = new ui::Button; {
                        addbtn->AddTo(hbox);
                        addbtn->SetHPreset(ui::Control::Preset::WRAP_AT_END);
                        addbtn->SetVPreset(ui::Control::Preset::FILL_FROM_CENTER);
                        addbtn->SetCaption("添加");//private
                    }
                }
            rpllabel = new ui::Label; {
                rpllabel->AddTo(flat);
                rpllabel->SetPreset(ui::Control::Preset::PLACE_AT_END);//maybe
                rpllabel->SetHSize(600);
                rpllabel->SetVSize(100);
                rpllabel->SetHAnchor(95);
                rpllabel->SetVAnchor(95);
            }//private
        }
    }
}

void lab::AddCourse::Logic(ui::Screen *screen) noexcept
{
    backbtn->SetClickCallback(UI_CALLBACK{
        SwitchTo(new lab::EnterCourse);
    });
    
}

void lab::AddCourse::Ready(ui::Screen *screen) noexcept
{
    ;
}

void lab::EnterReserve::Load(ui::Screen *screen) noexcept
{
    auto mar=new ui::Margin();{
        screen->Add(mar);
        mar->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
        mar->SetMargin(80, 80, 200, 200);
    }
    {
        auto flat=new ui::Flat();{
            mar->Add(flat);
            flat->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
        }
        {
            backbtn = new ui::Button; {
                backbtn->AddTo(flat);
                backbtn->SetHPreset(ui::Control::Preset::WRAP_AT_END);
                backbtn->SetVPreset(ui::Control::Preset::WRAP_AT_FRONT);
                backbtn->SetCaption("返回");//private
            }
            auto hbox1 = new ui::HorizontalBox();{
                hbox1->AddTo(flat);
                hbox1->SetHPreset(ui::Control::Preset::FILL_FROM_CENTER);
                hbox1->SetVPreset(ui::Control::Preset::WRAP_AT_FRONT);
                hbox1->SetHSize(75);
                hbox1->SetVAnchor(5);

            }
            {   
                input = new ui::InputBox;{
                    input->AddTo(hbox1);
                    input->SetPreset(ui::Control::Preset::FILL_FROM_END);//private
                }
                auto hbtnbox1 = new ui::HorizontalBox();{
                    hbtnbox1->AddTo(hbox1);
                    hbtnbox1->SetHPreset(ui::Control::Preset::WRAP_AT_FRONT);
                    hbtnbox1->SetVPreset(ui::Control::Preset::FILL_FROM_CENTER);
                }
                {
                    btn1 = new ui::Button;{
                        btn1->AddTo(hbtnbox1);
                        btn1->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
                        btn1->SetCaption("查询");//private
                    }
                }
            }
            glabel=new ui::Label();{
                glabel->AddTo(flat);
                glabel->SetHPreset(ui::Control::Preset::WRAP_AT_CENTER);
                glabel->SetVPreset(ui::Control::Preset::WRAP_AT_FRONT);
                glabel->SetVAnchor(15);
                glabel->SetSizeWrap(ui::Control::Direction::HORIZONTAL, false);
                glabel->SetHSize(700);
            }//private
            auto hbtnbox2 = new ui::HorizontalBox();{
                hbtnbox2->AddTo(flat);
                hbtnbox2->SetPreset(ui::Control::Preset::WRAP_AT_CENTER);
            }
            {
                btn2 = new ui::Button;{
                    btn2->AddTo(hbtnbox2);
                    btn2->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
                    btn2->SetCaption("查看全部预约");//可能要稍作修改
                }//private
            }
        }
    }
}

void lab::EnterReserve::Logic(ui::Screen *screen) noexcept
{
    backbtn->SetClickCallback(UI_CALLBACK{
        SwitchTo(new lab::EnterCourse);
    });
    btn2->SetClickCallback(UI_CALLBACK{
        SwitchTo(new lab::ReserveList);
    });
}

void lab::EnterReserve::Ready(ui::Screen *screen) noexcept
{
    ;
}

void lab::ReserveList::Load(ui::Screen *screen) noexcept
{
    auto mar = new ui::Margin; {
        screen->Add(mar);
        mar->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
        mar->SetMargin(80, 80, 200, 200);
    }
    {
        auto flat = new ui::Flat; {
            mar->Add(flat);
            flat->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
        }
        {
            backbtn = new ui::Button; {
                backbtn->AddTo(flat);
                backbtn->SetHPreset(ui::Control::Preset::WRAP_AT_END);
                backbtn->SetVPreset(ui::Control::Preset::WRAP_AT_FRONT);
                backbtn->SetCaption("返回");//private
            }
            auto vsbox = new ui::VerticalScrollingBox;{
                vsbox->AddTo(flat);
                vsbox->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
                vsbox->SetSize(ui::Control::Direction::HORIZONTAL,90);
                vsbox->SetSize(ui::Control::Direction::VERTICAL, 80);
            }
            {
                auto hbox1 = new ui::HorizontalBox;{
                    hbox1->AddTo(vsbox);
                    hbox1->SetVPreset(ui::Control::Preset::WRAP_AT_FRONT);
                    hbox1->SetHPreset(ui::Control::Preset::FILL_FROM_CENTER);
                    hbox1->SetGap(50);
                }
                {
                    label0 = new ui::Label;{
                        label0->AddTo(hbox1);
                        label0->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
                        label0->SetContent("日期");//private
                    }
                    label1 = new ui::Label;{
                        label1->AddTo(hbox1);
                        label1->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
                        label1->SetContent("时间");//private
                    }
                    label2 = new ui::Label;{
                        label2->AddTo(hbox1);
                        label2->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
                        label2->SetContent("剩余名额");//private
                    }
                    label3 = new ui::Label;{
                        label3->AddTo(hbox1);
                        label3->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
                        label3->SetContent("预约状态");//private 
                    }
                }
                auto hbox2 = new ui::HorizontalBox;{
                    hbox2->AddTo(vsbox);
                    hbox2->SetVPreset(ui::Control::Preset::WRAP_AT_FRONT);
                    hbox2->SetHPreset(ui::Control::Preset::FILL_FROM_CENTER);
                    hbox2->SetGap(50);
                }
                {
                    auto label20 = new ui::Label;{
                        label20->AddTo(hbox2);
                        label20->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
                        label20->SetContent("日期");
                    }
                    auto label21 = new ui::Label;{
                        label21->AddTo(hbox2);
                        label21->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
                        label21->SetContent("时间");
                    }
                    auto label22 = new ui::Label;{
                        label22->AddTo(hbox2);
                        label22->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
                        label22->SetContent("剩余名额");
                    }
                    auto label23 = new ui::Label;{
                        label23->AddTo(hbox2);
                        label23->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
                        label23->SetContent("预约状态");
                    }
                }
            } 
        }
    }
}

void lab::ReserveList::Logic(ui::Screen *screen) noexcept
{
    backbtn->SetClickCallback(UI_CALLBACK{
        SwitchTo(new lab::EnterReserve);
    });
}

void lab::ReserveList::Ready(ui::Screen *screen) noexcept
{
    ;
}

void lab::Reserve::Load(ui::Screen *screen) noexcept
{
    auto mar = new ui::Margin; {
        screen->Add(mar);
        mar->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
        mar->SetMargin(200, 200, 200, 200);
    }
    auto flat = new ui::Flat; {
        mar->Add(flat);
        flat->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
    }
    {
        backbtn = new ui::Button; {
            backbtn->AddTo(flat);
            backbtn->SetHPreset(ui::Control::Preset::WRAP_AT_END);
            backbtn->SetVPreset(ui::Control::Preset::WRAP_AT_FRONT);
            backbtn->SetCaption("返回");//private
        }
        auto hbox = new ui::HorizontalBox; {
            hbox->AddTo(flat);
            hbox->SetVPreset(ui::Control::Preset::WRAP_AT_CENTER);
            hbox->SetHPreset(ui::Control::Preset::FILL_FROM_CENTER);
            hbox->SetSizeWrap(ui::Control::Direction::VERTICAL, false);
            hbox->SetVSize(250);
        }
        {
            label0 = new ui::Label; {
                label0->AddTo(hbox);
                label0->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
                label0->SetContent("日期");//private
            }
            label1 = new ui::Label; {
                label1->AddTo(hbox);
                label1->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
                label1->SetContent("时间");//private
            }
            label2 = new ui::Label; {
                label2->AddTo(hbox);
                label2->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
                label2->SetContent("剩余名额");//private
            }
            label3 = new ui::Label; {
                label3->AddTo(hbox);
                label3->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
                label3->SetContent("预约状态");//private
            }
            auto vbox = new ui::VerticalBox; {
                vbox->AddTo(hbox);
                vbox->SetHPreset(ui::Control::Preset::FILL_FROM_END);
                vbox->SetVPreset(ui::Control::Preset::FILL_FROM_CENTER);
                vbox->SetSizeWrap(ui::Control::Direction::HORIZONTAL, false);
                vbox->SetHSize(50);
            }
            {
                addbtn = new ui::Button; {
                    addbtn->AddTo(vbox);
                    addbtn->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
                    addbtn->SetCaption("添加");//private
                }
                debtn = new ui::Button; {
                    debtn->AddTo(vbox);
                    debtn->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
                    debtn->SetCaption("删除");//private
                }
            }
        }
        rpllabel = new ui::Label; {
            rpllabel->AddTo(flat);
            rpllabel->SetPreset(ui::Control::Preset::WRAP_AT_END);
            rpllabel->SetSizeWrap(ui::Control::Direction::HORIZONTAL, false);
            rpllabel->SetSizeWrap(ui::Control::Direction::VERTICAL, false);
            rpllabel->SetHSize(700);
            rpllabel->SetVSize(100);
            rpllabel->SetHAnchor(95);
            rpllabel->SetVAnchor(95);
        }//private
    }
}

void lab::Reserve::Logic(ui::Screen *screen) noexcept
{
    backbtn->SetClickCallback(UI_CALLBACK{
        SwitchTo(new lab::EnterReserve);
    });
}
//刚才push不上去，写个注释
void lab::Reserve::Ready(ui::Screen *screen) noexcept
{
    ;
}
