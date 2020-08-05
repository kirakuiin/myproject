// gameobj.cc -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Jul 28
// License: GPL.v3

#include "gameobj.h"
#include "include/commondef.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "manager.h"

GameObject::GameObject(vec2 pos, vec2 size, shared_ptr<Texture2D> text,
                       vec3 color, vec2 velocity)
    : Position(pos), Size(size), Velocity(velocity), Color(color), Sprite(text){
};

void
GameObject::Draw(shared_ptr<SpriteRender> render) {
    render->DrawSprite(Sprite, Position, Rotation, Size, Color);
}

BallObject::BallObject(vec2 pos, shared_ptr<Texture2D> text,
                       float radius, vec2 velocity)
    : GameObject(pos, vec2(radius*2), text, vec3(1.0f), velocity),
      Radius(radius) {
}


vec2
BallObject::Move(float delta, unsigned window_width) {
    if (!IsStuck) {
        Position += Velocity * delta;
        if (Position.x <= 0.0f) {
            Velocity.x *= -1;
            Position.x = 0.0f;
        } else if (Position.x + Size.x >= window_width) {
            Velocity.x *= -1;
            Position.x = window_width - Size.x;
        }
        if (Position.y <= 0.0f) {
            Velocity.y *= -1;
            Position.y = 0.0f;
        }
    }
    return Position;
}

void
BallObject::Reset(vec2 position, vec2 velocity) {
    Position = position;
    Velocity = velocity;
    IsStuck = true;
}

bool CheckIn(const GameObject& one, const GameObject& two) {
    vec2 center = one.Position + vec2(one.Size.x*0.5, one.Size.y*0.5);
    bool cx = (center.x >= two.Position.x
               && center.x <= two.Position.x + two.Size.x);
    bool cy = (center.y >= two.Position.y
               && center.y <= two.Position.y + two.Size.y);
    return cx && cy;
}

// axis-aligned bounding box
bool CheckCollision(const GameObject& one, const GameObject& two) {
    bool cx = (one.Position.x + one.Size.x >= two.Position.x
               && two.Position.x + two.Size.x >= one.Position.x);
    bool cy = (one.Position.y + one.Size.y >= two.Position.y
               && two.Position.y + two.Size.y >= one.Position.y);
    return cx && cy;
}

// circle and box check
Collision CheckCollision(BallObject& one, const GameObject& two) {
    // ball in rectangle
    while (CheckIn(one, two)) {
        one.Position -= glm::normalize(one.Velocity)*one.Radius*vec2(0.1f);
    }
    vec2 ball_pos = one.Position + vec2(one.Size.x*0.5, one.Size.y*0.5);
    vec2 rec_pos = two.Position + vec2(two.Size.x*0.5, two.Size.y*0.5);
    vec2 rtb = ball_pos - rec_pos;
    vec2 P = rec_pos + glm::clamp(rtb, -two.Size*vec2(0.5), two.Size*vec2(0.5));
    vec2 difference = ball_pos - P;
    if (glm::length(difference) <= one.Radius) {
        Direction dir = VectorDirection(difference);
        return std::make_tuple(true, dir, difference);
    } else {
        return std::make_tuple(false, Direction::UP, vec2(0));
    }
}

Direction VectorDirection(vec2 target) {
    vec2 compass[] = {
        vec2(0.0f, -1.0f),   // up
        vec2(1.0f, 0.0f),   // right
        vec2(0.0f, 1.0f),  // down
        vec2(-1.0f, 0.0f),  // left
    };
    float max = 0.0f;
    int best = -1;
    for (int i = 0; i < 4; ++i) {
        float dot = glm::dot(glm::normalize(target), compass[i]);
        if (dot > max) {
            max = dot;
            best = i;
        }
    }
    return static_cast<Direction>(best);
}

enum class TileType {
    EMPTY = 0,
    SOLID,
    RED_TILE,
    GREEN_TILE,
    BLUE_TILE,
    ORANGE_TILE,
};

void
GameLevel::Load(const std::string& filepath,
                unsigned int level_width, unsigned int level_height) {
    Bricks.clear();
    int tile_type;
    std::string line;
    std::ifstream fs(filepath);
    std::vector<std::vector<int>> tile_data;
    if (fs) {
        while (std::getline(fs, line)) {
            std::istringstream ss(line);
            std::vector<int> row;
            while (ss >> tile_type) {
                row.push_back(tile_type);
            }
            tile_data.push_back(row);
        }
        if (tile_data.size() > 0) {
            Init(tile_data, level_width, level_height);
        }
    } else {
        throw "Error: " + filepath + " not exists!";
    }
}

void
GameLevel::Draw(shared_ptr<SpriteRender> render) {
    for (auto& iter: Bricks) {
        if (!iter.Destroyed) {
            iter.Draw(render);
        }
    }
}

bool
GameLevel::IsComplete() {
    for (auto& iter: Bricks) {
        if (!iter.Destroyed && !iter.IsSolid) {
            return false;
        }
    }
    return true;
}

void
GameLevel::Init(const std::vector<std::vector<int>>& tile_data,
                unsigned int level_width, unsigned int level_height) {
    unsigned int height = tile_data.size();
    unsigned int width = tile_data[0].size();
    float unit_width = level_width / static_cast<float>(width);
    float unit_height = level_height / static_cast<float>(height);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (tile_data[y][x] != static_cast<int>(TileType::EMPTY)) {
                vec2 pos(unit_width * x, unit_height * y);
                vec2 size(unit_width, unit_height);
                GameObject obj(pos, size,
                        ResourceManager::GetTexture("block"),
                        vec3(1.0f));
                obj.IsSolid = false;
                switch (static_cast<TileType>(tile_data[y][x])) {
                    case TileType::SOLID:
                        obj.IsSolid = true;
                        obj.Color = vec3(0.8f, 0.8f, 0.7f);
                        obj.Sprite=(ResourceManager::GetTexture("block_solid"));
                        break;
                    case TileType::RED_TILE:
                        obj.Color = vec3(1.0f, 0.2f, 0.2f);
                        break;
                    case TileType::GREEN_TILE:
                        obj.Color = vec3(0.2f, 1.0f, 0.2f);
                        break;
                    case TileType::BLUE_TILE:
                        obj.Color = vec3(0.2f, 0.2f, 1.0f);
                        break;
                    case TileType::ORANGE_TILE:
                        obj.Color = vec3(1.0f, 165.0f/255.0f, 0.0f);
                        break;
                    default:
                        break;
                }
                Bricks.push_back(obj);
            }
        }
    }
}

