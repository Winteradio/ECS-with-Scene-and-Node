#include <Log/include/Log.h>
#include <Log/include/LogPlatform.h>

#include <Container/include/List.h>
#include <Container/include/StaticArray.h>
#include <Container/include/DynamicArray.h>

int MAIN()
{
    Log::Init(1024, Log::Enum::eMode_Print, Log::Enum::eLevel_Type | Log::Enum::eLevel_Time);

    LOGINFO() << "Hello World!";

    wtr::DynamicArray<int> vector;

    system("pause");

    return 0;
}