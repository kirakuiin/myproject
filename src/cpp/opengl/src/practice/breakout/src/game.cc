// game.cc -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Jul 27
// License: GPL.v3

#include "game.h"

#include <iostream>
#include <random>
#include <GLFW/glfw3.h>
#include "include/glm/gtc/matrix_transform.hpp"
#include "include/commondef.h"
#include "manager.h"

namespace {

vec2 INITIAL_BALL_VELOCITY = vec2(100.0f, -300.0f);
vec2 PADDLE_SIZE = vec2(100, 20);
float RADIUS = 12.5f;
std::vector<std::string> LEVELS {
    "config/breakout/test.lvl",
    "config/breakout/standard.lvl",
    "config/breakout/smallgaps.lvl",
    "config/breakout/spaceinvader.lvl",
    "config/breakout/bouncegalore.lvl",
};

std::random_device rd;
std::mt19937 generator(rd());
std::uniform_real_distribution<float> rand100(0, 100);

bool ShouldSpawn(float chance) {
    return rand100(generator) <= chance;
}

} // namespace

Game::Game(unsigned int width, unsigned int height)
    : state(GameState::GAME_MENU), keys(), width(width), height(height) {
}

Game::~Game() {
}

void
Game::Init() {
    auto shader = ResourceManager::LoadShader("sprite",
                                              "shaders/breakout/sprite.vert",
                                              "shaders/breakout/sprite.frag");
    auto partsh = ResourceManager::LoadShader("partsh",
                                              "shaders/breakout/particles.vert",
                                              "shaders/breakout/particles.frag");
    auto effesh = ResourceManager::LoadShader("effesh",
                                              "shaders/breakout/post.vert",
                                              "shaders/breakout/post.frag");
    glm::mat4 projection = glm::ortho(
            0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);
    shader->Use();
    shader->SetUniform("sprite", 0);
    shader->SetUniform("projection", projection);
    partsh->Use();
    partsh->SetUniform("projection", projection);
    partsh->SetUniform("sprite", 0);
    _srender.reset(new SpriteRender(shader));
    ResourceManager::LoadTexture("background",
                                 "images/texture/background_standard.jpg");
    ResourceManager::LoadTexture("face", "images/texture/awesomeface.png");
    ResourceManager::LoadTexture("block", "images/texture/block.png");
    ResourceManager::LoadTexture("block_solid",
                                 "images/texture/block_solid.png");
    ResourceManager::LoadTexture("paddle", "images/texture/paddle.png");
    ResourceManager::LoadTexture("particle", "images/texture/particle.png");
    ResourceManager::LoadTexture("speed", "images/texture/powerup_speed.png");
    ResourceManager::LoadTexture("incr", "images/texture/powerup_increase.png");
    ResourceManager::LoadTexture("pass", "images/texture/powerup_passthrough.png");
    ResourceManager::LoadTexture("stop", "images/texture/powerup_stop.png");
    ResourceManager::LoadTexture("sticky", "images/texture/powerup_sticky.png");
    ResourceManager::LoadTexture("chaos", "images/texture/powerup_chaos.png");
    ResourceManager::LoadTexture("confuse", "images/texture/powerup_confuse.png");
    ResourceManager::LoadTexture("life", "images/texture/powerup_life.png");
    // text
    _text.reset(new gl::Font(width, height));
    _text->Load("images/fonts/NewYork.ttf", 24);
    // audio
    _sound_engine.reset(irrklang::createIrrKlangDevice());
    _sound_engine->play2D("audio/breakout.mp3", true);
    // particle
    _gene.reset(
        new ParticleGenerator(ResourceManager::GetShader("partsh"),
                              ResourceManager::GetTexture("particle"),
                              500)
    );
    // postprocessor
    _effect.reset(
        new PostProcessor(ResourceManager::GetShader("effesh"), width, height)
    );
    // level
    GameLevel test;
    test.Load(LEVELS[0], width, height*0.5f);
    GameLevel one;
    one.Load(LEVELS[1], width, height*0.5f);
    GameLevel two;
    two.Load(LEVELS[2], width, height*0.5f);
    GameLevel three;
    three.Load(LEVELS[3], width, height*0.5f);
    GameLevel four;
    four.Load(LEVELS[4], width, height*0.5f);
    _levels.push_back(test);
    _levels.push_back(one);
    _levels.push_back(two);
    _levels.push_back(three);
    _levels.push_back(four);
    _level = 1;
    // paddle
    vec2 PADDLE_SIZE(100, 20);
    vec2 paddle_pos(width/2 - PADDLE_SIZE.x/2,
                    height - PADDLE_SIZE.y);
    _paddle.reset(new GameObject(paddle_pos, PADDLE_SIZE,
                                 ResourceManager::GetTexture("paddle")));
    _paddle->Velocity = vec2(500.0f, 0);
    // ball
    vec2 ballpos = paddle_pos + vec2(PADDLE_SIZE.x / 2 - RADIUS,
                                     -RADIUS*2-1);
    _ball.reset(new BallObject(ballpos,
                               ResourceManager::GetTexture("face"),
                               RADIUS, INITIAL_BALL_VELOCITY));
}

