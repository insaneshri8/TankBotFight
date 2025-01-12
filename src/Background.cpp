#include "Background.hpp"

#include <random>
#include <string>

#include "Files.hpp"

using namespace std::string_literals;
Background::Background(TextureStore& store) : mTextureStore(store), mGround(M_HEIGHT) {
  auto ground_types = generate_random_ground();
  for (int i = 0; i < M_HEIGHT; ++i) {
    for (int j = 0; j < M_WIDTH; ++j) {
      mGround[i].emplace_back(get_texture(ground_types, i, j), ground_types[i][j]);
    }
  }
}

sf::Texture& Background::get_texture(const Background::GroundTypeVec& v, int x, int y) {
  if (v[x][y] == GroundType::Sand) {
    if (x - 1 > 0 && v[x - 1][y] == GroundType::Grass) {
      return mTextureStore.get_texture("tileGrass_transitionS.png");
    } else if (y - 1 > 0 && v[x][y - 1] == GroundType::Grass) {
      return mTextureStore.get_texture("tileGrass_transitionE.png");
    } else if (x + 1 < M_HEIGHT && v[x + 1][y] == GroundType::Grass) {
      return mTextureStore.get_texture("tileGrass_transitionN.png");
    } else if (y + 1 < M_WIDTH && v[x][y + 1] == GroundType::Grass) {
      return mTextureStore.get_texture("tileGrass_transitionW.png");
    }
    return mTextureStore.get_texture(one_of("tileSand1.png"s, "tileSand2.png"s));
  }
  return mTextureStore.get_texture(one_of("tileGrass1.png"s, "tileGrass2.png"s));
}

Background::GroundTypeVec Background::generate_random_ground() {
  Background::GroundTypeVec ground_types(M_HEIGHT);
  for (int i = 0; i < M_HEIGHT; ++i) {
    ground_types[i] = std::vector(M_WIDTH, GroundType::Grass);
  }

  const bool is_vertical = static_cast<bool>(random_range(0, 1));
  const bool sand_above_number = static_cast<bool>(random_range(0, 1));

  int start_x = 0;
  int start_y = 0;
  int end_x = M_HEIGHT;
  int end_y = M_WIDTH;

  if (is_vertical) {
    if (sand_above_number) {
      start_x = random_range(0, M_HEIGHT - 1);
    } else {
      end_x = random_range(0, M_HEIGHT - 1);
    }
  } else {
    std::uniform_int_distribution<> rand_int(0, M_WIDTH - 1);
    if (sand_above_number) {
      start_y = random_range(0, M_WIDTH - 1);
    } else {
      end_y = random_range(0, M_WIDTH - 1);
    }
  }
  fill_with_sand(ground_types, start_x, start_y, end_x, end_y);
  return ground_types;
}

void Background::fill_with_sand(GroundTypeVec& ground_types, int start_x, int start_y, int end_x,
                                int end_y) {
  for (int i = start_x; i < end_x; ++i) {
    for (int j = start_y; j < end_y; ++j) {
      ground_types[i][j] = GroundType::Sand;
    }
  }
}

void Background::draw(sf::RenderWindow& window) {
  for (int i = 0; i < M_HEIGHT; ++i) {
    for (int j = 0; j < M_WIDTH; ++j) {
      mGround[i][j].draw(window, j * GROUND_HEIGHT, i * GROUND_WIDTH);
    }
  }
}
