#include "student_system.hpp"

ssys::ReserveSystem::ReserveSystem() noexcept
{
    
}

ssys::ReserveSystem::~ReserveSystem() noexcept
{
}

trm::Information ssys::ReserveSystem::CheckTime(const trm::Information& information) noexcept
{
    assert(information[0]==trm::rqs::CHECK_TIME);
    auto dateInformation=trm::ReserveDate(information[1]);
    auto reserveList=reserveBase[dateInformation.month][dateInformation.week][dateInformation.date];//找到指定日期的预约信息
    //待实现：只能在规定的天数内预约，比如只能预约最近一周
    trm::Information timeList;
    for(auto [time,reserve]:reserveList)//遍历预约信息
    {
        if(std::string(reserve)!="0")
        {
            timeList.push_back(time);//将可预约时间加入列表
        }
    }
    return timeList; 
}

trm::Information ssys::ReserveSystem::CheckReserveTime(const trm::Information& information) noexcept
{
    assert(information[0]==trm::rqs::CHECK_RESERVE_TIME);
    auto dateInformation=trm::ReserveDate(information[1]);
    auto targetReserve=reserveBase[dateInformation.month][dateInformation.week][dateInformation.date];//找到指定日期的预约信息
    for(auto [time,reserve]:targetReserve)//遍历预约信息
    {
        if(time==information[2])//找到指定时间的预约信息
        {
            if(std::string(reserve)=="0")//如果预约名额已满
            {
                return {trm::rpl::NO,trm::rpl::NO_LEFT_RESERVE};
            }
            else
            {
                return {trm::rpl::YES};
            }
        }
    }
    return {trm::rpl::NO,trm::rpl::NO_MATCH_TIME};//如果没有找到指定时间的预约信息
}

trm::Information ssys::ReserveSystem::RequestReserve(const trm::Information& information) noexcept
{
    assert(information[0] == trm::rqs::REQUEST_RESERVE); // Procession not matched.
    auto timeReply=SSys::Get().CheckReserveTime({trm::rqs::CHECK_RESERVE_TIME,information[1],information[2]});//检查预约时间
    if(timeReply[0]!=trm::rpl::YES)
    {
       return {trm::rpl::FAIL,trm::rpl::NO_MATCH_RESERVE,timeReply[1]};
    }
    if(clientBase[trm::IdAndPhone{information[3],information[4]}][information[1]][information[2]].Exists())
    {
        return {trm::rpl::FAIL,trm::rpl::RESERVE_EXISTS};
    }
    clientBase[trm::IdAndPhone{information[3],information[4]}][information[1]].Push(std::make_pair(information[2], "HAVE RESERVED, READY TO USE")); // 将预约信息加入数据库
    return {trm::rpl::SUCC};
    //也许会返回成功预约的时间和状态，就类似自动跳转
}

trm::Information ssys::ReserveSystem::CancelReserve(const trm::Information& information) noexcept
{
    assert(information[0] == trm::rqs::CANCEL_RESERVE); // Procession not matched.
    if(!clientBase[trm::IdAndPhone{information[3],information[4]}].Exists())//这个权限检查会不会太草率了
    {
       return{trm::rpl::NO_DERESERVE_ACCESS};
    }
    if(clientBase[trm::IdAndPhone{information[3],information[4]}][information[1]][information[2]].Exists())//检查是否存在预约信息//那时间就不对了怎么办
    {
        clientBase[trm::IdAndPhone{information[3],information[4]}][information[1]][information[2]].Clear();//清除预约信息
        return {trm::rpl::SUCC};
    }
    else
    {
        return {trm::rpl::FAIL,trm::rpl::NO_MATCH_RESERVE};
    }
}

trm::Information ssys::ReserveSystem::CheckReserveStatus(const trm::Information& information) noexcept
{
    assert(information[0] == trm::rqs::CHECK_RESERVE_STATUS); // Procession not matched.
    auto reserve=clientBase[trm::IdAndPhone{information[3],information[4]}][information[1]][information[2]];
    if(reserve.Exists())//检查是否存在预约信息
    {
        return {trm::rpl::SUCC,reserve};
    }
    else
    {
        return {trm::rpl::FAIL,trm::rpl::NO_MATCH_RESERVE};
    }
}

trm::Information ssys::ReserveSystem::CheckReserveList(const trm::Information& information) noexcept
{
    assert(information[0] == trm::rqs::CHECK_RESERVE_STATUS_LIST); // Procession not matched.
    if(!clientBase[trm::IdAndPhone{information[1],information[2]}].Exists())//检查是否存在预约信息
    {
        return {trm::rpl::FAIL,trm::rpl::NO_RESERVE_EXISTS};
    }
    trm::Information reserveList;
    reserveList.push_back(trm::rpl::SUCC);
    for(auto [date,reserve]:clientBase[trm::IdAndPhone{information[1],information[2]}])
    {
        for(auto [time,status]:reserve)
        {
            reserveList.push_back(trm::Combine({date,time,status}));//将预约信息加入列表
        }
    }
    return reserveList;
}