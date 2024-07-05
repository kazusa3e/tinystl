#include <tinystl/allocator.h>
#include <catch2/catch_all.hpp>

/*
 * WARN: this test is generated by Github Copilot.
 */

using namespace tinystl;

TEST_CASE("Allocator Tests", "[allocator]") {
    SECTION("Allocate and deallocate") {
        allocator<int> alloc;
        int* p = alloc.allocate(5);
        REQUIRE(p != nullptr);

        alloc.deallocate(p, 5);
    }

    SECTION("Construct and destroy") {
        allocator<std::string> alloc;
        std::string* p = alloc.allocate(1);
        alloc.construct(p, "Hello, World!");

        REQUIRE(*p == "Hello, World!");

        alloc.destroy(p);
        alloc.deallocate(p, 1);
    }

    SECTION("Address and max_size") {
        allocator<double> alloc;
        double* p = alloc.allocate(3);

        REQUIRE(alloc.address(*p) == p);
        REQUIRE(alloc.max_size() > 0);

        alloc.deallocate(p, 3);
    }

    SECTION("Allocate and deallocate multiple times") {
        allocator<int> alloc;
        int* p1 = alloc.allocate(3);
        int* p2 = alloc.allocate(2);
        REQUIRE(p1 != nullptr);
        REQUIRE(p2 != nullptr);

        alloc.deallocate(p1, 3);
        alloc.deallocate(p2, 2);
    }

    SECTION("Construct and destroy multiple objects") {
        allocator<std::string> alloc;
        std::string* p1 = alloc.allocate(2);
        std::string* p2 = alloc.allocate(2);
        alloc.construct(p1, "Hello");
        alloc.construct(p1 + 1, "World");
        alloc.construct(p2, "Foo");
        alloc.construct(p2 + 1, "Bar");

        REQUIRE(*p1 == "Hello");
        REQUIRE(*(p1 + 1) == "World");
        REQUIRE(*p2 == "Foo");
        REQUIRE(*(p2 + 1) == "Bar");

        alloc.destroy(p1);
        alloc.destroy(p1 + 1);
        alloc.destroy(p2);
        alloc.destroy(p2 + 1);
        alloc.deallocate(p1, 2);
        alloc.deallocate(p2, 2);
    }

    SECTION("Address and max_size of different types") {
        allocator<int> intAlloc;
        allocator<double> doubleAlloc;

        int* p1 = intAlloc.allocate(1);
        double* p2 = doubleAlloc.allocate(1);

        REQUIRE(intAlloc.address(*p1) == p1);
        REQUIRE(doubleAlloc.address(*p2) == p2);
        REQUIRE(intAlloc.max_size() > 0);
        REQUIRE(doubleAlloc.max_size() > 0);

        intAlloc.deallocate(p1, 1);
        doubleAlloc.deallocate(p2, 1);
    }

    SECTION("Allocate and deallocate zero elements") {
        allocator<int> alloc;
        int* p = alloc.allocate(0);
        REQUIRE(p == nullptr);
    }

    SECTION("Construct and destroy objects with different constructors") {
        allocator<std::vector<int>> alloc;
        std::vector<int>* p = alloc.allocate(1);
        alloc.construct(p, std::initializer_list<int>{1, 2, 3});

        REQUIRE(p->size() == 3);
        REQUIRE((*p)[0] == 1);
        REQUIRE((*p)[1] == 2);
        REQUIRE((*p)[2] == 3);

        alloc.destroy(p);
        alloc.deallocate(p, 1);
    }

    SECTION("Address and max_size of custom type") {
        struct CustomType {
            int value;
            std::string name;
        };

        allocator<CustomType> alloc;
        CustomType* p = alloc.allocate(1);
        alloc.construct(p, CustomType{42, "Test"});

        REQUIRE(alloc.address(*p) == p);
        REQUIRE(alloc.max_size() > 0);
        REQUIRE(p->value == 42);
        REQUIRE(p->name == "Test");

        alloc.destroy(p);
        alloc.deallocate(p, 1);
    }

    SECTION("Allocate and deallocate large number of elements") {
        allocator<int> alloc;
        const int numElements = 1000000;
        int* p = alloc.allocate(numElements);
        REQUIRE(p != nullptr);

        alloc.deallocate(p, numElements);
    }

    SECTION("Construct and destroy objects with different constructors") {
        allocator<std::vector<int>> alloc;
        std::vector<int>* p = alloc.allocate(1);
        alloc.construct(p, 5, 10);

        REQUIRE(p->size() == 5);
        REQUIRE((*p)[0] == 10);
        REQUIRE((*p)[1] == 10);
        REQUIRE((*p)[2] == 10);
        REQUIRE((*p)[3] == 10);
        REQUIRE((*p)[4] == 10);

        alloc.destroy(p);
        alloc.deallocate(p, 1);
    }

    SECTION("Address and max_size of custom type") {
        struct CustomType {
            int value;
            std::string name;
        };

        allocator<CustomType> alloc;
        CustomType* p = alloc.allocate(1);
        alloc.construct(p, CustomType{99, "Custom"});

        REQUIRE(alloc.address(*p) == p);
        REQUIRE(alloc.max_size() > 0);
        REQUIRE(p->value == 99);
        REQUIRE(p->name == "Custom");

        alloc.destroy(p);
        alloc.deallocate(p, 1);
    }

    SECTION("Allocate and deallocate large number of elements multiple times") {
        allocator<int> alloc;
        const int numElements = 1000000;
        int* p1 = alloc.allocate(numElements);
        int* p2 = alloc.allocate(numElements);
        REQUIRE(p1 != nullptr);
        REQUIRE(p2 != nullptr);

        alloc.deallocate(p1, numElements);
        alloc.deallocate(p2, numElements);
    }

    SECTION("Construct and destroy objects with different constructors multiple times") {
        allocator<std::vector<int>> alloc;
        std::vector<int>* p1 = alloc.allocate(1);
        std::vector<int>* p2 = alloc.allocate(1);
        alloc.construct(p1, 5, 10);
        alloc.construct(p2, 3, 7);

        REQUIRE(p1->size() == 5);
        REQUIRE((*p1)[0] == 10);
        REQUIRE((*p1)[1] == 10);
        REQUIRE((*p1)[2] == 10);
        REQUIRE((*p1)[3] == 10);
        REQUIRE((*p1)[4] == 10);

        REQUIRE(p2->size() == 3);
        REQUIRE((*p2)[0] == 7);
        REQUIRE((*p2)[1] == 7);
        REQUIRE((*p2)[2] == 7);

        alloc.destroy(p1);
        alloc.destroy(p2);
        alloc.deallocate(p1, 1);
        alloc.deallocate(p2, 1);
    }

    SECTION("Address and max_size of custom type multiple times") {
        struct CustomType {
            int value;
            std::string name;
        };

        allocator<CustomType> alloc;
        CustomType* p1 = alloc.allocate(1);
        CustomType* p2 = alloc.allocate(1);
        alloc.construct(p1, CustomType{99, "Custom1"});
        alloc.construct(p2, CustomType{42, "Custom2"});

        REQUIRE(alloc.address(*p1) == p1);
        REQUIRE(alloc.max_size() > 0);
        REQUIRE(p1->value == 99);
        REQUIRE(p1->name == "Custom1");

        REQUIRE(alloc.address(*p2) == p2);
        REQUIRE(alloc.max_size() > 0);
        REQUIRE(p2->value == 42);
        REQUIRE(p2->name == "Custom2");

        alloc.destroy(p1);
        alloc.destroy(p2);
        alloc.deallocate(p1, 1);
        alloc.deallocate(p2, 1);
    }
}
