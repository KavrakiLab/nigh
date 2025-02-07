// Software License Agreement (BSD-3-Clause)
//
// Copyright 2018 The University of North Carolina at Chapel Hill
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above
//    copyright notice, this list of conditions and the following
//    disclaimer in the documentation and/or other materials provided
//    with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
//    contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.

//! @author Jeff Ichnowski

#pragma once
#include <type_traits>
#ifndef NIGH_IMPL_BOX_DISTANCE_HPP
#define NIGH_IMPL_BOX_DISTANCE_HPP

#include <Eigen/Dense>

namespace unc::robotics::nigh::impl
{
    template <int p, typename Box, typename Key>
    auto boxDistance(const Eigen::MatrixBase<Box> &min, const Eigen::MatrixBase<Box> &max,
                     const Eigen::MatrixBase<Key> &key)
    {
        return (min - key).cwiseMax(key - max).cwiseMax(0).template lpNorm<p>();
    }

    // HACK: gross hack for our custom type
    template <int p, typename Box, typename Key,
              typename = std::enable_if_t<not std::is_base_of_v<Eigen::MatrixBase<Key>, Key>, bool>>
    auto boxDistance(const Eigen::MatrixBase<Box> &min, const Eigen::MatrixBase<Box> &max, const Key &key)
    {
        const auto &ek = Eigen::Map<Eigen::Matrix<float, Eigen::MatrixBase<Box>::RowsAtCompileTime, 1>>(key.v);
        return (min - ek).cwiseMax(ek - max).cwiseMax(0).template lpNorm<p>();
    }

    template <int p, typename Box, typename Key>
    auto boxDistance(const Eigen::MatrixBase<Box> &min, const Eigen::MatrixBase<Box> &max,
                     const Eigen::ArrayBase<Key> &key)
    {
        return boxDistance(min, max, key.matrix());
    }

    template <int p, typename Box, typename Key = float *>
    auto boxDistance(const Eigen::MatrixBase<Box> &min, const Eigen::MatrixBase<Box> &max, float *key)
    {
        const auto &ek = Eigen::Map<Eigen::Matrix<float, Eigen::MatrixBase<Box>::RowsAtCompileTime, 1>>(key);
        return (min - ek).cwiseMax(ek - max).cwiseMax(0).template lpNorm<p>();
    }
}  // namespace unc::robotics::nigh::impl

#endif
