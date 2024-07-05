#include <tinystl/memory.h>
#include <catch2/catch_all.hpp>

/*
 * WARN: this test is generated by Github Copilot.
 */

using namespace tinystl;
TEST_CASE("Unique Ptr Tests", "[memory]") {
    SECTION("Default constructor") {
        unique_ptr<int> ptr;
        REQUIRE(ptr.get() == nullptr);
    }

    SECTION("Constructor with raw pointer") {
        int* rawPtr = new int(42);
        unique_ptr<int> ptr(rawPtr);
        REQUIRE(ptr.get() == rawPtr);
    }

    SECTION("Move constructor") {
        unique_ptr<int> ptr1(new int(42));
        unique_ptr<int> ptr2(std::move(ptr1));
        REQUIRE(ptr1.get() == nullptr);
        REQUIRE(*ptr2 == 42);
    }

    SECTION("Move assignment operator") {
        unique_ptr<int> ptr1(new int(42));
        unique_ptr<int> ptr2;
        ptr2 = std::move(ptr1);
        REQUIRE(ptr1.get() == nullptr);
        REQUIRE(*ptr2 == 42);
    }

    SECTION("Reset") {
        unique_ptr<int> ptr(new int(42));
        ptr.reset();
        REQUIRE(ptr.get() == nullptr);
    }

    SECTION("Reset with raw pointer") {
        unique_ptr<int> ptr(new int(42));
        int* rawPtr = new int(24);
        ptr.reset(rawPtr);
        REQUIRE(ptr.get() == rawPtr);
    }

    SECTION("Release") {
        unique_ptr<int> ptr(new int(42));
        int* rawPtr = ptr.release();
        REQUIRE(ptr.get() == nullptr);
        REQUIRE(*rawPtr == 42);
        delete rawPtr;
    }

    SECTION("Make unique") {
        auto ptr = make_unique<int>(42);
        REQUIRE(*ptr == 42);
    }
}
