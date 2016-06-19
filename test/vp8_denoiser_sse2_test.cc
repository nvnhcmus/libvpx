/*
 *  Copyright (c) 2014 The WebM project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "third_party/googletest/src/include/gtest/gtest.h"
#include "test/acm_random.h"
#include "test/clear_system_state.h"
#include "test/register_state_check.h"
#include "test/util.h"

#include "vp8/encoder/denoising.h"
#include "vp8/common/reconinter.h"
#include "vpx/vpx_integer.h"
#include "vpx_mem/vpx_mem.h"

using libvpx_test::ACMRandom;

namespace {

const int kNumPixels = 16 * 16;
class VP8DenoiserTest : public ::testing::TestWithParam<int> {
 public:
  virtual ~VP8DenoiserTest() {}

  virtual void SetUp() {
    increase_denoising_ = GetParam();
  }

  virtual void TearDown() { libvpx_test::ClearSystemState(); }

 protected:
  int increase_denoising_;
};

TEST_P(VP8DenoiserTest, BitexactCheck) {
  ACMRandom rnd(ACMRandom::DeterministicSeed());
  const int count_test_block = 4000;
  const int stride = 16;

  // Allocate the space for input and output,
  // where sig_block_c/_sse2 is the block to be denoised,
  // mc_avg_block is the denoised reference block,
  // avg_block_c is the denoised result from C code,
  // avg_block_sse2 is the denoised result from SSE2 code.
  DECLARE_ALIGNED(16, uint8_t, sig_block_c[kNumPixels]);
  // Since in VP8 denoiser, the source signal will be changed,
  // we need another copy of the source signal as the input of sse2 code.
  DECLARE_ALIGNED(16, uint8_t, sig_block_sse2[kNumPixels]);
  DECLARE_ALIGNED(16, uint8_t, mc_avg_block[kNumPixels]);
  DECLARE_ALIGNED(16, uint8_t, avg_block_c[kNumPixels]);
  DECLARE_ALIGNED(16, uint8_t, avg_block_sse2[kNumPixels]);

  for (int i = 0; i < count_test_block; ++i) {
    // Generate random motion magnitude, 20% of which exceed the threshold.
    const int motion_magnitude_ran =
        rnd.Rand8() % static_cast<int>(MOTION_MAGNITUDE_THRESHOLD * 1.2);

    // Initialize a test block with random number in range [0, 255].
    for (int j = 0; j < kNumPixels; ++j) {
      int temp = 0;
      sig_block_sse2[j] = sig_block_c[j] = rnd.Rand8();
      // The pixels in mc_avg_block are generated by adding a random
      // number in range [-19, 19] to corresponding pixels in sig_block.
      temp = sig_block_c[j] + (rnd.Rand8() % 2 == 0 ? -1 : 1) *
             (rnd.Rand8() % 20);
      // Clip.
      mc_avg_block[j] = (temp < 0) ? 0 : ((temp > 255) ? 255 : temp);
    }

    // Test denosiser on Y component.
    ASM_REGISTER_STATE_CHECK(vp8_denoiser_filter_c(
        mc_avg_block, stride, avg_block_c, stride, sig_block_c, stride,
        motion_magnitude_ran, increase_denoising_));

    ASM_REGISTER_STATE_CHECK(vp8_denoiser_filter_sse2(
        mc_avg_block, stride, avg_block_sse2, stride, sig_block_sse2, stride,
        motion_magnitude_ran, increase_denoising_));

    // Check bitexactness.
    for (int h = 0; h < 16; ++h) {
      for (int w = 0; w < 16; ++w) {
        EXPECT_EQ(avg_block_c[h * stride + w], avg_block_sse2[h * stride + w]);
      }
    }

    // Test denoiser on UV component.
    ASM_REGISTER_STATE_CHECK(vp8_denoiser_filter_uv_c(
        mc_avg_block, stride, avg_block_c, stride, sig_block_c, stride,
        motion_magnitude_ran, increase_denoising_));

    ASM_REGISTER_STATE_CHECK(vp8_denoiser_filter_uv_sse2(
        mc_avg_block, stride, avg_block_sse2, stride, sig_block_sse2, stride,
        motion_magnitude_ran, increase_denoising_));

    // Check bitexactness.
    for (int h = 0; h < 16; ++h) {
      for (int w = 0; w < 16; ++w) {
        EXPECT_EQ(avg_block_c[h * stride + w], avg_block_sse2[h * stride + w]);
      }
    }
  }
}

// Test for all block size.
INSTANTIATE_TEST_CASE_P(SSE2, VP8DenoiserTest, ::testing::Values(0, 1));
}  // namespace
