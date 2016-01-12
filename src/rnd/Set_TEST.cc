/*
 * Copyright (c) 2012-2015, Nic McDonald
 * All rights reserved.
 *
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
 * - Neither the name of prim nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
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
#include <gtest/gtest.h>
#include <prim/prim.h>

#include <set>

#include "rnd/Set.h"

TEST(Set, u8Full) {
  bool debug = false;
  rnd::Set<u8> rs;
  rs.addSequence(0, 255);
  ASSERT_EQ(rs.size(), 256u);
  std::set<u8> set;
  for (u64 i = 0; rs.size() > 0; i++) {
    u8 val = rs.pop();
    ASSERT_TRUE(set.insert(val).second);
    if (debug) {
      printf("%lu -> %u\n", i, val);
    }
  }
  ASSERT_EQ(set.size(), 256u);
  ASSERT_EQ(rs.size(), 0u);
}

TEST(Set, s8Full) {
  bool debug = false;
  rnd::Set<s8> rs;
  rs.addSequence(-128, 127);
  ASSERT_EQ(rs.size(), 256u);
  std::set<s8> set;
  for (u64 i = 0; rs.size() > 0; i++) {
    s8 val = rs.pop();
    ASSERT_TRUE(set.insert(val).second);
    if (debug) {
      printf("%lu -> %u\n", i, val);
    }
  }
  ASSERT_EQ(set.size(), 256u);
  ASSERT_EQ(rs.size(), 0u);
}

TEST(Set, u8Partial) {
  bool debug = false;
  rnd::Set<u8> rs;
  rs.addSequence(64, 64 + 127);
  ASSERT_EQ(rs.size(), 128u);
  std::set<u8> set;
  for (u64 i = 0; rs.size() > 0; i++) {
    u8 val = rs.pop();
    ASSERT_GE(val, 64u);
    ASSERT_LE(val, 64u + 127u);
    ASSERT_TRUE(set.insert(val).second);
    if (debug) {
      printf("%lu -> %u\n", i, val);
    }
  }
  ASSERT_EQ(set.size(), 128u);
  ASSERT_EQ(rs.size(), 0u);
}

TEST(Set, s8Partial) {
  bool debug = false;
  rnd::Set<s8> rs;
  rs.addSequence(-120, -21);
  ASSERT_EQ(rs.size(), 100u);
  std::set<s8> set;
  for (u64 i = 0; rs.size() > 0; i++) {
    s8 val = rs.pop();
    ASSERT_GE(val, -120);
    ASSERT_LE(val, -21);
    ASSERT_TRUE(set.insert(val).second);
    if (debug) {
      printf("%lu -> %u\n", i, val);
    }
  }
  ASSERT_EQ(set.size(), 100u);
  ASSERT_EQ(rs.size(), 0u);
}

TEST(Set, u16Full) {
  bool debug = false;
  rnd::Set<u16> rs;
  rs.addSequence(0, 65535);
  ASSERT_EQ(rs.size(), 65536u);
  std::set<u16> set;
  for (u64 i = 0; rs.size() > 0; i++) {
    u16 val = rs.pop();
    ASSERT_TRUE(set.insert(val).second);
    if (debug) {
      printf("%lu -> %u\n", i, val);
    }
  }
  ASSERT_EQ(set.size(), 65536u);
  ASSERT_EQ(rs.size(), 0u);
}

TEST(Set, u32Single) {
  rnd::Set<u32> rs;
  rs.addSequence(1000, 1000);
  ASSERT_EQ(rs.size(), 1u);
  u32 val = rs.pop();
  ASSERT_EQ(val, 1000u);
  ASSERT_EQ(rs.size(), 0u);
}

TEST(Set, u64Partial) {
  bool debug = false;
  rnd::Set<u64> rs;
  rs.addSequence(0xA00000000, 0xA00000FFF);
  ASSERT_EQ(rs.size(), 4096u);
  std::set<u64> set;
  for (u64 i = 0; rs.size() > 0; i++) {
    u64 val = rs.pop();
    ASSERT_GE(val, 0xA00000000u);
    ASSERT_LE(val, 0xA00000FFFu);
    ASSERT_TRUE(set.insert(val).second);
    if (debug) {
      printf("%lu -> %lu\n", i, val);
    }
  }
  ASSERT_EQ(set.size(), 4096u);
  ASSERT_EQ(rs.size(), 0u);
}

TEST(Set, s64Partial) {
  bool debug = false;
  rnd::Set<s64> rs;
  rs.addSequence(-100000, 99999);
  ASSERT_EQ(rs.size(), 200000u);
  std::set<s64> set;
  for (u64 i = 0; rs.size() > 0; i++) {
    s64 val = rs.pop();
    ASSERT_GE(val, -100000);
    ASSERT_LE(val, 99999);
    ASSERT_TRUE(set.insert(val).second);
    if (debug) {
      printf("%lu -> %li\n", i, val);
    }
  }
  ASSERT_EQ(set.size(), 200000u);
  ASSERT_EQ(rs.size(), 0u);
}

TEST(Set, u32Empty) {
  rnd::Set<u32> rs;
  rs.addSequence(1001, 1000);
  ASSERT_EQ(rs.size(), 0u);
}

TEST(Set, addSet) {
  rnd::Set<u32> rs;
  rs.addSequence(10, 13);
  rs.addSet(std::set<u32>({14, 15, 16}));
  ASSERT_EQ(rs.size(), 7u);
  std::set<u32> exp({10, 11, 12, 13, 14, 15, 16});
  while (rs.size() > 0) {
    ASSERT_GT(exp.size(), 0u);
    u32 cur = rs.pop();
    ASSERT_EQ(exp.count(cur), 1u);
    exp.erase(cur);
  }
  ASSERT_EQ(rs.size(), 0u);
  ASSERT_EQ(exp.size(), 0u);
}
