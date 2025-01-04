#ifndef HLAM_SCENE_H
#define HLAM_SCENE_H

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

namespace hlam {

class Scene {
 public:
  virtual void Activate() = 0;
  virtual void Update(float dt) = 0;
  virtual void Draw() = 0;
  virtual void Exit() = 0;
  virtual ~Scene();
};

class SceneManager {
  std::unordered_map<std::string, std::unique_ptr<Scene>> scenes;
  std::string current;
  std::string next;

 public:
  SceneManager();

  template <class TScene, class... TParam>
  TScene* Register(const std::string& name, TParam&&... params) {
    scenes.emplace(name, std::make_unique<TScene>(std::forward<TParam>(params)...));
    return dynamic_cast<TScene*>(scenes[name].get());
  }
  void Change(const std::string& name);
  bool Update(float dt);
  bool Draw();

  std::string Current() const;
};

class ComboScene : public Scene {
  std::vector<std::unique_ptr<Scene>> scenes;

 public:
  ComboScene();
  void Activate();
  void Update(float dt);
  void Draw();
  void Exit();

  template <class TScene, class... TParam>
  ComboScene* With(TParam&&... params) {
    scenes.emplace_back(std::make_unique<TScene>(std::forward<TParam>(params)...));
    return this;
  }
};

class TimerScene : public Scene {
  SceneManager* sm;
  const float time;
  std::string next;
  float elapsed;

 public:
  TimerScene(SceneManager* sm, float time, std::string next);
  void Activate();
  void Update(float dt);
  void Draw();
  void Exit();
};

class LambdaScene : public Scene {
  std::function<void()> callback;

 public:
  LambdaScene(std::function<void()> callback);
  void Activate();
  void Update(float dt);
  void Draw();
  void Exit();
};

#ifdef RAYLIB_H
class TextureScene : public Scene {
  Texture2D tex;
  int width;
  int height;

 public:
  TextureScene(Texture2D tex, int width, int height);
  void Activate();
  void Update(float dt);
  void Draw();
  void Exit();
};
#endif  // RAYLIB_H

class KeyAwaitScene : public Scene {
  SceneManager* sm;
  int key;
  std::string next;

 public:
  KeyAwaitScene(SceneManager* sm, int key, std::string next);
  void Activate();
  void Update(float dt);
  void Draw();
  void Exit();
};

}  // namespace hlam

#endif /* HLAM_SCENE_H */

#ifdef HLAM_SCENE_IMPLEMENTATION

#include <iostream>

namespace hlam {

Scene::~Scene() = default;

SceneManager::SceneManager() {}

void SceneManager::Change(const std::string& name) { next = name; }

bool SceneManager::Update(float dt) {
  if (current != next) {
    std::cout << "[INFO] change scene from '" << current << "' to '" << next << "'\n";

    if (scenes.count(current) != 0) {
      scenes[current]->Exit();
    }

    if (scenes.count(next) != 0) {
      scenes[next]->Activate();
    }

    current = next;
  }

  if (scenes.count(current) != 0) {
    scenes.at(current)->Update(dt);
    return true;
  }
  return false;
}

bool SceneManager::Draw() {
  if (scenes.count(current) != 0) {
    scenes.at(current)->Draw();
    return true;
  }
  return false;
}

std::string SceneManager::Current() const { return current; }

ComboScene::ComboScene() {}
void ComboScene::Activate() {
  for (auto& scene : scenes) {
    scene->Activate();
  }
}
void ComboScene::Update(float dt) {
  for (auto& scene : scenes) {
    scene->Update(dt);
  }
}
void ComboScene::Draw() {
  for (auto& scene : scenes) {
    scene->Draw();
  }
}
void ComboScene::Exit() {
  for (auto& scene : scenes) {
    scene->Exit();
  }
}

#ifdef RAYLIB_H
KeyAwaitScene::KeyAwaitScene(SceneManager* sm, int key, std::string next) : sm(sm), key(key), next(next) {}
void KeyAwaitScene::Activate() {}
void KeyAwaitScene::Update(float dt) {
  if (GetKeyPressed() == key) {
    sm->Change(next);
  }
}
void KeyAwaitScene::Draw() {}
void KeyAwaitScene::Exit() {}

TextureScene::TextureScene(Texture2D tex, int width, int height) : tex(tex), width(width), height(height) {}
void TextureScene::Activate() {}
void TextureScene::Update(float dt) {}
void TextureScene::Draw() {
  Rectangle src{0, 0, float(tex.width), float(tex.height)};
  Rectangle dst{0, 0, float(width), float(height)};
  DrawTexturePro(tex, src, dst, {0, 0}, 0.0f, WHITE);
}
void TextureScene::Exit() {}
#endif  // RAYLIB_H

TimerScene::TimerScene(SceneManager* sm, float time, std::string next) : sm{sm}, time{time}, next{next} {}
void TimerScene::Activate() { elapsed = 0; }
void TimerScene::Update(float dt) {
  elapsed += dt;
  if (elapsed > time) {
    sm->Change(next);
  }
}

void TimerScene::Draw() {}

void TimerScene::Exit() {}

LambdaScene::LambdaScene(std::function<void()> callback) : callback(callback) {}

void LambdaScene::Activate() {}

void LambdaScene::Update(float dt) { callback(); }

void LambdaScene::Draw() {}

void LambdaScene::Exit() {}

}  // namespace hlam

#endif /* HLAM_SCENE_IMPLEMENTATION */