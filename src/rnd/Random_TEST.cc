/*
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * - Neither the name of prim nor the names of its contributors may be used to
 * endorse or promote products derived from this software without specific prior
 * written permission.
 *
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include "rnd/Random.h"

#include <cmath>
#include <ctime>
#include <deque>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "gtest/gtest.h"
#include "prim/prim.h"

TEST(Random, seed) {
  const u64 kRands = 1000;

  // Initializes a bunch of Random objects.
  std::vector<rnd::Random*> rands;
  for (u64 i = 0; i < kRands; i++) {
    rands.push_back(new rnd::Random((i + 1) << 32));
  }

  // Gets the first value of each random.
  std::vector<u64> values;
  for (u64 i = 0; i < kRands; i++) {
    values.push_back(rands[i]->nextU64(0, U64_MAX));
  }

  // Re-seeds each random, verifies the random produces the same value.
  for (u64 n = 0; n < 5; n++) {
    for (u64 i = 0; i < kRands; i++) {
      rands[i]->seed((i + 1) << 32);
      u64 value = rands.at(i)->nextU64(0, U64_MAX);
      ASSERT_EQ(value, values.at(i));
    }
  }

  // Cleans up the Random objects.
  for (u64 i = 0; i < kRands; i++) {
    delete rands[i];
  }
}

TEST(Random, u64) {
  const u64 kBkts = 1000;
  const u64 kRounds = 10000000;
  const u64 kTests = 4;
  for (u64 test = 0; test < kTests; test++) {
    std::vector<u64> buckets(kBkts, 0);
    u64 seed = 0xDEADBEEF12345678lu + test;
    rnd::Random rand(seed);

    for (u64 r = 0; r < kRounds; r++) {
      u64 value = rand.nextU64(1234, kBkts + 1234 - 1);
      buckets.at(value - 1234)++;
    }

    f64 sum = 0;
    for (u64 b = 0; b < kBkts; b++) {
      sum += buckets.at(b);
    }
    f64 mean = sum / kBkts;

    sum = 0;
    for (u64 b = 0; b < kBkts; b++) {
      f64 c = (static_cast<f64>(buckets.at(b)) - mean);
      c *= c;
      sum += c;
    }
    f64 std_dev = std::sqrt(sum / kBkts);
    std_dev /= kRounds;
    ASSERT_LE(std_dev, 0.00009);
  }
}

TEST(Random, bits_range) {
  const u64 kTests = 10;
  const u64 kRounds = 100000;
  for (u64 test = 0; test < kTests; test++) {
    u64 seed = 0xDEADBEEF12345678lu + test;
    for (u32 bits = 1; bits <= 64; bits++) {
      rnd::Random rand(seed);
      for (u64 r = 0; r < kRounds; r++) {
        u64 value = rand.nextU64(bits);
        ASSERT_LE(value, 0x1 << bits);
      }
    }
  }
}

TEST(Random, bits_8b_dist) {
  const u64 kBits = 8;
  const u64 kTests = 10;
  const u64 kRounds = 10000000;
  for (u64 test = 0; test < kTests; test++) {
    u64 seed = 0xDEADBEEF12345678lu + test;
    rnd::Random rand(seed);
    std::vector<u64> counts(0x1 << kBits, 0);
    for (u64 r = 0; r < kRounds; r++) {
      u64 value = rand.nextU64(kBits);
      counts.at(value) += 1;
    }
    f64 exp_ratio = (static_cast<f64>(kRounds) / (0x1 << kBits)) / kRounds;
    for (u64 count : counts) {
      f64 act_ratio = static_cast<f64>(count) / kRounds;
      ASSERT_NEAR(act_ratio, exp_ratio, 0.0001);
    }
  }
}

TEST(Random, f64) {
  const u64 kBkts = 1000;
  const u64 kRounds = 10000000;
  std::vector<u64> buckets(kBkts, 0);
  rnd::Random rand(0xDEADBEEF12345678lu);

  for (u64 r = 0; r < kRounds; r++) {
    f64 value = rand.nextF64(0, 1000);
    buckets.at(static_cast<u64>(value))++;
  }

  f64 sum = 0;
  for (u64 b = 0; b < kBkts; b++) {
    sum += buckets.at(b);
  }
  f64 mean = sum / kBkts;

  sum = 0;
  for (u64 b = 0; b < kBkts; b++) {
    f64 c = (static_cast<f64>(buckets.at(b)) - mean);
    c *= c;
    sum += c;
  }
  f64 std_dev = std::sqrt(sum / kBkts);
  std_dev /= kRounds;
  ASSERT_LE(std_dev, 0.00009);
}

TEST(Random, bool) {
  const u64 kTests = 1;
  const u64 kRounds = 1000000000;

  for (u64 test = 0; test < kTests; test++) {
    // Uses a non-deterministic RNG to seed our deterministic PRNG.
    srand(time(nullptr));

    // Gets a seed for the PRNG.
    u64 seed = 0;
    for (u64 b = 0; b < sizeof(u64); b++) {
      u64 byte = rand() % 0xFF;  // NOLINT
      seed = (seed << 8) | byte;
    }

    rnd::Random rand(seed);

    u64 count = 0;
    for (u64 r = 0; r < kRounds; r++) {
      if (rand.nextBool()) {
        count++;
      }
    }

    f64 ratio = static_cast<f64>(count) / kRounds;
    f64 dev = std::abs(0.5 - ratio);
    ASSERT_LE(dev, 0.0001);
  }
}

TEST(Random, shuffle) {
  const u64 kSize = 100;
  const u64 kRounds = 1000000;

  // Makes base vector.
  std::vector<u64> a;
  for (u64 idx = 0; idx < kSize; idx++) {
    a.push_back(idx);
  }

  // Tests seeding reproduces same shuffle.
  rnd::Random rand(123);
  std::vector<u64> b = a;
  std::vector<u64> c = a;
  rand.shuffle(b.begin(), b.end());  // iterator version
  rand.seed(123);
  rand.shuffle(&c);  // container version
  ASSERT_EQ(b, c);

  // Generates distribution.
  std::vector<u64> buckets(kSize, 0);
  for (u32 round = 0; round < kRounds; round++) {
    // Makes a copy.
    std::vector<u64> d = a;

    // Shuffles the copy.
    if (round % 2 == 0) {
      rand.shuffle(d.begin(), d.end());
    } else {
      rand.shuffle(&d);
    }

    // Finds where element 0 landed.
    bool found = false;
    for (u64 idx = 0; idx < kSize; idx++) {
      if (d[idx] == 0) {
        buckets.at(idx)++;
        found = true;
        break;
      }
    }
    ASSERT_TRUE(found);
  }

  // Verifies distribution is uniform.
  f64 sum = 0;
  for (u64 b = 0; b < kSize; b++) {
    sum += buckets.at(b);
  }
  f64 mean = sum / kSize;
  sum = 0;
  for (u64 b = 0; b < kSize; b++) {
    f64 c = (static_cast<f64>(buckets.at(b)) - mean);
    c *= c;
    sum += c;
  }
  f64 std_dev = std::sqrt(sum / kSize);
  std_dev /= kRounds;
  ASSERT_LE(std_dev, 0.00015);
}

TEST(Random, retrieve) {
  rnd::Random rnd(12345678);

  const std::set<u32> kSingleVals({1, 2, 3, 4});
  const std::set<std::pair<u32, u32> > kPairVals(
      {{1, 1}, {2, 2}, {3, 3}, {4, 4}});

  std::deque<u32> deque({1, 2, 3, 4});
  std::list<u32> list({1, 2, 3, 4});
  std::vector<u32> vector({1, 2, 3, 4});
  std::set<u32> oset({1, 2, 3, 4});
  std::map<u32, u32> omap({{1, 1}, {2, 2}, {3, 3}, {4, 4}});
  std::unordered_set<u32> uset({1, 2, 3, 4});
  std::unordered_map<u32, u32> umap({{1, 1}, {2, 2}, {3, 3}, {4, 4}});

  u32 r;
  std::pair<u32, u32> p;

  r = rnd.retrieve(&deque);
  ASSERT_EQ(kSingleVals.count(r), 1u);
  ASSERT_EQ(deque.size(), 4u);

  r = rnd.retrieve(&list);
  ASSERT_EQ(kSingleVals.count(r), 1u);
  ASSERT_EQ(list.size(), 4u);

  r = rnd.retrieve(&vector);
  ASSERT_EQ(kSingleVals.count(r), 1u);
  ASSERT_EQ(vector.size(), 4u);

  r = rnd.retrieve(&oset);
  ASSERT_EQ(kSingleVals.count(r), 1u);
  ASSERT_EQ(oset.size(), 4u);

  p = rnd.retrieve(&omap);
  ASSERT_EQ(kPairVals.count(p), 1u);
  ASSERT_EQ(omap.size(), 4u);

  r = rnd.retrieve(&uset);
  ASSERT_EQ(kSingleVals.count(r), 1u);
  ASSERT_EQ(uset.size(), 4u);

  p = rnd.retrieve(&umap);
  ASSERT_EQ(kPairVals.count(p), 1u);
  ASSERT_EQ(umap.size(), 4u);
}

TEST(Random, remove) {
  rnd::Random rnd(12345678);

  const std::set<u32> kSingleVals({1, 2, 3, 4});
  const std::set<std::pair<u32, u32> > kPairVals(
      {{1, 1}, {2, 2}, {3, 3}, {4, 4}});

  std::deque<u32> deque({1, 2, 3, 4});
  std::list<u32> list({1, 2, 3, 4});
  std::vector<u32> vector({1, 2, 3, 4});
  std::set<u32> oset({1, 2, 3, 4});
  std::map<u32, u32> omap({{1, 1}, {2, 2}, {3, 3}, {4, 4}});
  std::unordered_set<u32> uset({1, 2, 3, 4});
  std::unordered_map<u32, u32> umap({{1, 1}, {2, 2}, {3, 3}, {4, 4}});

  u32 r;
  std::pair<u32, u32> p;

  r = rnd.remove(&deque);
  ASSERT_EQ(kSingleVals.count(r), 1u);
  ASSERT_EQ(deque.size(), 3u);

  r = rnd.remove(&list);
  ASSERT_EQ(kSingleVals.count(r), 1u);
  ASSERT_EQ(list.size(), 3u);

  r = rnd.remove(&vector);
  ASSERT_EQ(kSingleVals.count(r), 1u);
  ASSERT_EQ(vector.size(), 3u);

  r = rnd.remove(&oset);
  ASSERT_EQ(kSingleVals.count(r), 1u);
  ASSERT_EQ(oset.size(), 3u);

  p = rnd.remove(&omap);
  ASSERT_EQ(kPairVals.count(p), 1u);
  ASSERT_EQ(omap.size(), 3u);

  r = rnd.remove(&uset);
  ASSERT_EQ(kSingleVals.count(r), 1u);
  ASSERT_EQ(uset.size(), 3u);

  p = rnd.remove(&umap);
  ASSERT_EQ(kPairVals.count(p), 1u);
  ASSERT_EQ(umap.size(), 3u);
}

TEST(Random, removeDist) {
  rnd::Random rnd(12345678);

  const std::set<u32> kSingleVals({1, 2, 3, 4});
  std::vector<u32> counts(4, 0);

  const u32 kRounds = 1000000;
  for (u32 r = 0; r < kRounds; r++) {
    std::unordered_set<u32> uset({1, 2, 3, 4});
    u32 v = rnd.remove(&uset);
    ASSERT_EQ(kSingleVals.count(v), 1u);
    ASSERT_EQ(uset.size(), 3u);
    counts.at(v - 1)++;
  }

  for (u32 c = 0; c < counts.size(); c++) {
    ASSERT_NEAR(counts.at(c), kRounds / 4.0, 0.001 * kRounds);
  }
}
