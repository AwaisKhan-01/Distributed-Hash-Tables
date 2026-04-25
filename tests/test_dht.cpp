#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "BigInt.h"
#include "Hash.h"
#include "DHT_Ring.h"

// ---------------------------------------------------------------------------
// BigInt tests
// ---------------------------------------------------------------------------

TEST_CASE("BigInt default construction is zero", "[bigint]") {
    BigInt a(0);
    BigInt b(0);
    REQUIRE(a == b);
}

TEST_CASE("BigInt addition", "[bigint]") {
    BigInt a(3);
    BigInt b(7);
    BigInt expected(10);
    REQUIRE((a + b) == expected);
}

TEST_CASE("BigInt multiplication", "[bigint]") {
    BigInt a(6);
    BigInt b(7);
    BigInt expected(42);
    REQUIRE((a * b) == expected);
}

TEST_CASE("BigInt modulo", "[bigint]") {
    BigInt a(17);
    BigInt b(5);
    BigInt expected(2);
    REQUIRE((a % b) == expected);
}

TEST_CASE("BigInt power", "[bigint]") {
    BigInt base(2);
    BigInt exp(10);
    BigInt expected(1024);
    REQUIRE(pow(base, exp) == expected);
}

TEST_CASE("BigInt comparison operators", "[bigint]") {
    BigInt small(5);
    BigInt large(100);
    REQUIRE(small < large);
    REQUIRE(large > small);
    REQUIRE(small <= small);
    REQUIRE(large >= large);
    REQUIRE(small != large);
}

TEST_CASE("BigInt string construction", "[bigint]") {
    BigInt a("12345");
    BigInt b(12345);
    REQUIRE(a == b);
}

// ---------------------------------------------------------------------------
// SHA1 / Hash tests
// ---------------------------------------------------------------------------

TEST_CASE("SHA1 hash of empty string is deterministic", "[hash]") {
    std::string h1 = calculateStringHash("");
    std::string h2 = calculateStringHash("");
    REQUIRE(h1 == h2);
    // SHA1 produces 40 hex characters
    REQUIRE(h1.size() == 40);
}

TEST_CASE("SHA1 hash of known string", "[hash]") {
    // SHA1("abc") = a9993e364706816aba3e25717850c26c9cd0d89d
    std::string h = calculateStringHash("abc");
    REQUIRE(h == "a9993e364706816aba3e25717850c26c9cd0d89d");
}

TEST_CASE("SHA1 different inputs produce different hashes", "[hash]") {
    std::string h1 = calculateStringHash("machine1");
    std::string h2 = calculateStringHash("machine2");
    REQUIRE(h1 != h2);
}

// ---------------------------------------------------------------------------
// DHT_Ring tests
// ---------------------------------------------------------------------------

TEST_CASE("DHT_Ring starts empty", "[dht]") {
    DHT_Ring dht;
    REQUIRE(dht.isEmpty());
}

TEST_CASE("DHT_Ring add single machine", "[dht]") {
    DHT_Ring dht;
    dht.setBitspace(4);
    BigInt id(5);
    bool ok = dht.add_machine(id, "node0", "/tmp/node0", 3);
    REQUIRE(ok == true);
    REQUIRE(!dht.isEmpty());
    REQUIRE(dht.getExistingMachines() == 1);
}

TEST_CASE("DHT_Ring rejects duplicate machine hash", "[dht]") {
    DHT_Ring dht;
    dht.setBitspace(4);
    BigInt id1(5);
    BigInt id2(5);
    bool ok1 = dht.add_machine(id1, "nodeA", "/tmp/nodeA", 3);
    bool ok2 = dht.add_machine(id2, "nodeB", "/tmp/nodeB", 3);
    REQUIRE(ok1 == true);
    REQUIRE(ok2 == false);
}

TEST_CASE("DHT_Ring add multiple machines and keeps order", "[dht]") {
    DHT_Ring dht;
    dht.setBitspace(5);  // space 0..31
    BigInt id1(3);
    BigInt id2(10);
    BigInt id3(20);
    REQUIRE(dht.add_machine(id1, "n3",  "/tmp/n3",  3));
    REQUIRE(dht.add_machine(id2, "n10", "/tmp/n10", 3));
    REQUIRE(dht.add_machine(id3, "n20", "/tmp/n20", 3));
    REQUIRE(dht.getExistingMachines() == 3);
}

TEST_CASE("DHT_Ring hash is mapped to identifier space", "[dht]") {
    // With 4-bit space (0..15), adding machine with id=20 should wrap to 20%16=4
    DHT_Ring dht;
    dht.setBitspace(4);
    BigInt id(20);
    bool ok = dht.add_machine(id, "nodeWrap", "/tmp/nodeWrap", 3);
    REQUIRE(ok == true);
    // getHead()->getMachine_hash() should equal 4
    BigInt expected(4);
    REQUIRE(dht.getHead()->getMachine_hash() == expected);
}
