#include "client_pages.hpp"

trm::Book vio::EnterLibrary::selectedBook;
std::vector<trm::Book> vio::EnterLibrary::books = {};
std::vector<trm::BorrowLog> vio::EnterLibrary::borrowLogs = {};

std::vector<std::string> vio::BookManage::room302 = {"数理科学与化学", "天文学、地球科学", "环境科学"};
std::vector<std::string> vio::BookManage::room303 = {"自动化技术", "计算机技术", "建筑设计"};
std::vector<std::string> vio::BookManage::room401 = {"法律", "经济"};
std::vector<std::string> vio::BookManage::room402 = {"马克思主义、列宁主义、毛泽东思想、邓小平理论", "社会科学", "哲学", "军事", "语言"};
std::vector<std::string> vio::BookManage::room403 = {"文学", "艺术", "历史"};
std::vector<std::string> vio::BookManage::room621 = {"医药、卫生"};

void vio::EnterLibrary::Load(ui::Screen *screen) noexcept
{
    ui::Margin *mar = new ui::Margin;{
        mar->AddTo(screen);
        mar->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
        mar->SetMargin(120, 150, 250, 250);
    }
    {
        vbox = new ui::VerticalBox;{
            vbox->AddTo(mar);
            vbox->SetGap(5);
        }
        {   
            returnBtn = new ui::Button;{
                returnBtn->AddTo(vbox);
                returnBtn->SetHPreset(ui::Control::Preset::WRAP_AT_FRONT);
                returnBtn->SetVPreset(ui::Control::Preset::WRAP_AT_FRONT);
                returnBtn->SetFontSize(50);
                returnBtn->SetCaption("←返回");
            }
            ui::HorizontalBox *topBox = new ui::HorizontalBox;{
                topBox->AddTo(vbox);
                topBox->SetHPreset(ui::Control::Preset::FILL_FROM_CENTER);
                topBox->SetVSize(30);
                topBox->SetGap(5);
            }
            {
                searchBox = new ui::InputBox;{
                    searchBox->AddTo(topBox);
                    searchBox->SetSize(1000, 30);
                    searchBox->SetText("在此输入搜索内容");
                }
                searchOptBtn = new ui::Button;{
                    searchOptBtn->AddTo(topBox);
                    searchOptBtn->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
                    searchOptBtn->SetCaption("筛选>");
                }
                searchBtn = new ui::Button;{
                    searchBtn->AddTo(topBox);
                    searchBtn->SetHPreset(ui::Control::Preset::WRAP_AT_END);
                    searchBtn->SetVPreset(ui::Control::Preset::FILL_FROM_CENTER);
                    searchBtn->SetCaption("搜索");
                }
            }
            midBox = new ui::HorizontalBox;{
                midBox->AddTo(vbox);
                midBox->SetHPreset(ui::Control::Preset::FILL_FROM_CENTER);
                midBox->SetVSize(320);
                midBox->SetGap(5);
            }
            {
                ui::Flat *midBoxFlat = new ui::Flat;{
                    midBoxFlat->AddTo(midBox);
                    midBoxFlat->SetHPreset(ui::Control::Preset::PLACE_AT_FRONT);
                    midBoxFlat->SetVPreset(ui::Control::Preset::FILL_FROM_CENTER);
                    midBoxFlat->SetHSize(searchBox->GetSize(ui::Control::Direction::HORIZONTAL));
                }
                {
                    hinderBox = new ui::HorizontalBox;{
                        hinderBox->AddTo(midBoxFlat);
                        hinderBox->SetHPreset(ui::Control::Preset::FILL_FROM_CENTER);
                        hinderBox->SetVPreset(ui::Control::Preset::WRAP_AT_FRONT);
                        hinderBox->SetVSize(100);
                    }
                }
                optBox = new ui::VerticalBox;{
                    optBox->AddTo(midBox);
                    optBox->SetGap(5);
                    optBox->HideAll();
                    optBox->SetHAnchor(60);
                    optBox->SetSize(120, 320);
                }
                {
                    chooseIsbn = new ui::Button;{
                        chooseIsbn->AddTo(optBox);
                        chooseIsbn->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
                        chooseIsbn->SetCaption("ISBN");
                        chooseIsbn->SetFontSize(30);
                        chooseIsbn->SetVisible(false);
                    }
                    chooseBookName = new ui::Button;{
                        chooseBookName->AddTo(optBox);
                        chooseBookName->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
                        chooseBookName->SetCaption("图书名称");
                        chooseBookName->SetFontSize(30);
                        chooseBookName->SetVisible(false);
                    }
                    chooseAuthor = new ui::Button;{
                        chooseAuthor->AddTo(optBox);
                        chooseAuthor->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
                        chooseAuthor->SetCaption("作者");
                        chooseAuthor->SetFontSize(30);
                        chooseAuthor->SetVisible(false);
                    }
                    choosePublishDate = new ui::Button;{
                        choosePublishDate->AddTo(optBox);
                        choosePublishDate->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);    
                        choosePublishDate->SetCaption("出版日期");
                        choosePublishDate->SetFontSize(30);
                        choosePublishDate->SetVisible(false);
                    }
                    chooseStorePosion = new ui::Button;{
                        chooseStorePosion->AddTo(optBox);
                        chooseStorePosion->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
                        chooseStorePosion->SetCaption("馆藏位置");
                        chooseStorePosion->SetFontSize(30);
                        chooseStorePosion->SetVisible(false);
                    }
                    chooseCategory = new ui::Button;{
                        chooseCategory->AddTo(optBox);
                        chooseCategory->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
                        chooseCategory->SetCaption("分类");
                        chooseCategory->SetFontSize(30);
                        chooseCategory->SetVisible(false);
                    }
                }
            }
            ui::HorizontalBox *bottomBox = new ui::HorizontalBox;{
                bottomBox->AddTo(vbox);
                bottomBox->SetHPreset(ui::Control::Preset::FILL_FROM_CENTER);
                bottomBox->SetVPreset(ui::Control::Preset::FILL_FROM_END);
                bottomBox->SetGap(5);
            }
            {
                ui::VerticalBox *leftSideBox = new ui::VerticalBox;{
                    leftSideBox->AddTo(bottomBox);
                    leftSideBox->SetHPreset(ui::Control::Preset::WRAP_AT_FRONT);
                    leftSideBox->SetVPreset(ui::Control::Preset::WRAP_AT_FRONT);
                    leftSideBox->SetSize(120, 100);
                }
                {
                    borrowManageBtn = new ui::Button;{
                        borrowManageBtn->AddTo(leftSideBox);
                        borrowManageBtn->SetHPreset(ui::Control::Preset::FILL_FROM_CENTER);
                        borrowManageBtn->SetVPreset(ui::Control::Preset::WRAP_AT_FRONT);
                        borrowManageBtn->SetCaption("借阅管理");
                    }
                    basicNolifyBtn = new ui::Button;{
                        basicNolifyBtn->AddTo(leftSideBox);
                        basicNolifyBtn->SetHPreset(ui::Control::Preset::FILL_FROM_CENTER);
                        basicNolifyBtn->SetVPreset(ui::Control::Preset::WRAP_AT_FRONT);
                        basicNolifyBtn->SetCaption("图书馆概况");
                    }
                }
                ui::Flat *flat = new ui::Flat;{
                    flat->AddTo(bottomBox);
                    flat->SetHPreset(ui::Control::Preset::FILL_FROM_CENTER);
                    flat->SetVPreset(ui::Control::Preset::FILL_FROM_CENTER);
                }
                {
                    bookManageBtn = new ui::Button;{
                        bookManageBtn->AddTo(flat);
                        bookManageBtn->SetHPreset(ui::Control::Preset::WRAP_AT_END);
                        bookManageBtn->SetVPosition(50);
                        bookManageBtn->SetCaption("图书管理");
                        bookManageBtn->SetVisible(false);
                    }
                }
            }
        }
    };
}

