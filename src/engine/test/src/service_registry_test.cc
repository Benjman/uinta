#include "uinta/engine/service_registry.h"

#include <gtest/gtest.h>

#include <thread>
#include <vector>

#include "./utils.h"
#include "uinta/engine/service_handle.h"
#include "uinta/mock/mock_platform.h"

namespace uinta {

class ServiceRegistryTest : public UintaTestF {};

// Test service classes
class TestServiceA {
 public:
  int value = 42;
};

class TestServiceB {
 public:
  std::string name = "ServiceB";
};

TEST_F(ServiceRegistryTest, RegisterAndRetrieveService) {
  ServiceRegistry registry;
  TestServiceA serviceA;

  registry.registerService(&serviceA);
  auto* retrieved = registry.service<TestServiceA>();

  ASSERT_NE(nullptr, retrieved);
  EXPECT_EQ(42, retrieved->value);
  EXPECT_EQ(&serviceA, retrieved);
}

TEST_F(ServiceRegistryTest, RetrieveUnregisteredServiceReturnsNullptr) {
  ServiceRegistry registry;

  auto* retrieved = registry.service<TestServiceA>();

  ASSERT_EQ(nullptr, retrieved);
}

TEST_F(ServiceRegistryTest, RegisterMultipleServiceTypes) {
  ServiceRegistry registry;
  TestServiceA serviceA;
  TestServiceB serviceB;

  registry.registerService(&serviceA);
  registry.registerService(&serviceB);

  auto* retrievedA = registry.service<TestServiceA>();
  auto* retrievedB = registry.service<TestServiceB>();

  ASSERT_NE(nullptr, retrievedA);
  ASSERT_NE(nullptr, retrievedB);
  EXPECT_EQ(&serviceA, retrievedA);
  EXPECT_EQ(&serviceB, retrievedB);
}

TEST_F(ServiceRegistryTest, UnregisterService) {
  ServiceRegistry registry;
  TestServiceA serviceA;

  registry.registerService(&serviceA);
  ASSERT_NE(nullptr, registry.service<TestServiceA>());

  registry.unregisterService<TestServiceA>();
  ASSERT_EQ(nullptr, registry.service<TestServiceA>());
}

TEST_F(ServiceRegistryTest, UnregisterNonexistentServiceIsNoOp) {
  ServiceRegistry registry;

  // Should not crash
  registry.unregisterService<TestServiceA>();
  ASSERT_EQ(nullptr, registry.service<TestServiceA>());
}

TEST_F(ServiceRegistryTest, ConstServiceAccess) {
  ServiceRegistry registry;
  TestServiceA serviceA;
  registry.registerService(&serviceA);

  const ServiceRegistry& constRegistry = registry;
  const auto* retrieved = constRegistry.service<TestServiceA>();

  ASSERT_NE(nullptr, retrieved);
  EXPECT_EQ(42, retrieved->value);
}

// Engine integration tests
TEST_F(ServiceRegistryTest, EngineRegisterAndRetrieveService) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);

  TestServiceA serviceA;
  engine.registerService(&serviceA);

  auto* retrieved = engine.service<TestServiceA>();
  ASSERT_NE(nullptr, retrieved);
  EXPECT_EQ(&serviceA, retrieved);
}

TEST_F(ServiceRegistryTest, EngineUnregisterService) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);

  TestServiceA serviceA;
  engine.registerService(&serviceA);
  ASSERT_NE(nullptr, engine.service<TestServiceA>());

  engine.unregisterService<TestServiceA>();
  ASSERT_EQ(nullptr, engine.service<TestServiceA>());
}

TEST_F(ServiceRegistryTest, EngineConstServiceAccess) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);

  TestServiceA serviceA;
  engine.registerService(&serviceA);

  const Engine& constEngine = engine;
  const auto* retrieved = constEngine.service<TestServiceA>();
  ASSERT_NE(nullptr, retrieved);
}

// ServiceHandle tests
class ServiceHandleTest : public UintaTestF {};

TEST_F(ServiceHandleTest, HandleRegistersServiceOnConstruction) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);

  TestServiceA serviceA;
  {
    ServiceHandle<TestServiceA> handle(&engine, &serviceA);
    EXPECT_NE(nullptr, engine.service<TestServiceA>());
  }
}

