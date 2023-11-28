#include <gtest/gtest.h>
#include "finite-state-machine.h"

namespace FSM
{
    struct EventA {};

    class MyClass1 {
    public:
        // Default constructor
        static int defaultConstructorCalls;
        static int parameterizedConstructorCalls;
        static int copyConstructorCalls;
        static int moveConstructorCalls;
        static int copyAssignmentOperatorCalls;
        static int moveAssignmentOperatorCalls;
        static int destructorCalls;

        MyClass1()
        {
            ++defaultConstructorCalls;
        }

        // Parameterized constructor
        MyClass1(int value)
        {
            ++parameterizedConstructorCalls;
        }

        // Copy constructor
        MyClass1(const MyClass1& other)
        {
            ++copyConstructorCalls;
        }

        // Move constructor
        MyClass1(MyClass1&& other) noexcept
        {
            ++moveConstructorCalls;
        }

        // Copy assignment operator
        MyClass1& operator=(const MyClass1& other)
        {
            ++copyAssignmentOperatorCalls;
            return *this;
        }

        // Move assignment operator
        MyClass1& operator=(MyClass1&& other) noexcept
        {
            ++moveAssignmentOperatorCalls;
            return *this;
        }

        // Destructor
        ~MyClass1()
        {
            ++destructorCalls;
        }
    };

    class MyClass2 {
    public:
        static int defaultConstructorCalls;
        static int parameterizedConstructorCalls;
        static int copyConstructorCalls;
        static int moveConstructorCalls;
        static int copyAssignmentOperatorCalls;
        static int moveAssignmentOperatorCalls;
        static int destructorCalls;
        // Default constructor
        MyClass2()
        {
            ++defaultConstructorCalls;
        }

        // Parameterized constructor
        MyClass2(int value)
        {
            ++parameterizedConstructorCalls;
        }

        // Copy constructor
        MyClass2(const MyClass2& other)
        {
            ++copyConstructorCalls;
        }

        // Move constructor
        MyClass2(MyClass2&& other) noexcept
        {
            ++moveConstructorCalls;
        }

        // Copy assignment operator
        MyClass2& operator=(const MyClass2& other)
        {
            ++copyAssignmentOperatorCalls;
            return *this;
        }

        // Move assignment operator
        MyClass2& operator=(MyClass2&& other) noexcept
        {
            ++moveAssignmentOperatorCalls;
            return *this;
        }

        // Destructor
        ~MyClass2()
        {
            ++destructorCalls;
        }
    };

    int MyClass1::defaultConstructorCalls = 0;
    int MyClass1::parameterizedConstructorCalls = 0;
    int MyClass1::copyConstructorCalls = 0;
    int MyClass1::moveConstructorCalls = 0;
    int MyClass1::copyAssignmentOperatorCalls = 0;
    int MyClass1::moveAssignmentOperatorCalls = 0;
    int MyClass1::destructorCalls = 0;

    int MyClass2::defaultConstructorCalls = 0;
    int MyClass2::parameterizedConstructorCalls = 0;
    int MyClass2::copyConstructorCalls = 0;
    int MyClass2::moveConstructorCalls = 0;
    int MyClass2::copyAssignmentOperatorCalls = 0;
    int MyClass2::moveAssignmentOperatorCalls = 0;
    int MyClass2::destructorCalls = 0;

    template <typename ClassName>
    void resetCounters()
    {
        ClassName::defaultConstructorCalls = 0;
        ClassName::parameterizedConstructorCalls = 0;
        ClassName::copyConstructorCalls = 0;
        ClassName::moveConstructorCalls = 0;
        ClassName::copyAssignmentOperatorCalls = 0;
        ClassName::moveAssignmentOperatorCalls = 0;
        ClassName::destructorCalls = 0;
    }

    TEST(InitialStatesCtorDtorTests, DefaultInitialState)
    {
        resetCounters<MyClass1>();
        resetCounters<MyClass2>();
        using transition1 = Transition<MyClass1, EventA, MyClass2>;
        using transitions_table = TransitionsTable<
            transition1
        >;
        auto stateMachine = StateMachine<transitions_table>{};
        EXPECT_EQ(MyClass1::defaultConstructorCalls, 1);
        EXPECT_EQ(MyClass1::parameterizedConstructorCalls, 0);
        EXPECT_EQ(MyClass1::copyConstructorCalls, 0);
        EXPECT_EQ(MyClass1::moveConstructorCalls, 0);
        EXPECT_EQ(MyClass1::copyAssignmentOperatorCalls, 0);
        EXPECT_EQ(MyClass1::moveAssignmentOperatorCalls, 0);
        EXPECT_EQ(MyClass1::destructorCalls, 0);

        EXPECT_EQ(MyClass2::defaultConstructorCalls, 0);
        EXPECT_EQ(MyClass2::parameterizedConstructorCalls, 0);
        EXPECT_EQ(MyClass2::copyConstructorCalls, 0);
        EXPECT_EQ(MyClass2::moveConstructorCalls, 0);
        EXPECT_EQ(MyClass2::copyAssignmentOperatorCalls, 0);
        EXPECT_EQ(MyClass2::moveAssignmentOperatorCalls, 0);
        EXPECT_EQ(MyClass2::destructorCalls, 0);
    }

    TEST(InitialStatesCtorDtorTests, ExplicitInitialState)
    {
        resetCounters<MyClass1>();
        resetCounters<MyClass2>();
        using transition1 = Transition<MyClass1, EventA, MyClass2>;
        using transitions_table = TransitionsTable<
            transition1
        >;
        auto stateMachine = StateMachine<transitions_table, MyClass2>{};
        EXPECT_EQ(MyClass1::defaultConstructorCalls, 0);
        EXPECT_EQ(MyClass1::parameterizedConstructorCalls, 0);
        EXPECT_EQ(MyClass1::copyConstructorCalls, 0);
        EXPECT_EQ(MyClass1::moveConstructorCalls, 0);
        EXPECT_EQ(MyClass1::copyAssignmentOperatorCalls, 0);
        EXPECT_EQ(MyClass1::moveAssignmentOperatorCalls, 0);
        EXPECT_EQ(MyClass1::destructorCalls, 0);

        EXPECT_EQ(MyClass2::defaultConstructorCalls, 1);
        EXPECT_EQ(MyClass2::parameterizedConstructorCalls, 0);
        EXPECT_EQ(MyClass2::copyConstructorCalls, 0);
        EXPECT_EQ(MyClass2::moveConstructorCalls, 0);
        EXPECT_EQ(MyClass2::copyAssignmentOperatorCalls, 0);
        EXPECT_EQ(MyClass2::moveAssignmentOperatorCalls, 0);
        EXPECT_EQ(MyClass2::destructorCalls, 0);
    }
}