void vio::EnterLibrary::Logic(ui::Screen *screen) noexcept
{   
    searchBox->SetInputCallback(UI_CALLBACK{
        userInput = searchBox->GetText();
    });

    searchBox->SetEndCallback(UI_CALLBACK{
        if (searchBox->GetText() == "") searchBox->SetText("在此输入搜索内容");
    });

    searchBox->SetBeginCallback(UI_CALLBACK{
        if (searchBox->GetText() == "在此输入搜索内容") searchBox->SetText("");
    });

    returnBtn->SetClickCallback(UI_CALLBACK{
        SwitchTo(new eea::MainPage);
    });

    bookManageBtn->SetClickCallback(UI_CALLBACK{
        SwitchTo(new BookManage);
    });

    searchOptBtn->SetClickCallback(UI_CALLBACK{
        if (!isClicked) {
            isClicked ^= 1;
            searchOptBtn->SetCaption(optStr + "﹀");
            optBox->SetVisible(true);
            optBox->ShowAll();
            for (auto btn : {chooseIsbn, chooseBookName, chooseAuthor, choosePublishDate, chooseStorePosion, chooseCategory}) {
                btn->SetClickCallback(UI_CALLBACK{
                    optStr = btn->GetCaption();
                    searchOptBtn->SetCaption(optStr + "﹀");
                });
            }
        }
        else {
            isClicked ^= 1;
            optBox->HideAll();
            if (optStr == "") searchOptBtn->SetCaption("筛选>");
        }
    });

    searchBtn->SetClickCallback(UI_CALLBACK{
        searchBtn->Disable();
        if (userInput == "") {
            hinderBox->FreeAll();
            hinderBox->Add(new ui::Label("注意，查询内容不能为空，请检查输入"));
        }
        else {
            hinderBox->FreeAll();
            ui::LoadingRing *loading = new ui::LoadingRing;{
                loading->AddTo(hinderBox);
                loading->SetHPreset(ui::Control::Preset::FILL_FROM_CENTER);
                loading->SetSize(30, 30);
                loading->Start();
            }

            int searchType;
            if (optStr == "ISBN") searchType = trm::rqs::BOOK_ISBN;
            else if (optStr == "图书名称") searchType = trm::rqs::BOOK_NAME;
            else if (optStr == "作者") searchType = trm::rqs::BOOK_AUTHOR;
            else if (optStr == "出版日期") searchType = trm::rqs::BOOK_PUBLISHDATE;
            else if (optStr == "馆藏位置") searchType = trm::rqs::BOOK_STROEPOSTION;
            else if (optStr == "分类") searchType = trm::rqs::BOOK_CATAGORY;
            else searchType = trm::rqs::BOOK_NAME;

            Listen(new trm::Sender({trm::rqs::SEARCH_BOOK, userInput, ToStr(searchType), "false", "false"}), SD_CALLBACK{
                hinderBox->FreeAll();
                if (!reply.empty()) {
                    if (reply[0] == trm::rpl::FAIL) {
                        hinderBox->Add(new ui::Label("查询失败，请检查输入"));
                        return;
                    }
                    else if (reply[0] == trm::rpl::TIME_OUT) {
                        hinderBox->Add(new ui::Label("服务端未响应，请稍后再试"));
                        return;
                    }
                    else {
                        books.clear();
                        for (auto bookInfo : reply) {
                            books.push_back(trm::Book(bookInfo));
                        }
                    }
                }
                SwitchTo(new BookList);
            });
        }
        searchBtn->Enable();
    });
}

void vio::EnterLibrary::Ready(ui::Screen *screen) noexcept
{
    books = {};
    borrowLogs = {};
    selectedBook = trm::Book();
    if (account["name"] == "最高管理员") {
        bookManageBtn->SetVisible(true);
    }
}

void vio::BookList::Load(ui::Screen *screen) noexcept
{
    ui::Margin *mar = new ui::Margin;{
        mar->AddTo(screen);
        mar->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
        mar->SetMargin(100, 100, 150, 150);
    }
    {
        backBtn = new ui::Button;{
            backBtn->AddTo(mar);
            backBtn->SetHPreset(ui::Control::Preset::FILL_FROM_CENTER);
            backBtn->SetVPreset(ui::Control::Preset::WRAP_AT_FRONT);
            backBtn->SetFontSize(50);
            backBtn->SetCaption("←返回");
        }
    }
}

void vio::BookList::Logic(ui::Screen *screen) noexcept
{
    backBtn->SetClickCallback(UI_CALLBACK{
        SwitchTo(new EnterLibrary);
    });
}

void vio::BookList::Ready(ui::Screen *screen) noexcept
{
    selectedBook = trm::Book();
}