TEST_F(ServiceHandleTest, HandleUnregistersServiceOnDestruction) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);

  TestServiceA serviceA;
  {
    ServiceHandle<TestServiceA> handle(&engine, &serviceA);
    EXPECT_NE(nullptr, engine.service<TestServiceA>());
  }
  // Handle destroyed, service should be unregistered
  EXPECT_EQ(nullptr, engine.service<TestServiceA>());
}

TEST_F(ServiceHandleTest, HandleMoveConstruction) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);

  TestServiceA serviceA;
  ServiceHandle<TestServiceA> handle1(&engine, &serviceA);
  EXPECT_NE(nullptr, engine.service<TestServiceA>());

  ServiceHandle<TestServiceA> handle2(std::move(handle1));
  // Service should still be registered (handle2 now owns it)
  EXPECT_NE(nullptr, engine.service<TestServiceA>());
}

TEST_F(ServiceHandleTest, HandleMoveAssignment) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);

  TestServiceA serviceA;
  TestServiceB serviceB;

  ServiceHandle<TestServiceA> handleA(&engine, &serviceA);
  {
    ServiceHandle<TestServiceB> handleB(&engine, &serviceB);
    EXPECT_NE(nullptr, engine.service<TestServiceA>());
    EXPECT_NE(nullptr, engine.service<TestServiceB>());
  }
  // handleB destroyed, ServiceB should be unregistered
  EXPECT_NE(nullptr, engine.service<TestServiceA>());
  EXPECT_EQ(nullptr, engine.service<TestServiceB>());
}

TEST_F(ServiceHandleTest, MovedFromHandleDoesNotUnregister) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);

  TestServiceA serviceA;
  {
    ServiceHandle<TestServiceA> handle1(&engine, &serviceA);
    {
      ServiceHandle<TestServiceA> handle2(std::move(handle1));
      EXPECT_NE(nullptr, engine.service<TestServiceA>());
    }
    // handle2 destroyed, service should now be unregistered
    EXPECT_EQ(nullptr, engine.service<TestServiceA>());
  }
  // handle1 (moved-from) destroyed, should not crash or double-unregister
  EXPECT_EQ(nullptr, engine.service<TestServiceA>());
}

// Service event tests
class ServiceEventTest : public UintaTestF {};

TEST_F(ServiceEventTest, ServiceRegisteredEventFired) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);

  bool eventFired = false;
  void* receivedService = nullptr;
  std::type_index receivedType = std::type_index(typeid(void));

  engine.dispatchers()->addListener<EngineEvent::ServiceRegistered>(
      [&](const ServiceRegistered& event) {
        eventFired = true;
        receivedService = event.service;
        receivedType = event.type;
      });

  TestServiceA serviceA;
  engine.registerService(&serviceA);

  EXPECT_TRUE(eventFired);
  EXPECT_EQ(&serviceA, receivedService);
  EXPECT_EQ(std::type_index(typeid(TestServiceA)), receivedType);
}

TEST_F(ServiceEventTest, ServiceUnregisteredEventFired) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);

  TestServiceA serviceA;
  engine.registerService(&serviceA);

  bool eventFired = false;
  std::type_index receivedType = std::type_index(typeid(void));

  engine.dispatchers()->addListener<EngineEvent::ServiceUnregistered>(
      [&](const ServiceUnregistered& event) {
        eventFired = true;
        receivedType = event.type;
      });

  engine.unregisterService<TestServiceA>();

  EXPECT_TRUE(eventFired);
  EXPECT_EQ(std::type_index(typeid(TestServiceA)), receivedType);
}

TEST_F(ServiceEventTest, ServiceRegisteredEventIsMethod) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);

  bool isTestServiceA = false;
  bool isTestServiceB = false;

  engine.dispatchers()->addListener<EngineEvent::ServiceRegistered>(
      [&](const ServiceRegistered& event) {
        isTestServiceA = event.is<TestServiceA>();
        isTestServiceB = event.is<TestServiceB>();
      });

  TestServiceA serviceA;
  engine.registerService(&serviceA);

  EXPECT_TRUE(isTestServiceA);
  EXPECT_FALSE(isTestServiceB);
}

TEST_F(ServiceEventTest, ServiceRegisteredEventAsMethod) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);

  TestServiceA* receivedA = nullptr;
  TestServiceB* receivedB = nullptr;

  engine.dispatchers()->addListener<EngineEvent::ServiceRegistered>(
      [&](const ServiceRegistered& event) {
        receivedA = event.as<TestServiceA>();
        receivedB = event.as<TestServiceB>();
      });

  TestServiceA serviceA;
  engine.registerService(&serviceA);

  EXPECT_EQ(&serviceA, receivedA);
  EXPECT_EQ(nullptr, receivedB);
}