void
Game::ResetVar() {
    _lives = 3;
    _win_num = 0;
    _score = 0;
    _hit_num = 0;
}

void
Game::ResetGame() {
    ResetPaddle();
    ResetLevel();
    ResetPowerUps();
}

void
Game::ResetLevel() {
    _levels[_level].Load(LEVELS[_level], width, height*0.5f);
}

void
Game::ResetPaddle() {
    _paddle->Position = vec2(width/2 - PADDLE_SIZE.x/2, height - PADDLE_SIZE.y);
    _ball->Position = _paddle->Position + vec2(PADDLE_SIZE.x / 2 - RADIUS, -RADIUS*2);
    _ball->IsStuck = true;
    _ball->Velocity = INITIAL_BALL_VELOCITY;
}

void
Game::ResetPowerUps() {
    for (auto& iter: _powerups) {
        iter.Duration = 0.0f;
        iter.Destroyed = true;
    }
}

void
Game::ProcessInput(float delta) {
    if (GameState::GAME_ACTIVE == state) {
        float velo = _paddle->Velocity.x * delta;
        if (keys[GLFW_KEY_A]) {
            if (_paddle->Position.x > 0) {
                _paddle->Position.x -= velo;
                if (_ball->IsStuck) {
                    _ball->Position.x -= velo;
                }
            } else {
                if (_ball->IsStuck) {
                    _ball->Position.x -= _paddle->Position.x;
                }
                _paddle->Position.x = 0;
            }
        }
        if (keys[GLFW_KEY_D]) {
            if (_paddle->Position.x < width-_paddle->Size.x) {
                _paddle->Position.x += velo;
                if (_ball->IsStuck) {
                    _ball->Position.x += velo;
                }
            } else {
                if (_ball->IsStuck) {
                    _ball->Position.x += width-_paddle->Size.x-_paddle->Position.x;
                }
                _paddle->Position.x = width-_paddle->Size.x;
            }
        }
        if (keys[GLFW_KEY_SPACE]) {
            if (_ball->IsStuck) {
                _ball->IsStuck = false;
            }
        }
    } else if (GameState::GAME_MENU == state) {
        if (keys_just_rel[GLFW_KEY_ENTER]) {
            keys_just_rel[GLFW_KEY_ENTER] = false;
            state = GameState::GAME_ACTIVE;
        }
        if (keys_just_rel[GLFW_KEY_W]) {
            keys_just_rel[GLFW_KEY_W] = false;
            ++_level;
            if (_level == LEVELS.size()) {
                _level = 1;
            }
        }
        if (keys_just_rel[GLFW_KEY_S]) {
            keys_just_rel[GLFW_KEY_S] = false;
            --_level;
            if (_level == 0) {
                _level = LEVELS.size()-1;
            }
        }
    } else {
        if (keys_just_rel[GLFW_KEY_ENTER]) {
            keys_just_rel[GLFW_KEY_ENTER] = false;
            state = GameState::GAME_MENU;
            _effect->Chaos = false;
            _win_num = 0;
        }
    }
}

void
Game::Update(float delta) {
    if (!_ball->Stop) {
        _ball->Move(delta, width);
        _gene->Update(delta, *_ball.get(), 2, vec2(_ball->Radius/2));
    }
    DoCollisions();
    UpdatePowerUps(delta);
    if (_ball->Position.y >= height) {
        --_lives;
        _score = fmax(_score-100, 0);
        if (_lives <= 0) {
            ResetVar();
            state = GameState::GAME_MENU;
            ResetGame();
        } else {
            ResetPaddle();
        }
    }
    if (_levels[_level].IsComplete()) {
        _level = (_level+1) % LEVELS.size();
        if (_level == 0) {
            _level = 1;
        }
        _win_num += 1;
        _score += 500;
        ResetGame();
    }
    if (IsWin()) {
        _level = 1;
        _lives = 3;
        _effect->Chaos = true;
        state = GameState::GAME_WIN;
    }
    if (_shake_time > 0.0f) {
        _shake_time -= delta;
        if (_shake_time < 0.0f) {
            _effect->Shake = false;
        }
    }
}