void vio::BookManage::Load(ui::Screen *screen) noexcept
{
    ui::Margin *mar = new ui::Margin;{
        mar->AddTo(screen);
        mar->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
        mar->SetMargin(100, 100, 150, 150);
    }
    {
        ui::HorizontalBox *wholeBox = new ui::HorizontalBox;{
            wholeBox->AddTo(mar);
            wholeBox->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
        }
        {
            ui::VerticalBox *leftMainBox = new ui::VerticalBox;{
                leftMainBox->AddTo(wholeBox);
                leftMainBox->SetHSizeValueType(ui::Control::ValueType::PERCENTAGE);
                leftMainBox->SetHSize(50);
                leftMainBox->SetVPreset(ui::Control::Preset::FILL_FROM_CENTER);
            }
            {
                backBtn = new ui::Button;{
                    backBtn->AddTo(leftMainBox);
                    backBtn->SetHPreset(ui::Control::Preset::WRAP_AT_FRONT);
                    backBtn->SetVPreset(ui::Control::Preset::WRAP_AT_FRONT);
                    backBtn->SetFontSize(40);
                    backBtn->SetCaption("←返回");
                }
                ui::HorizontalBox *lfTopBox = new ui::HorizontalBox;{
                    lfTopBox->AddTo(leftMainBox);
                    lfTopBox->SetHPreset(ui::Control::Preset::FILL_FROM_CENTER);
                    lfTopBox->SetVSize(40);
                }
                {
                    restoreBookBtn = new ui::Button;{
                        restoreBookBtn->AddTo(lfTopBox);
                        restoreBookBtn->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
                        restoreBookBtn->SetCaption("新增图书");
                        restoreBookBtn->SetFontSize(40);
                    }
                    modifyBookBtn = new ui::Button;{
                        modifyBookBtn->AddTo(lfTopBox);
                        modifyBookBtn->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
                        modifyBookBtn->SetCaption("修改图书");
                        modifyBookBtn->SetFontSize(40);
                    }
                    removeBookBtn = new ui::Button;{
                        removeBookBtn->AddTo(lfTopBox);
                        removeBookBtn->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
                        removeBookBtn->SetCaption("删除图书");
                        removeBookBtn->SetFontSize(40);
                    }
                }
                lfMidBox = new ui::VerticalBox;{
                    lfMidBox->AddTo(leftMainBox);
                    lfMidBox->SetHPreset(ui::Control::Preset::FILL_FROM_CENTER);
                    lfMidBox->SetVPreset(ui::Control::Preset::FILL_FROM_CENTER);
                    lfMidBox->SetGap(10);
                }
                {   
                    ui::Center *center = new ui::Center;{
                        center->AddTo(lfMidBox);
                        center->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
                    }
                    {
                        ui::Label *lfmdTip = new ui::Label("请选择你的操作");{
                            lfmdTip->AddTo(center);
                            lfmdTip->SetPreset(ui::Control::Preset::WRAP_AT_CENTER);
                            lfmdTip->SetHPreset(ui::Control::Preset::WRAP_AT_CENTER);
                        }
                    }
                }
            }
            ui::VerticalBox *rightMainBox = new ui::VerticalBox;{
                rightMainBox->AddTo(wholeBox);
                rightMainBox->SetHPreset(ui::Control::Preset::FILL_FROM_CENTER);
                rightMainBox->SetVPreset(ui::Control::Preset::FILL_FROM_CENTER);
            }
            {
                detailBox = new ui::VerticalScrollingBox;{
                    detailBox->AddTo(rightMainBox);
                    detailBox->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
                    detailBox->SetInsideBoxScrollable(true);
                }
                {   
                    ui::Center *center = new ui::Center;{
                        center->AddTo(detailBox);
                        center->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
                    }
                    {
                        ui::Label *rtmdTip = new ui::Label("请选择你的操作");{
                            rtmdTip->AddTo(center);
                            rtmdTip->SetPreset(ui::Control::Preset::WRAP_AT_CENTER);
                        }
                    }
                }
                ui::HorizontalBox *rtfootBox = new ui::HorizontalBox;{
                    rtfootBox->AddTo(rightMainBox);
                    rtfootBox->SetHPreset(ui::Control::Preset::FILL_FROM_CENTER);
                    rtfootBox->SetVSize(80);
                }
                {   
                    rtFootFlat = new ui::Flat;{
                        rtFootFlat->AddTo(rtfootBox);
                        rtFootFlat->SetHPreset(ui::Control::Preset::FILL_FROM_CENTER);
                        rtFootFlat->SetVPreset(ui::Control::Preset::FILL_FROM_CENTER);
                    }
                    confirmBtn = new ui::Button;{
                        confirmBtn->AddTo(rtfootBox);
                        confirmBtn->SetHPreset(ui::Control::Preset::PLACE_AT_END);
                        confirmBtn->SetSize(30, 30);
                        confirmBtn->SetCaption("确认");
                    }
                }
            }
        }
    }
}

