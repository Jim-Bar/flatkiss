#include "animation_player.hpp"

using std::string;
using std::to_string;
using std::unordered_map;
using std::vector;

AnimationPlayer::AnimationPlayer(
    unordered_map<uint16_t, Animation const>&& animations_per_tile_index) {
  this->animations_per_tile_index_.swap(animations_per_tile_index);
}

uint16_t AnimationPlayer::animatedTileIndexFor(uint16_t tile_index,
                                               int64_t tick) const {
  if (!animations_per_tile_index_.contains(tile_index)) {
    return tile_index;
  }

  Animation const& animation{animations_per_tile_index_.at(tile_index)};
  return animation.tileIndexAtStep(
      (tick % (animation.getPeriod() * animation.getDuration())) /
      animation.getDuration());
}

int64_t AnimationPlayer::animationDurationForTileIndex(
    uint16_t tile_index) const {
  if (!animations_per_tile_index_.contains(tile_index)) {
    return 0;
  }

  return animations_per_tile_index_.at(tile_index).getDuration();
}

AnimationPlayerLoader::AnimationPlayerLoader(int64_t count,
                                             string animations_files_directory,
                                             string animations_files_prefix,
                                             string animations_files_suffix)
    : count_{count},
      animations_files_directory_{animations_files_directory},
      animations_files_prefix_{animations_files_prefix},
      animations_files_suffix_{animations_files_suffix} {}

string AnimationPlayerLoader::animationsFilePath(int64_t index) const {
  return animations_files_directory_ + "/" + animations_files_prefix_ +
         to_string(index) + animations_files_suffix_;
  // FIXME: Use std::filesystem::path as below.
  /*return (path{animations_files_directory_} /
          path{animations_files_prefix_ + to_string(index)
     + animations_files_suffix_}) .string();*/
}

vector<AnimationPlayer> AnimationPlayerLoader::load() {
  vector<AnimationPlayer> animation_players;
  for (int64_t i{0}; i < count_; i++) {
    animation_players.push_back(AnimationLoader::load(animationsFilePath(i)));
  }

  return animation_players;
}
