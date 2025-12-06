#include <Log/include/Log.h>
#include <Log/include/LogPlatform.h>

#include <Memory/include/Container/List.h>
#include <Memory/include/Container/StaticArray.h>

#define LOG_ASSERT(expr, msg) \
    if (!(expr)) { \
        LOGERROR() << "[FAIL] " << msg << " (" << #expr << ")"; \
    } else { \
        LOGINFO() << "[PASS] " << msg; \
    }

void TestConstruction()
{
    LOGINFO("=== Test Construction & Partial Init ===");

    // 1. 꽉 채운 초기화
    Memory::StaticArray<int, 5> fullArr = { 1, 2, 3, 4, 5 };
    LOG_ASSERT(fullArr.Size() == 5, "Size Check");
    LOG_ASSERT(fullArr[4] == 5, "Full Init Check");

    // 2. 부분 초기화 (나머지는 0이어야 함)
    Memory::StaticArray<int, 5> partialArr = { 10, 20 };
    LOG_ASSERT(partialArr[0] == 10, "Partial Index 0");
    LOG_ASSERT(partialArr[1] == 20, "Partial Index 1");
    LOG_ASSERT(partialArr[2] == 0, "Partial Index 2 (Default Init)");
    LOG_ASSERT(partialArr[4] == 0, "Partial Index 4 (Default Init)");
}

void TestCopyAndAssign()
{
    LOGINFO("=== Test Copy & Assignment (Deep Copy) ===");

    Memory::StaticArray<int, 3> original = { 1, 2, 3 };
    Memory::StaticArray<int, 3> copy = original; // 복사 생성자

    // 복사본 수정
    copy[0] = 999;

    // 원본은 그대로여야 함 (Deep Copy 확인)
    LOG_ASSERT(original[0] == 1, "Original Should Not Change");
    LOG_ASSERT(copy[0] == 999, "Copy Should Change");

    // 대입 연산자 테스트
    Memory::StaticArray<int, 3> assigned;
    assigned = copy;
    LOG_ASSERT(assigned[0] == 999, "Assignment Check");
    LOG_ASSERT(assigned == copy, "Equality Operator Check");
    LOG_ASSERT(assigned != original, "Inequality Operator Check");
}

void TestIterators()
{
    LOGINFO("=== Test Forward Iterators ===");

    Memory::StaticArray<int, 5> arr = { 10, 20, 30, 40, 50 };

    int count = 0;
    int sum = 0;

    // Range-based for loop (begin, end)
    for (auto& val : arr)
    {
        sum += val;
        count++;
    }

    LOG_ASSERT(count == 5, "Iteration Count");
    LOG_ASSERT(sum == 150, "Iteration Sum");

    // 수동 조작 테스트
    auto it = arr.begin();
    LOG_ASSERT(*it == 10, "Begin Value");

    ++it; // 20
    LOG_ASSERT(*it == 20, "Pre-increment");

    it++; // 30 (후위 연산 후 내부는 30)
    LOG_ASSERT(*it == 30, "Post-increment");

    *it = 99; // 값 변경
    LOG_ASSERT(arr[2] == 99, "Iterator Modification");
}

void TestReverseIterators()
{
    LOGINFO() << "=== Test Reverse Iterators (Off-by-one logic) ===";

    // Index: 0, 1, 2, 3, 4
    // Value: 1, 2, 3, 4, 5
    Memory::StaticArray<int, 5> arr = { 1, 2, 3, 4, 5 };

    // rbegin() 테스트 (Tail을 가리켜야 함)
    auto rit = arr.rbegin();
    LOG_ASSERT(*rit == 5, "rbegin() should be last element");

    // 이동 테스트
    ++rit; // 4
    LOG_ASSERT(*rit == 4, "++rbegin should move backwards");

    // Loop 테스트
    int expectedValue = 5;
    bool orderCorrect = true;
    for (auto it = arr.rbegin(); it != arr.rend(); ++it)
    {
        if (*it != expectedValue)
        {
            orderCorrect = false;
            break;
        }
        expectedValue--;
    }
    LOG_ASSERT(orderCorrect, "Reverse Loop Order Correct (5 -> 1)");
    LOG_ASSERT(expectedValue == 0, "Loop reached rend correctly");
}

void TestConstCorrectness()
{
    LOGINFO() << "=== Test Const Correctness ===";

    const Memory::StaticArray<int, 3> constArr = { 100, 200, 300 };

    // 1. Const Iterator 반환 확인
    auto it = constArr.begin();
    // *it = 500; // <--- 컴파일 에러가 나야 정상! (Read-only)

    LOG_ASSERT(*it == 100, "Const Iterator Read");

    // 2. Front/Back/At/[] Const 버전 호출 확인
    LOG_ASSERT(constArr.Front() == 100, "Const Front");
    LOG_ASSERT(constArr.Back() == 300, "Const Back");
    LOG_ASSERT(constArr[1] == 200, "Const Operator[]");
}

void TestAlgorithmCompatibility()
{
    LOGINFO() << "=== Test STL Algorithm Compatibility ===";

    Memory::StaticArray<int, 5> arr = { 5, 1, 4, 2, 3 };

    // std::sort가 먹히려면 Random Access Iterator여야 함 (현재 구현에선 약간 부족할 수 있음)
    // 하지만 기본적인 for_each 등은 되어야 함.

    int sum = 0;
    // Iterator가 표준 스펙을 어느 정도 만족하는지 확인
    for (auto it = arr.begin(); it != arr.end(); ++it)
    {
        sum += *it;
    }
    LOG_ASSERT(sum == 15, "Standard loop compatibility");
}

int MAIN()
{
    Log::Init(1024, Log::Enum::eMode_Print, Log::Enum::eLevel_Type | Log::Enum::eLevel_Time);

    LOGINFO() << "Hello World!";

    TestList();

    TestConstruction();
    LOGINFO() << "--------------------------------";

    TestCopyAndAssign();
    LOGINFO() << "--------------------------------";

    TestIterators();
    LOGINFO() << "--------------------------------";

    TestReverseIterators();
    LOGINFO() << "--------------------------------";

    TestConstCorrectness();
    LOGINFO() << "--------------------------------";

    TestAlgorithmCompatibility();

    LOGINFO() << "--------------------------------";

    system("pause");

    return 0;
}