void vio::BookManage::Logic(ui::Screen *screen) noexcept
{
    auto removeprework = [=, this]() -> void {
        ui::VerticalBox *vbox = new ui::VerticalBox;{
            vbox->AddTo(wholeBox);
            vbox->SetHPreset(ui::Control::Preset::FILL_FROM_CENTER);
            vbox->SetVPreset(ui::Control::Preset::WRAP_AT_CENTER);
        }
        {
            ui::HorizontalBox *hbox = new ui::HorizontalBox;{
                hbox->AddTo(vbox);
                hbox->SetHPreset(ui::Control::Preset::FILL_FROM_CENTER);
                hbox->SetVPreset(ui::Control::Preset::WRAP_AT_CENTER);
            }
            {
                ui::Label *storenum = new ui::Label;{
                    storenum->AddTo(hbox);
                    storenum->SetContent("删除数量");
                    storenum->SetHSize(100);
                }
                ui::InputBox *numInput = new ui::InputBox;{
                    numInput->AddTo(hbox);
                    numInput->SetHSize(100);
                    if (selectedBook.bookTot != 0) numInput->SetText(std::to_string(selectedBook.bookTot));
                    numInput->SetInputCallback(UI_CALLBACK{
                        bookNum = numInput->GetText();
                    });
                }
                ui::Button *allBtn = new ui::Button;{
                    allBtn->AddTo(hbox);
                    allBtn->SetCaption("all");
                    allBtn->SetHSize(80);
                    allBtn->SetClickCallback(UI_CALLBACK{
                        bookNum = "all";
                        numInput->SetText(bookNum);
                    });
                }
            }
            numLimitTip = new ui::Label;{
                numLimitTip->AddTo(vbox);
                numLimitTip->SetContent("输入需为大于0的整数, 或者all");
                numLimitTip->SetFontSize(35);
                numLimitTip->SetFontColor(sf::Color::Red);
                numLimitTip->SetHPreset(ui::Control::Preset::FILL_FROM_CENTER);
                numLimitTip->SetVPreset(ui::Control::Preset::WRAP_AT_CENTER);
                numLimitTip->SetVisible(false);
            }
        }
    };
#pragma region right page logic
   auto drawCatergoryBox = [=, this](std::vector<std::string> &catergories) -> void {
        int slen = 0;
        int cnt = 0;
        catergoryLabel = new ui::Label("请选择分类");{
            catergoryLabel->AddTo(catergoryBox);
            catergoryLabel->SetHPreset(ui::Control::Preset::WRAP_AT_CENTER);
            catergoryLabel->SetVPreset(ui::Control::Preset::WRAP_AT_FRONT);
        }
        ui::HorizontalBox *hbox = nullptr;
        ui::Button *catergoryBtn = nullptr;
        for (auto catergory : catergories) {
            slen += catergory.length();
            if ((slen >= 30 || catergory.length() >= 15) && cnt) {
                slen = 0;
                hbox = new ui::HorizontalBox;{
                    hbox->AddTo(catergoryBox);
                    hbox->SetHPreset(ui::Control::Preset::FILL_FROM_CENTER);
                    hbox->SetVSize(40);
                }
                cnt = 0;
            }
            if (hbox == nullptr) {
                hbox = new ui::HorizontalBox;{
                    hbox->AddTo(catergoryBox);
                    hbox->SetHPreset(ui::Control::Preset::FILL_FROM_CENTER);
                    hbox->SetVSize(40);
                }
            }
            catergoryBtn = new ui::Button;{
                catergoryBtn->AddTo(hbox);
                catergoryBtn->SetFontSize(40);
                catergoryBtn->SetHPreset(ui::Control::Preset::FILL_FROM_CENTER);
                catergoryBtn->SetVPreset(ui::Control::Preset::FILL_FROM_CENTER);
                catergoryBtn->SetCaption(catergory);
                catergoryBtn->SetClickCallback(UI_CALLBACK{
                    selectedBook.bookCatagory = catergoryBtn->GetCaption();
                    catergoryLabel->SetContent(selectedBook.bookCatagory);
                });
            }
            cnt++;
        }
    };

    reSetCatergoryBox = [=, this]() -> void {
        if (!isChanged) return;
        isChanged ^= 1;
        catergoryBox->FreeAll();
        if (selectedBook.storePosition == "302") {
            drawCatergoryBox(room302);
        } else if (selectedBook.storePosition == "303") {
            drawCatergoryBox(room303);
        } else if (selectedBook.storePosition == "401") {
            drawCatergoryBox(room401);
        } else if (selectedBook.storePosition == "402") {
            drawCatergoryBox(room402);
        } else if (selectedBook.storePosition == "403") {
            drawCatergoryBox(room403);
        } else if (selectedBook.storePosition == "621") {
            drawCatergoryBox(room621);
        }
    };

    auto setStorePositon = [=, this]() -> void {
        for (auto btn : {room302Btn, room303Btn, room401Btn, room402Btn, room403Btn, room621Btn}) {
            btn->SetClickCallback(UI_CALLBACK{
                selectedBook.storePosition = btn->GetCaption();
                storePosition->SetContent("图书位置" + selectedBook.storePosition);
                isChanged ^= 1;
                reSetCatergoryBox();
            });
        }
    };

    auto printBookDetail = [=, this]() -> void {
        ui::VerticalBox *rtMidBox = new ui::VerticalBox;{
            rtMidBox->AddTo(wholeBox);
            rtMidBox->SetHPreset(ui::Control::Preset::FILL_FROM_CENTER);
            rtMidBox->SetVPreset(ui::Control::Preset::WRAP_AT_CENTER);
        }
        {   
            ui::HorizontalBox *hbox = new ui::HorizontalBox;{
                hbox->AddTo(rtMidBox);
                hbox->SetHPreset(ui::Control::Preset::FILL_FROM_CENTER);
            }
            {
                ui::Label *author = new ui::Label("作者");{
                    author->AddTo(hbox);
                    author->SetVPreset(ui::Control::Preset::WRAP_AT_CENTER);
                    author->SetHSize(100);
                }
                ui::InputBox *authorInput = new ui::InputBox;{
                    authorInput->AddTo(hbox);
                    authorInput->SetVPreset(ui::Control::Preset::WRAP_AT_CENTER);
                    authorInput->SetHPreset(ui::Control::Preset::FILL_FROM_CENTER);
                    if (!selectedBook.bookAuthor.empty()) {
                        std::string text = "";
                        int size = selectedBook.bookAuthor.size();
                        for (int i = 0; i < size; i++) {
                            if (i == size - 1) {
                                text += selectedBook.bookAuthor[i];
                            }
                            else text += selectedBook.bookAuthor[i] + ",";
                        }
                        authorInput->SetText(text);
                        bookAuthor = text;
                    }
                }
                authorInput->SetInputCallback(UI_CALLBACK{
                    bookAuthor= authorInput->GetText();
                });
            }
            ui::Label *tip = new ui::Label;{
                tip->AddTo(rtMidBox);
                tip->SetContent("多个作者请用\",\"隔开");
                tip->SetFontSize(35);
                tip->SetFontColor(sf::Color::Red);
                tip->SetVPreset(ui::Control::Preset::WRAP_AT_CENTER);
                tip->SetHPreset(ui::Control::Preset::FILL_FROM_CENTER);
            }
        }
        rtMidBox = new ui::VerticalBox;{
            rtMidBox->AddTo(wholeBox);
            rtMidBox->SetHPreset(ui::Control::Preset::FILL_FROM_CENTER);
            rtMidBox->SetVPreset(ui::Control::Preset::WRAP_AT_CENTER);
        }
        {
            ui::HorizontalBox *hbox = new ui::HorizontalBox;{
                hbox->AddTo(rtMidBox);
                hbox->SetHPreset(ui::Control::Preset::FILL_FROM_CENTER);
            }
            {
                ui::Label *publishDate = new ui::Label("出版日期");{
                    publishDate->AddTo(hbox);
                    publishDate->SetVPreset(ui::Control::Preset::WRAP_AT_CENTER);
                    publishDate->SetHSize(100);
                }
                ui::InputBox *publishDateInput = new ui::InputBox;{
                    publishDateInput->AddTo(hbox);
                    publishDateInput->SetVPreset(ui::Control::Preset::WRAP_AT_CENTER);
                    publishDateInput->SetHPreset(ui::Control::Preset::FILL_FROM_CENTER);
                    if (selectedBook.bookPublicationDate != "") publishDateInput->SetText(selectedBook.bookPublicationDate);    
                }
                publishDateInput->SetInputCallback(UI_CALLBACK{
                    selectedBook.bookPublicationDate = publishDateInput->GetText();
                });
            }
            bookpublishDateLimitTip = new ui::Label;{
                bookpublishDateLimitTip->AddTo(rtMidBox);
                bookpublishDateLimitTip->SetVPreset(ui::Control::Preset::WRAP_AT_CENTER);
                bookpublishDateLimitTip->SetHSize(300);
                bookpublishDateLimitTip->SetFontSize(35);
                bookpublishDateLimitTip->SetFontColor(sf::Color::Red);
                bookpublishDateLimitTip->SetContent("出版日期的输入形式需为yyyy-mm-dd");
                bookpublishDateLimitTip->SetVisible(false);
            }
        }
        ui::HorizontalBox *hbox1 = new ui::HorizontalBox;{
            hbox1->AddTo(wholeBox);
            hbox1->SetHPreset(ui::Control::Preset::FILL_FROM_CENTER);
            hbox1->SetVPreset(ui::Control::Preset::WRAP_AT_CENTER);
        }
        {
            storePosition = new ui::Label;{
                storePosition->AddTo(hbox1);
                storePosition->SetVPreset(ui::Control::Preset::WRAP_AT_CENTER);
                storePosition->SetHPreset(ui::Control::Preset::FILL_FROM_CENTER);
                storePosition->SetContent("图书位置");
            }
            room302Btn = new ui::Button;{
                room302Btn->AddTo(hbox1);
                room302Btn->SetVPreset(ui::Control::Preset::FILL_FROM_CENTER);
                room302Btn->SetHPreset(ui::Control::Preset::FILL_FROM_CENTER);
                room302Btn->SetCaption("302");
            }
            room303Btn = new ui::Button;{
                room303Btn->AddTo(hbox1);
                room303Btn->SetVPreset(ui::Control::Preset::FILL_FROM_CENTER);
                room303Btn->SetHPreset(ui::Control::Preset::FILL_FROM_CENTER);
                room303Btn->SetCaption("303");
            }
            room401Btn = new ui::Button;{
                room401Btn->AddTo(hbox1);
                room401Btn->SetVPreset(ui::Control::Preset::FILL_FROM_CENTER);
                room401Btn->SetHPreset(ui::Control::Preset::FILL_FROM_CENTER);
                room401Btn->SetCaption("401");
            }
            room402Btn = new ui::Button;{
                room402Btn->AddTo(hbox1);
                room402Btn->SetVPreset(ui::Control::Preset::FILL_FROM_CENTER);
                room402Btn->SetHPreset(ui::Control::Preset::FILL_FROM_CENTER);
                room402Btn->SetCaption("402");
            }
            room403Btn = new ui::Button;{
                room403Btn->AddTo(hbox1);
                room403Btn->SetVPreset(ui::Control::Preset::FILL_FROM_CENTER);
                room403Btn->SetHPreset(ui::Control::Preset::FILL_FROM_CENTER);
                room403Btn->SetCaption("403");
            }
            room621Btn = new ui::Button;{
                room621Btn->AddTo(hbox1);
                room621Btn->SetVPreset(ui::Control::Preset::FILL_FROM_CENTER);
                room621Btn->SetHPreset(ui::Control::Preset::FILL_FROM_CENTER);
                room621Btn->SetCaption("621");
            }
        }
        catergoryBox = new ui::VerticalBox;{
            catergoryBox->AddTo(wholeBox);
            catergoryBox->SetHPreset(ui::Control::Preset::FILL_FROM_CENTER);
            catergoryBox->SetVPreset(ui::Control::Preset::WRAP_AT_CENTER);
        }
        {
            ui::Label *catergory = new ui::Label;{
                catergory->AddTo(catergoryBox);
                catergory->SetHPreset(ui::Control::Preset::WRAP_AT_CENTER);
                catergory->SetVPreset(ui::Control::Preset::WRAP_AT_FRONT);
                catergory->SetContent("图书类别:  请选择图书位置");
            }
        }
    };

    auto printBasicDetail = [=, this]() -> void {
        ui::Margin *mar = new ui::Margin;{
            mar->AddTo(detailBox);
            mar->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
            mar->SetMargin(50, 50, 50, 50);
        }
        {
            wholeBox = new ui::VerticalBox;{
                wholeBox->AddTo(mar);
                wholeBox->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
                wholeBox->SetGap(80);
            }
            {
                ui::VerticalBox *rtMidBox = new ui::VerticalBox;{
                    rtMidBox->AddTo(wholeBox);
                    rtMidBox->SetHPreset(ui::Control::Preset::FILL_FROM_CENTER);
                    rtMidBox->SetVPreset(ui::Control::Preset::WRAP_AT_CENTER);
                }
                {
                    ui::HorizontalBox *hbox = new ui::HorizontalBox;{
                        hbox->AddTo(rtMidBox);
                        hbox->SetHPreset(ui::Control::Preset::FILL_FROM_CENTER);
                    }
                    {
                        ui::Label *isbn = new ui::Label("ISBN");{
                            isbn->AddTo(hbox);
                            isbn->SetVPreset(ui::Control::Preset::WRAP_AT_CENTER);
                            isbn->SetHSize(100);
                        }
                        if (opt != "删除图书") {
                            ui::InputBox *isbnInput = new ui::InputBox;{
                                isbnInput->AddTo(hbox);
                                isbnInput->SetVPreset(ui::Control::Preset::WRAP_AT_CENTER);
                                isbnInput->SetHPreset(ui::Control::Preset::FILL_FROM_CENTER);
                                if (selectedBook.bookIsbn != "") isbnInput->SetText(selectedBook.bookIsbn);
                            }
                            isbnInput->SetInputCallback(UI_CALLBACK{
                                selectedBook.bookIsbn = isbnInput->GetText();
                            });
                            bookisbnLimitTip = new ui::Label;{
                                bookisbnLimitTip->AddTo(rtMidBox);
                                bookisbnLimitTip->SetHPreset(ui::Control::Preset::WRAP_AT_CENTER);
                                bookisbnLimitTip->SetVPreset(ui::Control::Preset::WRAP_AT_CENTER);
                                bookisbnLimitTip->SetFontSize(35);
                                bookisbnLimitTip->SetFontColor(sf::Color::Red);
                                bookisbnLimitTip->SetContent("ISBN的输入形式需为xxx-x-x-xxxxxx-x, 其中x为数字");
                                bookisbnLimitTip->SetVisible(false);
                            }

                        }
                        else {
                            ui::Label *isbnLabel = new ui::Label;{
                                isbnLabel->AddTo(hbox);
                                isbnLabel->SetVPreset(ui::Control::Preset::WRAP_AT_CENTER);
                                isbnLabel->SetHSize(200);
                                isbnLabel->SetContent(selectedBook.bookIsbn);
                            }
                        }
                    }
                }
                ui::HorizontalBox *hbox1 = new ui::HorizontalBox;{
                    hbox1->AddTo(wholeBox);
                    hbox1->SetHPreset(ui::Control::Preset::FILL_FROM_CENTER);
                    hbox1->SetVPreset(ui::Control::Preset::WRAP_AT_CENTER);
                }
                {
                    ui::Label *name = new ui::Label("图书名称");{
                        name->AddTo(hbox1);
                        name->SetVPreset(ui::Control::Preset::WRAP_AT_CENTER);
                        name->SetHSize(100);
                    }
                    if (opt != "删除图书") {
                        ui::InputBox *nameInput = new ui::InputBox;{
                            nameInput->AddTo(hbox1);
                            nameInput->SetVPreset(ui::Control::Preset::WRAP_AT_CENTER);
                            nameInput->SetHPreset(ui::Control::Preset::FILL_FROM_CENTER);
                            if (selectedBook.bookName != "") nameInput->SetText(selectedBook.bookName);
                        }
                        nameInput->SetInputCallback(UI_CALLBACK{
                            selectedBook.bookName = nameInput->GetText();
                        });
                    }
                    else {
                        ui::Label *nameLabel = new ui::Label;{
                            nameLabel->AddTo(hbox1);
                            nameLabel->SetVPreset(ui::Control::Preset::WRAP_AT_CENTER);
                            nameLabel->SetHSize(200);
                            nameLabel->SetContent(selectedBook.bookName);
                        }
                    }
                
                }
                               
                // ui::Label *end = new ui::Label;{
                //     end->AddTo(wholeBox);
                //     end->SetHPreset(ui::Control::Preset::WRAP_AT_CENTER);
                //     end->SetVPreset(ui::Control::Preset::WRAP_AT_END);
                //     end->SetContent("请确认信息无误后，点击确认按钮\n==================END==================");
                //     end->SetFontSize(35);
                // }
            }
        }
    };

#pragma region left page logic
    auto optEnable = [=, this]() -> void {
        if (opt == "新增图书") {
            restoreBookBtn->Disable();
            modifyBookBtn->Enable();
            removeBookBtn->Enable();
        } else if (opt == "修改图书") {
            restoreBookBtn->Enable();
            modifyBookBtn->Disable();
            removeBookBtn->Enable();
        } else if (opt == "删除图书") {
            restoreBookBtn->Enable();
            modifyBookBtn->Enable();
            removeBookBtn->Disable();
        }
    };

    auto resetLeftDetail = [=, this]() -> void {
        bookList->FreeAllVisible();
        ui::VerticalBox *vbox = new ui::VerticalBox;{
            vbox->AddTo(bookList);
            vbox->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
            vbox->SetGap(15);
        }
        {   
            ui::Button *btn;
            for (int i = (page - 1) * 10; i < len && i < page * 10; i++) {
                btn = new ui::Button;{
                    btn->AddTo(vbox);
                    btn->SetFontSize(35);
                    btn->SetHPreset(ui::Control::Preset::FILL_FROM_CENTER);
                    btn->SetVPreset(ui::Control::Preset::WRAP_AT_FRONT);
                    btn->SetCaption(books[i].bookName + "\n" + books[i].bookIsbn + "  存量" + ToStr(books[i].bookTot));
                    btn->SetCaption(books[i].bookName + "\n" + books[i].bookIsbn + "  存量" + ToStr(books[i].bookTot));
                }
                btn->SetClickCallback(UI_CALLBACK{
                    selectedBook = books[i];
                    bookAuthor = "";
                    int len = selectedBook.bookAuthor.size();
                    for (int i = 0; i < len; i++) {
                        if (i == len - 1) bookAuthor += selectedBook.bookAuthor[i];
                        else bookAuthor += selectedBook.bookAuthor[i] + ",";
                    }
                    bookNum = ToStr(selectedBook.bookTot);
                    detailBox->FreeAll();
                    printBasicDetail();
                    if (opt == "删除图书") removeprework();
                    else {
                        printBookDetail();
                        setStorePositon();
                    }
                });
            }
        }
        if (len == 0) {
            ui::Label *label = new ui::Label;{
                label->AddTo(vbox);
                label->SetContent("暂无图书");
                label->SetPreset(ui::Control::Preset::WRAP_AT_CENTER);
            }
        }
    };

    auto printLeftDetail = [=, this]() -> void {
        lfMidBox->FreeAll();
        ui::Label *label = new ui::Label;{
            label->AddTo(lfMidBox);
            label->SetContent(opt);
            label->SetFontSize(40);
            label->SetHPreset(ui::Control::Preset::WRAP_AT_CENTER);
            label->SetVPreset(ui::Control::Preset::WRAP_AT_FRONT);
        }
        ui::HorizontalBox *hbox = new ui::HorizontalBox;{
            hbox->AddTo(lfMidBox);
            hbox->SetHPreset(ui::Control::Preset::FILL_FROM_CENTER);
            hbox->SetVPreset(ui::Control::Preset::WRAP_AT_CENTER);
            hbox->SetGap(5);
        }
        {
            ui::InputBox *searchBox = new ui::InputBox;{
                searchBox->AddTo(hbox);
                searchBox->SetHSize(350);
                searchBox->SetVSize(30);
                searchBox->SetInputCallback(UI_CALLBACK{
                    searchInput = searchBox->GetText();
                });
            }
            searchOpt = new ui::Button;{
                searchOpt->AddTo(hbox);
                searchOpt->SetCaption("图书名称>");
                searchOpt->SetHPreset(ui::Control::Preset::FILL_FROM_CENTER);
                searchOpt->SetVPreset(ui::Control::Preset::FILL_FROM_CENTER);
                searchOpt->SetFontSize(30);
                searchOpt->SetClickCallback(UI_CALLBACK{
                    isClicked ^= 1;
                    if (isClicked) {
                        optBox->SetVisible(true);
                        optBox->ShowAll();
                    }
                    else {
                        optBox->SetVisible(false);
                        optBox->HideAll();
                    }
                });
            }
            searchBtn = new ui::Button;{
                searchBtn->AddTo(hbox);
                searchBtn->SetCaption("搜索");
                searchBtn->SetHSize(100);
                searchBtn->SetVPreset(ui::Control::Preset::FILL_FROM_CENTER);
                searchBtn->SetFontSize(30);
            }
            searchBtn->SetClickCallback(UI_CALLBACK{
                searchBtn->Disable();
                int searchType;
                if (searchOpt->GetCaption() == "ISBN") searchType = trm::rqs::BOOK_ISBN;
                else if (searchOpt->GetCaption() == "图书名称") searchType = trm::rqs::BOOK_NAME;
                bookList->FreeAllVisible();
                bookListLoading = new ui::LoadingRing;{
                    bookListLoading->AddTo(bookList);
                    bookListLoading->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
                    bookListLoading->SetSize(30, 30);
                    bookListLoading->Start();
                }
                Listen(new trm::Sender({trm::rqs::SEARCH_BOOK, searchInput, ToStr(searchType), "false", "false"}), SD_CALLBACK{
                    if (reply.empty()) {
                        books.clear();
                        for (auto bookInfo : reply) {
                            books.push_back(trm::Book(bookInfo));
                        }
                        len = books.size();
                        totPage = (len + 10 - 1) / 10;
                        page = 1;
                        resetLeftDetail();
                    }
                    else if (reply[0] == trm::rpl::FAIL) {
                        bookList->Add(new ui::Label("查询失败，请检查输入"));
                    }
                    else if (reply[0] == trm::rpl::TIME_OUT) {
                        bookList->Add(new ui::Label("服务端未响应，请稍后再试"));
                    }
                    else {
                        books.clear();
                        for (auto bookInfo : reply) {
                            books.push_back(trm::Book(bookInfo));
                        }
                        len = books.size();
                        totPage = (len + 10 - 1) / 10;
                        page = 1;
                        resetLeftDetail();
                    }
                });
                searchBtn->Enable();
            });
        }
        optBox = new ui::HorizontalBox;{
            optBox->AddTo(lfMidBox);
            optBox->SetHPosition(350);
            optBox->SetVPreset(ui::Control::Preset::WRAP_AT_FRONT);
            optBox->SetGap(5);
            optBox->SetVisible(false);
        }
        {
            ui::Button *btn = new ui::Button;{
                btn->AddTo(optBox);
                btn->SetCaption("isbn");
                btn->SetHSize(50);
                btn->SetVPreset(ui::Control::Preset::WRAP_AT_FRONT);
                btn->SetFontSize(30);
                btn->SetVisible(false);
                btn->SetClickCallback(UI_CALLBACK{
                    searchOpt->SetCaption("ISBN");
                });
            }
            btn = new ui::Button;{
                btn->AddTo(optBox);
                btn->SetCaption("图书名称");
                btn->SetHSize(50);
                btn->SetVPreset(ui::Control::Preset::WRAP_AT_FRONT);
                btn->SetFontSize(30);
                btn->SetVisible(false);
                btn->SetClickCallback(UI_CALLBACK{
                    searchOpt->SetCaption("图书名称");
                });
            }
        }
        bookList = new ui::VerticalScrollingBox;{
            bookList->AddTo(lfMidBox);
            bookList->SetPreset(ui::Control::Preset::FILL_FROM_CENTER);
            bookList->SetInsideBoxScrollable(true);
        }
        ui::HorizontalBox *pageBox = new ui::HorizontalBox;{
            pageBox->AddTo(lfMidBox);
            pageBox->SetHPreset(ui::Control::Preset::WRAP_AT_CENTER);
            pageBox->SetVSize(40);
        }
        {
            lastBtn = new ui::Button;{
                lastBtn->AddTo(pageBox);
                lastBtn->SetCaption("上一页");
                lastBtn->SetHSize(80);
                lastBtn->SetFontSize(35);
                lastBtn->SetHPreset(ui::Control::Preset::PLACE_AT_CENTER);
                lastBtn->SetVPreset(ui::Control::Preset::FILL_FROM_CENTER);
                lastBtn->SetClickCallback(UI_CALLBACK{
                    nextBtn->Enable();
                    lastBtn->Disable();
                    if (page > 1) {
                        page--;
                        resetLeftDetail();
                    }
                    if (page == 1) lastBtn->Disable();
                    if (page == totPage) nextBtn->Disable();
                });
            }
            pageLabel = new ui::Label;{
                pageLabel->AddTo(pageBox);
                pageLabel->SetHSize(40);
                pageLabel->SetFontSize(35);
                pageLabel->SetHPreset(ui::Control::Preset::PLACE_AT_CENTER);
                pageLabel->SetVPreset(ui::Control::Preset::FILL_FROM_CENTER);
                pageLabel->SetContent(ToStr(page) + "/" + ToStr(totPage));
            }
            nextBtn = new ui::Button;{
                nextBtn->AddTo(pageBox);
                nextBtn->SetCaption("下一页");
                nextBtn->SetHSize(80);
                nextBtn->SetFontSize(35);
                nextBtn->SetHPreset(ui::Control::Preset::PLACE_AT_CENTER);
                nextBtn->SetVPreset(ui::Control::Preset::FILL_FROM_CENTER);
                nextBtn->SetClickCallback(UI_CALLBACK{
                    lastBtn->Enable();
                    nextBtn->Disable();
                    if (page < totPage) {
                        page++;
                        resetLeftDetail();
                    }
                    if (page == totPage) nextBtn->Disable();
                    if (page == 1) nextBtn->Disable();
                });
            }
        }
        ui::HorizontalBox *turnToBox = new ui::HorizontalBox;{
            turnToBox->AddTo(lfMidBox);
            turnToBox->SetHPreset(ui::Control::Preset::WRAP_AT_CENTER);
            turnToBox->SetVSize(40);
        }
        {
            ui::Label *tip = new ui::Label("非法页码");{
                tip->AddTo(turnToBox);
                tip->SetFontSize(35);
                tip->SetHPreset(ui::Control::Preset::WRAP_AT_CENTER);
                tip->SetVPreset(ui::Control::Preset::WRAP_AT_CENTER);
                tip->SetVisible(false);
            }
            ui::InputBox *inputBox = new ui::InputBox;{
                inputBox->AddTo(turnToBox);
                inputBox->SetHSize(200);
                inputBox->SetVSize(30);
                inputBox->SetHPreset(ui::Control::Preset::PLACE_AT_CENTER);
            }
            ui::Button *btn = new ui::Button;{
                btn->AddTo(turnToBox);
                btn->SetCaption("跳转至");
                btn->SetFontSize(35);
                btn->SetHPreset(ui::Control::Preset::WRAP_AT_CENTER);
                btn->SetVPreset(ui::Control::Preset::FILL_FROM_CENTER);
                btn->SetClickCallback(UI_CALLBACK{
                    std::string input = inputBox->GetText();
                    tip->SetVisible(false);
                    for (auto c : input) {
                        if (c < '0' || c > '9') {
                            tip->SetVisible(true);
                            return;
                        }
                    }
                    int inputPage = ToNum<int>(input);
                    if (inputPage == page) {
                        return;
                    }
                    else if (inputPage < 1) {
                        page = 1;
                    }
                    else if (inputPage > totPage) {
                        page = totPage;
                    }
                    else {
                        page = inputPage;
                    }
                    pageLabel->SetContent(ToStr(page) + "/" + ToStr(totPage));
                    resetLeftDetail();
                });
            }
        }
    };

#pragma region keybutton logic
    backBtn->SetClickCallback(UI_CALLBACK{
        SwitchTo(new EnterLibrary);
    });

    restoreBookBtn->SetClickCallback(UI_CALLBACK{
        opt = "新增图书";
        optEnable();
        selectedBook = trm::Book();
        detailBox->FreeAll();
        lfMidBox->FreeAll();
        ui::Label *label = new ui::Label;{
            label->AddTo(lfMidBox);
            label->SetFontSize(40);
            label->SetContent(opt);
            label->SetPreset(ui::Control::Preset::WRAP_AT_CENTER);
        }
        printBasicDetail();
        printBookDetail();
        ui::VerticalBox *vbox = new ui::VerticalBox;{
            vbox->AddTo(wholeBox);
            vbox->SetHPreset(ui::Control::Preset::FILL_FROM_CENTER);
            vbox->SetVPreset(ui::Control::Preset::WRAP_AT_CENTER);
        }
        {
            ui::HorizontalBox *hbox = new ui::HorizontalBox;{
                hbox->AddTo(vbox);
                hbox->SetHPreset(ui::Control::Preset::FILL_FROM_CENTER);
                hbox->SetVPreset(ui::Control::Preset::WRAP_AT_CENTER);
            }
            {
                ui::Label *storenum = new ui::Label;{
                    storenum->AddTo(hbox);
                    storenum->SetContent("存放数量");
                    storenum->SetHSize(100);
                }
                ui::InputBox *numInput = new ui::InputBox;{
                    numInput->AddTo(hbox);
                    numInput->SetHSize(100);
                    if (selectedBook.bookTot != 0) numInput->SetText(std::to_string(selectedBook.bookTot));
                    numInput->SetInputCallback(UI_CALLBACK{
                        bookNum = numInput->GetText();
                    });
                }
            }
            numLimitTip = new ui::Label;{
                numLimitTip->AddTo(vbox);
                numLimitTip->SetContent("输入需为大于0的整数");
                numLimitTip->SetFontSize(35);
                numLimitTip->SetFontColor(sf::Color::Red);
                numLimitTip->SetHPreset(ui::Control::Preset::FILL_FROM_CENTER);
                numLimitTip->SetVPreset(ui::Control::Preset::WRAP_AT_CENTER);
                numLimitTip->SetVisible(false);
            }
        }
        setStorePositon();
    });

    modifyBookBtn->SetClickCallback(UI_CALLBACK{
        opt = "修改图书";
        optEnable();
        selectedBook = trm::Book();
        detailBox->FreeAll();
        printLeftDetail();
        printBasicDetail();
        printBookDetail();
        setStorePositon();
    });

    removeBookBtn->SetClickCallback(UI_CALLBACK{
        opt = "删除图书";
        optEnable();
        selectedBook = trm::Book();
        detailBox->FreeAll();
        printLeftDetail();
        printBasicDetail();
        removeprework();
    });

    confirmBtn->SetClickCallback(UI_CALLBACK{
        std::vector<std::string> invalid = CheckInput();
        if (invalid.empty()) {
            screen->HideAll();
            std::string tip = "";
            tip = opt + " ISBN " + selectedBook.bookIsbn + 
                        "\n    书名 " + selectedBook.bookName + 
                        "\n    作者 ";
            std::string author = "";
            selectedBook.bookAuthor.clear();
            for (auto c : bookAuthor) {
                if (c == ',') {
                    tip += author + ", ";
                    selectedBook.bookAuthor.push_back(author);
                    author = "";
                } else {
                    author += c;
                }
            }
            tip += author;
            selectedBook.bookAuthor.push_back(author);
            tip += "\n    出版日期 " + selectedBook.bookPublicationDate + 
                    "\n    存放位置 " + selectedBook.storePosition + 
                    "\n    图书分类 " + selectedBook.bookCatagory;
                    
            if (opt == "新增图书" || opt == "删除图书") tip += "\n    数量 " + bookNum;
            int ret = MessageBox(screen, tip, {"取消", "确认信息无误"});
            if (ret == 0) {
                screen->FreeAllVisible();
                screen->ShowAll();
            }
            else {
                if (opt == "新增图书") {
                    screen->FreeAllVisible();
                    auto [success, reply] = WaitServer(screen, {trm::rqs::RESTORE_BOOK, account.code, account.hashedPassword, selectedBook.bookIsbn, bookNum, selectedBook}, "等待服务端响应");
                    if (success == 1) {
                        if (reply[0] == trm::rpl::SUCC) {
                            screen->FreeAllVisible();
                            int succret =MessageBox(screen, "图书添加成功", {"确认"});
                            if (succret == 0) {
                                SwitchTo(new BookManage);
                            }
                        } 
                        else if (reply[0] == trm::rpl::ACCESS_DENIED) {
                            int denyret = MessageBox(screen, "权限不足，请联系管理员", {"确认"});
                            if (denyret == 0) {
                                SwitchTo(new EnterLibrary);
                            }
                        }
                        else {
                            assert(false);
                        }
                    } 
                    else if (success == 0) {
                        int failret = MessageBox(screen, "服务端未响应，请稍后再试", {"确认"});
                        if (failret == 0) {
                            SwitchTo(new BookManage);
                        }
                    }
                }
                else if (opt == "修改图书") {
                    screen->FreeAllVisible();
                    auto [success, reply] = WaitServer(screen, {trm::rqs::MODIFY_BOOK_INFO, account.code, account.hashedPassword, selectedBook.bookIsbn, selectedBook}, "等待服务端响应");
                    if (success == 1) {
                        if (reply[0] == trm::rpl::SUCC) {
                            screen->FreeAllVisible();
                            int succret =MessageBox(screen, "图书修改成功", {"确认"});
                            if (succret == 0) {
                                SwitchTo(new BookManage);
                            }
                        } 
                        else if (reply[0] == trm::rpl::ACCESS_DENIED) {
                            int denyret = MessageBox(screen, "权限不足，请联系管理员", {"确认"});
                            if (denyret == 0) {
                                SwitchTo(new EnterLibrary);
                            }
                        }
                        else if (reply[0] == trm::rpl::NO_BOOK) {
                            int noret = MessageBox(screen, "该图书不存在", {"确认"});
                            if (noret == 0) {
                                SwitchTo(new BookManage);
                            }
                        }
                        else if (reply[0] == trm::rpl::EXIST_BOOK) {
                            int existret = MessageBox(screen, "修改后的图书与馆藏图书冲突", {"确认"});
                            if (existret == 0) {
                                SwitchTo(new BookManage);
                            }
                        }
                        else {
                            assert(false);
                        }
                    } 
                    else if (success == 0) {
                        int failret = MessageBox(screen, "服务端未响应，请稍后再试", {"确认"});
                        if (failret == 0) {
                            SwitchTo(new BookManage);
                        }
                    }
                }
                else if (opt == "删除图书") {
                    screen->FreeAllVisible();
                    auto [success, reply] = WaitServer(screen, {trm::rqs::REMOVE_BOOK, account.code, account.hashedPassword, selectedBook.bookIsbn, bookNum}, "等待服务端响应");
                    if (success == 1) {
                        if (reply[0] == trm::rpl::SUCC) {
                            screen->FreeAllVisible();
                            int succret =MessageBox(screen, "图书删除成功", {"确认"});
                            if (succret == 0) {
                                SwitchTo(new BookManage);
                            }
                        } 
                        else if (reply[0] == trm::rpl::ACCESS_DENIED) {
                            int denyret = MessageBox(screen, "权限不足，请联系管理员", {"确认"});
                            if (denyret == 0) {
                                SwitchTo(new EnterLibrary);
                            }
                        }
                        else if (reply[0] == trm::rpl::NO_BOOK) {
                            int noret = MessageBox(screen, "该图书不存在", {"确认"});
                            if (noret == 0) {
                                SwitchTo(new BookManage);
                            }
                        }
                        else if (reply[0] == trm::rpl::EXCEED_BOOK_NUM) {
                            int exceedret = MessageBox(screen, "图书数量不足", {"确认"});
                            if (exceedret == 0) {
                                SwitchTo(new BookManage);
                            }
                        }
                        else {
                            assert(false);
                        }
                    } 
                    else if (success == 0) {
                        int failret = MessageBox(screen, "服务端未响应，请稍后再试", {"确认"});
                        if (failret == 0) {
                            SwitchTo(new BookManage);
                        }
                    }
                }
            }
        }
        else {
            if (opt == "新增图书" || opt == "删除图书") numLimitTip->SetVisible(false);
            if (opt == "新增图书") {
                bookisbnLimitTip->SetVisible(false);
                bookpublishDateLimitTip->SetVisible(false);
            }
            for (auto s : invalid) {
                if (s == "isbn" && opt != "删除图书") {
                    bookisbnLimitTip->SetVisible(true);
                }
                else if (s == "date" && opt != "删除图书") {
                    bookpublishDateLimitTip->SetVisible(true);
                }
                else if (s == "num" && (opt == "新增图书" || opt == "删除图书")) {
                    numLimitTip->SetVisible(true);
                }
            }
        }
    });

}

