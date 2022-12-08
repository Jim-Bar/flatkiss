#include <libflatkiss/media/texture_atlas.hpp>

using std::forward_as_tuple;
using std::piecewise_construct;
using std::string;
using std::to_string;
using std::unordered_map;
using std::vector;

TextureAtlas::TextureAtlas(std::vector<Spriteset> const& spritesets,
                           Renderer const& renderer,
                           std::string texture_files_directory,
                           std::string texture_files_prefix,
                           std::string texture_files_suffix)
    : textures_{
          TextureAtlas::load(spritesets, renderer, texture_files_directory,
                             texture_files_prefix, texture_files_suffix)} {}

unordered_map<int64_t, Texture> TextureAtlas::load(
    vector<Spriteset> const& spritesets, Renderer const& renderer,
    std::string texture_files_directory, std::string texture_files_prefix,
    std::string texture_files_suffix) {
  unordered_map<int64_t, Texture> textures_by_indices;

  for (Spriteset const& spriteset : spritesets) {
    textures_by_indices.emplace(
        piecewise_construct, forward_as_tuple(spriteset.textureIndex()),
        forward_as_tuple(
            texturePath(spriteset.textureIndex(), texture_files_directory,
                        texture_files_prefix, texture_files_suffix),
            spriteset.alpha_red(), spriteset.alpha_green(),
            spriteset.alpha_blue(), renderer));
  }

  return textures_by_indices;
}

Texture const& TextureAtlas::texture_for_index(int64_t texture_index) const {
  return textures_.at(texture_index);
}

string TextureAtlas::texturePath(int64_t texture_index,
                                 std::string texture_files_directory,
                                 std::string texture_files_prefix,
                                 std::string texture_files_suffix) {
  return texture_files_directory + "/" + texture_files_prefix +
         to_string(texture_index) + texture_files_suffix;
  // FIXME: Use std::filesystem::path as below.
  /*return (path{texture_files_directory} /
          path{texture_files_prefix + to_string(texture_index) +
               texture_files_suffix})
      .string();*/
}
