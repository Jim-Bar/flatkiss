#include "characterset.hpp"

#include <fstream>

#include "renderer.hpp"

using std::ifstream;
using std::ios;
using std::istream;

using std::string;
using std::vector;

Characterset::Characterset(
    string const& file_path, int64_t sprites_width, int64_t sprites_height,
    int64_t width_in_sprites, int64_t height_in_sprites, int64_t left_offset,
    int64_t top_offset, int64_t gap, uint8_t alpha_red, uint8_t alpha_green,
    uint8_t alpha_blue, int64_t sprite_move_left_index,
    int64_t sprite_move_down_index, int64_t sprite_move_right_index,
    int64_t sprite_move_up_index, Renderer const& renderer)
    : sprites_width_{sprites_width},
      sprites_height_{sprites_height},
      width_in_sprites_{width_in_sprites},
      height_in_sprites_{height_in_sprites},
      left_offset_{left_offset},
      top_offset_{top_offset},
      gap_{gap},
      sprites_move_directions_indices_{
          sprite_move_left_index, sprite_move_down_index,
          sprite_move_right_index, sprite_move_up_index},
      texture_{Characterset::loadTexture(file_path, renderer, alpha_red,
                                         alpha_green, alpha_blue)} {}

Characterset::~Characterset() { SDL_DestroyTexture(texture_); }

int64_t Characterset::gap() const { return gap_; }

int64_t Characterset::heightInSprites() const { return height_in_sprites_; }

int64_t Characterset::leftOffset() const { return left_offset_; }

SDL_Texture* Characterset::loadTexture(string const& file_path,
                                       Renderer const& renderer,
                                       uint8_t alpha_red, uint8_t alpha_green,
                                       uint8_t alpha_blue) {
  SDL_Surface* surface = SDL_LoadBMP(file_path.c_str());
  SDL_Texture* texture = nullptr;
  if (surface != nullptr) {
    SDL_SetColorKey(
        surface, SDL_TRUE,
        SDL_MapRGB(surface->format, alpha_red, alpha_green, alpha_blue));
    texture = renderer.createTextureFromSurface(
        surface);  // TODO: Check nullity / raise exception.
    SDL_FreeSurface(surface);
  }  // TODO: Raise exception.

  return texture;
}

SDL_Rect Characterset::rectForMoveDirection(
    MoveDirection const& move_direction) const {
  // FIXME: Are you sure you want to use enum values as indices?
  return rectForSpriteIndex(
      sprites_move_directions_indices_.at(move_direction));
}

SDL_Rect Characterset::rectForSpriteIndex(int64_t sprite_index) const {
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

int64_t Characterset::spritesHeight() const { return sprites_height_; }

int64_t Characterset::spritesWidth() const { return sprites_width_; }

SDL_Texture* Characterset::texture() const { return texture_; }

int64_t Characterset::topOffset() const { return top_offset_; }

int64_t Characterset::widthInSprites() const { return width_in_sprites_; }

vector<Characterset> CharactersetLoader::load(
    std::string const& file_path, Configuration const& configuration,
    Renderer const& renderer) {
  vector<Characterset> charactersets;
  ifstream stream;
  stream.open(file_path, ios::in | ios::binary);
  if (stream.is_open()) {
    int64_t characterset_count{0};
    while (stream.peek() != istream::traits_type::eof()) {
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
      uint16_t move_left_index{0};
      uint16_t move_down_index{0};
      uint16_t move_right_index{0};
      uint16_t move_up_index{0};
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
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&move_left_index), 2);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&move_down_index), 2);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&move_right_index), 2);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&move_up_index), 2);
      charactersets.emplace_back(
          configuration.charactersetPath(characterset_count), sprites_width,
          sprites_height, width_in_sprites, height_in_tiles, left_offset,
          top_offset, gap, alpha_red, alpha_green, alpha_blue, move_left_index,
          move_down_index, move_right_index, move_up_index, renderer);
      characterset_count++;
    }
    stream.close();
  }  // FIXME: Raise exception.

  return charactersets;
}
