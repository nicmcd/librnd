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
#ifndef RND_SET_H_
#error "Do not include this .tcc file directly, use the .h file instead"
#else

#include <algorithm>
#include <set>

namespace rnd {

template <typename T>
Set<T>::Set() {}

template <typename T>
Set<T>::Set(u64 _seed) {
  seed(_seed);
}

template <typename T>
Set<T>::~Set() {}

template <typename T>
void Set<T>::seed(u64 _seed) {
  std::seed_seq seq = {(u32)((_seed >> 32) & 0xFFFFFFFFlu),
                       (u32)((_seed >>  0) & 0xFFFFFFFFlu)};
  prng_.seed(seq);
}

template <typename T>
void Set<T>::addSequence(T _start, T _stop) {
  if (_stop >= _start) {
    T current = _start;
    while (true) {
      bool last = (current == _stop);
      values_.push_back(current);
      current++;
      if (last) {
        break;
      }
    }
    std::shuffle(values_.begin(), values_.end(), prng_);
  }
}

template <typename T>
void Set<T>::addSet(const std::set<T>& _values) {
  for (auto it = _values.cbegin(); it != _values.cend(); ++it) {
    values_.push_back(*it);
  }
  std::shuffle(values_.begin(), values_.end(), prng_);
}

template <typename T>
void Set<T>::clear() {
  values_.clear();
}

template <typename T>
size_t Set<T>::size() const {
  return values_.size();
}

template <typename T>
T Set<T>::peek() {
  return values_.back();
}

template <typename T>
T Set<T>::pop() {
  T back = values_.back();
  values_.pop_back();
  return back;
}

template <typename T>
u64 Set<T>::remove(T _item, u64 _max) {
  u64 count = 0;
  for (auto it = values_.begin();
       it != values_.end() && count < _max;
       ++it) {
    if (_item == *it) {
      values_.erase(it);
      count++;
    }
  }
  return count;
}

}  // namespace rnd

#endif  // COMMON_RANDOM_RANDOMSET_H_
