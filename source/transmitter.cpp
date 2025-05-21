#include "transmitter.hpp"

std::string trm::Sender::link;
std::string trm::Sender::self;
std::string trm::Sender::selfAsSender;

int trm::GenerateID() noexcept
{
    static int count = 0;
    ++count;
    return count;
}

bool trm::MakeRequest(const std::string &link, const Request &request) noexcept
{
    if (!file::CheckDirectoryExists(link)) {
        return false;
    }
    std::string filePath = file::GetFilePath(link, Combine({ToStr(request.id), ToStr(GetTimeStamp()), ToStr(GenerateRandomCode())}, '.'));
    auto info = request.content;
    info.push_back(request.sender);
    return file::WriteFile(filePath, Encode(info));
}

std::pair<bool, std::vector<trm::Request>> trm::GetRequests(const std::string &self) noexcept
{
    if (!file::CheckDirectoryExists(self)) {
        return {false, {}};
    }
    std::vector<Request> result;
    auto [success, files] = file::ListDirectory(self);
    if (!success) {
        return {false, {}};
    }
    for (const auto &fileName : files) {
        Request request;
        auto id_time_code = Split(fileName, '.');
        request.id = ToNum(id_time_code[0]);
        auto [success, read] = file::ReadFile(file::GetFilePath(self, fileName));
        if (!success) {
            return {false, {}};
        }
        auto tmp = Decode(read);
        request.sender = tmp.back();
        tmp.pop_back();
        request.content = tmp;
        result.push_back(request);
    }
    for (const auto &fileName : files) {
        file::DeleteFile(file::GetFilePath(self, fileName));
    }
    return {true, std::move(result)};
}

bool trm::SendReply(const std::string &link, int id, const Information &reply) noexcept
{
    if (!file::CheckDirectoryExists(link)) {
        return false;
    }
    auto filePath = file::GetFilePath(link, ToStr(id));
    return file::WriteFile(filePath, Encode(reply));
}

std::pair<bool, trm::Information> trm::PollReply(const std::string &self, int id) noexcept
{
    auto filePath = file::GetFilePath(self, ToStr(id));
    if (!file::CheckFileExists(filePath)) {
        return {false, {}};
    }
    auto [success, read] = file::ReadFile(filePath);
    if (!success) {
        return {false, {}};
    }
    file::DeleteFile(filePath);
    return {true, Decode(read)};
}

trm::Message trm::Encode(const Information &information) noexcept
{
    return Combine(information);
}

trm::Information trm::Decode(const Message &message) noexcept
{
    return Split(message);
}
//建议使用
std::string trm::Combine(const std::vector<std::string> &series) noexcept
{
    std::string result = "";
    for (const auto &each : series) {
        result += '\x1d' + each + '\x1f';
    }
    return std::move(result);
}
//建议使用
std::vector<std::string> trm::Split(const std::string &str) noexcept
{
    std::vector<std::string> result;
    std::string current = "";
    int openerCount = 0;
    for (auto c : str) {
        if (openerCount > 0) {
            if (c == '\x1f') {
                --openerCount;
                if (openerCount == 0) {
                    result.push_back(current);
                    current = "";
                    continue;
                }
            }
            current += c;
        }
        if (c == '\x1d') {
            ++openerCount;
        }
    }
    return std::move(result);
}

std::string trm::Combine(const std::vector<std::string> &series, char delimiter) noexcept
{
    std::string result = "";
    for (const auto &each : series) {
        result += each + delimiter;
    }
    return std::move(result);
}

std::vector<std::string> trm::Split(const std::string &str, char delimiter) noexcept
{
    std::vector<std::string> result;
    std::string current = "";
    for (auto c : str) {
        if (c == delimiter) {
            result.push_back(current);
            current = "";
        } else {
            current += c;
        }
    }
    return std::move(result);
}

unsigned long long trm::GenerateRandomCode() noexcept
{
    static std::mt19937 generator(time(nullptr));
    static std::uniform_int_distribution<unsigned long long> distribution(0, 1e18);
    return distribution(generator);
}

unsigned long long trm::GetTimeStamp() noexcept
{
    return time(nullptr);
}

std::string trm::Hash(const std::string &str) noexcept
{
    return str; // TODO: implement hash function
}

double trm::FuzzyMatch(const std::string &str1, const std::string &str2) noexcept
{
    const int len1 = str1.length();
    const int len2 = str2.length();

    std::vector<std::vector<int>> LCS(len1 + 1, std::vector<int>(len2 + 1));
    for (int i = 1; i <= len1; i++) {
        for (int j = 1; j <= len2; j++) {
            if (str1[i - 1] == str2[j - 1]) LCS[i][j] = LCS[i - 1][j - 1] + 1;
            else LCS[i][j] = std::max(LCS[i - 1][j], LCS[i][j - 1]);
        }
    }

    return LCS[len1][len2] / (double) (len1 + len2) / 2;
}