TEST_F(ServiceEventTest, ServiceUnregisteredEventIsMethod) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);

  TestServiceA serviceA;
  engine.registerService(&serviceA);

  bool isTestServiceA = false;
  bool isTestServiceB = false;

  engine.dispatchers()->addListener<EngineEvent::ServiceUnregistered>(
      [&](const ServiceUnregistered& event) {
        isTestServiceA = event.is<TestServiceA>();
        isTestServiceB = event.is<TestServiceB>();
      });

  engine.unregisterService<TestServiceA>();

  EXPECT_TRUE(isTestServiceA);
  EXPECT_FALSE(isTestServiceB);
}

TEST_F(ServiceEventTest, ServiceHandleFiresEvents) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);

  int registeredCount = 0;
  int unregisteredCount = 0;

  engine.dispatchers()->addListener<EngineEvent::ServiceRegistered>(
      [&](const ServiceRegistered& event) {
        if (event.is<TestServiceA>()) {
          registeredCount++;
        }
      });

  engine.dispatchers()->addListener<EngineEvent::ServiceUnregistered>(
      [&](const ServiceUnregistered& event) {
        if (event.is<TestServiceA>()) {
          unregisteredCount++;
        }
      });

  TestServiceA serviceA;
  {
    ServiceHandle<TestServiceA> handle(&engine, &serviceA);
    EXPECT_EQ(1, registeredCount);
    EXPECT_EQ(0, unregisteredCount);
  }
  EXPECT_EQ(1, registeredCount);
  EXPECT_EQ(1, unregisteredCount);
}

TEST_F(ServiceEventTest, MultipleServicesFireSeparateEvents) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);

  int serviceARegistered = 0;
  int serviceBRegistered = 0;

  engine.dispatchers()->addListener<EngineEvent::ServiceRegistered>(
      [&](const ServiceRegistered& event) {
        if (event.is<TestServiceA>()) {
          serviceARegistered++;
        }
        if (event.is<TestServiceB>()) {
          serviceBRegistered++;
        }
      });

  TestServiceA serviceA;
  TestServiceB serviceB;

  engine.registerService(&serviceA);
  EXPECT_EQ(1, serviceARegistered);
  EXPECT_EQ(0, serviceBRegistered);

  engine.registerService(&serviceB);
  EXPECT_EQ(1, serviceARegistered);
  EXPECT_EQ(1, serviceBRegistered);
}

// Thread safety tests
TEST_F(ServiceRegistryTest, ConcurrentReads) {
  ServiceRegistry registry;
  TestServiceA serviceA;
  registry.registerService(&serviceA);

  std::vector<std::thread> threads;
  std::atomic<int> successCount{0};

  threads.reserve(10);
  for (int i = 0; i < 10; ++i) {
    threads.emplace_back([&registry, &serviceA, &successCount]() {
      for (int j = 0; j < 100; ++j) {
        auto* retrieved = registry.service<TestServiceA>();
        if (retrieved == &serviceA) {
          successCount++;
        }
      }
    });
  }

  for (auto& t : threads) {
    t.join();
  }

  EXPECT_EQ(1000, successCount);
}

TEST_F(ServiceRegistryTest, ConcurrentReadsDuringUnregister) {
  ServiceRegistry registry;
  TestServiceA serviceA;
  registry.registerService(&serviceA);

  std::atomic<bool> stopReading{false};
  std::atomic<int> readCount{0};
  std::atomic<int> nullCount{0};

  // Reader threads
  std::vector<std::thread> readers;
  readers.reserve(4);
  for (int i = 0; i < 4; ++i) {
    readers.emplace_back([&]() {
      while (!stopReading) {
        auto* retrieved = registry.service<TestServiceA>();
        if (retrieved != nullptr) {
          readCount++;
        } else {
          nullCount++;
        }
      }
    });
  }

  // Let readers run briefly
  std::this_thread::sleep_for(std::chrono::milliseconds(10));

  // Unregister while readers are active
  registry.unregisterService<TestServiceA>();

  // Let readers observe the change
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  stopReading = true;

  for (auto& t : readers) {
    t.join();
  }

  // Should have had some successful reads before unregister
  EXPECT_GT(readCount, 0);
  // Should have had some null reads after unregister
  EXPECT_GT(nullCount, 0);
}

}  // namespace uinta
