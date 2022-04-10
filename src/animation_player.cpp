#include "animation_player.hpp"

using std::string;
using std::to_string;
using std::unordered_map;
using std::vector;

AnimationPlayer::AnimationPlayer(
    unordered_map<uint16_t, Animation const>&& animations_per_sprite_index) {
  this->animations_per_sprite_index_.swap(animations_per_sprite_index);
}

uint16_t AnimationPlayer::animatedSpriteIndexFor(uint16_t sprite_index,
                                                 int64_t tick) const {
  if (!animations_per_sprite_index_.contains(sprite_index)) {
    return sprite_index;
  }

  Animation const& animation{animations_per_sprite_index_.at(sprite_index)};
  return animation.spriteIndexAtStep(
      (tick % (animation.getPeriod() * animation.getDuration())) /
      animation.getDuration());
}

int64_t AnimationPlayer::animationDurationForSpriteIndex(
    uint16_t sprite_index) const {
  if (!animations_per_sprite_index_.contains(sprite_index)) {
    return 0;
  }

  return animations_per_sprite_index_.at(sprite_index).getDuration();
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