void vio::BookManage::Ready(ui::Screen *screen) noexcept
{
    ;
}

std::vector<std::string> vio::BookManage::CheckInput() const noexcept
{   
    std::vector<std::string> invalid;
    if (selectedBook.bookIsbn.empty()) invalid.emplace_back("isbn");
    else {
        if (selectedBook.bookIsbn.length() != 16) invalid.emplace_back("isbn");
        else {
            for (int i = 0; i < 16; i++) {
                if (i == 3 || i == 5 || i == 7 || i == 14) {
                    if (selectedBook.bookIsbn[i] != '-') {
                        invalid.emplace_back("isbn");
                        break;
                    }
                }
                else if (selectedBook.bookIsbn[i] < '0' || selectedBook.bookIsbn[i] > '9') {
                    invalid.emplace_back("isbn");
                    break;
                }
            }
        }
    }
    if (selectedBook.bookPublicationDate.empty()) invalid.emplace_back("date");
    else {
        if (selectedBook.bookPublicationDate.length() != 10) invalid.emplace_back("date");
        else {
            for (int i = 0; i < 10; i++) {
                if (i == 4 || i == 7) {
                    if (selectedBook.bookPublicationDate[i] != '-') {
                        invalid.emplace_back("date");
                        break;
                    }
                }
                else if (selectedBook.bookPublicationDate[i] < '0' || selectedBook.bookPublicationDate[i] > '9') {
                    invalid.emplace_back("date");
                    break;
                }
            }
        }
    }
    if (opt == "删除图书" || opt == "新增图书") {
        if (bookNum == "" || bookNum == "0") invalid.emplace_back("num");
        else if (bookNum != "all") {
            for (auto c : bookNum) {
                if (c < '0' || c > '9') {
                    invalid.emplace_back("num");
                    break;
                }
            }
        }
        else if (opt != "删除图书") invalid.emplace_back("num");
    }

    return invalid;
}

void vio::EnterNolify::Load(ui::Screen *screen) noexcept
{
    ;
}

void vio::EnterNolify::Logic(ui::Screen *screen) noexcept
{
    ;
}

void vio::EnterNolify::Ready(ui::Screen *screen) noexcept
{
    ;
}
