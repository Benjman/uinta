#include "uinta/component.h"

#include <gtest/gtest.h>

#include <algorithm>

#include "uinta/mock/mock_component.h"
#include "uinta/mock/mock_runtime_getter.h"

namespace uinta {

// TODO: Interval components need testing

MockRuntimeGetter runtimeGetter;

TEST(ComponentManager, InitialState) {
  ComponentManager componentManager(&runtimeGetter);
  ASSERT_EQ(0, componentManager.intervalComponentQueue().size());
  ASSERT_EQ(0, componentManager.preTickComponents().size());
  ASSERT_EQ(0, componentManager.tickComponents().size());
  ASSERT_EQ(0, componentManager.postTickComponents().size());
  ASSERT_EQ(0, componentManager.preRenderComponents().size());
  ASSERT_EQ(0, componentManager.renderComponents().size());
  ASSERT_EQ(0, componentManager.postRenderComponents().size());
}

TEST(ComponentManager, AddIntervalComponent) {
  ComponentManager componentManager(&runtimeGetter);
  auto* component = componentManager.add<MockIntervalComponent>();
  ASSERT_TRUE(component);
  ASSERT_EQ(1, componentManager.intervalComponentQueue().size());
  ASSERT_EQ(component, componentManager.intervalComponentQueue().top().get());
}

template <typename T>
inline void testAddComponent(
    ComponentManager* componentManager,
    const ComponentManager::ComponentStorage<T>& collection) noexcept {
  auto* component = componentManager->add<MockComponent<T>>();
  ASSERT_TRUE(component);
  ASSERT_EQ(1, collection.size());
  ASSERT_EQ(component, collection.begin()->get());
}

TEST(ComponentManager, AddPostRenderComponent) {
  ComponentManager componentManager(&runtimeGetter);
  testAddComponent<PostRenderComponent>(
      &componentManager, componentManager.postRenderComponents());
}

TEST(ComponentManager, AddPostTickComponent) {
  ComponentManager componentManager(&runtimeGetter);
  testAddComponent<PostTickComponent>(&componentManager,
                                      componentManager.postTickComponents());
}

TEST(ComponentManager, AddPreRenderComponent) {
  ComponentManager componentManager(&runtimeGetter);
  testAddComponent<PreRenderComponent>(&componentManager,
                                       componentManager.preRenderComponents());
}

TEST(ComponentManager, AddPreTickComponent) {
  ComponentManager componentManager(&runtimeGetter);
  testAddComponent<PreTickComponent>(&componentManager,
                                     componentManager.preTickComponents());
}

TEST(ComponentManager, AddRenderComponent) {
  ComponentManager componentManager(&runtimeGetter);
  testAddComponent<RenderComponent>(&componentManager,
                                    componentManager.renderComponents());
}

TEST(ComponentManager, AddTickComponent) {
  ComponentManager componentManager(&runtimeGetter);
  testAddComponent<TickComponent>(&componentManager,
                                  componentManager.tickComponents());
}

template <typename T>
inline void testRemoveComponent(
    ComponentManager* componentManager,
    const ComponentManager::ComponentStorage<T>& collection) noexcept {
  auto* component = componentManager->add<MockComponent<T>>();
  ASSERT_EQ(collection.begin(),
            std::find_if(collection.begin(), collection.end(),
                         [component](const auto& componentPtr) {
                           return componentPtr.get() == component;
                         }));
  componentManager->remove(component);
  ASSERT_EQ(collection.end(),
            std::find_if(collection.begin(), collection.end(),
                         [component](const auto& componentPtr) {
                           return componentPtr.get() == component;
                         }));
  ASSERT_EQ(nullptr, component);
}

TEST(ComponentManager, RemovePostRenderComponent) {
  ComponentManager componentManager(&runtimeGetter);
  testRemoveComponent<PostRenderComponent>(
      &componentManager, componentManager.postRenderComponents());
}

TEST(ComponentManager, RemovePostTickComponent) {
  ComponentManager componentManager(&runtimeGetter);
  testRemoveComponent<PostTickComponent>(&componentManager,
                                         componentManager.postTickComponents());
}

TEST(ComponentManager, RemovePreRenderComponent) {
  ComponentManager componentManager(&runtimeGetter);
  testRemoveComponent<PreRenderComponent>(
      &componentManager, componentManager.preRenderComponents());
}

TEST(ComponentManager, RemovePreTickComponent) {
  ComponentManager componentManager(&runtimeGetter);
  testRemoveComponent<PreTickComponent>(&componentManager,
                                        componentManager.preTickComponents());
}

TEST(ComponentManager, RemoveRenderComponent) {
  ComponentManager componentManager(&runtimeGetter);
  testRemoveComponent<RenderComponent>(&componentManager,
                                       componentManager.renderComponents());
}

TEST(ComponentManager, RemoveTickComponent) {
  ComponentManager componentManager(&runtimeGetter);
  testRemoveComponent<TickComponent>(&componentManager,
                                     componentManager.tickComponents());
}

TEST(ComponentManager, FindPostRenderComponent) {
  ComponentManager componentManager(&runtimeGetter);
  auto* component = componentManager.add<MockComponent<PostRenderComponent>>();
  ASSERT_EQ(component,
            componentManager.find<MockComponent<PostRenderComponent>>());
}

TEST(ComponentManager, FindPostTickComponent) {
  ComponentManager componentManager(&runtimeGetter);
  auto* component = componentManager.add<MockComponent<PostTickComponent>>();
  ASSERT_EQ(component,
            componentManager.find<MockComponent<PostTickComponent>>());
}

TEST(ComponentManager, FindPreRenderComponent) {
  ComponentManager componentManager(&runtimeGetter);
  auto* component = componentManager.add<MockComponent<PreRenderComponent>>();
  ASSERT_EQ(component,
            componentManager.find<MockComponent<PreRenderComponent>>());
}

TEST(ComponentManager, FindPreTickComponent) {
  ComponentManager componentManager(&runtimeGetter);
  auto* component = componentManager.add<MockComponent<PreTickComponent>>();
  ASSERT_EQ(component,
            componentManager.find<MockComponent<PreTickComponent>>());
}

TEST(ComponentManager, FindRenderComponent) {
  ComponentManager componentManager(&runtimeGetter);
  auto* component = componentManager.add<MockComponent<RenderComponent>>();
  ASSERT_EQ(component, componentManager.find<MockComponent<RenderComponent>>());
}

TEST(ComponentManager, FindTickComponent) {
  ComponentManager componentManager(&runtimeGetter);
  auto* component = componentManager.add<MockComponent<TickComponent>>();
  ASSERT_EQ(component, componentManager.find<MockComponent<TickComponent>>());
}

}  // namespace uinta
