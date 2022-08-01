#include "spriteset.hpp"

#include <fstream>
#include <tuple>
#include <utility>

#include "renderer.hpp"

using std::ifstream;
using std::ios;
using std::istream;
using std::move;
using std::string;
using std::to_string;
using std::tuple;
using std::vector;

Spriteset::Spriteset(int64_t sprites_width, int64_t sprites_height,
                     int64_t width_in_sprites, int64_t height_in_sprites,
                     int64_t left_offset, int64_t top_offset, int64_t gap)
    : sprites_width_{sprites_width},
      sprites_height_{sprites_height},
      width_in_sprites_{width_in_sprites},
      height_in_sprites_{height_in_sprites},
      left_offset_{left_offset},
      top_offset_{top_offset},
      gap_{gap} {}

int64_t Spriteset::gap() const { return gap_; }

int64_t Spriteset::heightInSprites() const { return height_in_sprites_; }

int64_t Spriteset::leftOffset() const { return left_offset_; }

SDL_Rect Spriteset::rectForSpriteIndex(int64_t sprite_index) const {
  SDL_Rect source_rect;
  source_rect.w = static_cast<int>(spritesWidth());
  source_rect.h = static_cast<int>(spritesHeight());

  source_rect.x = static_cast<int>((sprite_index % widthInSprites()) *
                                       (source_rect.w + gap()) +
                                   leftOffset());
  source_rect.y = static_cast<int>((sprite_index / widthInSprites()) *
                                       (source_rect.h + gap()) +
                                   topOffset());

  return source_rect;
}

int64_t Spriteset::spritesHeight() const { return sprites_height_; }

int64_t Spriteset::spritesWidth() const { return sprites_width_; }

int64_t Spriteset::topOffset() const { return top_offset_; }

int64_t Spriteset::widthInSprites() const { return width_in_sprites_; }

CharactersetLoader::CharactersetLoader(string texture_files_directory,
                                       string texture_files_prefix,
                                       string texture_files_suffix)
    : texture_files_directory_{texture_files_directory},
      texture_files_prefix_{texture_files_prefix},
      texture_files_suffix_{texture_files_suffix} {}

string CharactersetLoader::texturePath(int64_t texture_index) const {
  return texture_files_directory_ + "/" + texture_files_prefix_ +
         to_string(texture_index) + texture_files_suffix_;
  // FIXME: Use std::filesystem::path as below.
  /*return (path{texture_files_directory_} /
          path{texture_files_prefix_ + to_string(texture_index) +
               texture_files_suffix_})
      .string();*/
}

tuple<vector<Spriteset>, vector<Texture>> CharactersetLoader::load(
    string const& file_path, Renderer const& renderer) {
  vector<Spriteset> charactersets;
  vector<Texture> textures;
  ifstream stream;
  stream.open(file_path, ios::in | ios::binary);
  if (stream.is_open()) {
    int64_t characterset_count{0};
    while (stream.peek() != istream::traits_type::eof()) {
      // FIXME: Use stream.get() for uint8_t variables.
      uint8_t sprites_width{0};
      uint8_t sprites_height{0};
      uint16_t width_in_sprites{0};
      uint16_t height_in_tiles{0};
      uint16_t top_offset{0};
      uint16_t left_offset{0};
      uint16_t gap{0};
      uint8_t alpha_red{0};
      uint8_t alpha_green{0};
      uint8_t alpha_blue{0};
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&sprites_width), 1);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&sprites_height), 1);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&width_in_sprites), 2);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&height_in_tiles), 2);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&top_offset), 2);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&left_offset), 2);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&gap), 2);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&alpha_red), 1);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&alpha_green), 1);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&alpha_blue), 1);
      charactersets.emplace_back(sprites_width, sprites_height,
                                 width_in_sprites, height_in_tiles, left_offset,
                                 top_offset, gap);
      textures.emplace_back(texturePath(characterset_count), alpha_red,
                            alpha_green, alpha_blue, renderer);
      characterset_count++;
    }
    stream.close();
  }  // FIXME: Raise exception.

  return {move(charactersets), move(textures)};
}