trm::Account::operator std::string() const noexcept
{
    return Combine({
        code, 
        hashedPassword, 
        Combine(Foreach<std::string, Access>(access, [](const Access &each){
            return AccessBox{each};
        })), 
        Combine(Foreach<std::string, Tag>(tags, [](const Tag &each){
            return Combine({each.first, each.second});
        }))
    });
}

trm::MailContent::operator std::string() const noexcept
{
    return Combine({
        ToStr(timeStamp), 
        sender, 
        receiver, 
        subject, 
        content, 
        ToStr((int)read)
    });
}

trm::MailContent::MailContent(const std::string &content) noexcept
{
    auto mailContent = Split(content);
    *this = {
        ToNum<unsigned long long>(mailContent[0]), 
        mailContent[1], 
        mailContent[2], 
        mailContent[3], 
        mailContent[4], 
        (bool)ToNum(mailContent[5])
    };
}

trm::Account::Account(const std::string &content) noexcept
{
    auto account = Split(content);
    *this = {
        account[0], 
        account[1], 
        Foreach<Access, std::string>(trm::Split(account[2]), [](const std::string &each){
            return AccessBox{each};
        }), 
        trm::Foreach<std::pair<std::string, std::string>, std::string>(trm::Split(account[3]), [](const std::string &each){
            auto pair = trm::Split(each);
            return std::make_pair(pair[0], pair[1]);
        })
    };
}

std::string trm::Account::operator[](const std::string &tagKey) noexcept
{
    for (const auto &[k, v] : tags) {
        if (k == tagKey) {
            return v;
        }
    }
    return "";
}

trm::CourseInformation::operator std::string() const noexcept
{
    return Combine({
        courseName,
        teacher, 
        location, 
        Combine(weeks)
    });
}

trm::CourseInformation::CourseInformation(const std::string &content) noexcept
{
    auto course = Split(content);
    *this = {
        course[0], 
        course[1], 
        course[2],
        trm::Split(course[3])
    };
}

trm::IdAndPhone::operator std::string() const noexcept
{
    std::string idandphone;
    idandphone += id + '-' + phone;
    return idandphone;
}

trm::IdAndPhone::IdAndPhone(const std::string &content) noexcept
{
    auto idAndPhone = Split(content, '-');
    *this = {
        idAndPhone[0], 
        idAndPhone[1]
    };
}

trm::ReserveDate::operator std::string() const noexcept
{
    return Combine({ToStr(month), ToStr(week), ToStr(date)},'-');
}
trm::Book::Book(const std::string &content) noexcept
{
    auto data = trm::Split(content);
    *this = {
        data[0],
        data[1],
        data[2],
        data[3],
        data[4],
        trm::Split(data[5]),
        ToNum<unsigned int>(data[6]),
        ToNum<unsigned int>(data[7]),
    };
}

trm::Book::operator std::string() const noexcept
{
    return trm::Combine({
        bookIsbn, bookName,
        bookPublicationDate,
        bookCatagory,
        storePosition,
        trm::Combine(bookAuthor),
        ToStr(bookTot),
        ToStr(bookBorrowed),
    });
}

trm::Date::operator std::string() const noexcept
{
    return trm::Combine({
        ToStr(currantTime)
    });
}
//date 和reservedate的函数交叉有点混乱，但是不建议改位置，很容易冲突
trm::ReserveDate::ReserveDate(const std::string &content) noexcept
{
    auto date = trm::Split(content,'-');
    *this = {
       date[0],
       date[1],
       date[2],
    };
}

trm::Date::Date(const std::string & content) noexcept
{
    auto data = trm::Split(content);
    *this = {
        ToNum<time_t>(data[0])
    };
}

trm::BorrowLog::operator std::string() const noexcept
{
    return trm::Combine({
        ToStr(borrowLast),
        start,
        borrower,
        bookIsbn,
    });
}

trm::BorrowLog::BorrowLog(const std::string &content) noexcept
{
    auto data = trm::Split(content);
    *this = {
        ToNum<int>(data[0]),
        data[1],
        data[2],
        data[3],
    };
}

trm::Sender::Sender(const Information &content, bool autoSend) noexcept
{
    saved = std::move(content);
    if (autoSend) {
        Send();
    }
}

void trm::Sender::SetContent(const Information &content) noexcept
{
    saved = std::move(content);
}

void trm::Sender::Send() noexcept
{
    id = GenerateID();
    if (!MakeRequest(link, {id, selfAsSender, saved})) {
        fail = true;
    }
}

std::pair<bool, trm::Information> trm::Sender::Poll() noexcept
{
    if (fail) {
        return {true, {rpl::FAIL}};
    }
    ++count;
    return PollReply(self, id);
}

trm::AccessBox::operator std::string() const noexcept
{
    return ToStr((int)access);
}

trm::AccessBox::AccessBox(const std::string &content) noexcept
{
    *this = {(Access)ToNum(content)};
}

trm::AccessBox::operator Access() const noexcept
{
    return access;
}
