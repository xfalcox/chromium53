// Copyright (c) 2017-2018 LG Electronics, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// SPDX-License-Identifier: Apache-2.0

#ifndef MEDIA_WEBOS_BASE_MEDIA_APIS_WRAPPER_GMP_H_
#define MEDIA_WEBOS_BASE_MEDIA_APIS_WRAPPER_GMP_H_

#include "media/webos/base/media_apis_wrapper.h"

#include <glib.h>
#include <gmp/PlayerTypes.h>

namespace gmp {
namespace player {
class MediaPlayerClient;
}
}

namespace base {
class SingleThreadTaskRunner;
}

namespace media {

class MEDIA_EXPORT MediaAPIsWrapperGMP : public MediaAPIsWrapper {
public:
  static void CheckCodecInfo(const std::string& codec_info);

  static void Callback(const gint type, const gint64 num_value,
                       const gchar* str_value, void* user_data);

  MediaAPIsWrapperGMP(
      const scoped_refptr<base::SingleThreadTaskRunner>& task_runner,
      bool video,
      const std::string& app_id,
      const PipelineStatusCB& error_cb);

  virtual ~MediaAPIsWrapperGMP();

  void Initialize(const AudioDecoderConfig& audio_config,
                  const VideoDecoderConfig& video_config,
                  const PipelineStatusCB& init_cb) override;

  void SetDisplayWindow(const gfx::Rect& rect,
                        const gfx::Rect& in_rect,
                        bool fullscreen,
                        bool forced) override;

  bool Loaded() override;
  bool Feed(const scoped_refptr<DecoderBuffer>& buffer, FeedType type) override;
  uint64_t GetCurrentTime() override;
  bool Seek(base::TimeDelta time) override;
  void SetPlaybackRate(float playback_rate) override;
  void Suspend() override;
  void Resume(base::TimeDelta paused_time,
              RestorePlaybackMode restore_playback_mode) override;
  bool IsReleasedMediaResource() override;
  bool AllowedFeedVideo() override;
  bool AllowedFeedAudio() override;
  void Finalize() override;

  void SetPlaybackVolume(double volume) override;
  bool IsEOSReceived() override;
  void SetSizeChangeCb(const base::Closure& size_change_cb) override;

 private:
  void DispatchCallback(const gint type, const gint64 num_value,
                        const std::string& str_value);

  bool LoadFeed(int64_t pts);
  bool PlayFeed();
  bool PauseFeed();
  void ResetFeed();
  void PushEOS();

  bool HasResources();
  bool IsSuspended();
  void SetVolumeInternal(double volume);
  void NotifyLoadComplete();

  FeedStatus FeedInternal(const scoped_refptr<DecoderBuffer>& buffer,
                          FeedType type);
  void UpdateVideoInfo(const std::string& info_str);

  bool MakeLoadData(int64_t time, MEDIA_LOAD_DATA_T* load_data);

  base::Closure size_change_cb_;

  gfx::Size natural_size_;

  std::unique_ptr<gmp::player::MediaPlayerClient> media_player_client_;

  DISALLOW_COPY_AND_ASSIGN(MediaAPIsWrapperGMP);
};

}  // namespace media

#endif  // MEDIA_WEBOS_BASE_MEDIA_APIS_WRAPPER_GMP_H_