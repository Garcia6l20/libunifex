/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once

namespace unifex::net
{
  namespace open_socket_
  {
    const inline struct fn_ {
      template <typename Scheduler>
      auto operator()(
          Scheduler& scheduler, int domain, int type, int protocol = 0) const
          noexcept(
              unifex::
                  is_nothrow_tag_invocable_v<fn_, Scheduler&, int, int, int>) {
        return unifex::tag_invoke(*this, scheduler, domain, type, protocol);
      }
    } open_socket;
  }  // namespace open_socket_
  using open_socket_::open_socket;

  namespace async_send_to_
  {
    const inline struct fn_ {
      template <typename Socket, typename Endpoint, typename BufferSequence>
      auto operator()(
          Socket& socket,
          Endpoint&& endpoint,
          BufferSequence&& bufferSequence) const
          noexcept(unifex::is_nothrow_tag_invocable_v<
                   fn_,
                   Socket&,
                   Endpoint,
                   BufferSequence>) {
        return unifex::tag_invoke(
            *this,
            socket,
            std::forward<Endpoint>(endpoint),
            std::forward<BufferSequence>(bufferSequence));
      }
    } async_send_to;
  }  // namespace async_send_to_
  using async_send_to_::async_send_to;

  namespace async_receive_from_
  {
    const inline struct fn_ {
      template <typename Socket, typename BufferSequence>
      auto operator()(Socket& socket, BufferSequence&& bufferSequence) const
          noexcept(
              unifex::
                  is_nothrow_tag_invocable_v<fn_, Socket&, BufferSequence>) {
        return unifex::tag_invoke(*this, socket, bufferSequence);
      }
    } async_receive_from;
  }  // namespace async_receive_from_
  using async_receive_from_::async_receive_from;

  //
  //  G6_CPO_DEF(open_socket, _net_cpo)
  //  G6_CPO_DEF(async_accept, _net_cpo)
  //  G6_CPO_DEF(async_connect, _net_cpo)
  //  G6_CPO_DEF(async_send, _net_cpo)
  //  G6_CPO_DEF(async_send_to, _net_cpo)
  //  G6_CPO_DEF(async_recv, _net_cpo)
  //  G6_CPO_DEF(async_recv_from, _net_cpo)
  //
  //  G6_CPO_DEF(make_server, _net_cpo)
}  // namespace unifex::net
