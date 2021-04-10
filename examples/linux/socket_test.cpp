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

#include <unifex/config.hpp>

// requires both coroutine and liburing support (for now)
#if !UNIFEX_NO_COROUTINES and !UNIFEX_NO_LIBURING
#  include <unifex/linux/io_uring_context.hpp>
using io_context = unifex::linuxos::io_uring_context;

#  include <unifex/sync_wait.hpp>
#  include <unifex/task.hpp>
#  include <unifex/when_all.hpp>
#  include <sys/socket.h>

#  include <iostream>

using namespace unifex;

task<void> hello_udp(auto sched) {
  auto server = net::open_socket(sched, AF_INET, SOCK_DGRAM);
  server.bind(net::ipv4_endpoint{net::ipv4_address::loopback(), 0});
  auto server_endpoint = *server.local_endpoint();
  std::cout << "udp server listening at: " << server_endpoint.to_string()
            << '\n';
  auto client = net::open_socket(sched, AF_INET, SOCK_DGRAM);
  co_await when_all(
      [&]() -> task<void> {
        co_await net::async_send_to(
            client, server_endpoint, as_bytes(span{"Hello !!!", 9}));
      }(),
      [&]() -> task<void> {
        std::array<char, 64> buffer{};
        auto [from, bytes_received] = co_await net::async_receive_from(
            server, as_writable_bytes(span{buffer}));
        std::cout << "received " << std::to_string(bytes_received) << " bytes from '" << from.to_string() << "': "
                  << std::string_view{buffer.data(), bytes_received} << '\n';
        co_return;
      }());
  co_return;
}

int main() {
  io_context ctx{};
  auto sched = ctx.get_scheduler();
  inplace_stop_source stopSource;
  std::thread t{[&] {
    ctx.run(stopSource.get_token());
  }};
  scope_guard stopOnExit = [&]() noexcept {
    stopSource.request_stop();
    t.join();
  };
  sync_wait(hello_udp(sched));
  return 0;
}
#else
#  include <cstdio>
int main() {
  printf("neither io_ring or coroutine support found\n");
  return 0;
}
#endif