void
Game::DoCollisions() {
    _ball->XRevert = false;
    _ball->YRevert = false;
    for (auto& box: _levels[_level].Bricks) {
        if (!box.Destroyed) {
            Collision coll = CheckCollision(*_ball.get(), box);
            if (std::get<0>(coll)) {
                if (!box.IsSolid) {
                    _hit_num += 1;
                    _score += _hit_num*10;
                    box.Destroyed = true;
                    SpawnPowerUps(box);
                    _sound_engine->play2D("audio/bleep.mp3", false);
                } else {
                    _shake_time = 0.05f;
                    _effect->Shake = true;
                    _sound_engine->play2D("audio/solid.wav", false);
                }
                Direction dir = std::get<1>(coll);
                vec2 diff = std::get<2>(coll);
                if (_ball->PassThrough && !box.IsSolid) {
                    continue;
                }
                if ((dir == Direction::LEFT || dir == Direction::RIGHT)
                    && !_ball->XRevert) {
                    _ball->XRevert = true;
                    _ball->Velocity.x *= -1;
                    // redirection
                    float penetration = (_ball->Radius - std::abs(diff.x));
                    if (dir == Direction::LEFT) {
                        _ball->Position.x -= penetration;
                    } else {
                        _ball->Position.x += penetration;
                    }
                } else if ((dir == Direction::UP || dir == Direction::DOWN)
                    && !_ball->YRevert) {
                    _ball->YRevert = true;
                    _ball->Velocity.y *= -1;
                    // redirection
                    float penetration = (_ball->Radius - std::abs(diff.y));
                    if (dir == Direction::UP) {
                        _ball->Position.y -= penetration;
                    } else {
                        _ball->Position.y += penetration;
                    }
                }
            }
        }
    }
    // check paddle
    Collision coll = CheckCollision(*_ball.get(), *_paddle.get());
    if (!_ball->IsStuck && std::get<0>(coll)) {
        float center = _paddle->Position.x + _paddle->Size.x/2;
        float distance = (_ball->Position.x + _ball->Radius) - center;
        float percentage = distance / (_paddle->Size.x/2);
        float strength = 2.0f;
        vec2 old_velo = _ball->Velocity;
        _ball->Velocity.x = strength * percentage * INITIAL_BALL_VELOCITY.x;
        _ball->Velocity.y = -std::abs(_ball->Velocity.y);
        _ball->Velocity = glm::normalize(_ball->Velocity) * glm::length(old_velo);
        if (_ball->Sticky) {
            _ball->IsStuck = true;
        }
        _sound_engine->play2D("audio/bleep.wav", false);
        _hit_num = 0;
    }
    // check powerup
    for (auto& iter: _powerups) {
        if (!iter.Destroyed) {
            if (iter.Position.y >= height) {
                iter.Destroyed = true;
            }
            if (CheckCollision(*_paddle.get(), iter)) {
                _sound_engine->play2D("audio/powerup.wav", false);
                ActivatePowerUp(iter);
                iter.Destroyed = true;
            }
        }
    }
}

void
Game::Render() {
    _effect->BeginRender();
    _srender->DrawSprite(ResourceManager::GetTexture("background"),
            vec2(0), 0.0f, vec2(width, height));
    _levels[_level].Draw(_srender);
    _paddle->Draw(_srender);
    _gene->Draw();
    _ball->Draw(_srender);
    for (auto& power: _powerups) {
        if (!power.Destroyed) {
            power.Draw(_srender);
        }
    }
    _effect->EndRender();
    _effect->Render(glfwGetTime());
    _text->Draw("Lives: " + std::to_string(_lives), vec3(5, 5, 1));
    _text->Draw("Score: " + std::to_string(_score), vec3(5, 25, 1));
    if (GameState::GAME_MENU == state) {
        _text->Draw("Press ENTER to start", vec3(300.0f, height/2-30.0f, 1.0f));
        _text->Draw("Press W or S to select level", vec3(270.0f, height/2+30.0f, 1.0f));
    }
    if (GameState::GAME_WIN == state) {
        _text->Draw("You WON!!!", vec3(320.0f, height/2-20.0f, 1.0f),
                    vec3(0, 1, 0));
        _text->Draw("Press ENTER to retry or ESC to quit", vec3(200.0f, height/2+30.0f, 1.0f), vec3(1, 1, 0));
    }
}

void
Game::SpawnPowerUps(GameObject& obj) {
    // 2 = 2%chance
    if (ShouldSpawn(2)) {
        _powerups.push_back(PowerUp(obj.Position, ResourceManager::GetTexture("life"),
                           "life", vec3(0.7, 0.0, 0.0), 0.0f));
    }
    if (ShouldSpawn(5)) {
        _powerups.push_back(PowerUp(obj.Position, ResourceManager::GetTexture("speed"),
                           "speed", vec3(0.5, 0.5, 1.0), 10.0f));
    }
    if (ShouldSpawn(5)) {
        _powerups.push_back(PowerUp(obj.Position, ResourceManager::GetTexture("sticky"),
                           "sticky", vec3(1.0, 0.5, 1.0), 20.0f));
    }
    if (ShouldSpawn(5)) {
        _powerups.push_back(PowerUp(obj.Position, ResourceManager::GetTexture("pass"),
                           "pass", vec3(1.0, 0.5, 0.5), 10.0f));
    }
    if (ShouldSpawn(5)) {
        _powerups.push_back(PowerUp(obj.Position, ResourceManager::GetTexture("incr"),
                           "incr", vec3(1.0, 0.6, 0.4), 20.0f));
    }
    if (ShouldSpawn(5)) {
        _powerups.push_back(PowerUp(obj.Position, ResourceManager::GetTexture("stop"),
                           "stop", vec3(0.3, 0.8, 0.4), 3.0f));
    }
    if (ShouldSpawn(7)) {
        _powerups.push_back(PowerUp(obj.Position, ResourceManager::GetTexture("confuse"),
                           "confuse", vec3(1.0, 0.3, 0.3), 15.0f));
    }
    if (ShouldSpawn(7)) {
        _powerups.push_back(PowerUp(obj.Position, ResourceManager::GetTexture("chaos"),
                           "chaos", vec3(0.9, 0.25, 0.25), 15.0f));
    }
}

void
Game::ActivatePowerUp(PowerUp& powerup) {
    powerup.Activated = true;
    if (powerup.Type == "speed") {
        _ball->Velocity *= 1.2;
        _ball->Color = vec3(0.5f, 0.5f, 1.0f);
    } else if (powerup.Type == "sticky") {
        _ball->Sticky = true;
        _paddle->Color = vec3(1.0f, 0.5f, 1.0f);
    } else if (powerup.Type == "pass") {
        _ball->PassThrough = true;
        _paddle->Color = vec3(1.0f, 0.5f, 0.5f);
    } else if (powerup.Type == "incr") {
        _paddle->Size.x += 50;
    } else if (powerup.Type == "stop") {
        _ball->Stop = true;
        _effect->TimeStop = true;
    } else if (powerup.Type == "confuse") {
        if (!_effect->Chaos) {
            _effect->Confuse = true;
        }
    } else if (powerup.Type == "chaos") {
        if (!_effect->Confuse) {
            _effect->Chaos= true;
        }
    } else if (powerup.Type == "life") {
        _lives += 1;
    }
}

void
Game::UpdatePowerUps(float delta) {
    for (auto& iter: _powerups) {
        iter.Position += iter.Velocity * delta;
        if (iter.Activated) {
            iter.Duration -= delta;

            if (iter.Duration <= 0.0f) {
                iter.Activated = false;
                if (iter.Type == "sticky") {
                    if (!IsOtherPowerUpActivate(iter)) {
                        _ball->Sticky = false;
                        _paddle->Color = vec3(1.0f);
                    }
                } else if (iter.Type == "speed") {
                    _ball->Velocity /= 1.2f;
                    if (!IsOtherPowerUpActivate(iter)) {
                        _ball->Color = vec3(1.0f);
                    }
                } else if (iter.Type == "incr") {
                    _paddle->Size.x -= 50;
                } else if (iter.Type == "pass") {
                    if (!IsOtherPowerUpActivate(iter)) {
                        _ball->PassThrough = false;
                        _ball->Color = vec3(1.0f);
                    }
                } else if (iter.Type == "stop") {
                    if (!IsOtherPowerUpActivate(iter)) {
                        _ball->Stop = false;
                        _effect->TimeStop = false;
                    }
                } else if (iter.Type == "confuse") {
                    if (!IsOtherPowerUpActivate(iter)) {
                        _effect->Confuse = false;
                    }
                } else if (iter.Type == "chaos") {
                    if (!IsOtherPowerUpActivate(iter)) {
                        _effect->Chaos = false;
                    }
                }
            }
        }
    }
    _powerups.erase(
            std::remove_if(_powerups.begin(), _powerups.end(),
                [](const PowerUp& i) {return i.Destroyed && !i.Activated;}), _powerups.end());
}

bool
Game::IsOtherPowerUpActivate(PowerUp& current) {
    for (const auto& iter: _powerups) {
        if (iter.Activated && iter.Type == current.Type) {
            return true;
        }
    }
    return false;
}

bool
Game::IsWin() {
    return _win_num == LEVELS.size()-1;
}

